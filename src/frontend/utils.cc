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
#include "utils.h"

#include "corevm/macros.h"
#include "runtime/instr.h"
#include "runtime/vector.h"

#include <boost/format.hpp>
#include <sneaker/json/json.h>

#include <limits>
#include <sstream>
#include <string>
#include <utility>


corevm::runtime::vector
corevm::frontend::get_vector_from_json(const JSON& json)
{
#if __DEBUG__
  ASSERT(json.type() == JSON::ARRAY);
#endif

  const JSON::array& vector_array = json.array_items();
  corevm::runtime::vector vector;

  for (auto itr = vector_array.begin(); itr != vector_array.end(); ++itr)
  {
    const JSON& instr_json = *itr;
    const JSON::array& instr_tuple = instr_json.array_items();

#if __DEBUG__
    ASSERT(instr_tuple.size() == 3);
#endif

    const JSON& code_raw = instr_tuple[0];
    const JSON& oprd1_raw = instr_tuple[1];
    const JSON& oprd2_raw = instr_tuple[2];

    corevm::runtime::instr_code code = \
      static_cast<corevm::runtime::instr_code>(code_raw.int_value());
    corevm::runtime::instr_oprd oprd1 = \
      static_cast<corevm::runtime::instr_oprd>(oprd1_raw.int_value());
    corevm::runtime::instr_oprd oprd2 = \
      static_cast<corevm::runtime::instr_oprd>(oprd2_raw.int_value());

    vector.push_back(
      corevm::runtime::instr {
        .code = code,
        .oprd1 = oprd1,
        .oprd2 = oprd2,
      }
    );
  }

  return std::move(vector);
}

// -----------------------------------------------------------------------------

const std::string
corevm::frontend::get_v0_1_instr_code_schema_definition()
{
  static const std::string unformatted_def(
    "{"
      "\"type\": \"integer\","
      "\"enum\": %s"
    "}"
  );

  std::stringstream ss;
  ss << "[";

  for (auto itr = runtime::instr_handler_meta::instr_info_map.begin();
       itr != runtime::instr_handler_meta::instr_info_map.end();
       ++itr)
  {
    const runtime::instr_code& code = itr->first;
    ss << code;

    auto itr_ = itr;
    ++itr_;

    if (itr_ != runtime::instr_handler_meta::instr_info_map.end())
    {
      ss << ",";
    }
  }
  ss << "]";

  const std::string def(
    str(boost::format(unformatted_def) % ss.str().c_str()));

  return std::move(def);
}

// -----------------------------------------------------------------------------

const std::string
corevm::frontend::get_v0_1_instr_oprd_schema_definition()
{
  static const std::string unformatted_def(
    "{"
      "\"type\": \"integer\","
      "\"minimum\": %llu,"
      "\"maximum\": %llu"
    "}"
  );

  // TODO: [COREVM-198] Bytecode schema does not handle 64-bit integers
  const std::string def(
    str(
      boost::format(unformatted_def)
        % std::numeric_limits<int>::min()
        % std::numeric_limits<int>::max()
    )
  );

  return std::move(def);
}

// -----------------------------------------------------------------------------

const std::string
corevm::frontend::get_v0_1_vector_schema_definition()
{
  static const std::string unformatted_def(
    "{"
      "\"type\": \"array\","
      "\"items\": {"
        "\"type\": \"array\","
        "\"items\": ["
          "%1%,"
          "%2%,"
          "%2%"
        "],"
        "\"minItems\": 3,"
        "\"maxItems\": 3,"
        "\"additionalItems\": false"
      "}"
    "}"
  );

  const std::string def(
    str(
      boost::format(unformatted_def)
        % get_v0_1_instr_code_schema_definition()
        % get_v0_1_instr_oprd_schema_definition()
    )
  );

  return std::move(def);
}

// -----------------------------------------------------------------------------
