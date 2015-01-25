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
#include "../../include/frontend/bytecode_loader.h"

#include "../../include/frontend/errors.h"
#include "../../include/frontend/utils.h"
#include "../../include/runtime/common.h"
#include "../../include/runtime/process.h"
#include "../../include/runtime/vector.h"

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
#include <vector>


using sneaker::json::JSON;


namespace corevm {


namespace frontend {


namespace internal {


class bytecode_loader {
public:
  virtual void load(const JSON&, corevm::runtime::process&) = 0;
  virtual std::string schema() const = 0;
};


class bytecode_loader_v0_1 : public bytecode_loader {
public:
  virtual std::string schema() const
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
            "\"type\": \"string\""
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

  virtual void load(const JSON& content_json, corevm::runtime::process& process)
  {
    const JSON::object& json_object = content_json.object_items();

    // [COREVM-116] Implement mechanism to check bytecode format and target verisons
    const JSON::string& format = json_object.at("format").string_value();
    const JSON::string& format_version = json_object.at("format-version").string_value();
    const JSON::string& target_version = json_object.at("target-version").string_value();
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

}; /* end class bytecode_loader_v0_1 */


typedef struct bytecode_loader_wrapper {
  const std::string version;
  bytecode_loader* loader;
} bytecode_loader_wrapper;


class schema_repository {
public:
  static const bytecode_loader_wrapper load_by_format_version(const std::string&);
private:
  static const std::vector<bytecode_loader_wrapper> bytecode_loader_definitions;
};


const std::vector<bytecode_loader_wrapper>
corevm::frontend::internal::schema_repository::bytecode_loader_definitions {
  {
    .version = "0.1",
    .loader = new bytecode_loader_v0_1(),
  }, /* end 0.1 */
};


const bytecode_loader_wrapper
corevm::frontend::internal::schema_repository::load_by_format_version(
  const std::string& format_version)
{
  auto itr = std::find_if(
    bytecode_loader_definitions.begin(),
    bytecode_loader_definitions.end(),
    [&format_version](const bytecode_loader_wrapper& wrapper) -> bool {
      return wrapper.version == format_version;
    }
  );

  if (itr == bytecode_loader_definitions.end()) {
    throw file_loading_error(
      str(
        boost::format("Unrecognized format-version: \"%s\"") % format_version
      )
    );
  }

  return static_cast<bytecode_loader_wrapper>(*itr);
}

void
validate_and_load(const JSON& content_json, corevm::runtime::process& process)
{
  const JSON::object& json_object = content_json.object_items();

  if (json_object.find("format-version") == json_object.end()) {
    throw corevm::frontend::file_loading_error("Missing \"format-version\" in file");
  }

  const JSON::string& format_version = json_object.at("format-version").string_value();
  const std::string format_version_str = static_cast<std::string>(format_version);

  const auto wrapper = corevm::frontend::internal::schema_repository::load_by_format_version(
    format_version_str
  );

  bytecode_loader* loader = wrapper.loader;
  const std::string& schema = loader->schema();

  // TODO: validate `target-version` field.

  const JSON schema_json = sneaker::json::parse(schema);

  try {
    sneaker::json::json_schema::validate(content_json, schema_json);
  } catch (const sneaker::json::json_validation_error& ex) {
    throw corevm::frontend::file_loading_error(
      str(
        boost::format("Invalid format in file: %s") % ex.what()
      )
    );
  }

  // Load
  loader->load(content_json, process);
}



} /* end namespace internal */


} /* end namespace frontend */


} /* end namespace corevm */


void
corevm::frontend::load(const std::string& path, corevm::runtime::process& process)
  throw(corevm::frontend::file_loading_error)
{
  std::ifstream f(path, std::ios::binary);
  std::stringstream buffer;

  try {
    f.exceptions(std::ios::failbit | std::ios::badbit);
    buffer << f.rdbuf();
    f.close();
  } catch (const std::ios_base::failure& ex) {
    throw corevm::frontend::file_loading_error(
      str(
        boost::format(
          "Error while loading file \"%s\": %s"
        ) % path % ex.what()
      )
    );
  }

  std::string content = buffer.str();

  JSON content_json;

  try {
    content_json = sneaker::json::parse(content);
  } catch (const sneaker::json::invalid_json_error& ex) {
    throw corevm::frontend::file_loading_error(
      str(
        boost::format("Error while parsing file \"%s\": %s") % path % ex.what()
      )
    );
  }

  corevm::frontend::internal::validate_and_load(content_json, process);
}
