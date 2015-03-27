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

#include <cstdint>
#include <stdexcept>


const size_t DEFAULT_NATIVE_MAP_INITIAL_CAPACITY = 10;

// -----------------------------------------------------------------------------

corevm::types::native_map::native_map()
  :
  native_map_base(DEFAULT_NATIVE_MAP_INITIAL_CAPACITY)
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
  native_map_base(other)
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
{
  throw corevm::types::conversion_error("int8", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map::operator int8_t() const
{
  throw corevm::types::conversion_error("map", "int8");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator+() const
{
  throw corevm::types::invalid_operator_error("+", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator-() const
{
  throw corevm::types::invalid_operator_error("-", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator++() const
{
  throw corevm::types::invalid_operator_error("++", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator--() const
{
  throw corevm::types::invalid_operator_error("--", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator!() const
{
  throw corevm::types::invalid_operator_error("!", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator~() const
{
  throw corevm::types::invalid_operator_error("~", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator+(const native_map&) const
{
  throw corevm::types::invalid_operator_error("+", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator-(const native_map&) const
{
  throw corevm::types::invalid_operator_error("-", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator*(const native_map&) const
{
  throw corevm::types::invalid_operator_error("*", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator/(const native_map&) const
{
  throw corevm::types::invalid_operator_error("/", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator%(const native_map&) const
{
  throw corevm::types::invalid_operator_error("%", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator&&(const native_map&) const
{
  throw corevm::types::invalid_operator_error("&&", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator||(const native_map&) const
{
  throw corevm::types::invalid_operator_error("||", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator&(const native_map&) const
{
  throw corevm::types::invalid_operator_error("&", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator|(const native_map&) const
{
  throw corevm::types::invalid_operator_error("|", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator^(const native_map&) const
{
  throw corevm::types::invalid_operator_error("^", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator<<(const native_map&) const
{
  throw corevm::types::invalid_operator_error("<<", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator>>(const native_map&) const
{
  throw corevm::types::invalid_operator_error(">>", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator<(const native_map&) const
{
  throw corevm::types::invalid_operator_error("<", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator<=(const native_map&) const
{
  throw corevm::types::invalid_operator_error("<=", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator>(const native_map&) const
{
  throw corevm::types::invalid_operator_error(">", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map&
corevm::types::native_map::operator>=(const native_map&) const
{
  throw corevm::types::invalid_operator_error(">=", "map");
}

// -----------------------------------------------------------------------------

corevm::types::native_map::mapped_type&
corevm::types::native_map::at(const key_type& k)
  throw(corevm::types::out_of_range_error)
{
  try
  {
    return native_map_base::at(k);
  }
  catch (const std::out_of_range&)
  {
    throw corevm::types::out_of_range_error("Map key out of range");
  }
}

// -----------------------------------------------------------------------------
