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
#include "../../include/frontend/loader.h"

#include "../../include/frontend/errors.h"
#include "../../include/frontend/schema_repository.h"
#include "../../include/frontend/utils.h"
#include "../../include/runtime/instr_block.h"
#include "../../include/runtime/process.h"

#include <boost/format.hpp>
#include <sneaker/json/json.h>
#include <sneaker/json/json_schema.h>

#include <cstdint>
#include <fstream>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <string>


using boost::format;
using namespace sneaker::json;
using sneaker::json::JSON;


corevm::frontend::loader::loader(const std::string& path):
  m_path(path)
{
  // Do nothing here.
}

const std::string
corevm::frontend::loader::path() const
{
  return m_path;
}

void
corevm::frontend::loader::load(corevm::runtime::process& process) throw(corevm::frontend::file_loading_error)
{
  std::ifstream f(m_path, std::ios::binary);
  std::stringstream buffer;

  try {
    f.exceptions(std::ios::failbit | std::ios::badbit);
    buffer << f.rdbuf();
    f.close();
  } catch (const std::ios_base::failure& ex) {
    throw corevm::frontend::file_loading_error(
      str(
        format(
          "Error while loading file \"%s\": %s"
        ) % m_path % ex.what()
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
        format("Error while parsing file \"%s\": %s") % m_path % ex.what()
      )
    );
  }

  this->validate(content_json);

  this->load_bytecode(content_json, process);
}

void
corevm::frontend::loader::validate(const JSON& content_json)
{
  const JSON::object& json_object = content_json.object_items();

  if(json_object.find("format-version") == json_object.end()) {
    throw corevm::frontend::file_loading_error(
      str(
        format("Missing \"format-version\" in file \"%s\"") % m_path
      )
    );
  }

  const JSON::string& format_version = json_object.at("format-version").string_value();
  const std::string format_version_str = static_cast<std::string>(format_version);

  const std::string raw_schema = corevm::frontend::schema_repository::load_by_format_version(
    format_version_str
  );

  if(raw_schema.empty()) {
    throw corevm::frontend::file_loading_error(
      str(
        format("Invalid \"format-version\" in file \"%s\"") % m_path
      )
    );
  }

  // TODO: validate `target-version` field.

  const JSON schema = sneaker::json::parse(raw_schema);

  try {
    sneaker::json::json_schema::validate(content_json, schema);
  } catch (const sneaker::json::json_validation_error& ex) {
    throw corevm::frontend::file_loading_error(
      str(
        format("Invalid format in file \"%s\": %s") % m_path % ex.what()
      )
    );
  }
}

void
corevm::frontend::loader::load_bytecode(const JSON& content_json, corevm::runtime::process& process)
{
  const JSON::object& json_object = content_json.object_items();

  const JSON::string& format = json_object.at("format").string_value();
  const JSON::string& format_version = json_object.at("format-version").string_value();
  const JSON::string& target_version = json_object.at("target-version").string_value();
  const JSON::string& encoding = json_object.at("encoding").string_value();
  const JSON::object& encoding_map = json_object.at("encoding_map").object_items();
  const JSON::object& __MAIN__ = json_object.at("__MAIN__").object_items();

  const JSON::object& object_attributes = __MAIN__.at("attributes").object_items();
  const JSON::array& object_vectors = __MAIN__.at("vector").array_items();

  // Load encoding map
  for (auto itr = encoding_map.begin(); itr != encoding_map.end(); ++itr) {
    const JSON& raw_value = static_cast<JSON>(itr->first);
    const JSON& raw_key = static_cast<JSON>(itr->second);

    // TODO: [COREVM-107] Robust validation on schema encoding map
    if (!raw_key.is_number() || !raw_value.is_string()) {
      continue;
    }

    const uint64_t key = static_cast<uint64_t>(raw_key.int_value());
    const std::string value = static_cast<std::string>(raw_value.string_value());

    process.set_encoding_key_value_pair(key, value);
  }

  // Load vectors
  for (auto itr = object_vectors.begin(); itr != object_vectors.end(); ++itr) {
    const JSON& vector_json_raw = static_cast<JSON>(*itr);
    const JSON::object& vector_object = vector_json_raw.object_items();

    const JSON::string& __name__ = vector_object.at("__name__").string_value();
    const JSON& __inner__ = vector_object.at("__inner__");

    corevm::runtime::instr_block block = corevm::frontend::get_vector_from_json(__inner__);

    process.append_instr_block(block);
  }
}
