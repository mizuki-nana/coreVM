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
#include "dyobj/flags.h"
#include "runtime/common.h"
#include "runtime/instr_info.h"

#include <sneaker/utility/cmdline_program.h>

#include <fstream>
#include <ostream>
#include <ios>
#include <sstream>
#include <string>


class ExtractMetadata : public sneaker::utility::cmdline_program
{
public:
  ExtractMetadata();

  virtual ~ExtractMetadata();

protected:
  virtual int do_run();

  virtual bool check_parameters() const;

private:
  const std::string extract_instr_info() const;
  const std::string extract_flags_info() const;

  std::string m_output;
};


// -----------------------------------------------------------------------------

const std::string INDENTATION = "    ";
const std::string DOUBLE_QUOTE= "\"";
const std::string INSTR_STR_TO_CODE_MAP = "INSTR_STR_TO_CODE_MAP";
const std::string DYOBJ_FLAG_STR_TO_VALUE_MAP = "DYOBJ_FLAG_STR_TO_VALUE_MAP";

// -----------------------------------------------------------------------------

ExtractMetadata::ExtractMetadata()
  :
  sneaker::utility::cmdline_program("Extract coreVM info"),
  m_output()
{
  add_positional_parameter("output", 1);
  add_string_parameter("output", "Output file", &m_output);
}

// -----------------------------------------------------------------------------

/* virtual */
ExtractMetadata::~ExtractMetadata()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

bool
ExtractMetadata::check_parameters() const
{
  return true;
}

// -----------------------------------------------------------------------------

int
ExtractMetadata::do_run()
{
  std::ofstream fd(m_output.c_str(), std::ios::out);

  if (!fd.is_open())
  {
    return -1;
  }

  std::stringstream ss;

  ss << "{" << std::endl;

  ss << INDENTATION << DOUBLE_QUOTE << INSTR_STR_TO_CODE_MAP
    << DOUBLE_QUOTE << ": " << extract_instr_info() << "," << std::endl;

  ss << INDENTATION << DOUBLE_QUOTE << DYOBJ_FLAG_STR_TO_VALUE_MAP
    << DOUBLE_QUOTE << ": " << extract_flags_info() << std::endl;

  ss << "}" << std::endl;

  fd << ss.str();

  fd.close();

  return 0;
}

// -----------------------------------------------------------------------------

const std::string
ExtractMetadata::extract_instr_info() const
{
  std::stringstream ss;

  ss << "{" << std::endl;

  for (size_t i = 0; i < corevm::runtime::InstrEnum::INSTR_CODE_MAX; ++i)
  {
    const corevm::runtime::instr_code code = static_cast<corevm::runtime::instr_code>(i);
    const corevm::runtime::InstrInfo& info = corevm::runtime::InstrSetInfo::instr_infos[code];

    ss << INDENTATION << INDENTATION << DOUBLE_QUOTE << info.name
      << DOUBLE_QUOTE << ": " << code;

    if (i + 1 != corevm::runtime::InstrEnum::INSTR_CODE_MAX)
    {
      ss << ",";
    }

    ss << std::endl;
  }

  ss << INDENTATION << "}";

  return std::move(ss.str());
}


// -----------------------------------------------------------------------------

const std::string
ExtractMetadata::extract_flags_info() const
{
  std::stringstream ss;

  auto array_size = corevm::dyobj::DYOBJ_FLAG_VALUES_ARRAY.size();

  ss << "{" << std::endl;

  for (size_t i = 0; i < array_size; ++i)
  {
    size_t flag_value = i;
    const char* flag_str = corevm::dyobj::DYOBJ_FLAG_VALUES_ARRAY[i];

    ss << INDENTATION << INDENTATION << DOUBLE_QUOTE << std::string(flag_str)
      << DOUBLE_QUOTE << ": " << flag_value;

    if (i + 1 != array_size)
    {
      ss << ",";
    }

    ss << std::endl;
  }

  ss << INDENTATION << "}";

  return std::move(ss.str());
}

// -----------------------------------------------------------------------------

int main(int argc, char** argv)
{
  ExtractMetadata program;
  return program.run(argc, argv);
}

// -----------------------------------------------------------------------------
