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
  T operator()(const T& handle)
  {
    return +handle;
  }
};

// -----------------------------------------------------------------------------

class negation : public op<unary_op_tag>
{
public:
  template<typename T>
  T operator()(const T& handle)
  {
    return -handle;
  }
};

// -----------------------------------------------------------------------------

class increment : public op<unary_op_tag>
{
public:
  template<typename T>
  T operator()(const T& handle)
  {
    T& handle_ = const_cast<T&>(handle);
    return ++handle_;
  }
};

// -----------------------------------------------------------------------------

// Special handling of boolean type is required since the increment operator
// on bool is deprecated.
template<>
inline
boolean
increment::operator()(const boolean& handle)
{
  uint64_t int_val = static_cast<uint64_t>(handle);
  ++int_val;
  return int_val;
}

// -----------------------------------------------------------------------------

class decrement : public op<unary_op_tag>
{
public:
  template<typename T>
  T operator()(const T& handle)
  {
    T& handle_ = const_cast<T&>(handle);
    --handle_;
    return handle;
  }
};

// -----------------------------------------------------------------------------

template<>
inline
boolean
decrement::operator()(const boolean& /* handle */)
{
  THROW(invalid_operator_error("--", "boolean"));
}

// -----------------------------------------------------------------------------

class logical_not : public op<typed_unary_op_tag, boolean>
{
public:
  template<typename T>
  result_type operator()(const T& handle)
  {
    return boolean(!handle);
  }
};

// -----------------------------------------------------------------------------

class bitwise_not : public op<unary_op_tag>
{
public:
  template<typename T>
  T operator()(const T& handle)
  {
    return ~handle;
  }
};

// -----------------------------------------------------------------------------

template<>
inline
decimal
bitwise_not::operator()(const decimal& /* handle */)
{
  THROW(invalid_operator_error("~", "decimal"));
}

// -----------------------------------------------------------------------------

template<>
inline
decimal2
bitwise_not::operator()(const decimal2& /* handle */)
{
  THROW(invalid_operator_error("~", "decimal2"));
}

// -----------------------------------------------------------------------------

template<>
inline
string
bitwise_not::operator()(const string& handle)
{
  return static_cast<string>(~handle);
}

// -----------------------------------------------------------------------------

template<>
inline
array
bitwise_not::operator()(const array& handle)
{
  return static_cast<array>(~handle);
}

// -----------------------------------------------------------------------------

template<>
inline
map
bitwise_not::operator()(const map& handle)
{
  return static_cast<map>(~handle);
}

// -----------------------------------------------------------------------------


} /* end namespace types */


} /* end namespace corevm */


#if defined(__clang__) and __clang__
  #pragma clang diagnostic pop
#endif


#endif /* COREVM_OPERATORS_UNARY_H_ */
