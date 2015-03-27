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
#include "signal_vector_loader.h"

#include "errors.h"
#include "utils.h"
#include "runtime/process.h"
#include "runtime/sighandler_registrar.h"
#include "runtime/vector.h"

#include <boost/format.hpp>
#include <sneaker/json/json.h>
#include <sneaker/json/json_schema.h>

#include <csignal>
#include <fstream>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <string>


using namespace sneaker::json;

// -----------------------------------------------------------------------------

const std::string
corevm::frontend::signal_vector_loader::schema() const
{
  static const std::string unformatted_def(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"signals\": {"
          "\"type\": \"object\","
          "\"properties\": {"

              /* Arithmetic and execution signals */

              "\"SIGFPE\": {"
                "\"$ref\": \"#/definitions/vector\""
              "},"
              "\"SIGKILL\": {"
                "\"$ref\": \"#/definitions/vector\""
              "},"
              "\"SIGSEGV\": {"
                "\"$ref\": \"#/definitions/vector\""
              "},"
              "\"SIGBUS\": {"
                "\"$ref\": \"#/definitions/vector\""
              "},"

              /* Termination signals */

              "\"SIGABRT\": {"
                "\"$ref\": \"#/definitions/vector\""
              "},"
              "\"SIGINT\": {"
                "\"$ref\": \"#/definitions/vector\""
              "},"
              "\"SIGTERM\": {"
                "\"$ref\": \"#/definitions/vector\""
              "},"
              "\"SIGQUIT\": {"
                "\"$ref\": \"#/definitions/vector\""
              "},"

              /* Alarm signals */

              "\"SIGALRM\": {"
                "\"$ref\": \"#/definitions/vector\""
              "},"
              "\"SIGVTALRM\": {"
                "\"$ref\": \"#/definitions/vector\""
              "},"
              "\"SIGPROF\": {"
                "\"$ref\": \"#/definitions/vector\""
              "},"

              /* Operation error signals */

              "\"SIGPIPE\": {"
                "\"$ref\": \"#/definitions/vector\""
              "},"
              "\"SIGXCPU\": {"
                "\"$ref\": \"#/definitions/vector\""
              "},"
              "\"SIGXFSZ\": {"
                "\"$ref\": \"#/definitions/vector\""
              "},"

              /* Asynchronous I/O signals */

              "\"SIGIO\": {"
                "\"$ref\": \"#/definitions/vector\""
              "},"
              "\"SIGURG\": {"
                "\"$ref\": \"#/definitions/vector\""
              "}"

          "},"
          "\"additionalProperties\": false"
        "}"
      "},"
      "\"definitions\": {"
        "\"instr\": {"
          "\"code\": %1%,"
          "\"oprd\": %2%"
        "},"
        "\"vector\": %3%"
      "}"
    "}"
  );

  const std::string def(
    str(
      boost::format(unformatted_def)
        % get_v0_1_instr_code_schema_definition()
        % get_v0_1_instr_oprd_schema_definition()
        % get_v0_1_vector_schema_definition()
    )
  );

  return def;
}

// -----------------------------------------------------------------------------

corevm::frontend::signal_vector_loader::signal_vector_loader(
  const std::string& path)
  :
  m_path(path)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

void
corevm::frontend::signal_vector_loader::load(corevm::runtime::process& process)
  throw(corevm::frontend::file_loading_error)
{
  std::ifstream f(m_path, std::ios::binary);
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
        ) % m_path % ex.what()
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
        boost::format(
          "Error while parsing file \"%s\": %s"
        ) % m_path % ex.what()
      )
    );
  }

  this->validate(content_json);

  const JSON::object json_object = content_json.object_items();
  const JSON::object& signals_object = json_object.at("signals").object_items();

  for (auto itr = signals_object.begin(); itr != signals_object.end(); ++itr)
  {
    std::string signal_str = static_cast<std::string>(itr->first);
    const JSON& signal_json = static_cast<const JSON>(itr->second);

    corevm::runtime::vector vector = \
      corevm::frontend::get_vector_from_json(signal_json);

    sig_atomic_t sig = \
      corevm::runtime::sighandler_registrar::get_sig_value_from_string(signal_str);

    process.set_sig_vector(sig, vector);
  }
}

// -----------------------------------------------------------------------------

void
corevm::frontend::signal_vector_loader::validate(const JSON& content_json)
{
  const JSON schema = sneaker::json::parse(this->schema());

  try
  {
    sneaker::json::json_schema::validate(content_json, schema);
  }
  catch (const sneaker::json::json_validation_error& ex)
  {
    throw corevm::frontend::file_loading_error(
      str(
        boost::format("Invalid format in file \"%s\": %s") % m_path % ex.what()
      )
    );
  }
}

// -----------------------------------------------------------------------------
