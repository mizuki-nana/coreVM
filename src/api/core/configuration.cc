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


namespace corevm {


namespace api {


namespace core {


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


} /* end namespace core */


} /* end namespace api */


} /* end namespace corevm */
