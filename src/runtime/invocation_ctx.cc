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
#include "invocation_ctx.h"

#include "closure.h"
#include "compartment.h"
#include "corevm/macros.h"


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

const size_t DEFAULT_PARAMS_LIST_CAPACITY = 10;

// -----------------------------------------------------------------------------

invocation_ctx::invocation_ctx(
  const runtime::closure_ctx& ctx,
  compartment* compartment_ptr,
  closure* closure_ptr)
  :
  m_closure_ctx(ctx),
  m_compartment_ptr(compartment_ptr),
  m_closure_ptr(closure_ptr),
  m_params_list(),
  m_param_value_map(),
  m_params_list_pop_index(0)
{
  m_params_list.reserve(DEFAULT_PARAMS_LIST_CAPACITY);
}

// -----------------------------------------------------------------------------

const closure_ctx&
invocation_ctx::closure_ctx() const
{
  return m_closure_ctx;
}

// -----------------------------------------------------------------------------

compartment*
invocation_ctx::compartment_ptr() const
{
  return m_compartment_ptr;
}

// -----------------------------------------------------------------------------

closure*
invocation_ctx::closure_ptr() const
{
  return m_closure_ptr;
}

// -----------------------------------------------------------------------------

const param_list_type&
invocation_ctx::params_list() const
{
  return m_params_list;
}

// -----------------------------------------------------------------------------

const param_value_map_type&
invocation_ctx::param_value_map() const
{
  return m_param_value_map;
}

// -----------------------------------------------------------------------------

bool
invocation_ctx::has_params() const
{
  return m_params_list_pop_index < m_params_list.size();
}

// -----------------------------------------------------------------------------

void
invocation_ctx::put_param(const dyobj::dyobj_id& id)
{
  m_params_list.push_back(id);
}

// -----------------------------------------------------------------------------

dyobj::dyobj_id
invocation_ctx::pop_param()
  throw(missing_parameter_error)
{
  if (m_params_list_pop_index >= m_params_list.size())
  {
    THROW(missing_parameter_error());
  }

  return m_params_list[m_params_list_pop_index++];
}

// -----------------------------------------------------------------------------

bool
invocation_ctx::has_param_value_pairs() const
{
  return !m_param_value_map.empty();
}

// -----------------------------------------------------------------------------

bool
invocation_ctx::has_param_value_pair_with_key(
  const variable_key& key) const
{
  return m_param_value_map.find(key) != m_param_value_map.end();
}

// -----------------------------------------------------------------------------

void
invocation_ctx::put_param_value_pair(
  const variable_key& key, const dyobj::dyobj_id& id)
{
  m_param_value_map[key] = id;
}

// -----------------------------------------------------------------------------

dyobj::dyobj_id
invocation_ctx::pop_param_value_pair(
  const variable_key& key)
  throw(missing_parameter_error)
{
  auto itr = m_param_value_map.find(key);

  if (itr == m_param_value_map.end())
  {
    THROW(missing_parameter_error());
  }

  dyobj::dyobj_id id = itr->second;

  m_param_value_map.erase(itr);

  return id;
}

// -----------------------------------------------------------------------------

std::vector<variable_key>
invocation_ctx::param_value_pair_keys() const
{
  std::vector<variable_key> keys;
  keys.reserve(m_param_value_map.size());

  for (auto itr = m_param_value_map.begin();
       itr != m_param_value_map.end(); ++itr)
  {
    variable_key key = itr->first;
    keys.push_back(key);
  }

  return keys;
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */
