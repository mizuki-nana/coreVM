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
#ifndef COREVM_NATIVE_TYPE_HANDLE_H_
#define COREVM_NATIVE_TYPE_HANDLE_H_

#include "operators.h"
#include "types.h"

#include "variant/static_visitor.h"
#include "variant/variant.h"


namespace corevm {


namespace types {


// -----------------------------------------------------------------------------

using native_type_handle = typename corevm::types::variant::variant<
  corevm::types::int8,
  corevm::types::uint8,
  corevm::types::int16,
  corevm::types::uint16,
  corevm::types::int32,
  corevm::types::uint32,
  corevm::types::int64,
  corevm::types::uint64,
  corevm::types::boolean,
  corevm::types::decimal,
  corevm::types::decimal2,
  corevm::types::string,
  corevm::types::array,
  corevm::types::map
>;

// -----------------------------------------------------------------------------

template<class op>
class native_type_intrinsic_unary_visitor : public variant::static_visitor<native_type_handle>
{
public:
  template<typename T>
  native_type_handle operator()(const T& operand) const
  {
    return op().template operator()<T>(operand);
  }
};

// -----------------------------------------------------------------------------

template<class op>
class native_type_parameterized_intrinsic_unary_visitor : public variant::static_visitor<native_type_handle>
{
public:
  template<typename... Arguments>
  explicit native_type_parameterized_intrinsic_unary_visitor(Arguments... args)
    :
    m_op(args...)
  {
  }

  template<typename T>
  native_type_handle operator()(const T& operand) const
  {
    return m_op.template operator()<T>(operand);
  }

private:
  op m_op;
};

// -----------------------------------------------------------------------------

template<class op, class R>
class native_type_cast_unary_visitor : public variant::static_visitor<R>
{
public:
  template<typename T>
  R operator()(const T& operand) const
  {
    return R(op().template operator()<T>(operand));
  }
};

// -----------------------------------------------------------------------------

template<class op>
class native_type_unary_visitor : public variant::static_visitor<native_type_handle>
{
public:
  template<typename T>
  native_type_handle operator()(const T& operand) const
  {
    return T(op().template operator()<T>(operand));
  }
};

// -----------------------------------------------------------------------------

template<class op>
class native_type_binary_visitor : public variant::static_visitor<native_type_handle>
{
public:
  template<typename T, typename U>
  native_type_handle operator()(const T& lhs, const U& rhs) const
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

  native_type_handle operator()(
    const corevm::types::string& lhs, const corevm::types::string& rhs) const
  {
    return corevm::types::boolean(op().template operator()<corevm::types::string>(lhs, rhs));
  }

  native_type_handle operator()(
    const corevm::types::array& lhs, const corevm::types::array& rhs) const
  {
    return corevm::types::boolean(op().template operator()<corevm::types::array>(lhs, rhs));
  }

  native_type_handle operator()(
    const corevm::types::map& lhs, const corevm::types::map& rhs) const
  {
    return corevm::types::boolean(op().template operator()<corevm::types::map>(lhs, rhs));
  }
};

// -----------------------------------------------------------------------------

template<class op, typename R>
class native_type_typed_binary_visitor : public variant::static_visitor<R>
{
public:
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

  R operator()(
    const corevm::types::string& lhs, const corevm::types::string& rhs) const
  {
    return R(op().template operator()<corevm::types::string>(lhs, rhs));
  }

  R operator()(
    const corevm::types::array& lhs, const corevm::types::array& rhs) const
  {
    return R(op().template operator()<corevm::types::array>(lhs, rhs));
  }

  R operator()(
    const corevm::types::map& lhs, const corevm::types::map& rhs) const
  {
    return R(op().template operator()<corevm::types::map>(lhs, rhs));
  }
};

// -----------------------------------------------------------------------------

template<typename T>
class native_type_value_visitor : public variant::static_visitor<T>
{
public:
  template<typename H>
  T operator()(const H& handle) const
  {
    return static_cast<T>(handle.value);
  }
};

// -----------------------------------------------------------------------------

/* Unary operator visitors */
class native_type_positive_visitor : public native_type_unary_visitor<positive> {};
class native_type_negation_visitor : public native_type_unary_visitor<negation> {};
class native_type_increment_visitor : public native_type_unary_visitor<increment> {};
class native_type_decrement_visitor : public native_type_unary_visitor<decrement> {};
class native_type_logical_not_visitor : public native_type_cast_unary_visitor<logical_not, corevm::types::boolean> {};
class native_type_bitwise_not_visitor : public native_type_unary_visitor<bitwise_not> {};
class native_type_truthy_visitor : public native_type_cast_unary_visitor<truthy, corevm::types::boolean> {};
class native_type_repr_visitor : public native_type_intrinsic_unary_visitor<repr> {};
class native_type_hash_visitor : public native_type_cast_unary_visitor<hash, corevm::types::int64> {};

typedef native_type_parameterized_intrinsic_unary_visitor<slice> native_type_slice_visitor;
typedef native_type_parameterized_intrinsic_unary_visitor<stride> native_type_stride_visitor;

class native_type_reverse_visitor : public native_type_unary_visitor<reverse> {};

// -----------------------------------------------------------------------------

/* Binary operator visitors */
class native_type_addition_visitor : public native_type_binary_visitor<addition> {};
class native_type_subtraction_visitor : public native_type_binary_visitor<subtraction> {};
class native_type_multiplication_visitor : public native_type_binary_visitor<multiplication> {};
class native_type_division_visitor : public native_type_binary_visitor<division> {};
class native_type_modulus_visitor : public native_type_binary_visitor<modulus> {};
class native_type_pow_visitor : public native_type_binary_visitor<pow_op> {};
class native_type_logical_and_visitor : public native_type_typed_binary_visitor<logical_and, corevm::types::boolean> {};
class native_type_logical_or_visitor : public native_type_typed_binary_visitor<logical_or, corevm::types::boolean> {};
class native_type_bitwise_and_visitor : public native_type_binary_visitor<bitwise_and> {};
class native_type_bitwise_or_visitor : public native_type_binary_visitor<bitwise_or> {};
class native_type_bitwise_xor_visitor : public native_type_binary_visitor<bitwise_xor> {};
class native_type_bitwise_left_shift_visitor : public native_type_binary_visitor<bitwise_left_shift> {};
class native_type_bitwise_right_shift_visitor : public native_type_binary_visitor<bitwise_right_shift> {};
class native_type_eq_visitor : public native_type_typed_binary_visitor<eq, corevm::types::boolean> {};
class native_type_neq_visitor : public native_type_typed_binary_visitor<neq, corevm::types::boolean> {};
class native_type_gt_visitor : public native_type_typed_binary_visitor<gt, corevm::types::boolean> {};
class native_type_lt_visitor : public native_type_typed_binary_visitor<lt, corevm::types::boolean> {};
class native_type_gte_visitor : public native_type_typed_binary_visitor<gte, corevm::types::boolean> {};
class native_type_lte_visitor : public native_type_typed_binary_visitor<lte, corevm::types::boolean> {};

// -----------------------------------------------------------------------------

template<typename T>
T
get_value_from_handle(corevm::types::native_type_handle& handle)
{
  return variant::apply_visitor(
    corevm::types::native_type_value_visitor<T>(), handle
  );
}

// -----------------------------------------------------------------------------

template<class operator_visitor>
corevm::types::native_type_handle
apply_unary_visitor(corevm::types::native_type_handle& handle)
{
  return variant::apply_visitor(operator_visitor(), handle);
}

// -----------------------------------------------------------------------------

template<class operator_visitor, typename... Arguments>
corevm::types::native_type_handle
apply_unary_visitor_parameterized(corevm::types::native_type_handle& handle, Arguments... args)
{
  return variant::apply_visitor(operator_visitor(args...), handle);
}

// -----------------------------------------------------------------------------

template<class operator_visitor>
corevm::types::native_type_handle
apply_binary_visitor(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs)
{
  return variant::apply_visitor(operator_visitor(), lhs, rhs);
}

// -----------------------------------------------------------------------------


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_NATIVE_TYPE_HANDLE_H_ */
