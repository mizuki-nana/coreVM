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
#include "invocation_ctx.h"

#include "closure.h"
#include "compartment.h"
#include "corevm/macros.h"


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

const size_t DEFAULT_PARAMS_LIST_CAPACITY = 8;

// -----------------------------------------------------------------------------

InvocationCtx::InvocationCtx(const runtime::ClosureCtx& ctx,
  Compartment* compartment,
  Closure* closure)
  :
  m_closure_ctx(ctx),
  m_compartment(compartment),
  m_closure(closure),
  m_params_list(),
  m_param_value_map(),
  m_params_list_pop_index(0)
{
  m_params_list.reserve(DEFAULT_PARAMS_LIST_CAPACITY);
}

// -----------------------------------------------------------------------------

const ClosureCtx&
InvocationCtx::closure_ctx() const
{
  return m_closure_ctx;
}

// -----------------------------------------------------------------------------

Compartment*
InvocationCtx::compartment() const
{
  return m_compartment;
}

// -----------------------------------------------------------------------------

Closure*
InvocationCtx::closure() const
{
  return m_closure;
}

// -----------------------------------------------------------------------------

const InvocationCtx::param_list_type&
InvocationCtx::params_list() const
{
  return m_params_list;
}

// -----------------------------------------------------------------------------

const InvocationCtx::param_value_map_type&
InvocationCtx::param_value_map() const
{
  return m_param_value_map;
}

// -----------------------------------------------------------------------------

bool
InvocationCtx::has_params() const
{
  return m_params_list_pop_index < m_params_list.size();
}

// -----------------------------------------------------------------------------

void
InvocationCtx::put_param(param_type param)
{
  m_params_list.push_back(param);
}

// -----------------------------------------------------------------------------

InvocationCtx::param_type
InvocationCtx::pop_param()
{
  if (m_params_list_pop_index >= m_params_list.size())
  {
    THROW(MissingParameterError());
  }

  return m_params_list[m_params_list_pop_index++];
}

// -----------------------------------------------------------------------------

bool
InvocationCtx::has_param_value_pairs() const
{
  return !m_param_value_map.empty();
}

// -----------------------------------------------------------------------------

bool
InvocationCtx::has_param_value_pair_with_key(variable_key_t key) const
{
  return m_param_value_map.find(key) != m_param_value_map.end();
}

// -----------------------------------------------------------------------------

void
InvocationCtx::put_param_value_pair(variable_key_t key, param_type param)
{
  m_param_value_map[key] = param;
}

// -----------------------------------------------------------------------------

InvocationCtx::param_type
InvocationCtx::pop_param_value_pair(variable_key_t key)
{
  auto itr = m_param_value_map.find(key);

  if (itr == m_param_value_map.end())
  {
    THROW(MissingParameterError());
  }

  auto param = itr->second;

  m_param_value_map.erase(itr);

  return param;
}

// -----------------------------------------------------------------------------

std::vector<variable_key_t>
InvocationCtx::param_value_pair_keys() const
{
  std::vector<variable_key_t> keys;
  keys.reserve(m_param_value_map.size());

  for (auto itr = m_param_value_map.begin(); itr != m_param_value_map.end(); ++itr)
  {
    variable_key_t key = itr->first;
    keys.push_back(key);
  }

  return keys;
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */
