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
#ifndef COREVM_NATIVE_TYPE_VISITORS_IMPL_H_
#define COREVM_NATIVE_TYPE_VISITORS_IMPL_H_

#include "fwd.h"
#include "variant/static_visitor.h"
#include "variant/variant.h"

#include <type_traits>

#if defined(__clang__) and __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wconversion"
  #pragma clang diagnostic ignored "-Wsign-conversion"
#endif


namespace corevm {
namespace types {

// -----------------------------------------------------------------------------

template<class op>
class native_type_unary_visitor : public variant::static_visitor<NativeTypeValue>
{
public:
  static_assert(std::is_same<typename op::operator_category, unary_op_tag>::value, "");

  template<typename T>
  NativeTypeValue operator()(const T& operand) const
  {
    return op().template operator()(operand);
  }
};

// -----------------------------------------------------------------------------

template<class op>
class native_type_parameterized_unary_visitor : public variant::static_visitor<NativeTypeValue>
{
public:
  static_assert(std::is_same<typename op::operator_category, unary_op_tag>::value, "");

  template<typename... Arguments>
  explicit native_type_parameterized_unary_visitor(Arguments... args)
    :
    m_op(args...)
  {
  }

  template<typename T>
  NativeTypeValue operator()(const T& operand) const
  {
    return m_op.template operator()(operand);
  }

private:
  op m_op;
};

// -----------------------------------------------------------------------------

template<class op, class R>
class native_type_typed_unary_visitor : public variant::static_visitor<R>
{
public:
  static_assert(std::is_same<typename op::operator_category, typed_unary_op_tag>::value, "");
  static_assert(std::is_same<typename op::result_type, R>::value, "");

  template<typename T>
  R operator()(const T& operand) const
  {
    return R(op().template operator()(operand));
  }
};

// -----------------------------------------------------------------------------

template<class op>
class native_type_binary_visitor : public variant::static_visitor<NativeTypeValue>
{
public:
  static_assert(std::is_same<typename op::operator_category, binary_op_tag>::value, "");

  template<typename T, typename U>
  NativeTypeValue operator()(const T& lhs, const U& rhs) const
  {
    if (sizeof(lhs) >= sizeof(rhs))
    {
      return T(op().template operator()<T>(lhs, rhs));
    }
    else
    {
      return U(op().template operator()<U>(lhs, rhs));
    }
  }
};

// -----------------------------------------------------------------------------

template<class op, typename R>
class native_type_typed_binary_visitor : public variant::static_visitor<R>
{
public:
  static_assert(std::is_same<typename op::operator_category, typed_binary_op_tag>::value, "");
  static_assert(std::is_same<typename op::result_type, R>::value, "");

  template<typename T, typename U>
  R operator()(const T& lhs, const U& rhs) const
  {
    if (sizeof(lhs) >= sizeof(rhs))
    {
      return R(op().template operator()<T>(lhs, rhs));
    }
    else
    {
      return R(op().template operator()<U>(lhs, rhs));
    }
  }
};

// -----------------------------------------------------------------------------

template<class op, typename R>
class native_type_typed_cast_binary_visitor : public variant::static_visitor<R>
{
public:
  static_assert(std::is_same<typename op::operator_category, typed_cast_binary_op_tag>::value, "");
  static_assert(std::is_same<typename op::result_type, R>::value, "");

  template<typename T, typename U>
  R operator()(const T& lhs, const U& rhs) const
  {
    return R(op().template operator()(lhs, rhs));
  }
};

// -----------------------------------------------------------------------------

template<typename T>
class native_type_value_visitor : public variant::static_visitor<T>
{
public:
  template<typename H>
  T operator()(const H& oprd) const
  {
    return static_cast<T>(oprd);
  }
};

// -----------------------------------------------------------------------------

} /* end namespace types */
} /* end namespace corevm */


#if defined(__clang__) and __clang__
  #pragma clang diagnostic pop
#endif


#endif /* COREVM_NATIVE_TYPE_VISITORS_IMPL_H_ */
