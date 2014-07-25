#ifndef COREVM_PROCESS_H_
#define COREVM_PROCESS_H_

#include <list>
#include <stack>
#include <sneaker/threading/fixed_time_interval_daemon_service.h>
#include "frame.h"
#include "instr.h"
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

  explicit process();
  explicit process(const uint16_t); 
  ~process();

  void maybe_gc();

  static void tick_handler(void* arg);

private:
  bool _should_gc() const;

  uint8_t _gc_flag;
  std::vector<corevm::runtime::instr> _instrs;
  corevm::runtime::instr_addr _pc;
  corevm::dyobj::dynamic_object_heap<garbage_collection_scheme::dynamic_object_manager> _dynamic_object_heap;
  std::stack<corevm::runtime::frame> _call_stack;
  std::list<corevm::types::native_type_handle> _ntv_handles_pool;
  corevm::runtime::instr_handler_meta _instr_handler_meta;
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_PROCESS_H_ */
