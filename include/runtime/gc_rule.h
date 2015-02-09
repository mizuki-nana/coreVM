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
#ifndef COREVM_GC_RULE_H_
#define COREVM_GC_RULE_H_

#include "common.h"

#include <cstdint>
#include <memory>
#include <unordered_map>


namespace corevm {


namespace runtime {


// Forward declaration of `corevm::runtime::process`.
class process;

// -----------------------------------------------------------------------------

class gc_rule
{
public:
  virtual bool should_gc(const corevm::runtime::process& process) const = 0;
};

// -----------------------------------------------------------------------------

class gc_rule_always : public gc_rule
{
public:
  virtual bool should_gc(const corevm::runtime::process& process) const;
};

// -----------------------------------------------------------------------------

class gc_rule_by_heap_size : public gc_rule
{
public:
  virtual bool should_gc(const corevm::runtime::process& process) const;

  static const double DEFAULT_CUTOFF;
};

// -----------------------------------------------------------------------------

class gc_rule_by_ntvhndl_pool_size : public gc_rule
{
public:
  virtual bool should_gc(const corevm::runtime::process& process) const;

  static const double DEFAULT_CUTOFF;
};

// -----------------------------------------------------------------------------

typedef struct gc_rule_wrapper
{
  const std::shared_ptr<corevm::runtime::gc_rule> gc_rule;
} gc_rule_wrapper;

// -----------------------------------------------------------------------------

class gc_rule_meta
{
public:
  enum gc_bitfields : uint8_t {
    GC_ALWAYS = 1,
    GC_BY_HEAP_SIZE = 2,
    GC_BY_NTV_POOLSIZE = 3,
  };

  static const corevm::runtime::gc_rule* get_gc_rule(gc_bitfields bit);

private:
  static const std::unordered_map<corevm::runtime::gc_bitfield_t, corevm::runtime::gc_rule_wrapper> gc_rule_map;
};

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_GC_RULE_H_ */
