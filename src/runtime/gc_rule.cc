#include "../../include/runtime/process.h"
#include "../../include/runtime/gc_rule.h"


const double corevm::runtime::gc_rule_by_heap_size::DEFAULT_CUTOFF = 0.75f;


const double corevm::runtime::gc_rule_by_ntvhndl_pool_size::DEFAULT_CUTOFF = 0.75f;


const std::unordered_map<corevm::runtime::gc_bitfield_t, corevm::runtime::gc_rule_wrapper>
corevm::runtime::gc_rule_meta::gc_rule_map = {
  {
    corevm::runtime::gc_rule_meta::gc_bitfields::GC_ALWAYS,
    { .gc_rule=new corevm::runtime::gc_rule_always() }
  },
  {
    corevm::runtime::gc_rule_meta::gc_bitfields::GC_BY_HEAP_SIZE,
    { .gc_rule=new corevm::runtime::gc_rule_by_heap_size() }
  },
  {
    corevm::runtime::gc_rule_meta::gc_bitfields::GC_BY_NTV_POOLSIZE,
    { .gc_rule=new corevm::runtime::gc_rule_by_ntvhndl_pool_size() }
  }
};


const corevm::runtime::gc_rule*
corevm::runtime::gc_rule_meta::get_gc_rule(gc_bitfields bit) {
  auto itr = corevm::runtime::gc_rule_meta::gc_rule_map.find(bit);
  return itr != corevm::runtime::gc_rule_meta::gc_rule_map.end() ? itr->second.gc_rule : nullptr;
}

bool
corevm::runtime::gc_rule_always::should_gc(
  const corevm::runtime::process& process) const
{
  return true;
}

bool
corevm::runtime::gc_rule_by_heap_size::should_gc(
  const corevm::runtime::process& process) const
{
  return process.heap_size() > (
    process.max_heap_size() * corevm::runtime::gc_rule_by_heap_size::DEFAULT_CUTOFF
  );
}

bool
corevm::runtime::gc_rule_by_ntvhndl_pool_size::should_gc(
  const corevm::runtime::process& process) const
{
  return process.ntvhndl_pool_size() > (
    process.max_ntvhndl_pool_size() * corevm::runtime::gc_rule_by_ntvhndl_pool_size::DEFAULT_CUTOFF
  );
}
