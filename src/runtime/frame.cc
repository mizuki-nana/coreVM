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

#include "common.h"
#include "errors.h"
#include "corevm/macros.h"

#include <cstdint>


// -----------------------------------------------------------------------------

corevm::runtime::frame::frame(
  const corevm::runtime::closure_ctx& closure_ctx,
  corevm::runtime::compartment* compartment_ptr,
  corevm::runtime::closure* closure_ptr)
  :
  m_closure_ctx(closure_ctx),
  m_compartment_ptr(compartment_ptr),
  m_closure_ptr(closure_ptr),
  m_return_addr(corevm::runtime::NONESET_INSTR_ADDR),
  m_visible_vars(),
  m_invisible_vars(),
  m_eval_stack(),
  m_exc_obj(0)
{
  // Do nothing here.
  m_eval_stack.reserve(100);
}

// -----------------------------------------------------------------------------

corevm::runtime::frame::frame(
  const corevm::runtime::closure_ctx& closure_ctx,
  corevm::runtime::compartment* compartment_ptr,
  corevm::runtime::closure* closure_ptr,
  corevm::runtime::instr_addr return_addr)
  :
  m_closure_ctx(closure_ctx),
  m_compartment_ptr(compartment_ptr),
  m_closure_ptr(closure_ptr),
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

corevm::runtime::frame::~frame()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

uint32_t
corevm::runtime::frame::eval_stack_size() const
{
  return m_eval_stack.size();
}

// -----------------------------------------------------------------------------

corevm::runtime::instr_addr
corevm::runtime::frame::return_addr() const
{
  return m_return_addr;
}

// -----------------------------------------------------------------------------

void
corevm::runtime::frame::set_return_addr(
  const corevm::runtime::instr_addr return_addr)
{
  m_return_addr = return_addr;
}

// -----------------------------------------------------------------------------

void
corevm::runtime::frame::push_eval_stack(
  corevm::types::native_type_handle& operand)
{
  m_eval_stack.push_back(operand);
}

// -----------------------------------------------------------------------------

void
corevm::runtime::frame::push_eval_stack(
  corevm::types::native_type_handle&& operand)
{
  m_eval_stack.push_back(
    std::forward<corevm::types::native_type_handle>(operand));
}

// -----------------------------------------------------------------------------
corevm::types::native_type_handle
corevm::runtime::frame::pop_eval_stack()
  throw(corevm::runtime::evaluation_stack_empty_error)
{
  if (m_eval_stack.empty())
  {
    THROW(corevm::runtime::evaluation_stack_empty_error());
  }

  corevm::types::native_type_handle operand = m_eval_stack.back();
  m_eval_stack.pop_back();
  return operand;
}

// -----------------------------------------------------------------------------

corevm::types::native_type_handle&
corevm::runtime::frame::top_eval_stack()
  throw(corevm::runtime::evaluation_stack_empty_error)
{
  if (m_eval_stack.empty())
  {
    THROW(corevm::runtime::evaluation_stack_empty_error());
  }

  return m_eval_stack.back();
}

// -----------------------------------------------------------------------------

const std::vector<corevm::types::native_type_handle>&
corevm::runtime::frame::eval_stack() const
{
  return m_eval_stack;
}

// -----------------------------------------------------------------------------

corevm::types::native_type_handle&
corevm::runtime::frame::eval_stack_element(size_t i)
{
  return m_eval_stack[i];
}

// -----------------------------------------------------------------------------

bool
corevm::runtime::frame::has_visible_var(
  const corevm::runtime::variable_key var_key) const
{
  return m_visible_vars.find(var_key) != m_visible_vars.end();
}

// -----------------------------------------------------------------------------

corevm::dyobj::dyobj_id
corevm::runtime::frame::get_visible_var(
  const corevm::runtime::variable_key var_key) const
  throw(corevm::runtime::name_not_found_error)
{
  if (!has_visible_var(var_key))
  {
    THROW(corevm::runtime::name_not_found_error());
  }

  return m_visible_vars.at(var_key);
};

// -----------------------------------------------------------------------------

corevm::dyobj::dyobj_id
corevm::runtime::frame::pop_visible_var(
  const corevm::runtime::variable_key var_key)
  throw(corevm::runtime::name_not_found_error)
{
  corevm::dyobj::dyobj_id obj_id = get_visible_var(var_key);
  m_visible_vars.erase(var_key);
  return obj_id;
}

// -----------------------------------------------------------------------------

void
corevm::runtime::frame::set_visible_var(
  corevm::runtime::variable_key var_key, corevm::dyobj::dyobj_id obj_id)
{
  m_visible_vars[var_key] = obj_id;
}

// -----------------------------------------------------------------------------

bool
corevm::runtime::frame::has_invisible_var(
  const corevm::runtime::variable_key var_key) const
{
  return m_invisible_vars.find(var_key) != m_visible_vars.end();
}

// -----------------------------------------------------------------------------

corevm::dyobj::dyobj_id
corevm::runtime::frame::get_invisible_var(
  const corevm::runtime::variable_key var_key) const
  throw(corevm::runtime::name_not_found_error)
{
  if (!has_invisible_var(var_key))
  {
    THROW(corevm::runtime::name_not_found_error());
  }

  return m_invisible_vars.at(var_key);
};

// -----------------------------------------------------------------------------

corevm::dyobj::dyobj_id
corevm::runtime::frame::pop_invisible_var(
  const corevm::runtime::variable_key var_key)
  throw(corevm::runtime::name_not_found_error)
{
  corevm::dyobj::dyobj_id obj_id = get_invisible_var(var_key);
  m_invisible_vars.erase(var_key);
  return obj_id;
}

// -----------------------------------------------------------------------------

void
corevm::runtime::frame::set_invisible_var(
  corevm::runtime::variable_key var_key, corevm::dyobj::dyobj_id obj_id)
{
  m_invisible_vars[var_key] = obj_id;
}

// -----------------------------------------------------------------------------

std::list<corevm::dyobj::dyobj_id>
corevm::runtime::frame::get_visible_objs() const
{
  std::list<corevm::dyobj::dyobj_id> ids;

  for (auto itr = m_visible_vars.begin(); itr != m_visible_vars.end(); ++itr)
  {
    corevm::dyobj::dyobj_id id = itr->second;
    ids.push_back(id);
  }

  return ids;
}

// -----------------------------------------------------------------------------

std::list<corevm::dyobj::dyobj_id>
corevm::runtime::frame::get_invisible_objs() const
{
  std::list<corevm::dyobj::dyobj_id> ids;

  for (auto itr = m_invisible_vars.begin(); itr != m_invisible_vars.end(); ++itr)
  {
    corevm::dyobj::dyobj_id id = itr->second;
    ids.push_back(id);
  }

  return ids;
}

// -----------------------------------------------------------------------------

corevm::runtime::closure_ctx
corevm::runtime::frame::closure_ctx() const
{
  return m_closure_ctx;
}

// -----------------------------------------------------------------------------

corevm::runtime::compartment*
corevm::runtime::frame::compartment_ptr() const
{
  return m_compartment_ptr;
}

// -----------------------------------------------------------------------------

corevm::runtime::closure*
corevm::runtime::frame::closure_ptr() const
{
  return m_closure_ptr;
}

// -----------------------------------------------------------------------------

corevm::dyobj::dyobj_id
corevm::runtime::frame::exc_obj() const
{
  return m_exc_obj;
}

// -----------------------------------------------------------------------------

void
corevm::runtime::frame::set_exc_obj(corevm::dyobj::dyobj_id exc_obj)
{
  m_exc_obj = exc_obj;
}

// -----------------------------------------------------------------------------

void
corevm::runtime::frame::clear_exc_obj()
{
  m_exc_obj = 0;
}

// -----------------------------------------------------------------------------
