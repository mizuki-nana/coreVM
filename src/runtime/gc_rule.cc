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


namespace corevm {
namespace runtime {

// -----------------------------------------------------------------------------

const uint8_t GCRuleMeta::DEFAULT_GC_FLAGS = 1 << GCRuleMeta::GC_ALWAYS;

// -----------------------------------------------------------------------------

GCRule
GCRuleMeta::gc_rules[GC_RULE_MAX] {
  gc_rule_always,
  gc_rule_by_heap_size,
  gc_rule_by_ntvhndl_pool_size
};

// -----------------------------------------------------------------------------

bool
gc_rule_always(const Process& /* process */)
{
  return true;
}

// -----------------------------------------------------------------------------

bool
gc_rule_by_heap_size(const Process& process)
{
  static const double GC_RULE_BY_HEAP_SIZE_DEFAULT_CUTOFF = 0.75f;
  return process.heap_size() > (
    process.max_heap_size() * GC_RULE_BY_HEAP_SIZE_DEFAULT_CUTOFF
  );
}

// -----------------------------------------------------------------------------

bool
gc_rule_by_ntvhndl_pool_size(const Process& process)
{
  static const double GC_RULE_BY_NTVHNDL_POOL_SIZE_DEFAULT_CUTOFF = 0.75f;
  return process.ntvhndl_pool_size() > (
    process.max_ntvhndl_pool_size() * GC_RULE_BY_NTVHNDL_POOL_SIZE_DEFAULT_CUTOFF
  );
}

// -----------------------------------------------------------------------------

} /* end namespace runtime */
} /* end namespace corevm */
