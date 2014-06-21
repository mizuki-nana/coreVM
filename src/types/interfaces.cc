#include "../../include/types/interfaces.h"

/************************** UNARY OPERATIONS **********************************/

template<typename native_type_visitor_type>
inline void __interface_apply_unary_operator(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  corevm::types::native_type_handle _result;
  _result = corevm::types::apply_unary_visitor<native_type_visitor_type>(operand);
  result = _result;
}

void corevm::types::interface_apply_positive_operator(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_unary_operator<corevm::types::native_type_positive_visitor>(operand, result);
}

void corevm::types::interface_apply_negation_operator(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_unary_operator<corevm::types::native_type_negation_visitor>(operand, result);
}

void corevm::types::interface_apply_increment_operator(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_unary_operator<corevm::types::native_type_increment_visitor>(operand, result);
}

void corevm::types::interface_apply_decrement_operator(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_unary_operator<corevm::types::native_type_decrement_visitor>(operand, result);
}

void corevm::types::interface_apply_logical_not_operator(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_unary_operator<corevm::types::native_type_logical_not_visitor>(operand, result);
}

void corevm::types::interface_apply_bitwise_not_operator(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_unary_operator<corevm::types::native_type_bitwise_not_visitor>(operand, result);
}

/************************** BINARY OPERATIONS **********************************/

template<typename native_type_visitor_type>
inline void __interface_apply_binary_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result)
{
  corevm::types::native_type_handle _result;
  _result = corevm::types::apply_binary_visitor<native_type_visitor_type>(lhs, rhs);
  result = _result;
}

void corevm::types::interface_apply_addition_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result)
{
  __interface_apply_binary_operator<corevm::types::native_type_addition_visitor>(lhs, rhs, result);
}

void corevm::types::interface_apply_subtraction_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result)
{
  __interface_apply_binary_operator<corevm::types::native_type_subtraction_visitor>(lhs, rhs, result);
}

void corevm::types::interface_apply_multiplication_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result)
{
  __interface_apply_binary_operator<corevm::types::native_type_multiplication_visitor>(lhs, rhs, result);
}

void corevm::types::interface_apply_division_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result)
{
  __interface_apply_binary_operator<corevm::types::native_type_division_visitor>(lhs, rhs, result);
}

void corevm::types::interface_apply_modulus_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result)
{
  __interface_apply_binary_operator<corevm::types::native_type_modulus_visitor>(lhs, rhs, result);
}

void corevm::types::interface_apply_logical_and_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result)
{
  __interface_apply_binary_operator<corevm::types::native_type_logical_and_visitor>(lhs, rhs, result);
}

void corevm::types::interface_apply_logical_or_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result)
{
  __interface_apply_binary_operator<corevm::types::native_type_logical_or_visitor>(lhs, rhs, result);
}

void corevm::types::interface_apply_bitwise_and_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result)
{
  __interface_apply_binary_operator<corevm::types::native_type_bitwise_and_visitor>(lhs, rhs, result);
}

void corevm::types::interface_apply_bitwise_or_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result)
{
  __interface_apply_binary_operator<corevm::types::native_type_bitwise_or_visitor>(lhs, rhs, result);
}

void corevm::types::interface_apply_bitwise_xor_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result)
{
  __interface_apply_binary_operator<corevm::types::native_type_bitwise_xor_visitor>(lhs, rhs, result);
}

void corevm::types::interface_apply_bitwise_left_shift_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result)
{
  __interface_apply_binary_operator<corevm::types::native_type_bitwise_left_shift_visitor>(lhs, rhs, result);
}

void corevm::types::interface_apply_bitwise_right_shift_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result)
{
  __interface_apply_binary_operator<corevm::types::native_type_bitwise_right_shift_visitor>(lhs, rhs, result);
}

void corevm::types::interface_apply_eq_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result)
{
  __interface_apply_binary_operator<corevm::types::native_type_eq_visitor>(lhs, rhs, result);
}

void corevm::types::interface_apply_neq_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result)
{
  __interface_apply_binary_operator<corevm::types::native_type_neq_visitor>(lhs, rhs, result);
}

void corevm::types::interface_apply_lt_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result)
{
  __interface_apply_binary_operator<corevm::types::native_type_lt_visitor>(lhs, rhs, result);
}

void corevm::types::interface_apply_gt_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result)
{
  __interface_apply_binary_operator<corevm::types::native_type_gt_visitor>(lhs, rhs, result);
}

void corevm::types::interface_apply_lte_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result)
{
  __interface_apply_binary_operator<corevm::types::native_type_lte_visitor>(lhs, rhs, result);
}

void corevm::types::interface_apply_gte_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result)
{
  __interface_apply_binary_operator<corevm::types::native_type_gte_visitor>(lhs, rhs, result);
}
