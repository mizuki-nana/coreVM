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
#include "frame.h"

#include "closure.h"
#include "common.h"
#include "compartment.h"
#include "errors.h"
#include "corevm/macros.h"
#include "types/native_type_handle.h"

#include <cstdint>


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

Frame::Frame(const runtime::ClosureCtx& closure_ctx,
  Compartment* compartment,
  Closure* closure)
  :
  m_pc(corevm::runtime::NONESET_INSTR_ADDR),
  m_closure_ctx(closure_ctx),
  m_compartment(compartment),
  m_closure(closure),
  m_parent(nullptr),
  m_return_addr(NONESET_INSTR_ADDR),
  m_visible_vars(),
  m_invisible_vars(),
  m_eval_stack(),
  m_exc_obj(NULL)
{
  // Do nothing here.
  m_eval_stack.reserve(100);
}

// -----------------------------------------------------------------------------

Frame::Frame(const runtime::ClosureCtx& closure_ctx,
  Compartment* compartment,
  Closure* closure,
  instr_addr_t return_addr)
  :
  m_pc(corevm::runtime::NONESET_INSTR_ADDR),
  m_closure_ctx(closure_ctx),
  m_compartment(compartment),
  m_closure(closure),
  m_parent(nullptr),
  m_return_addr(return_addr),
  m_visible_vars(),
  m_invisible_vars(),
  m_eval_stack(),
  m_exc_obj(NULL)
{
  // Do nothing here.
  m_eval_stack.reserve(100);
}

// -----------------------------------------------------------------------------

Frame::~Frame()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

size_t
Frame::eval_stack_size() const
{
  return m_eval_stack.size();
}

// -----------------------------------------------------------------------------

instr_addr_t
Frame::pc() const
{
  return m_pc;
}

// -----------------------------------------------------------------------------

void
Frame::set_pc(instr_addr_t addr)
{
  if ( addr != corevm::runtime::NONESET_INSTR_ADDR &&
      (addr < 0 || static_cast<size_t>(addr) >= m_closure->vector.size()) )
  {
    THROW(InvalidInstrAddrError());
  }

  m_pc = addr;
}

// -----------------------------------------------------------------------------

void
Frame::set_pc_safe(instr_addr_t addr)
{
  m_pc = addr;
}

// -----------------------------------------------------------------------------

const Instr&
Frame::current_instr() const
{
  return m_closure->vector[m_pc];
}

// -----------------------------------------------------------------------------

void
Frame::inc_pc()
{
  ++m_pc;
}

// -----------------------------------------------------------------------------

bool
Frame::can_execute() const
{
  return m_pc >= 0 && static_cast<size_t>(m_pc) < m_closure->vector.size();
}

// -----------------------------------------------------------------------------

instr_addr_t
Frame::return_addr() const
{
  return m_return_addr;
}

// -----------------------------------------------------------------------------

void
Frame::set_return_addr(instr_addr_t return_addr)
{
  m_return_addr = return_addr;
}

// -----------------------------------------------------------------------------

void
Frame::push_eval_stack(const types::NativeTypeHandle& operand)
{
  m_eval_stack.push_back(operand);
}

// -----------------------------------------------------------------------------

void
Frame::push_eval_stack(types::NativeTypeHandle&& operand)
{
  m_eval_stack.push_back(
    std::forward<types::NativeTypeHandle>(operand));
}

// -----------------------------------------------------------------------------

types::NativeTypeHandle
Frame::pop_eval_stack()
{
  if (m_eval_stack.empty())
  {
    THROW(EvaluationStackEmptyError());
  }

  types::NativeTypeHandle operand = m_eval_stack.back();
  m_eval_stack.pop_back();
  return operand;
}

// -----------------------------------------------------------------------------

types::NativeTypeHandle&
Frame::top_eval_stack()
{
  if (m_eval_stack.empty())
  {
    THROW(EvaluationStackEmptyError());
  }

  return m_eval_stack.back();
}

// -----------------------------------------------------------------------------

void
Frame::swap_eval_stack()
{
  const size_t eval_stack_size = m_eval_stack.size();

  if (eval_stack_size < 2u)
  {
    THROW(InvalidOperationError(
      "Cannot swap eval stack"));
  }

  auto& a = m_eval_stack[eval_stack_size - 1];
  auto& b = m_eval_stack[eval_stack_size - 2];
  std::swap(a, b);
}

// -----------------------------------------------------------------------------

const std::vector<types::NativeTypeHandle>&
Frame::eval_stack() const
{
  return m_eval_stack;
}

// -----------------------------------------------------------------------------

types::NativeTypeHandle&
Frame::eval_stack_element(size_t i)
{
  return m_eval_stack[i];
}

// -----------------------------------------------------------------------------

size_t
Frame::visible_var_count() const
{
  return m_visible_vars.size();
}

// -----------------------------------------------------------------------------

Frame::dyobj_ptr
Frame::get_visible_var(variable_key_t var_key) const
{
  auto itr = m_visible_vars.find(var_key);
  if (itr == m_visible_vars.end())
  {
    std::string name;
    compartment()->get_string_literal(static_cast<encoding_key_t>(var_key), &name);
    THROW(NameNotFoundError(name.c_str()));
  }

  return itr->second;
}

// -----------------------------------------------------------------------------

bool
Frame::get_visible_var_fast(variable_key_t var_key, dyobj_ptr* obj_ptr) const
{
  auto itr = m_visible_vars.find(var_key);
  if (itr == m_visible_vars.end())
  {
    return false;
  }

  *obj_ptr = itr->second;

  return true;
}

// -----------------------------------------------------------------------------

bool
Frame::get_visible_var_through_ancestry(variable_key_t key, dyobj_ptr* obj_ptr)
{
  dyobj_ptr obj = NULL;

  if (get_visible_var_fast(key, &obj))
  {
    *obj_ptr = obj;
    return true;
  }

  Frame* frame = parent();
  if (!frame)
  {
    return false;
  }

  while (!frame->get_visible_var_fast(key, &obj))
  {
    frame = frame->parent();

    if (!frame)
    {
      return false;
    }
  }

  *obj_ptr = obj;

  return true;
}

// -----------------------------------------------------------------------------

Frame::dyobj_ptr
Frame::pop_visible_var(variable_key_t var_key)
{
  auto ptr = get_visible_var(var_key);
  m_visible_vars.erase(var_key);
  return ptr;
}

// -----------------------------------------------------------------------------

void
Frame::set_visible_var(variable_key_t var_key, dyobj_ptr obj_ptr)
{
  m_visible_vars[var_key] = obj_ptr;
}

// -----------------------------------------------------------------------------

size_t
Frame::invisible_var_count() const
{
  return m_invisible_vars.size();
}

// -----------------------------------------------------------------------------

Frame::dyobj_ptr
Frame::get_invisible_var(variable_key_t var_key) const
{
  auto itr = m_invisible_vars.find(var_key);
  if (itr == m_invisible_vars.end())
  {
    std::string name;
    compartment()->get_string_literal(static_cast<encoding_key_t>(var_key), &name);
    THROW(NameNotFoundError(name.c_str()));
  }

  return itr->second;
}

// -----------------------------------------------------------------------------

bool
Frame::get_invisible_var_fast(variable_key_t var_key, dyobj_ptr* obj_ptr) const
{
  auto itr = m_invisible_vars.find(var_key);
  if (itr == m_invisible_vars.end())
  {
    return false;
  }

  *obj_ptr = itr->second;

  return true;
}

// -----------------------------------------------------------------------------

bool
Frame::get_invisible_var_through_ancestry(variable_key_t key, dyobj_ptr* obj_ptr)
{
  dyobj_ptr obj = NULL;

  if (get_invisible_var_fast(key, &obj))
  {
    *obj_ptr = obj;
    return true;
  }

  Frame* frame = parent();
  if (!frame)
  {
    return false;
  }

  while (!frame->get_invisible_var_fast(key, &obj))
  {
    frame = frame->parent();

    if (!frame)
    {
      return false;
    }
  }

  *obj_ptr = obj;

  return true;
}

// -----------------------------------------------------------------------------

Frame::dyobj_ptr
Frame::pop_invisible_var(variable_key_t var_key)
{
  auto obj_ptr = get_invisible_var(var_key);
  m_invisible_vars.erase(var_key);
  return obj_ptr;
}

// -----------------------------------------------------------------------------

void
Frame::set_invisible_var(variable_key_t var_key, dyobj_ptr obj_ptr)
{
  m_invisible_vars[var_key] = obj_ptr;
}

// -----------------------------------------------------------------------------

std::vector<variable_key_t>
Frame::visible_var_keys() const
{
  std::vector<variable_key_t> keys;
  keys.reserve(m_visible_vars.size());

  for (const auto& pair : m_visible_vars)
  {
    keys.push_back(pair.first);
  }

  return keys;
}

// -----------------------------------------------------------------------------

std::vector<variable_key_t>
Frame::invisible_var_keys() const
{
  std::vector<variable_key_t> keys;
  keys.reserve(m_invisible_vars.size());

  for (const auto& pair : m_invisible_vars)
  {
    keys.push_back(pair.first);
  }

  return keys;
}

// -----------------------------------------------------------------------------

std::vector<Frame::dyobj_ptr>
Frame::get_visible_objs() const
{
  std::vector<dyobj_ptr> obj_ptrs;
  obj_ptrs.reserve(m_visible_vars.size());

  for (auto itr = m_visible_vars.begin(); itr != m_visible_vars.end(); ++itr)
  {
    auto obj_ptr = itr->second;
    obj_ptrs.push_back(obj_ptr);
  }

  return obj_ptrs;
}

// -----------------------------------------------------------------------------

std::vector<Frame::dyobj_ptr>
Frame::get_invisible_objs() const
{
  std::vector<dyobj_ptr> obj_ptrs;
  obj_ptrs.reserve(m_invisible_vars.size());

  for (auto itr = m_invisible_vars.begin(); itr != m_invisible_vars.end(); ++itr)
  {
    auto obj_ptr = itr->second;
    obj_ptrs.push_back(obj_ptr);
  }

  return obj_ptrs;
}

// -----------------------------------------------------------------------------

ClosureCtx
Frame::closure_ctx() const
{
  return m_closure_ctx;
}

// -----------------------------------------------------------------------------

Compartment*
Frame::compartment() const
{
  return m_compartment;
}

// -----------------------------------------------------------------------------

Closure*
Frame::closure() const
{
  return m_closure;
}

// -----------------------------------------------------------------------------

Frame*
Frame::parent() const
{
  return m_parent;
}

// -----------------------------------------------------------------------------

void
Frame::set_parent(Frame* parent)
{
  m_parent = parent;
}

// -----------------------------------------------------------------------------

Frame::dyobj_ptr
Frame::exc_obj() const
{
  return m_exc_obj;
}

// -----------------------------------------------------------------------------

void
Frame::set_exc_obj(Frame::dyobj_ptr exc_obj)
{
  m_exc_obj = exc_obj;
}

// -----------------------------------------------------------------------------

void
Frame::clear_exc_obj()
{
  m_exc_obj = NULL;
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */
