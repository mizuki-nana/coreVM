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
#include <sneaker/threading/fixed_time_interval_daemon_service.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <stdexcept>

#include <setjmp.h>


namespace corevm {


namespace runtime {


namespace internal {


using _GarbageCollectorType = typename corevm::gc::garbage_collector<
  corevm::runtime::process::garbage_collection_scheme>;


class garbage_collector_callback : public _GarbageCollectorType::callback
{
private:
  using dynamic_object_type = typename _GarbageCollectorType::dynamic_object_type;

public:
  virtual void operator()(const dynamic_object_type& obj)
  {
    this->m_list.push_back(obj.get_ntvhndl_key());
  }

  const std::list<corevm::dyobj::ntvhndl_key>& list() const {
    return m_list;
  }

private:
  std::list<corevm::dyobj::ntvhndl_key> m_list;
};


} /* end namespace internal */


} /* end namespace runtime */


} /* end namespace corevm */


corevm::dyobj::dyobj_id
corevm::runtime::process::adapter::help_create_dyobj()
{
  return m_process.m_dynamic_object_heap.create_dyobj();
}

corevm::runtime::process::dynamic_object_type&
corevm::runtime::process::adapter::help_get_dyobj(corevm::dyobj::dyobj_id id)
{
  return m_process.m_dynamic_object_heap.at(id);
}


const int COREVM_PROCESS_DEFAULT_PAUSE_TIME = 10;


const int COREVM_PROCESS_DEFAULT_MAX_RUN_ITERATIONS = -1;


corevm::runtime::process::process():
  sneaker::threading::fixed_time_interval_daemon_service(
    COREVM_PROCESS_DEFAULT_PAUSE_TIME,
    corevm::runtime::process::tick_handler,
    false,
    COREVM_PROCESS_DEFAULT_MAX_RUN_ITERATIONS
  ),
  m_pause_exec(false),
  m_gc_flag(0),
  m_pc(0)
{
  // Do nothing here.
}

corevm::runtime::process::process(const uint16_t interval):
  sneaker::threading::fixed_time_interval_daemon_service(
    interval,
    corevm::runtime::process::tick_handler,
    false,
    COREVM_PROCESS_DEFAULT_MAX_RUN_ITERATIONS
  ),
  m_pause_exec(false),
  m_gc_flag(0),
  m_pc(0)
{
  // Do nothing here.
}

corevm::runtime::process::~process()
{
  // Do nothing here.
}

const corevm::runtime::instr_addr
corevm::runtime::process::top_addr() const
{
  return m_instrs.size() - 1;
}

const corevm::runtime::instr_addr
corevm::runtime::process::current_addr() const
{
  return m_pc;
}

uint64_t
corevm::runtime::process::call_stack_size() const
{
  return m_call_stack.size();
}

bool
corevm::runtime::process::has_frame() const
{
  return !(this->m_call_stack.empty());
}

corevm::runtime::frame&
corevm::runtime::process::top_frame() throw(corevm::runtime::frame_not_found_error)
{
  if (m_call_stack.empty()) {
    throw corevm::runtime::frame_not_found_error();
  }

  return m_call_stack.top();
}

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

  m_call_stack.pop();
}

void
corevm::runtime::process::push_frame(corevm::runtime::frame& frame)
{
  m_call_stack.push(frame);
}

uint64_t
corevm::runtime::process::stack_size() const
{
  return m_dyobj_stack.size();
}

const corevm::dyobj::dyobj_id&
corevm::runtime::process::top_stack() throw(corevm::runtime::object_stack_empty_error)
{
  if (m_dyobj_stack.empty()) {
    throw corevm::runtime::object_stack_empty_error();
  }

  return m_dyobj_stack.top();
}

void
corevm::runtime::process::push_stack(corevm::dyobj::dyobj_id& id)
{
  m_dyobj_stack.push(id);
}

const corevm::dyobj::dyobj_id
corevm::runtime::process::pop_stack() throw(corevm::runtime::object_stack_empty_error)
{
  if (m_dyobj_stack.empty()) {
    throw corevm::runtime::object_stack_empty_error();
  }

  corevm::dyobj::dyobj_id id = m_dyobj_stack.top();
  m_dyobj_stack.pop();
  return id;
}

corevm::runtime::process::dynamic_object_heap_type::size_type
corevm::runtime::process::heap_size() const
{
  return m_dynamic_object_heap.size();
}

corevm::runtime::process::dynamic_object_heap_type::size_type
corevm::runtime::process::max_heap_size() const
{
  return m_dynamic_object_heap.max_size();
}

corevm::runtime::process::native_types_pool_type::size_type
corevm::runtime::process::ntvhndl_pool_size() const
{
  return m_ntvhndl_pool.size();
}

corevm::runtime::process::native_types_pool_type::size_type
corevm::runtime::process::max_ntvhndl_pool_size() const
{
  return m_ntvhndl_pool.max_size();
}

bool
corevm::runtime::process::has_ntvhndl(corevm::dyobj::ntvhndl_key& key)
{
  try {
    m_ntvhndl_pool.at(key);
  } catch(const corevm::runtime::native_type_handle_not_found_error&) {
    return false;
  }

  return true;
}

corevm::types::native_type_handle&
corevm::runtime::process::get_ntvhndl(corevm::dyobj::ntvhndl_key& key)
  throw(corevm::runtime::native_type_handle_not_found_error)
{
  return m_ntvhndl_pool.at(key);
}

corevm::dyobj::ntvhndl_key
corevm::runtime::process::insert_ntvhndl(corevm::types::native_type_handle& hndl)
  throw(corevm::runtime::native_type_handle_insertion_error)
{
  auto key = m_ntvhndl_pool.create();

  corevm::types::native_type_handle& hndl_ = m_ntvhndl_pool.at(key);
  hndl_ = hndl;

  return key;
}

void
corevm::runtime::process::erase_ntvhndl(corevm::dyobj::ntvhndl_key& key)
  throw(corevm::runtime::native_type_handle_deletion_error)
{
  try {
    m_ntvhndl_pool.erase(key);
  } catch(const corevm::runtime::native_type_handle_not_found_error) {
    throw corevm::runtime::native_type_handle_deletion_error();
  }
}

const corevm::runtime::instr_handler*
corevm::runtime::process::get_instr_handler(corevm::runtime::instr_code code)
{
  corevm::runtime::instr_info instr_info = m_instr_handler_meta.find(code);
  return instr_info.handler;
}

void
corevm::runtime::process::pause_exec()
{
  m_pause_exec = true;
}

void
corevm::runtime::process::resume_exec()
{
  m_pause_exec = false;
}

bool
corevm::runtime::process::can_execute()
{
  return m_pc < m_instrs.size();
}

bool
corevm::runtime::process::start()
{
  bool res = sneaker::threading::fixed_time_interval_daemon_service::start();

  while (can_execute()) {
    while (m_pause_exec) {}

    corevm::runtime::instr instr = static_cast<corevm::runtime::instr>(m_instrs[m_pc]);

    corevm::runtime::instr_handler* handler =
      const_cast<corevm::runtime::instr_handler*>(this->get_instr_handler(instr.code));

    sigsetjmp(corevm::runtime::sighandler_registrar::get_sigjmp_env(), 1);

    if (!corevm::runtime::sighandler_registrar::sig_raised) {
      handler->execute(instr, *this);
    } else {
      if (!this->can_execute()) {
        break;
      }
    }
    corevm::runtime::sighandler_registrar::sig_raised = false;

    ++m_pc;
  }

  return res;
}

void
corevm::runtime::process::maybe_gc()
{
  if (!(this->should_gc())) {
    return;
  }

  this->pause_exec();

  corevm::gc::garbage_collector<garbage_collection_scheme> garbage_collector(
    m_dynamic_object_heap
  );
  corevm::runtime::internal::garbage_collector_callback callback;
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

void
corevm::runtime::process::set_pc(const corevm::runtime::instr_addr addr)
  throw(corevm::runtime::invalid_instr_addr_error)
{
  if (addr >= m_instrs.size()) {
    throw corevm::runtime::invalid_instr_addr_error();
  }

  m_pc = addr;

  while (this->has_frame()) {
    corevm::runtime::frame& frame = this->top_frame();

    if (! (frame.get_start_addr() > addr) ) {
      break;
    }

    this->pop_frame();
  }
}

void
corevm::runtime::process::append_instrs(const std::vector<corevm::runtime::instr>& instrs)
{
  m_instrs.insert(m_instrs.end(), instrs.begin(), instrs.end());
}

void
corevm::runtime::process::append_vector(const corevm::runtime::vector& vector)
{
  m_vectors.push_back(vector);
}

corevm::runtime::closure_id
corevm::runtime::process::get_new_closure_id()
{
  corevm::runtime::closure_id id = m_closure_id_incrementor;
  ++m_closure_id_incrementor;
  return id;
}

void
corevm::runtime::process::insert_closure(const corevm::runtime::closure closure)
{
  m_closure_table[closure.id] = closure;
}

size_t
corevm::runtime::process::closure_count() const
{
  return m_closure_table.size();
}

bool
corevm::runtime::process::should_gc()
{
  size_t flag_size = sizeof(m_gc_flag) * sizeof(char);

  for (int i = 0; i < flag_size; ++i) {
    bool bit_set = is_bit_set(m_gc_flag, i);

    if (!bit_set) {
      continue;
    }

    corevm::runtime::gc_rule_meta::gc_bitfields bit = static_cast<corevm::runtime::gc_rule_meta::gc_bitfields>(i);
    const corevm::runtime::gc_rule* gc_rule = corevm::runtime::gc_rule_meta::get_gc_rule(bit);

    if (gc_rule && gc_rule->should_gc(const_cast<const corevm::runtime::process&>(*this))) {
      return true;
    }
  }

  return false;
}

void
corevm::runtime::process::tick_handler(void* arg)
{
  assert(arg);
  corevm::runtime::process* process = static_cast<corevm::runtime::process*>(arg);
  process->maybe_gc();
}

void
corevm::runtime::process::set_encoding_key_value_pair(uint64_t key, const std::string& value)
{
  m_encoding_map.insert({key, value});
}

void
corevm::runtime::process::set_sig_vector(
  sig_atomic_t sig, corevm::runtime::vector& vector)
{
  m_sig_instr_map.insert({sig, vector});
}

void
corevm::runtime::process::insert_vector(corevm::runtime::vector& block)
{
  std::vector<corevm::runtime::instr>::iterator pos = m_instrs.begin() + m_pc + 1;
  m_instrs.insert(pos, block.begin(), block.end());
}

void
corevm::runtime::process::handle_signal(
  sig_atomic_t sig, corevm::runtime::sighandler* handler)
{
  auto itr = m_sig_instr_map.find(sig);

  if (itr != m_sig_instr_map.end()) {
    corevm::runtime::vector vector = itr->second;
    this->pause_exec();
    this->insert_vector(vector);
    this->resume_exec();

  } else if (handler != nullptr) {
    handler->handle_signal(sig, *this);
  }
}
