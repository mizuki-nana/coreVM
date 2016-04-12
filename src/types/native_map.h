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
#ifndef COREVM_NATIVE_MAP_H_
#define COREVM_NATIVE_MAP_H_

#include "errors.h"

#include <cstdint>
#include <unordered_map>


namespace corevm {


namespace types {


typedef uint64_t native_map_key_type;
typedef uint64_t native_map_mapped_type;


using native_map_base = typename std::unordered_map<native_map_key_type, native_map_mapped_type>;


class native_map : public native_map_base
{
public:
  native_map();

  native_map(const native_map_base&);

  native_map(native_map_base&&);

  native_map(std::initializer_list<value_type>);

  [[ noreturn ]] /** Avoid compiler warning [-Wmissing-noreturn]. */
  native_map(int8_t);

  operator int8_t() const;

  native_map& operator+() const;

  native_map& operator-() const;

  native_map& operator++() const;

  native_map& operator--() const;

  native_map& operator!() const;

  native_map& operator~() const;

  native_map& operator+(const native_map&) const;

  native_map& operator-(const native_map&) const;

  native_map& operator*(const native_map&) const;

  native_map& operator/(const native_map&) const;

  native_map& operator%(const native_map&) const;

  native_map& operator&&(const native_map&) const;

  native_map& operator||(const native_map&) const;

  native_map& operator&(const native_map&) const;

  native_map& operator|(const native_map&) const;

  native_map& operator^(const native_map&) const;

  native_map& operator<<(const native_map&) const;

  native_map& operator>>(const native_map&) const;

  native_map& operator<(const native_map&) const;

  native_map& operator<=(const native_map&) const;

  native_map& operator>(const native_map&) const;

  native_map& operator>=(const native_map&) const;

  mapped_type& at(const key_type& k);

  const mapped_type& at(const key_type& k) const;
};


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_NATIVE_MAP_H_ */
