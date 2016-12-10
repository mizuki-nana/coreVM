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
#ifndef COREVM_TYPES_INTERFACES_H_
#define COREVM_TYPES_INTERFACES_H_

#include "fwd.h"


namespace corevm {
namespace types {

/* --------------------------- UNARY OPERATIONS ----------------------------- */

void interface_apply_positive_operator(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_apply_negation_operator(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_apply_increment_operator(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_apply_decrement_operator(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_apply_logical_not_operator(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_apply_bitwise_not_operator(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_apply_abs_operator(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_apply_sqrt_operator(NativeTypeValue& operand);

// -----------------------------------------------------------------------------


/* -------------------------- BINARY OPERATIONS ----------------------------- */


// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_addition_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_subtraction_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_multiplication_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_division_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_modulus_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_pow_operator(NativeTypeValue& lhs, NativeTypeValue& rhs);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_logical_and_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_logical_or_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_bitwise_and_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_bitwise_or_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_bitwise_xor_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_bitwise_left_shift_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_bitwise_right_shift_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_eq_operator(NativeTypeValue& lhs, NativeTypeValue& rhs);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_neq_operator(NativeTypeValue& lhs, NativeTypeValue& rhs);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_lt_operator(NativeTypeValue& lhs, NativeTypeValue& rhs);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_gt_operator(NativeTypeValue& lhs, NativeTypeValue& rhs);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_lte_operator(NativeTypeValue& lhs, NativeTypeValue& rhs);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_gte_operator(NativeTypeValue& lhs, NativeTypeValue& rhs);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_cmp_operator(NativeTypeValue& lhs, NativeTypeValue& rhs);

// -----------------------------------------------------------------------------


/* -------------------------- CONVERSION OPERATIONS ------------------------- */


// -----------------------------------------------------------------------------

void interface_to_int8(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_to_uint8(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_to_int16(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_to_uint16(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_to_int32(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_to_uint32(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_to_int64(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_to_uint64(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_to_bool(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_to_dec1(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_to_dec2(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_to_str(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_to_ary(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_to_map(NativeTypeValue& operand);

// -----------------------------------------------------------------------------


/* ----------------------- MANIPULATION OPERATION --------------------------- */


// -----------------------------------------------------------------------------

NativeTypeValue interface_compute_truthy_value(const NativeTypeValue& operand);

// -----------------------------------------------------------------------------

NativeTypeValue interface_compute_repr_value(const NativeTypeValue& operand);

// -----------------------------------------------------------------------------

NativeTypeValue interface_compute_hash_value(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

NativeTypeValue interface_compute_slice(NativeTypeValue& operand,
  NativeTypeValue& start, NativeTypeValue& stop);

// -----------------------------------------------------------------------------

NativeTypeValue interface_compute_stride(NativeTypeValue& operand,
  NativeTypeValue& stride);

// -----------------------------------------------------------------------------

NativeTypeValue interface_compute_reverse(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

NativeTypeValue interface_apply_rounding(NativeTypeValue& operand,
  NativeTypeValue& ndigits);

// -----------------------------------------------------------------------------


/* -------------------------- STRING OPERATIONS ----------------------------- */


// -----------------------------------------------------------------------------

NativeTypeValue interface_string_get_size(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void interface_string_clear(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_string_at(NativeTypeValue& operand, NativeTypeValue& index);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_string_at_2(NativeTypeValue& operand, NativeTypeValue& index);

// -----------------------------------------------------------------------------

void
interface_string_append(NativeTypeValue& operand, NativeTypeValue& str);

// -----------------------------------------------------------------------------

void
interface_string_pushback(NativeTypeValue& operand, NativeTypeValue& c);

// -----------------------------------------------------------------------------

void
interface_string_insert_str(NativeTypeValue& operand,
  NativeTypeValue& pos, NativeTypeValue& str);

// -----------------------------------------------------------------------------

void
interface_string_insert_char(NativeTypeValue& operand,
  NativeTypeValue& pos, NativeTypeValue& c);

// -----------------------------------------------------------------------------

void
interface_string_erase(NativeTypeValue& operand,
  NativeTypeValue& pos);

// -----------------------------------------------------------------------------

void
interface_string_erase2(NativeTypeValue& operand,
  NativeTypeValue& pos, NativeTypeValue& len);

// -----------------------------------------------------------------------------

void
interface_string_replace_str(NativeTypeValue& operand,
  NativeTypeValue& pos, NativeTypeValue& len, NativeTypeValue& str);

// -----------------------------------------------------------------------------

void
interface_string_swap(NativeTypeValue& operand, NativeTypeValue& str);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_string_substr(NativeTypeValue& operand, NativeTypeValue& pos);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_string_substr2(NativeTypeValue& operand, NativeTypeValue& pos,
  NativeTypeValue& len);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_string_find(NativeTypeValue& operand, NativeTypeValue& str);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_string_find2(NativeTypeValue& operand, NativeTypeValue& str,
  NativeTypeValue& pos);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_string_rfind(NativeTypeValue& operand, NativeTypeValue& str);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_string_rfind2(NativeTypeValue& operand, NativeTypeValue& str,
  NativeTypeValue& pos);

// -----------------------------------------------------------------------------


/* --------------------------- ARRAY OPERATIONS ----------------------------- */


// -----------------------------------------------------------------------------

NativeTypeValue
interface_array_size(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_array_empty(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_array_at(NativeTypeValue& operand, NativeTypeValue& index);

// -----------------------------------------------------------------------------

NativeTypeValue interface_array_front(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

NativeTypeValue interface_array_back(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void
interface_array_put(NativeTypeValue& operand, NativeTypeValue& index,
  NativeTypeValue& value);

// -----------------------------------------------------------------------------

void
interface_array_append(NativeTypeValue& operand, NativeTypeValue& data);

// -----------------------------------------------------------------------------

void
interface_array_erase(NativeTypeValue& operand, NativeTypeValue& index);

// -----------------------------------------------------------------------------

NativeTypeValue interface_array_pop(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void
interface_array_swap(NativeTypeValue& operand, NativeTypeValue& other_operand);

// -----------------------------------------------------------------------------

void interface_array_clear(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_array_merge(NativeTypeValue& operand,
  NativeTypeValue& other_operand);

// -----------------------------------------------------------------------------


/* ---------------------------- MAP OPERATIONS ------------------------------ */


// -----------------------------------------------------------------------------

NativeTypeValue interface_map_size(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

NativeTypeValue interface_map_empty(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_map_find(NativeTypeValue& operand, NativeTypeValue& key);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_map_at(NativeTypeValue& operand, NativeTypeValue& key);

// -----------------------------------------------------------------------------

void
interface_map_put(NativeTypeValue& operand, NativeTypeValue& key,
  NativeTypeValue& data);

// -----------------------------------------------------------------------------

void
interface_map_erase(NativeTypeValue& operand, NativeTypeValue& key);

// -----------------------------------------------------------------------------

void interface_map_clear(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

void
interface_map_swap(NativeTypeValue& operand, NativeTypeValue& other_operand);

// -----------------------------------------------------------------------------

NativeTypeValue interface_map_keys(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

NativeTypeValue interface_map_vals(NativeTypeValue& operand);

// -----------------------------------------------------------------------------

NativeTypeValue
interface_map_merge(NativeTypeValue& operand, NativeTypeValue& other_operand);

// -----------------------------------------------------------------------------

} /* end namespace types */
} /* end namespace corevm */


#endif /* COREVM_TYPES_INTERFACES_H_ */
