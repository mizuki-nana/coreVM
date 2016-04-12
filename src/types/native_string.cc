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
#include "native_string.h"

#include "errors.h"
#include "corevm/macros.h"

#include <cstdint>
#include <stdexcept>
#include <utility>


namespace corevm {


namespace types {


// -----------------------------------------------------------------------------

native_string::native_string()
  :
  native_string_base()
{
}

// -----------------------------------------------------------------------------

native_string::native_string(const char* s)
  :
  native_string_base(s)
{
}

// -----------------------------------------------------------------------------

native_string::native_string(const native_string_base& str)
  :
  native_string_base(str)
{
}

// -----------------------------------------------------------------------------

native_string::native_string(native_string_base&& str)
  :
  native_string_base(std::forward<native_string_base>(str))
{
}

// -----------------------------------------------------------------------------

native_string::native_string(size_t n, char c)
  :
  native_string_base(n, c)
{
}

// -----------------------------------------------------------------------------

native_string::native_string(int8_t)
  :
  native_string_base()
{
  THROW(ConversionError("int8", "string"));
}

// -----------------------------------------------------------------------------

native_string::operator int8_t() const
{
  THROW(ConversionError("string", "int8"));
}

// -----------------------------------------------------------------------------

native_string&
native_string::operator+() const
{
  THROW(InvalidOperatorError("+", "string"));
}

// -----------------------------------------------------------------------------

native_string&
native_string::operator-() const
{
  THROW(InvalidOperatorError("-", "string"));
}

// -----------------------------------------------------------------------------

native_string&
native_string::operator++() const
{
  THROW(InvalidOperatorError("++", "string"));
}

// -----------------------------------------------------------------------------

native_string&
native_string::operator--() const
{
  THROW(InvalidOperatorError("--", "string"));
}

// -----------------------------------------------------------------------------

native_string&
native_string::operator!() const
{
  THROW(InvalidOperatorError("!", "string"));
}

// -----------------------------------------------------------------------------

native_string&
native_string::operator~() const
{
  THROW(InvalidOperatorError("~", "string"));
}

// -----------------------------------------------------------------------------

native_string&
native_string::operator+(const native_string&) const
{
  THROW(InvalidOperatorError("+", "string"));
}

// -----------------------------------------------------------------------------

native_string&
native_string::operator-(const native_string&) const
{
  THROW(InvalidOperatorError("-", "string"));
}

// -----------------------------------------------------------------------------

native_string&
native_string::operator*(const native_string&) const
{
  THROW(InvalidOperatorError("*", "string"));
}

// -----------------------------------------------------------------------------

native_string&
native_string::operator/(const native_string&) const
{
  THROW(InvalidOperatorError("/", "string"));
}

// -----------------------------------------------------------------------------

native_string&
native_string::operator%(const native_string&) const
{
  THROW(InvalidOperatorError("%", "string"));
}

// -----------------------------------------------------------------------------

native_string&
native_string::operator&&(const native_string&) const
{
  THROW(InvalidOperatorError("&&", "string"));
}

// -----------------------------------------------------------------------------

native_string&
native_string::operator||(const native_string&) const
{
  THROW(InvalidOperatorError("||", "string"));
}

// -----------------------------------------------------------------------------

native_string&
native_string::operator&(const native_string&) const
{
  THROW(InvalidOperatorError("&", "string"));
}

// -----------------------------------------------------------------------------

native_string&
native_string::operator|(const native_string&) const
{
  THROW(InvalidOperatorError("|", "string"));
}

// -----------------------------------------------------------------------------

native_string&
native_string::operator^(const native_string&) const
{
  THROW(InvalidOperatorError("^", "string"));
}

// -----------------------------------------------------------------------------

native_string&
native_string::operator<<(const native_string&) const
{
  THROW(InvalidOperatorError("<<", "string"));
}

// -----------------------------------------------------------------------------

native_string&
native_string::operator>>(const native_string&) const
{
  THROW(InvalidOperatorError(">>", "string"));
}

// -----------------------------------------------------------------------------

native_string::reference
native_string::at(size_type n)
{
  try
  {
    return native_string_base::at(n);
  }
  catch (const std::out_of_range&)
  {
    THROW(OutOfRangeError("String index out of range"));
  }
}

// -----------------------------------------------------------------------------

native_string::const_reference
native_string::at(size_type n) const
{
  try
  {
    return native_string_base::at(n);
  }
  catch (const std::out_of_range&)
  {
    THROW(OutOfRangeError("String index out of range"));
  }
}

// -----------------------------------------------------------------------------

native_string&
native_string::insert(size_type pos, const native_string& str)
{
  try
  {
    return static_cast<native_string&>(
      native_string_base::insert(pos, str));
  }
  catch (const std::out_of_range&)
  {
    THROW(OutOfRangeError("String index out of range"));
  }
}

// -----------------------------------------------------------------------------

native_string&
native_string::insert(size_type pos, size_type n, value_type c)
{
  try
  {
    return static_cast<native_string&>(
      native_string_base::insert(pos, n, c));
  }
  catch (const std::out_of_range&)
  {
    THROW(OutOfRangeError("String index out of range"));
  }
}

// -----------------------------------------------------------------------------

native_string&
native_string::erase(size_type pos)
{
  try
  {
    return static_cast<native_string&>(
      native_string_base::erase(pos));
  }
  catch (const std::out_of_range&)
  {
    THROW(OutOfRangeError("String index out of range"));
  }
}

// -----------------------------------------------------------------------------

native_string&
native_string::erase(size_type pos, size_type len)
{
  try
  {
    return static_cast<native_string&>(
      native_string_base::erase(pos, len));
  }
  catch (const std::out_of_range&)
  {
    THROW(OutOfRangeError("String index out of range"));
  }
}

// -----------------------------------------------------------------------------

native_string&
native_string::replace(size_type pos, size_type len, const native_string& str)
{
  try
  {
    return static_cast<native_string&>(
      native_string_base::replace(pos, len, str));
  }
  catch (const std::out_of_range&)
  {
    THROW(OutOfRangeError("String index out of range"));
  }
}

// -----------------------------------------------------------------------------


} /* end namespace types */


} /* end namespace corevm */
