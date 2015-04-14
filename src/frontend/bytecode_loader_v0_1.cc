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
#include "runtime/catch_site.h"
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


// -----------------------------------------------------------------------------

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
        "\"path\","
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
        "\"locs\": %2%,"
        "\"catch_sites\": %3%,"
        "\"closure\": {"
          "\"type\": \"object\","
          "\"properties\": {"
            "\"name\": {"
              "\"type\": \"string\""
            "},"
            "\"__id__\": {"
              "\"type\": \"integer\""
            "},"
            "\"__vector__\": {"
              "\"$ref\": \"#/definitions/vector\""
            "},"
            "\"__parent__\": {"
              "\"type\": \"integer\""
            "},"
            "\"locs\": {"
              "\"$ref\": \"#/definitions/locs\""
            "},"
            "\"catch_sites\": {"
              "\"$ref\": \"#/definitions/catch_sites\""
            "}"
          "},"
          "\"required\": ["
            "\"name\","
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
        % get_v0_1_locs_schema_definition()
        % get_v0_1_catch_sites_schema_definition()
    )
  );

  return def;
}

// -----------------------------------------------------------------------------

void
corevm::frontend::bytecode_loader_v0_1::load(
  const std::string& path,
  const JSON& content_json,
  corevm::runtime::process& process)
{
  const JSON::object& json_object = content_json.object_items();

  const std::string& source_path = json_object.at("path").string_value();

  corevm::runtime::compartment compartment(source_path);

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

    const JSON::string name_ = closure.at("name").string_value();
    std::string name = static_cast<std::string>(name_);

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

    corevm::runtime::loc_table locs_table;

    // Locs
    if (closure.find("locs") != closure.end())
    {
      const JSON::array items = closure.at("locs").array_items();

      for (auto locs_itr = items.begin(); locs_itr != items.end(); ++locs_itr)
      {
        const JSON::object item = static_cast<JSON>(*locs_itr).object_items();

        const int32_t index = item.at("index").int_value();

        const JSON::object loc_item = item.at("loc").object_items();

        const int32_t lineno = loc_item.at("lineno").int_value();
        const int32_t col_offset = loc_item.at("col_offset").int_value();

        corevm::runtime::loc_info loc {
          .lineno = lineno,
          .col_offset = col_offset
        };

        locs_table[index] = loc;
      }
    }

    corevm::runtime::catch_site_list catch_sites;

    // Catch sites
    if (closure.find("catch_sites") != closure.end())
    {
      const JSON::array items = closure.at("catch_sites").array_items();

      for (auto catch_sites_itr = items.begin();
          catch_sites_itr != items.end(); ++catch_sites_itr)
      {
        const JSON::object item = static_cast<JSON>(*catch_sites_itr).object_items();

        const uint32_t from = item.at("from").int_value();
        const uint32_t to = item.at("to").int_value();
        const uint32_t dst = item.at("dst").int_value();

        corevm::runtime::catch_site catch_site {
          .from = from,
          .to = to,
          .dst = dst
        };

        catch_sites.push_back(catch_site);
      }
    }

    closure_table.push_back(
      corevm::runtime::closure {
        .name = name,
        .id = id,
        .parent_id = parent_id,
        .vector = vector,
        .locs = locs_table,
        .catch_sites = catch_sites,
      }
    );

  } /* end for-loop */

  compartment.set_closure_table(closure_table);

  process.insert_compartment(compartment);
}

// -----------------------------------------------------------------------------
