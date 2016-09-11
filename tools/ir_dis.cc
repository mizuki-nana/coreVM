/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Yanzheng Li

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
#include "ir/reader.h"
#include "ir/format.h"

#include <sneaker/utility/cmdline_program.h>

#include <cstdio>
#include <string>

#include <cassert>
#include <fstream>
#include <iostream>


// -----------------------------------------------------------------------------

namespace {

const char*
ir_value_type_to_string(corevm::IRValueType value)
{
  switch (value)
  {
  case corevm::IRValueType::voidtype:
    return "void";
  case corevm::IRValueType::boolean:
    return "bool";
  case corevm::IRValueType::i8:
    return "i8";
  case corevm::IRValueType::ui8:
    return "ui8";
  case corevm::IRValueType::i16:
    return "i16";
  case corevm::IRValueType::ui16:
    return "ui16";
  case corevm::IRValueType::i32:
    return "i32";
  case corevm::IRValueType::ui32:
    return "ui32";
  case corevm::IRValueType::i64:
    return "i64";
  case corevm::IRValueType::ui64:
    return "ui64";
  case corevm::IRValueType::spf:
    return "spf";
  case corevm::IRValueType::dpf:
    return "dpf";
  case corevm::IRValueType::string:
    return "string";
  case corevm::IRValueType::array:
    return "array";
  case corevm::IRValueType::structtype:
    return "structtype";
  case corevm::IRValueType::ptrtype:
    return "ptr";
  }
}

const char*
ir_value_ref_type_to_string(corevm::IRValueRefType val)
{
  switch (val)
  {
  case corevm::IRValueRefType::pointer:
    return "*";
  case corevm::IRValueRefType::value:
    break;
  }

  return "";
}

const char*
ir_opcode_to_string(corevm::IROpcode val)
{
  switch (val)
  {
  case corevm::IROpcode::alloca:
      return "alloca";
  case corevm::IROpcode::load:
      return "load";
  case corevm::IROpcode::store:
      return "store";
  case corevm::IROpcode::getattr:
      return "getattr";
  case corevm::IROpcode::setattr:
      return "setattr";
  case corevm::IROpcode::delattr:
      return "delattr";
  case corevm::IROpcode::getelement:
      return "getelement";
  case corevm::IROpcode::putelement:
      return "putelement";
  case corevm::IROpcode::len:
      return "len";
  case corevm::IROpcode::ret:
      return "ret";
  case corevm::IROpcode::br:
      return "br";
  case corevm::IROpcode::switch2:
      return "switch2";
  case corevm::IROpcode::pos:
      return "pos";
  case corevm::IROpcode::neg:
      return "neg";
  case corevm::IROpcode::inc:
      return "inc";
  case corevm::IROpcode::dec:
      return "dec";
  case corevm::IROpcode::add:
      return "add";
  case corevm::IROpcode::sub:
      return "sub";
  case corevm::IROpcode::mul:
      return "mul";
  case corevm::IROpcode::div:
      return "div";
  case corevm::IROpcode::mod:
      return "mod";
  case corevm::IROpcode::bnot:
      return "bnot";
  case corevm::IROpcode::band:
      return "band";
  case corevm::IROpcode::bor:
      return "bor";
  case corevm::IROpcode::bxor:
      return "bxor";
  case corevm::IROpcode::bls:
      return "bls";
  case corevm::IROpcode::brs:
      return "brs";
  case corevm::IROpcode::eq:
      return "eq";
  case corevm::IROpcode::neq:
      return "neq";
  case corevm::IROpcode::gt:
      return "gt";
  case corevm::IROpcode::lt:
      return "lt";
  case corevm::IROpcode::gte:
      return "gte";
  case corevm::IROpcode::lte:
      return "lte";
  case corevm::IROpcode::lnot:
      return "lnot";
  case corevm::IROpcode::land:
      return "land";
  case corevm::IROpcode::lor:
    return "lor";
  case corevm::IROpcode::cmp:
    return "cmp";
  case corevm::IROpcode::call:
    return "call";
  }

  assert(0);
  return "";
}

} /* end anonymous namespace */

// -----------------------------------------------------------------------------

class IRDisassembler : public sneaker::utility::cmdline_program
{
public:
  IRDisassembler();

  virtual ~IRDisassembler() {}

protected:
  virtual int do_run();

  virtual bool check_parameters() const;

private:
  void disassemble(const corevm::IRModule&, std::ostream&) const;

  void disassemble(const corevm::IRModuleMeta&, std::ostream&) const;

  void disassemble(const corevm::IRStructDecl&, std::ostream&) const;

  void disassemble(const corevm::IRStructField&, std::ostream&) const;

  void disassemble(const corevm::IRClosure&, std::ostream&) const;

  void disassemble(const corevm::IRParameter&, std::ostream&) const;

  void disassemble(const corevm::IRBasicBlock&, std::ostream&) const;

  void disassemble(const corevm::IRInstruction&, std::ostream&) const;

  void disassemble(const corevm::IRValue&, std::ostream&) const;

  void disassemble(const corevm::IROperand&, std::ostream&) const;

  void disassemble(const corevm::IRLabel&, std::ostream&) const;

  std::string m_input;
  std::string m_output;
};

// -----------------------------------------------------------------------------

IRDisassembler::IRDisassembler()
  :
  sneaker::utility::cmdline_program("coreVM IR disassembler"),
  m_input(),
  m_output()
{
  add_positional_parameter("input", 1);
  add_string_parameter("input", "input file", &m_input);
  add_string_parameter("output", "output file", &m_output);
}

// -----------------------------------------------------------------------------

bool
IRDisassembler::check_parameters() const
{
  return !m_input.empty();
}

// -----------------------------------------------------------------------------

int
IRDisassembler::do_run()
{
  corevm::IRModule module;
  std::string err;

  const bool res = corevm::ir::read_module_from_file(
    m_input.c_str(), module, err);

  if (!res)
  {
    printf("Error while reading from %s :\n", m_input.c_str());
    printf("%s\n", err.c_str());
    return -1;
  }

  if (m_output.empty())
  {
    disassemble(module, std::cout);
  }
  else
  {
    std::ofstream stream(m_output.c_str(), std::ofstream::out);
    if (!stream.good())
    {
      printf("Error writing file to : %s\n", m_output.c_str());
      return -1;
    }
    else
    {
      disassemble(module, stream);
    }
  }

  return 0;
}

// -----------------------------------------------------------------------------

void
IRDisassembler::disassemble(const corevm::IRModule& module,
  std::ostream& stream) const
{
  disassemble(module.meta, stream);

  for (const auto& val : module.types)
  {
    disassemble(val, stream);
  }

  for (const auto& val : module.closures)
  {
    disassemble(val, stream);
  }
}

// -----------------------------------------------------------------------------

void
IRDisassembler::disassemble(const corevm::IRModuleMeta& meta,
  std::ostream& stream) const
{
  stream << "module name : " << meta.name << std::endl;
  stream << "format version : " << meta.format_version << std::endl;
  stream << "target version : " << meta.target_version << std::endl;
  stream << "path : " << meta.path << std::endl;
  stream << "author : " << meta.author << std::endl;
  stream << "timestamp : " << meta.timestamp << std::endl;
  stream << std::endl;
}

// -----------------------------------------------------------------------------

void
IRDisassembler::disassemble(const corevm::IRStructDecl& decl,
  std::ostream& stream) const
{
  stream << "structure " << decl.name << " {" << std::endl;
  for (const auto& field : decl.fields)
  {
    stream << '\t';
    disassemble(field, stream);
    stream << ir_value_ref_type_to_string(field.ref_type)
      << " " << field.identifier << std::endl;
  }
  stream << "}" << std::endl;
  stream << std::endl;
}

// -----------------------------------------------------------------------------

void
IRDisassembler::disassemble(const corevm::IRStructField& field,
  std::ostream& stream) const
{
  switch (field.type.idx())
  {
  case 0:
    {
      stream << field.type.get_string();
    }
    break;
  case 1:
    {
      stream << ir_value_type_to_string(field.type.get_IRValueType());
    }
    break;
  default:
    break;
  }
}

// -----------------------------------------------------------------------------

void
IRDisassembler::disassemble(const corevm::IRClosure& closure,
  std::ostream& stream) const
{
  stream << "def " << ir_value_type_to_string(closure.rettype)
    << ir_value_ref_type_to_string(closure.ret_reftype)
    << " " << closure.name << "(";

  size_t len = closure.parameters.size();
  for (const auto& parameter : closure.parameters)
  {
    disassemble(parameter, stream);
    if (len-- > 1)
    {
      stream << ", ";
    }
  }
  stream << ")";

  if (!closure.parent.is_null())
  {
    stream <<  " :" << closure.parent.get_string();
  }

  stream << " {" << std::endl;

  for (const auto& block : closure.blocks)
  {
    disassemble(block, stream);
  }
  stream << "}" << std::endl;
  stream << std::endl;
}

// -----------------------------------------------------------------------------

void
IRDisassembler::disassemble(const corevm::IRParameter& parameter,
  std::ostream& stream) const
{
  stream << ir_value_type_to_string(parameter.type)
    << ir_value_ref_type_to_string(parameter.ref_type)
    << " " << parameter.identifier;
}

// -----------------------------------------------------------------------------

void
IRDisassembler::disassemble(const corevm::IRBasicBlock& block,
  std::ostream& stream) const
{
  stream << block.label << ":" << std::endl;
  for (const auto& instr : block.body)
  {
    stream << '\t';
    disassemble(instr, stream);
  }
}

// -----------------------------------------------------------------------------

void
IRDisassembler::disassemble(const corevm::IRInstruction& instr,
  std::ostream& stream) const
{
  if (!instr.target.is_null())
  {
    stream << "%" << instr.target.get_string() << " = ";
  }
  stream << ir_opcode_to_string(instr.opcode) << " ";
  disassemble(instr.opcodeval, stream);

  if (!instr.oprds.empty())
  {
    stream << ",";
  }
  stream << " ";

  for (size_t i = 0; i < instr.oprds.size(); ++i)
  {
    disassemble(instr.oprds[i], stream);
    stream << " ";
  }

  if (!instr.labels.is_null())
  {
    const auto& labels = instr.labels.get_array();
    if (!labels.empty())
    {
      size_t len = labels.size();
      stream << " [ ";
      for (const auto& label : labels)
      {
        disassemble(label, stream);
        if (len-- > 1)
        {
          stream << ", ";
        }
      }
      stream << " ]";
    }
  }
  stream << std::endl;
}

// -----------------------------------------------------------------------------

void
IRDisassembler::disassemble(const corevm::IRValue& val,
  std::ostream& stream) const
{
  stream << ir_value_type_to_string(val.type) << " ";

  switch (val.value.idx())
  {
  case 0:
    // do nothing.
    stream << val.value.get_bool();
    break;
  case 1:
    stream << val.value.get_bool();
    break;
  case 2:
    stream << val.value.get_int();
    break;
  case 3:
    stream << val.value.get_long();
    break;
  case 4:
    stream << val.value.get_float();
    break;
  case 5:
    stream << val.value.get_double();
    break;
  case 6:
    stream << '"' << val.value.get_string() << '"';
    break;
  case 7:
    const auto& array_val = val.value.get_IRArrayValue();
    stream << "[ " << array_val.len << " * "
      << ir_value_type_to_string(array_val.type) << " ]";
    break;
  }
}

// -----------------------------------------------------------------------------

void
IRDisassembler::disassemble(const corevm::IROperand& oprd,
  std::ostream& stream) const
{
  if (oprd.type == corevm::IROperandType::constant)
  {
    disassemble(oprd.value.get_IRValue(), stream);
  }
  else
  {
    stream << "%" << oprd.value.get_string();
  }
}

// -----------------------------------------------------------------------------

void
IRDisassembler::disassemble(const corevm::IRLabel& label,
  std::ostream& stream) const
{
  stream << "label #" << label.name;
}

// -----------------------------------------------------------------------------

int main(int argc, char** argv)
{
  IRDisassembler program;
  return program.run(argc, argv);
}

// -----------------------------------------------------------------------------

