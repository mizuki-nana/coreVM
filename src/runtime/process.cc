#include <cassert>
#include <sneaker/libc/utils.h>
#include "../../include/runtime/process.h"


#ifndef COREVM_DEFAULT_PROCESS_PAUSE_TIME
  #define COREVM_DEFAULT_PROCESS_PAUSE_TIME 100
#endif

#ifndef COREVM_DEFAULT_GC_HEAP_SIZE_CUTOFF
  #define COREVM_DEFAULT_GC_HEAP_SIZE_CUTOFF 0.75f
#endif

#ifndef COREVM_DEFAULT_GC_NTV_POOL_SIZE_CUTOFF
  #define COREVM_DEFAULT_GC_NTV_POOL_SIZE_CUTOFF 0.75f
#endif


corevm::runtime::process::process():
  sneaker::threading::fixed_time_interval_daemon_service(
    COREVM_DEFAULT_PROCESS_PAUSE_TIME, corevm::runtime::process::tick_handler, true, -1)
{
}

corevm::runtime::process::process(const uint16_t interval):
  sneaker::threading::fixed_time_interval_daemon_service(
    interval, corevm::runtime::process::tick_handler, true, -1)
{
}

corevm::runtime::process::~process()
{
}

corevm::runtime::frame&
corevm::runtime::process::top_frame()
{
  if(_call_stack.empty()) {
    throw corevm::runtime::frame_not_found_error();
  }

  return _call_stack.top();
}

void
corevm::runtime::process::maybe_gc()
{
  if(!_should_gc()) {
    return;
  }

  // TODO: halt execution...

  // GC...
  corevm::gc::garbage_collector<garbage_collection_scheme> garbage_collector(_dynamic_object_heap);
  garbage_collector.gc();

  // TODO: resume execution...
}

bool
corevm::runtime::process::_should_gc() const
{
  if(is_bit_set(_gc_flag, corevm::runtime::process_gc_bitfield::GC_ALWAYS)) {
    return true;
  } else if(is_bit_set(_gc_flag, corevm::runtime::process_gc_bitfield::GC_ON_HEAP_SIZE)) {
    return _dynamic_object_heap.size() > _dynamic_object_heap.max_size() * COREVM_DEFAULT_GC_HEAP_SIZE_CUTOFF;
  } else if(is_bit_set(_gc_flag, corevm::runtime::process_gc_bitfield::GC_ON_NTV_POOLSIZE)) {
    return _ntv_handles_pool.size() > _ntv_handles_pool.max_size() * COREVM_DEFAULT_GC_NTV_POOL_SIZE_CUTOFF;
  }

  return false;
}

void
corevm::runtime::process::tick_handler(void* arg)
{
  assert(arg);
  corevm::runtime::process* process = static_cast<corevm::runtime::process*>(arg);
  process->maybe_gc();
}
