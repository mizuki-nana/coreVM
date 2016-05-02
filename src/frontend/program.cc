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
#include "program.h"

#include "configuration.h"
#include "runner.h"
#include "corevm/version.h"

#include <boost/format.hpp>


namespace corevm {


namespace frontend {


// -----------------------------------------------------------------------------

Program::Program()
  :
  sneaker::utility::cmdline_program(
    str(boost::format("coreVM v%s") % COREVM_CANONICAL_VERSION).c_str()),
  m_input_path(),
  m_config_path(),
  m_format(),
  m_log_mode(),
  m_heap_alloc_size(0),
  m_pool_alloc_size(0),
  m_gc_interval(0),
  m_gc_flag(0)
{
  add_positional_parameter("input", 1);
  add_string_parameter("input", "Input file", &m_input_path);
  add_string_parameter("config", "Configuration file", &m_config_path);
  add_string_parameter("format", "Bytecode format (binary or text)", &m_format);
  add_uint64_parameter("heap-alloc-size", "Dynamic Object Heap allocation size (bytes)", &m_heap_alloc_size);
  add_uint64_parameter("pool-alloc-size", "Native Types Pool allocation size (bytes)", &m_pool_alloc_size);
  add_uint32_parameter("gc-interval", "GC interval (ms)", &m_gc_interval);
  add_uint32_parameter("gc-flag", "GC flag", &m_gc_flag);
  add_string_parameter("logging", "Optional logging mode (i.e. stdout, stderr, file path)", &m_log_mode);
}

// -----------------------------------------------------------------------------

/* virtual */
Program::~Program()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

/* virtual */
bool
Program::check_parameters() const
{
  return !m_input_path.empty() &&
    (m_format.empty() || m_format == "text" || m_format == "binary");
}

// -----------------------------------------------------------------------------

/* virtual */
int
Program::do_run()
{
  Configuration configuration;

  if (option_provided("config"))
  {
    configuration = Configuration::load_config(m_config_path);
  }

  if (option_provided("heap-alloc-size"))
  {
    configuration.set_heap_alloc_size(m_heap_alloc_size);
  }

  if (option_provided("pool-alloc-size"))
  {
    configuration.set_pool_alloc_size(m_pool_alloc_size);
  }

  if (option_provided("gc-interval"))
  {
    configuration.set_gc_interval(m_gc_interval);
  }

  if (option_provided("gc-flag"))
  {
    configuration.set_gc_flag(static_cast<uint8_t>(m_gc_flag));
  }

  configuration.set_log_mode(m_log_mode);

  return Runner(m_input_path, configuration).run();
}

// -----------------------------------------------------------------------------


} /* end namespace frontend */


} /* end namespace corevm */
