#ifndef COREVM_TYPES_INTERFACES_H_
#define COREVM_TYPES_INTERFACES_H_

#include "native_type_handle.h"


namespace corevm {


namespace types {


/**************************** UNARY OPERATIONS ********************************/

void interface_apply_positive_operator(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result);

void interface_apply_negation_operator(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result);

void interface_apply_increment_operator(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result);

void interface_apply_decrement_operator(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result);

void interface_apply_logical_not_operator(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result);

void interface_apply_bitwise_not_operator(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result);


/**************************** BINARY OPERATIONS *******************************/

void interface_apply_addition_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result);

void interface_apply_subtraction_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result);

void interface_apply_multiplication_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result);

void interface_apply_division_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result);

void interface_apply_modulus_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result);

void interface_apply_logical_and_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result);

void interface_apply_logical_or_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result);

void interface_apply_bitwise_and_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result);

void interface_apply_bitwise_or_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result);

void interface_apply_bitwise_xor_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result);

void interface_apply_bitwise_left_shift_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result);

void interface_apply_bitwise_right_shift_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result);

void interface_apply_eq_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result);

void interface_apply_neq_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result);

void interface_apply_lt_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result);

void interface_apply_gt_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result);

void interface_apply_lte_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result);

void interface_apply_gte_operator(
  corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, corevm::types::native_type_handle& result);


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_TYPES_INTERFACES_H_ */
