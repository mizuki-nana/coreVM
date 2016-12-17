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
#ifndef COREVM_OPERATORS_UNARY_H_
#define COREVM_OPERATORS_UNARY_H_

#include "errors.h"
#include "operators.base.h"
#include "types.h"
#include "corevm/macros.h"

#if defined(__clang__) and __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wconversion"
#endif


namespace corevm {
namespace types {

// -----------------------------------------------------------------------------

class positive : public op<unary_op_tag>
{
public:
  template<typename T>
  T operator()(const T& oprd)
  {
    return +oprd;
  }
};

// -----------------------------------------------------------------------------

class negation : public op<unary_op_tag>
{
public:
  template<typename T>
  T operator()(const T& oprd)
  {
    return -oprd;
  }
};

// -----------------------------------------------------------------------------

class increment : public op<unary_op_tag>
{
public:
  template<typename T>
  T operator()(const T& oprd)
  {
    T& oprd_ = const_cast<T&>(oprd);
    return ++oprd_;
  }
};

// -----------------------------------------------------------------------------

// Special handling of boolean type is required since the increment operator
// on bool is deprecated.
template<>
inline
boolean
increment::operator()(const boolean& oprd)
{
  uint64_t int_val = static_cast<uint64_t>(oprd);
  ++int_val;
  return int_val;
}

// -----------------------------------------------------------------------------

class decrement : public op<unary_op_tag>
{
public:
  template<typename T>
  T operator()(const T& oprd)
  {
    T& oprd_ = const_cast<T&>(oprd);
    --oprd_;
    return oprd;
  }
};

// -----------------------------------------------------------------------------

template<>
inline
boolean
decrement::operator()(const boolean& /* oprd */)
{
  THROW(InvalidOperatorError("--", "boolean"));
}

// -----------------------------------------------------------------------------

class logical_not : public op<typed_unary_op_tag, boolean>
{
public:
  template<typename T>
  result_type operator()(const T& oprd)
  {
    return boolean(!oprd);
  }
};

// -----------------------------------------------------------------------------

class bitwise_not : public op<unary_op_tag>
{
public:
  template<typename T>
  T operator()(const T& oprd)
  {
    return ~oprd;
  }
};

// -----------------------------------------------------------------------------

template<>
inline
decimal
bitwise_not::operator()(const decimal& /* oprd */)
{
  THROW(InvalidOperatorError("~", "decimal"));
}

// -----------------------------------------------------------------------------

template<>
inline
decimal2
bitwise_not::operator()(const decimal2& /* oprd */)
{
  THROW(InvalidOperatorError("~", "decimal2"));
}

// -----------------------------------------------------------------------------

template<>
inline
string
bitwise_not::operator()(const string& oprd)
{
  return static_cast<string>(~oprd);
}

// -----------------------------------------------------------------------------

template<>
inline
array
bitwise_not::operator()(const array& oprd)
{
  return static_cast<array>(~oprd);
}

// -----------------------------------------------------------------------------

template<>
inline
map
bitwise_not::operator()(const map& oprd)
{
  return static_cast<map>(~oprd);
}

// -----------------------------------------------------------------------------

} /* end namespace types */
} /* end namespace corevm */


#if defined(__clang__) and __clang__
  #pragma clang diagnostic pop
#endif


#endif /* COREVM_OPERATORS_UNARY_H_ */
