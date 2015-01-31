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
#include "../../include/runtime/compartment.h"


corevm::runtime::compartment::compartment()
{
}


const corevm::runtime::compartment_id
corevm::runtime::compartment::id() const
{
  return m_id;
}

void
corevm::runtime::compartment::set_encoding_map(
  const corevm::runtime::encoding_map& encoding_map)
{
  m_encoding_map.clear();
  m_encoding_map.insert(encoding_map.begin(), encoding_map.end());
}

std::string
corevm::runtime::compartment::get_encoding_string(uint64_t key) const
  throw(corevm::runtime::encoding_string_not_found_error)
{
  auto itr = m_encoding_map.find(key);

  if (itr == m_encoding_map.end()) {
    throw corevm::runtime::encoding_string_not_found_error(key);
  }

  return static_cast<std::string>(itr->second);
}

size_t
corevm::runtime::compartment::closure_count() const
{
  return m_closure_table.size();
}

void
corevm::runtime::compartment::set_closure_table(
  const corevm::runtime::closure_table& closure_table)
{
  m_closure_table.clear();
  m_closure_table.insert(closure_table.begin(), closure_table.end());
}

const corevm::runtime::closure&
corevm::runtime::compartment::get_closure_by_id(corevm::runtime::closure_id id)
  const throw(corevm::runtime::closure_not_found_error)
{
  auto itr = m_closure_table.find(id);

  if (itr == m_closure_table.end()) {
    throw corevm::runtime::closure_not_found_error(id);
  }

  return static_cast<corevm::runtime::closure>(itr->second);
}
