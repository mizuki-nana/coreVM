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
#include "native_array.h"

#include "errors.h"
#include "corevm/macros.h"

#include <cstdint>
#include <stdexcept>
#include <utility>


corevm::types::native_array::native_array()
  :
  native_array_base()
{
}

// -----------------------------------------------------------------------------

corevm::types::native_array::native_array(const native_array_base& other)
  :
  native_array_base(other)
{
}

// -----------------------------------------------------------------------------

corevm::types::native_array::native_array(native_array_base&& other)
  :
  native_array_base(std::forward<native_array_base>(other))
{
}

// -----------------------------------------------------------------------------

corevm::types::native_array::native_array(std::initializer_list<value_type> il)
  :
  native_array_base(il)
{
}

// -----------------------------------------------------------------------------

corevm::types::native_array::native_array(int8_t)
{
  THROW(corevm::types::conversion_error("int8", "array"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array::operator int8_t() const
{
  THROW(corevm::types::conversion_error("array", "int8"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array&
corevm::types::native_array::operator+() const
{
  THROW(corevm::types::invalid_operator_error("+", "array"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array&
corevm::types::native_array::operator-() const
{
  THROW(corevm::types::invalid_operator_error("-", "array"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array&
corevm::types::native_array::operator++() const
{
  THROW(corevm::types::invalid_operator_error("++", "array"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array&
corevm::types::native_array::operator--() const
{
  THROW(corevm::types::invalid_operator_error("--", "array"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array&
corevm::types::native_array::operator!() const
{
  THROW(corevm::types::invalid_operator_error("!", "array"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array&
corevm::types::native_array::operator~() const
{
  THROW(corevm::types::invalid_operator_error("~", "array"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array&
corevm::types::native_array::operator+(const native_array&) const
{
  THROW(corevm::types::invalid_operator_error("+", "array"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array&
corevm::types::native_array::operator-(const native_array&) const
{
  THROW(corevm::types::invalid_operator_error("-", "array"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array&
corevm::types::native_array::operator*(const native_array&) const
{
  THROW(corevm::types::invalid_operator_error("*", "array"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array&
corevm::types::native_array::operator/(const native_array&) const
{
  THROW(corevm::types::invalid_operator_error("/", "array"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array&
corevm::types::native_array::operator%(const native_array&) const
{
  THROW(corevm::types::invalid_operator_error("%", "array"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array&
corevm::types::native_array::operator&&(const native_array&) const
{
  THROW(corevm::types::invalid_operator_error("&&", "array"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array&
corevm::types::native_array::operator||(const native_array&) const
{
  THROW(corevm::types::invalid_operator_error("||", "array"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array&
corevm::types::native_array::operator&(const native_array&) const
{
  THROW(corevm::types::invalid_operator_error("&", "array"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array&
corevm::types::native_array::operator|(const native_array&) const
{
  THROW(corevm::types::invalid_operator_error("|", "array"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array&
corevm::types::native_array::operator^(const native_array&) const
{
  THROW(corevm::types::invalid_operator_error("^", "array"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array&
corevm::types::native_array::operator<<(const native_array&) const
{
  THROW(corevm::types::invalid_operator_error("<<", "array"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array&
corevm::types::native_array::operator>>(const native_array&) const
{
  THROW(corevm::types::invalid_operator_error(">>", "array"));
}

// -----------------------------------------------------------------------------

corevm::types::native_array::reference
corevm::types::native_array::at(size_type n)
  throw(corevm::types::out_of_range_error)
{
  try
  {
    return native_array_base::at(n);
  }
  catch (const std::out_of_range&)
  {
    THROW(corevm::types::out_of_range_error("Array index out of range"));
  }
}

// -----------------------------------------------------------------------------

corevm::types::native_array::const_reference
corevm::types::native_array::at(size_type n) const
  throw(corevm::types::out_of_range_error)
{
  try
  {
    return native_array_base::at(n);
  }
  catch (const std::out_of_range&)
  {
    THROW(corevm::types::out_of_range_error("Array index out of range"));
  }
}

// -----------------------------------------------------------------------------
