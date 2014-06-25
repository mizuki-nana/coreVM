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


/**************************** STRING OPERATIONS *******************************/

void interface_string_get_size(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result);

void interface_string_clear(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result);

void interface_string_at(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& index, corevm::types::native_type_handle& result);

void interface_string_append(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& str, corevm::types::native_type_handle& result);

void interface_string_pushback(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& c, corevm::types::native_type_handle& result);

void interface_string_insert_str(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& pos, corevm::types::native_type_handle& str, corevm::types::native_type_handle& result);

void interface_string_insert_char(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& pos, corevm::types::native_type_handle& c, corevm::types::native_type_handle& result);

void interface_string_erase(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& pos, corevm::types::native_type_handle& result);

void interface_string_erase2(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& pos, corevm::types::native_type_handle& len, corevm::types::native_type_handle& result);

void interface_string_replace_str(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& pos, corevm::types::native_type_handle& len,
  corevm::types::native_type_handle& str, corevm::types::native_type_handle& result);

void interface_string_swap(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& str, corevm::types::native_type_handle& result);

void interface_string_substr(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& pos, corevm::types::native_type_handle& result);

void interface_string_substr2(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& pos, corevm::types::native_type_handle& len, corevm::types::native_type_handle& result);

void interface_string_find(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& str, corevm::types::native_type_handle& result);

void interface_string_find2(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& str, corevm::types::native_type_handle& pos, corevm::types::native_type_handle& result);

void interface_string_rfind(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& str, corevm::types::native_type_handle& result);

void interface_string_rfind2(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& str, corevm::types::native_type_handle& pos, corevm::types::native_type_handle& result);


/**************************** ARRAY OPERATIONS *******************************/

void interface_array_size(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result);

void interface_array_empty(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result);

void interface_array_at(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& index, corevm::types::native_type_handle& result);

void interface_array_front(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result);

void interface_array_back(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result);

void interface_array_append(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& data, corevm::types::native_type_handle& result);

void interface_array_pop(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result);

void interface_array_swap(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& other_operand, corevm::types::native_type_handle& result);

void interface_array_clear(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result);


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_TYPES_INTERFACES_H_ */
