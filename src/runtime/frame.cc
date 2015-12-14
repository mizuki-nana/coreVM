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

frame::frame(
  const runtime::closure_ctx& closure_ctx,
  compartment* compartment_ptr,
  closure* closure_ptr)
  :
  m_closure_ctx(closure_ctx),
  m_compartment_ptr(compartment_ptr),
  m_closure_ptr(closure_ptr),
  m_parent(nullptr),
  m_return_addr(NONESET_INSTR_ADDR),
  m_visible_vars(),
  m_invisible_vars(),
  m_eval_stack(),
  m_exc_obj(0)
{
  // Do nothing here.
  m_eval_stack.reserve(100);
}

// -----------------------------------------------------------------------------

frame::frame(
  const runtime::closure_ctx& closure_ctx,
  compartment* compartment_ptr,
  closure* closure_ptr,
  instr_addr return_addr)
  :
  m_closure_ctx(closure_ctx),
  m_compartment_ptr(compartment_ptr),
  m_closure_ptr(closure_ptr),
  m_parent(nullptr),
  m_return_addr(return_addr),
  m_visible_vars(),
  m_invisible_vars(),
  m_eval_stack(),
  m_exc_obj(0)
{
  // Do nothing here.
  m_eval_stack.reserve(100);
}

// -----------------------------------------------------------------------------

frame::~frame()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

size_t
frame::eval_stack_size() const
{
  return m_eval_stack.size();
}

// -----------------------------------------------------------------------------

instr_addr
frame::return_addr() const
{
  return m_return_addr;
}

// -----------------------------------------------------------------------------

void
frame::set_return_addr(
  const instr_addr return_addr)
{
  m_return_addr = return_addr;
}

// -----------------------------------------------------------------------------

void
frame::push_eval_stack(
  types::native_type_handle& operand)
{
  m_eval_stack.push_back(operand);
}

// -----------------------------------------------------------------------------

void
frame::push_eval_stack(
  types::native_type_handle&& operand)
{
  m_eval_stack.push_back(
    std::forward<types::native_type_handle>(operand));
}

// -----------------------------------------------------------------------------
types::native_type_handle
frame::pop_eval_stack()
  throw(evaluation_stack_empty_error)
{
  if (m_eval_stack.empty())
  {
    THROW(evaluation_stack_empty_error());
  }

  types::native_type_handle operand = m_eval_stack.back();
  m_eval_stack.pop_back();
  return operand;
}

// -----------------------------------------------------------------------------

types::native_type_handle&
frame::top_eval_stack()
  throw(evaluation_stack_empty_error)
{
  if (m_eval_stack.empty())
  {
    THROW(evaluation_stack_empty_error());
  }

  return m_eval_stack.back();
}

// -----------------------------------------------------------------------------

void
frame::swap_eval_stack()
{
  const size_t eval_stack_size = m_eval_stack.size();

  if (eval_stack_size < 2u)
  {
    THROW(invalid_operation_error(
      "Cannot swap eval stack"));
  }

  auto& a = m_eval_stack[eval_stack_size - 1];
  auto& b = m_eval_stack[eval_stack_size - 2];
  std::swap(a, b);
}

// -----------------------------------------------------------------------------

const std::vector<types::native_type_handle>&
frame::eval_stack() const
{
  return m_eval_stack;
}

// -----------------------------------------------------------------------------

types::native_type_handle&
frame::eval_stack_element(size_t i)
{
  return m_eval_stack[i];
}

// -----------------------------------------------------------------------------

size_t
frame::visible_var_count() const
{
  return m_visible_vars.size();
}

// -----------------------------------------------------------------------------

bool
frame::has_visible_var(
  const variable_key var_key) const
{
  return m_visible_vars.find(var_key) != m_visible_vars.end();
}

// -----------------------------------------------------------------------------

dyobj::dyobj_id
frame::get_visible_var(
  const variable_key var_key) const
  throw(name_not_found_error)
{
  auto itr = m_visible_vars.find(var_key);
  if (itr == m_visible_vars.end())
  {
    THROW(name_not_found_error());
  }

  return itr->second;
}

// -----------------------------------------------------------------------------

bool
frame::get_visible_var_fast(
  const variable_key var_key,
  dyobj::dyobj_id* id) const
{
  auto itr = m_visible_vars.find(var_key);
  if (itr == m_visible_vars.end())
  {
    return false;
  }

  *id = itr->second;

  return true;
}

// -----------------------------------------------------------------------------

dyobj::dyobj_id
frame::pop_visible_var(
  const variable_key var_key)
  throw(name_not_found_error)
{
  dyobj::dyobj_id obj_id = get_visible_var(var_key);
  m_visible_vars.erase(var_key);
  return obj_id;
}

// -----------------------------------------------------------------------------

void
frame::set_visible_var(
  variable_key var_key, dyobj::dyobj_id obj_id)
{
  m_visible_vars[var_key] = obj_id;
}

// -----------------------------------------------------------------------------

size_t
frame::invisible_var_count() const
{
  return m_invisible_vars.size();
}

// -----------------------------------------------------------------------------

bool
frame::has_invisible_var(
  const variable_key var_key) const
{
  return m_invisible_vars.find(var_key) != m_visible_vars.end();
}

// -----------------------------------------------------------------------------

dyobj::dyobj_id
frame::get_invisible_var(
  const variable_key var_key) const
  throw(name_not_found_error)
{
  auto itr = m_invisible_vars.find(var_key);
  if (itr == m_invisible_vars.end())
  {
    THROW(name_not_found_error());
  }

  return itr->second;
}

// -----------------------------------------------------------------------------

bool
frame::get_invisible_var_fast(
  const variable_key var_key,
  dyobj::dyobj_id* id) const
{
  auto itr = m_invisible_vars.find(var_key);
  if (itr == m_invisible_vars.end())
  {
    return false;
  }

  *id = itr->second;

  return true;
}

// -----------------------------------------------------------------------------

dyobj::dyobj_id
frame::pop_invisible_var(
  const variable_key var_key)
  throw(name_not_found_error)
{
  dyobj::dyobj_id obj_id = get_invisible_var(var_key);
  m_invisible_vars.erase(var_key);
  return obj_id;
}

// -----------------------------------------------------------------------------

void
frame::set_invisible_var(
  variable_key var_key, dyobj::dyobj_id obj_id)
{
  m_invisible_vars[var_key] = obj_id;
}

// -----------------------------------------------------------------------------

std::vector<variable_key>
frame::visible_var_keys() const
{
  std::vector<variable_key> keys;
  keys.reserve(m_visible_vars.size());

  for (const auto& pair : m_visible_vars)
  {
    keys.push_back(pair.first);
  }

  return keys;
}

// -----------------------------------------------------------------------------

std::vector<variable_key>
frame::invisible_var_keys() const
{
  std::vector<variable_key> keys;
  keys.reserve(m_invisible_vars.size());

  for (const auto& pair : m_invisible_vars)
  {
    keys.push_back(pair.first);
  }

  return keys;
}

// -----------------------------------------------------------------------------

std::list<dyobj::dyobj_id>
frame::get_visible_objs() const
{
  std::list<dyobj::dyobj_id> ids;

  for (auto itr = m_visible_vars.begin(); itr != m_visible_vars.end(); ++itr)
  {
    dyobj::dyobj_id id = itr->second;
    ids.push_back(id);
  }

  return ids;
}

// -----------------------------------------------------------------------------

std::list<dyobj::dyobj_id>
frame::get_invisible_objs() const
{
  std::list<dyobj::dyobj_id> ids;

  for (auto itr = m_invisible_vars.begin(); itr != m_invisible_vars.end(); ++itr)
  {
    dyobj::dyobj_id id = itr->second;
    ids.push_back(id);
  }

  return ids;
}

// -----------------------------------------------------------------------------

closure_ctx
frame::closure_ctx() const
{
  return m_closure_ctx;
}

// -----------------------------------------------------------------------------

compartment*
frame::compartment_ptr() const
{
  return m_compartment_ptr;
}

// -----------------------------------------------------------------------------

closure*
frame::closure_ptr() const
{
  return m_closure_ptr;
}

// -----------------------------------------------------------------------------

frame*
frame::parent() const
{
  return m_parent;
}

// -----------------------------------------------------------------------------

void
frame::set_parent(frame* parent)
{
  m_parent = parent;
}

// -----------------------------------------------------------------------------

dyobj::dyobj_id
frame::exc_obj() const
{
  return m_exc_obj;
}

// -----------------------------------------------------------------------------

void
frame::set_exc_obj(dyobj::dyobj_id exc_obj)
{
  m_exc_obj = exc_obj;
}

// -----------------------------------------------------------------------------

void
frame::clear_exc_obj()
{
  m_exc_obj = 0;
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */
