#ifndef COREVM_GC_RULE_H_
#define COREVM_GC_RULE_H_

#include <unordered_map>
#include "common.h"


namespace corevm {


namespace runtime {


// Forward declaration of `corevm::runtime::process`.
class process;


class gc_rule {
public:
  virtual bool should_gc(const corevm::runtime::process& process) = 0;
};


class gc_rule_always : public gc_rule {
public:
  virtual bool should_gc(const corevm::runtime::process& process);
};


class gc_rule_by_heap_size : public gc_rule {
public:
  virtual bool should_gc(const corevm::runtime::process& process);
};


class gc_rule_by_ntvhndl_pool_size : public gc_rule {
public:
  virtual bool should_gc(const corevm::runtime::process& process);
};


typedef struct gc_rule_wrapper {
  corevm::runtime::gc_rule* gc_rule;
} gc_rule_wrapper;


class gc_rule_meta {
public:
  enum gc_bitfields {
    GC_ALWAYS = 1,
    GC_BY_HEAP_SIZE = 2,
    GC_BY_NTV_POOLSIZE = 3,
  };

  corevm::runtime::gc_rule* get_gc_rule(gc_bitfields bit) {
    auto itr = _gc_rule_map.find(bit);
    return itr != _gc_rule_map.end() ? itr->second.gc_rule : nullptr;
  }

private:
  std::unordered_map<gc_bitfield_t, corevm::runtime::gc_rule_wrapper> _gc_rule_map {
    {gc_bitfields::GC_ALWAYS, {.gc_rule=new corevm::runtime::gc_rule_always()}},
    {gc_bitfields::GC_BY_HEAP_SIZE, {.gc_rule=new corevm::runtime::gc_rule_by_heap_size()}},
    {gc_bitfields::GC_BY_NTV_POOLSIZE, {.gc_rule=new corevm::runtime::gc_rule_by_ntvhndl_pool_size()}}
  };
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_GC_RULE_H_ */
