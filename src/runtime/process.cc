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
#include "process.h"

#include "closure.h"
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
#include "gc/garbage_collection_scheme.h"

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <list>
#include <ostream>
#include <stack>
#include <stdexcept>
#include <utility>
#include <unordered_map>

#include <setjmp.h>


namespace corevm {


namespace runtime {


namespace internal {


using _GarbageCollectorType = typename corevm::gc::garbage_collector<
  corevm::runtime::process::garbage_collection_scheme>;

// -----------------------------------------------------------------------------

class ntvhndl_collector_gc_callback : public _GarbageCollectorType::callback
{
private:
  using dynamic_object_type = typename _GarbageCollectorType::dynamic_object_type;

public:
  virtual void operator()(const dynamic_object_type& obj)
  {
    this->m_list.push_back(obj.ntvhndl_key());
  }

  const std::list<corevm::dyobj::ntvhndl_key>& list() const
  {
    return m_list;
  }

private:
  std::list<corevm::dyobj::ntvhndl_key> m_list;
};

// -----------------------------------------------------------------------------


} /* end namespace internal */


} /* end namespace runtime */


} /* end namespace corevm */


// -----------------------------------------------------------------------------

corevm::dyobj::dyobj_id
corevm::runtime::process::create_dyobj()
{
  return m_dynamic_object_heap.create_dyobj();
}

// -----------------------------------------------------------------------------

corevm::runtime::process::dynamic_object_type&
corevm::runtime::process::get_dyobj(corevm::dyobj::dyobj_id id)
{
  return m_dynamic_object_heap.at(id);
}

// -----------------------------------------------------------------------------

corevm::runtime::process::process()
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
  m_compartments()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

corevm::runtime::process::process(
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
  m_compartments()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

corevm::runtime::process::~process()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

uint64_t
corevm::runtime::process::call_stack_size() const
{
  return m_call_stack.size();
}

// -----------------------------------------------------------------------------

bool
corevm::runtime::process::has_frame() const
{
  return !(this->m_call_stack.empty());
}

// -----------------------------------------------------------------------------

corevm::runtime::frame&
corevm::runtime::process::top_frame() throw(corevm::runtime::frame_not_found_error)
{
  if (m_call_stack.empty())
  {
    THROW(corevm::runtime::frame_not_found_error());
  }

  return m_call_stack.back();
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::pop_frame() throw(corevm::runtime::frame_not_found_error)
{
  corevm::runtime::frame& frame = this->top_frame();

  std::list<corevm::dyobj::dyobj_id> visible_objs = frame.get_visible_objs();
  std::list<corevm::dyobj::dyobj_id> invisible_objs = frame.get_invisible_objs();

  std::for_each(
    visible_objs.begin(),
    visible_objs.end(),
    [this](corevm::dyobj::dyobj_id id) {
      auto &obj = this->get_dyobj(id);
      obj.manager().on_exit();
    }
  );

  std::for_each(
    invisible_objs.begin(),
    invisible_objs.end(),
    [this](corevm::dyobj::dyobj_id id) {
      auto &obj = this->get_dyobj(id);
      obj.manager().on_exit();
    }
  );

  set_pc(frame.return_addr());

  const corevm::runtime::closure* closure_ptr = frame.closure_ptr();

  auto begin_itr = m_instrs.begin() + pc() + 1;
  auto end_itr = begin_itr + closure_ptr->vector.size();

  m_instrs.erase(begin_itr, end_itr);

  m_call_stack.pop_back();

  this->pop_invocation_ctx();
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::check_call_stack_capacity()
{
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
corevm::runtime::process::push_frame(corevm::runtime::frame& frame)
{
  check_call_stack_capacity();
  m_call_stack.push_back(frame);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::emplace_frame(
  const corevm::runtime::closure_ctx& ctx,
  corevm::runtime::compartment* compartment_ptr, corevm::runtime::closure* closure_ptr)
{
  ASSERT(compartment_ptr);
  ASSERT(closure_ptr);
  check_call_stack_capacity();
  m_call_stack.emplace_back(ctx, compartment_ptr, closure_ptr);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::emplace_frame(
  const corevm::runtime::closure_ctx& ctx,
  corevm::runtime::compartment* compartment_ptr,
  corevm::runtime::closure* closure_ptr, corevm::runtime::instr_addr return_addr)
{
  ASSERT(compartment_ptr);
  ASSERT(closure_ptr);
  m_call_stack.emplace_back(ctx, compartment_ptr, closure_ptr, return_addr);
}

// -----------------------------------------------------------------------------

uint64_t
corevm::runtime::process::stack_size() const
{
  return m_dyobj_stack.size();
}

// -----------------------------------------------------------------------------

corevm::runtime::invocation_ctx&
corevm::runtime::process::top_invocation_ctx()
  throw(corevm::runtime::invocation_ctx_not_found_error)
{
  if (m_invocation_ctx_stack.empty())
  {
    THROW(invocation_ctx_not_found_error());
  }

  return m_invocation_ctx_stack.back();
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::check_invk_ctx_stack_capacity()
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
corevm::runtime::process::push_invocation_ctx(const invocation_ctx& invk_ctx)
{
  check_invk_ctx_stack_capacity();
  m_invocation_ctx_stack.push_back(invk_ctx);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::emplace_invocation_ctx(
  const corevm::runtime::closure_ctx& ctx,
  corevm::runtime::compartment* compartment_ptr,
  corevm::runtime::closure* closure_ptr)
{
  ASSERT(compartment_ptr);
  ASSERT(closure_ptr);
  check_invk_ctx_stack_capacity();
  m_invocation_ctx_stack.emplace_back(ctx, compartment_ptr, closure_ptr);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::pop_invocation_ctx()
  throw(corevm::runtime::invocation_ctx_not_found_error)
{
  if (m_invocation_ctx_stack.empty())
  {
    THROW(invocation_ctx_not_found_error());
  }

  m_invocation_ctx_stack.pop_back();
}

// -----------------------------------------------------------------------------

const corevm::dyobj::dyobj_id&
corevm::runtime::process::top_stack()
  throw(corevm::runtime::object_stack_empty_error)
{
  if (m_dyobj_stack.empty())
  {
    THROW(corevm::runtime::object_stack_empty_error());
  }

  return m_dyobj_stack.back();
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::push_stack(corevm::dyobj::dyobj_id& id)
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

const corevm::dyobj::dyobj_id
corevm::runtime::process::pop_stack()
  throw(corevm::runtime::object_stack_empty_error)
{
  if (m_dyobj_stack.empty())
  {
    THROW(corevm::runtime::object_stack_empty_error());
  }

  corevm::dyobj::dyobj_id id = m_dyobj_stack.back();
  m_dyobj_stack.pop_back();
  return id;
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::swap_stack()
{
  if (m_dyobj_stack.size() < 2)
  {
    THROW(corevm::runtime::invalid_operation_error(
      "Cannot swap top of object stack"));
  }

  auto& a = m_dyobj_stack[m_dyobj_stack.size() - 1];
  auto& b = m_dyobj_stack[m_dyobj_stack.size() - 2];
  std::swap(a, b);
}

// -----------------------------------------------------------------------------

corevm::runtime::process::dynamic_object_heap_type::size_type
corevm::runtime::process::heap_size() const
{
  return m_dynamic_object_heap.size();
}

// -----------------------------------------------------------------------------

corevm::runtime::process::dynamic_object_heap_type::size_type
corevm::runtime::process::max_heap_size() const
{
  return m_dynamic_object_heap.max_size();
}

// -----------------------------------------------------------------------------

corevm::runtime::process::native_types_pool_type::size_type
corevm::runtime::process::ntvhndl_pool_size() const
{
  return m_ntvhndl_pool.size();
}

// -----------------------------------------------------------------------------

corevm::runtime::process::native_types_pool_type::size_type
corevm::runtime::process::max_ntvhndl_pool_size() const
{
  return m_ntvhndl_pool.max_size();
}

// -----------------------------------------------------------------------------

bool
corevm::runtime::process::has_ntvhndl(corevm::dyobj::ntvhndl_key& key)
{
  try
  {
    m_ntvhndl_pool.at(key);
  }
  catch(const corevm::runtime::native_type_handle_not_found_error&)
  {
    return false;
  }

  return true;
}

// -----------------------------------------------------------------------------

corevm::types::native_type_handle&
corevm::runtime::process::get_ntvhndl(corevm::dyobj::ntvhndl_key key)
  throw(corevm::runtime::native_type_handle_not_found_error)
{
  return m_ntvhndl_pool.at(key);
}

// -----------------------------------------------------------------------------

corevm::dyobj::ntvhndl_key
corevm::runtime::process::insert_ntvhndl(corevm::types::native_type_handle& hndl)
  throw(corevm::runtime::native_type_handle_insertion_error)
{
  return m_ntvhndl_pool.create(hndl);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::erase_ntvhndl(corevm::dyobj::ntvhndl_key key)
  throw(corevm::runtime::native_type_handle_deletion_error)
{
  if (key == corevm::dyobj::NONESET_NTVHNDL_KEY)
  {
    return;
  }

  try
  {
    m_ntvhndl_pool.erase(key);
  }
  catch(const corevm::runtime::native_type_handle_not_found_error)
  {
    THROW(corevm::runtime::native_type_handle_deletion_error());
  }
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::pause_exec()
{
  m_pause_exec = true;
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::resume_exec()
{
  m_pause_exec = false;
}

// -----------------------------------------------------------------------------

inline bool
corevm::runtime::process::is_valid_pc() const
{
  return m_pc != NONESET_INSTR_ADDR && (m_pc >= 0 && m_pc < m_instrs.size());
}

// -----------------------------------------------------------------------------

inline bool
corevm::runtime::process::can_execute()
{
  return is_valid_pc();
}

// -----------------------------------------------------------------------------

bool
corevm::runtime::process::pre_start()
{
  if (m_compartments.empty())
  {
    return false;
  }

  corevm::runtime::closure* closure = nullptr;
  corevm::runtime::compartment* compartment = &m_compartments.front();

  bool res = compartment->get_starting_closure(&closure);

  // If we found the starting compartment and closure, create a frame with the
  // closure context, and loads and vector into the process.
  if (res)
  {
#if __DEBUG__
    ASSERT(closure);
#endif

    corevm::runtime::closure_ctx ctx {
      .compartment_id = 0,
      .closure_id = closure->id
    };

    append_vector(closure->vector);

    emplace_frame(ctx, compartment, closure, m_pc);

    emplace_invocation_ctx(ctx, compartment, closure);

    m_pc = 0;

    m_dyobj_stack.reserve(100);
  }

  return res;
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::start()
{
  if (!pre_start())
  {
    return;
  }

  while (can_execute())
  {
    while (m_pause_exec) {}

    const corevm::runtime::instr& instr = m_instrs[m_pc];

    auto& handler = corevm::runtime::instr_handler_meta::instr_set[instr.code].handler;

    handler->execute(instr, *this);

    /**
     * TODO: [COREVM-246] Enable support for signal handling mechanism
     *
    sigsetjmp(corevm::runtime::sighandler_registrar::get_sigjmp_env(), 1);

    if (!corevm::runtime::sighandler_registrar::is_sig_raised())
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

    corevm::runtime::sighandler_registrar::clear_sig_raised();
    *
    **/

    ++m_pc;

  } /* end `while (can_execute())` */
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::maybe_gc()
{
  if (!(this->should_gc()))
  {
    return;
  }

  do_gc();
};

// -----------------------------------------------------------------------------

void
corevm::runtime::process::do_gc()
{
  this->pause_exec();

  corevm::gc::garbage_collector<garbage_collection_scheme> garbage_collector(
    m_dynamic_object_heap);

  corevm::runtime::internal::ntvhndl_collector_gc_callback callback;
  garbage_collector.gc(&callback);

  std::for_each(
    callback.list().begin(),
    callback.list().end(),
    [&](corevm::dyobj::ntvhndl_key key) {
      this->erase_ntvhndl(key);
    }
  );

  this->resume_exec();
}

// -----------------------------------------------------------------------------

const corevm::runtime::instr_addr
corevm::runtime::process::pc() const
{
  return m_pc;
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::set_pc(const corevm::runtime::instr_addr addr)
  throw(corevm::runtime::invalid_instr_addr_error)
{
  if ( addr != corevm::runtime::NONESET_INSTR_ADDR &&
      (addr < 0 || addr >= m_instrs.size()) )
  {
    THROW(corevm::runtime::invalid_instr_addr_error());
  }

  m_pc = addr;
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::append_vector(const corevm::runtime::vector& vector)
{
  // Inserts the vector at the very end of the instr array.
  // (This is different than `process::insert_vector`.
  //
  // NOTE: Please update `process_unittest::TestAppendVector` if the
  // behavior here changes.
  std::copy(vector.begin(), vector.end(), std::back_inserter(m_instrs));
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::insert_vector(const corevm::runtime::vector& vector)
{
  // We want to insert the vector right after the current pc().
  //
  // NOTE: Please update `process_unittest::TestInsertVector` if the
  // behavior here changes.
  m_instrs.insert(m_instrs.begin() + pc() + 1, vector.begin(), vector.end());
}

// -----------------------------------------------------------------------------

bool
corevm::runtime::process::get_frame_by_closure_ctx(
  corevm::runtime::closure_ctx& closure_ctx, corevm::runtime::frame** frame_ptr)
{
  auto itr = std::find_if(
    m_call_stack.begin(),
    m_call_stack.end(),
    [&closure_ctx](const corevm::runtime::frame& frame) -> bool {
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
corevm::runtime::process::should_gc() const
{
  if (!m_gc_flag)
  {
    return false;
  }

  size_t flag_size = sizeof(m_gc_flag) * sizeof(char);

  for (int i = 0; i < flag_size; ++i)
  {
    bool bit_set = is_bit_set(m_gc_flag, i);

    if (!bit_set)
    {
      continue;
    }

    corevm::runtime::gc_rule_meta::gc_bitfields bit =
      static_cast<corevm::runtime::gc_rule_meta::gc_bitfields>(i);

    const corevm::runtime::gc_rule* gc_rule =
      corevm::runtime::gc_rule_meta::get_gc_rule(bit);

    if (gc_rule &&
        gc_rule->should_gc(const_cast<const corevm::runtime::process&>(*this)))
    {
      return true;
    }
  }

  return false;
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::set_sig_vector(
  sig_atomic_t sig, corevm::runtime::vector& vector)
{
  m_sig_instr_map.insert({sig, vector});
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::handle_signal(
  sig_atomic_t sig, corevm::runtime::sighandler* handler)
{
  auto itr = m_sig_instr_map.find(sig);

  if (itr != m_sig_instr_map.end())
  {
    corevm::runtime::vector vector = itr->second;
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

corevm::runtime::compartment_id
corevm::runtime::process::insert_compartment(
  const corevm::runtime::compartment& compartment)
{
  m_compartments.push_back(compartment);
  return static_cast<corevm::runtime::compartment_id>(m_compartments.size() - 1);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::get_compartment(
  corevm::runtime::compartment_id id, corevm::runtime::compartment** ptr)
{
  if (id < m_compartments.size())
  {
    *ptr = &m_compartments[id];
  }
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::reset()
{
  m_gc_flag = 0;
  m_pc = corevm::runtime::NONESET_INSTR_ADDR;
  m_dyobj_stack.clear();
  m_call_stack.clear();
  m_invocation_ctx_stack.clear();
  m_compartments.clear();
}

// -----------------------------------------------------------------------------

corevm::runtime::frame*
corevm::runtime::process::find_frame_by_ctx(
  corevm::runtime::closure_ctx ctx,
  corevm::runtime::compartment* compartment,
  corevm::runtime::process& process)
{
#if __DEBUG__
  ASSERT(compartment);
#endif

  corevm::runtime::frame* frame = nullptr;

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

    corevm::runtime::closure *closure = nullptr;
    compartment->get_closure_by_id(ctx.closure_id, &closure);

#if __DEBUG__
    ASSERT(closure);
#endif

    ctx.closure_id = closure->parent_id;

    if (ctx.closure_id == corevm::runtime::NONESET_CLOSURE_ID)
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

corevm::runtime::frame*
corevm::runtime::process::find_parent_frame_in_process(
  corevm::runtime::frame* frame_ptr,
  corevm::runtime::process& process)
{
#if __DEBUG__
  ASSERT(frame_ptr);
#endif

  const corevm::runtime::closure* closure = frame_ptr->closure_ptr();

#if __DEBUG__
  ASSERT(closure);
#endif

  corevm::runtime::closure_id parent_closure_id = closure->parent_id;

  if (parent_closure_id == corevm::runtime::NONESET_CLOSURE_ID)
  {
    return nullptr;
  }

  closure_ctx ctx {
    .compartment_id = frame_ptr->closure_ctx().compartment_id,
    .closure_id = parent_closure_id
  };

  corevm::runtime::compartment* compartment = frame_ptr->compartment_ptr();

#if __DEBUG__
  ASSERT(compartment);
#endif

  frame_ptr = corevm::runtime::process::find_frame_by_ctx(
    ctx, compartment, process);

  return frame_ptr;
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::unwind_stack(
  corevm::runtime::process& process, size_t limit)
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

    corevm::runtime::frame& frame = process.top_frame();

    const corevm::runtime::compartment* compartment = frame.compartment_ptr();

    line_ss << "    " << "File " << '\"' << compartment->path() << '\"';

    const corevm::runtime::closure* closure = frame.closure_ptr();

    const corevm::runtime::loc_table& locs = closure->locs;

    int32_t index = process.pc() - frame.return_addr();

    if (locs.find(index) != locs.end())
    {
      const corevm::runtime::loc_info& loc = locs.at(index);

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

namespace corevm {


namespace runtime {


std::ostream& operator<<(
  std::ostream& ost, const corevm::runtime::process& process)
{
  ost << "Process" << std::endl;
  ost << std::endl;
  ost << "-- BEGIN --" << std::endl;
  ost << "Heap size: " << process.heap_size() << std::endl;
  ost << "Max heap size: " << process.max_heap_size() << std::endl;
  ost << "Native types pool size: " << process.ntvhndl_pool_size() << std::endl;
  ost << "Max native types pool size: " << process.max_ntvhndl_pool_size() << std::endl;
  ost << "Compartments: " << process.m_compartments.size() << std::endl;
  ost << std::endl;

  for (auto itr = process.m_compartments.cbegin();
       itr != process.m_compartments.cend();
       ++itr)
  {
    const corevm::runtime::compartment& compartment = *itr;
    ost << compartment << std::endl;
  }

  ost << "Total Instructions: " << process.m_instrs.size() << std::endl;
  ost << "Program counter: " << process.pc() << std::endl;
  ost << "-- END --" << std::endl;
  ost << std::endl;

  ost << process.m_dynamic_object_heap << std::endl;
  ost << process.m_ntvhndl_pool << std::endl;

  return ost;
}


} /* end namespace runtime */


} /* end namespace corevm */

// -----------------------------------------------------------------------------
