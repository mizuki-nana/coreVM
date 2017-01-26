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
#include <cstdio>
#include <unordered_set>


namespace corevm {
namespace ir {

// -----------------------------------------------------------------------------

#define ERROR(fmt, ...)                                 \
  {                                                     \
    char buf[256] = {0};                                \
    snprintf(buf, sizeof(buf), (fmt), __VA_ARGS__);     \
    m_msg.assign(buf);                                  \
    return false;                                       \
  }

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
      ERROR("Duplicate type definition of \"%s\"", type_decl.name.c_str());
    }
  }

  for (const auto& type_decl : m_module.types)
  {
    for (const auto& field : type_decl.fields)
    {
      if (!check_identifier_type(field.type))
      {
        ERROR("Unknown type used for type member %s::%s",
          type_decl.name.c_str(), field.identifier.c_str());
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
      ERROR("Duplicate function definition of \"%s\"", closure.name.c_str());
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
    if (!m_index->has_func_def(parent_name))
    {
      ERROR("Invalid parent of function of \"%s\": \"%s\"",
        closure.name.c_str(), parent_name.c_str());
    }
  }

  // Return type.
  if (!check_identifier_type(closure.rettype))
  {
    ERROR("Invalid return type for function \"%s\"", closure.name.c_str());
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
      ERROR("Duplicate parameter \"%s\" in function \"%s\"",
        parameter.identifier.c_str(), closure.name.c_str());
    }

    if (!check_identifier_type(parameter.type))
    {
      ERROR("Invalid type for parameter \"%s\" of function \"%s\"",
        parameter.identifier.c_str(), closure.name.c_str());
    }
  }

  // Basic blocks.
  std::unordered_set<std::string> bb_set;
  FuncDefCheckContext ctx;
  ctx.closure = &closure;
  ctx.bb = nullptr;
  ctx.func_def_index = &m_index->function_index[closure.name];
  for (const auto& bb : closure.blocks)
  {
    const auto itr = bb_set.find(bb.label);
    if (itr == bb_set.end())
    {
      bb_set.insert(bb.label);
    }
    else
    {
      ERROR("Duplicate basic block \"%s\" in function \"%s\"",
        bb.label.c_str(), closure.name.c_str());
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
          ERROR("Duplicate instruction target \"%s\" in function \"%s\" under block \"%s\"",
            target.c_str(), ctx.closure->name.c_str(), ctx.bb->label.c_str());
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
      ERROR("Invalid type used in instruction \"%s\" in function \"%s\" under block \"%s\"",
        IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
        ctx.bb->label.c_str());
    }
  }

  // Labels.
  if (!instr.labels.is_null())
  {
    const auto& bb_index = ctx.func_def_index->bb_index;
    const auto& labels = instr.labels.get_array();
    for (const auto& label : labels)
    {
      const auto itr = bb_index.find(label.name);
      if (itr == bb_index.end())
      {
        ERROR("Invalid label used in instruction \"%s\" in function \"%s\" under block \"%s\": \"%s\"",
          IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
          ctx.bb->label.c_str(), label.name.c_str());
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
  }

  // Instruction specific check.
  if (!check_instruction_dispatch(instr, ctx))
  {
    return false;
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
    return check_instr_with_UNARY_ARITHMETIC_OPCODE(instr, ctx);
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
    ERROR("Invalid instruction code encountered in function \"%s\" under block \"%s\"",
      ctx.closure->name.c_str(), ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::are_operands_of_same_type(const IROperand& lhs, const IROperand& rhs,
  const FuncDefCheckContext& ctx)
{
  const auto lhs_type = get_operand_type(lhs, ctx);
  const auto rhs_type = get_operand_type(rhs, ctx);
  return operator==(lhs_type, rhs_type);
}

// -----------------------------------------------------------------------------

const IRIdentifierType
Verifier::get_operand_type(const IROperand& oprd,
  const FuncDefCheckContext& ctx)
{
  const auto& function_index = *ctx.func_def_index;
  switch (oprd.type)
  {
  case corevm::constant:
    return create_ir_value_type(oprd.value.get_IRValue().type);
  case corevm::ref:
    {
      const std::string ref_name = oprd.value.get_string();
      const auto itr = function_index.identifier_type_index.find(ref_name);
      assert(itr != function_index.identifier_type_index.end());
      return itr->second;
    }
    break;
  default:
    return create_ir_void_value_type();
  }

  return create_ir_void_value_type();
}

// -----------------------------------------------------------------------------

bool
Verifier::is_operand_integer_type(const IROperand& oprd,
  const FuncDefCheckContext& ctx)
{
  const auto identifier_type = get_operand_type(oprd, ctx);
  return identifier_type.type == IdentifierType_ValueType &&
    is_ir_value_integer_type(identifier_type.value.get_IRValueType());
}

// -----------------------------------------------------------------------------

bool
Verifier::is_operand_numeric_type(const IROperand& oprd,
  const FuncDefCheckContext& ctx)
{
  const auto identifier_type = get_operand_type(oprd, ctx);
  return identifier_type.type == IdentifierType_ValueType &&
    is_ir_value_numeric_type(identifier_type.value.get_IRValueType());
}

// -----------------------------------------------------------------------------

bool
Verifier::is_operand_numeric_or_boolean_type(const IROperand& oprd,
  const FuncDefCheckContext& ctx)
{
  const auto identifier_type = get_operand_type(oprd, ctx);
  return identifier_type.type == IdentifierType_ValueType &&
    is_ir_value_numeric_or_boolean_type(identifier_type.value.get_IRValueType());
}

// -----------------------------------------------------------------------------

bool
Verifier::is_operand_boolean_type(const IROperand& oprd,
  const FuncDefCheckContext& ctx)
{
  const auto identifier_type = get_operand_type(oprd, ctx);
  return identifier_type.type == IdentifierType_ValueType &&
    is_ir_value_boolean_type(identifier_type.value.get_IRValueType());
}

// -----------------------------------------------------------------------------

bool
Verifier::is_operand_array_type(const IROperand& oprd,
  const FuncDefCheckContext& ctx)
{
  const auto identifier_type = get_operand_type(oprd, ctx);
  return identifier_type.type == IdentifierType_Array;
}

// -----------------------------------------------------------------------------

bool
Verifier::is_operand_string_type(const IROperand& oprd, const FuncDefCheckContext& ctx)
{
  const auto identifier_type = get_operand_type(oprd, ctx);
  return identifier_type.type == IdentifierType_ValueType &&
    is_ir_value_string_type(identifier_type.value.get_IRValueType());
}

// -----------------------------------------------------------------------------

bool
Verifier::is_operand_struct_type(const IROperand& oprd, const FuncDefCheckContext& ctx)
{
  const auto identifier_type = get_operand_type(oprd, ctx);
  return identifier_type.type == IdentifierType_Identifier;
}

// -----------------------------------------------------------------------------

bool
Verifier::is_operand_object_type(const IROperand& oprd, const FuncDefCheckContext& ctx)
{
  const auto identifier_type = get_operand_type(oprd, ctx);
  return is_ir_value_object_type(identifier_type.value.get_IRValueType());
}

// -----------------------------------------------------------------------------

bool
Verifier::is_operand_struct_or_object_type(const IROperand& oprd, const FuncDefCheckContext& ctx)
{
  const auto identifier_type = get_operand_type(oprd, ctx);
  return identifier_type.type == IdentifierType_Identifier ||
    is_ir_value_object_type(identifier_type.value.get_IRValueType());
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

      const auto& function_index = *ctx.func_def_index;
      const auto& parameter_index = function_index.parameter_index;
      const auto parameter_itr = parameter_index.find(ref);

      if (itr == ctx.target_set.end() && parameter_itr == parameter_index.end())
      {
        ERROR("Undeclared operand used in instruction \"%s\" in function \"%s\" under block \"%s\": \"%s\"",
          IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
          ctx.bb->label.c_str(), ref.c_str());
      }

      // Check if operand type is compatible with instruction type.
      if (!instr.type.is_null())
      {
        const auto& instr_index = function_index.bb_index.at(ctx.bb->label);
        const auto& oprd_type = get_operand_type(oprd, ctx);

        if (!are_compatible_types(oprd_type, instr.type.get_IRIdentifierType()))
        {
          ERROR("Incompatible operand type in instruction \"%s\" in function \"%s\" under block \"%s\"",
            IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
            ctx.bb->label.c_str());
        }
      }
    }
    break;
  default:
    ERROR("Invalid operand type in instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
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
      ERROR("Invalid type field type: %u", identifier_type.type);
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
Verifier::check_type_attribute(const IROperand& attr_oprd,
  const IROperand& object_oprd, const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (attr_oprd.type == corevm::constant && is_operand_struct_type(object_oprd, ctx))
  {
    const std::string struct_name = get_operand_type(object_oprd, ctx).value.get_string();

    if (!check_type_string(struct_name))
    {
      ERROR("Invalid type for second operand in instruction \"%s\" in function \"%s\" under block \"%s\"",
        IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
        ctx.bb->label.c_str());
    }

    const auto type_decl = m_index->type_index[struct_name];
    const auto attr_name_value = attr_oprd.value.get_IRValue();
    if (!is_ir_value_string_type(attr_name_value.type))
    {
      ERROR("Invalid type for first operand in instruction \"%s\" in function \"%s\" under block \"%s\"",
        IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
        ctx.bb->label.c_str());
    }
    const std::string attr_name = attr_name_value.value.get_string();
    if (!type_decl_has_field(*type_decl, attr_name))
    {
      ERROR("Invalid type accessor constant for first operand in instruction \"%s\" in function \"%s\" under block \"%s\"",
        IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
        ctx.bb->label.c_str());
    }
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_type_attribute(const IROperand& attr_name_oprd,
    const IROperand& attr_value_oprd, const IROperand& object_oprd,
    const IRInstruction& instr, const FuncDefCheckContext& ctx)
{
  if (attr_name_oprd.type == corevm::constant && is_operand_struct_type(object_oprd, ctx))
  {
    const std::string struct_name = get_operand_type(object_oprd, ctx).value.get_string();

    if (!check_type_string(struct_name))
    {
      ERROR("Invalid type for second operand in instruction \"%s\" in function \"%s\" under block \"%s\"",
        IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
        ctx.bb->label.c_str());
    }

    const auto type_decl = m_index->type_index[struct_name];
    const auto attr_name_value = attr_name_oprd.value.get_IRValue();
    if (!is_ir_value_string_type(attr_name_value.type))
    {
      ERROR("Invalid type for first operand in instruction \"%s\" in function \"%s\" under block \"%s\"",
        IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
        ctx.bb->label.c_str());
    }
    const std::string attr_name = attr_name_value.value.get_string();
    if (!type_decl_has_field(*type_decl, attr_name))
    {
      ERROR("Invalid type accessor constant for first operand in instruction \"%s\" in function \"%s\" under block \"%s\"",
        IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
        ctx.bb->label.c_str());
    }

    const auto field_type = get_type_decl_field_type(*type_decl, attr_name);
    if (field_type != get_operand_type(attr_value_oprd, ctx))
    {
      ERROR("Mismatch between source type and target field type in instruction \"%s\" in function \"%s\" under block \"%s\"",
        IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
        ctx.bb->label.c_str());
    }
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instruction_options_count(const IRInstruction& instr,
  size_t count, const FuncDefCheckContext& ctx)
{
  if (instr.options.size() != count)
  {
    ERROR(
      "Instruction \"%s\" in function \"%s\" under block \"%s\" has incorrect number of options",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instruction_operands_count(const IRInstruction& instr,
  size_t count, const FuncDefCheckContext& ctx)
{
  if (instr.oprds.size() != count)
  {
    ERROR("Instruction \"%s\" in function \"%s\" under block \"%s\" has incorrect number of operands",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instruction_labels_count(const IRInstruction& instr,
  size_t count, const FuncDefCheckContext& ctx)
{
  bool res = true;
  if (instr.labels.is_null())
  {
    if (count)
    {
      res = false;
    }
  }
  else
  {
    const auto& labels = instr.labels.get_array();
    if (labels.size() != count)
    {
      res = false;
    }
  }

  if (!res)
  {
    ERROR("Instruction \"%s\" in function \"%s\" under block \"%s\" has incorrect number of labels",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return res;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instruction_has_type(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (instr.type.is_null())
  {
    ERROR("Expected type defined for instruction \"%s\" in function \"%s\" under block \"%s\" has incorrect number of labels",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_ALLOCA(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 1, ctx))
  {
    return false;
  }
  else
  {
    const auto& option = instr.options.front();
    if (option != "auto" && option != "static")
    {
      ERROR("Unrecognized option in instruction \"%s\" in function \"%s\" under block \"%s\": \"%s\"",
        IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
        ctx.bb->label.c_str(), option.c_str());
    }
  }

  if (!check_instruction_operands_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_LOAD(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_operands_count(instr, 1, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_has_type(instr, ctx))
  {
    return false;
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_STORE(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_operands_count(instr, 2, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_has_type(instr, ctx))
  {
    return false;
  }

  if (get_operand_type(instr.oprds[0], ctx) != get_type_of_instr(instr))
  {
    ERROR("Mismatch between instruction and first operand types in instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_GETATTR(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_operands_count(instr, 2, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  // Check first operand is of string type.
  if (!is_operand_string_type(instr.oprds[0], ctx))
  {
    ERROR("Invalid type for first operand in instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  // Check second operand is of type struct or object.
  if (!is_operand_struct_or_object_type(instr.oprds[1], ctx))
  {
    ERROR("Invalid type for second operand in instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  if (!check_type_attribute(instr.oprds[0], instr.oprds[1], instr, ctx))
  {
    return false;
  }
  
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_SETATTR(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_operands_count(instr, 3, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  // Check first operand is of string type.
  if (!is_operand_string_type(instr.oprds[0], ctx))
  {
    ERROR("Invalid type for first operand in instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  // Check third operand is of type struct or object.
  if (!is_operand_struct_or_object_type(instr.oprds[2], ctx))
  {
    ERROR("Invalid type for third operand in instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  if (is_operand_struct_type(instr.oprds[2], ctx))
  {
    if (!check_type_attribute(instr.oprds[0], instr.oprds[1], instr.oprds[2], instr, ctx))
    {
      return false;
    }
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_DELATTR(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_operands_count(instr, 2, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  // Check first operand is of string type.
  if (!is_operand_string_type(instr.oprds[0], ctx))
  {
    ERROR("Invalid type for first operand in instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  // Check second operand is of type object.
  if (!is_operand_object_type(instr.oprds[1], ctx))
  {
    ERROR("Invalid type for second operand in instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_GETELEMENT(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_operands_count(instr, 2, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  // Check first operand is of type array.
  if (!is_operand_array_type(instr.oprds[0], ctx))
  {
    ERROR("Invalid type for first operand in instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  // Check instruction type matches with first operand.
  const auto src_identifier_type = get_operand_type(instr.oprds[0], ctx);
  assert(src_identifier_type.type == IdentifierType_Array);
  const auto src_array_type = src_identifier_type.value.get_IRArrayType();
  if (instr.type.is_null() || instr.type.get_IRIdentifierType() != src_array_type.type)
  {
    ERROR("Incompatble types for first operand in instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  // Check second operand is of integer type.
  if (!is_operand_integer_type(instr.oprds[1], ctx))
  {
    ERROR("Invalid type for second operand in instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_PUTELEMENT(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_operands_count(instr, 3, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  // Check second operand is of array type.
  if (!is_operand_array_type(instr.oprds[1], ctx))
  {
    ERROR("Invalid type for second operand in instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  // Check first operand type matches with nested element type of second operand.
  const auto dst_identifier_type = get_operand_type(instr.oprds[1], ctx);
  assert(dst_identifier_type.type == IdentifierType_Array);
  const auto dst_array_type = dst_identifier_type.value.get_IRArrayType();
  const auto src_identifier_type = get_operand_type(instr.oprds[0], ctx);
  if (src_identifier_type != dst_array_type.type)
  {
    ERROR("Incompatble types for first and second operands in instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  // Check third operand is of integer type.
  if (!is_operand_integer_type(instr.oprds[2], ctx))
  {
    ERROR("Invalid type for third operand operand in instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_LEN(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_operands_count(instr, 1, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  if (!is_operand_array_type(instr.oprds[0], ctx))
  {
    ERROR("Invalid operand type for instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_RET(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_operands_count(instr, 1, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  // Check return type matches with function signature.
  const auto oprd_type = get_operand_type(instr.oprds.front(), ctx);
  if (oprd_type != ctx.closure->rettype)
  {
    ERROR("Invalid return type for instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_BR(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_operands_count(instr, 1, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 2, ctx))
  {
    return false;
  }

  if (!is_operand_boolean_type(instr.oprds[0], ctx))
  {
    ERROR("Invalid operand type for instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_SWITCH2(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (instr.oprds.size() < 2)
  {
    ERROR("Need at least 2 operands for instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  // Check oprds count == (labels count + 1).
  const size_t label_count = instr.labels.is_null() ?
    0 : instr.labels.get_array().size();
  if ( instr.oprds.size() != (label_count + 1) )
  {
    ERROR("Number of operands and labels do not match for instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  // Check if operands have the same type.
  for (size_t i = 1; i < instr.oprds.size(); ++i)
  {
    const auto& first_operand = instr.oprds.front();
    if (!are_operands_of_same_type(first_operand, instr.oprds[i], ctx))
    {
      ERROR("Operands at index %u and %u have incompatible type in instruction \"%s\" in function \"%s\" under block \"%s\"",
        0, i, IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
        ctx.bb->label.c_str());
    }
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_UNARY_ARITHMETIC_OPCODE(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_operands_count(instr, 1, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  if (!is_operand_numeric_type(instr.oprds[0], ctx))
  {
    ERROR("Invalid operand type for instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_BINARY_ARITHMETIC_OPCODE(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_operands_count(instr, 2, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  if (!is_operand_numeric_type(instr.oprds[0], ctx) ||
      !is_operand_numeric_type(instr.oprds[1], ctx))
  {
    ERROR("Invalid operand type for instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_BNOT(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_operands_count(instr, 1, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  if (!is_operand_numeric_type(instr.oprds[0], ctx))
  {
    ERROR("Invalid operand type for instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_BINARY_BITWISE_OPCODE(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_operands_count(instr, 2, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  if (!is_operand_numeric_type(instr.oprds[0], ctx) ||
      !is_operand_numeric_type(instr.oprds[1], ctx))
  {
    ERROR("Invalid operand type for instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_BITSHIFT_OPCODE(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_operands_count(instr, 2, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  if (!is_operand_integer_type(instr.oprds[0], ctx) ||
      !is_operand_integer_type(instr.oprds[1], ctx))
  {
    ERROR("Invalid operand type for instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_EQUALITY_OPCODE(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_operands_count(instr, 2, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  if (!are_operands_of_same_type(instr.oprds[0], instr.oprds[1], ctx))
  {
    ERROR("Incompatible operand types for instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_LNOT(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_operands_count(instr, 1, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  if (!is_operand_numeric_or_boolean_type(instr.oprds[0], ctx))
  {
    ERROR("Invalid operand type for instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_BINARY_LOGICAL_OPCODE(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_operands_count(instr, 2, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  if (! (is_operand_numeric_or_boolean_type(instr.oprds[0], ctx) &&
         is_operand_numeric_or_boolean_type(instr.oprds[1], ctx)) )
  {
    ERROR("Invalid operand type for instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_CMP(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_operands_count(instr, 2, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  if (!are_operands_of_same_type(instr.oprds[0], instr.oprds[1], ctx))
  {
    ERROR("Incompatible operand types for instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_instr_with_OPCODE_CALL(const IRInstruction& instr,
  const FuncDefCheckContext& ctx)
{
  if (!check_instruction_options_count(instr, 0, ctx))
  {
    return false;
  }

  if (!check_instruction_labels_count(instr, 0, ctx))
  {
    return false;
  }

  if (instr.oprds.size() < 1)
  {
    ERROR("Expect at least 1 operand for instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  const auto& first_operand = instr.oprds[0];
  if (!is_operand_string_type(first_operand, ctx))
  {
    ERROR("Invalid type of first operand in instruction \"%s\" in function \"%s\" under block \"%s\"",
      IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
      ctx.bb->label.c_str());
  }

  if (first_operand.type == corevm::constant)
  {
    const auto first_oprd_value = first_operand.value.get_IRValue();
    if (!is_ir_value_string_type(first_oprd_value.type))
    {
      ERROR("Invalid type of first operand in instruction \"%s\" in function \"%s\" under block \"%s\"",
        IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
        ctx.bb->label.c_str());
    }
    const std::string func_name = first_oprd_value.value.get_string();
    const auto itr = m_index->function_index.find(func_name);
    if (itr == m_index->function_index.end())
    {
      ERROR("Invalid callee in instruction \"%s\" in function \"%s\" under block \"%s\"",
        IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
        ctx.bb->label.c_str());
    }

    // Check instruction type matches with function return type.
    const IRClosure* target_closure = itr->second.closure;
    if (get_type_of_instr(instr) != target_closure->rettype)
    {
      ERROR("Invalid type of instruction \"%s\" in function \"%s\" under block \"%s\"",
        IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
        ctx.bb->label.c_str()); 
    }

    // Validate arguments and parameter types match.
    const size_t parameter_count = target_closure->parameters.size();
    if ( instr.oprds.size() != (parameter_count + 1) )
    {
      ERROR("Invalid number of arguments in instruction \"%s\" in function \"%s\" under block \"%s\"",
        IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
        ctx.bb->label.c_str());
    }

    for (size_t i = 0; i < parameter_count; ++i)
    {
      if (get_operand_type(instr.oprds[i + 1], ctx) != target_closure->parameters[i].type)
      {
        ERROR("Invalid argument type in instruction \"%s\" in function \"%s\" under block \"%s\"",
          IROpcode_to_string(instr.opcode), ctx.closure->name.c_str(),
          ctx.bb->label.c_str());
      }
    }
  }

  return true;
}

// -----------------------------------------------------------------------------

} /* end namespace ir */
} /* end namespace corevm */
