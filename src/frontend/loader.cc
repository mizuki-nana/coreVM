/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2014 Yanzheng Li

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
#include <fstream>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <string>
#include <boost/format.hpp>
#include <sneaker/json/json.h>
#include <sneaker/json/json_schema.h>
#include "../../include/frontend/errors.h"
#include "../../include/frontend/loader.h"
#include "../../include/frontend/schema_repository.h"


using boost::format;
using namespace sneaker::json;


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

corevm::frontend::bytecode_runner*
corevm::frontend::loader::load() throw(corevm::frontend::file_loading_error)
{
  corevm::frontend::bytecode_runner* runner = nullptr;

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

  // TODO: [COREVM-55] Design and implement bytecode loading mechanism
  runner = new bytecode_runner();

  return runner;
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
