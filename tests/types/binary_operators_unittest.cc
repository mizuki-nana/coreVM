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
#include "types/operators.h"
#include "types/types.h"

#include <gtest/gtest.h>

#include <cmath>
#include <limits>
#include <type_traits>


// -----------------------------------------------------------------------------

class binary_operators_unittest_base : public ::testing::Test
{
protected:
  template<class op, class ResultType, class T, class U>
  void call_binary_op_and_assert_result(
    const T oprd1, const U oprd2, const ResultType expected_result) const
  {
    const ResultType actual_result =
      op().template operator()<ResultType>(oprd1, oprd2);

    if (std::is_floating_point<ResultType>::value)
    {
      ASSERT_FLOAT_EQ(expected_result, actual_result);
    }
    else
    {
      ASSERT_EQ(expected_result, actual_result);
    }
  }

  template<class op, class T, class U>
  void call_typed_binary_op_and_assert_result(
    const T oprd1, const U oprd2, const typename op::result_type expected_result) const
  {
    call_binary_op_and_assert_result<op, typename op::result_type>(oprd1, oprd2, expected_result);
  }
};

// -----------------------------------------------------------------------------

class addition_operator_unittest : public binary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(addition_operator_unittest, TestWithIntegeralOperands)
{
  corevm::types::int64 oprd1 = 64;
  corevm::types::uint32 oprd2 = std::numeric_limits<corevm::types::uint32>::max();

  corevm::types::int64 expected_result = oprd1 + oprd2;

  call_binary_op_and_assert_result<
    corevm::types::addition, corevm::types::int64>(oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(addition_operator_unittest, TestWithIntegeralAndFloatingPointOperands)
{
  corevm::types::int64 oprd1 = 64;
  corevm::types::decimal oprd2 = 0.64f;

  corevm::types::decimal expected_result = oprd1 + oprd2;

  call_binary_op_and_assert_result<
    corevm::types::addition, corevm::types::decimal>(oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

class subtraction_operator_unittest : public binary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(subtraction_operator_unittest, TestWithIntegeralOperands)
{
  corevm::types::int64 oprd1 = 64;
  corevm::types::uint32 oprd2 = std::numeric_limits<corevm::types::uint32>::max();

  corevm::types::int64 expected_result = oprd1 - oprd2;

  call_binary_op_and_assert_result<
    corevm::types::subtraction, corevm::types::int64>(oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(subtraction_operator_unittest, TestWithIntegeralAndFloatingPointOperands)
{
  corevm::types::int64 oprd1 = 64;
  corevm::types::decimal oprd2 = 0.64f;

  corevm::types::decimal expected_result = oprd1 - oprd2;

  call_binary_op_and_assert_result<
    corevm::types::subtraction, corevm::types::decimal>(oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

class multiplication_operator_unittest : public binary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(multiplication_operator_unittest, TestWithIntegeralOperands)
{
  corevm::types::int64 oprd1 = 64;
  corevm::types::uint32 oprd2 = std::numeric_limits<corevm::types::uint32>::max();

  corevm::types::int64 expected_result = oprd1 * oprd2;

  call_binary_op_and_assert_result<
    corevm::types::multiplication, corevm::types::int64>(oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(multiplication_operator_unittest, TestWithIntegeralAndFloatingPointOperands)
{
  corevm::types::int64 oprd1 = 64;
  corevm::types::decimal oprd2 = 0.64f;

  corevm::types::decimal expected_result = oprd1 * oprd2;

  call_binary_op_and_assert_result<
    corevm::types::multiplication, corevm::types::decimal>(oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

class division_operator_unittest : public binary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(division_operator_unittest, TestWithIntegeralOperands)
{
  corevm::types::uint32 oprd1 = std::numeric_limits<corevm::types::uint32>::max();
  corevm::types::int64 oprd2 = 64;

  corevm::types::int64 expected_result = oprd1 / oprd2;

  call_binary_op_and_assert_result<
    corevm::types::division, corevm::types::int64>(oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(division_operator_unittest, TestWithIntegeralAndFloatingPointOperands)
{
  corevm::types::int64 oprd1 = 64;
  corevm::types::decimal oprd2 = 0.64f;

  corevm::types::decimal expected_result = oprd1 / oprd2;

  call_binary_op_and_assert_result<
    corevm::types::division, corevm::types::decimal>(oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

class modulus_operator_unittest : public binary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(modulus_operator_unittest, TestWithIntegeralOperands)
{
  corevm::types::uint32 oprd1 = std::numeric_limits<corevm::types::uint32>::max();
  corevm::types::int64 oprd2 = 64;

  // See implementation of `corevm::types::modulus` operator.
  corevm::types::int64 expected_result = (oprd1 % oprd2) + oprd2;

  call_binary_op_and_assert_result<
    corevm::types::modulus, corevm::types::int64>(oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(modulus_operator_unittest, TestWithIntegeralAndFloatingPointOperands)
{
  corevm::types::int64 oprd1 = 64;
  corevm::types::decimal oprd2 = 0.64f;

  corevm::types::decimal expected_result =
    static_cast<corevm::types::decimal>(fmod(static_cast<corevm::types::int64>(oprd1), oprd2));

  call_binary_op_and_assert_result<
    corevm::types::modulus, corevm::types::decimal>(oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

class pow_operator_unittest : public binary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(pow_operator_unittest, TestWithIntegeralOperands)
{
  corevm::types::uint32 oprd1 = 10000u;
  corevm::types::int64 oprd2 = 2;

  corevm::types::int64 expected_result = static_cast<corevm::types::int64>(
    pow(static_cast<corevm::types::decimal2>(oprd1), static_cast<corevm::types::decimal2>(oprd2)));

  call_binary_op_and_assert_result<
    corevm::types::pow_op, corevm::types::int64>(oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(pow_operator_unittest, TestWithIntegeralAndFloatingPointOperands)
{
  corevm::types::int64 oprd1 = 64;
  corevm::types::decimal oprd2 = 0.64f;

  corevm::types::decimal expected_result =
    static_cast<corevm::types::decimal>(pow(oprd1, oprd2));

  call_binary_op_and_assert_result<
    corevm::types::pow_op, corevm::types::decimal>(oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

class bitwise_and_operator_unittest : public binary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(bitwise_and_operator_unittest, TestWithIntegeralOperands)
{
  corevm::types::uint32 oprd1 = std::numeric_limits<corevm::types::uint32>::max();
  corevm::types::int64 oprd2 = 64;

  corevm::types::bitwise_and::result_type expected_result = oprd1 & oprd2;

  call_typed_binary_op_and_assert_result<corevm::types::bitwise_and>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(bitwise_and_operator_unittest, TestWithIntegeralAndFloatingPointOperands)
{
  corevm::types::int64 oprd1 = 64;
  corevm::types::decimal oprd2 = 6.64f;

  corevm::types::bitwise_and::result_type expected_result =
    oprd1 & static_cast<corevm::types::int64>(oprd2);

  call_typed_binary_op_and_assert_result<corevm::types::bitwise_and>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

class bitwise_or_operator_unittest : public binary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(bitwise_or_operator_unittest, TestWithIntegeralOperands)
{
  corevm::types::uint32 oprd1 = std::numeric_limits<corevm::types::uint32>::max();
  corevm::types::int64 oprd2 = 64;

  corevm::types::bitwise_or::result_type expected_result = oprd1 | oprd2;

  call_typed_binary_op_and_assert_result<corevm::types::bitwise_or>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(bitwise_or_operator_unittest, TestWithIntegeralAndFloatingPointOperands)
{
  corevm::types::int64 oprd1 = 64;
  corevm::types::decimal oprd2 = 6.64f;

  corevm::types::bitwise_or::result_type expected_result =
    oprd1 | static_cast<corevm::types::int64>(oprd2);

  call_typed_binary_op_and_assert_result<corevm::types::bitwise_or>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

class bitwise_xor_operator_unittest : public binary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(bitwise_xor_operator_unittest, TestWithIntegeralOperands)
{
  corevm::types::uint32 oprd1 = std::numeric_limits<corevm::types::uint32>::max();
  corevm::types::int64 oprd2 = 64;

  corevm::types::bitwise_xor::result_type expected_result = oprd1 ^ oprd2;

  call_typed_binary_op_and_assert_result<corevm::types::bitwise_xor>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(bitwise_xor_operator_unittest, TestWithIntegeralAndFloatingPointOperands)
{
  corevm::types::int64 oprd1 = 64;
  corevm::types::decimal oprd2 = 6.64f;

  corevm::types::bitwise_xor::result_type expected_result =
    oprd1 ^ static_cast<corevm::types::int64>(oprd2);

  call_typed_binary_op_and_assert_result<corevm::types::bitwise_xor>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

class bitwise_left_shift_operator_unittest : public binary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(bitwise_left_shift_operator_unittest, TestWithIntegeralOperands)
{
  corevm::types::uint32 oprd1 = std::numeric_limits<corevm::types::uint32>::max();
  corevm::types::int64 oprd2 = 6;

  corevm::types::bitwise_left_shift::result_type expected_result =
    static_cast<corevm::types::int64>(oprd1) << oprd2;

  call_typed_binary_op_and_assert_result<corevm::types::bitwise_left_shift>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(bitwise_left_shift_operator_unittest, TestWithIntegeralAndFloatingPointOperands)
{
  corevm::types::int64 oprd1 = 64;
  corevm::types::decimal oprd2 = 6.64f;

  corevm::types::bitwise_left_shift::result_type expected_result =
    oprd1 << static_cast<corevm::types::int64>(oprd2);

  call_typed_binary_op_and_assert_result<corevm::types::bitwise_left_shift>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

class bitwise_right_shift_operator_unittest : public binary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(bitwise_right_shift_operator_unittest, TestWithIntegeralOperands)
{
  corevm::types::uint32 oprd1 = std::numeric_limits<corevm::types::uint32>::max();
  corevm::types::int64 oprd2 = 4;

  corevm::types::bitwise_right_shift::result_type expected_result = oprd1 >> oprd2;

  call_typed_binary_op_and_assert_result<corevm::types::bitwise_right_shift>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(bitwise_right_shift_operator_unittest, TestWithIntegeralAndFloatingPointOperands)
{
  corevm::types::int64 oprd1 = std::numeric_limits<corevm::types::int64>::max();
  corevm::types::decimal oprd2 = 6.64f;

  corevm::types::bitwise_right_shift::result_type expected_result =
    oprd1 >> static_cast<corevm::types::int64>(oprd2);

  call_typed_binary_op_and_assert_result<corevm::types::bitwise_right_shift>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

class logical_and_operator_unittest : public binary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(logical_and_operator_unittest, TestWithIntegeralOperands)
{
  corevm::types::uint32 oprd1 = std::numeric_limits<corevm::types::uint32>::max();
  corevm::types::int64 oprd2 = 4;

  corevm::types::logical_and::result_type expected_result = oprd1 && oprd2;

  call_typed_binary_op_and_assert_result<corevm::types::logical_and>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(logical_and_operator_unittest, TestWithIntegeralAndFloatingPointOperands)
{
  corevm::types::int64 oprd1 = std::numeric_limits<corevm::types::int64>::max();
  corevm::types::decimal oprd2 = 6.64f;

  corevm::types::logical_and::result_type expected_result = true;

  call_typed_binary_op_and_assert_result<corevm::types::logical_and>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

class logical_or_operator_unittest : public binary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(logical_or_operator_unittest, TestWithIntegeralOperands)
{
  corevm::types::uint32 oprd1 = std::numeric_limits<corevm::types::uint32>::max();
  corevm::types::int64 oprd2 = 4;

  corevm::types::logical_or::result_type expected_result = oprd1 || oprd2;

  call_typed_binary_op_and_assert_result<corevm::types::logical_or>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(logical_or_operator_unittest, TestWithIntegeralAndFloatingPointOperands)
{
  corevm::types::int64 oprd1 = std::numeric_limits<corevm::types::int64>::max();
  corevm::types::decimal oprd2 = 6.64f;

  corevm::types::logical_or::result_type expected_result = true;

  call_typed_binary_op_and_assert_result<corevm::types::logical_or>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

class eq_operator_unittest : public binary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(eq_operator_unittest, TestWithIntegeralOperands)
{
  corevm::types::uint32 oprd1 = std::numeric_limits<corevm::types::uint32>::max();
  corevm::types::int64 oprd2 = std::numeric_limits<corevm::types::uint32>::max();

  corevm::types::eq::result_type expected_result = oprd1 == oprd2;

  call_typed_binary_op_and_assert_result<corevm::types::eq>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(eq_operator_unittest, TestWithIntegeralAndFloatingPointOperands)
{
  corevm::types::int64 oprd1 = 6;
  corevm::types::decimal oprd2 = 6.64f;

  corevm::types::eq::result_type expected_result =
    oprd1 == static_cast<corevm::types::int64>(oprd2);

  call_typed_binary_op_and_assert_result<corevm::types::eq>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

class neq_operator_unittest : public binary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(neq_operator_unittest, TestWithIntegeralOperands)
{
  corevm::types::uint32 oprd1 = std::numeric_limits<corevm::types::uint32>::max();
  corevm::types::int64 oprd2 = std::numeric_limits<corevm::types::uint32>::max();

  corevm::types::neq::result_type expected_result = oprd1 != oprd2;

  call_typed_binary_op_and_assert_result<corevm::types::neq>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(neq_operator_unittest, TestWithIntegeralAndFloatingPointOperands)
{
  corevm::types::int64 oprd1 = 6;
  corevm::types::decimal oprd2 = 6.64f;

  corevm::types::neq::result_type expected_result = oprd1 !=
    static_cast<corevm::types::int64>(oprd2);

  call_typed_binary_op_and_assert_result<corevm::types::neq>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

class gt_operator_unittest : public binary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(gt_operator_unittest, TestWithIntegeralOperands)
{
  corevm::types::uint32 oprd1 = std::numeric_limits<corevm::types::uint32>::max();
  corevm::types::int64 oprd2 = std::numeric_limits<corevm::types::uint32>::max();

  corevm::types::gt::result_type expected_result = oprd1 > oprd2;

  call_typed_binary_op_and_assert_result<corevm::types::gt>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(gt_operator_unittest, TestWithIntegeralAndFloatingPointOperands)
{
  corevm::types::int64 oprd1 = 6;
  corevm::types::decimal oprd2 = 6.64f;

  corevm::types::gt::result_type expected_result = oprd1 >
    static_cast<corevm::types::int64>(oprd2);

  call_typed_binary_op_and_assert_result<corevm::types::gt>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

class lt_operator_unittest : public binary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(lt_operator_unittest, TestWithIntegeralOperands)
{
  corevm::types::uint32 oprd1 = std::numeric_limits<corevm::types::uint32>::max();
  corevm::types::int64 oprd2 = std::numeric_limits<corevm::types::uint32>::max();

  corevm::types::lt::result_type expected_result = oprd1 < oprd2;

  call_typed_binary_op_and_assert_result<corevm::types::lt>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(lt_operator_unittest, TestWithIntegeralAndFloatingPointOperands)
{
  corevm::types::int64 oprd1 = 6;
  corevm::types::decimal oprd2 = 6.64f;

  corevm::types::lt::result_type expected_result = oprd1 <
    static_cast<corevm::types::int64>(oprd2);

  call_typed_binary_op_and_assert_result<corevm::types::lt>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

class gte_operator_unittest : public binary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(gte_operator_unittest, TestWithIntegeralOperands)
{
  corevm::types::uint32 oprd1 = std::numeric_limits<corevm::types::uint32>::max();
  corevm::types::int64 oprd2 = std::numeric_limits<corevm::types::uint32>::max();

  corevm::types::gte::result_type expected_result = oprd1 >= oprd2;

  call_typed_binary_op_and_assert_result<corevm::types::gte>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(gte_operator_unittest, TestWithIntegeralAndFloatingPointOperands)
{
  corevm::types::int64 oprd1 = 6;
  corevm::types::decimal oprd2 = 6.64f;

  corevm::types::gte::result_type expected_result = oprd1 >=
    static_cast<corevm::types::int64>(oprd2);

  call_typed_binary_op_and_assert_result<corevm::types::gte>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

class lte_operator_unittest : public binary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(lte_operator_unittest, TestWithIntegeralOperands)
{
  corevm::types::uint32 oprd1 = std::numeric_limits<corevm::types::uint32>::max();
  corevm::types::int64 oprd2 = std::numeric_limits<corevm::types::uint32>::max();

  corevm::types::lte::result_type expected_result = oprd1 <= oprd2;

  call_typed_binary_op_and_assert_result<corevm::types::lte>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(lte_operator_unittest, TestWithIntegeralAndFloatingPointOperands)
{
  corevm::types::int64 oprd1 = 6;
  corevm::types::decimal oprd2 = 6.64f;

  corevm::types::lte::result_type expected_result = oprd1 <=
    static_cast<corevm::types::int64>(oprd2);

  call_typed_binary_op_and_assert_result<corevm::types::lte>(
    oprd1, oprd2, expected_result);
}

// -----------------------------------------------------------------------------
