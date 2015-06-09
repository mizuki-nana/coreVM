/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Yanzheng Li

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

#include "compartment.h"
#include "closure.h"
#include "closure_ctx.h"
#include "common.h"
#include "errors.h"
#include "frame.h"
#include "instr.h"
#include "invocation_ctx.h"
#include "native_types_pool.h"
#include "sighandler.h"
#include "vector.h"
#include "dyobj/common.h"
#include "dyobj/dynamic_object_heap.h"
#include "gc/garbage_collector.h"
#include "gc/reference_count_garbage_collection_scheme.h"

#include <climits>
#include <cstdint>
#include <ostream>
#include <type_traits>
#include <unordered_map>
#include <vector>


namespace corevm {


namespace runtime {


/**
 * A process is a unit for executing a sequence of instructions.
 * It's supposed to have the following:
 *
 * - A flag for pause/resume execution.
 * - A flag for GC.
 * - A sequence of instructions.
 * - A sequence of instruction blocks.
 * - A program counter.
 * - A heap for holding dynamic objects.
 * - A call stack for executing blocks of instructions.
 * - A stack of invocation contexts.
 * - A pool of native type handles.
 * - A set of compartments.
 */
class process
{
public:
  typedef corevm::gc::reference_count_garbage_collection_scheme garbage_collection_scheme;
  using dynamic_object_type = typename corevm::dyobj::dynamic_object<garbage_collection_scheme::dynamic_object_manager>;
  using dynamic_object_heap_type = typename corevm::dyobj::dynamic_object_heap<garbage_collection_scheme::dynamic_object_manager>;
  typedef corevm::runtime::native_types_pool native_types_pool_type;

public:
  process();
  explicit process(uint64_t, uint64_t);
  ~process();

  /* Processes should not be copyable. */
  process(const process&) = delete;
  process& operator=(const process&) = delete;

  corevm::dyobj::dyobj_id create_dyobj();

  dynamic_object_type& get_dyobj(corevm::dyobj::dyobj_id id);

  uint64_t call_stack_size() const;

  bool has_frame() const;

  corevm::runtime::frame& top_frame()
    throw(corevm::runtime::frame_not_found_error);

  void push_frame(corevm::runtime::frame&);

  void emplace_frame(
    const corevm::runtime::closure_ctx&,
    corevm::runtime::compartment*, corevm::runtime::closure*);

  void emplace_frame(
    const corevm::runtime::closure_ctx&,
    corevm::runtime::compartment*,
    corevm::runtime::closure*, corevm::runtime::instr_addr);

  void pop_frame() throw(corevm::runtime::frame_not_found_error);

  uint64_t stack_size() const;

  corevm::runtime::invocation_ctx& top_invocation_ctx()
    throw(corevm::runtime::invocation_ctx_not_found_error);

  void push_invocation_ctx(const invocation_ctx&);

  void emplace_invocation_ctx(
    const corevm::runtime::closure_ctx&,
    corevm::runtime::compartment*, corevm::runtime::closure*);

  void pop_invocation_ctx()
    throw(corevm::runtime::invocation_ctx_not_found_error);

  const corevm::dyobj::dyobj_id& top_stack()
    throw(corevm::runtime::object_stack_empty_error);

  const corevm::dyobj::dyobj_id pop_stack()
    throw(corevm::runtime::object_stack_empty_error);

  void swap_stack();

  void push_stack(corevm::dyobj::dyobj_id&);

  bool has_ntvhndl(corevm::dyobj::ntvhndl_key&);

  corevm::types::native_type_handle& get_ntvhndl(corevm::dyobj::ntvhndl_key)
    throw(corevm::runtime::native_type_handle_not_found_error);

  corevm::dyobj::ntvhndl_key insert_ntvhndl(corevm::types::native_type_handle&)
    throw(corevm::runtime::native_type_handle_insertion_error);

  void erase_ntvhndl(corevm::dyobj::ntvhndl_key)
    throw(corevm::runtime::native_type_handle_deletion_error);

  const corevm::runtime::instr_addr pc() const;

  void set_pc(const corevm::runtime::instr_addr)
    throw(corevm::runtime::invalid_instr_addr_error);

  void append_vector(const corevm::runtime::vector&);

  void insert_vector(const corevm::runtime::vector& vector);

  bool get_frame_by_closure_ctx(
    corevm::runtime::closure_ctx&, corevm::runtime::frame**);

  void start();

  void maybe_gc();

  void do_gc();

  bool can_execute();

  void pause_exec();

  void resume_exec();

  void set_sig_vector(sig_atomic_t, corevm::runtime::vector&);

  void handle_signal(sig_atomic_t, corevm::runtime::sighandler*);

  dynamic_object_heap_type::size_type heap_size() const;

  dynamic_object_heap_type::size_type max_heap_size() const;

  native_types_pool_type::size_type ntvhndl_pool_size() const;

  native_types_pool_type::size_type max_ntvhndl_pool_size() const;

  corevm::runtime::compartment_id insert_compartment(
    const corevm::runtime::compartment&);

  void get_compartment(
    corevm::runtime::compartment_id, corevm::runtime::compartment**);

  void reset();

  /**
   * Given a starting closure context, find the existing frame associated with
   * it, following the closure tree.
   *
   * Returns a pointer that points to the frame, if found.
   * Returns a null pointer otherwise.
   */
  static
  corevm::runtime::frame* find_frame_by_ctx(
    corevm::runtime::closure_ctx ctx,
    corevm::runtime::compartment* compartment,
    corevm::runtime::process& process);

  /**
   * Given a pointer to a starting frame, find the existing frame associated
   * with the parent of the given frame's closure context.
   *
   * Returns a pointer that points to the frame, if found.
   * Returns a null pointer otherwise.
   */
  static
  corevm::runtime::frame* find_parent_frame_in_process(
    corevm::runtime::frame* frame_ptr,
    corevm::runtime::process& process);

  // NOTE: Once the stack is unwinded, the action cannot be undone.
  static void unwind_stack(
    corevm::runtime::process&, size_t limit=COREVM_DEFAULT_STACK_UNWIND_COUNT);

  friend std::ostream& operator<<(std::ostream&, const corevm::runtime::process&);

private:
  bool is_valid_pc() const;

  bool pre_start();

  bool should_gc() const;

  void check_call_stack_capacity();

  void check_invk_ctx_stack_capacity();

  bool m_pause_exec;
  uint8_t m_gc_flag;
  corevm::runtime::instr_addr m_pc;
  corevm::runtime::vector m_instrs;
  corevm::dyobj::dynamic_object_heap<garbage_collection_scheme::dynamic_object_manager> m_dynamic_object_heap;
  std::vector<corevm::dyobj::dyobj_id> m_dyobj_stack;
  std::vector<corevm::runtime::frame> m_call_stack;
  std::vector<invocation_ctx> m_invocation_ctx_stack;
  native_types_pool_type m_ntvhndl_pool;
  std::unordered_map<sig_atomic_t, corevm::runtime::vector> m_sig_instr_map;
  std::vector<corevm::runtime::compartment> m_compartments;

  static_assert(
    std::numeric_limits<corevm::runtime::vector::size_type>::max() >=
    std::numeric_limits<corevm::runtime::instr_addr>::max(),
    "Vector size incompatibility"
  );

  static_assert(
    std::numeric_limits<std::vector<corevm::runtime::compartment>::size_type>::max() >=
    std::numeric_limits<corevm::runtime::compartment_id>::max(),
    "Compartment ID incompatibility"
  );
};

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_PROCESS_H_ */
