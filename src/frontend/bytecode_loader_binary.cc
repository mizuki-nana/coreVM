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
#include "bytecode_loader_binary.h"

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
bytecode_loader_binary::load(const std::string& path, runtime::process& process)
{
  // `avro::DataFileReader` documentation:
  // http://avro.apache.org/docs/1.6.3/api/cpp/html/classavro_1_1DataFileReader.html
  corevm::bytecode bytecode_data;
  avro::DataFileReader<corevm::bytecode> reader(path.c_str());
  reader.read(bytecode_data);

  // Load source path.
  const std::string& source_path = bytecode_data.path;

  runtime::compartment compartment(source_path);

  // Load encoding map.
  compartment.set_encoding_map(bytecode_data.encoding_map);

  // Load closures.
  runtime::closure_table closure_table;

  for (auto itr = bytecode_data.__MAIN__.cbegin();
       itr != bytecode_data.__MAIN__.cend();
       ++itr)
  {
    const auto& closure = *itr;

    // Name
    const std::string& name = closure.name;

    // ID
    const auto id = static_cast<runtime::closure_id>(closure.__id__);

    // Parent ID
    runtime::closure_id parent_id = runtime::NONESET_CLOSURE_ID;
    if (!closure.__parent__.is_null())
    {
      parent_id =
        static_cast<runtime::closure_id>(closure.__parent__.get_long());
    }

    // Vector
    runtime::vector vector;

    for (auto vector_itr = closure.__vector__.cbegin();
         vector_itr != closure.__vector__.cend();
         ++vector_itr)
    {
      const auto& instr = *vector_itr;

      const auto code = static_cast<runtime::instr_code>(instr.code);
      const auto oprd1 = static_cast<runtime::instr_oprd>(instr.oprd1);
      const auto oprd2 = static_cast<runtime::instr_oprd>(instr.oprd2);

      vector.emplace_back(code, oprd1, oprd2);
    }

    // Locs
    runtime::loc_table locs_table;
    if (!closure.locs.is_null())
    {
      const auto& locs_array = closure.locs.get_array();
      for (auto locs_itr = locs_array.cbegin();
           locs_itr != locs_array.cend();
           ++locs_itr)
      {
        const auto& loc_record = *locs_itr;

        const auto index = static_cast<int32_t>(loc_record.index);
        const auto lineno = static_cast<int32_t>(loc_record.lineno);
        const auto col_offset = static_cast<int32_t>(loc_record.col_offset);

        runtime::loc_info loc(lineno, col_offset);

        locs_table[index] = loc;
      }
    }

    // Catch sites
    runtime::catch_site_list catch_sites;
    if (!closure.catch_sites.is_null())
    {
      const auto& catch_sites_array = closure.catch_sites.get_array();
      for (auto catch_sites_itr = catch_sites_array.cbegin();
           catch_sites_itr != catch_sites_array.cend();
           ++catch_sites_itr)
      {
        const auto& catch_site_record = *catch_sites_itr;

        const auto from = static_cast<uint32_t>(catch_site_record.from);
        const auto to = static_cast<uint32_t>(catch_site_record.to);
        const auto dst = static_cast<uint32_t>(catch_site_record.dst);

        catch_sites.emplace_back(from, to, dst);
      }
    }

    closure_table.emplace_back(
      std::move(name),
      id,
      parent_id,
      std::move(vector),
      std::move(locs_table),
      std::move(catch_sites));

  } /* end for-loop */

  compartment.set_closure_table(std::move(closure_table));

  process.insert_compartment(std::move(compartment));
}

// -----------------------------------------------------------------------------


} /* end namespace frontend */


} /* end namespace corevm */
