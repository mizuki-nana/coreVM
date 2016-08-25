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
#include "compartment.h"

#include "corevm/macros.h"

#include <algorithm>
#include <ostream>


namespace corevm {
namespace runtime {

// -----------------------------------------------------------------------------

Compartment::Compartment(const std::string& path)
  :
  m_path(path)
{
}

// -----------------------------------------------------------------------------

const std::string&
Compartment::path() const
{
  return m_path;
}

// -----------------------------------------------------------------------------

void
Compartment::set_string_literal_table(const StringLiteralTable& table)
{
  m_str_literal_table = table;
}

// -----------------------------------------------------------------------------

void
Compartment::set_string_literal_table(StringLiteralTable&& table)
{
  m_str_literal_table = std::move(table);
}

// -----------------------------------------------------------------------------

void
Compartment::set_fpt_literal_table(const FptLiteralTable& table)
{
  m_fpt_literal_table = table;
}

// -----------------------------------------------------------------------------

void
Compartment::set_fpt_literal_table(FptLiteralTable&& table)
{
  m_fpt_literal_table = std::move(table);
}

// -----------------------------------------------------------------------------

std::string
Compartment::get_string_literal(encoding_key_t key) const
{
  if (key >= m_str_literal_table.size())
  {
    THROW(StringLiteralNotFoundError(key));
  }

  return m_str_literal_table[key];
}

// -----------------------------------------------------------------------------

void
Compartment::get_string_literal(encoding_key_t key, std::string* str) const
{
  const char* str_ = NULL;
  get_string_literal(key, &str_);

  if (str_)
  {
    str->assign(str_);
  }
}

// -----------------------------------------------------------------------------

void
Compartment::get_string_literal(encoding_key_t key, const char** str) const
{
  if (key < m_str_literal_table.size())
  {
    *str = m_str_literal_table[key].c_str();
  }
}

// -----------------------------------------------------------------------------

double
Compartment::get_fpt_literal(encoding_key_t key) const
{
  if (key >= m_fpt_literal_table.size())
  {
    THROW(FptLiteralNotFoundError(key));
  }

  return m_fpt_literal_table[key];
}

// -----------------------------------------------------------------------------

size_t
Compartment::closure_count() const
{
  return m_closure_table.size();
}

// -----------------------------------------------------------------------------

void
Compartment::set_closure_table(const ClosureTable&& closure_table)
{
  m_closure_table = closure_table;
}

// -----------------------------------------------------------------------------

const Closure
Compartment::get_closure_by_id(closure_id_t id) const
{
  if (id < static_cast<closure_id_t>(m_closure_table.size()))
  {
    return m_closure_table[static_cast<size_t>(id)];
  }

  THROW(ClosureNotFoundError(id));
}

// -----------------------------------------------------------------------------

void
Compartment::get_closure_by_id(closure_id_t id, Closure** closure_ptr)
{
  if (id < static_cast<closure_id_t>(m_closure_table.size()))
  {
    *closure_ptr = &m_closure_table[static_cast<size_t>(id)];
  }
}

// -----------------------------------------------------------------------------

bool
Compartment::get_starting_closure(Closure** closure)
{
  for (size_t i = 0; i < m_closure_table.size(); ++i)
  {
    const runtime::Closure& closure_ = m_closure_table[i];

    if (closure_.id != NONESET_CLOSURE_ID &&
        closure_.parent_id == NONESET_CLOSURE_ID)
    {
      *closure = const_cast<runtime::Closure*>(&closure_);

      return true;
    }
  }

  return false;
}

// -----------------------------------------------------------------------------

} /* end namespace runtime */
} /* end namespace corevm */
