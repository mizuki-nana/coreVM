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

class GCRule
{
public:
  virtual bool should_gc(const Process& process) const = 0;

  virtual ~GCRule();
};

// -----------------------------------------------------------------------------

class GCRuleAlways : public GCRule
{
public:
  virtual bool should_gc(const Process& process) const;
};

// -----------------------------------------------------------------------------

class GCRuleByHeapSize : public GCRule
{
public:
  virtual bool should_gc(const Process& process) const;

  static const double DEFAULT_CUTOFF;
};

// -----------------------------------------------------------------------------

class GCRuleByNtvhndlPoolSize : public GCRule
{
public:
  virtual bool should_gc(const Process& process) const;

  static const double DEFAULT_CUTOFF;
};

// -----------------------------------------------------------------------------

typedef std::shared_ptr<GCRule> GCRulePtr;

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

  static const GCRulePtr get_gc_rule(GCBitfields bit);

  static const uint8_t DEFAULT_GC_FLAGS;

private:
  static const GCRulePtr gc_rules[GC_RULE_MAX];
};

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_GC_RULE_H_ */
