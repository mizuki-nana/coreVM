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
#include "../../include/frontend/bytecode_loader_v0_1.h"

#include "../../include/frontend/errors.h"
#include "../../include/frontend/utils.h"
#include "../../include/runtime/closure.h"
#include "../../include/runtime/common.h"
#include "../../include/runtime/process.h"
#include "../../include/runtime/vector.h"

#include <sneaker/json/json.h>
#include <sneaker/json/json_schema.h>

#include <string>


const std::string BYTECODE_LOADER_V0_1_FORMAT = "application/json";

const std::string BYTECODE_LOADER_V0_1_VERSION = "0.1";


std::string
corevm::frontend::bytecode_loader_v0_1::format() const
{
  return BYTECODE_LOADER_V0_1_FORMAT;
}

std::string
corevm::frontend::bytecode_loader_v0_1::version() const
{
  return BYTECODE_LOADER_V0_1_VERSION;
}

std::string
corevm::frontend::bytecode_loader_v0_1::schema() const
{
  return \
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
        "\"instr\": {"
          "\"type\": \"integer\","
          "\"minimum\": 0,"
          "\"maximum\": 4294967295"
        "},"
        "\"vector\": {"
          "\"type:\": \"array\","
          "\"items\": {"
            "\"type:\": \"array\","
            "\"items\": ["
              "{"
                "\"$ref\": \"#/definitions/instr\""
              "},"
              "{"
                "\"$ref\": \"#/definitions/instr\""
              "},"
              "{"
                "\"$ref\": \"#/definitions/instr\""
              "}"
            "],"
            "\"minItems\": 1,"
            "\"maxItems\": 3,"
            "\"additionalItems\": false"
          "}"
        "},"
        "\"closure\": {"
          "\"type\": \"object\","
          "\"properties\": {"
            "\"__name__\": {"
              "\"type\": \"string\""
            "},"
            "\"__vector__\": {"
              "\"$ref\": \"#/definitions/vector\""
            "},"
            "\"__parent__\": {"
              "\"type\": \"string\""
            "}"
          "},"
          "\"required\": ["
            "\"__name__\","
            "\"__vector__\""
          "]"
        "}"
      "}"
    "}";
}

void
corevm::frontend::bytecode_loader_v0_1::load(
  const JSON& content_json, corevm::runtime::process& process)
{
  const JSON::object& json_object = content_json.object_items();

  const JSON::string& encoding = json_object.at("encoding").string_value();

  // Load encoding map.
  const JSON::array& encoding_map = json_object.at("encoding_map").array_items();

  for (auto itr = encoding_map.begin(); itr != encoding_map.end(); ++itr) {
    const JSON& raw_encoding_pair = static_cast<JSON>(*itr);
    const JSON::object& encoding_pair = raw_encoding_pair.object_items();

    // Keys and values are flipped.
    const JSON& raw_value = encoding_pair.at("key");
    const JSON& raw_key = encoding_pair.at("value");

    const std::string value = static_cast<std::string>(raw_value.string_value());
    const uint64_t key = static_cast<uint64_t>(raw_key.int_value());

    process.set_encoding_key_value_pair(key, value);
  }

  const JSON::array& closures = json_object.at("__MAIN__").array_items();

  /* --------------------------- Load closures. --------------------------- */

  // Translate local closure identifiers to global IDs.
  std::unordered_map<std::string, corevm::runtime::closure_id> str_to_closure_id_map;

  for (auto itr = closures.begin(); itr != closures.end(); ++itr) {
    const JSON& closure_raw = static_cast<JSON>(*itr);
    const JSON::object& closure = closure_raw.object_items();

    const JSON::string& __name__ = closure.at("__name__").string_value();
    const JSON& __vector__ = closure.at("__vector__");

    const std::string name = static_cast<std::string>(__name__);
    corevm::runtime::vector vector = corevm::frontend::get_vector_from_json(__vector__);

    if (closure.find("__parent__") == closure.end()) {
      process.append_vector(vector);
      continue;
    }

    const JSON::string& __parent__ = closure.at("__parent__").string_value();
    const std::string parent = static_cast<std::string>(__parent__);

    if (str_to_closure_id_map.find(name) == str_to_closure_id_map.end()) {
      str_to_closure_id_map[name] = process.get_new_closure_id();
    }

    if (str_to_closure_id_map.find(parent) == str_to_closure_id_map.end()) {
      str_to_closure_id_map[parent] = \
        parent.empty() ? corevm::runtime::NONESET_CLOSURE_ID : process.get_new_closure_id();
    }

    corevm::runtime::closure_id id = str_to_closure_id_map.at(name);
    corevm::runtime::closure_id parent_id = str_to_closure_id_map.at(parent);

    process.insert_closure(
      corevm::runtime::closure {
        .id = id,
        .parent_id = parent_id,
        .vector = vector
      }
    );
  }
}
