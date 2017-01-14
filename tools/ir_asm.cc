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
#include "ir/ir_parser_driver.h"

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

class IRAssembler : public sneaker::utility::cmdline_program
{
public:
  IRAssembler();

  virtual ~IRAssembler() = default;

protected:
  virtual int do_run();

  virtual bool check_parameters() const;

private:
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

  corevm::ir::IRParserDriver driver;
  if (int res = driver.parse_from_file(m_input) != 0)
  {
    return res;
  }

  if (m_debug)
  {
    printf("\nFinished parsing IR module\n");
  }

  const corevm::IRModule& module = driver.module();

  static const char* SCHEMA_FILEPATH = "schemas/corevm_ir_schema.json";

  std::ifstream schema_ifs(SCHEMA_FILEPATH);
  avro::ValidSchema schema;
  avro::compileJsonSchema(schema_ifs, schema);

  avro::DataFileWriter<corevm::IRModule> writer(m_output.c_str(), schema);
  writer.write(module);

  writer.close();
  schema_ifs.close();

  if (m_debug)
  {
    printf("\nSuccessfully written IR module to %s\n", m_output.c_str());
  }

  return 0;
}

// -----------------------------------------------------------------------------

int main(int argc, char** argv)
{
  IRAssembler program;
  return program.run(argc, argv);
}

// -----------------------------------------------------------------------------  
