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
#include "gc_rule.h"

#include "process.h"

#include <memory>


const double corevm::runtime::gc_rule_by_heap_size::DEFAULT_CUTOFF = 0.75f;


// -----------------------------------------------------------------------------

const double corevm::runtime::gc_rule_by_ntvhndl_pool_size::DEFAULT_CUTOFF = 0.75f;

// -----------------------------------------------------------------------------

const std::unordered_map<corevm::runtime::gc_bitfield_t, corevm::runtime::gc_rule_wrapper>
corevm::runtime::gc_rule_meta::gc_rule_map {
  {
    corevm::runtime::gc_rule_meta::gc_bitfields::GC_ALWAYS,
    {
      .gc_rule=std::make_shared<corevm::runtime::gc_rule_always>()
    }
  },
  {
    corevm::runtime::gc_rule_meta::gc_bitfields::GC_BY_HEAP_SIZE,
    {
      .gc_rule=std::make_shared<corevm::runtime::gc_rule_by_heap_size>()
    }
  },
  {
    corevm::runtime::gc_rule_meta::gc_bitfields::GC_BY_NTV_POOLSIZE,
    {
      .gc_rule=std::make_shared<corevm::runtime::gc_rule_by_ntvhndl_pool_size>()
    }
  }
};

// -----------------------------------------------------------------------------

const corevm::runtime::gc_rule*
corevm::runtime::gc_rule_meta::get_gc_rule(gc_bitfields bit)
{
  auto itr = corevm::runtime::gc_rule_meta::gc_rule_map.find(bit);
  return itr != corevm::runtime::gc_rule_meta::gc_rule_map.end() ? itr->second.gc_rule.get() : nullptr;
}

// -----------------------------------------------------------------------------

bool
corevm::runtime::gc_rule_always::should_gc(
  const corevm::runtime::process& process) const
{
  return true;
}

// -----------------------------------------------------------------------------

bool
corevm::runtime::gc_rule_by_heap_size::should_gc(
  const corevm::runtime::process& process) const
{
  return process.heap_size() > (
    process.max_heap_size() * corevm::runtime::gc_rule_by_heap_size::DEFAULT_CUTOFF
  );
}

// -----------------------------------------------------------------------------

bool
corevm::runtime::gc_rule_by_ntvhndl_pool_size::should_gc(
  const corevm::runtime::process& process) const
{
  return process.ntvhndl_pool_size() > (
    process.max_ntvhndl_pool_size() * corevm::runtime::gc_rule_by_ntvhndl_pool_size::DEFAULT_CUTOFF
  );
}

// -----------------------------------------------------------------------------
