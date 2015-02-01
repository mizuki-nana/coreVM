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
#ifndef COREVM_NATIVE_ARRAY_H_
#define COREVM_NATIVE_ARRAY_H_

#include "errors.h"

#include <cstdint>
#include <stdexcept>
#include <vector>


namespace corevm {


namespace types {


typedef uint64_t element_type;


using native_array_base = typename std::vector<element_type>;


class native_array : public native_array_base
{
public:
  explicit native_array() : native_array_base() {}

  native_array(const native_array_base& x) : native_array_base(x) {}
  native_array(native_array_base&& x) : native_array_base(x) {}

  native_array(std::initializer_list<value_type> il) : native_array_base(il) {}

  native_array(int8_t)
  {
    throw corevm::types::conversion_error("int8", "array");
  }

  operator int8_t() const
  {
    throw corevm::types::conversion_error("array", "int8");
  }

  native_array& operator+() const
  {
    throw corevm::types::invalid_operator_error("+", "array");
  }

  native_array& operator-() const
  {
    throw corevm::types::invalid_operator_error("-", "array");
  }

  native_array& operator++() const
  {
    throw corevm::types::invalid_operator_error("++", "array");
  }

  native_array& operator--() const
  {
    throw corevm::types::invalid_operator_error("--", "array");
  }

  native_array& operator!() const
  {
    throw corevm::types::invalid_operator_error("!", "array");
  }

  native_array& operator~() const
  {
    throw corevm::types::invalid_operator_error("~", "array");
  }

  native_array& operator+(const native_array&) const
  {
    throw corevm::types::invalid_operator_error("+", "array");
  }

  native_array& operator-(const native_array&) const
  {
    throw corevm::types::invalid_operator_error("-", "array");
  }

  native_array& operator*(const native_array&) const
  {
    throw corevm::types::invalid_operator_error("*", "array");
  }

  native_array& operator/(const native_array&) const
  {
    throw corevm::types::invalid_operator_error("/", "array");
  }

  native_array& operator%(const native_array&) const
  {
    throw corevm::types::invalid_operator_error("%", "array");
  }

  native_array& operator&&(const native_array&) const
  {
    throw corevm::types::invalid_operator_error("&&", "array");
  }

  native_array& operator||(const native_array&) const
  {
    throw corevm::types::invalid_operator_error("||", "array");
  }

  native_array& operator&(const native_array&) const
  {
    throw corevm::types::invalid_operator_error("&", "array");
  }

  native_array& operator|(const native_array&) const
  {
    throw corevm::types::invalid_operator_error("|", "array");
  }

  native_array& operator^(const native_array&) const
  {
    throw corevm::types::invalid_operator_error("^", "array");
  }

  native_array& operator<<(const native_array&) const
  {
    throw corevm::types::invalid_operator_error("<<", "array");
  }

  native_array& operator>>(const native_array&) const
  {
    throw corevm::types::invalid_operator_error(">>", "array");
  }

  reference at(size_type n) throw(corevm::types::out_of_range_error)
  {
    try
    {
      return native_array_base::at(n);
    }
    catch (const std::out_of_range&)
    {
      throw corevm::types::out_of_range_error("Array index out of range");
    }
  }

  const_reference at(size_type n) const throw(corevm::types::out_of_range_error)
  {
    try
    {
      return native_array_base::at(n);
    }
    catch (const std::out_of_range&)
    {
      throw corevm::types::out_of_range_error("Array index out of range");
    }
  }
};


}; /* end namespace types */


}; /* end namespace corevm */


#endif /* COREVM_NATIVE_ARRAY_H_ */
