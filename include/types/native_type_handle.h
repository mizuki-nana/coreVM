#ifndef COREVM_NATIVE_TYPE_HANDLE_H_
#define COREVM_NATIVE_TYPE_HANDLE_H_

#include <boost/variant/variant.hpp>
#include <boost/variant/static_visitor.hpp>
#include "operators.h"
#include "types.h"


namespace corevm {


namespace types {


using native_type_handle = typename boost::variant<
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
  corevm::types::array
>;


template<class op>
class native_type_unary_visitor : public boost::static_visitor<native_type_handle> {
public:
  template<typename T>
  native_type_handle operator()(const T& operand) const {
    return T(op().template operator()<T>(operand));
  }
};


template<class op>
class native_type_binary_visitor : public boost::static_visitor<native_type_handle> {
public:
  template<typename T, typename U>
  native_type_handle operator()(const T& lhs, const U& rhs) const {
    if(sizeof(lhs) >= sizeof(rhs)) {
      return T(op().template operator()<T>(lhs, rhs));
    } else {
      return U(op().template operator()<U>(lhs, rhs));
    }
  }

  native_type_handle operator()(
    const corevm::types::string& lhs, const corevm::types::string& rhs) const {
    return corevm::types::boolean(op().template operator()<corevm::types::string>(lhs, rhs));
  }

  native_type_handle operator()(
    const corevm::types::array& lhs, const corevm::types::array& rhs) const {
    return corevm::types::boolean(op().template operator()<corevm::types::array>(lhs, rhs));
  }
};


template<typename T>
class native_type_value_visitor : public boost::static_visitor<T> {
public:
  template<typename H>
  T operator()(const H& handle) const {
    return static_cast<T>(handle.value);
  }
};


/* Unary operator visitors */
class native_type_positive_visitor : public native_type_unary_visitor<positive> {};
class native_type_negation_visitor : public native_type_unary_visitor<negation> {};
class native_type_increment_visitor : public native_type_unary_visitor<increment> {};
class native_type_decrement_visitor : public native_type_unary_visitor<decrement> {};
class native_type_logical_not_visitor : public native_type_unary_visitor<logical_not> {};
class native_type_bitwise_not_visitor : public native_type_unary_visitor<bitwise_not> {};


/* Binary operator visitors */
class native_type_addition_visitor : public native_type_binary_visitor<addition> {};
class native_type_subtraction_visitor : public native_type_binary_visitor<subtraction> {};
class native_type_multiplication_visitor : public native_type_binary_visitor<multiplication> {};
class native_type_division_visitor : public native_type_binary_visitor<division> {};
class native_type_modulus_visitor : public native_type_binary_visitor<modulus> {};
class native_type_logical_and_visitor : public native_type_binary_visitor<logical_and> {};
class native_type_logical_or_visitor : public native_type_binary_visitor<logical_or> {};
class native_type_bitwise_and_visitor : public native_type_binary_visitor<bitwise_and> {};
class native_type_bitwise_or_visitor : public native_type_binary_visitor<bitwise_or> {};
class native_type_bitwise_xor_visitor : public native_type_binary_visitor<bitwise_xor> {};
class native_type_bitwise_left_shift_visitor : public native_type_binary_visitor<bitwise_left_shift> {};
class native_type_bitwise_right_shift_visitor : public native_type_binary_visitor<bitwise_right_shift> {};
class native_type_eq_visitor : public native_type_binary_visitor<eq> {};
class native_type_neq_visitor : public native_type_binary_visitor<neq> {};
class native_type_gt_visitor : public native_type_binary_visitor<gt> {};
class native_type_lt_visitor : public native_type_binary_visitor<lt> {};
class native_type_gte_visitor : public native_type_binary_visitor<gte> {};
class native_type_lte_visitor : public native_type_binary_visitor<lte> {};


template<typename T>
T
get_value_from_handle(corevm::types::native_type_handle& handle)
{
  return boost::apply_visitor(
    corevm::types::native_type_value_visitor<T>(), handle
  );
}


template<class operator_visitor>
corevm::types::native_type_handle
apply_unary_visitor(corevm::types::native_type_handle& handle)
{
  return boost::apply_visitor(operator_visitor(), handle);
}


template<class operator_visitor>
corevm::types::native_type_handle
apply_binary_visitor(
  corevm::types::native_type_handle& h1, corevm::types::native_type_handle& h2)
{
  return boost::apply_visitor(operator_visitor(), h1, h2);
}


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_NATIVE_TYPE_HANDLE_H_ */