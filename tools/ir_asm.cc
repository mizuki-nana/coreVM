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
#include "ir/format.h"

#include <avro/Encoder.hh>
#include <avro/Decoder.hh>
#include <avro/ValidSchema.hh>
#include <avro/Compiler.hh>
#include <avro/DataFile.hh>

#include <sneaker/utility/cmdline_program.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>


// -----------------------------------------------------------------------------

namespace {

// -----------------------------------------------------------------------------

static const std::vector<const char*> TYPE_KEYWORDS {
  "void",
  "bool",
  "i8",
  "ui8",
  "i16",
  "ui16",
  "i32",
  "ui32",
  "i64",
  "ui64",
  "spf",
  "dpf",
  "string",
  "array",
  "structtype",
  "ptr"
};

// -----------------------------------------------------------------------------

static const std::vector<const char*> OPCODES {
  "alloca",
  "load",
  "store",
  "getattr",
  "setattr",
  "delattr",
  "getelement",
  "putelement",
  "len",
  "ret",
  "br",
  "switch2",
  "pos",
  "neg",
  "inc",
  "dec",
  "add",
  "sub",
  "mul",
  "div",
  "mod",
  "bnot",
  "band",
  "bor",
  "bxor",
  "bls",
  "brs",
  "eq",
  "neq",
  "gt",
  "lt",
  "gte",
  "lte",
  "lnot",
  "land",
  "lor",
  "cmp",
  "call"
};

// -----------------------------------------------------------------------------

corevm::IRValueType
str_to_ir_value_type(const std::string& str)
{
  for (size_t i = 0; i < TYPE_KEYWORDS.size(); ++i)
  {
    if (str == TYPE_KEYWORDS[i])
    {
      return static_cast<corevm::IRValueType>(i);
    }
  }

  assert(0);
  return corevm::IRValueType::voidtype;
}

// -----------------------------------------------------------------------------

corevm::IROpcode
str_to_ir_opcode(const std::string& str)
{
  for (size_t i = 0; i < OPCODES.size(); ++i)
  {
    if (str == OPCODES[i])
    {
      return static_cast<corevm::IROpcode>(i);
    }
  }
  assert(0);
  return static_cast<corevm::IROpcode>(0);
}

// -----------------------------------------------------------------------------

std::string
extract_metadata_value(const std::string& line, const char* key)
{
  std::string value;
  if (line.find(key) == 0)
  {
    value = line.substr(strlen(key) + 1);
  }

  return value;
}

// -----------------------------------------------------------------------------

} /* end anonymous namespace */

// -----------------------------------------------------------------------------

class IRAssembler : public sneaker::utility::cmdline_program
{
public:
  IRAssembler();

  virtual ~IRAssembler() = default;

protected:
  virtual int do_run();

  virtual bool check_parameters() const;

private:
  struct Lexer
  {
    enum Token
    {
      tok_eof = -1,
      tok_structure = -2,
      tok_def = -3,
      tok_label = -4,
      tok_type = -5,
      tok_opcode = -6,
      tok_identifier = -7,
      tok_number = -8
    };

    Lexer(std::istream&);
    int gettok();

    std::string IdentifierStr;
    double NumVal;
    int LastChar;
    std::istream& m_stream;
  };

  struct Parser
  {
  public:
    Parser(std::istream&, bool debug);

    bool parse(corevm::IRModule&);

  private:
    int getNextToken();
    bool parse_module_metadata(corevm::IRModule&);
    bool parse_module_body(corevm::IRModule&);
    bool parse_structure(corevm::IRModule&);
    bool parse_closure(corevm::IRModule&);
    bool parse_closure_parameter(corevm::IRParameter&);
    bool parse_basic_block(corevm::IRBasicBlock&);
    bool parse_instruction(corevm::IRInstruction&);
    bool parse_ir_value(corevm::IRValue&);
    bool parse_operand(corevm::IROperand&);
    bool parse_label(corevm::IRLabel&);
    void error(const char*) const;

    Lexer lexer;
    int CurTok;
    bool m_debug;
  };

  std::string m_input;
  std::string m_output;
  bool m_debug;
};

// -----------------------------------------------------------------------------

IRAssembler::IRAssembler()
  :
  sneaker::utility::cmdline_program("coreVM IR assembler"),
  m_input(),
  m_output(),
  m_debug(false)
{
  add_positional_parameter("input", 1);
  add_string_parameter("input", "input file", &m_input);
  add_string_parameter("output", "output file", &m_output);
  add_boolean_parameter("debug", "debug mode", &m_debug);
}

// -----------------------------------------------------------------------------

bool
IRAssembler::check_parameters() const
{
  return !m_input.empty() && !m_output.empty();
}

// -----------------------------------------------------------------------------

int
IRAssembler::do_run()
{
  if (m_debug)
  {
    printf("Begin parsing module at %s\n\n", m_input.c_str());
  }

  std::ifstream stream(m_input.c_str());
  if (!stream.good())
  {
    fprintf(stderr, "Error openning input file : %s\n", m_input.c_str());
    return -1;
  }

  corevm::IRModule module;

  Parser parser(stream, m_debug);
  if (!parser.parse(module))
  {
    return -1;
  }

  if (m_debug)
  {
    printf("\nFinished parsing IR module\n");
  }

  static const char* SCHEMA_FILEPATH = "schemas/corevm_ir_schema.json";

  std::ifstream schema_ifs(SCHEMA_FILEPATH);
  avro::ValidSchema schema;
  avro::compileJsonSchema(schema_ifs, schema);

  avro::DataFileWriter<corevm::IRModule> writer(m_output.c_str(), schema);
  writer.write(module);

  writer.close();
  schema_ifs.close();
  stream.close();

  if (m_debug)
  {
    printf("\nSuccessfully written IR module to %s\n", m_output.c_str());
  }

  return 0;
}

// -----------------------------------------------------------------------------

IRAssembler::Lexer::Lexer(std::istream& stream)
  :
  IdentifierStr(),
  NumVal(),
  LastChar(' '),
  m_stream(stream)
{
}

// -----------------------------------------------------------------------------

int
IRAssembler::Lexer::gettok()
{
  // Skip any whitespace.
  while (isspace(LastChar))
  {
    LastChar = m_stream.get();
  }

  // identifier: [a-zA-Z][a-zA-Z0-9]*
  if (isalpha(LastChar) || LastChar == '_')
  {
    // Reset the string.
    IdentifierStr = std::string(1, LastChar);

    LastChar = m_stream.get();
    while (isalnum(LastChar) || LastChar == '_')
    {
      IdentifierStr += static_cast<char>(LastChar);
      LastChar = m_stream.get();
    }

    if (IdentifierStr == "structure")
    {
      return Lexer::tok_structure;
    }
    else if (IdentifierStr == "def")
    {
      return Lexer::tok_def;
    }
    else if (IdentifierStr == "label")
    {
      return Lexer::tok_label;
    }
    else if (std::find(TYPE_KEYWORDS.begin(), TYPE_KEYWORDS.end(),
                       IdentifierStr) != TYPE_KEYWORDS.end())
    {
      return Lexer::tok_type;
    }
    else if (std::find(OPCODES.begin(), OPCODES.end(),
                       IdentifierStr) != OPCODES.end())
    {
      return Lexer::tok_opcode;
    }

    return Lexer::tok_identifier;
  }

  // Number: [0-9.]+
  if (isdigit(LastChar) || LastChar == '.')
  {
    std::string NumStr;
    do {
      NumStr += static_cast<char>(LastChar);
      LastChar = m_stream.get();
    } while (isdigit(LastChar) || LastChar == '.');

    NumVal = strtod(NumStr.c_str(), nullptr);
    return Lexer::tok_number;
  }

  // Check for end of file.  Don't eat the EOF.
  if (LastChar == EOF)
  {
    return Lexer::tok_eof;
  }

  // Otherwise, just return the character as its ascii value.
  int ThisChar = LastChar;
  LastChar = m_stream.get();
  return ThisChar;
}

// -----------------------------------------------------------------------------

IRAssembler::Parser::Parser(std::istream& stream, bool debug)
  :
  lexer(stream),
  CurTok(' '),
  m_debug(debug)
{
}

// -----------------------------------------------------------------------------

bool
IRAssembler::Parser::parse(corevm::IRModule& module)
{
  return parse_module_metadata(module) &&
    parse_module_body(module);
}

// -----------------------------------------------------------------------------

int
IRAssembler::Parser::getNextToken()
{
  return CurTok = lexer.gettok();
}

// -----------------------------------------------------------------------------

bool
IRAssembler::Parser::parse_module_metadata(corevm::IRModule& module)
{
  for (std::string line; std::getline(lexer.m_stream, line); )
  {
    if (line.empty())
    {
      break;
    }

    std::string value;
    if (!(value = extract_metadata_value(line, "module name :")).empty())
    {
      module.meta.name = value;
      if (m_debug)
      {
        printf("Module name : %s\n", module.meta.name.c_str());
      }
    }
    else if (!(value = extract_metadata_value(line, "format version :")).empty())
    {
      module.meta.format_version = value;
      if (m_debug)
      {
        printf("Module format version : %s\n",
          module.meta.format_version.c_str());
      }
    }
    else if (!(value = extract_metadata_value(line, "target version :")).empty())
    {
      module.meta.target_version = value;
      if (m_debug)
      {
        printf("Module target version : %s\n",
          module.meta.target_version.c_str());
      }
    }
    else if (!(value = extract_metadata_value(line, "path :")).empty())
    {
      module.meta.path = value;
      if (m_debug)
      {
        printf("Module path : %s\n", module.meta.path.c_str());
      }
    }
    else if (!(value = extract_metadata_value(line, "author :")).empty())
    {
      module.meta.author = value;
      if (m_debug)
      {
        printf("Module author : %s\n", module.meta.author.c_str());
      }
    }
    else if (line.find("timestamp : ") != std::string::npos)
    {
      int64_t timestamp;
#ifdef __linux__
      if (sscanf(line.c_str(), "timestamp : %ld\n", &timestamp))
#else
      if (sscanf(line.c_str(), "timestamp : %lld\n", &timestamp))
#endif
      {
        module.meta.timestamp = timestamp;
        if (m_debug)
        {
#ifdef __linux__
          printf("Module timestamp: %ld\n", module.meta.timestamp);
#else
          printf("Module timestamp: %lld\n", module.meta.timestamp);
#endif
        }
      }
    }
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
IRAssembler::Parser::parse_module_body(corevm::IRModule& module)
{
  getNextToken();
  while (true)
  {
    switch (CurTok)
    {
    case Lexer::tok_eof:
      return true;
    case Lexer::tok_def:
      if (!parse_closure(module))
      {
        return false;
      }
      break;
    case Lexer::tok_structure:
      if (!parse_structure(module))
      {
        return false;
      }
      getNextToken();
      break;
    default:
      getNextToken();
      break;
    }
  }
}

// -----------------------------------------------------------------------------

bool
IRAssembler::Parser::parse_structure(corevm::IRModule& module)
{
  if (m_debug)
  {
    printf("\n-- Start parsing structure definition --\n");
  }

  corevm::IRStructDecl decl;

  if (getNextToken() == Lexer::tok_identifier)
  {
    decl.name = lexer.IdentifierStr;
  }
  else
  {
    error("Expected structure name");
    return false;
  }

  getNextToken();

  if (CurTok == '{')
  {
    getNextToken();
  }
  else
  {
    error("Expected { in structure definition");
    return false;
  }

  while (CurTok != '}')
  {
    corevm::IRStructField field;

    if (CurTok == Lexer::tok_identifier)
    {
      field.type.set_string(lexer.IdentifierStr);
    }
    else if (CurTok == Lexer::tok_type)
    {
      field.type.set_IRValueType(str_to_ir_value_type(lexer.IdentifierStr));
    }
    else
    {
      error("Expected field type in structure definition");
      return false;
    }

    getNextToken();
    if (CurTok == '*')
    {
      field.ref_type = corevm::IRValueRefType::pointer;
      getNextToken();
    }
    else
    {
      field.ref_type = corevm::IRValueRefType::value;
    }

    if (CurTok == Lexer::tok_identifier)
    {
      field.identifier = lexer.IdentifierStr;
    }
    else
    {
      error("Expected struct field identifier");
      return false;
    }

    getNextToken();

    decl.fields.push_back(field);

    if (m_debug)
    {
      printf("Finished one struct field : %s\n", field.identifier.c_str());
    }
  }
  
  module.types.push_back(decl);

  if (m_debug)
  {
    printf("-- End parsing structure definition : %s --\n", decl.name.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
IRAssembler::Parser::parse_closure(corevm::IRModule& module)
{
  if (m_debug)
  {
    printf("\n-- Begin parsing closure --\n");
  }

  corevm::IRClosure closure;

  if (getNextToken() == Lexer::tok_type)
  {
    closure.rettype = str_to_ir_value_type(lexer.IdentifierStr);
  }
  else
  {
    error("Expected closure return type");
    return false;
  }

  if (getNextToken() == '*')
  {
    closure.ret_reftype = corevm::IRValueRefType::pointer;
    getNextToken();
  }
  else
  {
    closure.ret_reftype = corevm::IRValueRefType::value;
  }

  if (CurTok == Lexer::tok_identifier)
  {
    closure.name = lexer.IdentifierStr;
    if (m_debug)
    {
      printf("Closure name: %s\n", closure.name.c_str());
    }
  }
  else
  {
    error("Expected closure name");
    return false;
  }

  if (getNextToken() != '(')
  {
    error("Expected closure open bracket");
    return false;
  }
  
  getNextToken();
  while (CurTok != ')')
  {
    corevm::IRParameter parameter;
    if (!parse_closure_parameter(parameter))
    {
      return false;
    }
    closure.parameters.push_back(parameter);
  }

  if (getNextToken() == ':')
  {
    if (getNextToken() == Lexer::tok_identifier)
    {
      closure.parent.set_string(lexer.IdentifierStr);
      getNextToken();
    }
    else
    {
      error("Expected string-type parent closure name");
      return false;
    }
  }

  if (CurTok != '{')
  {
    error("Expected closure open brace");
    return false;
  }

  getNextToken();
  while (CurTok != '}')
  {
    corevm::IRBasicBlock block;
    if (!parse_basic_block(block))
    {
      return false;
    }
    closure.blocks.push_back(block);
  }

  module.closures.push_back(closure);

  if (m_debug)
  {
    printf("\n-- End parsing closure : %s --\n", closure.name.c_str());
  }

  getNextToken();

  return true;
}

// -----------------------------------------------------------------------------

bool
IRAssembler::Parser::parse_closure_parameter(corevm::IRParameter& parameter)
{
  if (m_debug)
  {
    printf("-- Begin parsing parameter --\n");
  }

  if (CurTok == Lexer::tok_type)
  {
    parameter.type = str_to_ir_value_type(lexer.IdentifierStr);
    if (m_debug)
    {
      printf("parameter type: %s\n", lexer.IdentifierStr.c_str());
    }
  }
  else
  {
    error("Expected parameter type");
    return false;
  }

  if (getNextToken() == '*')
  {
    parameter.ref_type = corevm::IRValueRefType::pointer;
    getNextToken();
  }
  else
  {
    parameter.ref_type = corevm::IRValueRefType::value;
  }

  if (CurTok == Lexer::tok_identifier)
  {
    parameter.identifier = lexer.IdentifierStr;
    if (m_debug)
    {
      printf("parameter name : %s\n", parameter.identifier.c_str());
    }
  }
  else
  {
    error("Expected parameter name");
    return false;
  }

  if (getNextToken() == ',')
  {
    getNextToken();
  }

  if (m_debug)
  {
    printf("-- End parsing parameter : %s --\n", parameter.identifier.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
IRAssembler::Parser::parse_basic_block(corevm::IRBasicBlock& block)
{
  if (m_debug)
  {
    printf("\n-- Begin parsing block --\n");
  }

  if (CurTok == Lexer::tok_identifier)
  {
    block.label = lexer.IdentifierStr;
    if (m_debug)
    {
      printf("Block label : %s\n", block.label.c_str());
    }
  }
  else
  {
    error("Expected block label");
    return false;
  }

  if (getNextToken() != ':')
  {
    error("Expected colon afer block label");
    return false;
  }

  getNextToken();
  while (true)
  {
    corevm::IRInstruction instruction;
    if (!parse_instruction(instruction))
    {
      return false;
    }

    block.body.push_back(instruction);

    if (!(CurTok == '%' || CurTok == Lexer::tok_opcode))
    {
      break;
    }
  }

  if (m_debug)
  {
    printf("-- End parsing block : %s --\n", block.label.c_str());
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
IRAssembler::Parser::parse_instruction(corevm::IRInstruction& instruction)
{
  if (m_debug)
  {
    printf("--- Begin Parsing instruction ---\n");
  }

  bool has_target = false;
  if (CurTok == '%')
  {
    getNextToken();
    if (CurTok == Lexer::tok_identifier)
    {
      instruction.target.set_string(lexer.IdentifierStr);
      if (m_debug)
      {
        printf("Target: %s\n", lexer.IdentifierStr.c_str());
      }
      has_target = true;
    }
    else
    {
      error("Expected instruction target");
      return false;
    }

    getNextToken();

    if (has_target && CurTok != '=')
    {
      error("Expected '=' in instruction");
      return false;
    }

    getNextToken();
  }

  if (CurTok == Lexer::tok_opcode)
  {
    instruction.opcode = str_to_ir_opcode(lexer.IdentifierStr);
    if (m_debug)
    {
      printf("Opcode: %s\n", lexer.IdentifierStr.c_str());
    }
  }
  else
  {
    error("Expected opcode in instruction");
    return false;
  }

  getNextToken();
  if (!parse_ir_value(instruction.opcodeval))
  {
    return false;
  }

  if (CurTok == ',')
  {
    getNextToken();
  }

  while (CurTok == Lexer::tok_type || CurTok == '%')
  {
    corevm::IROperand oprd;
    if (!parse_operand(oprd))
    {
      return false;
    }
    if (m_debug)
    {
      printf("Added operand\n");
    }
    instruction.oprds.push_back(oprd);
  }
  
  std::vector<corevm::IRLabel> labels;
  bool maybe_labels = false;
  if (CurTok == '[')
  {  
    maybe_labels = true;
    getNextToken();
    while (CurTok != ']')
    {
      corevm::IRLabel label;
      if (!parse_label(label))
      {
        return false;
      }
      labels.push_back(label);
    } 
  }
  else
  {
    if (m_debug)
    {
      printf("--- End Parsing instruction ---\n\n");
    }

    return true;
  }
  
  if (CurTok == ']')
  {
    if (!maybe_labels)
    {
      error("Unexpected ']' in instruction");
      return false;
    }
    instruction.labels.set_array(labels);
    getNextToken();
  }

  if (m_debug)
  {
    printf("--- End Parsing instruction ---\n\n");
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
IRAssembler::Parser::parse_ir_value(corevm::IRValue& ir_val)
{
  if (CurTok == Lexer::tok_type)
  {
    ir_val.type = str_to_ir_value_type(lexer.IdentifierStr);
  }
  else
  {
    error("Expected opcode in instruction");
    return false;
  }

  getNextToken();

  if (ir_val.type == corevm::IRValueType::voidtype)
  {
    return true; 
  }
  else if (ir_val.type == corevm::IRValueType::string)
  {
    if (CurTok != '"')
    {
      error("Expected double-quote for string constant");
      return false;
    }
    if (getNextToken() == Lexer::tok_identifier)
    {
      ir_val.value.set_string(lexer.IdentifierStr);
    }
    getNextToken();
    if (CurTok != '"')
    {
      error("Expected double-quote for string constant");
      return false;
    }
  }
  else if (ir_val.type == corevm::IRValueType::structtype)
  {
    // Currently not supported.
    printf("Encountered IRValue unsupported type 'structtype'\n");
    return false;
  }
  else if (ir_val.type == corevm::IRValueType::ptrtype)
  {
    // Currently not supported.
    printf("Encountered IRValue unsupported type 'ptrtype'\n");
    return false;
  }
  else if (ir_val.type == corevm::IRValueType::i8 ||
           ir_val.type == corevm::IRValueType::ui8 ||
           ir_val.type == corevm::IRValueType::i16 ||
           ir_val.type == corevm::IRValueType::ui16)
  {
    if (CurTok == Lexer::tok_number)
    {
      ir_val.value.set_int(static_cast<int32_t>(lexer.NumVal));
    }
    else
    {
      error("Expected numeric constant");
      return false;
    }
  }
  else if (ir_val.type == corevm::IRValueType::i32 ||
           ir_val.type == corevm::IRValueType::ui32 ||
           ir_val.type == corevm::IRValueType::i64 ||
           ir_val.type == corevm::IRValueType::ui64)
  {
    if (CurTok == Lexer::tok_number)
    {
      ir_val.value.set_long(static_cast<int64_t>(lexer.NumVal));
    }
    else
    {
      error("Expected numeric constant");
      return false;
    }
  }
  else if (ir_val.type == corevm::IRValueType::spf)
  {
    if (CurTok == Lexer::tok_number)
    {
      ir_val.value.set_float(static_cast<float>(lexer.NumVal));
    }
    else
    {
      error("Expected numeric constant");
      return false;
    }
  }
  else if (ir_val.type == corevm::IRValueType::dpf)
  {
    if (CurTok == Lexer::tok_number)
    {
      ir_val.value.set_int(static_cast<int32_t>(lexer.NumVal));
    }
    else
    {
      error("Expected numeric constant");
      return false;
    }
  }
  else if (ir_val.type == corevm::IRValueType::array)
  {
    if (CurTok != '[')
    {
      error("Expected '[' in array constant");
      return false;
    }

    corevm::IRArrayValue array_val;
    if (getNextToken() == Lexer::tok_number)
    {
      array_val.len = static_cast<int32_t>(lexer.NumVal);
    }
    else
    {
      error("Expected numeric constant in array constant");
      return false;
    }

    if (getNextToken() != '*')
    {
      error("Expected '*' in array constant");
      return false;
    }

    if (getNextToken() == Lexer::tok_type)
    {
      array_val.type = str_to_ir_value_type(lexer.IdentifierStr);
    }
    else
    {
      error("Expected value type in array constant");
      return false;
    }

    if (getNextToken() != ']')
    {
      error("Expected ']' in array constant");
      return false;
    }

    ir_val.value.set_IRArrayValue(array_val);
  }

  getNextToken();

  return true;
}

// -----------------------------------------------------------------------------

bool
IRAssembler::Parser::parse_operand(corevm::IROperand& operand)
{
  if (CurTok == Lexer::tok_type)
  {
    corevm::IRValue oprd_val;
    if (!parse_ir_value(oprd_val))
    {
      return false;
    }
    operand.type = corevm::IROperandType::constant;
    operand.value.set_IRValue(oprd_val);
  }
  else if (CurTok == '%')
  {
    if (getNextToken() == Lexer::tok_identifier)
    {
      operand.type = corevm::IROperandType::ref;
      operand.value.set_string(lexer.IdentifierStr);
      getNextToken();
    }
    else
    {
      error("Expected identifier string in operand");
      return false;
    }
  }
  else
  {
    char buf[256] = {0};
    sprintf(buf, "Unexpected character '%c' in operand", CurTok);
    error(buf);
    return false;
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
IRAssembler::Parser::parse_label(corevm::IRLabel& label)
{
  if (CurTok != Lexer::tok_label)
  {
    error("Expected string \"label\" to start label");
    return false;
  }

  if (getNextToken() != '#')
  {
    error("Expected '#' to in label");
    return false;
  }

  if (getNextToken() == Lexer::tok_identifier)
  {
    label.name = lexer.IdentifierStr;
    if (m_debug)
    {
      printf("label: %s\n", label.name.c_str());
    }
  }

  if (getNextToken() == ',')
  {
    getNextToken();
  }

  return true;
}

// -----------------------------------------------------------------------------

void
IRAssembler::Parser::error(const char* format) const
{
  fprintf(stderr, "%s [CurTok:%d]\n", format, CurTok);
}

// -----------------------------------------------------------------------------

int main(int argc, char** argv)
{
  IRAssembler program;
  return program.run(argc, argv);
}

// -----------------------------------------------------------------------------  
