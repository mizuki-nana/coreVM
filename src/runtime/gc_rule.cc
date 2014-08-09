#include "../../include/runtime/process.h"
#include "../../include/runtime/gc_rule.h"

#ifndef COREVM_DEFAULT_GC_HEAP_SIZE_CUTOFF
  #define COREVM_DEFAULT_GC_HEAP_SIZE_CUTOFF 0.75f
#endif

#ifndef COREVM_DEFAULT_GC_NTV_POOL_SIZE_CUTOFF
  #define COREVM_DEFAULT_GC_NTV_POOL_SIZE_CUTOFF 0.75f
#endif


bool
corevm::runtime::gc_rule_always::should_gc(const corevm::runtime::process& process)
{
  return true;
}

bool
corevm::runtime::gc_rule_by_heap_size::should_gc(const corevm::runtime::process& process)
{
  return process.heap_size() > process.max_heap_size() * COREVM_DEFAULT_GC_HEAP_SIZE_CUTOFF;
}

bool
corevm::runtime::gc_rule_by_ntvhndl_pool_size::should_gc(const corevm::runtime::process& process)
{
  return process.ntvhndl_pool_size() > process.max_ntvhndl_pool_size() * COREVM_DEFAULT_GC_NTV_POOL_SIZE_CUTOFF;
}
