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
#include "native_map.h"

#include "errors.h"
#include "corevm/macros.h"

#include <cstdint>
#include <stdexcept>
#include <utility>


namespace corevm {
namespace types {

// -----------------------------------------------------------------------------

native_map::native_map()
  :
  native_map_base()
{
}

// -----------------------------------------------------------------------------

native_map::native_map(const native_map_base& other)
  :
  native_map_base(other)
{
}

// -----------------------------------------------------------------------------

native_map::native_map(native_map_base&& other)
  :
  native_map_base(std::forward<native_map_base>(other))
{
}

// -----------------------------------------------------------------------------

native_map::native_map(std::initializer_list<value_type> il)
  :
  native_map_base(il)
{
}

// -----------------------------------------------------------------------------

native_map::native_map(int8_t)
  :
  native_map_base()
{
  THROW(ConversionError("int8", "map"));
}

// -----------------------------------------------------------------------------

native_map::operator int8_t() const
{
  THROW(ConversionError("map", "int8"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator+() const
{
  THROW(InvalidOperatorError("+", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator-() const
{
  THROW(InvalidOperatorError("-", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator++() const
{
  THROW(InvalidOperatorError("++", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator--() const
{
  THROW(InvalidOperatorError("--", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator!() const
{
  THROW(InvalidOperatorError("!", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator~() const
{
  THROW(InvalidOperatorError("~", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator+(const native_map&) const
{
  THROW(InvalidOperatorError("+", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator-(const native_map&) const
{
  THROW(InvalidOperatorError("-", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator*(const native_map&) const
{
  THROW(InvalidOperatorError("*", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator/(const native_map&) const
{
  THROW(InvalidOperatorError("/", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator%(const native_map&) const
{
  THROW(InvalidOperatorError("%", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator&&(const native_map&) const
{
  THROW(InvalidOperatorError("&&", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator||(const native_map&) const
{
  THROW(InvalidOperatorError("||", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator&(const native_map&) const
{
  THROW(InvalidOperatorError("&", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator|(const native_map&) const
{
  THROW(InvalidOperatorError("|", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator^(const native_map&) const
{
  THROW(InvalidOperatorError("^", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator<<(const native_map&) const
{
  THROW(InvalidOperatorError("<<", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator>>(const native_map&) const
{
  THROW(InvalidOperatorError(">>", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator<(const native_map&) const
{
  THROW(InvalidOperatorError("<", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator<=(const native_map&) const
{
  THROW(InvalidOperatorError("<=", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator>(const native_map&) const
{
  THROW(InvalidOperatorError(">", "map"));
}

// -----------------------------------------------------------------------------

native_map&
native_map::operator>=(const native_map&) const
{
  THROW(InvalidOperatorError(">=", "map"));
}

// -----------------------------------------------------------------------------

native_map::mapped_type&
native_map::at(const key_type& k)
{
  return const_cast<mapped_type&>(
    static_cast<const native_map&>(*this).at(k));
}

// -----------------------------------------------------------------------------

const native_map::mapped_type&
native_map::at(const key_type& k) const
{
  auto itr = find(k);

  if (itr == end())
  {
    THROW(OutOfRangeError("Map key out of range"));
  }

  return itr->second;
}

// -----------------------------------------------------------------------------

} /* end namespace types */
} /* end namespace corevm */
