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
#include "bytecode_loader.h"

#include "bytecode_loader_v0_1.h"
#include "errors.h"
#include "utils.h"
#include "corevm/version.h"
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
#include <vector>


using sneaker::json::JSON;


namespace corevm {


namespace frontend {


namespace internal {


typedef struct bytecode_loader_wrapper
{
  bytecode_loader* loader;
} bytecode_loader_wrapper;


// -----------------------------------------------------------------------------

class schema_repository
{
public:
  static bytecode_loader* load_by_format_and_version(
    const std::string&, const std::string&);

private:
  static const std::vector<bytecode_loader_wrapper> bytecode_loader_definitions;
};

// -----------------------------------------------------------------------------

const std::vector<bytecode_loader_wrapper>
corevm::frontend::internal::schema_repository::bytecode_loader_definitions
{
  {
    .loader = new bytecode_loader_v0_1(),
  },
};

// -----------------------------------------------------------------------------

bytecode_loader*
corevm::frontend::internal::schema_repository::load_by_format_and_version(
  const std::string& format, const std::string& format_version)
{
  std::list<bytecode_loader*> loaders;

  std::for_each(
    bytecode_loader_definitions.begin(),
    bytecode_loader_definitions.end(),
    [&format, &loaders](const bytecode_loader_wrapper& wrapper) {
      if (wrapper.loader->format() == format) {
        loaders.push_back(wrapper.loader);
      }
    }
  );

  if (loaders.empty())
  {
    throw file_loading_error(
      str(
        boost::format("Unrecognized format: \"%s\"") % format
      )
    );
  }

  auto itr = std::find_if(
    loaders.begin(),
    loaders.end(),
    [&format_version](const bytecode_loader* loader) -> bool {
      return loader->version() == format_version;
    }
  );

  if (itr == loaders.end())
  {
    throw file_loading_error(
      str(
        boost::format("Unrecognized format-version: \"%s\"") % format_version
      )
    );
  }

  return static_cast<bytecode_loader*>(*itr);
}

// -----------------------------------------------------------------------------


} /* end namespace internal */


} /* end namespace frontend */


} /* end namespace corevm */


// -----------------------------------------------------------------------------

void
corevm::frontend::bytecode_loader::validate_and_load(
  const std::string& path, const JSON& content_json, corevm::runtime::process& process)
{
  const JSON::object& json_object = content_json.object_items();

  const JSON::string& target_version = json_object.at("target-version").string_value();
  const std::string target_version_str = static_cast<std::string>(target_version);

  if (target_version_str != COREVM_SHORT_CANONICAL_VERSION)
  {
    throw corevm::frontend::file_loading_error(
      str(
        boost::format("Invalid target-version: %s") % target_version_str
      )
    );
  }

  const JSON::string& format = json_object.at("format").string_value();
  const JSON::string& format_version = json_object.at("format-version").string_value();

  const std::string format_str = static_cast<std::string>(format);
  const std::string format_version_str = static_cast<std::string>(format_version);

  bytecode_loader* loader = corevm::frontend::internal::schema_repository::load_by_format_and_version(
    format_str, format_version_str
  );

  const std::string& schema = loader->schema();

  const JSON schema_json = sneaker::json::parse(schema);

  try
  {
    sneaker::json::json_schema::validate(content_json, schema_json);
  }
  catch (const sneaker::json::json_validation_error& ex)
  {
    throw corevm::frontend::file_loading_error(
      str(
        boost::format("Invalid format in file: %s") % ex.what()
      )
    );
  }

  // Load
  loader->load(path, content_json, process);
}

// -----------------------------------------------------------------------------

void
corevm::frontend::bytecode_loader::load(
  const std::string& path, corevm::runtime::process& process)
  throw(corevm::frontend::file_loading_error)
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

  try
  {
    content_json = sneaker::json::parse(content);
  }
  catch (const sneaker::json::invalid_json_error& ex)
  {
    throw corevm::frontend::file_loading_error(
      str(
        boost::format("Error while parsing file \"%s\": %s") % path % ex.what()
      )
    );
  }

  bytecode_loader::validate_and_load(path, content_json, process);
}

// -----------------------------------------------------------------------------
