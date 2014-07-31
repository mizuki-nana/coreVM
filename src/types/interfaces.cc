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

/**************************** CONVERSION OPERATIONS ***************************/

template<typename DstType>
void __interface_apply_conversion(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  typename DstType::value_type original_value = corevm::types::get_value_from_handle<typename DstType::value_type>(operand);
  operand = DstType(original_value);
  result = operand;
}

void corevm::types::interface_to_int8(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::int8>(operand, result);
}

void corevm::types::interface_to_uint8(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::uint8>(operand, result);
}

void corevm::types::interface_to_int16(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::int16>(operand, result);
}

void corevm::types::interface_to_uint16(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::uint16>(operand, result);
}

void corevm::types::interface_to_int32(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::int32>(operand, result);
}

void corevm::types::interface_to_uint32(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::uint32>(operand, result);
}

void corevm::types::interface_to_int64(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::int64>(operand, result);
}

void corevm::types::interface_to_uint64(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::uint64>(operand, result);
}

void corevm::types::interface_to_bool(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::boolean>(operand, result);
}

void corevm::types::interface_to_dec1(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::decimal>(operand, result);
}

void corevm::types::interface_to_dec2(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::decimal2>(operand, result);
}

void corevm::types::interface_to_str(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::string>(operand, result);
}

void corevm::types::interface_to_ary(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::array>(operand, result);
}

void corevm::types::interface_to_map(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  __interface_apply_conversion<corevm::types::map>(operand, result);
}

/**************************** STRING OPERATIONS *******************************/

void corevm::types::interface_string_get_size(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  corevm::types::native_string string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(operand);
  corevm::types::int32 size = string_value.size();
  result = size;
}

void corevm::types::interface_string_clear(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  corevm::types::native_string string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(operand);
  string_value.clear();
  operand = string_value;
  result = string_value;
}

void corevm::types::interface_string_at(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& index, corevm::types::native_type_handle& result)
{
  corevm::types::native_string string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(operand);
  int32_t index_value = corevm::types::get_value_from_handle<int32_t>(index);

  char char_value = string_value.at(index_value);
  corevm::types::uint32 result_value = char_value;
  result = result_value;
}

void corevm::types::interface_string_append(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& str, corevm::types::native_type_handle& result)
{
  corevm::types::native_string string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(operand);
  corevm::types::native_string other_string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(str);

  string_value.append(other_string_value);
  result = string_value;
}

void corevm::types::interface_string_pushback(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& c, corevm::types::native_type_handle& result)
{
  corevm::types::native_string string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(operand);
  char char_value = corevm::types::get_value_from_handle<char>(c);

  string_value.push_back(char_value);
  result = string_value;
}

void corevm::types::interface_string_insert_str(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& pos, corevm::types::native_type_handle& str, corevm::types::native_type_handle& result)
{
  corevm::types::native_string string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(operand);
  size_t pos_value = corevm::types::get_value_from_handle<size_t>(pos);
  corevm::types::native_string other_string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(str);

  corevm::types::native_string result_value = string_value.insert(pos_value, other_string_value);
  result = result_value;
}

void corevm::types::interface_string_insert_char(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& pos, corevm::types::native_type_handle& c, corevm::types::native_type_handle& result)
{
  corevm::types::native_string string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(operand);
  size_t pos_value = corevm::types::get_value_from_handle<size_t>(pos);
  char char_value = corevm::types::get_value_from_handle<char>(c);

  corevm::types::native_string result_value = string_value.insert(pos_value, 1, char_value);
  result = result_value;
}

void corevm::types::interface_string_erase(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& pos, corevm::types::native_type_handle& result)
{
  corevm::types::native_string string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(operand);
  size_t pos_value = corevm::types::get_value_from_handle<size_t>(pos);

  corevm::types::native_string result_value = string_value.erase(pos_value);
  result = result_value;
}

void corevm::types::interface_string_erase2(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& pos, corevm::types::native_type_handle& len, corevm::types::native_type_handle& result)
{
  corevm::types::native_string string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(operand);
  size_t pos_value = corevm::types::get_value_from_handle<size_t>(pos);
  size_t len_value = corevm::types::get_value_from_handle<size_t>(len);

  corevm::types::native_string result_value = string_value.erase(pos_value, len_value);
  result = result_value;
}

void corevm::types::interface_string_replace_str(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& pos, corevm::types::native_type_handle& len,
  corevm::types::native_type_handle& str, corevm::types::native_type_handle& result)
{
  corevm::types::native_string string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(operand);
  size_t pos_value = corevm::types::get_value_from_handle<size_t>(pos);
  size_t len_value = corevm::types::get_value_from_handle<size_t>(len);
  corevm::types::native_string str_value = corevm::types::get_value_from_handle<corevm::types::native_string>(str);

  corevm::types::native_string result_value = string_value.replace(pos_value, len_value, str_value);
  result = result_value;
}

void corevm::types::interface_string_swap(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& str, corevm::types::native_type_handle& result)
{
  corevm::types::native_string string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(operand);
  corevm::types::native_string other_string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(str);

  string_value.swap(other_string_value);
  result = string_value;
}

void corevm::types::interface_string_substr(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& pos, corevm::types::native_type_handle& result)
{
  corevm::types::native_string string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(operand);
  size_t pos_value = corevm::types::get_value_from_handle<size_t>(pos);

  corevm::types::native_string result_value = string_value.substr(pos_value);
  result = result_value;
}

void corevm::types::interface_string_substr2(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& pos, corevm::types::native_type_handle& len, corevm::types::native_type_handle& result)
{
  corevm::types::native_string string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(operand);
  size_t pos_value = corevm::types::get_value_from_handle<size_t>(pos);
  size_t len_value = corevm::types::get_value_from_handle<size_t>(len);

  corevm::types::native_string result_value = string_value.substr(pos_value, len_value);
  result = result_value;
}

void corevm::types::interface_string_find(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& str, corevm::types::native_type_handle& result)
{
  corevm::types::native_string string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(operand);
  corevm::types::native_string other_string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(str);

  corevm::types::uint32 result_value = string_value.find(other_string_value);
  result = result_value;
}

void corevm::types::interface_string_find2(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& str, corevm::types::native_type_handle& pos, corevm::types::native_type_handle& result)
{
  corevm::types::native_string string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(operand);
  corevm::types::native_string other_string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(str);
  size_t pos_value = corevm::types::get_value_from_handle<size_t>(pos);

  corevm::types::uint32 result_value = string_value.find(other_string_value, pos_value);
  result = result_value;
}

void corevm::types::interface_string_rfind(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& str, corevm::types::native_type_handle& result)
{
  corevm::types::native_string string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(operand);
  corevm::types::native_string other_string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(str);

  corevm::types::uint32 result_value = string_value.rfind(other_string_value);
  result = result_value;
}

void corevm::types::interface_string_rfind2(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& str, corevm::types::native_type_handle& pos, corevm::types::native_type_handle& result)
{
  corevm::types::native_string string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(operand);
  corevm::types::native_string other_string_value = corevm::types::get_value_from_handle<corevm::types::native_string>(str);
  size_t pos_value = corevm::types::get_value_from_handle<size_t>(pos);

  corevm::types::uint32 result_value = string_value.rfind(other_string_value, pos_value);
  result = result_value;
}


/**************************** ARRAY OPERATIONS *******************************/

void corevm::types::interface_array_size(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  corevm::types::native_array array_value = corevm::types::get_value_from_handle<corevm::types::native_array>(operand);
  corevm::types::int32 size = array_value.size();
  result = size;
}

void corevm::types::interface_array_empty(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  corevm::types::native_array array_value = corevm::types::get_value_from_handle<corevm::types::native_array>(operand);
  corevm::types::boolean empty = array_value.empty();
  result = empty;
}

void corevm::types::interface_array_at(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& index, corevm::types::native_type_handle& result)
{
  corevm::types::native_array array_value = corevm::types::get_value_from_handle<corevm::types::native_array>(operand);
  size_t index_value = corevm::types::get_value_from_handle<size_t>(index);

  corevm::types::uint64 result_value = array_value.at(index_value);
  result = result_value;
}

void corevm::types::interface_array_front(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  corevm::types::native_array array_value = corevm::types::get_value_from_handle<corevm::types::native_array>(operand);
  corevm::types::uint64 result_value = array_value.front();
  result = result_value;
}

void corevm::types::interface_array_back(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  corevm::types::native_array array_value = corevm::types::get_value_from_handle<corevm::types::native_array>(operand);
  corevm::types::uint64 result_value = array_value.back();
  result = result_value;
}

void corevm::types::interface_array_append(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& data, corevm::types::native_type_handle& result)
{
  corevm::types::native_array array_value = corevm::types::get_value_from_handle<corevm::types::native_array>(operand);
  corevm::types::native_array::value_type data_value = corevm::types::get_value_from_handle<corevm::types::native_array::value_type>(data);

  array_value.push_back(data_value);
  result = array_value;
}

void corevm::types::interface_array_pop(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  corevm::types::native_array array_value = corevm::types::get_value_from_handle<corevm::types::native_array>(operand);
  array_value.pop_back();
  result = array_value;
}

void corevm::types::interface_array_swap(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& other_operand, corevm::types::native_type_handle& result)
{
  corevm::types::native_array array_value = corevm::types::get_value_from_handle<corevm::types::native_array>(operand);
  corevm::types::native_array other_array_value = corevm::types::get_value_from_handle<corevm::types::native_array>(other_operand);

  array_value.swap(other_array_value);
  result = array_value;
}

void corevm::types::interface_array_clear(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  corevm::types::native_array array_value = corevm::types::get_value_from_handle<corevm::types::native_array>(operand);
  array_value.clear();
  result = array_value;
}

/****************************** MAP OPERATIONS ********************************/

void corevm::types::interface_map_size(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  corevm::types::native_map map_value = corevm::types::get_value_from_handle<corevm::types::native_map>(operand);
  corevm::types::uint32 result_value = map_value.size();
  result = result_value;
}

void corevm::types::interface_map_empty(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  corevm::types::native_map map_value = corevm::types::get_value_from_handle<corevm::types::native_map>(operand);
  corevm::types::boolean result_value = map_value.empty();
  result = result_value;
}

void corevm::types::interface_map_at(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& key, corevm::types::native_type_handle& result)
{
  corevm::types::native_map map_value = corevm::types::get_value_from_handle<corevm::types::native_map>(operand);
  corevm::types::native_map::key_type key_value = corevm::types::get_value_from_handle<corevm::types::native_map::key_type>(key);

  corevm::types::uint64 result_value = map_value.at(key_value);
  result = result_value;
}

void corevm::types::interface_map_put(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& key, corevm::types::native_type_handle& data, corevm::types::native_type_handle& result)
{
  corevm::types::native_map map_value = corevm::types::get_value_from_handle<corevm::types::native_map>(operand);
  corevm::types::native_map::key_type key_value = corevm::types::get_value_from_handle<corevm::types::native_map::key_type>(key);
  corevm::types::native_map::mapped_type data_value = corevm::types::get_value_from_handle<corevm::types::native_map::mapped_type>(data);

  map_value[key_value] = data_value;
  result = map_value;
}

void corevm::types::interface_map_erase(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& key, corevm::types::native_type_handle& result)
{
  corevm::types::native_map map_value = corevm::types::get_value_from_handle<corevm::types::native_map>(operand);
  corevm::types::native_map::key_type key_value = corevm::types::get_value_from_handle<corevm::types::native_map::key_type>(key);

  map_value.erase(key_value);
  result = map_value;
}

void corevm::types::interface_map_clear(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& result)
{
  corevm::types::native_map map_value = corevm::types::get_value_from_handle<corevm::types::native_map>(operand);
  map_value.clear();
  result = map_value;
}

void corevm::types::interface_map_swap(
  corevm::types::native_type_handle& operand, corevm::types::native_type_handle& other_operand, corevm::types::native_type_handle& result)
{
  corevm::types::native_map map_value = corevm::types::get_value_from_handle<corevm::types::native_map>(operand);
  corevm::types::native_map other_map_value = corevm::types::get_value_from_handle<corevm::types::native_map>(other_operand);

  map_value.swap(other_map_value);
  result = map_value;
}
