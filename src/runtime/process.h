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

#include "fwd.h"
#include "closure.h"
#include "common.h"
#include "runtime_types.h"
#include "dyobj/common.h"
#include "corevm/logging.h"

#include <cstdint>
#include <iosfwd>
#include <type_traits>


namespace corevm {
namespace runtime {

/**
 * A process is a unit for managing the execution of a sequence of instructions.
 * It's supposed to have the following at the very minimum:
 *
 * - A flag for pause/resume execution.
 * - A flag for GC.
 * - A sequence of instruction blocks.
 * - A program counter.
 * - A heap for holding dynamic objects.
 * - A call stack for executing blocks of instructions.
 * - A stack of invocation contexts.
 * - A pool of native type values.
 * - A set of compartments.
 */
class Process : public Loggable
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

  enum ExecutionStatus : uint8_t
  {
    EXECUTION_STATUS_IDLE,
    EXECUTION_STATUS_ACTIVE,
    EXECUTION_STATUS_PAUSED,
    EXECUTION_STATUS_TERMINATED
  };

  class Printer
  {
  public:
    Printer(const Process&, uint32_t opts);

    std::ostream& operator()(std::ostream&) const;

  private:
    const Process& m_process;
    const uint32_t m_opts;
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

  const Frame& top_frame() const;

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

  types::NativeTypeValue& get_type_value(const types::NativeTypeValue*);

  types::NativeTypeValue* insert_type_value(const types::NativeTypeValue&);

  void erase_type_value(const types::NativeTypeValue*);

  void insert_attr_name(dyobj::attr_key_t, const char*);

  bool get_attr_name(dyobj::attr_key_t, const char**) const;

  instr_addr_t pc() const;

  void set_pc(instr_addr_t);

  bool get_frame_by_closure_ctx(ClosureCtx&, Frame**);

  void run();

  bool should_gc() const;

  void set_do_gc();

  void do_gc();

  void set_gc_flag(uint8_t gc_flag);

  ExecutionStatus execution_status() const;

  void pause_exec();

  void resume_exec();

  void terminate_exec();

  dynamic_object_heap_type::size_type heap_size() const;

  dynamic_object_heap_type::size_type max_heap_size() const;

  size_t native_type_pool_size() const;

  size_t max_native_type_pool_size() const;

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
  Frame* find_frame_by_ctx(ClosureCtx ctx, Compartment* compartment);

  /**
   * Given a pointer to a starting frame, find the existing frame associated
   * with the parent of the given frame's closure context.
   *
   * Returns a pointer that points to the frame, if found.
   * Returns a null pointer otherwise.
   */
  Frame* find_parent_frame_in_process(Frame* frame_ptr);

  // A few things to note:
  //
  // 1. Once the stack is unwinded, the action cannot be undone.
  // 2. This function has to be exception safe because there are no catch
  //    blocks to catch exceptions from it.
  void unwind_stack(size_t limit=COREVM_DEFAULT_STACK_UNWIND_COUNT);

private:
  class Impl;

  std::unique_ptr<Impl> m_impl;
};

// -----------------------------------------------------------------------------

} /* end namespace runtime */
} /* end namespace corevm */


#endif /* COREVM_PROCESS_H_ */
