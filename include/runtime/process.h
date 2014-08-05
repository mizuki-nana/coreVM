#ifndef COREVM_PROCESS_H_
#define COREVM_PROCESS_H_

#include <climits>
#include <stack>
#include <unordered_map>
#include <sneaker/threading/fixed_time_interval_daemon_service.h>
#include "frame.h"
#include "instr.h"
#include "../../include/dyobj/common.h"
#include "../../include/dyobj/dynamic_object_heap.h"
#include "../../include/gc/garbage_collector.h"
#include "../../include/gc/garbage_collection_scheme.h"


namespace corevm {


namespace runtime {


enum process_gc_bitfield {
  GC_ALWAYS = 1,
  GC_ON_HEAP_SIZE = 2,
  GC_ON_NTV_POOLSIZE = 3,
};


/* A process is a unit for executing a sequence of instructions.
 * It's supposed to have the following:
 *
 * - A sequence of instructions.
 * - Program counter.
 * - A heap for holding dynamic objects.
 * - A call stack for executing blocks of instructions.
 * - A pool of native type handles.
 * - An instance of instr handler meta class. */
class process : public sneaker::threading::fixed_time_interval_daemon_service {
public:
  using garbage_collection_scheme = typename corevm::gc::reference_count_garbage_collection_scheme;
  using dynamic_object_type = typename corevm::dyobj::dynamic_object<garbage_collection_scheme::dynamic_object_manager>;

  explicit process();
  explicit process(const uint16_t); 
  ~process();

  uint64_t call_stack_size() const;

  corevm::runtime::frame& top_frame() throw(corevm::runtime::frame_not_found_error);

  void push_frame(corevm::runtime::frame&);

  uint64_t stack_size() const;

  const corevm::dyobj::dyobj_id& top_stack() throw(corevm::runtime::object_stack_empty_error);

  const corevm::dyobj::dyobj_id pop_stack() throw(corevm::runtime::object_stack_empty_error);

  void push_stack(corevm::dyobj::dyobj_id&);

  bool has_ntvhndl(corevm::dyobj::ntvhndl_key&);

  corevm::types::native_type_handle& get_ntvhndl(corevm::dyobj::ntvhndl_key&)
    throw(corevm::runtime::native_type_handle_not_found_error);

  corevm::dyobj::ntvhndl_key insert_ntvhndl(corevm::types::native_type_handle&);

  void erase_ntvhndl(corevm::dyobj::ntvhndl_key&)
    throw(corevm::runtime::native_type_handle_deletion_error);

  void maybe_gc();

  static void tick_handler(void*);

  /* Helper functions */
  corevm::dyobj::dyobj_id __helper_create_dyobj() {
    return _dynamic_object_heap.create_dyobj();
  }

  dynamic_object_type& __helper_at(corevm::dyobj::dyobj_id id) {
    return _dynamic_object_heap.at(id);
  }

private:
  bool _should_gc() const;

  uint8_t _gc_flag;
  std::vector<corevm::runtime::instr> _instrs;
  corevm::runtime::instr_addr _pc;
  corevm::dyobj::dynamic_object_heap<garbage_collection_scheme::dynamic_object_manager> _dynamic_object_heap;
  std::stack<corevm::dyobj::dyobj_id> _dyobj_stack;
  std::stack<corevm::runtime::frame> _call_stack;
  std::unordered_map<corevm::dyobj::ntvhndl_key, corevm::types::native_type_handle> _ntv_handles_pool;
  corevm::runtime::instr_handler_meta _instr_handler_meta;
  sneaker::atomic::atomic_incrementor<corevm::dyobj::ntvhndl_key, INT_MAX> _ntv_handles_incrementor;
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_PROCESS_H_ */
