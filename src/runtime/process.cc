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
#include "process.h"

#include "closure.h"
#include "closure_ctx.h"
#include "common.h"
#include "errors.h"
#include "frame.h"
#include "gc_rule.h"
#include "instr.h"
#include "native_types_pool.h"
#include "sighandler_registrar.h"
#include "vector.h"
#include "corevm/macros.h"
#include "dyobj/common.h"
#include "dyobj/dynamic_object_heap.h"
#include "gc/garbage_collector.h"
#include "types/native_type_handle.h"

#if __MEASURE_INSTRS__
  #include "measurement.h"
#endif

#include <algorithm>
#include <array>
#include <cstdint>
#include <list>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <utility>

#include <setjmp.h>


namespace corevm {


namespace runtime {


namespace internal {


// -----------------------------------------------------------------------------

class NtvhndlCollectorGcCallback :
  public gc::GarbageCollector<Process::garbage_collection_scheme>::Callback
{
public:
  virtual void operator()(const dynamic_object_type& obj);

  const std::vector<const types::NativeTypeHandle*>& list() const
  {
    return m_ntvhndls;
  }

private:
  std::vector<const types::NativeTypeHandle*> m_ntvhndls;
};

// -----------------------------------------------------------------------------

/* virtual */
void
NtvhndlCollectorGcCallback::operator()(const dynamic_object_type& obj)
{
  if (obj.has_ntvhndl())
  {
    m_ntvhndls.push_back(&obj.ntvhndl());
  }
}

// -----------------------------------------------------------------------------


} /* end namespace internal */


// -----------------------------------------------------------------------------

const size_t DEFAULT_CALL_STACK_CAPACITY = 100;

// -----------------------------------------------------------------------------

const size_t DEFAULT_INVOCATION_STACK_CAPACITY = 100;

// -----------------------------------------------------------------------------

const size_t DEFAULT_OBJECT_STACK_CAPACITY = 100;

// -----------------------------------------------------------------------------

const size_t DEFAULT_COMPARTMENTS_TABLE_CAPACITY = 10;

// -----------------------------------------------------------------------------

const size_t DEFAULT_VECTOR_CAPACITY = 1 << 14;

// -----------------------------------------------------------------------------

static_assert(
  std::numeric_limits<runtime::Vector::size_type>::max() >=
  std::numeric_limits<instr_addr_t>::max(),
  "Vector size incompatibility"
);

// -----------------------------------------------------------------------------

static_assert(
  std::numeric_limits<std::vector<Compartment>::size_type>::max() >=
  std::numeric_limits<compartment_id_t>::max(),
  "Compartment ID incompatibility"
);

// -----------------------------------------------------------------------------

Process::Options::Options()
  :
  heap_alloc_size(dyobj::COREVM_DEFAULT_HEAP_SIZE),
  pool_alloc_size(COREVM_DEFAULT_NATIVE_TYPES_POOL_SIZE),
  gc_flag(GCRuleMeta::DEFAULT_GC_FLAGS)
{
}

// -----------------------------------------------------------------------------

Process::dyobj_ptr
Process::create_dyobj()
{
  return m_dynamic_object_heap.create_dyobj();
}

// -----------------------------------------------------------------------------

Process::dyobj_ptr
Process::create_dyobjs(size_t n)
{
  return m_dynamic_object_heap.create_dyobjs(n);
}

// -----------------------------------------------------------------------------

Process::dynamic_object_type&
Process::get_dyobj(dyobj::dyobj_id_t id)
{
  return m_dynamic_object_heap.at(id);
}

// -----------------------------------------------------------------------------

Process::Process()
  :
  Loggable(),
  m_pause_exec(false),
  m_do_gc(false),
  m_gc_flag(0),
  m_pc(NONESET_INSTR_ADDR),
  m_dynamic_object_heap(),
  m_dyobj_stack(),
  m_call_stack(),
  m_invocation_ctx_stack(),
  m_ntvhndl_pool(),
  m_sig_instr_map(),
  m_compartments(),
  m_gc_rules()
{
  init();
}

// -----------------------------------------------------------------------------

Process::Process(uint64_t heap_alloc_size, uint64_t pool_alloc_size)
  :
  Loggable(),
  m_pause_exec(false),
  m_do_gc(false),
  m_gc_flag(0),
  m_pc(NONESET_INSTR_ADDR),
  m_dynamic_object_heap(heap_alloc_size),
  m_dyobj_stack(),
  m_call_stack(),
  m_invocation_ctx_stack(),
  m_ntvhndl_pool(pool_alloc_size),
  m_sig_instr_map(),
  m_compartments(),
  m_gc_rules()
{
  init();
}

// -----------------------------------------------------------------------------

Process::Process(const Process::Options& options)
  :
  Loggable(),
  m_pause_exec(false),
  m_do_gc(false),
  m_gc_flag(options.gc_flag),
  m_pc(NONESET_INSTR_ADDR),
  m_dynamic_object_heap(options.heap_alloc_size),
  m_dyobj_stack(),
  m_call_stack(),
  m_invocation_ctx_stack(),
  m_ntvhndl_pool(options.pool_alloc_size),
  m_sig_instr_map(),
  m_compartments(),
  m_gc_rules()
{
  init();
}

// -----------------------------------------------------------------------------

void
Process::init()
{
  m_compartments.reserve(DEFAULT_COMPARTMENTS_TABLE_CAPACITY);

  // Initialize gc rules.
  const size_t flag_size = sizeof(m_gc_flag) * sizeof(char);

  for (size_t i = 0; i < flag_size; ++i)
  {
    // Add 1 to bit since the gc flag values are 0 index based.
    if (is_bit_set(m_gc_flag, static_cast<char>(i) + 1))
    {
      GCRuleMeta::GCBitfields bit =
        static_cast<GCRuleMeta::GCBitfields>(i);

      const GCRulePtr gc_rule =
        GCRuleMeta::get_gc_rule(bit);

      m_gc_rules.push_back(gc_rule);
    }
  }
}

// -----------------------------------------------------------------------------

Process::~Process()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

uint64_t
Process::call_stack_size() const
{
  return m_call_stack.size();
}

// -----------------------------------------------------------------------------

bool
Process::has_frame() const
{
  return !(this->m_call_stack.empty());
}

// -----------------------------------------------------------------------------

Frame&
Process::top_frame()
{
  if (m_call_stack.empty())
  {
    THROW(FrameNotFoundError());
  }

  return m_call_stack.back();
}

// -----------------------------------------------------------------------------

void
Process::top_frame(Frame** frame_ptr)
{
  *frame_ptr = &m_call_stack.back();
}

// -----------------------------------------------------------------------------

Frame&
Process::top_nth_frame(size_t n)
{
  if (n >= m_call_stack.size())
  {
    THROW(FrameNotFoundError());
  }

  return m_call_stack[m_call_stack.size() - n - 1];
}

// -----------------------------------------------------------------------------

void
Process::pop_frame()
{
  Frame& frame = this->top_frame();

  auto visible_objs = frame.get_visible_objs();
  auto invisible_objs = frame.get_invisible_objs();

  std::for_each(visible_objs.begin(), visible_objs.end(),
    [this](Process::dyobj_ptr ptr) {
      ptr->manager().on_exit();
    }
  );

  std::for_each(invisible_objs.begin(), invisible_objs.end(),
    [this](Process::dyobj_ptr ptr) {
      ptr->manager().on_exit();
    }
  );

  set_pc(frame.return_addr());

  const Closure* closure_ptr = frame.closure_ptr();

  auto begin_itr = m_instrs.begin() + pc() + 1;
  auto end_itr = begin_itr;
  std::advance(end_itr, closure_ptr->vector.size());

  m_instrs.erase(begin_itr, end_itr);

  m_call_stack.pop_back();

  this->pop_invocation_ctx();
}

// -----------------------------------------------------------------------------

void
Process::pop_frame_safe()
{
  m_call_stack.pop_back();
  pop_invocation_ctx();
}

// -----------------------------------------------------------------------------

void
Process::check_call_stack_capacity()
{
  // NOTE: Pointers of frames on the call stack are used to set as parents
  // on frames. If memories for frames are ever relocated here, we need to
  // reset the pointers.
  size_t current_size = m_call_stack.size();
  if (current_size == m_call_stack.capacity())
  {
    size_t new_size = current_size << 1;
    if (new_size > current_size)
    {
      m_call_stack.reserve(new_size);
    }
  }
}

// -----------------------------------------------------------------------------

void
Process::push_frame(Frame& frame)
{
  check_call_stack_capacity();
  m_call_stack.push_back(frame);

  set_parent_for_top_frame();
}

// -----------------------------------------------------------------------------

void
Process::emplace_frame(const ClosureCtx& ctx, Compartment* compartment_ptr,
  Closure* closure_ptr)
{
  ASSERT(compartment_ptr);
  ASSERT(closure_ptr);
  check_call_stack_capacity();
  m_call_stack.emplace_back(ctx, compartment_ptr, closure_ptr);

  set_parent_for_top_frame();
}

// -----------------------------------------------------------------------------

void
Process::emplace_frame(const ClosureCtx& ctx, Compartment* compartment_ptr,
  Closure* closure_ptr, instr_addr_t return_addr)
{
  ASSERT(compartment_ptr);
  ASSERT(closure_ptr);
  check_call_stack_capacity();
  m_call_stack.emplace_back(ctx, compartment_ptr, closure_ptr, return_addr);

  set_parent_for_top_frame();
}

// -----------------------------------------------------------------------------

void
Process::set_parent_for_top_frame()
{
  Frame* frame = &m_call_stack.back();
  runtime::Frame* parent =
    Process::find_parent_frame_in_process(frame, *this);

  frame->set_parent(parent);
}

// -----------------------------------------------------------------------------

uint64_t
Process::stack_size() const
{
  return m_dyobj_stack.size();
}

// -----------------------------------------------------------------------------

InvocationCtx&
Process::top_invocation_ctx()
{
  if (m_invocation_ctx_stack.empty())
  {
    THROW(InvocationCtxNotFoundError());
  }

  return m_invocation_ctx_stack.back();
}

// -----------------------------------------------------------------------------

void
Process::top_invocation_ctx(InvocationCtx** invk_ctx_ptr)
{
  *invk_ctx_ptr = &m_invocation_ctx_stack.back();
}

// -----------------------------------------------------------------------------

void
Process::check_invk_ctx_stack_capacity()
{
  size_t current_size = m_invocation_ctx_stack.size();
  if (current_size == m_invocation_ctx_stack.capacity())
  {
    size_t new_size = current_size << 1;
    if (new_size > current_size)
    {
      m_invocation_ctx_stack.reserve(new_size);
    }
  }
}

// -----------------------------------------------------------------------------

void
Process::push_invocation_ctx(const InvocationCtx& invk_ctx)
{
  check_invk_ctx_stack_capacity();
  m_invocation_ctx_stack.push_back(invk_ctx);
}

// -----------------------------------------------------------------------------

void
Process::emplace_invocation_ctx(const ClosureCtx& ctx,
  Compartment* compartment_ptr,
  Closure* closure_ptr)
{
  ASSERT(compartment_ptr);
  ASSERT(closure_ptr);
  check_invk_ctx_stack_capacity();
  m_invocation_ctx_stack.emplace_back(ctx, compartment_ptr, closure_ptr);
}

// -----------------------------------------------------------------------------

void
Process::pop_invocation_ctx()
{
  if (m_invocation_ctx_stack.empty())
  {
    THROW(InvocationCtxNotFoundError());
  }

  m_invocation_ctx_stack.pop_back();
}

// -----------------------------------------------------------------------------

Process::dyobj_ptr
Process::top_stack()
{
  if (m_dyobj_stack.empty())
  {
    THROW(ObjectStackEmptyError());
  }

  return m_dyobj_stack.back();
}

// -----------------------------------------------------------------------------

void
Process::push_stack(dyobj_ptr ptr)
{
  size_t current_size = m_dyobj_stack.size();
  if (current_size == m_dyobj_stack.capacity())
  {
    size_t new_size = current_size << 1;
    if (new_size > current_size)
    {
      m_dyobj_stack.reserve(new_size);
    }
  }
  m_dyobj_stack.push_back(ptr);
}

// -----------------------------------------------------------------------------

Process::dyobj_ptr
Process::pop_stack()
{
  if (m_dyobj_stack.empty())
  {
    THROW(ObjectStackEmptyError());
  }

  auto ptr = m_dyobj_stack.back();
  m_dyobj_stack.pop_back();
  return ptr;
}

// -----------------------------------------------------------------------------

void
Process::swap_stack()
{
  if (m_dyobj_stack.size() < 2)
  {
    THROW(InvalidOperationError(
      "Cannot swap top of object stack"));
  }

  auto& a = m_dyobj_stack[m_dyobj_stack.size() - 1];
  auto& b = m_dyobj_stack[m_dyobj_stack.size() - 2];
  std::swap(a, b);
}

// -----------------------------------------------------------------------------

Process::dynamic_object_heap_type::size_type
Process::heap_size() const
{
  return m_dynamic_object_heap.size();
}

// -----------------------------------------------------------------------------

Process::dynamic_object_heap_type::size_type
Process::max_heap_size() const
{
  return m_dynamic_object_heap.max_size();
}

// -----------------------------------------------------------------------------

NativeTypesPool::size_type
Process::ntvhndl_pool_size() const
{
  return m_ntvhndl_pool.size();
}

// -----------------------------------------------------------------------------

NativeTypesPool::size_type
Process::max_ntvhndl_pool_size() const
{
  return m_ntvhndl_pool.max_size();
}

// -----------------------------------------------------------------------------

types::NativeTypeHandle&
Process::get_ntvhndl(const types::NativeTypeHandle* hndl)
{
  return m_ntvhndl_pool.at(hndl);
}

// -----------------------------------------------------------------------------

types::NativeTypeHandle*
Process::insert_ntvhndl(const types::NativeTypeHandle& hndl)
{
  return m_ntvhndl_pool.create(hndl);
}

// -----------------------------------------------------------------------------

void
Process::erase_ntvhndl(const types::NativeTypeHandle* ptr)
{
  try
  {
    m_ntvhndl_pool.erase(const_cast<types::NativeTypeHandle*>(ptr));
  }
  catch (const NativeTypeHandleNotFoundError)
  {
    THROW(NativeTypeHandleDeletionError());
  }
}

// -----------------------------------------------------------------------------

void
Process::pause_exec()
{
  m_pause_exec = true;
}

// -----------------------------------------------------------------------------

void
Process::resume_exec()
{
  m_pause_exec = false;
}

// -----------------------------------------------------------------------------

inline bool
Process::is_valid_pc() const
{
  return (uint64_t)m_pc < m_instrs.size();
}

// -----------------------------------------------------------------------------

inline bool
Process::can_execute()
{
  return is_valid_pc();
}

// -----------------------------------------------------------------------------

bool
Process::pre_start()
{
  if (m_compartments.empty())
  {
    return false;
  }

  Closure* closure = nullptr;
  Compartment* compartment = &m_compartments.front();

  bool res = compartment->get_starting_closure(&closure);

  // If we found the starting compartment and closure, create a frame with the
  // closure context, and loads and vector into the process.
  if (res)
  {
#if __DEBUG__
    ASSERT(closure);
#endif

    m_instrs.reserve(DEFAULT_VECTOR_CAPACITY);
    append_vector(closure->vector);

    ClosureCtx ctx(0, closure->id);

    m_call_stack.reserve(DEFAULT_CALL_STACK_CAPACITY);
    emplace_frame(ctx, compartment, closure, m_pc);

    m_invocation_ctx_stack.reserve(DEFAULT_INVOCATION_STACK_CAPACITY);
    emplace_invocation_ctx(ctx, compartment, closure);

    m_dyobj_stack.reserve(DEFAULT_OBJECT_STACK_CAPACITY);

    m_pc = 0;
  }

  return res;
}

// -----------------------------------------------------------------------------

void
Process::start()
{
  if (!pre_start())
  {
    return;
  }

#if __DEBUG__
  ASSERT(!m_call_stack.empty());
  ASSERT(!m_invocation_ctx_stack.empty());
#endif

  Frame* frame = &m_call_stack.back();
  InvocationCtx* invk_ctx = &m_invocation_ctx_stack.back();

  runtime::Frame** frame_ptr = &frame;
  InvocationCtx** invk_ctx_ptr = &invk_ctx;

#if __MEASURE_INSTRS__
  std::array<InstrMeasurement, INSTR_CODE_MAX> measurements;
  boost::timer::cpu_timer t;
#endif

  while (can_execute())
  {
    while (m_pause_exec) {}

    const Instr& instr = m_instrs[static_cast<size_t>(m_pc)];

    auto& handler =
      instr_handler_meta::instr_handlers[instr.code].handler;

#if __MEASURE_INSTRS__
    t.start();
#endif

    handler->execute(instr, *this, frame_ptr, invk_ctx_ptr);

    if (m_do_gc)
    {
      do_gc();
      m_do_gc = false;
    }

#if __MEASURE_INSTRS__
    t.stop();

    const boost::timer::cpu_times res = t.elapsed();
    measurements[instr.code].cumulative_wall_time += res.wall;
    ++measurements[instr.code].invocation_count;
#endif

    /**
     * TODO: [COREVM-246] Enable support for signal handling mechanism
     *
    sigsetjmp(SigHandlerRegistrar::get_sigjmp_env(), 1);

    if (!SigHandlerRegistrar::is_sig_raised())
    {
      handler->execute(instr, *this);
    }
    else
    {
      if (!this->can_execute())
      {
        break;
      }
    }

    SigHandlerRegistrar::clear_sig_raised();
    *
    **/

    ++m_pc;

  } /* end `while (can_execute())` */

#if __MEASURE_INSTRS__
  pretty_print_measurements(measurements);
#endif
}

// -----------------------------------------------------------------------------

void
Process::set_do_gc()
{
  m_do_gc = true;
}

// -----------------------------------------------------------------------------

void
Process::do_gc()
{
  this->pause_exec();

  gc::GarbageCollector<garbage_collection_scheme> garbage_collector(
    m_dynamic_object_heap);

  garbage_collector.set_logger(m_logger);

  internal::NtvhndlCollectorGcCallback callback;
  garbage_collector.gc(&callback);

  std::for_each(
    callback.list().begin(),
    callback.list().end(),
    [&](const types::NativeTypeHandle* ptr) {
      this->erase_ntvhndl(ptr);
    }
  );

  this->resume_exec();
}

// -----------------------------------------------------------------------------

void
Process::set_gc_flag(uint8_t gc_flag)
{
  m_gc_flag = gc_flag;
}

// -----------------------------------------------------------------------------

instr_addr_t
Process::pc() const
{
  return m_pc;
}

// -----------------------------------------------------------------------------

void
Process::set_pc(const instr_addr_t addr)
{
  if ( (uint64_t)addr >= m_instrs.size() )
  {
    THROW(InvalidInstrAddrError());
  }

  m_pc = addr;
}

// -----------------------------------------------------------------------------

void
Process::append_vector(const Vector& vector)
{
  // Inserts the vector at the very end of the instr array.
  std::copy(vector.begin(), vector.end(), std::back_inserter(m_instrs));
}

// -----------------------------------------------------------------------------

void
Process::insert_vector(const Vector& vector)
{
  // We want to insert the vector right after the current pc().
  //
  // NOTE: changes in the capacity of `m_instr` will result in AddressSanitizer
  // report "heap-use-after-free".
  m_instrs.insert(m_instrs.begin() + pc() + 1, vector.begin(), vector.end());
}

// -----------------------------------------------------------------------------

bool
Process::get_frame_by_closure_ctx(ClosureCtx& closure_ctx, Frame** frame_ptr)
{
  auto itr = std::find_if(
    m_call_stack.begin(),
    m_call_stack.end(),
    [&closure_ctx](const Frame& frame) -> bool {
      return frame.closure_ctx() == closure_ctx;
    }
  );

  if (itr != m_call_stack.end())
  {
    *frame_ptr = &(*itr);
    return true;
  }

  return false;
}

// -----------------------------------------------------------------------------

bool
Process::should_gc() const
{
  for (const auto& gc_rule : m_gc_rules)
  {
    if (gc_rule->should_gc(*this))
    {
      return true;
    }
  }

  return false;
}

// -----------------------------------------------------------------------------

void
Process::set_sig_vector(sig_atomic_t sig, Vector& vector)
{
  m_sig_instr_map.insert({sig, vector});
}

// -----------------------------------------------------------------------------

void
Process::handle_signal(sig_atomic_t sig, SigHandler* handler)
{
  auto itr = m_sig_instr_map.find(sig);

  if (itr != m_sig_instr_map.end())
  {
    Vector vector = itr->second;
    this->pause_exec();
    this->insert_vector(vector);
    this->resume_exec();
  }
  else if (handler != nullptr)
  {
    handler->handle_signal(sig, *this);
  }
}

// -----------------------------------------------------------------------------

size_t
Process::compartment_count() const
{
  return m_compartments.size();
}

// -----------------------------------------------------------------------------

compartment_id_t
Process::insert_compartment(const Compartment& compartment)
{
  m_compartments.push_back(compartment);
  return static_cast<compartment_id_t>(m_compartments.size() - 1);
}

// -----------------------------------------------------------------------------

compartment_id_t
Process::insert_compartment(const Compartment&& compartment)
{
  m_compartments.push_back(
    std::forward<const runtime::Compartment>(compartment));

  return static_cast<compartment_id_t>(m_compartments.size() - 1);
}

// -----------------------------------------------------------------------------

void
Process::get_compartment(compartment_id_t id, Compartment** ptr)
{
  if (id < static_cast<compartment_id_t>(m_compartments.size()))
  {
    *ptr = &m_compartments[static_cast<size_t>(id)];
  }
}

// -----------------------------------------------------------------------------

void
Process::reset()
{
  m_gc_flag = 0;
  m_pc = NONESET_INSTR_ADDR;
  m_dyobj_stack.clear();
  m_call_stack.clear();
  m_invocation_ctx_stack.clear();
  m_compartments.clear();
  m_gc_rules.clear();
}

// -----------------------------------------------------------------------------

Frame*
Process::find_frame_by_ctx(ClosureCtx ctx, Compartment* compartment,
  Process& process)
{
#if __DEBUG__
  ASSERT(compartment);
#endif

  Frame* frame = nullptr;

  while (!frame)
  {
    bool res = process.get_frame_by_closure_ctx(ctx, &frame);

    if (res)
    {
#if __DEBUG__
      ASSERT(frame);
#endif
      break;
    }

    Closure *closure = nullptr;
    compartment->get_closure_by_id(ctx.closure_id, &closure);

#if __DEBUG__
    ASSERT(closure);
#endif

    ctx.closure_id = closure->parent_id;

    if (ctx.closure_id == NONESET_CLOSURE_ID)
    {
#if __DEBUG__
      ASSERT(!frame);
#endif
      break;
    }
  }

  return frame;
}

// -----------------------------------------------------------------------------

Frame*
Process::find_parent_frame_in_process(Frame* frame_ptr, Process& process)
{
#if __DEBUG__
  ASSERT(frame_ptr);
#endif

  const Closure* closure = frame_ptr->closure_ptr();

#if __DEBUG__
  ASSERT(closure);
#endif

  closure_id_t parent_closure_id = closure->parent_id;

  if (parent_closure_id == NONESET_CLOSURE_ID)
  {
    return nullptr;
  }

  ClosureCtx ctx(frame_ptr->closure_ctx().compartment_id, parent_closure_id);

  Compartment* compartment = frame_ptr->compartment_ptr();

#if __DEBUG__
  ASSERT(compartment);
#endif

  frame_ptr = Process::find_frame_by_ctx(
    ctx, compartment, process);

  return frame_ptr;
}

// -----------------------------------------------------------------------------

void
Process::unwind_stack(Process& process, size_t limit)
{
  size_t unwind_count = 0;
  std::vector<std::string> output_lines;

  if (limit)
  {
    output_lines.reserve(limit);
  }

  while (process.has_frame())
  {
    std::stringstream line_ss;

    Frame& frame = process.top_frame();

    const Compartment* compartment = frame.compartment_ptr();

    line_ss << "    " << "File " << '\"' << compartment->path() << '\"';

    const Closure* closure = frame.closure_ptr();

    const LocTable& locs = closure->locs;

    int32_t index = process.pc() - frame.return_addr();

    if (locs.find(index) != locs.end())
    {
      const LocInfo& loc = locs.at(index);

      line_ss << " (" << "line " << loc.lineno << " col " << loc.col_offset << ')';
    }

    line_ss << " in " << closure->name;

    output_lines.push_back(std::move(line_ss.str()));

    process.pop_frame_safe();

    ++unwind_count;

    if (limit && unwind_count >= limit)
    {
      break;
    }
  }

  process.reset();

  // Dump output.
  std::cerr << "Trackback:" << std::endl;
  for (auto itr = output_lines.crbegin(); itr != output_lines.crend(); ++itr)
  {
    const std::string& line_str = *itr;
    std::cerr << line_str << std::endl;
  }

  std::cerr << std::endl;
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */
