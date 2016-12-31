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
#include "ir/ir_parser_driver.h"

#include <sneaker/utility/cmdline_program.h>

#include <cstdio>
#include <string>


// -----------------------------------------------------------------------------

class ir_parser_driver : public sneaker::utility::cmdline_program
{
public:
  ir_parser_driver();
  virtual ~ir_parser_driver() = default;

protected:
  virtual int do_run();
  virtual bool check_parameters() const;

private:
  std::string m_input;
  bool m_debug;
};

// -----------------------------------------------------------------------------

ir_parser_driver::ir_parser_driver()
  :
  sneaker::utility::cmdline_program("coreVM IR parser driver"),
  m_input(),
  m_debug(false)
{
  add_positional_parameter("input", 1);
  add_string_parameter("input", "input file", &m_input);
  add_boolean_parameter("debug", "debug mode", &m_debug);
}

// -----------------------------------------------------------------------------

bool
ir_parser_driver::check_parameters() const
{
  return !m_input.empty();
}

// -----------------------------------------------------------------------------

int
ir_parser_driver::do_run()
{
  IRParserDriver driver;

  if (m_debug)
  {
    printf("Debug mode ON\n");
    driver.set_trace_scanning(true);
    driver.set_trace_parsing(true);
  }

  return driver.parse_from_file(m_input);
}

// -----------------------------------------------------------------------------

int main(int argc, char** argv)
{
  ir_parser_driver driver;
  return driver.run(argc, argv);
}

// -----------------------------------------------------------------------------
