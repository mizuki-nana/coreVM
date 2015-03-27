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
#include "bytecode_loader_v0_1.h"

#include "errors.h"
#include "utils.h"
#include "runtime/closure.h"
#include "runtime/common.h"
#include "runtime/compartment.h"
#include "runtime/process.h"
#include "runtime/vector.h"

#include <boost/format.hpp>
#include <sneaker/json/json.h>
#include <sneaker/json/json_schema.h>

#include <string>
#include <utility>


const std::string BYTECODE_LOADER_V0_1_FORMAT = "application/json";

// -----------------------------------------------------------------------------

const std::string BYTECODE_LOADER_V0_1_VERSION = "0.1";

// -----------------------------------------------------------------------------

const std::string
corevm::frontend::bytecode_loader_v0_1::format() const
{
  return BYTECODE_LOADER_V0_1_FORMAT;
}

// -----------------------------------------------------------------------------

const std::string
corevm::frontend::bytecode_loader_v0_1::version() const
{
  return BYTECODE_LOADER_V0_1_VERSION;
}

// -----------------------------------------------------------------------------

const std::string
corevm::frontend::bytecode_loader_v0_1::schema() const
{
  static const std::string unformatted_def(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"format\": {"
          "\"type\": \"string\""
        "},"
        "\"format-version\": {"
          "\"type\": \"string\""
        "},"
        "\"target-version\": {"
          "\"type\": \"string\","
          "\"pattern\": \"(0|1|2|3|4|5|6|7|8|9)(\.)(0|1|2|3|4|5|6|7|8|9)\""
        "},"
        "\"path\": {"
          "\"type\": \"string\""
        "},"
        "\"timestamp\": {"
          "\"type\": \"string\","
          "\"format\": \"date-time\""
        "},"
        "\"encoding\": {"
          "\"type\": \"string\","
          "\"enum\": ["
            "\"utf8\""
          "]"
        "},"
        "\"author\": {"
          "\"type\": \"string\""
        "},"
        "\"encoding_map\": {"
          "\"type\": \"array\","
          "\"items\": {"
            "\"$ref\": \"#/definitions/encoding_pair\""
          "}"
        "},"
        "\"__MAIN__\": {"
          "\"type\": \"array\","
          "\"items\": {"
            "\"$ref\": \"#/definitions/closure\""
          "}"
        "}"
      "},"
      "\"additionalProperties\": false,"
      "\"required\": ["
        "\"format\","
        "\"format-version\","
        "\"target-version\","
        "\"encoding\","
        "\"encoding_map\","
        "\"__MAIN__\""
      "],"
      "\"definitions\": {"
        "\"encoding_pair\": {"
          "\"type\": \"object\","
          "\"properties\": {"
            "\"key\": {"
              "\"type\": \"string\""
            "},"
            "\"value\": {"
              "\"type\": \"integer\""
            "}"
          "}"
        "},"
        "\"vector\": %1%,"
        "\"closure\": {"
          "\"type\": \"object\","
          "\"properties\": {"
            "\"__id__\": {"
              "\"type\": \"integer\""
            "},"
            "\"__vector__\": {"
              "\"$ref\": \"#/definitions/vector\""
            "},"
            "\"__parent__\": {"
              "\"type\": \"integer\""
            "}"
          "},"
          "\"required\": ["
            "\"__id__\","
            "\"__vector__\""
          "]"
        "}"
      "}"
    "}"
  );

  const std::string def(
    str(
      boost::format(unformatted_def)
        % get_v0_1_vector_schema_definition()
    )
  );

  return def;
}

// -----------------------------------------------------------------------------

void
corevm::frontend::bytecode_loader_v0_1::load(
  const std::string& path, const JSON& content_json,
  corevm::runtime::process& process)
{
  corevm::runtime::compartment compartment(path);

  const JSON::object& json_object = content_json.object_items();

  // Load encoding map.
  corevm::runtime::encoding_map encoding_map;
  const JSON::array& encoding_array = \
    json_object.at("encoding_map").array_items();

  for (auto itr = encoding_array.begin(); itr != encoding_array.end(); ++itr)
  {
    const JSON& raw_encoding_pair = static_cast<JSON>(*itr);
    const JSON::object& encoding_pair = raw_encoding_pair.object_items();

    // Keys and values are flipped.
    const JSON& raw_value = encoding_pair.at("key");
    const JSON& raw_key = encoding_pair.at("value");

    const std::string value = static_cast<std::string>(raw_value.string_value());
    const uint64_t key = static_cast<uint64_t>(raw_key.int_value());

    encoding_map[key] = value;
  }

  compartment.set_encoding_map(encoding_map);

  /* --------------------------- Load closures. --------------------------- */

  corevm::runtime::closure_table closure_table;

  const JSON::array& closures = json_object.at("__MAIN__").array_items();

  for (auto itr = closures.begin(); itr != closures.end(); ++itr)
  {
    const JSON& closure_raw = static_cast<JSON>(*itr);
    const JSON::object& closure = closure_raw.object_items();

    // ID
    const int __id__ = closure.at("__id__").int_value();
    corevm::runtime::closure_id id = __id__;

    // Parent ID
    corevm::runtime::closure_id parent_id = corevm::runtime::NONESET_CLOSURE_ID;
    if (closure.find("__parent__") != closure.end())
    {
      const int __parent__ = closure.at("__parent__").int_value();
      parent_id = __parent__;
    }

    // Vector
    const JSON& __vector__ = closure.at("__vector__");
    corevm::runtime::vector vector = \
      corevm::frontend::get_vector_from_json(__vector__);

    closure_table.push_back(
      corevm::runtime::closure {
        .id = id,
        .parent_id = parent_id,
        .vector = vector
      }
    );

  } /* end for-loop */

  compartment.set_closure_table(closure_table);

  process.insert_compartment(compartment);
}

// -----------------------------------------------------------------------------
