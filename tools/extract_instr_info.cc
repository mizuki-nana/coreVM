/*******************************************************************************
The MIT License (MIT)
Copyright (c) 2015 Yanzheng Li
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
#ifndef COREVM_TOOLS_EXTRACT_INSTRS_INFO_H_
#define COREVM_TOOLS_EXTRACT_INSTRS_INFO_H_


#include "../include/runtime/instr.h"

#include <sneaker/utility/cmdline_program.h>

#include <fstream>
#include <ostream>
#include <ios>
#include <sstream>


class extract_instr_info : public sneaker::utility::cmdline_program
{
public:
  extract_instr_info();

protected:
  virtual int do_run();

  virtual bool check_parameters() const;

private:
  std::string m_output;
};


#endif /* COREVM_TOOLS_EXTRACT_INSTRS_INFO_H_ */


// -----------------------------------------------------------------------------

extract_instr_info::extract_instr_info()
  :
  sneaker::utility::cmdline_program("Extract coreVM instruction set info"),
  m_output()
{
  add_positional_parameter("output", 1);
  add_string_parameter("output", "Output file", &m_output);
}

// -----------------------------------------------------------------------------

bool
extract_instr_info::check_parameters() const
{
  return true;
}

// -----------------------------------------------------------------------------

int
extract_instr_info::do_run()
{
  std::ofstream fd(m_output.c_str(), std::ios::out);

  if (!fd.is_open())
  {
    return -1;
  }

  std::stringstream ss;

  const std::string INDENTATION = "    ";
  const std::string DOUBLE_QUOTE= "\"";

  ss << "{" << std::endl;

  for (auto itr = corevm::runtime::instr_handler_meta::instr_info_map.begin();
       itr != corevm::runtime::instr_handler_meta::instr_info_map.end();)
  {
    const corevm::runtime::instr_code& code = itr->first;
    const corevm::runtime::instr_info& info = itr->second;

    ss << INDENTATION << DOUBLE_QUOTE << info.str << DOUBLE_QUOTE << ": " << code;

    if (++itr != corevm::runtime::instr_handler_meta::instr_info_map.end())
    {
      ss << ",";
    }

    ss << std::endl;
  }

  ss << "}" << std::endl;

  fd << ss.str();

  fd.close();

  return 0;
}

// -----------------------------------------------------------------------------

int main(int argc, char** argv)
{
  extract_instr_info program;
  return program.run(argc, argv);
}

// -----------------------------------------------------------------------------
