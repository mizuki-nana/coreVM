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
#include "../../include/runtime/process.h"

#include "../../include/dyobj/common.h"
#include "../../include/dyobj/dynamic_object_heap.h"
#include "../../include/gc/garbage_collector.h"
#include "../../include/gc/garbage_collection_scheme.h"
#include "../../include/runtime/closure.h"
#include "../../include/runtime/common.h"
#include "../../include/runtime/errors.h"
#include "../../include/runtime/frame.h"
#include "../../include/runtime/gc_rule.h"
#include "../../include/runtime/instr.h"
#include "../../include/runtime/native_types_pool.h"
#include "../../include/runtime/sighandler_registrar.h"
#include "../../include/runtime/vector.h"

#include <sneaker/atomic/atomic_incrementor.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <list>
#include <stack>
#include <stdexcept>
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
    this->m_list.push_back(obj.get_ntvhndl_key());
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


corevm::dyobj::dyobj_id
corevm::runtime::process::adapter::help_create_dyobj()
{
  return m_process.m_dynamic_object_heap.create_dyobj();
}

// -----------------------------------------------------------------------------

corevm::runtime::process::dynamic_object_type&
corevm::runtime::process::adapter::help_get_dyobj(corevm::dyobj::dyobj_id id)
{
  return m_process.m_dynamic_object_heap.at(id);
}

// -----------------------------------------------------------------------------

corevm::runtime::process::process()
  :
  m_pause_exec(false),
  m_gc_flag(0),
  m_pc(0),
  m_dynamic_object_heap(),
  m_dyobj_stack(),
  m_call_stack(),
  m_ntvhndl_pool(),
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

const corevm::runtime::instr_addr
corevm::runtime::process::current_addr() const
{
  return m_pc;
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
    throw corevm::runtime::frame_not_found_error();
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
      auto &obj = corevm::runtime::process::adapter(*this).help_get_dyobj(id);
      obj.manager().on_exit();
    }
  );

  std::for_each(
    invisible_objs.begin(),
    invisible_objs.end(),
    [this](corevm::dyobj::dyobj_id id) {
      auto &obj = corevm::runtime::process::adapter(*this).help_get_dyobj(id);
      obj.manager().on_exit();
    }
  );

  m_call_stack.pop_back();
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::push_frame(corevm::runtime::frame& frame)
{
  m_call_stack.push_back(frame);
}

// -----------------------------------------------------------------------------

uint64_t
corevm::runtime::process::stack_size() const
{
  return m_dyobj_stack.size();
}

// -----------------------------------------------------------------------------

const corevm::dyobj::dyobj_id&
corevm::runtime::process::top_stack() throw(corevm::runtime::object_stack_empty_error)
{
  if (m_dyobj_stack.empty())
  {
    throw corevm::runtime::object_stack_empty_error();
  }

  return m_dyobj_stack.top();
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::push_stack(corevm::dyobj::dyobj_id& id)
{
  m_dyobj_stack.push(id);
}

// -----------------------------------------------------------------------------

const corevm::dyobj::dyobj_id
corevm::runtime::process::pop_stack() throw(corevm::runtime::object_stack_empty_error)
{
  if (m_dyobj_stack.empty())
  {
    throw corevm::runtime::object_stack_empty_error();
  }

  corevm::dyobj::dyobj_id id = m_dyobj_stack.top();
  m_dyobj_stack.pop();
  return id;
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
corevm::runtime::process::get_ntvhndl(corevm::dyobj::ntvhndl_key& key)
  throw(corevm::runtime::native_type_handle_not_found_error)
{
  return m_ntvhndl_pool.at(key);
}

// -----------------------------------------------------------------------------

corevm::dyobj::ntvhndl_key
corevm::runtime::process::insert_ntvhndl(corevm::types::native_type_handle& hndl)
  throw(corevm::runtime::native_type_handle_insertion_error)
{
  auto key = m_ntvhndl_pool.create();

  corevm::types::native_type_handle& hndl_ = m_ntvhndl_pool.at(key);
  hndl_ = hndl;

  return key;
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::erase_ntvhndl(corevm::dyobj::ntvhndl_key& key)
  throw(corevm::runtime::native_type_handle_deletion_error)
{
  try
  {
    m_ntvhndl_pool.erase(key);
  }
  catch(const corevm::runtime::native_type_handle_not_found_error)
  {
    throw corevm::runtime::native_type_handle_deletion_error();
  }
}

// -----------------------------------------------------------------------------

const corevm::runtime::instr_handler*
corevm::runtime::process::get_instr_handler(corevm::runtime::instr_code code)
{
  corevm::runtime::instr_info instr_info = corevm::runtime::instr_handler_meta::find(code);
  return instr_info.handler.get();
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

bool
corevm::runtime::process::can_execute()
{
  return m_pc < m_instrs.size();
}

// -----------------------------------------------------------------------------

bool
corevm::runtime::process::pre_start()
{
  corevm::runtime::closure closure;

  if (m_compartments.empty())
  {
    return false;
  }

  bool res = m_compartments.front().get_starting_closure(&closure);

  if (res)
  {
    insert_vector(closure.vector);
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

    corevm::runtime::instr instr = static_cast<corevm::runtime::instr>(m_instrs[m_pc]);

    corevm::runtime::instr_handler* handler =
      const_cast<corevm::runtime::instr_handler*>(this->get_instr_handler(instr.code));

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
};

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
  if (addr >= m_instrs.size())
  {
    throw corevm::runtime::invalid_instr_addr_error();
  }

  m_pc = addr;

  m_instrs.erase(m_instrs.begin() + pc(), m_instrs.end());
}

// -----------------------------------------------------------------------------

void
corevm::runtime::process::append_vector(const corevm::runtime::vector& vector)
{
  m_instrs.insert(m_instrs.end(), vector.begin(), vector.end());
}

// -----------------------------------------------------------------------------

void
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
  }
}

// -----------------------------------------------------------------------------

bool
corevm::runtime::process::should_gc()
{
  size_t flag_size = sizeof(m_gc_flag) * sizeof(char);

  for (int i = 0; i < flag_size; ++i)
  {
    bool bit_set = is_bit_set(m_gc_flag, i);

    if (!bit_set)
    {
      continue;
    }

    corevm::runtime::gc_rule_meta::gc_bitfields bit = static_cast<corevm::runtime::gc_rule_meta::gc_bitfields>(i);
    const corevm::runtime::gc_rule* gc_rule = corevm::runtime::gc_rule_meta::get_gc_rule(bit);

    if (gc_rule && gc_rule->should_gc(const_cast<const corevm::runtime::process&>(*this)))
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
corevm::runtime::process::insert_vector(corevm::runtime::vector& vector)
{
  std::vector<corevm::runtime::instr>::iterator pos = m_instrs.begin() + m_pc + 1;
  m_instrs.insert(pos, vector.begin(), vector.end());
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

size_t
corevm::runtime::process::insert_compartment(
  const corevm::runtime::compartment& compartment)
{
  m_compartments.push_back(compartment);
  return m_compartments.size();
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
