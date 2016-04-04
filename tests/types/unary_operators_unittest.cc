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
#include <cstdlib>
#include <limits>
#include <type_traits>


// -----------------------------------------------------------------------------

class UnaryOperatorsUnitTestBase : public ::testing::Test
{
protected:
  template<class op, class ResultType, class T>
  void call_unary_op_and_assert_result(
    const T oprd, const ResultType expected_result) const
  {
    const ResultType actual_result = op().template operator()(oprd);

    if (std::is_floating_point<ResultType>::value)
    {
      ASSERT_FLOAT_EQ(expected_result, actual_result);
    }
    else
    {
      ASSERT_EQ(expected_result, actual_result);
    }
  }

  template<class op, class T>
  void call_typed_unary_op_and_assert_result(
    const T oprd, const typename op::result_type expected_result) const
  {
    call_unary_op_and_assert_result<op, typename op::result_type>(oprd, expected_result);
  }
};

// -----------------------------------------------------------------------------

class PositiveOperatorUnitTest : public UnaryOperatorsUnitTestBase {};

// -----------------------------------------------------------------------------

TEST_F(PositiveOperatorUnitTest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = std::numeric_limits<corevm::types::int64>::max();

  corevm::types::int64 expected_result = +oprd;

  call_unary_op_and_assert_result<
    corevm::types::positive, corevm::types::int64>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(PositiveOperatorUnitTest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = 3.1415f;

  corevm::types::decimal expected_result = +oprd;

  call_unary_op_and_assert_result<
    corevm::types::positive, corevm::types::decimal>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

class NegationOperatorUnitTest : public UnaryOperatorsUnitTestBase {};

// -----------------------------------------------------------------------------

TEST_F(NegationOperatorUnitTest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = std::numeric_limits<corevm::types::int64>::max();

  corevm::types::int64 expected_result = -oprd;

  call_unary_op_and_assert_result<
    corevm::types::negation, corevm::types::int64>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(NegationOperatorUnitTest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = 3.1415f;

  corevm::types::decimal expected_result = -oprd;

  call_unary_op_and_assert_result<
    corevm::types::negation, corevm::types::decimal>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

class IncrementOperatorUnitTest : public UnaryOperatorsUnitTestBase {};

// -----------------------------------------------------------------------------

TEST_F(IncrementOperatorUnitTest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = std::numeric_limits<corevm::types::int64>::max();

  corevm::types::int64 expected_result = oprd + 1;

  call_unary_op_and_assert_result<
    corevm::types::increment, corevm::types::int64>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(IncrementOperatorUnitTest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = 3.1415f;

  corevm::types::decimal expected_result = oprd + 1;

  call_unary_op_and_assert_result<
    corevm::types::increment, corevm::types::decimal>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

class DecrementOperatorUnitTest : public UnaryOperatorsUnitTestBase {};

// -----------------------------------------------------------------------------

TEST_F(DecrementOperatorUnitTest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = std::numeric_limits<corevm::types::int64>::max();

  corevm::types::int64 expected_result = oprd - 1;

  call_unary_op_and_assert_result<
    corevm::types::decrement, corevm::types::int64>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(DecrementOperatorUnitTest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = 3.1415f;

  corevm::types::decimal expected_result = oprd - 1;

  call_unary_op_and_assert_result<
    corevm::types::decrement, corevm::types::decimal>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

class BitwiseNotOperatorUnitTest : public UnaryOperatorsUnitTestBase {};

// -----------------------------------------------------------------------------

TEST_F(BitwiseNotOperatorUnitTest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = std::numeric_limits<corevm::types::int64>::max();

  corevm::types::int64 expected_result = ~oprd;

  call_unary_op_and_assert_result<
    corevm::types::bitwise_not, corevm::types::int64>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

class ReverseOperatorUnitTest : public UnaryOperatorsUnitTestBase {};

// -----------------------------------------------------------------------------

TEST_F(ReverseOperatorUnitTest, TestWithStringOperand)
{
  corevm::types::string oprd("hello");
  corevm::types::string expected_result("olleh");

  call_unary_op_and_assert_result<
    corevm::types::reverse, corevm::types::string>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(ReverseOperatorUnitTest, TestWithArrayOperand)
{
  corevm::types::array oprd { 1, 2, 3 };
  corevm::types::array expected_result { 3, 2, 1 };

  call_unary_op_and_assert_result<
    corevm::types::reverse, corevm::types::array>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

class AbsoluteOperatorUnitTest : public UnaryOperatorsUnitTestBase {};

// -----------------------------------------------------------------------------

TEST_F(AbsoluteOperatorUnitTest, TestWithIntegeralOperand)
{
  corevm::types::int32 oprd = -std::numeric_limits<corevm::types::int32>::max();

  corevm::types::int32 expected_result = std::abs(oprd);

  call_unary_op_and_assert_result<
    corevm::types::absolute, corevm::types::int32>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(AbsoluteOperatorUnitTest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = -3.1415f;

  corevm::types::decimal expected_result = std::abs(oprd);

  call_unary_op_and_assert_result<
    corevm::types::absolute, corevm::types::decimal>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

class sqrt_operator_unittest : public UnaryOperatorsUnitTestBase {};

// -----------------------------------------------------------------------------

TEST_F(sqrt_operator_unittest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = std::numeric_limits<corevm::types::int64>::max();

  // TODO: investigate use version of `sqrt` that returns int64_t.
  corevm::types::int64 expected_result = static_cast<corevm::types::int64>(sqrt(oprd));

  call_unary_op_and_assert_result<
    corevm::types::sqrt, corevm::types::int64>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(sqrt_operator_unittest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = 3.1415f;

  // TODO: investigate use version of `sqrt` that returns float.
  corevm::types::decimal expected_result = static_cast<corevm::types::decimal>(sqrt(oprd));

  call_unary_op_and_assert_result<
    corevm::types::sqrt, corevm::types::decimal>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

class ReprOperatorUnitTest : public UnaryOperatorsUnitTestBase {};

// -----------------------------------------------------------------------------

TEST_F(ReprOperatorUnitTest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = 123456789;

  corevm::types::string expected_result("123456789");

  call_typed_unary_op_and_assert_result<corevm::types::repr>(
    oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(ReprOperatorUnitTest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = 3.141593f;

  corevm::types::string expected_result("3.141593");

  call_typed_unary_op_and_assert_result<corevm::types::repr>(
    oprd, expected_result);
}

// -----------------------------------------------------------------------------

class LogicalNotOperatorUnitTest : public UnaryOperatorsUnitTestBase {};

// -----------------------------------------------------------------------------

TEST_F(LogicalNotOperatorUnitTest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = 123456789;

  corevm::types::boolean expected_result = false;

  call_typed_unary_op_and_assert_result<corevm::types::logical_not>(
    oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(LogicalNotOperatorUnitTest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = 3.1415926f;

  corevm::types::boolean expected_result = false;

  call_typed_unary_op_and_assert_result<corevm::types::logical_not>(
    oprd, expected_result);
}

// -----------------------------------------------------------------------------

class TruthyOperatorUnitTest : public UnaryOperatorsUnitTestBase {};

// -----------------------------------------------------------------------------

TEST_F(TruthyOperatorUnitTest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = 123456789;

  corevm::types::boolean expected_result = true;

  call_typed_unary_op_and_assert_result<corevm::types::truthy>(
    oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(TruthyOperatorUnitTest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = 0.1415926f;

  corevm::types::boolean expected_result = true;

  call_typed_unary_op_and_assert_result<corevm::types::truthy>(
    oprd, expected_result);
}

// -----------------------------------------------------------------------------

class HashOperatorUnitTest : public UnaryOperatorsUnitTestBase {};

// -----------------------------------------------------------------------------

TEST_F(HashOperatorUnitTest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = 123456789;

  ASSERT_NE(corevm::types::hash()(oprd), 0u);
}

// -----------------------------------------------------------------------------

TEST_F(HashOperatorUnitTest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = 0.1415926f;

  ASSERT_NE(corevm::types::hash()(oprd), 0u);
}

// -----------------------------------------------------------------------------
