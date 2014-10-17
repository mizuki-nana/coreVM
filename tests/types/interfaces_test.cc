#include "native_type_interfaces_test_base.h"


class native_type_unary_operator_interfaces_test : public native_type_interfaces_test_base {
public:
  template<typename T, typename F>
  void apply_unary_operator_and_assert_result(corevm::types::native_type_handle& operand, F func, T expected_value)
  {
    corevm::types::native_type_handle result;
    func(operand, result);
    T actual_result = corevm::types::get_value_from_handle<T>(result);
    ASSERT_EQ(expected_value, actual_result);
  }
};


TEST_F(native_type_unary_operator_interfaces_test, TestPositiveOperator)
{
  corevm::types::native_type_handle operand = corevm::types::int32(5);
  this->apply_unary_operator_and_assert_result<int>(operand, corevm::types::interface_apply_positive_operator, 5);
}

TEST_F(native_type_unary_operator_interfaces_test, TestNegationOperator)
{
  corevm::types::native_type_handle operand = corevm::types::int32(5);
  this->apply_unary_operator_and_assert_result<int>(operand, corevm::types::interface_apply_negation_operator, -5);
}

TEST_F(native_type_unary_operator_interfaces_test, TestIncrementOperator)
{
  corevm::types::native_type_handle operand = corevm::types::int32(5);
  this->apply_unary_operator_and_assert_result<int>(operand, corevm::types::interface_apply_increment_operator, 6);
}

TEST_F(native_type_unary_operator_interfaces_test, TestDecrementOperator)
{
  corevm::types::native_type_handle operand = corevm::types::int32(5);
  this->apply_unary_operator_and_assert_result<int>(operand, corevm::types::interface_apply_decrement_operator, 4);
}

TEST_F(native_type_unary_operator_interfaces_test, TestLogicalNotOperator)
{
  corevm::types::native_type_handle operand = corevm::types::int32(5);
  this->apply_unary_operator_and_assert_result<int>(operand, corevm::types::interface_apply_logical_not_operator, !5);
}

TEST_F(native_type_unary_operator_interfaces_test, TestBitwiseNotOperator)
{
  corevm::types::native_type_handle operand = corevm::types::int32(5);
  this->apply_unary_operator_and_assert_result<int>(operand, corevm::types::interface_apply_bitwise_not_operator, ~5);
}

TEST_F(native_type_unary_operator_interfaces_test, TestInvalidOperator)
{
  corevm::types::native_type_handle operand = corevm::types::array();

  ASSERT_THROW(
    {
      this->apply_unary_operator_and_assert_result<int>(operand, corevm::types::interface_apply_decrement_operator, 0);
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}


class native_type_binary_operator_interfaces_test : public native_type_interfaces_test_base {
public:
  template<typename T, typename F>
  void apply_binary_operator_and_assert_result(
    corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, F func, T expected_value)
  {
    corevm::types::native_type_handle result;
    func(lhs, rhs, result);
    T actual_result = corevm::types::get_value_from_handle<T>(result);
    ASSERT_EQ(expected_value, actual_result);
  }
};


TEST_F(native_type_binary_operator_interfaces_test, TestAdditionOperator)
{
  corevm::types::native_type_handle lhs = corevm::types::int32(10);
  corevm::types::native_type_handle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(lhs, rhs, corevm::types::interface_apply_addition_operator, 10 + 5);
}

TEST_F(native_type_binary_operator_interfaces_test, TestSubtractionOperator)
{
  corevm::types::native_type_handle lhs = corevm::types::int32(10);
  corevm::types::native_type_handle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(lhs, rhs, corevm::types::interface_apply_subtraction_operator, 10 - 5);
}

TEST_F(native_type_binary_operator_interfaces_test, TestMultiplicationOperator)
{
  corevm::types::native_type_handle lhs = corevm::types::int32(10);
  corevm::types::native_type_handle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(lhs, rhs, corevm::types::interface_apply_multiplication_operator, 10 * 5);
}

TEST_F(native_type_binary_operator_interfaces_test, TestDivisionOperator)
{
  corevm::types::native_type_handle lhs = corevm::types::int32(10);
  corevm::types::native_type_handle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(lhs, rhs, corevm::types::interface_apply_division_operator, 10 / 5);
}

TEST_F(native_type_binary_operator_interfaces_test, TestModulusOperator)
{
  corevm::types::native_type_handle lhs = corevm::types::int32(10);
  corevm::types::native_type_handle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(lhs, rhs, corevm::types::interface_apply_modulus_operator, 10 % 5);
}

TEST_F(native_type_binary_operator_interfaces_test, TestLogicalANDOperator)
{
  corevm::types::native_type_handle lhs = corevm::types::int32(10);
  corevm::types::native_type_handle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(lhs, rhs, corevm::types::interface_apply_logical_and_operator, true);
}

TEST_F(native_type_binary_operator_interfaces_test, TestLogicalOROperator)
{
  corevm::types::native_type_handle lhs = corevm::types::int32(10);
  corevm::types::native_type_handle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(lhs, rhs, corevm::types::interface_apply_logical_or_operator, true);
}

TEST_F(native_type_binary_operator_interfaces_test, TestBitwiseANDOperator)
{
  corevm::types::native_type_handle lhs = corevm::types::int32(10);
  corevm::types::native_type_handle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(lhs, rhs, corevm::types::interface_apply_bitwise_and_operator, 10 & 5);
}

TEST_F(native_type_binary_operator_interfaces_test, TestBitwiseOROperator)
{
  corevm::types::native_type_handle lhs = corevm::types::int32(10);
  corevm::types::native_type_handle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(lhs, rhs, corevm::types::interface_apply_bitwise_or_operator, 10 | 5);
}

TEST_F(native_type_binary_operator_interfaces_test, TestBitwiseXOROperator)
{
  corevm::types::native_type_handle lhs = corevm::types::int32(10);
  corevm::types::native_type_handle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(lhs, rhs, corevm::types::interface_apply_bitwise_xor_operator, 10 ^ 5);
}

TEST_F(native_type_binary_operator_interfaces_test, TestBitwiseLeftShiftOperator)
{
  corevm::types::native_type_handle lhs = corevm::types::int32(10);
  corevm::types::native_type_handle rhs = corevm::types::int32(1);

  this->apply_binary_operator_and_assert_result<int>(lhs, rhs, corevm::types::interface_apply_bitwise_left_shift_operator, 10 << 1);
}

TEST_F(native_type_binary_operator_interfaces_test, TestBitwiseRightShiftOperator)
{
  corevm::types::native_type_handle lhs = corevm::types::int32(10);
  corevm::types::native_type_handle rhs = corevm::types::int32(1);

  this->apply_binary_operator_and_assert_result<int>(lhs, rhs, corevm::types::interface_apply_bitwise_right_shift_operator, 10 >> 1);
}

TEST_F(native_type_binary_operator_interfaces_test, TestEqOperator)
{
  corevm::types::native_type_handle lhs = corevm::types::int32(10);
  corevm::types::native_type_handle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(lhs, rhs, corevm::types::interface_apply_eq_operator, 10 == 5);
}

TEST_F(native_type_binary_operator_interfaces_test, TestNeqOperator)
{
  corevm::types::native_type_handle lhs = corevm::types::int32(10);
  corevm::types::native_type_handle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(lhs, rhs, corevm::types::interface_apply_neq_operator, 10 != 5);
}

TEST_F(native_type_binary_operator_interfaces_test, TestLtOperator)
{
  corevm::types::native_type_handle lhs = corevm::types::int32(10);
  corevm::types::native_type_handle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(lhs, rhs, corevm::types::interface_apply_lt_operator, 10 < 5);
}

TEST_F(native_type_binary_operator_interfaces_test, TestGtOperator)
{
  corevm::types::native_type_handle lhs = corevm::types::int32(10);
  corevm::types::native_type_handle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(lhs, rhs, corevm::types::interface_apply_gt_operator, 10 > 5);
}

TEST_F(native_type_binary_operator_interfaces_test, TestLteOperator)
{
  corevm::types::native_type_handle lhs = corevm::types::int32(10);
  corevm::types::native_type_handle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(lhs, rhs, corevm::types::interface_apply_lte_operator, 10 <= 5);
}

TEST_F(native_type_binary_operator_interfaces_test, TestGteOperator)
{
  corevm::types::native_type_handle lhs = corevm::types::int32(10);
  corevm::types::native_type_handle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(lhs, rhs, corevm::types::interface_apply_gte_operator, 10 >= 5);
}


class native_type_invalid_binary_operator_interfaces_test : public native_type_binary_operator_interfaces_test {
public:
  template<typename ErrorType, typename F>
  void apply_binary_operator_and_assert_result(
    corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs, F func)
  {
    corevm::types::native_type_handle result;

    ASSERT_THROW(
      { func(lhs, rhs, result); },
      ErrorType
    );
  }
};


TEST_F(native_type_invalid_binary_operator_interfaces_test, TestInvalidOperator)
{
  corevm::types::native_type_handle lhs = corevm::types::array();
  corevm::types::native_type_handle rhs = corevm::types::map();

  this->apply_binary_operator_and_assert_result<
    corevm::types::corevm_native_type_conversion_error>(lhs, rhs, corevm::types::interface_apply_addition_operator);
}
