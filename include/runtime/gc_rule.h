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
  virtual bool should_gc(const corevm::runtime::process& process) const = 0;
};


class gc_rule_always : public gc_rule {
public:
  virtual bool should_gc(const corevm::runtime::process& process) const;
};


class gc_rule_by_heap_size : public gc_rule {
public:
  virtual bool should_gc(const corevm::runtime::process& process) const;

  static const double DEFAULT_CUTOFF;
};


class gc_rule_by_ntvhndl_pool_size : public gc_rule {
public:
  virtual bool should_gc(const corevm::runtime::process& process) const;

  static const double DEFAULT_CUTOFF;
};


typedef struct gc_rule_wrapper {
  const corevm::runtime::gc_rule* gc_rule;
} gc_rule_wrapper;


class gc_rule_meta {
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



} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_GC_RULE_H_ */
