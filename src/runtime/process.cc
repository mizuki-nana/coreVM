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

uint64_t
corevm::runtime::process::call_stack_size() const
{
  return _call_stack.size();
}

corevm::runtime::frame&
corevm::runtime::process::top_frame() throw(corevm::runtime::frame_not_found_error)
{
  if(_call_stack.empty()) {
    throw corevm::runtime::frame_not_found_error();
  }

  return _call_stack.top();
}

void
corevm::runtime::process::push_frame(corevm::runtime::frame& frame)
{
  _call_stack.push(frame);
}

uint64_t
corevm::runtime::process::stack_size() const
{
  return _dyobj_stack.size();
}

const corevm::dyobj::dyobj_id&
corevm::runtime::process::top_stack() throw(corevm::runtime::object_stack_empty_error)
{
  if(_dyobj_stack.empty()) {
    throw corevm::runtime::object_stack_empty_error();
  }

  return _dyobj_stack.top();
}

void
corevm::runtime::process::push_stack(corevm::dyobj::dyobj_id& id)
{
  _dyobj_stack.push(id);
}

const corevm::dyobj::dyobj_id
corevm::runtime::process::pop_stack() throw(corevm::runtime::object_stack_empty_error)
{
  if(_dyobj_stack.empty()) {
    throw corevm::runtime::object_stack_empty_error();
  }

  corevm::dyobj::dyobj_id id = _dyobj_stack.top();
  _dyobj_stack.pop();
  return id;
}

bool
corevm::runtime::process::has_ntvhndl(corevm::dyobj::ntvhndl_key& key)
{
  return _ntv_handles_pool.find(key) != _ntv_handles_pool.end();
}

corevm::types::native_type_handle&
corevm::runtime::process::get_ntvhndl(corevm::dyobj::ntvhndl_key& key)
  throw(corevm::runtime::native_type_handle_not_found_error)
{
  try {
    return _ntv_handles_pool.at(key);  
  } catch (...)  {// TODO: should catch `std::out_of_range` here.
    throw corevm::runtime::native_type_handle_not_found_error();
  }
}

corevm::dyobj::ntvhndl_key
corevm::runtime::process::insert_ntvhndl(corevm::types::native_type_handle& hndl)
{
  corevm::dyobj::ntvhndl_key key = ++_ntv_handles_incrementor;

  // TODO: not sure if we want to do .insert here or simply use the access operator...
  _ntv_handles_pool.insert(
    std::pair<corevm::dyobj::ntvhndl_key, corevm::types::native_type_handle>(key, hndl)
  );

  return key;
}

void
corevm::runtime::process::erase_ntvhndl(corevm::dyobj::ntvhndl_key& key)
  throw(corevm::runtime::native_type_handle_deletion_error)
{
  size_t res = _ntv_handles_pool.erase(key);

  if(res == 0) {
    throw corevm::runtime::native_type_handle_deletion_error();
  }
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
