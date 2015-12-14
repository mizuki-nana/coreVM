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
#include "compartment.h"

#include "corevm/macros.h"

#include <algorithm>
#include <ostream>


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

compartment::compartment(const std::string& path)
  :
  m_path(path)
{
}

// -----------------------------------------------------------------------------

const std::string&
compartment::path() const
{
  return m_path;
}

// -----------------------------------------------------------------------------

void
compartment::set_encoding_map(
  const encoding_map& encoding_map)
{
  m_encoding_map = encoding_map;
}

// -----------------------------------------------------------------------------

void
compartment::set_encoding_map(
  const encoding_map&& encoding_map)
{
  m_encoding_map = encoding_map;
}

// -----------------------------------------------------------------------------

std::string
compartment::get_encoding_string(
  encoding_key key) const
  throw(encoding_string_not_found_error)
{
  if (key >= m_encoding_map.size())
  {
    THROW(encoding_string_not_found_error(key));
  }

  return m_encoding_map[key];
}

// -----------------------------------------------------------------------------

void
compartment::get_encoding_string(
  encoding_key key, std::string* str) const
{
  if (!str)
  {
    return;
  }

  if (key < m_encoding_map.size())
  {
    *str = m_encoding_map[key];
  }
}

// -----------------------------------------------------------------------------

size_t
compartment::closure_count() const
{
  return m_closure_table.size();
}

// -----------------------------------------------------------------------------

void
compartment::set_closure_table(
  const closure_table&& closure_table)
{
  m_closure_table = closure_table;
}

// -----------------------------------------------------------------------------

const closure
compartment::get_closure_by_id(closure_id id)
  const throw(closure_not_found_error)
{
  if (id < static_cast<closure_id>(m_closure_table.size()))
  {
    return m_closure_table[static_cast<size_t>(id)];
  }

  THROW(closure_not_found_error(id));
}

// -----------------------------------------------------------------------------

void
compartment::get_closure_by_id(
  closure_id id, closure** closure_ptr)
{
  if (id < static_cast<closure_id>(m_closure_table.size()))
  {
    *closure_ptr = &m_closure_table[static_cast<size_t>(id)];
  }
}

// -----------------------------------------------------------------------------

bool
compartment::get_starting_closure(
  closure** closure)
{
  for (size_t i = 0; i < m_closure_table.size(); ++i)
  {
    const runtime::closure& closure_ = m_closure_table[i];

    if (closure_.id != NONESET_CLOSURE_ID &&
        closure_.parent_id == NONESET_CLOSURE_ID)
    {
      *closure = const_cast<runtime::closure*>(&closure_);

      return true;
    }
  }

  return false;
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */
