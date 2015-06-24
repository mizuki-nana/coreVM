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
#include "bytecode_loader_text.h"

#include "errors.h"
#include "utils.h"
#include "corevm/macros.h"
#include "corevm/version.h"
#include "runtime/catch_site.h"
#include "runtime/closure.h"
#include "runtime/common.h"
#include "runtime/compartment.h"
#include "runtime/process.h"
#include "runtime/vector.h"

#include <boost/format.hpp>
#include <sneaker/json/json.h>
#include <sneaker/json/json_schema.h>

#include <cstdint>
#include <fstream>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>


// -----------------------------------------------------------------------------

const std::string
corevm::frontend::bytecode_loader_text::format() const
{
  return "application/json";
}

// -----------------------------------------------------------------------------

const std::string
corevm::frontend::bytecode_loader_text::schema() const
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
            "\"type\": \"string\""
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
corevm::frontend::bytecode_loader_text::load(
  const std::string& path, corevm::runtime::process& process)
{
  std::ifstream f(path, std::ios::binary);
  std::stringstream buffer;

  try
  {
    f.exceptions(std::ios::failbit | std::ios::badbit);
    buffer << f.rdbuf();
    f.close();
  }
  catch (const std::ios_base::failure& ex)
  {
    THROW(corevm::frontend::file_loading_error(
      str(
        boost::format(
          "Error while loading file \"%s\": %s"
        ) % path % ex.what()
      )
    ));
  }

  std::string content = buffer.str();

  JSON content_json;

  try
  {
    content_json = sneaker::json::parse(content);
  }
  catch (const sneaker::json::invalid_json_error& ex)
  {
    THROW(corevm::frontend::file_loading_error(
      str(
        boost::format("Error while parsing file \"%s\": %s") % path % ex.what()
      )
    ));
  }

  validate_and_load(path, content_json, process);
}

// -----------------------------------------------------------------------------

void
corevm::frontend::bytecode_loader_text::validate_and_load(
  const std::string& path,
  const JSON& content_json,
  corevm::runtime::process& process)
{
  const JSON::object& json_object = content_json.object_items();

  const JSON::string& target_version = json_object.at("target-version").string_value();
  const std::string target_version_str = static_cast<std::string>(target_version);

  if (target_version_str != COREVM_SHORT_CANONICAL_VERSION)
  {
    THROW(corevm::frontend::file_loading_error(
      str(
        boost::format("Invalid target-version: %s") % target_version_str
      )
    ));
  }

  // TODO: validate format and format version.
  /*
  const JSON::string& format = json_object.at("format").string_value();
  const JSON::string& format_version = json_object.at("format-version").string_value();

  const std::string& format_str = static_cast<std::string>(format);
  const std::string& format_version_str = static_cast<std::string>(format_version);
  */

  const std::string& schema = this->schema();
  const JSON schema_json = sneaker::json::parse(schema);

  try
  {
    sneaker::json::json_schema::validate(content_json, schema_json);
  }
  catch (const sneaker::json::json_validation_error& ex)
  {
    THROW(corevm::frontend::file_loading_error(
      str(
        boost::format("Invalid format in file: %s") % ex.what()
      )
    ));
  }

  // Load
  load_bytecode(path, content_json, process);
}

// -----------------------------------------------------------------------------

void
corevm::frontend::bytecode_loader_text::load_bytecode(
  const std::string& path,
  const JSON& content_json,
  corevm::runtime::process& process)
{
  const JSON::object& json_object = content_json.object_items();

  const std::string& source_path = json_object.at("path").string_value();

  corevm::runtime::compartment compartment(source_path);

  // Load encoding map.
  const JSON::array& encoding_array =
    json_object.at("encoding_map").array_items();

  corevm::runtime::encoding_map encoding_map(encoding_array.size());

  uint64_t i = 0;
  for (auto itr = encoding_array.begin();
       itr != encoding_array.end();
       ++itr, ++i)
  {
    const JSON& raw_encoding_str = static_cast<JSON>(*itr);
    const std::string encoding_str =
      static_cast<std::string>(raw_encoding_str.string_value());

    encoding_map[i] = encoding_str;
  }

  compartment.set_encoding_map(std::move(encoding_map));

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
    corevm::runtime::vector vector =
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
          catch_sites_itr != items.end();
          ++catch_sites_itr)
      {
        const JSON::object item =
          static_cast<JSON>(*catch_sites_itr).object_items();

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
        .name = std::move(name),
        .id = id,
        .parent_id = parent_id,
        .vector = std::move(vector),
        .locs = std::move(locs_table),
        .catch_sites = std::move(catch_sites),
      }
    );

  } /* end for-loop */

  compartment.set_closure_table(std::move(closure_table));

  process.insert_compartment(std::move(compartment));
}

// -----------------------------------------------------------------------------
