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

#include "common.h"
#include "errors.h"
#include "corevm/macros.h"
#include "dyobj/dyobj_id.h"


// -----------------------------------------------------------------------------

corevm::runtime::invocation_ctx::invocation_ctx(
  const corevm::runtime::closure_ctx& ctx,
  corevm::runtime::compartment* compartment_ptr,
  corevm::runtime::closure* closure_ptr)
  :
  m_closure_ctx(ctx),
  m_compartment_ptr(compartment_ptr),
  m_closure_ptr(closure_ptr),
  m_params_list(),
  m_param_value_map()
{
}

// -----------------------------------------------------------------------------

const corevm::runtime::closure_ctx&
corevm::runtime::invocation_ctx::closure_ctx() const
{
  return m_closure_ctx;
}

// -----------------------------------------------------------------------------

corevm::runtime::compartment*
corevm::runtime::invocation_ctx::compartment_ptr() const
{
  return m_compartment_ptr;
}

// -----------------------------------------------------------------------------

corevm::runtime::closure*
corevm::runtime::invocation_ctx::closure_ptr() const
{
  return m_closure_ptr;
}

// -----------------------------------------------------------------------------

const corevm::runtime::param_list_type&
corevm::runtime::invocation_ctx::params_list() const
{
  return m_params_list;
}

// -----------------------------------------------------------------------------

const corevm::runtime::param_value_map_type&
corevm::runtime::invocation_ctx::param_value_map() const
{
  return m_param_value_map;
}

// -----------------------------------------------------------------------------

bool
corevm::runtime::invocation_ctx::has_params() const
{
  return !m_params_list.empty();
}

// -----------------------------------------------------------------------------

void
corevm::runtime::invocation_ctx::put_param(const corevm::dyobj::dyobj_id& id)
{
  m_params_list.push_back(id);
}

// -----------------------------------------------------------------------------

const corevm::dyobj::dyobj_id
corevm::runtime::invocation_ctx::pop_param()
  throw(corevm::runtime::missing_parameter_error)
{
  if (m_params_list.empty())
  {
    THROW(corevm::runtime::missing_parameter_error());
  }

  corevm::dyobj::dyobj_id id = m_params_list.front();
  m_params_list.pop_front();
  return id;
}

// -----------------------------------------------------------------------------

bool
corevm::runtime::invocation_ctx::has_param_value_pairs() const
{
  return !m_param_value_map.empty();
}

// -----------------------------------------------------------------------------

bool
corevm::runtime::invocation_ctx::has_param_value_pair_with_key(
  const corevm::runtime::variable_key& key) const
{
  return m_param_value_map.find(key) != m_param_value_map.end();
}

// -----------------------------------------------------------------------------

void
corevm::runtime::invocation_ctx::put_param_value_pair(
  const corevm::runtime::variable_key& key, const corevm::dyobj::dyobj_id& id)
{
  m_param_value_map[key] = id;
}

// -----------------------------------------------------------------------------

const corevm::dyobj::dyobj_id
corevm::runtime::invocation_ctx::pop_param_value_pair(
  const corevm::runtime::variable_key& key)
  throw(corevm::runtime::missing_parameter_error)
{
  auto itr = m_param_value_map.find(key);

  if (itr == m_param_value_map.end())
  {
    THROW(corevm::runtime::missing_parameter_error());
  }

  corevm::dyobj::dyobj_id id = itr->second;

  m_param_value_map.erase(itr);

  return id;
}

// -----------------------------------------------------------------------------

std::list<corevm::runtime::variable_key>
corevm::runtime::invocation_ctx::param_value_pair_keys() const
{
  std::list<corevm::runtime::variable_key> keys;

  for (auto itr = m_param_value_map.begin();
       itr != m_param_value_map.end(); ++itr)
  {
    corevm::runtime::variable_key key = itr->first;
    keys.push_back(key);
  }

  return keys;
}

// -----------------------------------------------------------------------------
