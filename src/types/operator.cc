#include "../../include/types/operator.h"


/* decrement operator */

template<>
typename corevm::types::string::value_type
decrement::operator()<corevm::types::string>(const corevm::types::string& handle) {
  return static_cast<typename corevm::types::string::value_type>(--handle.value);
}

template<>
typename corevm::types::array::value_type
decrement::operator()<corevm::types::array>(const corevm::types::array& handle) {
  return static_cast<typename corevm::types::array::value_type>(--handle.value);
}

/* bitwise NOT operator */

template<>
typename corevm::types::string::value_type
bitwise_not::operator()<corevm::types::string>(const corevm::types::string& handle) {
  return static_cast<typename corevm::types::string::value_type>(~handle.value);
}

template<>
typename corevm::types::array::value_type
bitwise_not::operator()<corevm::types::array>(const corevm::types::array& handle) {
  return static_cast<typename corevm::types::array::value_type>(~handle.value);
}

/* addition operator */

template<>
typename corevm::types::string::value_type
addition::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value + rhs.value);
}

/* subtraction operator */

template<>
typename corevm::types::string::value_type
subtraction::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value - rhs.value);
}

/* multiplication operator */

template<>
typename corevm::types::string::value_type
multiplication::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value * rhs.value);
}

/* division operator */

template<>
typename corevm::types::string::value_type
division::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value / rhs.value);
}

/* modulus operator */

template<>
typename corevm::types::string::value_type
modulus::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value % rhs.value);
}

template<>
typename corevm::types::array::value_type
modulus::operator()<corevm::types::array>(
  const corevm::types::array& lhs, const corevm::types::array& rhs)
{
  return static_cast<typename corevm::types::array::value_type>(lhs.value % rhs.value);
}

/* logical AND operator */

template<>
typename corevm::types::string::value_type
logical_and::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value && rhs.value);
}

/* logical OR operator */

template<>
typename corevm::types::string::value_type
logical_or::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value || rhs.value);
}

/* bitwise AND operator */

template<>
typename corevm::types::string::value_type
bitwise_and::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value & rhs.value);
}

template<>
typename corevm::types::array::value_type
bitwise_and::operator()<corevm::types::array>(
  const corevm::types::array& lhs, const corevm::types::array& rhs)
{
  return static_cast<typename corevm::types::array::value_type>(lhs.value & rhs.value);
}

/* bitwise OR operator */

template<>
typename corevm::types::string::value_type
bitwise_or::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value | rhs.value);
}

template<>
typename corevm::types::array::value_type
bitwise_or::operator()<corevm::types::array>(
  const corevm::types::array& lhs, const corevm::types::array& rhs)
{
  return static_cast<typename corevm::types::array::value_type>(lhs.value | rhs.value);
}

/* bitwise XOR operator */

template<>
typename corevm::types::string::value_type
bitwise_xor::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value ^ rhs.value);
}

template<>
typename corevm::types::array::value_type
bitwise_xor::operator()<corevm::types::array>(
  const corevm::types::array& lhs, const corevm::types::array& rhs)
{
  return static_cast<typename corevm::types::array::value_type>(lhs.value ^ rhs.value);
}

/* bitwise left shift operator */

template<>
typename corevm::types::string::value_type
bitwise_left_shift::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value << rhs.value);
}

template<>
typename corevm::types::array::value_type
bitwise_left_shift::operator()<corevm::types::array>(
  const corevm::types::array& lhs, const corevm::types::array& rhs)
{
  return static_cast<typename corevm::types::array::value_type>(lhs.value << rhs.value);
}

/* bitwise right shift operator */

template<>
typename corevm::types::string::value_type
bitwise_right_shift::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value >> rhs.value);
}
template<>
typename corevm::types::array::value_type
bitwise_right_shift::operator()<corevm::types::array>(
  const corevm::types::array& lhs, const corevm::types::array& rhs)
{
  return static_cast<typename corevm::types::array::value_type>(lhs.value >> rhs.value);
}