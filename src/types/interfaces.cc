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
#include "interfaces.h"
#include "native_type_value.h"


namespace corevm {
namespace types {

// -----------------------------------------------------------------------------


/* ------------------------ UNARY OPERATIONS -------------------------------- */


template<typename native_type_visitor_type>
inline void __interface_apply_unary_operator(NativeTypeValue& operand)
{
  operand = std::move(apply_unary_visitor<native_type_visitor_type>(operand));
}

// -----------------------------------------------------------------------------

void interface_apply_positive_operator(NativeTypeValue& operand)
{
  __interface_apply_unary_operator<
    native_type_positive_visitor>(operand);
}

// -----------------------------------------------------------------------------

void interface_apply_negation_operator(NativeTypeValue& operand)
{
  __interface_apply_unary_operator<
    native_type_negation_visitor>(operand);
}

// -----------------------------------------------------------------------------

void interface_apply_increment_operator(
  NativeTypeValue& operand)
{
  __interface_apply_unary_operator<
    native_type_increment_visitor>(operand);
}

// -----------------------------------------------------------------------------

void interface_apply_decrement_operator(
  NativeTypeValue& operand)
{
  __interface_apply_unary_operator<
    native_type_decrement_visitor>(operand);
}

// -----------------------------------------------------------------------------

void interface_apply_logical_not_operator(
  NativeTypeValue& operand)
{
  __interface_apply_unary_operator<
    native_type_logical_not_visitor>(operand);
}

// -----------------------------------------------------------------------------

void interface_apply_bitwise_not_operator(
  NativeTypeValue& operand)
{
  __interface_apply_unary_operator<
    native_type_bitwise_not_visitor>(operand);
}

// -----------------------------------------------------------------------------

void interface_apply_abs_operator(
  NativeTypeValue& operand)
{
  __interface_apply_unary_operator<
    native_type_abs_visitor>(operand);
}

// -----------------------------------------------------------------------------

void interface_apply_sqrt_operator(
  NativeTypeValue& operand)
{
  __interface_apply_unary_operator<
    native_type_sqrt_visitor>(operand);
}

// -----------------------------------------------------------------------------


/* ------------------------- BINARY OPERATIONS ------------------------------ */


// -----------------------------------------------------------------------------

template<typename native_type_visitor_type>
inline
NativeTypeValue
__interface_apply_binary_operator(NativeTypeValue& lhs, NativeTypeValue& rhs)
{
  return apply_binary_visitor<native_type_visitor_type>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_addition_operator(NativeTypeValue& lhs, NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_addition_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_subtraction_operator(NativeTypeValue& lhs, NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_subtraction_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_multiplication_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_multiplication_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_division_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_division_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_modulus_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_modulus_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_pow_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_pow_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_logical_and_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_logical_and_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_logical_or_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_logical_or_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_bitwise_and_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_bitwise_and_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_bitwise_or_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_bitwise_or_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_bitwise_xor_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_bitwise_xor_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_bitwise_left_shift_operator(
  NativeTypeValue& lhs, NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_bitwise_left_shift_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_bitwise_right_shift_operator(NativeTypeValue& lhs,
  NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_bitwise_right_shift_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_eq_operator(NativeTypeValue& lhs, NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_eq_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_neq_operator(NativeTypeValue& lhs, NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_neq_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_lt_operator(NativeTypeValue& lhs, NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_lt_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_gt_operator(NativeTypeValue& lhs, NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_gt_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_lte_operator(NativeTypeValue& lhs, NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_lte_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_gte_operator(NativeTypeValue& lhs, NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_gte_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_apply_cmp_operator(NativeTypeValue& lhs, NativeTypeValue& rhs)
{
  return __interface_apply_binary_operator<
    native_type_cmp_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------


/* -------------------------- CONVERSION OPERATIONS ------------------------- */


// -----------------------------------------------------------------------------

template<typename DstType>
void __interface_apply_conversion(NativeTypeValue& operand)
{
  DstType original_value =
    get_intrinsic_value_from_type_value<DstType>(operand);

  operand = std::move(DstType(original_value));
}

// -----------------------------------------------------------------------------

void interface_to_int8(NativeTypeValue& operand)
{
  __interface_apply_conversion<int8>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_uint8(
  NativeTypeValue& operand)
{
  __interface_apply_conversion<uint8>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_int16(
  NativeTypeValue& operand)
{
  __interface_apply_conversion<int16>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_uint16(NativeTypeValue& operand)
{
  __interface_apply_conversion<uint16>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_int32(NativeTypeValue& operand)
{
  __interface_apply_conversion<int32>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_uint32(NativeTypeValue& operand)
{
  __interface_apply_conversion<uint32>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_int64(NativeTypeValue& operand)
{
  __interface_apply_conversion<int64>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_uint64(NativeTypeValue& operand)
{
  __interface_apply_conversion<uint64>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_bool(NativeTypeValue& operand)
{
  __interface_apply_conversion<boolean>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_dec1(NativeTypeValue& operand)
{
  __interface_apply_conversion<decimal>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_dec2(NativeTypeValue& operand)
{
  __interface_apply_conversion<decimal2>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_str(NativeTypeValue& operand)
{
  __interface_apply_conversion<string>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_ary(NativeTypeValue& operand)
{
  __interface_apply_conversion<array>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_map(NativeTypeValue& operand)
{
  __interface_apply_conversion<map>(operand);
}

// -----------------------------------------------------------------------------


/* ----------------------- MANIPULATION OPERATION --------------------------- */

// -----------------------------------------------------------------------------

NativeTypeValue interface_compute_truthy_value(const NativeTypeValue& operand)
{
  return apply_unary_visitor<native_type_truthy_visitor>(operand);
}

// -----------------------------------------------------------------------------

NativeTypeValue interface_compute_repr_value(const NativeTypeValue& operand)
{
  return apply_unary_visitor<native_type_repr_visitor>(operand);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_compute_hash_value(NativeTypeValue& operand)
{
  return apply_unary_visitor<native_type_hash_visitor>(operand);
}

// -----------------------------------------------------------------------------

NativeTypeValue interface_compute_slice(NativeTypeValue& operand,
  NativeTypeValue& start, NativeTypeValue& stop)
{
  uint32_t start_value = get_intrinsic_value_from_type_value<uint32_t>(start);
  uint32_t stop_value = get_intrinsic_value_from_type_value<uint32_t>(stop);

  return apply_unary_visitor_parameterized<
    native_type_slice_visitor>(operand, start_value, stop_value);
}

// -----------------------------------------------------------------------------

NativeTypeValue interface_compute_stride(NativeTypeValue& operand,
  NativeTypeValue& stride)
{
  int32_t stride_value = get_intrinsic_value_from_type_value<int32_t>(stride);

  return apply_unary_visitor_parameterized<
    native_type_stride_visitor>(operand, stride_value);;
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_compute_reverse(NativeTypeValue& operand)
{
  return apply_unary_visitor<
    native_type_reverse_visitor>(operand);
}

// -----------------------------------------------------------------------------

NativeTypeValue interface_apply_rounding(NativeTypeValue& operand,
  NativeTypeValue& ndigits)
{
  const auto operand_val =
    get_intrinsic_value_from_type_value<decimal2>(operand);
  const auto ndigits_val =
    get_intrinsic_value_from_type_value<uint32>(ndigits);

  decimal2 result_val = 0.0;

  char buff[32] = {0};
  sprintf(buff, "%.*f", ndigits_val, operand_val);

  result_val = atof(buff);

  return NativeTypeValue(decimal2(result_val));
}

// -----------------------------------------------------------------------------


/* -------------------------- STRING OPERATIONS ----------------------------- */


// -----------------------------------------------------------------------------

NativeTypeValue
interface_string_get_size(NativeTypeValue& operand)
{
  const auto& string_value =
    get_value_ref_from_type_value<native_string>(operand);

  uint64 result_value = string_value.size();

  return NativeTypeValue(result_value);
}

// -----------------------------------------------------------------------------

void
interface_string_clear(NativeTypeValue& operand)
{
  auto& string_value =
    get_value_ref_from_type_value<native_string>(operand);

  string_value.clear();
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_string_at(NativeTypeValue& operand, NativeTypeValue& index)
{
  const auto& string_value =
    get_value_ref_from_type_value<native_string>(operand);

  const int32_t index_value =
    get_intrinsic_value_from_type_value<int32_t>(index);

  char char_value = string_value.at(static_cast<size_t>(index_value));
  int8 result_value = char_value;

  return NativeTypeValue(result_value);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_string_at_2(NativeTypeValue& operand, NativeTypeValue& index)
{
  const auto& string_value =
    get_value_ref_from_type_value<native_string>(operand);

  const int32_t index_value = get_intrinsic_value_from_type_value<int32_t>(index);

  char char_value = string_value.at(static_cast<size_t>(index_value));
  string result_value(1, char_value);

  return NativeTypeValue(result_value);
}

// -----------------------------------------------------------------------------

void
interface_string_append(NativeTypeValue& operand, NativeTypeValue& str)
{
  auto& string_value =
    get_value_ref_from_type_value<native_string>(operand);

  const auto& other_string_value =
    get_value_ref_from_type_value<native_string>(str);

  string_value.append(other_string_value);
}

// -----------------------------------------------------------------------------

void
interface_string_pushback(NativeTypeValue& operand, NativeTypeValue& c)
{
  auto& string_value =
    get_value_ref_from_type_value<native_string>(operand);

  char char_value = get_intrinsic_value_from_type_value<char>(c);

  string_value.push_back(char_value);
}

// -----------------------------------------------------------------------------

void
interface_string_insert_str(NativeTypeValue& operand,
  NativeTypeValue& pos, NativeTypeValue& str)
{
  auto& string_value =
    get_value_ref_from_type_value<native_string>(operand);

  size_t pos_value = get_intrinsic_value_from_type_value<size_t>(pos);

  const auto& other_string_value =
    get_value_ref_from_type_value<native_string>(str);

  string_value.insert(pos_value, other_string_value);
}

// -----------------------------------------------------------------------------

void
interface_string_insert_char(NativeTypeValue& operand,
  NativeTypeValue& pos, NativeTypeValue& c)
{
  auto& string_value =
    get_value_ref_from_type_value<native_string>(operand);

  size_t pos_value = get_intrinsic_value_from_type_value<size_t>(pos);
  char char_value = get_intrinsic_value_from_type_value<char>(c);

  string_value.insert(pos_value, 1, char_value);
}

// -----------------------------------------------------------------------------

void
interface_string_erase(NativeTypeValue& operand,
  NativeTypeValue& pos)
{
  auto& string_value =
    get_value_ref_from_type_value<native_string>(operand);

  size_t pos_value = get_intrinsic_value_from_type_value<size_t>(pos);

  string_value.erase(pos_value);
}

// -----------------------------------------------------------------------------

void
interface_string_erase2(NativeTypeValue& operand,
  NativeTypeValue& pos, NativeTypeValue& len)
{
  auto& string_value =
    get_value_ref_from_type_value<native_string>(operand);

  size_t pos_value = get_intrinsic_value_from_type_value<size_t>(pos);
  size_t len_value = get_intrinsic_value_from_type_value<size_t>(len);

  string_value.erase(pos_value, len_value);
}

// -----------------------------------------------------------------------------

void
interface_string_replace_str(NativeTypeValue& operand,
  NativeTypeValue& pos, NativeTypeValue& len, NativeTypeValue& str)
{
  auto& string_value =
    get_value_ref_from_type_value<native_string>(operand);

  size_t pos_value = get_intrinsic_value_from_type_value<size_t>(pos);
  size_t len_value = get_intrinsic_value_from_type_value<size_t>(len);

  const auto& str_value =
    get_value_ref_from_type_value<native_string>(str);

  string_value.replace(pos_value, len_value, str_value);
}

// -----------------------------------------------------------------------------

void
interface_string_swap(NativeTypeValue& operand, NativeTypeValue& str)
{
  auto& string_value =
    get_value_ref_from_type_value<native_string>(operand);

  native_string other_string_value =
    get_intrinsic_value_from_type_value<native_string>(str);

  string_value.swap(other_string_value);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_string_substr(NativeTypeValue& operand, NativeTypeValue& pos)
{
  native_string string_value =
    get_intrinsic_value_from_type_value<native_string>(operand);

  size_t pos_value = get_intrinsic_value_from_type_value<size_t>(pos);

  native_string result_value = string_value.substr(pos_value);

  return NativeTypeValue(result_value);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_string_substr2(NativeTypeValue& operand,
  NativeTypeValue& pos, NativeTypeValue& len)
{
  const auto& string_value =
    get_value_ref_from_type_value<native_string>(operand);

  size_t pos_value = get_intrinsic_value_from_type_value<size_t>(pos);
  size_t len_value = get_intrinsic_value_from_type_value<size_t>(len);

  native_string result_value =
    string_value.substr(pos_value, len_value);

  return NativeTypeValue(result_value);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_string_find(NativeTypeValue& operand, NativeTypeValue& str)
{
  const auto& string_value =
    get_value_ref_from_type_value<native_string>(operand);

  const auto& other_string_value =
    get_value_ref_from_type_value<native_string>(str);

  uint64 result_value = string_value.find(other_string_value);

  return NativeTypeValue(result_value);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_string_find2(NativeTypeValue& operand, NativeTypeValue& str,
  NativeTypeValue& pos)
{
  const auto& string_value =
    get_value_ref_from_type_value<native_string>(operand);

  const auto& other_string_value =
    get_value_ref_from_type_value<native_string>(str);

  size_t pos_value = get_intrinsic_value_from_type_value<size_t>(pos);

  uint64 result_value =
    string_value.find(other_string_value, pos_value);

  return NativeTypeValue(result_value);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_string_rfind(NativeTypeValue& operand, NativeTypeValue& str)
{
  const auto& string_value =
    get_value_ref_from_type_value<native_string>(operand);

  const auto& other_string_value =
    get_value_ref_from_type_value<native_string>(str);

  uint64 result_value = string_value.rfind(other_string_value);

  return NativeTypeValue(result_value);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_string_rfind2(NativeTypeValue& operand, NativeTypeValue& str,
  NativeTypeValue& pos)
{
  const auto& string_value =
    get_value_ref_from_type_value<native_string>(operand);

  const auto& other_string_value =
    get_value_ref_from_type_value<native_string>(str);

  size_t pos_value = get_intrinsic_value_from_type_value<size_t>(pos);

  uint64 result_value =
    string_value.rfind(other_string_value, pos_value);

  return NativeTypeValue(result_value);
}

// -----------------------------------------------------------------------------


/* --------------------------- ARRAY OPERATIONS ----------------------------- */


// -----------------------------------------------------------------------------

NativeTypeValue
interface_array_size(NativeTypeValue& operand)
{
  const auto& array_value =
    get_value_ref_from_type_value<native_array>(operand);

  uint64 result_value = array_value.size();

  return NativeTypeValue(result_value);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_array_empty(NativeTypeValue& operand)
{
  const auto& array_value =
    get_value_ref_from_type_value<native_array>(operand);

  boolean result_value = array_value.empty();

  return NativeTypeValue(result_value);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_array_at(NativeTypeValue& operand, NativeTypeValue& index)
{
  const auto& array_value =
    get_value_ref_from_type_value<native_array>(operand);

  size_t index_value = get_intrinsic_value_from_type_value<size_t>(index);

  uint64 result_value = array_value.at(index_value);

  return NativeTypeValue(result_value);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_array_front(NativeTypeValue& operand)
{
  const auto& array_value =
    get_value_ref_from_type_value<native_array>(operand);

  uint64 result_value = array_value.front();

  return NativeTypeValue(result_value);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_array_back(NativeTypeValue& operand)
{
  const auto& array_value =
    get_value_ref_from_type_value<native_array>(operand);

  uint64 result_value = array_value.back();

  return NativeTypeValue(result_value);
}

// -----------------------------------------------------------------------------

void
interface_array_put(NativeTypeValue& operand, NativeTypeValue& index,
  NativeTypeValue& value)
{
  auto& array_value =
    get_value_ref_from_type_value<native_array>(operand);

  uint64_t index_value =
    get_intrinsic_value_from_type_value<uint64_t>(index);

  auto data_value =
    get_intrinsic_value_from_type_value<native_array::value_type>(value);

  array_value.at(index_value) = data_value;
}

// -----------------------------------------------------------------------------

void
interface_array_append(NativeTypeValue& operand, NativeTypeValue& data)
{
  auto& array_value =
    get_value_ref_from_type_value<native_array>(operand);

  native_array::value_type data_value =
    get_intrinsic_value_from_type_value<native_array::value_type>(data);

  array_value.push_back(data_value);
}

// -----------------------------------------------------------------------------

void
interface_array_erase(NativeTypeValue& operand, NativeTypeValue& index)
{
  auto& array_value =
    get_value_ref_from_type_value<native_array>(operand);

  uint32 index_value =
    get_intrinsic_value_from_type_value<uint32>(index);

  array_value.erase(index_value);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_array_pop(NativeTypeValue& operand)
{
  native_array array_value =
    get_intrinsic_value_from_type_value<native_array>(operand);

  array_value.pop_back();

  return NativeTypeValue(array_value);
}

// -----------------------------------------------------------------------------

void
interface_array_swap(NativeTypeValue& operand, NativeTypeValue& other_operand)
{
  auto& array_value =
    get_value_ref_from_type_value<native_array>(operand);

  auto& other_array_value =
    get_value_ref_from_type_value<native_array>(other_operand);

  array_value.swap(other_array_value);
}

// -----------------------------------------------------------------------------

void
interface_array_clear(NativeTypeValue& operand)
{
  auto& array_value =
    get_value_ref_from_type_value<native_array>(operand);

  array_value.clear();
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_array_merge(NativeTypeValue& operand, NativeTypeValue& other_operand)
{
  native_array array_value =
    get_intrinsic_value_from_type_value<native_array>(operand);

  const auto& other_array_value =
    get_value_ref_from_type_value<native_array>(other_operand);

  array_value.insert(
    array_value.end(), other_array_value.begin(), other_array_value.end());

  return NativeTypeValue(array_value);
}

// -----------------------------------------------------------------------------


/* --------------------------- MAP OPERATIONS ------------------------------- */


// -----------------------------------------------------------------------------

NativeTypeValue
interface_map_size(NativeTypeValue& operand)
{
  const auto& map_value =
    get_value_ref_from_type_value<native_map>(operand);

  uint64 result_value = map_value.size();

  return NativeTypeValue(result_value);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_map_empty(NativeTypeValue& operand)
{
  const auto& map_value =
    get_value_ref_from_type_value<native_map>(operand);

  boolean result_value = map_value.empty();

  return NativeTypeValue(result_value);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_map_find(NativeTypeValue& operand, NativeTypeValue& key)
{
  const auto& map_value =
    get_value_ref_from_type_value<native_map>(operand);

  const auto key_value =
    get_intrinsic_value_from_type_value<native_map::key_type>(key);

  boolean result_value = map_value.find(key_value) != map_value.end();

  return NativeTypeValue(result_value);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_map_at(NativeTypeValue& operand, NativeTypeValue& key)
{
  const auto& map_value =
    get_value_ref_from_type_value<native_map>(operand);

  const auto key_value =
    get_intrinsic_value_from_type_value<native_map::key_type>(key);

  uint64 result_value = map_value.at(key_value);

  return NativeTypeValue(result_value);
}

// -----------------------------------------------------------------------------

void
interface_map_put(NativeTypeValue& operand, NativeTypeValue& key,
  NativeTypeValue& data)
{
  auto& map_value =
    get_value_ref_from_type_value<native_map>(operand);

  const auto key_value =
    get_intrinsic_value_from_type_value<native_map::key_type>(key);

  const auto data_value =
    get_intrinsic_value_from_type_value<native_map::mapped_type>(data);

  map_value[key_value] = data_value;
}

// -----------------------------------------------------------------------------

void
interface_map_erase(NativeTypeValue& operand, NativeTypeValue& key)
{
  auto& map_value =
    get_value_ref_from_type_value<native_map>(operand);

  const auto key_value =
    get_intrinsic_value_from_type_value<native_map::key_type>(key);

  map_value.erase(key_value);
}

// -----------------------------------------------------------------------------

void
interface_map_clear(NativeTypeValue& operand)
{
  auto& map_value =
    get_value_ref_from_type_value<native_map>(operand);

  map_value.clear();
}

// -----------------------------------------------------------------------------

void
interface_map_swap(NativeTypeValue& operand, NativeTypeValue& other_operand)
{
  auto& map_value =
    get_value_ref_from_type_value<native_map>(operand);

  auto& other_map_value =
    get_value_ref_from_type_value<native_map>(other_operand);

  map_value.swap(other_map_value);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_map_keys(NativeTypeValue& operand)
{
  const auto& map_value =
    get_value_ref_from_type_value<native_map>(operand);

  native_array array_value;

  for (auto itr = map_value.begin(); itr != map_value.end(); ++itr)
  {
    native_map_key_type key =
      static_cast<native_map_key_type>(itr->first);

    native_array_element_type element =
      static_cast<native_array_element_type>(key);

    array_value.push_back(element);
  }

  return NativeTypeValue(array_value);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_map_vals(NativeTypeValue& operand)
{
  const auto& map_value =
    get_value_ref_from_type_value<native_map>(operand);

  native_array array_value;

  for (auto itr = map_value.begin(); itr != map_value.end(); ++itr)
  {
    native_map_mapped_type value =
      static_cast<native_map_mapped_type>(itr->second);

    native_array_element_type element =
      static_cast<native_array_element_type>(value);

    array_value.push_back(element);
  }

  return NativeTypeValue(array_value);
}

// -----------------------------------------------------------------------------

NativeTypeValue
interface_map_merge(NativeTypeValue& operand, NativeTypeValue& other_operand)
{
  native_map map_value =
    get_intrinsic_value_from_type_value<native_map>(operand);

  const auto& other_map_value =
    get_value_ref_from_type_value<native_map>(other_operand);

  map_value.insert(other_map_value.begin(), other_map_value.end());

  return NativeTypeValue(map_value);
}

// -----------------------------------------------------------------------------

} /* end namespace types */
} /* end namespace corevm */
