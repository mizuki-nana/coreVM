/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2017 Yanzheng Li

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************/
#include "verifier.h"
#include "format.h"
#include "format_util.h"
#include "ir_module_index.h"
#include <cstdio>
#include <unordered_set>


namespace corevm {
namespace ir {

// -----------------------------------------------------------------------------

Verifier::Verifier(const IRModule& module)
  :
  m_module(module),
  m_index(new IRModuleIndex(module)),
  m_msg()
{
}

// -----------------------------------------------------------------------------

bool
Verifier::run(std::string& msg)
{
  if (!check_type_decls() || !check_func_defs())
  {
    msg = std::move(m_msg);
    return false;
  }

  return true;
}

// -----------------------------------------------------------------------------

void
Verifier::get_index(std::unique_ptr<IRModuleIndex>& index)
{
  m_index.swap(index);
}

// -----------------------------------------------------------------------------

bool
Verifier::check_type_decls()
{
  std::unordered_set<std::string> type_name_set;
  for (const auto& type_decl : m_module.types)
  {
    auto itr = type_name_set.find(type_decl.name);
    if (itr == type_name_set.end())
    {
      type_name_set.insert(type_decl.name);
    }
    else
    {
      char buf[256] = {0};
      snprintf(buf, sizeof(buf),
        "Duplicate type definition of \"%s\"", type_decl.name.c_str());
      m_msg.assign(buf);
      return false;
    }
  }

  for (const auto& type_decl : m_module.types)
  {
    for (const auto& field : type_decl.fields)
    {
      if (!check_identifier_type(field.type))
      {
        char buf[256] = {0};
        snprintf(buf, sizeof(buf),
          "Unknown type used for type member %s::%s",
          type_decl.name.c_str(), field.identifier.c_str());
        m_msg.assign(buf);
        return false;
      }
    }
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_func_defs()
{
  std::unordered_set<std::string> closure_names;
  for (const auto& closure : m_module.closures)
  {
    const auto itr = closure_names.find(closure.name);
    if (itr == closure_names.cend())
    {
      closure_names.insert(closure.name);
    }
    else
    {
      char buf[256] = {0};
      snprintf(buf, sizeof(buf),
        "Duplicate function definition of \"%s\"", closure.name.c_str());
      m_msg.assign(buf);
      return false;
    }
  }

  for (const auto& closure : m_module.closures)
  {
    if (!check_func_def(closure))
    {
      return false;
    }
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_func_def(const IRClosure& closure)
{
  // Check parent.
  if (!closure.parent.is_null())
  {
    const std::string parent_name = closure.parent.get_string();
    if (m_index->function_index.find(parent_name) == m_index->function_index.cend())
    {
      char buf[256] = {0};
      snprintf(buf, sizeof(buf),
        "Invalid parent of function of \"%s\": \"%s\"",
        closure.name.c_str(), parent_name.c_str());
      m_msg.assign(buf);
      return false;
    }
  }

  // Return type.
  if (!check_identifier_type(closure.rettype))
  {
    char buf[256] = {0};
    snprintf(buf, sizeof(buf),
      "Invalid return type for function \"%s\"", closure.name.c_str());
    m_msg.assign(buf);
    return false;
  }

  // Parameters.
  std::unordered_set<std::string> parameters_set;
  for (const auto& parameter : closure.parameters)
  {
    const auto itr = parameters_set.find(parameter.identifier);
    if (itr == parameters_set.end())
    {
      parameters_set.insert(parameter.identifier);
    }
    else
    {
      char buf[256] = {0};
      snprintf(buf, sizeof(buf),
        "Duplicate parameter \"%s\" in function \"%s\"",
        parameter.identifier.c_str(), closure.name.c_str());
      m_msg.assign(buf);
      return false;
    }

    if (!check_identifier_type(parameter.type))
    {
      char buf[256] = {0};
      snprintf(buf, sizeof(buf),
        "Invalid type for parameter \"%s\" of function \"%s\"",
        parameter.identifier.c_str(), closure.name.c_str());
      m_msg.assign(buf);
      return false;
    }
  }

  // Basic blocks.
  std::unordered_set<std::string> bb_set;
  FuncDefCheckContext ctx;
  ctx.closure = &closure;
  ctx.bb = nullptr;
  for (const auto& bb : closure.blocks)
  {
    const auto itr = bb_set.find(bb.label);
    if (itr == bb_set.end())
    {
      bb_set.insert(bb.label);
    }
    else
    {
      char buf[256] = {0};
      snprintf(buf, sizeof(buf),
        "Duplicate basic block \"%s\" in function \"%s\"",
        bb.label.c_str(), closure.name.c_str());
      m_msg.assign(buf);
      return false;
    }

    ctx.bb = &bb;
    if (!check_basic_block(bb, ctx))
    {
      return false;
    } 
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_basic_block(const IRBasicBlock& bb, FuncDefCheckContext& ctx)
{
  for (const auto& instr : bb.body)
  {
    if (!instr.target.is_null())
    {
      const std::string target = instr.target.get_string();
      const auto itr = ctx.target_set.find(target);
      {
        if (itr == ctx.target_set.end())
        {
          ctx.target_set.insert(target);
        }
        else
        {
          char buf[256] = {0};
          snprintf(buf, sizeof(buf),
            "Duplicate instruction target \"%s\" in function \"%s\"",
            target.c_str(), ctx.closure->name.c_str());
          m_msg.assign(buf);
          return false;
        }
      }
    }

    if (!check_instruction(instr, ctx))
    {
      return false;
    }
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instruction(const IRInstruction& instr, FuncDefCheckContext& ctx)
{
  // Target type.
  if (!instr.type.is_null())
  {
    if (!check_identifier_type(instr.type.get_IRIdentifierType()))
    {
      char buf[256] = {0};
      snprintf(buf, sizeof(buf),
        "Invalid type used in instruction \"%s\"",
        IROpcode_to_string(instr.opcode));
      m_msg.assign(buf);
      return false;
    }
  }

  // Labels.
  if (!instr.labels.is_null())
  {
    const auto& labels = instr.labels.get_array();
    for (const auto& label : labels)
    {
      const auto& bb_index = m_index->function_index[ctx.closure->name].bb_index;
      const auto itr = bb_index.find(label.name);
      if (itr == bb_index.end())
      {
        char buf[256] = {0};
        snprintf(buf, sizeof(buf),
          "Invalid label used in instruction \"%s\" in function \"%s\": \"%s\"",
          IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(), label.name.c_str());
        m_msg.assign(buf);
        return false;
      }
    }
  }

  // Operands.
  for (const auto& oprd : instr.oprds)
  {
    if (!check_operand(oprd, instr, ctx))
    {
      return false;
    }

    if (!check_instruction_dispatch(instr, ctx))
    {
      return false;
    }
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instruction_dispatch(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  switch (instr.opcode)
  {
  case corevm::alloca:
    return check_instr_with_OPCODE_ALLOCA(instr, ctx);
  case corevm::load:
    return check_instr_with_OPCODE_LOAD(instr, ctx);
  case corevm::store:
    return check_instr_with_OPCODE_STORE(instr, ctx);
  case corevm::getattr:
    return check_instr_with_OPCODE_GETATTR(instr, ctx);
  case corevm::setattr:
    return check_instr_with_OPCODE_SETATTR(instr, ctx);
  case corevm::delattr:
    return check_instr_with_OPCODE_DELATTR(instr, ctx);
  case corevm::getelement:
    return check_instr_with_OPCODE_GETELEMENT(instr, ctx);
  case corevm::putelement:
    return check_instr_with_OPCODE_PUTELEMENT(instr, ctx);
  case corevm::len:
    return check_instr_with_OPCODE_LEN(instr, ctx);
  case corevm::ret:
    return check_instr_with_OPCODE_RET(instr, ctx);
  case corevm::br:
    return check_instr_with_OPCODE_BR(instr, ctx);
  case switch2:
    return check_instr_with_OPCODE_SWITCH2(instr, ctx);
  case corevm::pos:
  case corevm::neg:
  case corevm::inc:
  case corevm::dec:
    return check_instr_with_UNARY_ARITHMETOC_OPCODE(instr, ctx);
  case corevm::add:
  case corevm::sub:
  case corevm::mul:
  case corevm::div:
  case corevm::mod:
    return check_instr_with_BINARY_ARITHMETIC_OPCODE(instr, ctx);
  case corevm::bnot:
    return check_instr_with_OPCODE_BNOT(instr, ctx);
  case corevm::band:
  case corevm::bor:
  case corevm::bxor:
    return check_instr_with_BINARY_BITWISE_OPCODE(instr, ctx);
  case corevm::bls:
  case corevm::brs:
    return check_instr_with_BITSHIFT_OPCODE(instr, ctx);
  case corevm::eq:
  case corevm::neq:
  case corevm::gt:
  case corevm::lt:
  case corevm::gte:
  case corevm::lte:
    return check_instr_with_EQUALITY_OPCODE(instr, ctx);
  case corevm::lnot:
    return check_instr_with_OPCODE_LNOT(instr, ctx);
  case corevm::land:
  case corevm::lor:
    return check_instr_with_BINARY_LOGICAL_OPCODE(instr, ctx);
  case corevm::cmp:
    return check_instr_with_OPCODE_CMP(instr, ctx);
  case corevm::call:
    return check_instr_with_OPCODE_CALL(instr, ctx);
  default:
    char buf[256] = {0};
    snprintf(buf, sizeof(buf),
      "Invalid instruction code encountered");
    m_msg.assign(buf);
    return false;
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_operand(const IROperand& oprd, const IRInstruction& instr,
  FuncDefCheckContext& ctx)
{
  switch (oprd.type)
  {
  case corevm::constant:
    // No need to do anything here actually.
    break;
  case corevm::ref:
    {
      const std::string ref = oprd.value.get_string();
      const auto itr = ctx.target_set.find(ref);
      if (itr == ctx.target_set.end())
      {
        char buf[256] = {0};
        snprintf(buf, sizeof(buf),
          "Undeclared operand used in instruction \"%s\" in function \"%s\": \"%s\"",
          IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(), ref.c_str());
        m_msg.assign(buf);
        return false;
      }
    }
    break;
  default:
    char buf[256] = {0};
    snprintf(buf, sizeof(buf),
      "Invalid operand type in instruction \"%s\"",
      IROpcode_to_string(instr.opcode));
    m_msg.assign(buf);
    return false;
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_identifier_type(const IRIdentifierType& identifier_type)
{
  switch (identifier_type.type)
  {
  case IdentifierType_Identifier:
    {
      const std::string type_name = identifier_type.value.get_string();
      if (!check_type_string(type_name))
      {
        return false;
      }
    }
    break;
  case IdentifierType_Array:
    {
      const IRArrayType array_type = identifier_type.value.get_IRArrayType();
      if (!check_identifier_type(array_type.type))
      {
        return false;
      }
    }
    break;
  case IdentifierType_ValueType:
    break;
  default:
    {
      char buf[256] = {0};
      snprintf(buf, sizeof(buf), "Invalid type field type: %u", identifier_type.type);
      m_msg.assign(buf);
      return false;
    }
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_type_string(const std::string& type_name)
{
  return m_index->type_index.find(type_name) != m_index->type_index.end();
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_ALLOCA(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_LOAD(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_STORE(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_GETATTR(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_SETATTR(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_DELATTR(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_GETELEMENT(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_PUTELEMENT(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_LEN(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_RET(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_BR(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_SWITCH2(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_UNARY_ARITHMETOC_OPCODE(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_BINARY_ARITHMETIC_OPCODE(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_BNOT(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_BINARY_BITWISE_OPCODE(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_BITSHIFT_OPCODE(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_EQUALITY_OPCODE(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_LNOT(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_BINARY_LOGICAL_OPCODE(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_CMP(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_CALL(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  return true;
}

// -----------------------------------------------------------------------------

} /* end namespace ir */
} /* end namespace corevm */
