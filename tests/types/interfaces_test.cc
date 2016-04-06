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
#include "native_type_interfaces_test_base.h"

#include <cmath>


// -----------------------------------------------------------------------------

class NativeTypeUnaryOperatorInterfacesTest : public NativeTypeInterfacesTestBase
{
public:
  template<typename T, typename F>
  void apply_unary_operator_and_assert_result(
    corevm::types::NativeTypeHandle& operand, F func, T expected_value,
    bool is_decimal=false)
  {
    func(operand);
    T actual_result = corevm::types::get_value_from_handle<T>(operand);

    if (is_decimal)
    {
      ASSERT_FLOAT_EQ(expected_value, actual_result);
    }
    else
    {
      ASSERT_EQ(expected_value, actual_result);
    }
  }
};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeUnaryOperatorInterfacesTest, TestPositiveOperator)
{
  corevm::types::NativeTypeHandle operand = corevm::types::int32(5);

  this->apply_unary_operator_and_assert_result<int>(
    operand,
    corevm::types::interface_apply_positive_operator,
    5
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeUnaryOperatorInterfacesTest, TestNegationOperator)
{
  corevm::types::NativeTypeHandle operand = corevm::types::int32(5);

  this->apply_unary_operator_and_assert_result<int>(
    operand,
    corevm::types::interface_apply_negation_operator,
    -5
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeUnaryOperatorInterfacesTest, TestIncrementOperator)
{
  corevm::types::NativeTypeHandle operand = corevm::types::int32(5);
  this->apply_unary_operator_and_assert_result<int>(
    operand,
    corevm::types::interface_apply_increment_operator,
    6
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeUnaryOperatorInterfacesTest, TestDecrementOperator)
{
  corevm::types::NativeTypeHandle operand = corevm::types::int32(5);

  this->apply_unary_operator_and_assert_result<int>(
    operand,
    corevm::types::interface_apply_decrement_operator,
    4
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeUnaryOperatorInterfacesTest, TestLogicalNotOperator)
{
  corevm::types::NativeTypeHandle operand = corevm::types::int32(5);

  this->apply_unary_operator_and_assert_result<int>(
    operand,
    corevm::types::interface_apply_logical_not_operator,
    !5
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeUnaryOperatorInterfacesTest, TestBitwiseNotOperator)
{
  corevm::types::NativeTypeHandle operand = corevm::types::int32(5);

  this->apply_unary_operator_and_assert_result<int>(
    operand,
    corevm::types::interface_apply_bitwise_not_operator,
    ~5
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeUnaryOperatorInterfacesTest, TestAbsOperator)
{
  corevm::types::NativeTypeHandle operand = corevm::types::decimal(-912.54689);

  apply_unary_operator_and_assert_result<float>(
    operand,
    corevm::types::interface_apply_abs_operator,
    912.54689f,
    /* is_decimal */ true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeUnaryOperatorInterfacesTest, TestSqrtOperator)
{
  corevm::types::NativeTypeHandle operand = corevm::types::decimal(9.869600);

  apply_unary_operator_and_assert_result<float>(
    operand,
    corevm::types::interface_apply_sqrt_operator,
    3.141592f,
    /* is_decimal */ true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeUnaryOperatorInterfacesTest, TestInvalidOperator)
{
  corevm::types::NativeTypeHandle operand = corevm::types::array();

  ASSERT_THROW(
    {
      this->apply_unary_operator_and_assert_result<int>(
        operand,
        corevm::types::interface_apply_decrement_operator,
        0
      );
    },
    corevm::types::InvalidOperatorError
  );
}

// -----------------------------------------------------------------------------

class NativeTypeBinaryOperatorInterfacesTest : public NativeTypeInterfacesTestBase
{
public:
  template<typename T, typename F>
  void apply_binary_operator_and_assert_result(
    corevm::types::NativeTypeHandle& lhs,
    corevm::types::NativeTypeHandle& rhs,
    F func,
    T expected_value,
    bool is_decimal=false)
  {
    auto res = func(lhs, rhs);
    T actual_result = corevm::types::get_value_from_handle<T>(res);

    if (!is_decimal)
    {
      ASSERT_EQ(expected_value, actual_result);
    }
    else
    {
      ASSERT_FLOAT_EQ(expected_value, actual_result);
    }
  }
};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestAdditionOperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::int32(10);
  corevm::types::NativeTypeHandle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(
    lhs,
    rhs,
    corevm::types::interface_apply_addition_operator,
    10 + 5
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestSubtractionOperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::int32(10);
  corevm::types::NativeTypeHandle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(
    lhs,
    rhs,
    corevm::types::interface_apply_subtraction_operator,
    10 - 5
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestMultiplicationOperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::int32(10);
  corevm::types::NativeTypeHandle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(
    lhs,
    rhs,
    corevm::types::interface_apply_multiplication_operator,
    10 * 5
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestDivisionOperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::int32(10);
  corevm::types::NativeTypeHandle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(
    lhs,
    rhs,
    corevm::types::interface_apply_division_operator,
    10 / 5
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestModulusOperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::int32(10);
  corevm::types::NativeTypeHandle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(
    lhs,
    rhs,
    corevm::types::interface_apply_modulus_operator,
    10 % 5
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestModulusOperatorWithFloatingPointOperands)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::decimal2(100.000001);
  corevm::types::NativeTypeHandle rhs = corevm::types::decimal2(33.000000);

  this->apply_binary_operator_and_assert_result<double>(
    lhs,
    rhs,
    corevm::types::interface_apply_modulus_operator,
    /* 100.000001 % 33.000000 */ 1.0000009999999975,
    /* is_decimal */ true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestModulusOperatorWithFloatingPointOperandsWithOppositeSigns)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::decimal2(-180.0);
  corevm::types::NativeTypeHandle rhs = corevm::types::decimal2(3.141592);

  this->apply_binary_operator_and_assert_result<double>(
    lhs,
    rhs,
    corevm::types::interface_apply_modulus_operator,
    /* -180.0 % 3.141592 */ 2.2123361,
    /* is_decimal */ true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestPowOperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::decimal(2.3);
  corevm::types::NativeTypeHandle rhs = corevm::types::decimal2(1.4);

  this->apply_binary_operator_and_assert_result<double>(
    lhs,
    rhs,
    corevm::types::interface_apply_pow_operator,
    pow(2.3, 1.4),
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestLogicalANDOperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::int32(10);
  corevm::types::NativeTypeHandle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(
    lhs,
    rhs,
    corevm::types::interface_apply_logical_and_operator,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestLogicalOROperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::int32(10);
  corevm::types::NativeTypeHandle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(
    lhs,
    rhs,
    corevm::types::interface_apply_logical_or_operator,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestBitwiseANDOperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::int32(10);
  corevm::types::NativeTypeHandle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(
    lhs,
    rhs,
    corevm::types::interface_apply_bitwise_and_operator,
    10 & 5
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestBitwiseOROperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::int32(10);
  corevm::types::NativeTypeHandle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(
    lhs,
    rhs,
    corevm::types::interface_apply_bitwise_or_operator,
    10 | 5
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestBitwiseXOROperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::int32(10);
  corevm::types::NativeTypeHandle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(
    lhs,
    rhs,
    corevm::types::interface_apply_bitwise_xor_operator,
    10 ^ 5
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestBitwiseLeftShiftOperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::int32(10);
  corevm::types::NativeTypeHandle rhs = corevm::types::int32(1);

  this->apply_binary_operator_and_assert_result<int>(
    lhs,
    rhs,
    corevm::types::interface_apply_bitwise_left_shift_operator,
    10 << 1
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestBitwiseRightShiftOperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::int32(10);
  corevm::types::NativeTypeHandle rhs = corevm::types::int32(1);

  this->apply_binary_operator_and_assert_result<int>(
    lhs,
    rhs,
    corevm::types::interface_apply_bitwise_right_shift_operator,
    10 >> 1
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestEqOperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::int32(10);
  corevm::types::NativeTypeHandle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(
    lhs,
    rhs,
    corevm::types::interface_apply_eq_operator,
    10 == 5
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestNeqOperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::int32(10);
  corevm::types::NativeTypeHandle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(
    lhs,
    rhs,
    corevm::types::interface_apply_neq_operator,
    10 != 5
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestLtOperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::int32(10);
  corevm::types::NativeTypeHandle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(
    lhs,
    rhs,
    corevm::types::interface_apply_lt_operator, 10 < 5
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestGtOperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::int32(10);
  corevm::types::NativeTypeHandle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(
    lhs,
    rhs,
    corevm::types::interface_apply_gt_operator,
    10 > 5
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestLteOperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::int32(10);
  corevm::types::NativeTypeHandle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(
    lhs,
    rhs,
    corevm::types::interface_apply_lte_operator,
    10 <= 5
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestGteOperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::int32(10);
  corevm::types::NativeTypeHandle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int>(
    lhs,
    rhs,
    corevm::types::interface_apply_gte_operator,
    10 >= 5
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeBinaryOperatorInterfacesTest, TestCmpOperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::int32(10);
  corevm::types::NativeTypeHandle rhs = corevm::types::int32(5);

  this->apply_binary_operator_and_assert_result<int32_t>(
    lhs,
    rhs,
    corevm::types::interface_apply_cmp_operator,
    1
  );
}

// -----------------------------------------------------------------------------

class NativeTypeInvalidBinaryOperatorInterfacesTest : public NativeTypeBinaryOperatorInterfacesTest
{
public:
  template<typename ErrorType, typename F>
  void apply_binary_operator_and_assert_result(
    corevm::types::NativeTypeHandle& lhs, corevm::types::NativeTypeHandle& rhs, F func)
  {
    ASSERT_THROW(
      {
        func(lhs, rhs);
      },
      ErrorType
    );
  }
};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInvalidBinaryOperatorInterfacesTest, TestInvalidOperator)
{
  corevm::types::NativeTypeHandle lhs = corevm::types::array();
  corevm::types::NativeTypeHandle rhs = corevm::types::map();

  this->apply_binary_operator_and_assert_result<corevm::types::ConversionError>(
    lhs,
    rhs,
    corevm::types::interface_apply_addition_operator
  );
}

// -----------------------------------------------------------------------------

class NativeTypeGenericInterfacesTest : public NativeTypeInterfacesTestBase {};

// -----------------------------------------------------------------------------

class NativeTypeInterfaceComputeSliceTest : public NativeTypeGenericInterfacesTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeSliceTest, TestWithEmptyArrayOperandAndEmptyRange)
{
  /**
   * Slicing [] with [0:0] = [].
   */
  corevm::types::NativeTypeHandle oprd1 = corevm::types::array();
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(0);
  corevm::types::NativeTypeHandle oprd3 = corevm::types::uint32(0);

  corevm::types::array expected_result;

  apply_interface_on_three_operands_and_assert_result<corevm::types::array>(
    oprd1,
    oprd2,
    oprd3,
    corevm::types::interface_compute_slice,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeSliceTest, TestWithEmptyArrayOperandAndInvalidRange)
{
  /**
   * Slicing [] with [1:10] = [].
   */
  corevm::types::NativeTypeHandle oprd1 = corevm::types::array();
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(1);
  corevm::types::NativeTypeHandle oprd3 = corevm::types::uint32(10);

  corevm::types::array expected_result;

  apply_interface_on_three_operands_and_assert_result<corevm::types::array>(
    oprd1,
    oprd2,
    oprd3,
    corevm::types::interface_compute_slice,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeSliceTest, TestWithArrayOperandAndValidRange)
{
  /**
   * Slicing [1, 2, 3, 4, 5] with [1:3] = [2, 3].
   */
  corevm::types::NativeTypeHandle oprd1 = corevm::types::array({1, 2, 3, 4, 5});
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(1);
  corevm::types::NativeTypeHandle oprd3 = corevm::types::uint32(3);

  corevm::types::array expected_result { 2, 3 };

  apply_interface_on_three_operands_and_assert_result<corevm::types::array>(
    oprd1,
    oprd2,
    oprd3,
    corevm::types::interface_compute_slice,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeSliceTest, TestWithArrayOperandAndValidRange2)
{
  /**
   * Slicing [1, 2, 3, 4, 5] with [0:3] = [1, 2, 3].
   */
  corevm::types::NativeTypeHandle oprd1 = corevm::types::array({1, 2, 3, 4, 5});
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(0);
  corevm::types::NativeTypeHandle oprd3 = corevm::types::uint32(3);

  corevm::types::array expected_result { 1, 2, 3 };

  apply_interface_on_three_operands_and_assert_result<corevm::types::array>(
    oprd1,
    oprd2,
    oprd3,
    corevm::types::interface_compute_slice,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeSliceTest, TestWithArrayOperandAndValidRange3)
{
  /**
   * Slicing [1, 2, 3, 4, 5] with [1:1] = [].
   */
  corevm::types::NativeTypeHandle oprd1 = corevm::types::array({1, 2, 3, 4, 5});
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(1);
  corevm::types::NativeTypeHandle oprd3 = corevm::types::uint32(1);

  corevm::types::array expected_result;

  apply_interface_on_three_operands_and_assert_result<corevm::types::array>(
    oprd1,
    oprd2,
    oprd3,
    corevm::types::interface_compute_slice,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeSliceTest, TestWithArrayOperandAndInValidRangeWithExceedingStopValue)
{
  /**
   * Slicing [1, 2, 3, 4, 5] with [0:5] = [1, 2, 3, 4, 5].
   */
  corevm::types::NativeTypeHandle oprd1 = corevm::types::array({1, 2, 3, 4, 5});
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(0);
  corevm::types::NativeTypeHandle oprd3 = corevm::types::uint32(5);

  corevm::types::array expected_result { 1, 2, 3, 4, 5 };

  apply_interface_on_three_operands_and_assert_result<corevm::types::array>(
    oprd1,
    oprd2,
    oprd3,
    corevm::types::interface_compute_slice,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeSliceTest, TestWithArrayOperandAndInValidRangeWithExceedingStartAndStopValues)
{
  /**
   * Slicing [1, 2, 3, 4, 5] with [5:10] = [].
   */
  corevm::types::NativeTypeHandle oprd1 = corevm::types::array({1, 2, 3, 4, 5});
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(5);
  corevm::types::NativeTypeHandle oprd3 = corevm::types::uint32(10);

  corevm::types::array expected_result;

  apply_interface_on_three_operands_and_assert_result<corevm::types::array>(
    oprd1,
    oprd2,
    oprd3,
    corevm::types::interface_compute_slice,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeSliceTest, TestWithArrayOperandAndInValidRangeWithReversedValues)
{
  /**
   * Slicing [1, 2, 3, 4, 5] with [4:0] = [].
   */
  corevm::types::NativeTypeHandle oprd1 = corevm::types::array({1, 2, 3, 4, 5});
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(4);
  corevm::types::NativeTypeHandle oprd3 = corevm::types::uint32(0);

  corevm::types::array expected_result;

  apply_interface_on_three_operands_and_assert_result<corevm::types::array>(
    oprd1,
    oprd2,
    oprd3,
    corevm::types::interface_compute_slice,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeSliceTest, TestWithArrayOperandAndInValidRangeWithExceedingReversedValues)
{
  /**
   * Slicing [1, 2, 3, 4, 5] with [10:5] = [].
   */
  corevm::types::NativeTypeHandle oprd1 = corevm::types::array({1, 2, 3, 4, 5});
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(10);
  corevm::types::NativeTypeHandle oprd3 = corevm::types::uint32(5);

  corevm::types::array expected_result;

  apply_interface_on_three_operands_and_assert_result<corevm::types::array>(
    oprd1,
    oprd2,
    oprd3,
    corevm::types::interface_compute_slice,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeSliceTest, TestWithEmptyStringOperandAndEmptyRange)
{
  /**
   * Slicing "" with [0:0] = "".
   */
  corevm::types::NativeTypeHandle oprd1 = corevm::types::string();
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(0);
  corevm::types::NativeTypeHandle oprd3 = corevm::types::uint32(0);

  corevm::types::string expected_result;

  apply_interface_on_three_operands_and_assert_result<corevm::types::string>(
    oprd1,
    oprd2,
    oprd3,
    corevm::types::interface_compute_slice,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeSliceTest, TestWithEmptyStringOperandAndInvalidRange)
{
  /**
   * Slicing "" with [1:10] = "".
   */
  corevm::types::NativeTypeHandle oprd1 = corevm::types::string();
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(1);
  corevm::types::NativeTypeHandle oprd3 = corevm::types::uint32(10);

  corevm::types::string expected_result;

  apply_interface_on_three_operands_and_assert_result<corevm::types::string>(
    oprd1,
    oprd2,
    oprd3,
    corevm::types::interface_compute_slice,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeSliceTest, TestWithStringOperandAndValidRange)
{
  /**
   * Slicing "Hello" with [1:3] = "ll".
   */
  corevm::types::NativeTypeHandle oprd1 = corevm::types::string("Hello");
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(1);
  corevm::types::NativeTypeHandle oprd3 = corevm::types::uint32(3);

  corevm::types::string expected_result("el");

  apply_interface_on_three_operands_and_assert_result<corevm::types::string>(
    oprd1,
    oprd2,
    oprd3,
    corevm::types::interface_compute_slice,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeSliceTest, TestWithStringOperandAndValidRange2)
{
  /**
   * Slicing "Hello" with [0:3] = "ell".
   */
  corevm::types::NativeTypeHandle oprd1 = corevm::types::string("Hello");
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(0);
  corevm::types::NativeTypeHandle oprd3 = corevm::types::uint32(3);

  corevm::types::string expected_result("Hel");

  apply_interface_on_three_operands_and_assert_result<corevm::types::string>(
    oprd1,
    oprd2,
    oprd3,
    corevm::types::interface_compute_slice,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeSliceTest, TestWithStringOperandAndValidRange3)
{
  /**
   * Slicing "Hello" with [1:1] = "".
   */
  corevm::types::NativeTypeHandle oprd1 = corevm::types::string("Hello");
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(1);
  corevm::types::NativeTypeHandle oprd3 = corevm::types::uint32(1);

  corevm::types::string expected_result;

  apply_interface_on_three_operands_and_assert_result<corevm::types::string>(
    oprd1,
    oprd2,
    oprd3,
    corevm::types::interface_compute_slice,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeSliceTest, TestWithStringOperandAndInValidRangeWithExceedingStopValue)
{
  /**
   * Slicing "Hello" with [0:5] = "Hello".
   */
  corevm::types::NativeTypeHandle oprd1 = corevm::types::string("Hello");
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(0);
  corevm::types::NativeTypeHandle oprd3 = corevm::types::uint32(5);

  corevm::types::string expected_result("Hello");

  apply_interface_on_three_operands_and_assert_result<corevm::types::string>(
    oprd1,
    oprd2,
    oprd3,
    corevm::types::interface_compute_slice,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeSliceTest, TestWithStringOperandAndInValidRangeWithExceedingStartAndStopValues)
{
  /**
   * Slicing "Hello" with [5:10] = "".
   */
  corevm::types::NativeTypeHandle oprd1 = corevm::types::string("Hello");
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(5);
  corevm::types::NativeTypeHandle oprd3 = corevm::types::uint32(10);

  corevm::types::string expected_result;

  apply_interface_on_three_operands_and_assert_result<corevm::types::string>(
    oprd1,
    oprd2,
    oprd3,
    corevm::types::interface_compute_slice,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeSliceTest, TestWithStringOperandAndInValidRangeWithReversedValues)
{
  /**
   * Slicing "Hello" with [4:0] = "".
   */
  corevm::types::NativeTypeHandle oprd1 = corevm::types::string("Hello");
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(4);
  corevm::types::NativeTypeHandle oprd3 = corevm::types::uint32(0);

  corevm::types::string expected_result;

  apply_interface_on_three_operands_and_assert_result<corevm::types::string>(
    oprd1,
    oprd2,
    oprd3,
    corevm::types::interface_compute_slice,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeSliceTest, TestWithStringOperandAndInValidRangeWithExceedingReversedValues)
{
  /**
   * Slicing "Hello" with [10:5] = "".
   */
  corevm::types::NativeTypeHandle oprd1 = corevm::types::string("Hello");
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(10);
  corevm::types::NativeTypeHandle oprd3 = corevm::types::uint32(5);

  corevm::types::string expected_result;

  apply_interface_on_three_operands_and_assert_result<corevm::types::string>(
    oprd1,
    oprd2,
    oprd3,
    corevm::types::interface_compute_slice,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeSliceTest, TestWithInvalidType)
{
  corevm::types::NativeTypeHandle oprd1 = corevm::types::uint32(100);
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(10);
  corevm::types::NativeTypeHandle oprd3 = corevm::types::uint32(20);

  ASSERT_THROW(
    {
      auto result = corevm::types::interface_compute_slice(oprd1, oprd2, oprd3);
    },
    corevm::types::RuntimeError
  );
}

// -----------------------------------------------------------------------------

class NativeTypeInterfaceComputeStrideTest : public NativeTypeGenericInterfacesTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeStrideTest, TestWithEmptyArrayOperandAndZeroStride)
{
  corevm::types::NativeTypeHandle oprd = corevm::types::array();
  corevm::types::NativeTypeHandle stride = corevm::types::int32(0);

  corevm::types::array expected_result;

  apply_interface_on_two_operands_and_assert_result<corevm::types::array>(
    oprd,
    stride,
    corevm::types::interface_compute_stride,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeStrideTest, TestWithEmptyArrayOperandAndPositiveStride)
{
  corevm::types::NativeTypeHandle oprd = corevm::types::array();
  corevm::types::NativeTypeHandle stride = corevm::types::int32(2);

  corevm::types::array expected_result;

  apply_interface_on_two_operands_and_assert_result<corevm::types::array>(
    oprd,
    stride,
    corevm::types::interface_compute_stride,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeStrideTest, TestWithArrayOperandAndZeroStride)
{
  corevm::types::NativeTypeHandle oprd = corevm::types::array({1, 2, 3, 4, 5});
  corevm::types::NativeTypeHandle stride = corevm::types::int32(0);

  corevm::types::array expected_result;

  apply_interface_on_two_operands_and_assert_result<corevm::types::array>(
    oprd,
    stride,
    corevm::types::interface_compute_stride,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeStrideTest, TestWithArrayOperandAndValidStride)
{
  corevm::types::NativeTypeHandle oprd = corevm::types::array({1, 2, 3, 4, 5});
  corevm::types::NativeTypeHandle stride = corevm::types::int32(2);

  corevm::types::array expected_result { 1, 3, 5 };

  apply_interface_on_two_operands_and_assert_result<corevm::types::array>(
    oprd,
    stride,
    corevm::types::interface_compute_stride,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeStrideTest, TestWithArrayOperandAndInvalidStride)
{
  corevm::types::NativeTypeHandle oprd = corevm::types::array({1, 2, 3, 4, 5});
  corevm::types::NativeTypeHandle stride = corevm::types::int32(5);

  corevm::types::array expected_result { 1 };

  apply_interface_on_two_operands_and_assert_result<corevm::types::array>(
    oprd,
    stride,
    corevm::types::interface_compute_stride,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeStrideTest, TestWithArrayOperandAndNegativeStride)
{
  corevm::types::NativeTypeHandle oprd = corevm::types::array({1, 2, 3, 4, 5});
  corevm::types::NativeTypeHandle stride = corevm::types::int32(-1);

  corevm::types::array expected_result;

  apply_interface_on_two_operands_and_assert_result<corevm::types::array>(
    oprd,
    stride,
    corevm::types::interface_compute_stride,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeStrideTest, TestWithEmptyStringOperandAndZeroStride)
{
  corevm::types::NativeTypeHandle oprd = corevm::types::string();
  corevm::types::NativeTypeHandle stride = corevm::types::int32(0);

  corevm::types::string expected_result;

  apply_interface_on_two_operands_and_assert_result<corevm::types::string>(
    oprd,
    stride,
    corevm::types::interface_compute_stride,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeStrideTest, TestWithEmptyStringOperandAndPositiveStride)
{
  corevm::types::NativeTypeHandle oprd = corevm::types::string();
  corevm::types::NativeTypeHandle stride = corevm::types::int32(2);

  corevm::types::string expected_result;

  apply_interface_on_two_operands_and_assert_result<corevm::types::string>(
    oprd,
    stride,
    corevm::types::interface_compute_stride,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeStrideTest, TestWithStringOperandAndZeroStride)
{
  corevm::types::NativeTypeHandle oprd = corevm::types::string("Hello");
  corevm::types::NativeTypeHandle stride = corevm::types::int32(0);

  corevm::types::string expected_result;

  apply_interface_on_two_operands_and_assert_result<corevm::types::string>(
    oprd,
    stride,
    corevm::types::interface_compute_stride,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeStrideTest, TestWithStringOperandAndValidStride)
{
  corevm::types::NativeTypeHandle oprd = corevm::types::string("Hello");
  corevm::types::NativeTypeHandle stride = corevm::types::int32(2);

  corevm::types::string expected_result("Hlo");

  apply_interface_on_two_operands_and_assert_result<corevm::types::string>(
    oprd,
    stride,
    corevm::types::interface_compute_stride,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeStrideTest, TestWithStringOperandAndInvalidStride)
{
  corevm::types::NativeTypeHandle oprd = corevm::types::string("Hello");
  corevm::types::NativeTypeHandle stride = corevm::types::int32(5);

  corevm::types::string expected_result("H");

  apply_interface_on_two_operands_and_assert_result<corevm::types::string>(
    oprd,
    stride,
    corevm::types::interface_compute_stride,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeStrideTest, TestWithStringOperandAndNegativeStride)
{
  corevm::types::NativeTypeHandle oprd = corevm::types::string("Hello");
  corevm::types::NativeTypeHandle stride = corevm::types::int32(-1);

  corevm::types::string expected_result;

  apply_interface_on_two_operands_and_assert_result<corevm::types::string>(
    oprd,
    stride,
    corevm::types::interface_compute_stride,
    expected_result);
}

// -----------------------------------------------------------------------------

class NativeTypeInterfaceComputeReverseTest : public NativeTypeGenericInterfacesTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeReverseTest, TestWithEmptyArrayOperand)
{
  corevm::types::NativeTypeHandle oprd = corevm::types::array();

  corevm::types::array expected_result;

  apply_interface_on_single_operand_and_assert_result<corevm::types::array>(
    oprd,
    corevm::types::interface_compute_reverse,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeReverseTest, TestWithArrayOperand)
{
  corevm::types::NativeTypeHandle oprd = corevm::types::array({1, 2, 3, 4, 5});

  corevm::types::array expected_result { 5, 4, 3, 2, 1 };

  apply_interface_on_single_operand_and_assert_result<corevm::types::array>(
    oprd,
    corevm::types::interface_compute_reverse,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeReverseTest, TestWithEmptyStringOperand)
{
  corevm::types::NativeTypeHandle oprd = corevm::types::string();

  corevm::types::string expected_result;

  apply_interface_on_single_operand_and_assert_result<corevm::types::string>(
    oprd,
    corevm::types::interface_compute_reverse,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeReverseTest, TestWithStringOperand)
{
  corevm::types::NativeTypeHandle oprd = corevm::types::string("Hello");

  corevm::types::string expected_result("olleH");

  apply_interface_on_single_operand_and_assert_result<corevm::types::string>(
    oprd,
    corevm::types::interface_compute_reverse,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceComputeReverseTest, TestWithInvalidTypeOperand)
{
  corevm::types::NativeTypeHandle oprd = corevm::types::decimal(3.1415);

  ASSERT_THROW(
    {
      auto result = corevm::types::interface_compute_reverse(oprd);
    },
    corevm::types::RuntimeError
  );
}

// -----------------------------------------------------------------------------

class NativeTypeInterfaceApplyRoundingTest : public NativeTypeGenericInterfacesTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceApplyRoundingTest, TestWithFloatingOperand)
{
  corevm::types::NativeTypeHandle oprd(corevm::types::decimal2(103.141592));
  corevm::types::NativeTypeHandle oprd2(corevm::types::uint32(5));

  const corevm::types::decimal2 expected_result = 103.14159;

  apply_interface_on_two_operands_and_assert_result<corevm::types::decimal2>(
    oprd,
    oprd2,
    corevm::types::interface_apply_rounding,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceApplyRoundingTest, TestWithSecondOperandEqualsToZero)
{
  corevm::types::NativeTypeHandle oprd(corevm::types::decimal2(103.141592));
  corevm::types::NativeTypeHandle oprd2(corevm::types::uint32(0));

  const corevm::types::decimal2 expected_result = 103.0;

  apply_interface_on_two_operands_and_assert_result<corevm::types::decimal2>(
    oprd,
    oprd2,
    corevm::types::interface_apply_rounding,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceApplyRoundingTest, TestWithLargeSecondOperand)
{
  corevm::types::NativeTypeHandle oprd(corevm::types::decimal2(103.141592));
  corevm::types::NativeTypeHandle oprd2(corevm::types::uint32(10));

  const corevm::types::decimal2 expected_result = 103.141592;

  apply_interface_on_two_operands_and_assert_result<corevm::types::decimal2>(
    oprd,
    oprd2,
    corevm::types::interface_apply_rounding,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceApplyRoundingTest, TestWithNegativeFloatingOperand)
{
  corevm::types::NativeTypeHandle oprd(corevm::types::decimal2(-103.141592));
  corevm::types::NativeTypeHandle oprd2(corevm::types::uint32(5));

  const corevm::types::decimal2 expected_result = -103.14159;

  apply_interface_on_two_operands_and_assert_result<corevm::types::decimal2>(
    oprd,
    oprd2,
    corevm::types::interface_apply_rounding,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceApplyRoundingTest, TestWithFirstOperandEqualsToZero)
{
  corevm::types::NativeTypeHandle oprd(corevm::types::decimal2(0.0));
  corevm::types::NativeTypeHandle oprd2(corevm::types::uint32(5));

  const corevm::types::decimal2 expected_result = 0.0;

  apply_interface_on_two_operands_and_assert_result<corevm::types::decimal2>(
    oprd,
    oprd2,
    corevm::types::interface_apply_rounding,
    expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeInterfaceApplyRoundingTest, TestWithInvalidFirstOperand)
{
  corevm::types::NativeTypeHandle oprd(corevm::types::string("hello"));
  corevm::types::NativeTypeHandle oprd2(corevm::types::uint32(5));

  ASSERT_THROW(
    {
      auto result = corevm::types::interface_apply_rounding(oprd, oprd2);
    },
    corevm::types::ConversionError
  );
}

// -----------------------------------------------------------------------------
