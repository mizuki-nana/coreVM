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
#include "interfaces.h"
#include "native_type_handle.h"


namespace corevm {


namespace types {


// -----------------------------------------------------------------------------


/* ------------------------ UNARY OPERATIONS -------------------------------- */


template<typename native_type_visitor_type>
inline void __interface_apply_unary_operator(native_type_handle& operand)
{
  operand = std::move(apply_unary_visitor<native_type_visitor_type>(operand));
}

// -----------------------------------------------------------------------------

void interface_apply_positive_operator(native_type_handle& operand)
{
  __interface_apply_unary_operator<
    native_type_positive_visitor>(operand);
}

// -----------------------------------------------------------------------------

void interface_apply_negation_operator(native_type_handle& operand)
{
  __interface_apply_unary_operator<
    native_type_negation_visitor>(operand);
}

// -----------------------------------------------------------------------------

void interface_apply_increment_operator(
  native_type_handle& operand)
{
  __interface_apply_unary_operator<
    native_type_increment_visitor>(operand);
}

// -----------------------------------------------------------------------------

void interface_apply_decrement_operator(
  native_type_handle& operand)
{
  __interface_apply_unary_operator<
    native_type_decrement_visitor>(operand);
}

// -----------------------------------------------------------------------------

void interface_apply_logical_not_operator(
  native_type_handle& operand)
{
  __interface_apply_unary_operator<
    native_type_logical_not_visitor>(operand);
}

// -----------------------------------------------------------------------------

void interface_apply_bitwise_not_operator(
  native_type_handle& operand)
{
  __interface_apply_unary_operator<
    native_type_bitwise_not_visitor>(operand);
}

// -----------------------------------------------------------------------------

void interface_apply_abs_operator(
  native_type_handle& operand)
{
  __interface_apply_unary_operator<
    native_type_abs_visitor>(operand);
}

// -----------------------------------------------------------------------------

void interface_apply_sqrt_operator(
  native_type_handle& operand)
{
  __interface_apply_unary_operator<
    native_type_sqrt_visitor>(operand);
}

// -----------------------------------------------------------------------------


/* ------------------------- BINARY OPERATIONS ------------------------------ */


// -----------------------------------------------------------------------------

template<typename native_type_visitor_type>
inline
native_type_handle
__interface_apply_binary_operator(native_type_handle& lhs, native_type_handle& rhs)
{
  return apply_binary_visitor<native_type_visitor_type>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_addition_operator(native_type_handle& lhs,
  native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_addition_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_subtraction_operator(native_type_handle& lhs,
  native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_subtraction_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_multiplication_operator(native_type_handle& lhs,
  native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_multiplication_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_division_operator(native_type_handle& lhs,
  native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_division_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_modulus_operator(native_type_handle& lhs,
  native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_modulus_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_pow_operator(native_type_handle& lhs,
  native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_pow_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_logical_and_operator(native_type_handle& lhs,
  native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_logical_and_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_logical_or_operator(native_type_handle& lhs,
  native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_logical_or_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_bitwise_and_operator(native_type_handle& lhs,
  native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_bitwise_and_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_bitwise_or_operator(native_type_handle& lhs,
  native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_bitwise_or_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_bitwise_xor_operator(native_type_handle& lhs,
  native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_bitwise_xor_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_bitwise_left_shift_operator(
  native_type_handle& lhs, native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_bitwise_left_shift_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_bitwise_right_shift_operator(
  native_type_handle& lhs, native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_bitwise_right_shift_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_eq_operator(native_type_handle& lhs,
  native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_eq_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_neq_operator(native_type_handle& lhs,
  native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_neq_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_lt_operator(native_type_handle& lhs,
  native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_lt_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_gt_operator(native_type_handle& lhs,
  native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_gt_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_lte_operator(native_type_handle& lhs,
  native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_lte_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_gte_operator(native_type_handle& lhs,
  native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_gte_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_apply_cmp_operator(native_type_handle& lhs,
  native_type_handle& rhs)
{
  return __interface_apply_binary_operator<
    native_type_cmp_visitor>(lhs, rhs);
}

// -----------------------------------------------------------------------------


/* -------------------------- CONVERSION OPERATIONS ------------------------- */


// -----------------------------------------------------------------------------

template<typename DstType>
void __interface_apply_conversion(native_type_handle& operand)
{
  DstType original_value =
    get_value_from_handle<DstType>(operand);

  operand = std::move(DstType(original_value));
}

// -----------------------------------------------------------------------------

void interface_to_int8(native_type_handle& operand)
{
  __interface_apply_conversion<int8>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_uint8(
  native_type_handle& operand)
{
  __interface_apply_conversion<uint8>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_int16(
  native_type_handle& operand)
{
  __interface_apply_conversion<int16>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_uint16(native_type_handle& operand)
{
  __interface_apply_conversion<uint16>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_int32(native_type_handle& operand)
{
  __interface_apply_conversion<int32>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_uint32(native_type_handle& operand)
{
  __interface_apply_conversion<uint32>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_int64(native_type_handle& operand)
{
  __interface_apply_conversion<int64>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_uint64(native_type_handle& operand)
{
  __interface_apply_conversion<uint64>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_bool(native_type_handle& operand)
{
  __interface_apply_conversion<boolean>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_dec1(native_type_handle& operand)
{
  __interface_apply_conversion<decimal>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_dec2(native_type_handle& operand)
{
  __interface_apply_conversion<decimal2>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_str(native_type_handle& operand)
{
  __interface_apply_conversion<string>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_ary(native_type_handle& operand)
{
  __interface_apply_conversion<array>(operand);
}

// -----------------------------------------------------------------------------

void interface_to_map(native_type_handle& operand)
{
  __interface_apply_conversion<map>(operand);
}

// -----------------------------------------------------------------------------


/* ----------------------- MANIPULATION OPERATION --------------------------- */

// -----------------------------------------------------------------------------

native_type_handle interface_compute_truthy_value(
  native_type_handle& operand)
{
  return apply_unary_visitor<native_type_truthy_visitor>(operand);
}

// -----------------------------------------------------------------------------

native_type_handle interface_compute_repr_value(
  native_type_handle& operand)
{
  return apply_unary_visitor<native_type_repr_visitor>(operand);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_compute_hash_value(native_type_handle& operand)
{
  return apply_unary_visitor<native_type_hash_visitor>(operand);
}

// -----------------------------------------------------------------------------

native_type_handle interface_compute_slice(
  native_type_handle& operand,
  native_type_handle& start,
  native_type_handle& stop)
{
  uint32_t start_value = get_value_from_handle<uint32_t>(start);
  uint32_t stop_value = get_value_from_handle<uint32_t>(stop);

  return apply_unary_visitor_parameterized<
    native_type_slice_visitor>(operand, start_value, stop_value);
}

// -----------------------------------------------------------------------------

native_type_handle interface_compute_stride(
  native_type_handle& operand, native_type_handle& stride)
{
  int32_t stride_value = get_value_from_handle<int32_t>(stride);

  return apply_unary_visitor_parameterized<
    native_type_stride_visitor>(operand, stride_value);;
}

// -----------------------------------------------------------------------------

native_type_handle
interface_compute_reverse(native_type_handle& operand)
{
  return apply_unary_visitor<
    native_type_reverse_visitor>(operand);
}

// -----------------------------------------------------------------------------

native_type_handle interface_apply_rounding(
  native_type_handle& operand, native_type_handle& ndigits)
{
  const auto operand_val =
    get_value_from_handle<decimal2>(operand);
  const auto ndigits_val =
    get_value_from_handle<uint32>(ndigits);

  decimal2 result_val = 0.0;

  char buff[32] = {0};
  sprintf(buff, "%.*f", ndigits_val, operand_val);

  result_val = atof(buff);

  return native_type_handle(decimal2(result_val));
}

// -----------------------------------------------------------------------------


/* -------------------------- STRING OPERATIONS ----------------------------- */


// -----------------------------------------------------------------------------

native_type_handle
interface_string_get_size(native_type_handle& operand)
{
  const auto& string_value =
    get_value_ref_from_handle<native_string>(operand);

  uint64 result_value = string_value.size();

  return native_type_handle(result_value);
}

// -----------------------------------------------------------------------------

void
interface_string_clear(native_type_handle& operand)
{
  auto& string_value =
    get_value_ref_from_handle<native_string>(operand);

  string_value.clear();
}

// -----------------------------------------------------------------------------

native_type_handle
interface_string_at(native_type_handle& operand,
  native_type_handle& index)
{
  const auto& string_value =
    get_value_ref_from_handle<native_string>(operand);

  const int32_t index_value =
    get_value_from_handle<int32_t>(index);

  char char_value = string_value.at(static_cast<size_t>(index_value));
  int8 result_value = char_value;

  return native_type_handle(result_value);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_string_at_2(native_type_handle& operand,
  native_type_handle& index)
{
  const auto& string_value =
    get_value_ref_from_handle<native_string>(operand);

  const int32_t index_value = get_value_from_handle<int32_t>(index);

  char char_value = string_value.at(static_cast<size_t>(index_value));
  string result_value(1, char_value);

  return native_type_handle(result_value);
}

// -----------------------------------------------------------------------------

void
interface_string_append(native_type_handle& operand,
  native_type_handle& str)
{
  auto& string_value =
    get_value_ref_from_handle<native_string>(operand);

  const auto& other_string_value =
    get_value_ref_from_handle<native_string>(str);

  string_value.append(other_string_value);
}

// -----------------------------------------------------------------------------

void
interface_string_pushback(native_type_handle& operand,
  native_type_handle& c)
{
  auto& string_value =
    get_value_ref_from_handle<native_string>(operand);

  char char_value = get_value_from_handle<char>(c);

  string_value.push_back(char_value);
}

// -----------------------------------------------------------------------------

void
interface_string_insert_str(native_type_handle& operand,
  native_type_handle& pos, native_type_handle& str)
{
  auto& string_value =
    get_value_ref_from_handle<native_string>(operand);

  size_t pos_value = get_value_from_handle<size_t>(pos);

  const auto& other_string_value =
    get_value_ref_from_handle<native_string>(str);

  string_value.insert(pos_value, other_string_value);
}

// -----------------------------------------------------------------------------

void
interface_string_insert_char(native_type_handle& operand,
  native_type_handle& pos, native_type_handle& c)
{
  auto& string_value =
    get_value_ref_from_handle<native_string>(operand);

  size_t pos_value = get_value_from_handle<size_t>(pos);
  char char_value = get_value_from_handle<char>(c);

  string_value.insert(pos_value, 1, char_value);
}

// -----------------------------------------------------------------------------

void
interface_string_erase(native_type_handle& operand,
  native_type_handle& pos)
{
  auto& string_value =
    get_value_ref_from_handle<native_string>(operand);

  size_t pos_value = get_value_from_handle<size_t>(pos);

  string_value.erase(pos_value);
}

// -----------------------------------------------------------------------------

void
interface_string_erase2(native_type_handle& operand,
  native_type_handle& pos, native_type_handle& len)
{
  auto& string_value =
    get_value_ref_from_handle<native_string>(operand);

  size_t pos_value = get_value_from_handle<size_t>(pos);
  size_t len_value = get_value_from_handle<size_t>(len);

  string_value.erase(pos_value, len_value);
}

// -----------------------------------------------------------------------------

void
interface_string_replace_str(native_type_handle& operand,
  native_type_handle& pos, native_type_handle& len, native_type_handle& str)
{
  auto& string_value =
    get_value_ref_from_handle<native_string>(operand);

  size_t pos_value = get_value_from_handle<size_t>(pos);
  size_t len_value = get_value_from_handle<size_t>(len);

  const auto& str_value =
    get_value_ref_from_handle<native_string>(str);

  string_value.replace(pos_value, len_value, str_value);
}

// -----------------------------------------------------------------------------

void
interface_string_swap(native_type_handle& operand,
  native_type_handle& str)
{
  auto& string_value =
    get_value_ref_from_handle<native_string>(operand);

  native_string other_string_value =
    get_value_from_handle<native_string>(str);

  string_value.swap(other_string_value);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_string_substr(native_type_handle& operand,
  native_type_handle& pos)
{
  native_string string_value =
    get_value_from_handle<native_string>(operand);

  size_t pos_value = get_value_from_handle<size_t>(pos);

  native_string result_value = string_value.substr(pos_value);

  return native_type_handle(result_value);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_string_substr2(native_type_handle& operand,
  native_type_handle& pos, native_type_handle& len)
{
  const auto& string_value =
    get_value_ref_from_handle<native_string>(operand);

  size_t pos_value = get_value_from_handle<size_t>(pos);
  size_t len_value = get_value_from_handle<size_t>(len);

  native_string result_value =
    string_value.substr(pos_value, len_value);

  return native_type_handle(result_value);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_string_find(
  native_type_handle& operand, native_type_handle& str)
{
  const auto& string_value =
    get_value_ref_from_handle<native_string>(operand);

  const auto& other_string_value =
    get_value_ref_from_handle<native_string>(str);

  uint64 result_value = string_value.find(other_string_value);

  return native_type_handle(result_value);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_string_find2(native_type_handle& operand,
  native_type_handle& str, native_type_handle& pos)
{
  const auto& string_value =
    get_value_ref_from_handle<native_string>(operand);

  const auto& other_string_value =
    get_value_ref_from_handle<native_string>(str);

  size_t pos_value = get_value_from_handle<size_t>(pos);

  uint64 result_value =
    string_value.find(other_string_value, pos_value);

  return native_type_handle(result_value);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_string_rfind(native_type_handle& operand,
  native_type_handle& str)
{
  const auto& string_value =
    get_value_ref_from_handle<native_string>(operand);

  const auto& other_string_value =
    get_value_ref_from_handle<native_string>(str);

  uint64 result_value = string_value.rfind(other_string_value);

  return native_type_handle(result_value);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_string_rfind2(native_type_handle& operand,
  native_type_handle& str, native_type_handle& pos)
{
  const auto& string_value =
    get_value_ref_from_handle<native_string>(operand);

  const auto& other_string_value =
    get_value_ref_from_handle<native_string>(str);

  size_t pos_value = get_value_from_handle<size_t>(pos);

  uint64 result_value =
    string_value.rfind(other_string_value, pos_value);

  return native_type_handle(result_value);
}

// -----------------------------------------------------------------------------


/* --------------------------- ARRAY OPERATIONS ----------------------------- */


// -----------------------------------------------------------------------------

native_type_handle
interface_array_size(native_type_handle& operand)
{
  const auto& array_value =
    get_value_ref_from_handle<native_array>(operand);

  uint64 result_value = array_value.size();

  return native_type_handle(result_value);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_array_empty(native_type_handle& operand)
{
  const auto& array_value =
    get_value_ref_from_handle<native_array>(operand);

  boolean result_value = array_value.empty();

  return native_type_handle(result_value);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_array_at(native_type_handle& operand,
  native_type_handle& index)
{
  const auto& array_value =
    get_value_ref_from_handle<native_array>(operand);

  size_t index_value = get_value_from_handle<size_t>(index);

  uint64 result_value = array_value.at(index_value);

  return native_type_handle(result_value);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_array_front(native_type_handle& operand)
{
  const auto& array_value =
    get_value_ref_from_handle<native_array>(operand);

  uint64 result_value = array_value.front();

  return native_type_handle(result_value);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_array_back(native_type_handle& operand)
{
  const auto& array_value =
    get_value_ref_from_handle<native_array>(operand);

  uint64 result_value = array_value.back();

  return native_type_handle(result_value);
}

// -----------------------------------------------------------------------------

void
interface_array_put(native_type_handle& operand,
  native_type_handle& index, native_type_handle& value)
{
  auto& array_value =
    get_value_ref_from_handle<native_array>(operand);

  uint64_t index_value =
    get_value_from_handle<uint64_t>(index);

  auto data_value =
    get_value_from_handle<native_array::value_type>(value);

  array_value.at(index_value) = data_value;
}

// -----------------------------------------------------------------------------

void
interface_array_append(native_type_handle& operand,
  native_type_handle& data)
{
  auto& array_value =
    get_value_ref_from_handle<native_array>(operand);

  native_array::value_type data_value =
    get_value_from_handle<native_array::value_type>(data);

  array_value.push_back(data_value);
}

// -----------------------------------------------------------------------------

void
interface_array_erase(native_type_handle& operand,
  native_type_handle& index)
{
  auto& array_value =
    get_value_ref_from_handle<native_array>(operand);

  uint32 index_value =
    get_value_from_handle<uint32>(index);

  array_value.erase(index_value);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_array_pop(native_type_handle& operand)
{
  native_array array_value =
    get_value_from_handle<native_array>(operand);

  array_value.pop_back();

  return native_type_handle(array_value);
}

// -----------------------------------------------------------------------------

void
interface_array_swap(native_type_handle& operand,
  native_type_handle& other_operand)
{
  auto& array_value =
    get_value_ref_from_handle<native_array>(operand);

  auto& other_array_value =
    get_value_ref_from_handle<native_array>(other_operand);

  array_value.swap(other_array_value);
}

// -----------------------------------------------------------------------------

void
interface_array_clear(native_type_handle& operand)
{
  auto& array_value =
    get_value_ref_from_handle<native_array>(operand);

  array_value.clear();
}

// -----------------------------------------------------------------------------

native_type_handle
interface_array_merge(native_type_handle& operand,
  native_type_handle& other_operand)
{
  native_array array_value =
    get_value_from_handle<native_array>(operand);

  const auto& other_array_value =
    get_value_ref_from_handle<native_array>(other_operand);

  array_value.insert(
    array_value.end(), other_array_value.begin(), other_array_value.end());

  return native_type_handle(array_value);
}

// -----------------------------------------------------------------------------


/* --------------------------- MAP OPERATIONS ------------------------------- */


// -----------------------------------------------------------------------------

native_type_handle
interface_map_size(native_type_handle& operand)
{
  const auto& map_value =
    get_value_ref_from_handle<native_map>(operand);

  uint64 result_value = map_value.size();

  return native_type_handle(result_value);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_map_empty(native_type_handle& operand)
{
  const auto& map_value =
    get_value_ref_from_handle<native_map>(operand);

  boolean result_value = map_value.empty();

  return native_type_handle(result_value);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_map_find(native_type_handle& operand,
  native_type_handle& key)
{
  const auto& map_value =
    get_value_ref_from_handle<native_map>(operand);

  const auto key_value =
    get_value_from_handle<native_map::key_type>(key);

  boolean result_value = map_value.find(key_value) != map_value.end();

  return native_type_handle(result_value);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_map_at(native_type_handle& operand,
  native_type_handle& key)
{
  const auto& map_value =
    get_value_ref_from_handle<native_map>(operand);

  const auto key_value =
    get_value_from_handle<native_map::key_type>(key);

  uint64 result_value = map_value.at(key_value);

  return native_type_handle(result_value);
}

// -----------------------------------------------------------------------------

void
interface_map_put(native_type_handle& operand,
  native_type_handle& key, native_type_handle& data)
{
  auto& map_value =
    get_value_ref_from_handle<native_map>(operand);

  const auto key_value =
    get_value_from_handle<native_map::key_type>(key);

  const auto data_value =
    get_value_from_handle<native_map::mapped_type>(data);

  map_value[key_value] = data_value;
}

// -----------------------------------------------------------------------------

void
interface_map_erase(native_type_handle& operand,
  native_type_handle& key)
{
  auto& map_value =
    get_value_ref_from_handle<native_map>(operand);

  const auto key_value =
    get_value_from_handle<native_map::key_type>(key);

  map_value.erase(key_value);
}

// -----------------------------------------------------------------------------

void
interface_map_clear(native_type_handle& operand)
{
  auto& map_value =
    get_value_ref_from_handle<native_map>(operand);

  map_value.clear();
}

// -----------------------------------------------------------------------------

void
interface_map_swap(native_type_handle& operand,
  native_type_handle& other_operand)
{
  auto& map_value =
    get_value_ref_from_handle<native_map>(operand);

  auto& other_map_value =
    get_value_ref_from_handle<native_map>(other_operand);

  map_value.swap(other_map_value);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_map_keys(native_type_handle& operand)
{
  const auto& map_value =
    get_value_ref_from_handle<native_map>(operand);

  native_array array_value;

  for (auto itr = map_value.begin(); itr != map_value.end(); ++itr)
  {
    native_map_key_type key =
      static_cast<native_map_key_type>(itr->first);

    native_array_element_type element =
      static_cast<native_array_element_type>(key);

    array_value.push_back(element);
  }

  return native_type_handle(array_value);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_map_vals(native_type_handle& operand)
{
  const auto& map_value =
    get_value_ref_from_handle<native_map>(operand);

  native_array array_value;

  for (auto itr = map_value.begin(); itr != map_value.end(); ++itr)
  {
    native_map_mapped_type value =
      static_cast<native_map_mapped_type>(itr->second);

    native_array_element_type element =
      static_cast<native_array_element_type>(value);

    array_value.push_back(element);
  }

  return native_type_handle(array_value);
}

// -----------------------------------------------------------------------------

native_type_handle
interface_map_merge(native_type_handle& operand,
  native_type_handle& other_operand)
{
  native_map map_value =
    get_value_from_handle<native_map>(operand);

  const auto& other_map_value =
    get_value_ref_from_handle<native_map>(other_operand);

  map_value.insert(other_map_value.begin(), other_map_value.end());

  return native_type_handle(map_value);
}

// -----------------------------------------------------------------------------


} /* end namespace types */


} /* end namespace corevm */
