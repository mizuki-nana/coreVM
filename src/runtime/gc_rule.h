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
#ifndef COREVM_GC_RULE_H_
#define COREVM_GC_RULE_H_

#include "common.h"

#include <cstdint>
#include <memory>


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

// Forward declaration of `Process`.
class Process;

// -----------------------------------------------------------------------------

bool gc_rule_always(const Process&);

// -----------------------------------------------------------------------------

bool gc_rule_by_heap_size(const Process&);

// -----------------------------------------------------------------------------

bool gc_rule_by_ntvhndl_pool_size(const Process&);

// -----------------------------------------------------------------------------

typedef bool (*GCRule)(const Process&);

// -----------------------------------------------------------------------------

class GCRuleMeta
{
public:
  enum GCBitfields : uint8_t
  {
    GC_ALWAYS,
    GC_BY_HEAP_SIZE,
    GC_BY_NTV_POOLSIZE,
    GC_RULE_MAX
  };

  static const uint8_t DEFAULT_GC_FLAGS;

  static GCRule gc_rules[GC_RULE_MAX];
};

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_GC_RULE_H_ */
