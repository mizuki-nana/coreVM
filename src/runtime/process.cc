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
#include "compartment.h"
#include "compartment_printer.h"
#include "errors.h"
#include "frame.h"
#include "frame_cache.h"
#include "gc_rule.h"
#include "instr.h"
#include "invocation_ctx.h"
#include "loc_info.h"
#include "native_types_pool.h"
#include "vector.h"
#include "corevm/macros.h"
#include "dyobj/common.h"
#include "dyobj/dynamic_object_heap.h"
#include "gc/garbage_collector.h"
#include "types/native_type_handle.h"
#include "corevm/llvm_smallvector.h"

#if __MEASURE_INSTRS__
  #include "measurement.h"
#endif

#include <llvm/ADT/SmallString.h>

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

class Process::Impl
{
public:
  typedef Process::dynamic_object_type dynamic_object_type;
  typedef Process::dynamic_object_heap_type dynamic_object_heap_type;
  typedef Process::garbage_collection_scheme garbage_collection_scheme;
  typedef Process::dyobj_ptr dyobj_ptr;

  Impl(Process* parent);
  Impl(Process* parent, uint64_t, uint64_t);
  explicit Impl(Process* parent, const Process::Options&);

  ~Impl();

  dyobj_ptr create_dyobj();

  dyobj_ptr create_dyobjs(size_t n);

  dynamic_object_type& get_dyobj(dyobj::dyobj_id_t id);

  uint64_t call_stack_size() const;

  bool has_frame() const;

  Frame& top_frame();

  const Frame& top_frame() const;

  void top_frame(Frame**);

  Frame& top_nth_frame(size_t n);

  void push_frame(Frame&);

  void emplace_frame(const ClosureCtx&, Compartment*, Closure*);

  void emplace_frame(const ClosureCtx&, Compartment*, Closure*, instr_addr_t);

  void pop_frame();

  void pop_frame_safe();

  uint64_t stack_size() const;

  InvocationCtx& top_invocation_ctx();

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

  Process::ExecutionStatus execution_status() const;

  void pause_exec();

  void resume_exec();

  void terminate_exec();

  dynamic_object_heap_type::size_type heap_size() const;

  dynamic_object_heap_type::size_type max_heap_size() const;

  NativeTypesPool::size_type ntvhndl_pool_size() const;

  NativeTypesPool::size_type max_ntvhndl_pool_size() const;

  size_t compartment_count() const;

  compartment_id_t insert_compartment(const Compartment&);

  compartment_id_t insert_compartment(const Compartment&&);

  void get_compartment(compartment_id_t, Compartment**);

  void reset();

  Frame* find_frame_by_ctx(ClosureCtx ctx, Compartment* compartment);

  Frame* find_parent_frame_in_process(Frame* frame_ptr);

  void unwind_stack(size_t limit);

  friend class Process::Printer;

private:
  void init();

  bool can_execute() const;

  bool is_valid_pc() const;

  bool start();

  void check_call_stack_capacity();

  void check_invk_ctx_stack_capacity();

  void set_parent_for_top_frame();

  typedef llvm::SmallString<16> AttributeNameType;
  typedef std::unordered_map<dyobj::attr_key_t, AttributeNameType> AttributeNameStore;
  typedef llvm::SmallVector<dyobj_ptr, 20> DynamicObjectStack;
  typedef llvm::SmallVector<Frame, 20> CallStack;
  typedef llvm::SmallVector<InvocationCtx, 20> InvocationCtxStack;
  typedef llvm::SmallVector<Compartment, 5> CompartmentStore;

  Process::ExecutionStatus m_execution_status;
  bool m_do_gc;
  uint8_t m_gc_flag;
  dynamic_object_heap_type m_dynamic_object_heap;
  DynamicObjectStack m_dyobj_stack;
  CallStack m_call_stack;
  InvocationCtxStack m_invocation_ctx_stack;
  NativeTypesPool m_ntvhndl_pool;
  CompartmentStore m_compartments;
  FrameCache m_frame_cache;
  AttributeNameStore m_attr_name_store;

  Process* m_owner;
};

// -----------------------------------------------------------------------------

Process::Impl::Impl(Process* owner)
  :
  m_execution_status(Process::EXECUTION_STATUS_IDLE),
  m_do_gc(false),
  m_gc_flag(0),
  m_dynamic_object_heap(),
  m_dyobj_stack(),
  m_call_stack(),
  m_invocation_ctx_stack(),
  m_ntvhndl_pool(),
  m_compartments(),
  m_frame_cache(),
  m_attr_name_store(),
  m_owner(owner)
{
  init();
}

// -----------------------------------------------------------------------------

Process::Impl::Impl(Process* owner, uint64_t heap_alloc_size,
  uint64_t pool_alloc_size)
  :
  m_execution_status(Process::EXECUTION_STATUS_IDLE),
  m_do_gc(false),
  m_gc_flag(0),
  m_dynamic_object_heap(heap_alloc_size),
  m_dyobj_stack(),
  m_call_stack(),
  m_invocation_ctx_stack(),
  m_ntvhndl_pool(pool_alloc_size),
  m_compartments(),
  m_frame_cache(),
  m_attr_name_store(),
  m_owner(owner)
{
  init();
}

// -----------------------------------------------------------------------------

Process::Impl::Impl(Process* owner, const Process::Options& options)
  :
  m_execution_status(Process::EXECUTION_STATUS_IDLE),
  m_do_gc(false),
  m_gc_flag(options.gc_flag),
  m_dynamic_object_heap(options.heap_alloc_size),
  m_dyobj_stack(),
  m_call_stack(),
  m_invocation_ctx_stack(),
  m_ntvhndl_pool(options.pool_alloc_size),
  m_compartments(),
  m_frame_cache(),
  m_attr_name_store(),
  m_owner(owner)
{
  init();
}

// -----------------------------------------------------------------------------

void
Process::Impl::init()
{
  m_compartments.reserve(DEFAULT_COMPARTMENTS_TABLE_CAPACITY);
}

// -----------------------------------------------------------------------------

Process::Impl::~Impl()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

Process::dyobj_ptr
Process::Impl::create_dyobj()
{
  return m_dynamic_object_heap.create_dyobj();
}

// -----------------------------------------------------------------------------

Process::dyobj_ptr
Process::Impl::create_dyobjs(size_t n)
{
  return m_dynamic_object_heap.create_dyobjs(n);
}

// -----------------------------------------------------------------------------

Process::dynamic_object_type&
Process::Impl::get_dyobj(dyobj::dyobj_id_t id)
{
  return m_dynamic_object_heap.at(id);
}

// -----------------------------------------------------------------------------

uint64_t
Process::Impl::call_stack_size() const
{
  return m_call_stack.size();
}

// -----------------------------------------------------------------------------

bool
Process::Impl::has_frame() const
{
  return !(m_call_stack.empty());
}

// -----------------------------------------------------------------------------

Frame&
Process::Impl::top_frame()
{
  if (m_call_stack.empty())
  {
    THROW(FrameNotFoundError());
  }

  return m_call_stack.back();
}

// -----------------------------------------------------------------------------

const Frame&
Process::Impl::top_frame() const
{
  return const_cast<Process::Impl*>(this)->top_frame();
}

// -----------------------------------------------------------------------------

void
Process::Impl::top_frame(Frame** frame_ptr)
{
  *frame_ptr = &m_call_stack.back();
}

// -----------------------------------------------------------------------------

Frame&
Process::Impl::top_nth_frame(size_t n)
{
  if (n >= m_call_stack.size())
  {
    THROW(FrameNotFoundError());
  }

  return m_call_stack[m_call_stack.size() - n - 1];
}

// -----------------------------------------------------------------------------

void
Process::Impl::pop_frame()
{
  Frame& frame = top_frame();

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

  m_frame_cache.erase_parent_frame(&frame);

  m_call_stack.pop_back();

  pop_invocation_ctx();
}

// -----------------------------------------------------------------------------

void
Process::Impl::pop_frame_safe()
{
  m_call_stack.pop_back();
  pop_invocation_ctx();
}

// -----------------------------------------------------------------------------

void
Process::Impl::check_call_stack_capacity()
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
Process::Impl::push_frame(Frame& frame)
{
  check_call_stack_capacity();
  m_call_stack.push_back(frame);

  set_parent_for_top_frame();
}

// -----------------------------------------------------------------------------

void
Process::Impl::emplace_frame(const ClosureCtx& ctx, Compartment* compartment,
  Closure* closure)
{
  ASSERT(compartment);
  ASSERT(closure);
  check_call_stack_capacity();
  m_call_stack.push_back(Frame(ctx, compartment, closure));

  set_parent_for_top_frame();
}

// -----------------------------------------------------------------------------

void
Process::Impl::emplace_frame(const ClosureCtx& ctx, Compartment* compartment,
  Closure* closure, instr_addr_t return_addr)
{
  ASSERT(compartment);
  ASSERT(closure);
  check_call_stack_capacity();
  m_call_stack.push_back(Frame(ctx, compartment, closure, return_addr));

  set_parent_for_top_frame();
}

// -----------------------------------------------------------------------------

void
Process::Impl::set_parent_for_top_frame()
{
  Frame* frame = &m_call_stack.back();

  Frame* parent = NULL;

  parent = m_frame_cache.parent_frame_of(frame->closure_ctx());

  if (!parent)
  {
    parent = find_parent_frame_in_process(frame);
    if (parent)
    {
      m_frame_cache.insert_parent_frame(frame->closure_ctx(), parent);
    }
  }

  frame->set_parent(parent);
}

// -----------------------------------------------------------------------------

uint64_t
Process::Impl::stack_size() const
{
  return m_dyobj_stack.size();
}

// -----------------------------------------------------------------------------

InvocationCtx&
Process::Impl::top_invocation_ctx()
{
  if (m_invocation_ctx_stack.empty())
  {
    THROW(InvocationCtxNotFoundError());
  }

  return m_invocation_ctx_stack.back();
}

// -----------------------------------------------------------------------------

void
Process::Impl::top_invocation_ctx(InvocationCtx** invk_ctx_ptr)
{
  *invk_ctx_ptr = &m_invocation_ctx_stack.back();
}

// -----------------------------------------------------------------------------

void
Process::Impl::check_invk_ctx_stack_capacity()
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
Process::Impl::push_invocation_ctx(const InvocationCtx& invk_ctx)
{
  check_invk_ctx_stack_capacity();
  m_invocation_ctx_stack.push_back(invk_ctx);
}

// -----------------------------------------------------------------------------

void
Process::Impl::emplace_invocation_ctx(const ClosureCtx& ctx,
  Compartment* compartment,
  Closure* closure)
{
  ASSERT(compartment);
  ASSERT(closure);
  check_invk_ctx_stack_capacity();
  m_invocation_ctx_stack.push_back(InvocationCtx(ctx, compartment, closure));
}

// -----------------------------------------------------------------------------

void
Process::Impl::pop_invocation_ctx()
{
  if (m_invocation_ctx_stack.empty())
  {
    THROW(InvocationCtxNotFoundError());
  }

  m_invocation_ctx_stack.pop_back();
}

// -----------------------------------------------------------------------------

Process::dyobj_ptr
Process::Impl::top_stack()
{
  if (m_dyobj_stack.empty())
  {
    THROW(ObjectStackEmptyError());
  }

  return m_dyobj_stack.back();
}

// -----------------------------------------------------------------------------

void
Process::Impl::push_stack(dyobj_ptr obj)
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
  obj->manager().lock();
  m_dyobj_stack.push_back(obj);
}

// -----------------------------------------------------------------------------

Process::dyobj_ptr
Process::Impl::pop_stack()
{
  if (m_dyobj_stack.empty())
  {
    THROW(ObjectStackEmptyError());
  }

  auto obj = m_dyobj_stack.back();
  m_dyobj_stack.pop_back();
  obj->manager().unlock();
  return obj;
}

// -----------------------------------------------------------------------------

void
Process::Impl::swap_stack()
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
Process::Impl::heap_size() const
{
  return m_dynamic_object_heap.size();
}

// -----------------------------------------------------------------------------

Process::dynamic_object_heap_type::size_type
Process::Impl::max_heap_size() const
{
  return m_dynamic_object_heap.max_size();
}

// -----------------------------------------------------------------------------

NativeTypesPool::size_type
Process::Impl::ntvhndl_pool_size() const
{
  return m_ntvhndl_pool.size();
}

// -----------------------------------------------------------------------------

NativeTypesPool::size_type
Process::Impl::max_ntvhndl_pool_size() const
{
  return m_ntvhndl_pool.max_size();
}

// -----------------------------------------------------------------------------

types::NativeTypeHandle&
Process::Impl::get_ntvhndl(const types::NativeTypeHandle* hndl)
{
  return m_ntvhndl_pool.at(hndl);
}

// -----------------------------------------------------------------------------

types::NativeTypeHandle*
Process::Impl::insert_ntvhndl(const types::NativeTypeHandle& hndl)
{
  return m_ntvhndl_pool.create(hndl);
}

// -----------------------------------------------------------------------------

void
Process::Impl::erase_ntvhndl(const types::NativeTypeHandle* ptr)
{
  m_ntvhndl_pool.erase(const_cast<types::NativeTypeHandle*>(ptr));
}

// -----------------------------------------------------------------------------

void
Process::Impl::insert_attr_name(dyobj::attr_key_t attr_key, const char* attr_name)
{
  m_attr_name_store.insert(std::make_pair(attr_key, AttributeNameType(attr_name)));
}

// -----------------------------------------------------------------------------

bool
Process::Impl::get_attr_name(dyobj::attr_key_t attr_key, const char** attr_name) const
{
  auto itr = m_attr_name_store.find(attr_key);

  const bool key_found = itr != m_attr_name_store.end();

  if (key_found)
  {
    *attr_name = const_cast<AttributeNameType&>(itr->second).c_str();
  }

  return key_found;
}

// -----------------------------------------------------------------------------

Process::ExecutionStatus
Process::Impl::execution_status() const
{
  return m_execution_status;
}

// -----------------------------------------------------------------------------

void
Process::Impl::pause_exec()
{
  if (m_execution_status == Process::EXECUTION_STATUS_ACTIVE)
  {
    m_execution_status = Process::EXECUTION_STATUS_PAUSED;
  }
}

// -----------------------------------------------------------------------------

void
Process::Impl::resume_exec()
{
  if (m_execution_status == Process::EXECUTION_STATUS_PAUSED)
  {
    m_execution_status = Process::EXECUTION_STATUS_ACTIVE;
  }
}

// -----------------------------------------------------------------------------

inline bool
Process::Impl::can_execute() const
{
  return (m_execution_status == Process::EXECUTION_STATUS_ACTIVE ||
          m_execution_status == Process::EXECUTION_STATUS_PAUSED)
    && has_frame() && top_frame().can_execute();
}

// -----------------------------------------------------------------------------

bool
Process::Impl::start()
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

    ClosureCtx ctx(0, closure->id);

    m_call_stack.reserve(DEFAULT_CALL_STACK_CAPACITY);
    emplace_frame(ctx, compartment, closure, 0);

    m_invocation_ctx_stack.reserve(DEFAULT_INVOCATION_STACK_CAPACITY);
    emplace_invocation_ctx(ctx, compartment, closure);

    m_dyobj_stack.reserve(DEFAULT_OBJECT_STACK_CAPACITY);

    top_frame().set_pc_safe(0);

    m_execution_status = Process::EXECUTION_STATUS_ACTIVE;
  }

  return res;
}

// -----------------------------------------------------------------------------

void
Process::Impl::run()
{
  if (!start())
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
    while (m_execution_status == Process::EXECUTION_STATUS_PAUSED) {}

    const Instr& instr = top_frame().current_instr();

#if __MEASURE_INSTRS__
    t.start();
#endif

    InstrHandlerMeta::instr_handlers[instr.code](
      instr, *m_owner, frame_ptr, invk_ctx_ptr);

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

    top_frame().inc_pc();

  } /* end `while (can_execute())` */

#if __MEASURE_INSTRS__
  pretty_print_measurements(measurements);
#endif
}

// -----------------------------------------------------------------------------

void
Process::Impl::set_do_gc()
{
  m_do_gc = true;
}

// -----------------------------------------------------------------------------

void
Process::Impl::do_gc()
{
  pause_exec();

  gc::GarbageCollector<garbage_collection_scheme> garbage_collector(
    m_dynamic_object_heap);

  garbage_collector.set_logger(m_owner->m_logger);

  internal::NtvhndlCollectorGcCallback callback;
  garbage_collector.gc(&callback);

  std::for_each(
    callback.list().begin(),
    callback.list().end(),
    [&](const types::NativeTypeHandle* ptr) {
      this->erase_ntvhndl(ptr);
    }
  );

  resume_exec();
}

// -----------------------------------------------------------------------------

void
Process::Impl::terminate_exec()
{
  m_execution_status = Process::EXECUTION_STATUS_TERMINATED;
}

// -----------------------------------------------------------------------------

void
Process::Impl::set_gc_flag(uint8_t gc_flag)
{
  m_gc_flag = gc_flag;
}

// -----------------------------------------------------------------------------

instr_addr_t
Process::Impl::pc() const
{
  if (!m_call_stack.empty())
  {
    return m_call_stack.back().pc();
  }

  return NONESET_INSTR_ADDR;
}

// -----------------------------------------------------------------------------

void
Process::Impl::set_pc(instr_addr_t addr)
{
  top_frame().set_pc(addr);
}

// -----------------------------------------------------------------------------

bool
Process::Impl::get_frame_by_closure_ctx(ClosureCtx& closure_ctx, Frame** frame_ptr)
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
Process::Impl::should_gc() const
{
  for (size_t i = 0; i < GCRuleMeta::GC_RULE_MAX; ++i)
  {
    if (m_gc_flag & (1 << i) && GCRuleMeta::gc_rules[i](*m_owner))
    {
      return true;
    }
  }

  return false;
}

// -----------------------------------------------------------------------------

size_t
Process::Impl::compartment_count() const
{
  return m_compartments.size();
}

// -----------------------------------------------------------------------------

compartment_id_t
Process::Impl::insert_compartment(const Compartment& compartment)
{
  m_compartments.push_back(compartment);
  return static_cast<compartment_id_t>(m_compartments.size() - 1);
}

// -----------------------------------------------------------------------------

compartment_id_t
Process::Impl::insert_compartment(const Compartment&& compartment)
{
  m_compartments.push_back(
    std::forward<const runtime::Compartment>(compartment));

  return static_cast<compartment_id_t>(m_compartments.size() - 1);
}

// -----------------------------------------------------------------------------

void
Process::Impl::get_compartment(compartment_id_t id, Compartment** ptr)
{
  if (id < static_cast<compartment_id_t>(m_compartments.size()))
  {
    *ptr = &m_compartments[static_cast<size_t>(id)];
  }
}

// -----------------------------------------------------------------------------

void
Process::Impl::reset()
{
  m_gc_flag = 0;
  m_dyobj_stack.clear();
  m_call_stack.clear();
  m_invocation_ctx_stack.clear();
  m_compartments.clear();
}

// -----------------------------------------------------------------------------

Frame*
Process::Impl::find_frame_by_ctx(ClosureCtx ctx, Compartment* compartment)
{
#if __DEBUG__
  ASSERT(compartment);
#endif

  Frame* frame = nullptr;

  while (!frame)
  {
    bool res = get_frame_by_closure_ctx(ctx, &frame);

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
Process::Impl::find_parent_frame_in_process(Frame* frame_ptr)
{
#if __DEBUG__
  ASSERT(frame_ptr);
#endif

  const Closure* closure = frame_ptr->closure();

#if __DEBUG__
  ASSERT(closure);
#endif

  closure_id_t parent_closure_id = closure->parent_id;

  if (parent_closure_id == NONESET_CLOSURE_ID)
  {
    return nullptr;
  }

  ClosureCtx ctx(frame_ptr->closure_ctx().compartment_id, parent_closure_id);

  Compartment* compartment = frame_ptr->compartment();

#if __DEBUG__
  ASSERT(compartment);
#endif

  frame_ptr = find_frame_by_ctx(ctx, compartment);

  return frame_ptr;
}

// -----------------------------------------------------------------------------

struct LocInfoPred
{
  LocInfoPred(int64_t index)
    :
    m_index(index)
  {
  }

  bool operator()(const LocInfo& loc_info) const
  {
    return loc_info.index == m_index;
  }

private:
  int64_t m_index;
};

// -----------------------------------------------------------------------------

void
Process::Impl::unwind_stack(size_t limit)
{
  size_t unwind_count = 0;
  std::vector<std::string> output_lines;

  if (limit)
  {
    output_lines.reserve(limit);
  }

  while (has_frame())
  {
    std::stringstream line_ss;

    Frame& frame = top_frame();

    const Compartment* compartment = frame.compartment();

    line_ss << "    " << "File " << '\"' << compartment->path() << '\"';

    const Closure* closure = frame.closure();

    const LocTable& locs = closure->locs;

    int64_t index = pc() - frame.return_addr();

    auto itr = std::find_if(locs.begin(), locs.end(), LocInfoPred(index));
    if (itr != locs.end())
    {
      const LocInfo& loc = *itr;

      line_ss << " (" << "line " << loc.lineno << " col " << loc.col_offset << ')';
    }

    line_ss << " in " << closure->name;

    output_lines.push_back(std::move(line_ss.str()));

    pop_frame_safe();

    ++unwind_count;

    if (limit && unwind_count >= limit)
    {
      break;
    }
  }

  reset();

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

Process::Process()
  :
  Loggable(),
  m_impl(new Process::Impl(this))
{
}

// -----------------------------------------------------------------------------

Process::Process(uint64_t heap_alloc_size, uint64_t pool_alloc_size)
  :
  Loggable(),
  m_impl(new Process::Impl(this, heap_alloc_size, pool_alloc_size))
{
}

// -----------------------------------------------------------------------------

Process::Process(const Process::Options& options)
  :
  Loggable(),
  m_impl(new Process::Impl(this, options))
{
}

// -----------------------------------------------------------------------------

Process::~Process()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

Process::dyobj_ptr
Process::create_dyobj()
{
  return m_impl->create_dyobj();
}

// -----------------------------------------------------------------------------

Process::dyobj_ptr
Process::create_dyobjs(size_t n)
{
  return m_impl->create_dyobjs(n);
}

// -----------------------------------------------------------------------------

Process::dynamic_object_type&
Process::get_dyobj(dyobj::dyobj_id_t id)
{
  return m_impl->get_dyobj(id);
}

// -----------------------------------------------------------------------------

uint64_t
Process::call_stack_size() const
{
  return m_impl->call_stack_size();
}

// -----------------------------------------------------------------------------

bool
Process::has_frame() const
{
  return m_impl->has_frame();
}

// -----------------------------------------------------------------------------

Frame&
Process::top_frame()
{
  return m_impl->top_frame();
}

// -----------------------------------------------------------------------------

const Frame&
Process::top_frame() const
{
  return m_impl->top_frame();
}

// -----------------------------------------------------------------------------

void
Process::top_frame(Frame** frame_ptr)
{
  m_impl->top_frame(frame_ptr);
}

// -----------------------------------------------------------------------------

Frame&
Process::top_nth_frame(size_t n)
{
  return m_impl->top_nth_frame(n);
}

// -----------------------------------------------------------------------------

void
Process::pop_frame()
{
  m_impl->pop_frame();
}

// -----------------------------------------------------------------------------

void
Process::pop_frame_safe()
{
  m_impl->pop_frame_safe();
}

// -----------------------------------------------------------------------------

void
Process::push_frame(Frame& frame)
{
  m_impl->push_frame(frame);
}

// -----------------------------------------------------------------------------

void
Process::emplace_frame(const ClosureCtx& ctx, Compartment* compartment,
  Closure* closure)
{
  m_impl->emplace_frame(ctx, compartment, closure);
}

// -----------------------------------------------------------------------------

void
Process::emplace_frame(const ClosureCtx& ctx, Compartment* compartment,
  Closure* closure, instr_addr_t return_addr)
{
  m_impl->emplace_frame(ctx, compartment, closure, return_addr);
}

// -----------------------------------------------------------------------------

uint64_t
Process::stack_size() const
{
  return m_impl->stack_size();
}

// -----------------------------------------------------------------------------

InvocationCtx&
Process::top_invocation_ctx()
{
  return m_impl->top_invocation_ctx();
}

// -----------------------------------------------------------------------------

void
Process::top_invocation_ctx(InvocationCtx** invk_ctx_ptr)
{
  m_impl->top_invocation_ctx(invk_ctx_ptr);
}

// -----------------------------------------------------------------------------

void
Process::push_invocation_ctx(const InvocationCtx& invk_ctx)
{
  m_impl->push_invocation_ctx(invk_ctx);
}

// -----------------------------------------------------------------------------

void
Process::emplace_invocation_ctx(const ClosureCtx& ctx,
  Compartment* compartment,
  Closure* closure)
{
  m_impl->emplace_invocation_ctx(ctx, compartment, closure);
}

// -----------------------------------------------------------------------------

void
Process::pop_invocation_ctx()
{
  m_impl->pop_invocation_ctx();
}

// -----------------------------------------------------------------------------

Process::dyobj_ptr
Process::top_stack()
{
  return m_impl->top_stack();
}

// -----------------------------------------------------------------------------

void
Process::push_stack(dyobj_ptr obj)
{
  m_impl->push_stack(obj);
}

// -----------------------------------------------------------------------------

Process::dyobj_ptr
Process::pop_stack()
{
  return m_impl->pop_stack();
}

// -----------------------------------------------------------------------------

void
Process::swap_stack()
{
  m_impl->swap_stack();
}

// -----------------------------------------------------------------------------

Process::dynamic_object_heap_type::size_type
Process::heap_size() const
{
  return m_impl->heap_size();
}

// -----------------------------------------------------------------------------

Process::dynamic_object_heap_type::size_type
Process::max_heap_size() const
{
  return m_impl->max_heap_size();
}

// -----------------------------------------------------------------------------

NativeTypesPool::size_type
Process::ntvhndl_pool_size() const
{
  return m_impl->ntvhndl_pool_size();
}

// -----------------------------------------------------------------------------

NativeTypesPool::size_type
Process::max_ntvhndl_pool_size() const
{
  return m_impl->max_ntvhndl_pool_size();
}

// -----------------------------------------------------------------------------

types::NativeTypeHandle&
Process::get_ntvhndl(const types::NativeTypeHandle* hndl)
{
  return m_impl->get_ntvhndl(hndl);
}

// -----------------------------------------------------------------------------

types::NativeTypeHandle*
Process::insert_ntvhndl(const types::NativeTypeHandle& hndl)
{
  return m_impl->insert_ntvhndl(hndl);
}

// -----------------------------------------------------------------------------

void
Process::erase_ntvhndl(const types::NativeTypeHandle* ptr)
{
  m_impl->erase_ntvhndl(ptr);
}

// -----------------------------------------------------------------------------

void
Process::insert_attr_name(dyobj::attr_key_t attr_key, const char* attr_name)
{
  m_impl->insert_attr_name(attr_key, attr_name);
}

// -----------------------------------------------------------------------------

bool
Process::get_attr_name(dyobj::attr_key_t attr_key, const char** attr_name) const
{
  return m_impl->get_attr_name(attr_key, attr_name);
}

// -----------------------------------------------------------------------------

Process::ExecutionStatus
Process::execution_status() const
{
  return m_impl->execution_status();
}

// -----------------------------------------------------------------------------

void
Process::pause_exec()
{
  m_impl->pause_exec();
}

// -----------------------------------------------------------------------------

void
Process::resume_exec()
{
  m_impl->resume_exec();
}

// -----------------------------------------------------------------------------

void
Process::run()
{
  m_impl->run();
}

// -----------------------------------------------------------------------------

void
Process::set_do_gc()
{
  m_impl->set_do_gc();
}

// -----------------------------------------------------------------------------

void
Process::do_gc()
{
  m_impl->do_gc();
}

// -----------------------------------------------------------------------------

void
Process::terminate_exec()
{
  m_impl->terminate_exec();
}

// -----------------------------------------------------------------------------

void
Process::set_gc_flag(uint8_t gc_flag)
{
  m_impl->set_gc_flag(gc_flag);
}

// -----------------------------------------------------------------------------

instr_addr_t
Process::pc() const
{
  return m_impl->pc();
}

// -----------------------------------------------------------------------------

void
Process::set_pc(instr_addr_t addr)
{
  m_impl->set_pc(addr);
}

// -----------------------------------------------------------------------------

bool
Process::get_frame_by_closure_ctx(ClosureCtx& closure_ctx, Frame** frame_ptr)
{
  return m_impl->get_frame_by_closure_ctx(closure_ctx, frame_ptr);
}

// -----------------------------------------------------------------------------

bool
Process::should_gc() const
{
  return m_impl->should_gc();
}

// -----------------------------------------------------------------------------

size_t
Process::compartment_count() const
{
  return m_impl->compartment_count();
}

// -----------------------------------------------------------------------------

compartment_id_t
Process::insert_compartment(const Compartment& compartment)
{
  return m_impl->insert_compartment(compartment);
}

// -----------------------------------------------------------------------------

compartment_id_t
Process::insert_compartment(const Compartment&& compartment)
{
  return m_impl->insert_compartment(compartment);
}

// -----------------------------------------------------------------------------

void
Process::get_compartment(compartment_id_t id, Compartment** ptr)
{
  m_impl->get_compartment(id, ptr);
}

// -----------------------------------------------------------------------------

void
Process::reset()
{
  m_impl->reset();
}

// -----------------------------------------------------------------------------

Frame*
Process::find_frame_by_ctx(ClosureCtx ctx, Compartment* compartment)
{
  return m_impl->find_frame_by_ctx(ctx, compartment);
}

// -----------------------------------------------------------------------------

Frame*
Process::find_parent_frame_in_process(Frame* frame_ptr)
{
  return m_impl->find_parent_frame_in_process(frame_ptr);
}

// -----------------------------------------------------------------------------

void
Process::unwind_stack(size_t limit)
{
  m_impl->unwind_stack(limit);
}

// -----------------------------------------------------------------------------

Process::Printer::Printer(const Process& process, uint32_t opts)
  :
  m_process(process),
  m_opts(opts)
{
}

// -----------------------------------------------------------------------------

std::ostream&
Process::Printer::operator()(std::ostream& ost) const
{
  ost << "Process" << std::endl;
  ost << std::endl;
  ost << "-- BEGIN --" << std::endl;

  ost << "Heap size: " << m_process.heap_size() << std::endl;
  ost << "Max heap size: " << m_process.max_heap_size() << std::endl;
  ost << "Native types pool size: " << m_process.ntvhndl_pool_size() << std::endl;
  ost << "Max native types pool size: " << m_process.max_ntvhndl_pool_size() << std::endl;
  ost << "Compartments: " << m_process.compartment_count() << std::endl;

  for (const auto& compartment : m_process.m_impl.get()->m_compartments)
  {
    CompartmentPrinter printer(compartment, m_opts);
    printer(ost) << std::endl;
  }

  ost << m_process.m_impl->m_dynamic_object_heap << std::endl;
  ost << m_process.m_impl->m_ntvhndl_pool << std::endl;

  ost << "-- END --" << std::endl;
  ost << std::endl;

  return ost;
}

// -----------------------------------------------------------------------------

} /* end namespace runtime */
} /* end namespace corevm */
