/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2014 Yanzheng Li

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
#include "../../include/runtime/frame.h"


corevm::runtime::frame::frame():
  m_start_addr(corevm::runtime::NONESET_INSTR_ADDR),
  m_return_addr(corevm::runtime::NONESET_INSTR_ADDR),
  m_parent_scope_frame_ptr(nullptr),
  m_visible_vars(std::unordered_map<corevm::runtime::variable_key, corevm::dyobj::dyobj_id>()),
  m_invisible_vars(std::unordered_map<corevm::runtime::variable_key, corevm::dyobj::dyobj_id>()),
  m_params_list(std::list<corevm::dyobj::dyobj_id>()),
  m_param_value_map(std::unordered_map<corevm::runtime::variable_key, corevm::dyobj::dyobj_id>()),
  m_eval_stack(std::stack<corevm::types::native_type_handle>())
{
  // Do nothing here.
}

corevm::runtime::frame::~frame()
{
  // Do nothing here.
}

void
corevm::runtime::frame::check_state_before_destruction()
  throw(corevm::runtime::evaluation_stack_not_empty_error)
{
  // Validation checks on the state of the frame before it gets destroyed.
  if(!m_eval_stack.empty()) {
    throw corevm::runtime::evaluation_stack_not_empty_error();
  }
}

uint32_t
corevm::runtime::frame::eval_stack_size() const
{
  return m_eval_stack.size();
}

corevm::runtime::instr_addr
corevm::runtime::frame::get_start_addr() const
{
  return m_start_addr;
}

void
corevm::runtime::frame::set_start_addr(const corevm::runtime::instr_addr start_addr)
{
  m_start_addr = start_addr;
}

corevm::runtime::instr_addr
corevm::runtime::frame::get_return_addr() const
{
  return m_return_addr;
}

void
corevm::runtime::frame::set_return_addr(const corevm::runtime::instr_addr return_addr)
{
  m_return_addr = return_addr;
}

const corevm::runtime::frame*
corevm::runtime::frame::get_parent_scope_frame_ptr() const
{
  return m_parent_scope_frame_ptr;
}

void
corevm::runtime::frame::set_parent_scope_frame_ptr(corevm::runtime::frame* ptr)
{
  m_parent_scope_frame_ptr = ptr;
}

void
corevm::runtime::frame::push_eval_stack(corevm::types::native_type_handle& operand)
{
  m_eval_stack.push(operand);
}

corevm::types::native_type_handle
corevm::runtime::frame::pop_eval_stack() throw(corevm::runtime::evaluation_stack_empty_error)
{
  if(m_eval_stack.empty()) {
    throw corevm::runtime::evaluation_stack_empty_error();
  }

  corevm::types::native_type_handle operand = m_eval_stack.top();
  m_eval_stack.pop();
  return operand;
}

bool
corevm::runtime::frame::has_visible_var(const corevm::runtime::variable_key var_key) const
{
  return m_visible_vars.find(var_key) != m_visible_vars.end();
}

corevm::dyobj::dyobj_id
corevm::runtime::frame::get_visible_var(const corevm::runtime::variable_key var_key) const
  throw(corevm::runtime::local_variable_not_found_error)
{
  if(!has_visible_var(var_key)) {
    throw corevm::runtime::local_variable_not_found_error();
  }

  return m_visible_vars.at(var_key);
};

corevm::dyobj::dyobj_id
corevm::runtime::frame::pop_visible_var(
  const corevm::runtime::variable_key var_key) throw(corevm::runtime::local_variable_not_found_error)
{
  corevm::dyobj::dyobj_id obj_id = get_visible_var(var_key);
  m_visible_vars.erase(var_key);
  return obj_id;
}

void
corevm::runtime::frame::set_visible_var(
  corevm::runtime::variable_key var_key, corevm::dyobj::dyobj_id obj_id)
{
  m_visible_vars[var_key] = obj_id;
}

bool
corevm::runtime::frame::has_invisible_var(const corevm::runtime::variable_key var_key) const
{
  return m_invisible_vars.find(var_key) != m_visible_vars.end();
}

corevm::dyobj::dyobj_id
corevm::runtime::frame::get_invisible_var(const corevm::runtime::variable_key var_key) const
  throw(corevm::runtime::local_variable_not_found_error)
{
  if(!has_invisible_var(var_key)) {
    throw corevm::runtime::local_variable_not_found_error();
  }

  return m_invisible_vars.at(var_key);
};

corevm::dyobj::dyobj_id
corevm::runtime::frame::pop_invisible_var(
  const corevm::runtime::variable_key var_key) throw(corevm::runtime::local_variable_not_found_error)
{
  corevm::dyobj::dyobj_id obj_id = get_invisible_var(var_key);
  m_invisible_vars.erase(var_key);
  return obj_id;
}

void
corevm::runtime::frame::set_invisible_var(
  corevm::runtime::variable_key var_key, corevm::dyobj::dyobj_id obj_id)
{
  m_invisible_vars[var_key] = obj_id;
}

bool
corevm::runtime::frame::has_params() const
{
  return !m_params_list.empty();
}

void
corevm::runtime::frame::put_param(const corevm::dyobj::dyobj_id& id)
{
  m_params_list.push_back(id);
}

const corevm::dyobj::dyobj_id
corevm::runtime::frame::pop_param() throw(corevm::runtime::missing_parameter_error)
{
  if(m_params_list.empty()) {
    throw corevm::runtime::missing_parameter_error();
  }

  corevm::dyobj::dyobj_id id = m_params_list.back();
  m_params_list.pop_back();
  return id;
}

bool
corevm::runtime::frame::has_param_value_pairs() const
{
  return !m_param_value_map.empty();
}

void
corevm::runtime::frame::put_param_value_pair(
  const corevm::runtime::variable_key key, const corevm::dyobj::dyobj_id& id)
{
  m_param_value_map[key] = id;
}

const corevm::dyobj::dyobj_id
corevm::runtime::frame::pop_param_value_pair(const corevm::runtime::variable_key key) throw(
  corevm::runtime::missing_parameter_error)
{
  auto itr = m_param_value_map.find(key);

  if(itr == m_param_value_map.end()) {
    throw corevm::runtime::missing_parameter_error();
  }

  corevm::dyobj::dyobj_id id = itr->second;

  m_param_value_map.erase(itr);

  return id;
}

std::list<corevm::runtime::variable_key>
corevm::runtime::frame::param_value_pair_keys() const
{
  std::list<corevm::runtime::variable_key> keys;

  for(auto itr = m_param_value_map.begin(); itr != m_param_value_map.end(); ++itr) {
    corevm::runtime::variable_key key = itr->first;
    keys.push_back(key);
  }

  return keys;
}

std::list<corevm::dyobj::dyobj_id>
corevm::runtime::frame::get_visible_objs() const
{
  std::list<corevm::dyobj::dyobj_id> ids;

  for(auto itr = m_visible_vars.begin(); itr != m_visible_vars.end(); ++itr) {
    corevm::dyobj::dyobj_id id = itr->second;
    ids.push_back(id);
  }

  return ids;
}

std::list<corevm::dyobj::dyobj_id>
corevm::runtime::frame::get_invisible_objs() const
{
  std::list<corevm::dyobj::dyobj_id> ids;

  for(auto itr = m_invisible_vars.begin(); itr != m_invisible_vars.end(); ++itr) {
    corevm::dyobj::dyobj_id id = itr->second;
    ids.push_back(id);
  }

  return ids;
}
