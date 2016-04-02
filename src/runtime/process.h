/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Yanzheng Li

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
#include "common.h"
#include "errors.h"
#include "frame.h"
#include "gc_rule.h"
#include "instr.h"
#include "invocation_ctx.h"
#include "native_types_pool.h"
#include "sighandler.h"
#include "vector.h"
#include "dyobj/common.h"
#include "dyobj/dynamic_object_heap.h"
#include "gc/reference_count_garbage_collection_scheme.h"

#include <cstdint>
#include <type_traits>
#include <unordered_map>
#include <vector>


namespace corevm {


namespace runtime {


/** Forward declaration of `Closure` */
struct Closure;


/** Forward declaration of `ClosureCtx` */
struct ClosureCtx;


/** Forward declaration of `ProcessPrinter` */
class ProcessPrinter;


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
class Process
{
public:
  typedef gc::RefCountGarbageCollectionScheme garbage_collection_scheme;
  using dynamic_object_type = typename dyobj::DynamicObject<garbage_collection_scheme::DynamicObjectManager>;
  using dynamic_object_heap_type = typename dyobj::DynamicObjectHeap<garbage_collection_scheme::DynamicObjectManager>;
  typedef NativeTypesPool native_types_pool_type;

public:
  struct Options
  {
    Options();

    uint64_t heap_alloc_size;
    uint64_t pool_alloc_size;
    uint8_t gc_flag;
  };

  Process();
  Process(uint64_t, uint64_t);
  explicit Process(const Process::Options&);
  ~Process();

  /* Processes should not be copyable. */
  Process(const Process&) = delete;
  Process& operator=(const Process&) = delete;

  dyobj::dyobj_id create_dyobj();

  dynamic_object_type* create_dyobjs(size_t n);

  dynamic_object_type& get_dyobj(dyobj::dyobj_id id);

  uint64_t call_stack_size() const;

  bool has_frame() const;

  Frame& top_frame()
    throw(FrameNotFoundError);

  /**
   * Gets the top frame, only when the call stack is not empty.
   * Will result in undefined behavior otherwise.
   */
  void top_frame(Frame**);

  /**
   * Gets the `n`th frame from the top of the call stack.
   * A value of 0 means the top frame.
   */
  Frame& top_nth_frame(size_t n)
    throw(FrameNotFoundError);

  void push_frame(Frame&);

  void emplace_frame(
    const ClosureCtx&,
    Compartment*, Closure*);

  void emplace_frame(
    const ClosureCtx&,
    Compartment*,
    Closure*, instr_addr);

  void pop_frame() throw(FrameNotFoundError);

  void pop_frame_safe();

  uint64_t stack_size() const;

  InvocationCtx& top_invocation_ctx()
    throw(InvocationCtxNotFoundError);

  /**
   * Gets the top invocation context, only when the invocation stack
   * is not empty.
   * Will result in undefined behavior otherwise.
   */
  void top_invocation_ctx(InvocationCtx**);

  void push_invocation_ctx(const InvocationCtx&);

  void emplace_invocation_ctx(
    const ClosureCtx&,
    Compartment*, Closure*);

  void pop_invocation_ctx()
    throw(InvocationCtxNotFoundError);

  const dyobj::dyobj_id& top_stack()
    throw(ObjectStackEmptyError);

  dyobj::dyobj_id pop_stack()
    throw(ObjectStackEmptyError);

  void swap_stack();

  void push_stack(dyobj::dyobj_id&);

  bool has_ntvhndl(dyobj::ntvhndl_key&);

  types::native_type_handle& get_ntvhndl(dyobj::ntvhndl_key)
    throw(NativeTypeHandleNotFoundError);

  dyobj::ntvhndl_key insert_ntvhndl(types::native_type_handle&)
    throw(NativeTypeHandleInsertionError);

  void erase_ntvhndl(dyobj::ntvhndl_key)
    throw(NativeTypeHandleDeletionError);

  instr_addr pc() const;

  void set_pc(const instr_addr)
    throw(InvalidInstrAddrError);

  void append_vector(const vector&);

  void insert_vector(const vector& vector);

  bool get_frame_by_closure_ctx(
    ClosureCtx&, Frame**);

  void start();

  bool should_gc() const;

  void set_do_gc();

  void do_gc();

  void set_gc_flag(uint8_t gc_flag);

  bool can_execute();

  void pause_exec();

  void resume_exec();

  void set_sig_vector(sig_atomic_t, vector&);

  void handle_signal(sig_atomic_t, SigHandler*);

  dynamic_object_heap_type::size_type heap_size() const;

  dynamic_object_heap_type::size_type max_heap_size() const;

  native_types_pool_type::size_type ntvhndl_pool_size() const;

  native_types_pool_type::size_type max_ntvhndl_pool_size() const;

  size_t compartment_count() const;

  compartment_id insert_compartment(
    const Compartment&);

  compartment_id insert_compartment(
    const Compartment&&);

  void get_compartment(
    compartment_id, Compartment**);

  void reset();

  /**
   * Given a starting closure context, find the existing frame associated with
   * it, following the closure tree.
   *
   * Returns a pointer that points to the frame, if found.
   * Returns a null pointer otherwise.
   */
  static
  Frame* find_frame_by_ctx(
    ClosureCtx ctx,
    Compartment* compartment,
    Process& process);

  /**
   * Given a pointer to a starting frame, find the existing frame associated
   * with the parent of the given frame's closure context.
   *
   * Returns a pointer that points to the frame, if found.
   * Returns a null pointer otherwise.
   */
  static
  Frame* find_parent_frame_in_process(
    Frame* frame_ptr,
    Process& process);

  // A few things to note:
  //
  // 1. Once the stack is unwinded, the action cannot be undone.
  // 2. This function has to be exception safe because there are no catch
  //    blocks to catch exceptions from it.
  static void unwind_stack(
    Process&, size_t limit=COREVM_DEFAULT_STACK_UNWIND_COUNT);

  friend class ProcessPrinter;

private:
  void init();

  bool is_valid_pc() const;

  bool pre_start();

  void check_call_stack_capacity();

  void check_invk_ctx_stack_capacity();

  void set_parent_for_top_frame();

  bool m_pause_exec;
  bool m_do_gc;
  uint8_t m_gc_flag;
  instr_addr m_pc;
  vector m_instrs;
  dyobj::DynamicObjectHeap<garbage_collection_scheme::DynamicObjectManager> m_dynamic_object_heap;
  std::vector<dyobj::dyobj_id> m_dyobj_stack;
  std::vector<Frame> m_call_stack;
  std::vector<InvocationCtx> m_invocation_ctx_stack;
  native_types_pool_type m_ntvhndl_pool;
  std::unordered_map<sig_atomic_t, vector> m_sig_instr_map;
  std::vector<Compartment> m_compartments;
  std::vector<GCRulePtr> m_gc_rules;
};

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_PROCESS_H_ */
