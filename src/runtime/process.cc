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

class ntvhndl_collector_gc_callback :
  public gc::garbage_collector<process::garbage_collection_scheme>::callback
{
public:
  virtual void operator()(const dynamic_object_type& obj);

  const std::vector<dyobj::ntvhndl_key>& list() const
  {
    return m_ntvhndl_keys;
  }

private:
  std::vector<dyobj::ntvhndl_key> m_ntvhndl_keys;
};

// -----------------------------------------------------------------------------

/* virtual */
void
ntvhndl_collector_gc_callback::operator()(const dynamic_object_type& obj)
{
  m_ntvhndl_keys.push_back(obj.ntvhndl_key());
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
  std::numeric_limits<runtime::vector::size_type>::max() >=
  std::numeric_limits<instr_addr>::max(),
  "Vector size incompatibility"
);

// -----------------------------------------------------------------------------

static_assert(
  std::numeric_limits<std::vector<compartment>::size_type>::max() >=
  std::numeric_limits<compartment_id>::max(),
  "Compartment ID incompatibility"
);

// -----------------------------------------------------------------------------

process::options::options()
  :
  heap_alloc_size(dyobj::COREVM_DEFAULT_HEAP_SIZE),
  pool_alloc_size(COREVM_DEFAULT_NATIVE_TYPES_POOL_SIZE),
  gc_flag(gc_rule_meta::DEFAULT_GC_FLAGS)
{
}

// -----------------------------------------------------------------------------

dyobj::dyobj_id
process::create_dyobj()
{
  return m_dynamic_object_heap.create_dyobj();
}

// -----------------------------------------------------------------------------

process::dynamic_object_type*
process::create_dyobjs(size_t n)
{
  return m_dynamic_object_heap.create_dyobjs(n);
}

// -----------------------------------------------------------------------------

process::dynamic_object_type&
process::get_dyobj(dyobj::dyobj_id id)
{
  return m_dynamic_object_heap.at(id);
}

// -----------------------------------------------------------------------------

process::process()
  :
  m_pause_exec(false),
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

process::process(
  uint64_t heap_alloc_size, uint64_t pool_alloc_size)
  :
  m_pause_exec(false),
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

process::process(const process::options& options)
  :
  m_pause_exec(false),
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
process::init()
{
  m_compartments.reserve(DEFAULT_COMPARTMENTS_TABLE_CAPACITY);

  // Initialize gc rules.
  const size_t flag_size = sizeof(m_gc_flag) * sizeof(char);

  for (size_t i = 0; i < flag_size; ++i)
  {
    // Add 1 to bit since the gc flag values are 0 index based.
    if (is_bit_set(m_gc_flag, static_cast<char>(i) + 1))
    {
      gc_rule_meta::gc_bitfields bit =
        static_cast<gc_rule_meta::gc_bitfields>(i);

      const gc_rule_ptr gc_rule =
        gc_rule_meta::get_gc_rule(bit);

      m_gc_rules.push_back(gc_rule);
    }
  }
}

// -----------------------------------------------------------------------------

process::~process()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

uint64_t
process::call_stack_size() const
{
  return m_call_stack.size();
}

// -----------------------------------------------------------------------------

bool
process::has_frame() const
{
  return !(this->m_call_stack.empty());
}

// -----------------------------------------------------------------------------

frame&
process::top_frame()
  throw(frame_not_found_error)
{
  if (m_call_stack.empty())
  {
    THROW(frame_not_found_error());
  }

  return m_call_stack.back();
}

// -----------------------------------------------------------------------------

void
process::top_frame(frame** frame_ptr)
{
  *frame_ptr = &m_call_stack.back();
}

// -----------------------------------------------------------------------------

frame&
process::top_nth_frame(size_t n)
  throw(frame_not_found_error)
{
  if (n >= m_call_stack.size())
  {
    THROW(frame_not_found_error());
  }

  return m_call_stack[m_call_stack.size() - n - 1];
}

// -----------------------------------------------------------------------------

void
process::pop_frame()
  throw(frame_not_found_error)
{
  frame& frame = this->top_frame();

  std::list<dyobj::dyobj_id> visible_objs = frame.get_visible_objs();
  std::list<dyobj::dyobj_id> invisible_objs = frame.get_invisible_objs();

  std::for_each(
    visible_objs.begin(),
    visible_objs.end(),
    [this](dyobj::dyobj_id id) {
      auto &obj = this->get_dyobj(id);
      obj.manager().on_exit();
    }
  );

  std::for_each(
    invisible_objs.begin(),
    invisible_objs.end(),
    [this](dyobj::dyobj_id id) {
      auto &obj = this->get_dyobj(id);
      obj.manager().on_exit();
    }
  );

  set_pc(frame.return_addr());

  const closure* closure_ptr = frame.closure_ptr();

  auto begin_itr = m_instrs.begin() + pc() + 1;
  auto end_itr = begin_itr;
  std::advance(end_itr, closure_ptr->vector.size());

  m_instrs.erase(begin_itr, end_itr);

  m_call_stack.pop_back();

  this->pop_invocation_ctx();
}

// -----------------------------------------------------------------------------

void
process::check_call_stack_capacity()
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
process::push_frame(frame& frame)
{
  check_call_stack_capacity();
  m_call_stack.push_back(frame);

  set_parent_for_top_frame();
}

// -----------------------------------------------------------------------------

void
process::emplace_frame(
  const closure_ctx& ctx,
  compartment* compartment_ptr,
  closure* closure_ptr)
{
  ASSERT(compartment_ptr);
  ASSERT(closure_ptr);
  check_call_stack_capacity();
  m_call_stack.emplace_back(ctx, compartment_ptr, closure_ptr);

  set_parent_for_top_frame();
}

// -----------------------------------------------------------------------------

void
process::emplace_frame(
  const closure_ctx& ctx,
  compartment* compartment_ptr,
  closure* closure_ptr, instr_addr return_addr)
{
  ASSERT(compartment_ptr);
  ASSERT(closure_ptr);
  check_call_stack_capacity();
  m_call_stack.emplace_back(ctx, compartment_ptr, closure_ptr, return_addr);

  set_parent_for_top_frame();
}

// -----------------------------------------------------------------------------

void
process::set_parent_for_top_frame()
{
  frame* frame = &m_call_stack.back();
  runtime::frame* parent =
    process::find_parent_frame_in_process(frame, *this);

  frame->set_parent(parent);
}

// -----------------------------------------------------------------------------

uint64_t
process::stack_size() const
{
  return m_dyobj_stack.size();
}

// -----------------------------------------------------------------------------

invocation_ctx&
process::top_invocation_ctx()
  throw(invocation_ctx_not_found_error)
{
  if (m_invocation_ctx_stack.empty())
  {
    THROW(invocation_ctx_not_found_error());
  }

  return m_invocation_ctx_stack.back();
}

// -----------------------------------------------------------------------------

void
process::top_invocation_ctx(
  invocation_ctx** invk_ctx_ptr)
{
  *invk_ctx_ptr = &m_invocation_ctx_stack.back();
}

// -----------------------------------------------------------------------------

void
process::check_invk_ctx_stack_capacity()
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
process::push_invocation_ctx(const invocation_ctx& invk_ctx)
{
  check_invk_ctx_stack_capacity();
  m_invocation_ctx_stack.push_back(invk_ctx);
}

// -----------------------------------------------------------------------------

void
process::emplace_invocation_ctx(
  const closure_ctx& ctx,
  compartment* compartment_ptr,
  closure* closure_ptr)
{
  ASSERT(compartment_ptr);
  ASSERT(closure_ptr);
  check_invk_ctx_stack_capacity();
  m_invocation_ctx_stack.emplace_back(ctx, compartment_ptr, closure_ptr);
}

// -----------------------------------------------------------------------------

void
process::pop_invocation_ctx()
  throw(invocation_ctx_not_found_error)
{
  if (m_invocation_ctx_stack.empty())
  {
    THROW(invocation_ctx_not_found_error());
  }

  m_invocation_ctx_stack.pop_back();
}

// -----------------------------------------------------------------------------

const dyobj::dyobj_id&
process::top_stack()
  throw(object_stack_empty_error)
{
  if (m_dyobj_stack.empty())
  {
    THROW(object_stack_empty_error());
  }

  return m_dyobj_stack.back();
}

// -----------------------------------------------------------------------------

void
process::push_stack(dyobj::dyobj_id& id)
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
  m_dyobj_stack.push_back(id);
}

// -----------------------------------------------------------------------------

dyobj::dyobj_id
process::pop_stack()
  throw(object_stack_empty_error)
{
  if (m_dyobj_stack.empty())
  {
    THROW(object_stack_empty_error());
  }

  dyobj::dyobj_id id = m_dyobj_stack.back();
  m_dyobj_stack.pop_back();
  return id;
}

// -----------------------------------------------------------------------------

void
process::swap_stack()
{
  if (m_dyobj_stack.size() < 2)
  {
    THROW(invalid_operation_error(
      "Cannot swap top of object stack"));
  }

  auto& a = m_dyobj_stack[m_dyobj_stack.size() - 1];
  auto& b = m_dyobj_stack[m_dyobj_stack.size() - 2];
  std::swap(a, b);
}

// -----------------------------------------------------------------------------

process::dynamic_object_heap_type::size_type
process::heap_size() const
{
  return m_dynamic_object_heap.size();
}

// -----------------------------------------------------------------------------

process::dynamic_object_heap_type::size_type
process::max_heap_size() const
{
  return m_dynamic_object_heap.max_size();
}

// -----------------------------------------------------------------------------

process::native_types_pool_type::size_type
process::ntvhndl_pool_size() const
{
  return m_ntvhndl_pool.size();
}

// -----------------------------------------------------------------------------

process::native_types_pool_type::size_type
process::max_ntvhndl_pool_size() const
{
  return m_ntvhndl_pool.max_size();
}

// -----------------------------------------------------------------------------

bool
process::has_ntvhndl(dyobj::ntvhndl_key& key)
{
  try
  {
    m_ntvhndl_pool.at(key);
  }
  catch(const native_type_handle_not_found_error&)
  {
    return false;
  }

  return true;
}

// -----------------------------------------------------------------------------

types::native_type_handle&
process::get_ntvhndl(dyobj::ntvhndl_key key)
  throw(native_type_handle_not_found_error)
{
  return m_ntvhndl_pool.at(key);
}

// -----------------------------------------------------------------------------

dyobj::ntvhndl_key
process::insert_ntvhndl(types::native_type_handle& hndl)
  throw(native_type_handle_insertion_error)
{
  return m_ntvhndl_pool.create(hndl);
}

// -----------------------------------------------------------------------------

void
process::erase_ntvhndl(dyobj::ntvhndl_key key)
  throw(native_type_handle_deletion_error)
{
  if (key == dyobj::NONESET_NTVHNDL_KEY)
  {
    return;
  }

  try
  {
    m_ntvhndl_pool.erase(key);
  }
  catch(const native_type_handle_not_found_error)
  {
    THROW(native_type_handle_deletion_error());
  }
}

// -----------------------------------------------------------------------------

void
process::pause_exec()
{
  m_pause_exec = true;
}

// -----------------------------------------------------------------------------

void
process::resume_exec()
{
  m_pause_exec = false;
}

// -----------------------------------------------------------------------------

inline bool
process::is_valid_pc() const
{
  return (uint64_t)m_pc < m_instrs.size();
}

// -----------------------------------------------------------------------------

inline bool
process::can_execute()
{
  return is_valid_pc();
}

// -----------------------------------------------------------------------------

bool
process::pre_start()
{
  if (m_compartments.empty())
  {
    return false;
  }

  closure* closure = nullptr;
  compartment* compartment = &m_compartments.front();

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

    closure_ctx ctx(0, closure->id);

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
process::start()
{
  if (!pre_start())
  {
    return;
  }

#if __DEBUG__
  ASSERT(!m_call_stack.empty());
  ASSERT(!m_invocation_ctx_stack.empty());
#endif

  frame* frame = &m_call_stack.back();
  invocation_ctx* invk_ctx = &m_invocation_ctx_stack.back();

  runtime::frame** frame_ptr = &frame;
  invocation_ctx** invk_ctx_ptr = &invk_ctx;

#if __MEASURE_INSTRS__
  std::array<instr_measurement, INSTR_CODE_MAX> measurements;
  boost::timer::cpu_timer t;
#endif

  while (can_execute())
  {
    while (m_pause_exec) {}

    const instr& instr = m_instrs[static_cast<size_t>(m_pc)];

    auto& handler =
      instr_handler_meta::instr_handlers[instr.code].handler;

#if __MEASURE_INSTRS__
    t.start();
#endif

    handler->execute(instr, *this, frame_ptr, invk_ctx_ptr);

#if __MEASURE_INSTRS__
    t.stop();

    const boost::timer::cpu_times res = t.elapsed();
    measurements[instr.code].cumulative_wall_time += res.wall;
    ++measurements[instr.code].invocation_count;
#endif

    /**
     * TODO: [COREVM-246] Enable support for signal handling mechanism
     *
    sigsetjmp(sighandler_registrar::get_sigjmp_env(), 1);

    if (!sighandler_registrar::is_sig_raised())
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

    sighandler_registrar::clear_sig_raised();
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
process::maybe_gc()
{
  if (should_gc())
  {
    do_gc();
  }
}

// -----------------------------------------------------------------------------

void
process::do_gc()
{
  this->pause_exec();

  gc::garbage_collector<garbage_collection_scheme> garbage_collector(
    m_dynamic_object_heap);

  internal::ntvhndl_collector_gc_callback callback;
  garbage_collector.gc(&callback);

  std::for_each(
    callback.list().begin(),
    callback.list().end(),
    [&](dyobj::ntvhndl_key key) {
      this->erase_ntvhndl(key);
    }
  );

  this->resume_exec();
}

// -----------------------------------------------------------------------------

void
process::set_gc_flag(uint8_t gc_flag)
{
  m_gc_flag = gc_flag;
}

// -----------------------------------------------------------------------------

instr_addr
process::pc() const
{
  return m_pc;
}

// -----------------------------------------------------------------------------

void
process::set_pc(const instr_addr addr)
  throw(invalid_instr_addr_error)
{
  if ( (uint64_t)addr >= m_instrs.size() )
  {
    THROW(invalid_instr_addr_error());
  }

  m_pc = addr;
}

// -----------------------------------------------------------------------------

void
process::append_vector(const vector& vector)
{
  // Inserts the vector at the very end of the instr array.
  std::copy(vector.begin(), vector.end(), std::back_inserter(m_instrs));
}

// -----------------------------------------------------------------------------

void
process::insert_vector(const vector& vector)
{
  // We want to insert the vector right after the current pc().
  //
  // NOTE: changes in the capacity of `m_instr` will result in AddressSanitizer
  // report "heap-use-after-free".
  m_instrs.insert(m_instrs.begin() + pc() + 1, vector.begin(), vector.end());
}

// -----------------------------------------------------------------------------

bool
process::get_frame_by_closure_ctx(
  closure_ctx& closure_ctx, frame** frame_ptr)
{
  auto itr = std::find_if(
    m_call_stack.begin(),
    m_call_stack.end(),
    [&closure_ctx](const frame& frame) -> bool {
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
process::should_gc() const
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
process::set_sig_vector(
  sig_atomic_t sig, vector& vector)
{
  m_sig_instr_map.insert({sig, vector});
}

// -----------------------------------------------------------------------------

void
process::handle_signal(
  sig_atomic_t sig, sighandler* handler)
{
  auto itr = m_sig_instr_map.find(sig);

  if (itr != m_sig_instr_map.end())
  {
    vector vector = itr->second;
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
process::compartment_count() const
{
  return m_compartments.size();
}

// -----------------------------------------------------------------------------

compartment_id
process::insert_compartment(
  const compartment& compartment)
{
  m_compartments.push_back(compartment);
  return static_cast<compartment_id>(m_compartments.size() - 1);
}

// -----------------------------------------------------------------------------

compartment_id
process::insert_compartment(
  const compartment&& compartment)
{
  m_compartments.push_back(
    std::forward<const runtime::compartment>(compartment));

  return static_cast<compartment_id>(m_compartments.size() - 1);
}

// -----------------------------------------------------------------------------

void
process::get_compartment(
  compartment_id id, compartment** ptr)
{
  if (id < static_cast<compartment_id>(m_compartments.size()))
  {
    *ptr = &m_compartments[static_cast<size_t>(id)];
  }
}

// -----------------------------------------------------------------------------

void
process::reset()
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

frame*
process::find_frame_by_ctx(
  closure_ctx ctx,
  compartment* compartment,
  process& process)
{
#if __DEBUG__
  ASSERT(compartment);
#endif

  frame* frame = nullptr;

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

    closure *closure = nullptr;
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

frame*
process::find_parent_frame_in_process(
  frame* frame_ptr,
  process& process)
{
#if __DEBUG__
  ASSERT(frame_ptr);
#endif

  const closure* closure = frame_ptr->closure_ptr();

#if __DEBUG__
  ASSERT(closure);
#endif

  closure_id parent_closure_id = closure->parent_id;

  if (parent_closure_id == NONESET_CLOSURE_ID)
  {
    return nullptr;
  }

  closure_ctx ctx(frame_ptr->closure_ctx().compartment_id, parent_closure_id);

  compartment* compartment = frame_ptr->compartment_ptr();

#if __DEBUG__
  ASSERT(compartment);
#endif

  frame_ptr = process::find_frame_by_ctx(
    ctx, compartment, process);

  return frame_ptr;
}

// -----------------------------------------------------------------------------

void
process::unwind_stack(
  process& process, size_t limit)
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

    frame& frame = process.top_frame();

    const compartment* compartment = frame.compartment_ptr();

    line_ss << "    " << "File " << '\"' << compartment->path() << '\"';

    const closure* closure = frame.closure_ptr();

    const loc_table& locs = closure->locs;

    int32_t index = process.pc() - frame.return_addr();

    if (locs.find(index) != locs.end())
    {
      const loc_info& loc = locs.at(index);

      line_ss << " (" << "line " << loc.lineno << " col " << loc.col_offset << ')';
    }

    line_ss << " in " << closure->name;

    output_lines.push_back(std::move(line_ss.str()));

    process.pop_frame();

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
