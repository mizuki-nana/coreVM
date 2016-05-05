/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Yanzheng Li

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
#include "configuration.h"

#include "errors.h"
#include "corevm/macros.h"

#include <boost/format.hpp>
#include <sneaker/json/json.h>
#include <sneaker/json/json_schema.h>

#include <fstream>
#include <ios>
#include <sstream>
#include <string>
#include <utility>


using sneaker::json::JSON;


namespace corevm {


namespace frontend {


// -----------------------------------------------------------------------------

const std::string Configuration::schema =
  "{"
    "\"type\": \"object\","
    "\"properties\": {"
      "\"heap-alloc-size\": {"
        "\"type\": \"integer\""
      "},"
      "\"pool-alloc-size\": {"
        "\"type\": \"integer\""
      "},"
      "\"gc-interval\": {"
        "\"type\": \"integer\""
      "},"
      "\"gc-flag\": {"
        "\"type\": \"integer\""
      "},"
      "\"logging\": {"
        "\"type\": \"string\""
      "}"
    "}"
  "}";

// -----------------------------------------------------------------------------

Configuration::Configuration()
  :
  m_heap_alloc_size(0u),
  m_pool_alloc_size(0u),
  m_gc_interval(0u),
  m_gc_flag(),
  m_log_mode()
{
}

// -----------------------------------------------------------------------------

uint64_t
Configuration::heap_alloc_size() const
{
  return m_heap_alloc_size;
}

// -----------------------------------------------------------------------------

uint64_t
Configuration::pool_alloc_size() const
{
  return m_pool_alloc_size;
}

// -----------------------------------------------------------------------------

uint32_t
Configuration::gc_interval() const
{
  return m_gc_interval;
}

// -----------------------------------------------------------------------------

bool
Configuration::has_gc_flag() const
{
  return m_gc_flag.is_initialized();
}

// -----------------------------------------------------------------------------

uint8_t
Configuration::gc_flag() const
{
  return m_gc_flag.get();
}

// -----------------------------------------------------------------------------

const std::string&
Configuration::log_mode() const
{
  return m_log_mode;
}

// -----------------------------------------------------------------------------

void
Configuration::set_heap_alloc_size(uint64_t heap_alloc_size)
{
  m_heap_alloc_size = heap_alloc_size;
}

// -----------------------------------------------------------------------------

void
Configuration::set_pool_alloc_size(uint64_t pool_alloc_size)
{
  m_pool_alloc_size = pool_alloc_size;
}

// -----------------------------------------------------------------------------

void
Configuration::set_gc_interval(uint32_t gc_interval)
{
  m_gc_interval = gc_interval;
}

// -----------------------------------------------------------------------------

void
Configuration::set_gc_flag(uint8_t gc_flag)
{
  m_gc_flag = gc_flag;
}

// -----------------------------------------------------------------------------

void
Configuration::set_log_mode(const std::string& log_mode)
{
  m_log_mode = log_mode;
}

// -----------------------------------------------------------------------------

Configuration
Configuration::load_config(const std::string& path)
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
    THROW(ConfigurationLoadingError(str(boost::format(
      "Error loading configuration file %s: %s") % path % ex.what()).c_str()));
  }

  std::string content = buffer.str();

  JSON content_json;

  try
  {
    content_json = sneaker::json::parse(content);
  }
  catch (const sneaker::json::invalid_json_error& ex)
  {
    THROW(ConfigurationLoadingError(str(boost::format(
      "Error parsing configuration file %s: %s") % path % ex.what()).c_str()));
  }

  JSON schema_json = sneaker::json::parse(Configuration::schema);

  try
  {
    sneaker::json::json_schema::validate(content_json, schema_json);
  }
  catch (const sneaker::json::json_validation_error& ex)
  {
    THROW(ConfigurationLoadingError(str(boost::format(
      "Invalid configuration file %s: %s") % path % ex.what()).c_str()));
  }

  Configuration configuration;

  set_values(configuration, content_json);

  return std::move(configuration);
}

// -----------------------------------------------------------------------------

void
Configuration::set_values(Configuration& configuration, const JSON& config_json)
{
#if __DEBUG__
  ASSERT(config_json.is_object());
#endif

  const JSON::object& config_obj = config_json.object_items();

  // Set heap alloc size.
  if (config_obj.find("heap-alloc-size") != config_obj.end())
  {
    const JSON& heap_alloc_size_raw = config_obj.at("heap-alloc-size");
    uint64_t heap_alloc_size =
      static_cast<uint64_t>(heap_alloc_size_raw.int_value());
    configuration.set_heap_alloc_size(heap_alloc_size);
  }

  // Set ntv hndl pool alloc size.
  if (config_obj.find("pool-alloc-size") != config_obj.end())
  {
    const JSON& pool_alloc_size_raw = config_obj.at("pool-alloc-size");
    uint64_t pool_alloc_size =
      static_cast<uint64_t>(pool_alloc_size_raw.int_value());
    configuration.set_pool_alloc_size(pool_alloc_size);
  }

  // GC interval.
  if (config_obj.find("gc-interval") != config_obj.end())
  {
    const JSON& gc_interval_raw = config_obj.at("gc-interval");
    uint32_t gc_interval =
      static_cast<uint32_t>(gc_interval_raw.int_value());
    configuration.set_gc_interval(gc_interval);
  }

  // GC flag.
  if (config_obj.find("gc-flag") != config_obj.end())
  {
    const JSON& gc_flag_raw = config_obj.at("gc-flag");
    const uint8_t gc_flag =
      static_cast<uint8_t>(gc_flag_raw.int_value());
    configuration.set_gc_flag(gc_flag);
  }

  // Log mode.
  if (config_obj.find("logging") != config_obj.end())
  {
    const JSON& log_mode_raw = config_obj.at("logging");
    const std::string log_mode =
      static_cast<std::string>(log_mode_raw.string_value());
    configuration.set_log_mode(log_mode);
  }
}

// -----------------------------------------------------------------------------


} /* end namespace frontend */


} /* end namespace corevm */
