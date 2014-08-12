#ifndef COREVM_PROCESS_H_
#define COREVM_PROCESS_H_

#include <climits>
#include <stack>
#include <unordered_map>
#include <sneaker/threading/fixed_time_interval_daemon_service.h>
#include "frame.h"
#include "gc_rule.h"
#include "instr.h"
#include "instr_block.h"
#include "../../include/dyobj/common.h"
#include "../../include/dyobj/dynamic_object_heap.h"
#include "../../include/gc/garbage_collector.h"
#include "../../include/gc/garbage_collection_scheme.h"


namespace corevm {


namespace runtime {


/* A process is a unit for executing a sequence of instructions.
 * It's supposed to have the following:
 *
 * - A flag for pause/resume execution.
 * - A flag for GC.
 * - A sequence of instructions.
 * - A sequence of instruction blocks.
 * - A program counter.
 * - A heap for holding dynamic objects.
 * - A call stack for executing blocks of instructions.
 * - A pool of native type handles.
 * - An incrementor for native handle IDs.
 * - An instance of instr handler meta class. */
class process : public sneaker::threading::fixed_time_interval_daemon_service {
public:
  using garbage_collection_scheme = typename corevm::gc::reference_count_garbage_collection_scheme;
  using dynamic_object_type = typename corevm::dyobj::dynamic_object<garbage_collection_scheme::dynamic_object_manager>;
  using dynamic_object_heap_type = typename corevm::dyobj::dynamic_object_heap<garbage_collection_scheme::dynamic_object_manager>;
  using native_handles_pool_type = typename std::unordered_map<corevm::dyobj::ntvhndl_key, corevm::types::native_type_handle>;

  explicit process();
  explicit process(const uint16_t); 
  ~process();

  const corevm::runtime::instr_addr top_addr() const;

  const corevm::runtime::instr_addr current_addr() const;

  uint64_t call_stack_size() const;

  bool has_frame() const;

  corevm::runtime::frame& top_frame() throw(corevm::runtime::frame_not_found_error);

  void push_frame(corevm::runtime::frame&);

  void pop_frame() throw(corevm::runtime::frame_not_found_error);

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

  void set_pc(const corevm::runtime::instr_addr) throw(corevm::runtime::invalid_instr_addr_error);

  void append_instrs(const std::vector<corevm::runtime::instr>&);

  void append_instr_block(const corevm::runtime::instr_block&);

  virtual bool start();

  void maybe_gc();

  void pause_exec();
  void resume_exec();

  const corevm::runtime::instr_handler* get_instr_handler(corevm::runtime::instr_code);

  /* Accessors */
  dynamic_object_heap_type::size_type heap_size() const {
    return _dynamic_object_heap.size();
  }

  dynamic_object_heap_type::size_type max_heap_size() const {
    return _dynamic_object_heap.max_size();
  }

  native_handles_pool_type::size_type ntvhndl_pool_size() const {
    return _ntv_handles_pool.size();
  }

  native_handles_pool_type::size_type max_ntvhndl_pool_size() const {
    return _ntv_handles_pool.size();
  }

  /* Helper functions */
  corevm::dyobj::dyobj_id __helper_create_dyobj() {
    return _dynamic_object_heap.create_dyobj();
  }

  dynamic_object_type& __helper_at(corevm::dyobj::dyobj_id id) {
    return _dynamic_object_heap.at(id);
  }

  /* signal handlers */
  // Arithmetic and execution signals
  void handle_SIGFPE();
  void handle_SIGILL();
  void handle_SIGSEGV();
  // Termination signals
  void handle_SIGABRT();
  void handle_SIGINT();
  void handle_SIGTERM();
  void handle_SIGQUIT();
  // Alarm signals
  void handle_SIGALRM();
  void handle_SIGVTALRM();
  void handle_SIGPROF();
  // Operation error signals
  void handle_SIGPIPE();
  void handle_SIGXCPU();
  void handle_SIGXFSZ();
  // Asynchronous I/O signals
  void handle_SIGIO();
  void handle_SIGURG();

private:
  static void tick_handler(void*);

  bool _should_gc();

  bool _pause_exec = false;
  uint8_t _gc_flag = 0;
  corevm::runtime::gc_rule_meta _gc_rule_meta;
  std::vector<corevm::runtime::instr> _instrs;
  std::vector<corevm::runtime::instr_block> _instr_blocks;
  corevm::runtime::instr_addr _pc = 0; // corevm::runtime::NONESET_INSTR_ADDR;
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
