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
#include "binary_bytecode_loader.h"

#include "errors.h"
#include "corevm/corevm_bytecode_schema.h" // Compiled.
#include "runtime/catch_site.h"
#include "runtime/closure.h"
#include "runtime/compartment.h"
#include "runtime/instr.h"
#include "runtime/loc_info.h"
#include "runtime/process.h"
#include "runtime/vector.h"

#include <avro/DataFile.hh>


namespace corevm {


namespace frontend {


// -----------------------------------------------------------------------------

void
BinaryBytecodeLoader::load(const std::string& path, runtime::Process& process)
{
  // `avro::DataFileReader` documentation:
  // http://avro.apache.org/docs/1.6.3/api/cpp/html/classavro_1_1DataFileReader.html
  corevm::bytecode bytecode_data;
  avro::DataFileReader<corevm::bytecode> reader(path.c_str());
  reader.read(bytecode_data);

  // Load source path.
  const std::string& source_path = bytecode_data.path;

  runtime::Compartment compartment(source_path);

  // Load string literal table.
  compartment.set_string_literal_table(std::move(bytecode_data.string_literal_table));

  // Load fpt literal table.
  compartment.set_fpt_literal_table(std::move(bytecode_data.fpt_literal_table));

  // Load closures.
  compartment.set_closure_table(std::move(bytecode_data.__MAIN__));

  // Insert compartment.
  process.insert_compartment(std::move(compartment));
}

// -----------------------------------------------------------------------------


} /* end namespace frontend */


} /* end namespace corevm */
