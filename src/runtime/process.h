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
#include "runtime_types.h"
#include "sighandler.h"
#include "vector.h"
#include "dyobj/common.h"
#include "dyobj/dynamic_object_heap.h"

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
  typedef RuntimeTypes::dynamic_object_type dynamic_object_type;
  typedef RuntimeTypes::dynamic_object_heap_type dynamic_object_heap_type;
  typedef RuntimeTypes::garbage_collection_scheme garbage_collection_scheme;
  typedef RuntimeTypes::dyobj_ptr_type dyobj_ptr;

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

  dyobj_ptr create_dyobj();

  dyobj_ptr create_dyobjs(size_t n);

  dynamic_object_type& get_dyobj(dyobj::dyobj_id_t id);

  uint64_t call_stack_size() const;

  bool has_frame() const;

  Frame& top_frame();

  /**
   * Gets the top frame, only when the call stack is not empty.
   * Will result in undefined behavior otherwise.
   */
  void top_frame(Frame**);

  /**
   * Gets the `n`th frame from the top of the call stack.
   * A value of 0 means the top frame.
   */
  Frame& top_nth_frame(size_t n);

  void push_frame(Frame&);

  void emplace_frame(const ClosureCtx&, Compartment*, Closure*);

  void emplace_frame(const ClosureCtx&, Compartment*, Closure*, instr_addr_t);

  void pop_frame();

  void pop_frame_safe();

  uint64_t stack_size() const;

  InvocationCtx& top_invocation_ctx();

  /**
   * Gets the top invocation context, only when the invocation stack
   * is not empty.
   * Will result in undefined behavior otherwise.
   */
  void top_invocation_ctx(InvocationCtx**);

  void push_invocation_ctx(const InvocationCtx&);

  void emplace_invocation_ctx(const ClosureCtx&, Compartment*, Closure*);

  void pop_invocation_ctx();

  dyobj_ptr top_stack();

  dyobj_ptr pop_stack();

  void swap_stack();

  void push_stack(dyobj_ptr);

  types::NativeTypeHandle& get_ntvhndl(const types::NativeTypeHandle*);

  types::NativeTypeHandle* insert_ntvhndl(const types::NativeTypeHandle&);

  void erase_ntvhndl(const types::NativeTypeHandle*);

  instr_addr_t pc() const;

  void set_pc(const instr_addr_t);

  void append_vector(const Vector&);

  void insert_vector(const Vector& vector);

  bool get_frame_by_closure_ctx(ClosureCtx&, Frame**);

  void start();

  bool should_gc() const;

  void set_do_gc();

  void do_gc();

  void set_gc_flag(uint8_t gc_flag);

  bool can_execute();

  void pause_exec();

  void resume_exec();

  void set_sig_vector(sig_atomic_t, Vector&);

  void handle_signal(sig_atomic_t, SigHandler*);

  dynamic_object_heap_type::size_type heap_size() const;

  dynamic_object_heap_type::size_type max_heap_size() const;

  NativeTypesPool::size_type ntvhndl_pool_size() const;

  NativeTypesPool::size_type max_ntvhndl_pool_size() const;

  size_t compartment_count() const;

  compartment_id_t insert_compartment(const Compartment&);

  compartment_id_t insert_compartment(const Compartment&&);

  void get_compartment(compartment_id_t, Compartment**);

  void reset();

  /**
   * Given a starting closure context, find the existing frame associated with
   * it, following the closure tree.
   *
   * Returns a pointer that points to the frame, if found.
   * Returns a null pointer otherwise.
   */
  static
  Frame* find_frame_by_ctx(ClosureCtx ctx, Compartment* compartment,
    Process& process);

  /**
   * Given a pointer to a starting frame, find the existing frame associated
   * with the parent of the given frame's closure context.
   *
   * Returns a pointer that points to the frame, if found.
   * Returns a null pointer otherwise.
   */
  static
  Frame* find_parent_frame_in_process(Frame* frame_ptr, Process& process);

  // A few things to note:
  //
  // 1. Once the stack is unwinded, the action cannot be undone.
  // 2. This function has to be exception safe because there are no catch
  //    blocks to catch exceptions from it.
  static void unwind_stack(Process&,
    size_t limit=COREVM_DEFAULT_STACK_UNWIND_COUNT);

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
  instr_addr_t m_pc;
  Vector m_instrs;
  dynamic_object_heap_type m_dynamic_object_heap;
  std::vector<dyobj_ptr> m_dyobj_stack;
  std::vector<Frame> m_call_stack;
  std::vector<InvocationCtx> m_invocation_ctx_stack;
  NativeTypesPool m_ntvhndl_pool;
  std::unordered_map<sig_atomic_t, Vector> m_sig_instr_map;
  std::vector<Compartment> m_compartments;
  std::vector<GCRulePtr> m_gc_rules;
};

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_PROCESS_H_ */
