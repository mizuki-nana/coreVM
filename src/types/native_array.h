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
  native_array();

  native_array(const native_array_base&);

  native_array(native_array_base&&);

  native_array(std::initializer_list<value_type>);

  native_array(int8_t);

  operator int8_t() const;

  native_array& operator+() const;

  native_array& operator-() const;

  native_array& operator++() const;

  native_array& operator--() const;

  native_array& operator!() const;

  native_array& operator~() const;

  native_array& operator+(const native_array&) const;

  native_array& operator-(const native_array&) const;

  native_array& operator*(const native_array&) const;

  native_array& operator/(const native_array&) const;

  native_array& operator%(const native_array&) const;

  native_array& operator&&(const native_array&) const;

  native_array& operator||(const native_array&) const;

  native_array& operator&(const native_array&) const;

  native_array& operator|(const native_array&) const;

  native_array& operator^(const native_array&) const;

  native_array& operator<<(const native_array&) const;

  native_array& operator>>(const native_array&) const;

  reference at(size_type n) throw(corevm::types::out_of_range_error);

  const_reference at(size_type n) const throw(corevm::types::out_of_range_error);
};


}; /* end namespace types */


}; /* end namespace corevm */


#endif /* COREVM_NATIVE_ARRAY_H_ */
