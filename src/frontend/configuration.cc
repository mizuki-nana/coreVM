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
#include "../../include/frontend/configuration.h"

#include "../../include/frontend/errors.h"

#include <boost/format.hpp>
#include <sneaker/json/json.h>
#include <sneaker/json/json_schema.h>

#include <cassert>
#include <fstream>
#include <ios>
#include <sstream>
#include <string>
#include <utility>


using sneaker::json::JSON;


// -----------------------------------------------------------------------------

const std::string corevm::frontend::configuration::schema = \
  "{"
    "\"type\": \"object\","
    "\"properties\": {"
      "\"alloc-size\": {"
        "\"type\": \"integer\""
      "},"
      "\"gc-interval\": {"
        "\"type\": \"integer\""
      "}"
    "}"
  "}";

// -----------------------------------------------------------------------------

corevm::frontend::configuration::configuration()
  :
  m_alloc_size(0),
  m_gc_interval(0)
{
}

// -----------------------------------------------------------------------------

uint64_t
corevm::frontend::configuration::alloc_size() const
{
  return m_alloc_size;
}

// -----------------------------------------------------------------------------

uint32_t
corevm::frontend::configuration::gc_interval() const
{
  return m_gc_interval;
}

// -----------------------------------------------------------------------------

void
corevm::frontend::configuration::set_alloc_size(uint64_t alloc_size)
{
  m_alloc_size = alloc_size;
}

// -----------------------------------------------------------------------------

void
corevm::frontend::configuration::set_gc_interval(uint32_t gc_interval)
{
  m_gc_interval = gc_interval;
}

// -----------------------------------------------------------------------------

corevm::frontend::configuration
corevm::frontend::configuration::load_config(const std::string& path)
  throw(corevm::frontend::configuration_loading_error)
{
  std::ifstream fs(path, std::ios::binary);
  std::stringstream buffer;

  try
  {
    fs.exceptions(std::ios::failbit | std::ios::badbit);
    buffer << fs.rdbuf();
    fs.close();
  }
  catch (const std::ios_base::failure& ex)
  {
    throw corevm::frontend::configuration_loading_error(
      str(
        boost::format(
          "Error loading configuration file %s: %s"
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
    throw corevm::frontend::configuration_loading_error(
      str(
        boost::format(
          "Error parsing configuration file %s: %s"
        ) % path % ex.what()
      )
    );
  }

  JSON schema_json = sneaker::json::parse(configuration::schema);

  try
  {
    sneaker::json::json_schema::validate(content_json, schema_json);
  }
  catch (const sneaker::json::json_validation_error& ex)
  {
    throw corevm::frontend::configuration_loading_error(
      str(
        boost::format(
          "Invalid configuration file %s: %s"
        ) % path % ex.what()
      )
    );
  }

  corevm::frontend::configuration configuration;

  set_values(configuration, content_json);

  return std::move(configuration);
}

// -----------------------------------------------------------------------------

void
corevm::frontend::configuration::set_values(
  corevm::frontend::configuration& configuration, const JSON& config_json)
{
  assert(config_json.is_object());

  JSON::object config_obj = config_json.object_items();

  // Set alloc size.
  if (config_obj.find("alloc-size") != config_obj.end())
  {
    JSON alloc_size_raw = config_obj.at("alloc-size");
    configuration.m_alloc_size = static_cast<uint64_t>(alloc_size_raw.int_value());
  }

  // GC interval.
  if (config_obj.find("gc-interval") != config_obj.end())
  {
    JSON gc_interval_raw = config_obj.at("gc-interval");
    configuration.m_gc_interval = static_cast<uint32_t>(gc_interval_raw.int_value());
  }
}

// -----------------------------------------------------------------------------
