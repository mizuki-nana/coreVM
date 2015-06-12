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

#include <boost/type_traits.hpp>


// -----------------------------------------------------------------------------

/**
 * Static assertions to check if all the native types are
 * nothrow default-constructible. This is to enable the underlying optimizations
 * in `boost::variant`'s implementation to avoid the performance penalty of
 * achieving the "never-empty guarantee".
 *
 * "If any bounded type is nothrow default-constructible (as indicated by
 * boost::has_nothrow_constructor), the library guarantees variant will use
 * only single storage and in-place construction for every bounded type in the
 * variant. Note, however, that in the event of assignment failure, an
 * unspecified nothrow default-constructible bounded type will be
 * default-constructed in the left-hand side operand so as to preserve the
 * never-empty guarantee".
 *
 * Please refer to the following link for more details:
 * http://www.boost.org/doc/libs/1_58_0/doc/html/variant/design.html#variant.design.never-empty.optimizations
 */
static_assert(boost::has_nothrow_constructor<corevm::types::int8>::value, "types::int8 does not statisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::uint8>::value, "types::uint8 does not statisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::int16>::value, "types::int16 does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::uint16>::value, "types::uint16 does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::int32>::value, "types::int32 does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::uint32>::value, "types::uint32 does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::int64>::value, "types::int64 does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::uint64>::value, "types::uint64 does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::boolean>::value, "types::boolean does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::decimal>::value, "types::decimal does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::decimal2>::value, "types::decimal2 does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::string>::value, "types::string does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::array>::value, "types::array does not satisfy nothrow default-constructible requirement");
static_assert(boost::has_nothrow_constructor<corevm::types::map>::value, "types::map does not satisfy nothrow default-constructible requirement");

// -----------------------------------------------------------------------------

/**
 * Static assertions to check if all the native types are
 * nothrow copy-constructible. This is to enable the underlying optimizations
 * in `boost::variant`'s implementation to avoid the performance penalty of
 * achieving the "never-empty guarantee".
 *
 * "For each bounded type T that is nothrow copy-constructible
 * (as indicated by boost::has_nothrow_copy), the library guarantees variant
 * will use only single storage and in-place construction for T".
 *
 * Please refer to the following link for more details:
 * http://www.boost.org/doc/libs/1_58_0/doc/html/variant/design.html#variant.design.never-empty.optimizations
 *
 * NOTE: Currently the native string, array and map types are not nothrow
 * copy-constructible.
 */
static_assert(boost::has_nothrow_copy<corevm::types::int8>::value, "types::int8 does not statisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::uint8>::value, "types::uint8 does not statisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::int16>::value, "types::int16 does not satisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::uint16>::value, "types::uint16 does not satisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::int32>::value, "types::int32 does not satisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::uint32>::value, "types::uint32 does not satisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::int64>::value, "types::int64 does not satisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::uint64>::value, "types::uint64 does not satisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::boolean>::value, "types::boolean does not satisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::decimal>::value, "types::decimal does not satisfy nothrow copy-constructible requirement");
static_assert(boost::has_nothrow_copy<corevm::types::decimal2>::value, "types::decimal2 does not satisfy nothrow copy-constructible requirement");
//static_assert(boost::has_nothrow_copy<corevm::types::string>::value, "types::string does not satisfy nothrow copy-constructible requirement");
//static_assert(boost::has_nothrow_copy<corevm::types::array>::value, "types::array does not satisfy nothrow copy-constructible requirement");
//static_assert(boost::has_nothrow_copy<corevm::types::map>::value, "types::map does not satisfy nothrow copy-constructible requirement");

// -----------------------------------------------------------------------------

using corevm::types::native_type_handle;


/* ------------------------ UNARY OPERATIONS -------------------------------- */


template<typename native_type_visitor_type>
inline void __interface_apply_unary_operator(
  native_type_handle& operand, native_type_handle& result)
{
  result = corevm::types::apply_unary_visitor<native_type_visitor_type>(operand);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_positive_operator(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_unary_operator<
    corevm::types::native_type_positive_visitor>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_negation_operator(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_unary_operator<
    corevm::types::native_type_negation_visitor>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_increment_operator(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_unary_operator<
    corevm::types::native_type_increment_visitor>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_decrement_operator(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_unary_operator<
    corevm::types::native_type_decrement_visitor>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_logical_not_operator(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_unary_operator<
    corevm::types::native_type_logical_not_visitor>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_bitwise_not_operator(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_unary_operator<
    corevm::types::native_type_bitwise_not_visitor>(operand, result);
}

// -----------------------------------------------------------------------------


/* ------------------------- BINARY OPERATIONS ------------------------------ */


// -----------------------------------------------------------------------------

template<typename native_type_visitor_type>
inline void __interface_apply_binary_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  result = corevm::types::apply_binary_visitor<native_type_visitor_type>(lhs, rhs);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_addition_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  __interface_apply_binary_operator<
    corevm::types::native_type_addition_visitor>(lhs, rhs, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_subtraction_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  __interface_apply_binary_operator<
    corevm::types::native_type_subtraction_visitor>(lhs, rhs, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_multiplication_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  __interface_apply_binary_operator<
    corevm::types::native_type_multiplication_visitor>(lhs, rhs, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_division_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  __interface_apply_binary_operator<
    corevm::types::native_type_division_visitor>(lhs, rhs, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_modulus_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  __interface_apply_binary_operator<
    corevm::types::native_type_modulus_visitor>(lhs, rhs, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_pow_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  __interface_apply_binary_operator<
    corevm::types::native_type_pow_visitor>(lhs, rhs, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_logical_and_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  __interface_apply_binary_operator<
    corevm::types::native_type_logical_and_visitor>(lhs, rhs, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_logical_or_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  __interface_apply_binary_operator<
    corevm::types::native_type_logical_or_visitor>(lhs, rhs, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_bitwise_and_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  __interface_apply_binary_operator<
    corevm::types::native_type_bitwise_and_visitor>(lhs, rhs, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_bitwise_or_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  __interface_apply_binary_operator<
    corevm::types::native_type_bitwise_or_visitor>(lhs, rhs, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_bitwise_xor_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  __interface_apply_binary_operator<
    corevm::types::native_type_bitwise_xor_visitor>(lhs, rhs, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_bitwise_left_shift_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  __interface_apply_binary_operator<
    corevm::types::native_type_bitwise_left_shift_visitor>(lhs, rhs, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_bitwise_right_shift_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  __interface_apply_binary_operator<
    corevm::types::native_type_bitwise_right_shift_visitor>(lhs, rhs, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_eq_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  __interface_apply_binary_operator<
    corevm::types::native_type_eq_visitor>(lhs, rhs, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_neq_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  __interface_apply_binary_operator<
    corevm::types::native_type_neq_visitor>(lhs, rhs, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_lt_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  __interface_apply_binary_operator<
    corevm::types::native_type_lt_visitor>(lhs, rhs, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_gt_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  __interface_apply_binary_operator<
    corevm::types::native_type_gt_visitor>(lhs, rhs, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_apply_lte_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  __interface_apply_binary_operator<
    corevm::types::native_type_lte_visitor>(lhs, rhs, result);
}

void corevm::types::interface_apply_gte_operator(
  native_type_handle& lhs, native_type_handle& rhs, native_type_handle& result)
{
  __interface_apply_binary_operator<
    corevm::types::native_type_gte_visitor>(lhs, rhs, result);
}

// -----------------------------------------------------------------------------


/* -------------------------- CONVERSION OPERATIONS ------------------------- */


// -----------------------------------------------------------------------------

template<typename DstType>
void __interface_apply_conversion(
  native_type_handle& operand, native_type_handle& result)
{
  typename DstType::value_type original_value =
    corevm::types::get_value_from_handle<typename DstType::value_type>(operand);

  operand = DstType(original_value);
  result = operand;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_to_int8(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::int8>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_to_uint8(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::uint8>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_to_int16(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::int16>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_to_uint16(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::uint16>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_to_int32(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::int32>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_to_uint32(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::uint32>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_to_int64(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::int64>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_to_uint64(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::uint64>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_to_bool(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::boolean>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_to_dec1(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::decimal>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_to_dec2(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::decimal2>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_to_str(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::string>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_to_ary(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::array>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_to_map(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::map>(operand, result);
}

// -----------------------------------------------------------------------------


/* ----------------------- MANIPULATION OPERATION --------------------------- */

// -----------------------------------------------------------------------------

void corevm::types::interface_compute_truthy_value(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_unary_operator<native_type_truthy_visitor>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_compute_repr_value(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_unary_operator<native_type_repr_visitor>(operand, result);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_compute_hash_value(
  native_type_handle& operand, native_type_handle& result)
{
  __interface_apply_unary_operator<native_type_hash_visitor>(operand, result);
}

// -----------------------------------------------------------------------------


/* -------------------------- STRING OPERATIONS ----------------------------- */


// -----------------------------------------------------------------------------

void corevm::types::interface_string_get_size(
  native_type_handle& operand, native_type_handle& result)
{
  corevm::types::native_string string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(operand);

  corevm::types::int32 size = string_value.size();
  result = size;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_string_clear(
  native_type_handle& operand, native_type_handle& result)
{
  corevm::types::native_string string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(operand);

  string_value.clear();
  operand = string_value;
  result = string_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_string_at(
  native_type_handle& operand, native_type_handle& index,
  native_type_handle& result)
{
  corevm::types::native_string string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(operand);
  int32_t index_value = corevm::types::get_value_from_handle<int32_t>(index);

  char char_value = string_value.at(index_value);
  corevm::types::uint32 result_value = char_value;
  result = result_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_string_append(
  native_type_handle& operand, native_type_handle& str,
  native_type_handle& result)
{
  corevm::types::native_string string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(operand);

  corevm::types::native_string other_string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(str);

  string_value.append(other_string_value);
  result = string_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_string_pushback(
  native_type_handle& operand, native_type_handle& c,
  native_type_handle& result)
{
  corevm::types::native_string string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(operand);

  char char_value = corevm::types::get_value_from_handle<char>(c);

  string_value.push_back(char_value);
  result = string_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_string_insert_str(
  native_type_handle& operand,
  native_type_handle& pos,
  native_type_handle& str,
  native_type_handle& result)
{
  corevm::types::native_string string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(operand);

  size_t pos_value = corevm::types::get_value_from_handle<size_t>(pos);

  corevm::types::native_string other_string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(str);

  corevm::types::native_string result_value =
    string_value.insert(pos_value, other_string_value);

  result = result_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_string_insert_char(
  native_type_handle& operand,
  native_type_handle& pos,
  native_type_handle& c,
  native_type_handle& result)
{
  corevm::types::native_string string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(operand);

  size_t pos_value = corevm::types::get_value_from_handle<size_t>(pos);
  char char_value = corevm::types::get_value_from_handle<char>(c);

  corevm::types::native_string result_value =
    string_value.insert(pos_value, 1, char_value);

  result = result_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_string_erase(
  native_type_handle& operand, native_type_handle& pos,
  native_type_handle& result)
{
  corevm::types::native_string string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(operand);

  size_t pos_value = corevm::types::get_value_from_handle<size_t>(pos);

  corevm::types::native_string result_value = string_value.erase(pos_value);

  result = result_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_string_erase2(
  native_type_handle& operand,
  native_type_handle& pos,
  native_type_handle& len,
  native_type_handle& result)
{
  corevm::types::native_string string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(operand);

  size_t pos_value = corevm::types::get_value_from_handle<size_t>(pos);
  size_t len_value = corevm::types::get_value_from_handle<size_t>(len);

  corevm::types::native_string result_value =
    string_value.erase(pos_value, len_value);

  result = result_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_string_replace_str(
  native_type_handle& operand,
  native_type_handle& pos,
  native_type_handle& len,
  native_type_handle& str,
  native_type_handle& result)
{
  corevm::types::native_string string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(operand);

  size_t pos_value = corevm::types::get_value_from_handle<size_t>(pos);
  size_t len_value = corevm::types::get_value_from_handle<size_t>(len);

  corevm::types::native_string str_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(str);

  corevm::types::native_string result_value =
    string_value.replace(pos_value, len_value, str_value);

  result = result_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_string_swap(
  native_type_handle& operand, native_type_handle& str,
  native_type_handle& result)
{
  corevm::types::native_string string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(operand);

  corevm::types::native_string other_string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(str);

  string_value.swap(other_string_value);
  result = string_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_string_substr(
  native_type_handle& operand, native_type_handle& pos,
  native_type_handle& result)
{
  corevm::types::native_string string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(operand);

  size_t pos_value = corevm::types::get_value_from_handle<size_t>(pos);

  corevm::types::native_string result_value = string_value.substr(pos_value);
  result = result_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_string_substr2(
  native_type_handle& operand,
  native_type_handle& pos,
  native_type_handle& len,
  native_type_handle& result)
{
  corevm::types::native_string string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(operand);

  size_t pos_value = corevm::types::get_value_from_handle<size_t>(pos);
  size_t len_value = corevm::types::get_value_from_handle<size_t>(len);

  corevm::types::native_string result_value =
    string_value.substr(pos_value, len_value);

  result = result_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_string_find(
  native_type_handle& operand, native_type_handle& str,
  native_type_handle& result)
{
  corevm::types::native_string string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(operand);

  corevm::types::native_string other_string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(str);

  corevm::types::uint32 result_value = string_value.find(other_string_value);
  result = result_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_string_find2(
  native_type_handle& operand,
  native_type_handle& str,
  native_type_handle& pos,
  native_type_handle& result)
{
  corevm::types::native_string string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(operand);

  corevm::types::native_string other_string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(str);

  size_t pos_value = corevm::types::get_value_from_handle<size_t>(pos);

  corevm::types::uint32 result_value =
    string_value.find(other_string_value, pos_value);

  result = result_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_string_rfind(
  native_type_handle& operand, native_type_handle& str,
  native_type_handle& result)
{
  corevm::types::native_string string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(operand);

  corevm::types::native_string other_string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(str);

  corevm::types::uint32 result_value = string_value.rfind(other_string_value);
  result = result_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_string_rfind2(
  native_type_handle& operand,
  native_type_handle& str,
  native_type_handle& pos,
  native_type_handle& result)
{
  corevm::types::native_string string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(operand);

  corevm::types::native_string other_string_value =
    corevm::types::get_value_from_handle<corevm::types::native_string>(str);

  size_t pos_value = corevm::types::get_value_from_handle<size_t>(pos);

  corevm::types::uint32 result_value =
    string_value.rfind(other_string_value, pos_value);

  result = result_value;
}

// -----------------------------------------------------------------------------


/* --------------------------- ARRAY OPERATIONS ----------------------------- */


// -----------------------------------------------------------------------------

void corevm::types::interface_array_size(
  native_type_handle& operand, native_type_handle& result)
{
  corevm::types::native_array array_value =
    corevm::types::get_value_from_handle<corevm::types::native_array>(operand);

  corevm::types::int32 size = array_value.size();
  result = size;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_array_empty(
  native_type_handle& operand, native_type_handle& result)
{
  corevm::types::native_array array_value =
    corevm::types::get_value_from_handle<corevm::types::native_array>(operand);

  corevm::types::boolean empty = array_value.empty();
  result = empty;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_array_at(
  native_type_handle& operand, native_type_handle& index,
  native_type_handle& result)
{
  corevm::types::native_array array_value =
    corevm::types::get_value_from_handle<corevm::types::native_array>(operand);

  size_t index_value = corevm::types::get_value_from_handle<size_t>(index);

  corevm::types::uint64 result_value = array_value.at(index_value);
  result = result_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_array_front(
  native_type_handle& operand, native_type_handle& result)
{
  corevm::types::native_array array_value =
    corevm::types::get_value_from_handle<corevm::types::native_array>(operand);

  corevm::types::uint64 result_value = array_value.front();
  result = result_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_array_back(
  native_type_handle& operand, native_type_handle& result)
{
  corevm::types::native_array array_value =
    corevm::types::get_value_from_handle<corevm::types::native_array>(operand);

  corevm::types::uint64 result_value = array_value.back();
  result = result_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_array_put(
  native_type_handle& operand, native_type_handle& index,
  native_type_handle& value, native_type_handle& result)
{
  corevm::types::native_array array_value =
    corevm::types::get_value_from_handle<corevm::types::native_array>(operand);

  uint64_t index_value =
    corevm::types::get_value_from_handle<uint64_t>(index);

  corevm::types::native_array::value_type data_value =
    corevm::types::get_value_from_handle<corevm::types::native_array::value_type>(value);

  array_value.at(index_value) = data_value;

  result = array_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_array_append(
  native_type_handle& operand, native_type_handle& data,
  native_type_handle& result)
{
  corevm::types::native_array array_value =
    corevm::types::get_value_from_handle<corevm::types::native_array>(operand);

  corevm::types::native_array::value_type data_value =
    corevm::types::get_value_from_handle<corevm::types::native_array::value_type>(data);

  array_value.push_back(data_value);
  result = array_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_array_pop(
  native_type_handle& operand, native_type_handle& result)
{
  corevm::types::native_array array_value =
    corevm::types::get_value_from_handle<corevm::types::native_array>(operand);

  array_value.pop_back();
  result = array_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_array_swap(
  native_type_handle& operand, native_type_handle& other_operand,
  native_type_handle& result)
{
  corevm::types::native_array array_value =
    corevm::types::get_value_from_handle<corevm::types::native_array>(operand);

  corevm::types::native_array other_array_value =
    corevm::types::get_value_from_handle<corevm::types::native_array>(other_operand);

  array_value.swap(other_array_value);
  result = array_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_array_clear(
  native_type_handle& operand, native_type_handle& result)
{
  corevm::types::native_array array_value =
    corevm::types::get_value_from_handle<corevm::types::native_array>(operand);

  array_value.clear();
  result = array_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_array_merge(
  native_type_handle& operand, native_type_handle& other_operand,
  native_type_handle& result)
{
  corevm::types::native_array array_value =
    corevm::types::get_value_from_handle<corevm::types::native_array>(operand);

  corevm::types::native_array other_array_value =
    corevm::types::get_value_from_handle<corevm::types::native_array>(other_operand);

  array_value.insert(
    array_value.end(), other_array_value.begin(), other_array_value.end());

  result = array_value;
}

// -----------------------------------------------------------------------------


/* --------------------------- MAP OPERATIONS ------------------------------- */


// -----------------------------------------------------------------------------

void corevm::types::interface_map_size(
  native_type_handle& operand, native_type_handle& result)
{
  corevm::types::native_map map_value =
    corevm::types::get_value_from_handle<corevm::types::native_map>(operand);

  corevm::types::uint32 result_value = map_value.size();
  result = result_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_map_empty(
  native_type_handle& operand, native_type_handle& result)
{
  corevm::types::native_map map_value =
    corevm::types::get_value_from_handle<corevm::types::native_map>(operand);

  corevm::types::boolean result_value = map_value.empty();
  result = result_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_map_find(
  native_type_handle& operand, native_type_handle& key,
  native_type_handle& result)
{
  corevm::types::native_map map_value =
    corevm::types::get_value_from_handle<corevm::types::native_map>(operand);

  corevm::types::native_map::key_type key_value =
    corevm::types::get_value_from_handle<corevm::types::native_map::key_type>(key);

  corevm::types::boolean result_value = map_value.find(key_value) != map_value.end();
  result = result_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_map_at(
  native_type_handle& operand, native_type_handle& key,
  native_type_handle& result)
{
  corevm::types::native_map map_value =
    corevm::types::get_value_from_handle<corevm::types::native_map>(operand);

  corevm::types::native_map::key_type key_value =
    corevm::types::get_value_from_handle<corevm::types::native_map::key_type>(key);

  corevm::types::uint64 result_value = map_value.at(key_value);
  result = result_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_map_put(
  native_type_handle& operand,
  native_type_handle& key,
  native_type_handle& data,
  native_type_handle& result)
{
  corevm::types::native_map map_value =
    corevm::types::get_value_from_handle<corevm::types::native_map>(operand);

  corevm::types::native_map::key_type key_value =
    corevm::types::get_value_from_handle<corevm::types::native_map::key_type>(key);

  corevm::types::native_map::mapped_type data_value =
    corevm::types::get_value_from_handle<corevm::types::native_map::mapped_type>(data);

  map_value[key_value] = data_value;
  result = map_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_map_erase(
  native_type_handle& operand, native_type_handle& key,
  native_type_handle& result)
{
  corevm::types::native_map map_value =
    corevm::types::get_value_from_handle<corevm::types::native_map>(operand);

  corevm::types::native_map::key_type key_value =
    corevm::types::get_value_from_handle<corevm::types::native_map::key_type>(key);

  map_value.erase(key_value);
  result = map_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_map_clear(
  native_type_handle& operand, native_type_handle& result)
{
  corevm::types::native_map map_value =
    corevm::types::get_value_from_handle<corevm::types::native_map>(operand);

  map_value.clear();
  result = map_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_map_swap(
  native_type_handle& operand, native_type_handle& other_operand,
  native_type_handle& result)
{
  corevm::types::native_map map_value =
    corevm::types::get_value_from_handle<corevm::types::native_map>(operand);

  corevm::types::native_map other_map_value =
    corevm::types::get_value_from_handle<corevm::types::native_map>(other_operand);

  map_value.swap(other_map_value);
  result = map_value;
}

// -----------------------------------------------------------------------------

void corevm::types::interface_map_keys(
  native_type_handle& operand, native_type_handle& result)
{
  corevm::types::native_map map_value =
    corevm::types::get_value_from_handle<corevm::types::native_map>(operand);

  corevm::types::native_array array_value;

  for (auto itr = map_value.begin(); itr != map_value.end(); ++itr)
  {
    corevm::types::native_map_key_type key =
      static_cast<corevm::types::native_map_key_type>(itr->first);

    corevm::types::native_array_element_type element =
      static_cast<corevm::types::native_array_element_type>(key);

    array_value.push_back(element);
  }

  result = std::move(array_value);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_map_vals(
  native_type_handle& operand, native_type_handle& result)
{
  corevm::types::native_map map_value =
    corevm::types::get_value_from_handle<corevm::types::native_map>(operand);

  corevm::types::native_array array_value;

  for (auto itr = map_value.begin(); itr != map_value.end(); ++itr)
  {
    corevm::types::native_map_mapped_type value =
      static_cast<corevm::types::native_map_mapped_type>(itr->second);

    corevm::types::native_array_element_type element =
      static_cast<corevm::types::native_array_element_type>(value);

    array_value.push_back(element);
  }

  result = std::move(array_value);
}

// -----------------------------------------------------------------------------

void corevm::types::interface_map_merge(
  native_type_handle& operand, native_type_handle& other_operand,
  native_type_handle& result)
{
  corevm::types::native_map map_value =
    corevm::types::get_value_from_handle<corevm::types::native_map>(operand);

  corevm::types::native_map other_map_value =
    corevm::types::get_value_from_handle<corevm::types::native_map>(other_operand);

  map_value.insert(other_map_value.begin(), other_map_value.end());

  result = map_value;
}

// -----------------------------------------------------------------------------
