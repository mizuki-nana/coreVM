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
#include "native_string.h"

#include "errors.h"

#include <cstdint>
#include <stdexcept>


corevm::types::native_string::native_string()
  :
  native_string_base()
{
}

// -----------------------------------------------------------------------------

corevm::types::native_string::native_string(const char* s)
  :
  native_string_base(s)
{
}

// -----------------------------------------------------------------------------

corevm::types::native_string::native_string(const native_string_base& str)
  :
  native_string_base(str)
{
}

// -----------------------------------------------------------------------------

corevm::types::native_string::native_string(native_string_base&& str)
  :
  native_string_base(str)
{
}

// -----------------------------------------------------------------------------

corevm::types::native_string::native_string(int8_t)
{
  throw corevm::types::conversion_error("int8", "string");
}

// -----------------------------------------------------------------------------

corevm::types::native_string::operator int8_t() const
{
  throw corevm::types::conversion_error("string", "int8");
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::operator+() const
{
  throw corevm::types::invalid_operator_error("+", "string");
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::operator-() const
{
  throw corevm::types::invalid_operator_error("-", "string");
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::operator++() const
{
  throw corevm::types::invalid_operator_error("++", "string");
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::operator--() const
{
  throw corevm::types::invalid_operator_error("--", "string");
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::operator!() const
{
  throw corevm::types::invalid_operator_error("!", "string");
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::operator~() const
{
  throw corevm::types::invalid_operator_error("~", "string");
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::operator+(const native_string&) const
{
  throw corevm::types::invalid_operator_error("+", "string");
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::operator-(const native_string&) const
{
  throw corevm::types::invalid_operator_error("-", "string");
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::operator*(const native_string&) const
{
  throw corevm::types::invalid_operator_error("*", "string");
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::operator/(const native_string&) const
{
  throw corevm::types::invalid_operator_error("/", "string");
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::operator%(const native_string&) const
{
  throw corevm::types::invalid_operator_error("%", "string");
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::operator&&(const native_string&) const
{
  throw corevm::types::invalid_operator_error("&&", "string");
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::operator||(const native_string&) const
{
  throw corevm::types::invalid_operator_error("||", "string");
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::operator&(const native_string&) const
{
  throw corevm::types::invalid_operator_error("&", "string");
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::operator|(const native_string&) const
{
  throw corevm::types::invalid_operator_error("|", "string");
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::operator^(const native_string&) const
{
  throw corevm::types::invalid_operator_error("^", "string");
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::operator<<(const native_string&) const
{
  throw corevm::types::invalid_operator_error("<<", "string");
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::operator>>(const native_string&) const
{
  throw corevm::types::invalid_operator_error(">>", "string");
}

// -----------------------------------------------------------------------------

corevm::types::native_string::reference
corevm::types::native_string::at(size_type n) throw(corevm::types::out_of_range_error)
{
  try
  {
    return native_string_base::at(n);
  }
  catch (const std::out_of_range&)
  {
    throw corevm::types::out_of_range_error("String index out of range");
  }
}

// -----------------------------------------------------------------------------

corevm::types::native_string::const_reference
corevm::types::native_string::at(size_type n) const throw(corevm::types::out_of_range_error)
{
  try
  {
    return native_string_base::at(n);
  }
  catch (const std::out_of_range&)
  {
    throw corevm::types::out_of_range_error("String index out of range");
  }
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::insert(
  size_type pos, const corevm::types::native_string& str) throw(corevm::types::out_of_range_error)
{
  try
  {
    return static_cast<corevm::types::native_string&>(native_string_base::insert(pos, str));
  }
  catch (const std::out_of_range&)
  {
    throw corevm::types::out_of_range_error("String index out of range");
  }
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::insert(size_type pos, size_type n, value_type c)
  throw(corevm::types::out_of_range_error)
{
  try
  {
    return static_cast<corevm::types::native_string&>(native_string_base::insert(pos, n, c));
  }
  catch (const std::out_of_range&)
  {
    throw corevm::types::out_of_range_error("String index out of range");
  }
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::erase(size_type pos)
  throw(corevm::types::out_of_range_error)
{
  try
  {
    return static_cast<corevm::types::native_string&>(native_string_base::erase(pos));
  }
  catch (const std::out_of_range&)
  {
    throw corevm::types::out_of_range_error("String index out of range");
  }
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::erase(size_type pos, size_type len)
  throw(corevm::types::out_of_range_error)
{
  try
  {
    return static_cast<corevm::types::native_string&>(native_string_base::erase(pos, len));
  }
  catch (const std::out_of_range&)
  {
    throw corevm::types::out_of_range_error("String index out of range");
  }
}

// -----------------------------------------------------------------------------

corevm::types::native_string&
corevm::types::native_string::replace(size_type pos, size_type len,
  const corevm::types::native_string& str) throw(corevm::types::out_of_range_error)
{
  try
  {
    return static_cast<corevm::types::native_string&>(native_string_base::replace(pos, len, str));
  }
  catch (const std::out_of_range&)
  {
    throw corevm::types::out_of_range_error("String index out of range");
  }
}

// -----------------------------------------------------------------------------
