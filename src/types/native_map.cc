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
#include "native_map.h"

#include "errors.h"
#include "corevm/macros.h"

#include <cstdint>
#include <stdexcept>
#include <utility>


// -----------------------------------------------------------------------------

corevm::types::native_map::native_map()
  :
  native_map_base()
{
}

// -----------------------------------------------------------------------------

corevm::types::native_map::native_map(const native_map_base& other)
  :
  native_map_base(other)
{
}

// -----------------------------------------------------------------------------

corevm::types::native_map::native_map(native_map_base&& other)
  :
  native_map_base(std::forward<native_map_base>(other))
{
}

// -----------------------------------------------------------------------------

corevm::types::native_map::native_map(std::initializer_list<value_type> il)
  :
  native_map_base(il)
{
}

// -----------------------------------------------------------------------------

corevm::types::native_map::native_map(int8_t)
  :
  native_map_base()
{
  THROW(corevm::types::conversion_error("int8", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map::operator int8_t() const
{
  THROW(corevm::types::conversion_error("map", "int8"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator+() const
{
  THROW(corevm::types::invalid_operator_error("+", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator-() const
{
  THROW(corevm::types::invalid_operator_error("-", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator++() const
{
  THROW(corevm::types::invalid_operator_error("++", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator--() const
{
  THROW(corevm::types::invalid_operator_error("--", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator!() const
{
  THROW(corevm::types::invalid_operator_error("!", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator~() const
{
  THROW(corevm::types::invalid_operator_error("~", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator+(const native_map&) const
{
  THROW(corevm::types::invalid_operator_error("+", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator-(const native_map&) const
{
  THROW(corevm::types::invalid_operator_error("-", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator*(const native_map&) const
{
  THROW(corevm::types::invalid_operator_error("*", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator/(const native_map&) const
{
  THROW(corevm::types::invalid_operator_error("/", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator%(const native_map&) const
{
  THROW(corevm::types::invalid_operator_error("%", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator&&(const native_map&) const
{
  THROW(corevm::types::invalid_operator_error("&&", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator||(const native_map&) const
{
  THROW(corevm::types::invalid_operator_error("||", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator&(const native_map&) const
{
  THROW(corevm::types::invalid_operator_error("&", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator|(const native_map&) const
{
  THROW(corevm::types::invalid_operator_error("|", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator^(const native_map&) const
{
  THROW(corevm::types::invalid_operator_error("^", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator<<(const native_map&) const
{
  THROW(corevm::types::invalid_operator_error("<<", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator>>(const native_map&) const
{
  THROW(corevm::types::invalid_operator_error(">>", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator<(const native_map&) const
{
  THROW(corevm::types::invalid_operator_error("<", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator<=(const native_map&) const
{
  THROW(corevm::types::invalid_operator_error("<=", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator>(const native_map&) const
{
  THROW(corevm::types::invalid_operator_error(">", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator>=(const native_map&) const
{
  THROW(corevm::types::invalid_operator_error(">=", "map"));
}

// -----------------------------------------------------------------------------

corevm::types::native_map::mapped_type&
corevm::types::native_map::at(const key_type& k)
  throw(corevm::types::out_of_range_error)
{
  return const_cast<mapped_type&>(
    static_cast<const native_map&>(*this).at(k));
}

// -----------------------------------------------------------------------------

const corevm::types::native_map::mapped_type&
corevm::types::native_map::at(const key_type& k) const
  throw(corevm::types::out_of_range_error)
{
  auto itr = find(k);

  if (itr == end())
  {
    THROW(corevm::types::out_of_range_error("Map key out of range"));
  }

  return itr->second;
}

// -----------------------------------------------------------------------------
