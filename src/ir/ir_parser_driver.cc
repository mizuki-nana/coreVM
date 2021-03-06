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
#include "ir_parser_driver.h"
#include "ir_parser.tab.hh"
#include "lex.yy.hh"

#include <iostream>
#include <iterator>
#include <fstream>


namespace corevm {
namespace ir {

// -----------------------------------------------------------------------------

IRParserDriver::IRParserDriver()
  :
  m_trace_parsing(false),
  m_input_file()
{
}

// -----------------------------------------------------------------------------

IRParserDriver::~IRParserDriver()
{
}

// -----------------------------------------------------------------------------

bool
IRParserDriver::trace_parsing() const
{
  return m_trace_parsing;
}

// -----------------------------------------------------------------------------

void
IRParserDriver::set_trace_parsing(bool val)
{
  m_trace_parsing = val;
}

// -----------------------------------------------------------------------------

int
IRParserDriver::parse_from_file(const std::string& filepath)
{
  m_input_file = filepath;
  std::ifstream infile(filepath.c_str());
  std::string file_contents((std::istreambuf_iterator<char>(infile)),
    std::istreambuf_iterator<char>());
  return parse_from_string(file_contents.c_str());
}

// -----------------------------------------------------------------------------

int IRParserDriver::parse_from_string(const char* input)
{
  yyscan_t scanner;
  YY_BUFFER_STATE buf;
  yylex_init(&scanner);
  buf = yy_scan_string(input, scanner);
  yy::ir_parser parser(*this, scanner);
  parser.set_debug_level(trace_parsing());
  parser.parse();
  yy_delete_buffer(buf, scanner);
  yylex_destroy(scanner);
  return 0;
}

// -----------------------------------------------------------------------------

const std::string&
IRParserDriver::input_file() const
{
  return m_input_file;
}

// -----------------------------------------------------------------------------

std::string&
IRParserDriver::input_file()
{
  return m_input_file;
}

// -----------------------------------------------------------------------------

void
IRParserDriver::error(const yy::location& l, const std::string& m)
{
  std::cerr << "Parser error [" << l << "] : " << m << std::endl;
}

// -----------------------------------------------------------------------------

void
IRParserDriver::error(const std::string& m)
{
  std::cerr << "Parser error: " << m << std::endl;
}

// -----------------------------------------------------------------------------

void
IRParserDriver::set_module(corevm::IRModule&& module)
{
  m_module = std::move(module);
}

// -----------------------------------------------------------------------------

const corevm::IRModule&
IRParserDriver::module() const
{
  return m_module;
}

// -----------------------------------------------------------------------------

} /* end namespace ir */
} /* end namespace corevm */
