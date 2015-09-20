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
#ifndef COREVM_TYPES_INTERFACES_H_
#define COREVM_TYPES_INTERFACES_H_

#include "native_type_handle.h"


namespace corevm {


namespace types {


/* --------------------------- UNARY OPERATIONS ----------------------------- */


void interface_apply_positive_operator(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_apply_negation_operator(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_apply_increment_operator(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_apply_decrement_operator(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_apply_logical_not_operator(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_apply_bitwise_not_operator(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_apply_abs_operator(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_apply_sqrt_operator(native_type_handle& operand);

// -----------------------------------------------------------------------------


/* -------------------------- BINARY OPERATIONS ----------------------------- */


// -----------------------------------------------------------------------------

void interface_apply_addition_operator(
  native_type_handle& lhs, native_type_handle& rhs);

// -----------------------------------------------------------------------------

void interface_apply_subtraction_operator(
  native_type_handle& lhs, native_type_handle& rhs);

// -----------------------------------------------------------------------------

void interface_apply_multiplication_operator(
  native_type_handle& lhs, native_type_handle& rhs);

// -----------------------------------------------------------------------------

void interface_apply_division_operator(
  native_type_handle& lhs, native_type_handle& rhs);

// -----------------------------------------------------------------------------

void interface_apply_modulus_operator(
  native_type_handle& lhs, native_type_handle& rhs);

// -----------------------------------------------------------------------------

void interface_apply_pow_operator(
  native_type_handle& lhs, native_type_handle& rhs);

// -----------------------------------------------------------------------------

void interface_apply_logical_and_operator(
  native_type_handle& lhs, native_type_handle& rhs);

// -----------------------------------------------------------------------------

void interface_apply_logical_or_operator(
  native_type_handle& lhs, native_type_handle& rhs);

// -----------------------------------------------------------------------------

void interface_apply_bitwise_and_operator(
  native_type_handle& lhs, native_type_handle& rhs);

// -----------------------------------------------------------------------------

void interface_apply_bitwise_or_operator(
  native_type_handle& lhs, native_type_handle& rhs);

// -----------------------------------------------------------------------------

void interface_apply_bitwise_xor_operator(
  native_type_handle& lhs, native_type_handle& rhs);

// -----------------------------------------------------------------------------

void interface_apply_bitwise_left_shift_operator(
  native_type_handle& lhs, native_type_handle& rhs);

// -----------------------------------------------------------------------------

void interface_apply_bitwise_right_shift_operator(
  native_type_handle& lhs, native_type_handle& rhs);

// -----------------------------------------------------------------------------

void interface_apply_eq_operator(
  native_type_handle& lhs, native_type_handle& rhs);

// -----------------------------------------------------------------------------

void interface_apply_neq_operator(
  native_type_handle& lhs, native_type_handle& rhs);

// -----------------------------------------------------------------------------

void interface_apply_lt_operator(
  native_type_handle& lhs, native_type_handle& rhs);

// -----------------------------------------------------------------------------

void interface_apply_gt_operator(
  native_type_handle& lhs, native_type_handle& rhs);

// -----------------------------------------------------------------------------

void interface_apply_lte_operator(
  native_type_handle& lhs, native_type_handle& rhs);

// -----------------------------------------------------------------------------

void interface_apply_gte_operator(
  native_type_handle& lhs, native_type_handle& rhs);

// -----------------------------------------------------------------------------


/* -------------------------- CONVERSION OPERATIONS ------------------------- */


// -----------------------------------------------------------------------------

void interface_to_int8(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_to_uint8(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_to_int16(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_to_uint16(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_to_int32(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_to_uint32(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_to_int64(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_to_uint64(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_to_bool(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_to_dec1(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_to_dec2(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_to_str(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_to_ary(native_type_handle& operand);

// -----------------------------------------------------------------------------

void interface_to_map(native_type_handle& operand);

// -----------------------------------------------------------------------------


/* ----------------------- MANIPULATION OPERATION --------------------------- */


// -----------------------------------------------------------------------------

void interface_compute_truthy_value(
  native_type_handle& operand, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_compute_repr_value(
  native_type_handle& operand, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_compute_hash_value(
  native_type_handle& operand, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_compute_slice(
  native_type_handle& operand,
  native_type_handle& start,
  native_type_handle& stop,
  native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_compute_stride(
  native_type_handle& operand,
  native_type_handle& stride,
  native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_compute_reverse(
  native_type_handle& operand, native_type_handle& result);

// -----------------------------------------------------------------------------


/* -------------------------- STRING OPERATIONS ----------------------------- */


// -----------------------------------------------------------------------------

void interface_string_get_size(
  native_type_handle& operand, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_string_clear(
  native_type_handle& operand, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_string_at(
  native_type_handle& operand, native_type_handle& index,
  native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_string_at_2(
  native_type_handle& operand, native_type_handle& index,
  native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_string_append(
  native_type_handle& operand, native_type_handle& str,
  native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_string_pushback(
  native_type_handle& operand, native_type_handle& c,
  native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_string_insert_str(
  native_type_handle& operand, native_type_handle& pos,
  native_type_handle& str, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_string_insert_char(
  native_type_handle& operand, native_type_handle& pos, native_type_handle& c,
  native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_string_erase(
  native_type_handle& operand, native_type_handle& pos, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_string_erase2(
  native_type_handle& operand, native_type_handle& pos, native_type_handle& len, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_string_replace_str(
  native_type_handle& operand, native_type_handle& pos, native_type_handle& len,
  native_type_handle& str, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_string_swap(
  native_type_handle& operand, native_type_handle& str, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_string_substr(
  native_type_handle& operand, native_type_handle& pos, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_string_substr2(
  native_type_handle& operand, native_type_handle& pos,
  native_type_handle& len, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_string_find(
  native_type_handle& operand, native_type_handle& str,
  native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_string_find2(
  native_type_handle& operand, native_type_handle& str,
  native_type_handle& pos, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_string_rfind(
  native_type_handle& operand, native_type_handle& str,
  native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_string_rfind2(
  native_type_handle& operand, native_type_handle& str,
  native_type_handle& pos, native_type_handle& result);

// -----------------------------------------------------------------------------


/* --------------------------- ARRAY OPERATIONS ----------------------------- */


// -----------------------------------------------------------------------------

void interface_array_size(
  native_type_handle& operand, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_array_empty(
  native_type_handle& operand, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_array_at(
  native_type_handle& operand, native_type_handle& index,
  native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_array_front(
  native_type_handle& operand, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_array_back(
  native_type_handle& operand, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_array_put(
  native_type_handle& operand, native_type_handle& index,
  native_type_handle& value, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_array_append(
  native_type_handle& operand, native_type_handle& data,
  native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_array_erase(
  native_type_handle& operand, native_type_handle& index,
  native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_array_pop(
  native_type_handle& operand, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_array_swap(
  native_type_handle& operand, native_type_handle& other_operand,
  native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_array_clear(
  native_type_handle& operand, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_array_merge(
  native_type_handle& operand, native_type_handle& other_operand,
  native_type_handle& result);

// -----------------------------------------------------------------------------


/* ---------------------------- MAP OPERATIONS ------------------------------ */


// -----------------------------------------------------------------------------

void interface_map_size(
  native_type_handle& operand, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_map_empty(
  native_type_handle& operand, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_map_find(
  native_type_handle& operand, native_type_handle& key,
  native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_map_at(
  native_type_handle& operand, native_type_handle& key,
  native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_map_put(
  native_type_handle& operand, native_type_handle& key,
  native_type_handle& data, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_map_erase(
  native_type_handle& operand, native_type_handle& key,
  native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_map_clear(
  native_type_handle& operand, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_map_swap(
  native_type_handle& operand, native_type_handle& other_operand,
  native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_map_keys(
  native_type_handle& operand, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_map_vals(
  native_type_handle& operand, native_type_handle& result);

// -----------------------------------------------------------------------------

void interface_map_merge(
  native_type_handle& operand, native_type_handle& other_operand,
  native_type_handle& result);

// -----------------------------------------------------------------------------

} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_TYPES_INTERFACES_H_ */
