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
#include "gc_rule.h"

#include "process.h"

#include <memory>


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

const uint8_t gc_rule_meta::DEFAULT_GC_FLAGS = 1 << gc_rule_meta::GC_ALWAYS;

// -----------------------------------------------------------------------------

const double gc_rule_by_heap_size::DEFAULT_CUTOFF = 0.75f;

// -----------------------------------------------------------------------------

const double gc_rule_by_ntvhndl_pool_size::DEFAULT_CUTOFF = 0.75f;

// -----------------------------------------------------------------------------

/* virtual */
gc_rule::~gc_rule()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

const gc_rule_ptr
gc_rule_meta::gc_rules[GC_RULE_MAX] {
  std::make_shared<gc_rule_always>(),
  std::make_shared<gc_rule_by_heap_size>(),
  std::make_shared<gc_rule_by_ntvhndl_pool_size>()
};

// -----------------------------------------------------------------------------

const gc_rule_ptr
gc_rule_meta::get_gc_rule(gc_bitfields bit)
{
  const uint8_t uint8_bit = static_cast<uint8_t>(bit);

  gc_rule_ptr gc_rule;

  if (uint8_bit < GC_RULE_MAX)
  {
    gc_rule = gc_rule_meta::gc_rules[uint8_bit];
  }

  return gc_rule;
}

// -----------------------------------------------------------------------------

bool
gc_rule_always::should_gc(
  const process& /* process */) const
{
  return true;
}

// -----------------------------------------------------------------------------

bool
gc_rule_by_heap_size::should_gc(
  const process& process) const
{
  return process.heap_size() > (
    process.max_heap_size() * gc_rule_by_heap_size::DEFAULT_CUTOFF
  );
}

// -----------------------------------------------------------------------------

bool
gc_rule_by_ntvhndl_pool_size::should_gc(
  const process& process) const
{
  return process.ntvhndl_pool_size() > (
    process.max_ntvhndl_pool_size() * gc_rule_by_ntvhndl_pool_size::DEFAULT_CUTOFF
  );
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */
