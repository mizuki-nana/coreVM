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


// -----------------------------------------------------------------------------

corevm::runtime::compartment::compartment(const std::string& path)
  :
  m_path(path)
{
}

// -----------------------------------------------------------------------------

const std::string&
corevm::runtime::compartment::path() const
{
  return m_path;
}

// -----------------------------------------------------------------------------

void
corevm::runtime::compartment::set_encoding_map(
  const corevm::runtime::encoding_map& encoding_map)
{
  m_encoding_map = encoding_map;
}

// -----------------------------------------------------------------------------

void
corevm::runtime::compartment::set_encoding_map(
  const corevm::runtime::encoding_map&& encoding_map)
{
  m_encoding_map = encoding_map;
}

// -----------------------------------------------------------------------------

std::string
corevm::runtime::compartment::get_encoding_string(
  corevm::runtime::encoding_key key) const
  throw(corevm::runtime::encoding_string_not_found_error)
{
  if (key >= m_encoding_map.size())
  {
    THROW(corevm::runtime::encoding_string_not_found_error(key));
  }

  return m_encoding_map[key];
}

// -----------------------------------------------------------------------------

void
corevm::runtime::compartment::get_encoding_string(
  corevm::runtime::encoding_key key, std::string* str) const
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
corevm::runtime::compartment::closure_count() const
{
  return m_closure_table.size();
}

// -----------------------------------------------------------------------------

void
corevm::runtime::compartment::set_closure_table(
  const corevm::runtime::closure_table&& closure_table)
{
  m_closure_table = closure_table;
}

// -----------------------------------------------------------------------------

const corevm::runtime::closure
corevm::runtime::compartment::get_closure_by_id(corevm::runtime::closure_id id)
  const throw(corevm::runtime::closure_not_found_error)
{
  if (id < static_cast<corevm::runtime::closure_id>(m_closure_table.size()))
  {
    return m_closure_table[static_cast<size_t>(id)];
  }

  THROW(corevm::runtime::closure_not_found_error(id));
}

// -----------------------------------------------------------------------------

void
corevm::runtime::compartment::get_closure_by_id(
  corevm::runtime::closure_id id, corevm::runtime::closure** closure_ptr)
{
  if (id < static_cast<corevm::runtime::closure_id>(m_closure_table.size()))
  {
    *closure_ptr = &m_closure_table[static_cast<size_t>(id)];
  }
}

// -----------------------------------------------------------------------------

bool
corevm::runtime::compartment::get_starting_closure(
  corevm::runtime::closure** closure)
{
  for (size_t i = 0; i < m_closure_table.size(); ++i)
  {
    const corevm::runtime::closure& closure_ = m_closure_table[i];

    if (closure_.id != corevm::runtime::NONESET_CLOSURE_ID &&
        closure_.parent_id == corevm::runtime::NONESET_CLOSURE_ID)
    {
      *closure = const_cast<corevm::runtime::closure*>(&closure_);

      return true;
    }
  }

  return false;
}

// -----------------------------------------------------------------------------


namespace corevm {


namespace runtime {


std::ostream& operator<<(
  std::ostream& ost, const corevm::runtime::compartment& compartment)
{
  const std::string LINE = "----------------------------";
  ost << LINE << std::endl;
  ost << "Compartment" << std::endl;
  ost << std::endl;
  ost << "Path: " << compartment.path() << std::endl;
  ost << std::endl;

  for (auto itr = compartment.m_closure_table.cbegin();
      itr != compartment.m_closure_table.cend();
      ++itr)
  {
    const corevm::runtime::closure& closure = *itr;
    ost << closure << std::endl;
  }

  return ost;
}


} /* end namespace runtime */


} /* end namespace corevm */


// -----------------------------------------------------------------------------
