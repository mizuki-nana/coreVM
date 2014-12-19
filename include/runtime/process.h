/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2014 Yanzheng Li

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
#ifndef COREVM_PROCESS_H_
#define COREVM_PROCESS_H_

#include <climits>
#include <stack>
#include <unordered_map>
#include <sneaker/threading/fixed_time_interval_daemon_service.h>
#include "errors.h"
#include "frame.h"
#include "instr.h"
#include "instr_block.h"
#include "native_types_pool.h"
#include "sighandler.h"
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
 * */
class process : public sneaker::threading::fixed_time_interval_daemon_service {

public:
  typedef corevm::gc::reference_count_garbage_collection_scheme garbage_collection_scheme;
  using dynamic_object_type = typename corevm::dyobj::dynamic_object<garbage_collection_scheme::dynamic_object_manager>;
  using dynamic_object_heap_type = typename corevm::dyobj::dynamic_object_heap<garbage_collection_scheme::dynamic_object_manager>;
  typedef corevm::runtime::native_types_pool native_types_pool_type;

  class adapter {
  public:
    explicit adapter(corevm::runtime::process& process):
      m_process(process)
    {
    }

    corevm::dyobj::dyobj_id help_create_dyobj();

    dynamic_object_type& help_get_dyobj(corevm::dyobj::dyobj_id id);

  private:
    corevm::runtime::process& m_process;
  };

  friend class adapter;

public:
  explicit process();
  explicit process(const uint16_t);
  ~process();

  const corevm::runtime::instr_addr top_addr() const;

  const corevm::runtime::instr_addr current_addr() const;

  uint64_t call_stack_size() const;

  bool has_frame() const;

  corevm::runtime::frame& top_frame()
    throw(corevm::runtime::frame_not_found_error);

  void push_frame(corevm::runtime::frame&);

  void pop_frame() throw(corevm::runtime::frame_not_found_error);

  uint64_t stack_size() const;

  const corevm::dyobj::dyobj_id& top_stack()
    throw(corevm::runtime::object_stack_empty_error);

  const corevm::dyobj::dyobj_id pop_stack()
    throw(corevm::runtime::object_stack_empty_error);

  void push_stack(corevm::dyobj::dyobj_id&);

  bool has_ntvhndl(corevm::dyobj::ntvhndl_key&);

  corevm::types::native_type_handle& get_ntvhndl(corevm::dyobj::ntvhndl_key&)
    throw(corevm::runtime::native_type_handle_not_found_error);

  corevm::dyobj::ntvhndl_key insert_ntvhndl(corevm::types::native_type_handle&)
    throw(corevm::runtime::native_type_handle_insertion_error);

  void erase_ntvhndl(corevm::dyobj::ntvhndl_key&)
    throw(corevm::runtime::native_type_handle_deletion_error);

  void set_pc(const corevm::runtime::instr_addr)
    throw(corevm::runtime::invalid_instr_addr_error);

  void append_instrs(const std::vector<corevm::runtime::instr>&);

  void append_instr_block(const corevm::runtime::instr_block&);

  virtual bool start();

  void maybe_gc();

  bool can_execute();

  void pause_exec();

  void resume_exec();

  const corevm::runtime::instr_handler* get_instr_handler(corevm::runtime::instr_code);

  void set_sig_instr_block(sig_atomic_t, corevm::runtime::instr_block&);

  void handle_signal(sig_atomic_t, corevm::runtime::sighandler*);

  dynamic_object_heap_type::size_type heap_size() const;

  dynamic_object_heap_type::size_type max_heap_size() const;

  native_types_pool_type::size_type ntvhndl_pool_size() const;

  native_types_pool_type::size_type max_ntvhndl_pool_size() const;

private:
  static void tick_handler(void*);

  bool should_gc();

  void insert_instr_block(corevm::runtime::instr_block& block);

  bool m_pause_exec;
  uint8_t m_gc_flag;
  corevm::runtime::instr_addr m_pc;
  std::vector<corevm::runtime::instr> m_instrs;
  std::vector<corevm::runtime::instr_block> m_instr_blocks;
  corevm::dyobj::dynamic_object_heap<garbage_collection_scheme::dynamic_object_manager> m_dynamic_object_heap;
  std::stack<corevm::dyobj::dyobj_id> m_dyobj_stack;
  std::stack<corevm::runtime::frame> m_call_stack;
  native_types_pool_type m_ntvhndl_pool;
  corevm::runtime::instr_handler_meta m_instr_handler_meta;
  std::unordered_map<sig_atomic_t, corevm::runtime::instr_block> m_sig_instr_map;
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_PROCESS_H_ */
