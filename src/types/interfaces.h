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

#include "fwd.h"


namespace corevm {


namespace types {


/* --------------------------- UNARY OPERATIONS ----------------------------- */

void interface_apply_positive_operator(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_apply_negation_operator(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_apply_increment_operator(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_apply_decrement_operator(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_apply_logical_not_operator(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_apply_bitwise_not_operator(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_apply_abs_operator(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_apply_sqrt_operator(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------


/* -------------------------- BINARY OPERATIONS ----------------------------- */


// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_addition_operator(NativeTypeHandle& lhs,
  NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_subtraction_operator(NativeTypeHandle& lhs,
  NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_multiplication_operator(NativeTypeHandle& lhs,
  NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_division_operator(NativeTypeHandle& lhs,
  NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_modulus_operator(NativeTypeHandle& lhs,
  NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_pow_operator(NativeTypeHandle& lhs, NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_logical_and_operator(NativeTypeHandle& lhs,
  NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_logical_or_operator(NativeTypeHandle& lhs,
  NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_bitwise_and_operator(NativeTypeHandle& lhs,
  NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_bitwise_or_operator(NativeTypeHandle& lhs,
  NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_bitwise_xor_operator(NativeTypeHandle& lhs,
  NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_bitwise_left_shift_operator(NativeTypeHandle& lhs,
  NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_bitwise_right_shift_operator(NativeTypeHandle& lhs,
  NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_eq_operator(NativeTypeHandle& lhs, NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_neq_operator(NativeTypeHandle& lhs, NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_lt_operator(NativeTypeHandle& lhs, NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_gt_operator(NativeTypeHandle& lhs, NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_lte_operator(NativeTypeHandle& lhs, NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_gte_operator(NativeTypeHandle& lhs, NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_apply_cmp_operator(NativeTypeHandle& lhs, NativeTypeHandle& rhs);

// -----------------------------------------------------------------------------


/* -------------------------- CONVERSION OPERATIONS ------------------------- */


// -----------------------------------------------------------------------------

void interface_to_int8(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_to_uint8(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_to_int16(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_to_uint16(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_to_int32(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_to_uint32(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_to_int64(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_to_uint64(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_to_bool(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_to_dec1(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_to_dec2(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_to_str(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_to_ary(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_to_map(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------


/* ----------------------- MANIPULATION OPERATION --------------------------- */


// -----------------------------------------------------------------------------

NativeTypeHandle interface_compute_truthy_value(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

NativeTypeHandle interface_compute_repr_value(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

NativeTypeHandle interface_compute_hash_value(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

NativeTypeHandle interface_compute_slice(NativeTypeHandle& operand,
  NativeTypeHandle& start, NativeTypeHandle& stop);

// -----------------------------------------------------------------------------

NativeTypeHandle interface_compute_stride(NativeTypeHandle& operand,
  NativeTypeHandle& stride);

// -----------------------------------------------------------------------------

NativeTypeHandle interface_compute_reverse(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

NativeTypeHandle interface_apply_rounding(NativeTypeHandle& operand,
  NativeTypeHandle& ndigits);

// -----------------------------------------------------------------------------


/* -------------------------- STRING OPERATIONS ----------------------------- */


// -----------------------------------------------------------------------------

NativeTypeHandle interface_string_get_size(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void interface_string_clear(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_string_at(NativeTypeHandle& operand, NativeTypeHandle& index);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_string_at_2(NativeTypeHandle& operand, NativeTypeHandle& index);

// -----------------------------------------------------------------------------

void
interface_string_append(NativeTypeHandle& operand, NativeTypeHandle& str);

// -----------------------------------------------------------------------------

void
interface_string_pushback(NativeTypeHandle& operand, NativeTypeHandle& c);

// -----------------------------------------------------------------------------

void
interface_string_insert_str(NativeTypeHandle& operand,
  NativeTypeHandle& pos, NativeTypeHandle& str);

// -----------------------------------------------------------------------------

void
interface_string_insert_char(NativeTypeHandle& operand,
  NativeTypeHandle& pos, NativeTypeHandle& c);

// -----------------------------------------------------------------------------

void
interface_string_erase(NativeTypeHandle& operand,
  NativeTypeHandle& pos);

// -----------------------------------------------------------------------------

void
interface_string_erase2(NativeTypeHandle& operand,
  NativeTypeHandle& pos, NativeTypeHandle& len);

// -----------------------------------------------------------------------------

void
interface_string_replace_str(NativeTypeHandle& operand,
  NativeTypeHandle& pos, NativeTypeHandle& len, NativeTypeHandle& str);

// -----------------------------------------------------------------------------

void
interface_string_swap(NativeTypeHandle& operand, NativeTypeHandle& str);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_string_substr(NativeTypeHandle& operand, NativeTypeHandle& pos);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_string_substr2(NativeTypeHandle& operand, NativeTypeHandle& pos,
  NativeTypeHandle& len);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_string_find(NativeTypeHandle& operand, NativeTypeHandle& str);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_string_find2(NativeTypeHandle& operand, NativeTypeHandle& str,
  NativeTypeHandle& pos);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_string_rfind(NativeTypeHandle& operand, NativeTypeHandle& str);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_string_rfind2(NativeTypeHandle& operand, NativeTypeHandle& str,
  NativeTypeHandle& pos);

// -----------------------------------------------------------------------------


/* --------------------------- ARRAY OPERATIONS ----------------------------- */


// -----------------------------------------------------------------------------

NativeTypeHandle
interface_array_size(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_array_empty(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_array_at(NativeTypeHandle& operand, NativeTypeHandle& index);

// -----------------------------------------------------------------------------

NativeTypeHandle interface_array_front(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

NativeTypeHandle interface_array_back(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void
interface_array_put(NativeTypeHandle& operand, NativeTypeHandle& index,
  NativeTypeHandle& value);

// -----------------------------------------------------------------------------

void
interface_array_append(NativeTypeHandle& operand, NativeTypeHandle& data);

// -----------------------------------------------------------------------------

void
interface_array_erase(NativeTypeHandle& operand, NativeTypeHandle& index);

// -----------------------------------------------------------------------------

NativeTypeHandle interface_array_pop(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void
interface_array_swap(NativeTypeHandle& operand, NativeTypeHandle& other_operand);

// -----------------------------------------------------------------------------

void interface_array_clear(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_array_merge(NativeTypeHandle& operand,
  NativeTypeHandle& other_operand);

// -----------------------------------------------------------------------------


/* ---------------------------- MAP OPERATIONS ------------------------------ */


// -----------------------------------------------------------------------------

NativeTypeHandle interface_map_size(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

NativeTypeHandle interface_map_empty(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_map_find(NativeTypeHandle& operand, NativeTypeHandle& key);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_map_at(NativeTypeHandle& operand, NativeTypeHandle& key);

// -----------------------------------------------------------------------------

void
interface_map_put(NativeTypeHandle& operand, NativeTypeHandle& key,
  NativeTypeHandle& data);

// -----------------------------------------------------------------------------

void
interface_map_erase(NativeTypeHandle& operand, NativeTypeHandle& key);

// -----------------------------------------------------------------------------

void interface_map_clear(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

void
interface_map_swap(NativeTypeHandle& operand, NativeTypeHandle& other_operand);

// -----------------------------------------------------------------------------

NativeTypeHandle interface_map_keys(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

NativeTypeHandle interface_map_vals(NativeTypeHandle& operand);

// -----------------------------------------------------------------------------

NativeTypeHandle
interface_map_merge(NativeTypeHandle& operand, NativeTypeHandle& other_operand);

// -----------------------------------------------------------------------------

} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_TYPES_INTERFACES_H_ */
