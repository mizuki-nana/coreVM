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


// -----------------------------------------------------------------------------

IRParserDriver::IRParserDriver()
  :
  m_trace_scanning(false),
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
IRParserDriver::trace_scanning() const
{
  return m_trace_scanning;
}

// -----------------------------------------------------------------------------

void
IRParserDriver::set_trace_scanning(bool val)
{
  m_trace_scanning = val;
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
IRParserDriver::parse_from_file(const std::string &f)
{
  m_input_file = f;
  scan_begin();
  yy::ir_parser parser(*this);
  parser.set_debug_level(trace_parsing());
  int res = parser.parse();
  scan_end();
  return res;
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
  std::cerr << l << ": " << m << std::endl;
}

// -----------------------------------------------------------------------------

void
IRParserDriver::error(const std::string& m)
{
  std::cerr << m << std::endl;
}

// -----------------------------------------------------------------------------

void
IRParserDriver::scan_begin()
{
  yyset_debug(trace_scanning());
  if (m_input_file.empty () || m_input_file == "-")
  {
    yyin = stdin;
  }
  else if (!(yyin = fopen (m_input_file.c_str (), "r")))
  {
    error("cannot open " + m_input_file + ": " + strerror(errno));
    exit(EXIT_FAILURE);
  }
}

// -----------------------------------------------------------------------------

void
IRParserDriver::scan_end()
{
  fclose(yyin);
}

// -----------------------------------------------------------------------------
