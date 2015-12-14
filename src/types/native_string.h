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
#ifndef COREVM_NATIVE_STRING_H_
#define COREVM_NATIVE_STRING_H_

#include "errors.h"

#include <cstdint>
#include <string>


namespace corevm {


namespace types {


typedef std::string native_string_base;


class native_string : public native_string_base
{
public:
  native_string();

  native_string(const char* s);

  native_string(const native_string_base& str);

  native_string(native_string_base&& str);

  native_string(size_t n, char c);

  [[ noreturn ]] /** Avoid compiler warning [-Wmissing-noreturn]. */
  native_string(int8_t);

  template <class InputIterator>
  native_string(InputIterator first, InputIterator last)
    :
    native_string_base(first, last)
  {
  }

  operator int8_t() const;

  native_string& operator+() const;

  native_string& operator-() const;

  native_string& operator++() const;

  native_string& operator--() const;

  native_string& operator!() const;

  native_string& operator~() const;

  native_string& operator+(const native_string&) const;

  native_string& operator-(const native_string&) const;

  native_string& operator*(const native_string&) const;

  native_string& operator/(const native_string&) const;

  native_string& operator%(const native_string&) const;

  native_string& operator&&(const native_string&) const;

  native_string& operator||(const native_string&) const;

  native_string& operator&(const native_string&) const;

  native_string& operator|(const native_string&) const;

  native_string& operator^(const native_string&) const;

  native_string& operator<<(const native_string&) const;

  native_string& operator>>(const native_string&) const;

  reference at(size_type n) throw(out_of_range_error);

  const_reference at(size_type n) const
    throw(out_of_range_error);

  native_string& insert(size_type pos, const native_string& str)
    throw(out_of_range_error);

  native_string& insert(size_type pos, size_type n, value_type c)
    throw(out_of_range_error);

  native_string& erase(size_type pos)
    throw(out_of_range_error);

  native_string& erase(
    size_type pos, size_type len) throw(out_of_range_error);

  native_string& replace(size_type pos, size_type len, const native_string& str)
    throw(out_of_range_error);
};


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_NATIVE_STRING_H_ */
