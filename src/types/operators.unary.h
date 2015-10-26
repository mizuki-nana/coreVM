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
corevm::types::boolean
corevm::types::increment::operator()(const corevm::types::boolean& handle)
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
corevm::types::boolean
corevm::types::decrement::operator()(const corevm::types::boolean& /* handle */)
{
  THROW(corevm::types::invalid_operator_error("--", "boolean"));
}

// -----------------------------------------------------------------------------

class logical_not : public op<typed_unary_op_tag, corevm::types::boolean>
{
public:
  template<typename T>
  result_type operator()(const T& handle)
  {
    return corevm::types::boolean(!handle);
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
corevm::types::decimal
corevm::types::bitwise_not::operator()(const corevm::types::decimal& /* handle */)
{
  THROW(corevm::types::invalid_operator_error("~", "decimal"));
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::decimal2
corevm::types::bitwise_not::operator()(const corevm::types::decimal2& /* handle */)
{
  THROW(corevm::types::invalid_operator_error("~", "decimal2"));
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::string
corevm::types::bitwise_not::operator()(const corevm::types::string& handle)
{
  return static_cast<corevm::types::string>(~handle);
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::array
corevm::types::bitwise_not::operator()(const corevm::types::array& handle)
{
  return static_cast<corevm::types::array>(~handle);
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::map
corevm::types::bitwise_not::operator()(const corevm::types::map& handle)
{
  return static_cast<corevm::types::map>(~handle);
}

// -----------------------------------------------------------------------------


} /* end namespace types */


} /* end namespace corevm */


#if defined(__clang__) and __clang__
  #pragma clang diagnostic pop
#endif


#endif /* COREVM_OPERATORS_UNARY_H_ */
