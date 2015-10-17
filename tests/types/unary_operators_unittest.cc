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

#include <sneaker/testing/_unittest.h>

#include <cmath>
#include <cstdlib>
#include <limits>
#include <type_traits>


// -----------------------------------------------------------------------------

class unary_operators_unittest_base : public ::testing::Test
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

class positive_operator_unittest : public unary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(positive_operator_unittest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = std::numeric_limits<corevm::types::int64>::max();

  corevm::types::int64 expected_result = +oprd;

  call_unary_op_and_assert_result<
    corevm::types::positive, corevm::types::int64>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(positive_operator_unittest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = 3.1415;

  corevm::types::decimal expected_result = +oprd;

  call_unary_op_and_assert_result<
    corevm::types::positive, corevm::types::decimal>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

class negation_operator_unittest : public unary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(negation_operator_unittest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = std::numeric_limits<corevm::types::int64>::max();

  corevm::types::int64 expected_result = -oprd;

  call_unary_op_and_assert_result<
    corevm::types::negation, corevm::types::int64>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(negation_operator_unittest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = 3.1415;

  corevm::types::decimal expected_result = -oprd;

  call_unary_op_and_assert_result<
    corevm::types::negation, corevm::types::decimal>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

class increment_operator_unittest : public unary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(increment_operator_unittest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = std::numeric_limits<corevm::types::int64>::max();

  corevm::types::int64 expected_result = oprd + 1;

  call_unary_op_and_assert_result<
    corevm::types::increment, corevm::types::int64>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(increment_operator_unittest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = 3.1415;

  corevm::types::decimal expected_result = oprd + 1;

  call_unary_op_and_assert_result<
    corevm::types::increment, corevm::types::decimal>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

class decrement_operator_unittest : public unary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(decrement_operator_unittest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = std::numeric_limits<corevm::types::int64>::max();

  corevm::types::int64 expected_result = oprd - 1;

  call_unary_op_and_assert_result<
    corevm::types::decrement, corevm::types::int64>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(decrement_operator_unittest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = 3.1415;

  corevm::types::decimal expected_result = oprd - 1;

  call_unary_op_and_assert_result<
    corevm::types::decrement, corevm::types::decimal>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

class bitwise_not_operator_unittest : public unary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(bitwise_not_operator_unittest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = std::numeric_limits<corevm::types::int64>::max();

  corevm::types::int64 expected_result = ~oprd;

  call_unary_op_and_assert_result<
    corevm::types::bitwise_not, corevm::types::int64>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

class reverse_operator_unittest : public unary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(reverse_operator_unittest, TestWithStringOperand)
{
  corevm::types::string oprd("hello");
  corevm::types::string expected_result("olleh");

  call_unary_op_and_assert_result<
    corevm::types::reverse, corevm::types::string>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(reverse_operator_unittest, TestWithArrayOperand)
{
  corevm::types::array oprd { 1, 2, 3 };
  corevm::types::array expected_result { 3, 2, 1 };

  call_unary_op_and_assert_result<
    corevm::types::reverse, corevm::types::array>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

class absolute_operator_unittest : public unary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(absolute_operator_unittest, TestWithIntegeralOperand)
{
  corevm::types::int32 oprd = -std::numeric_limits<corevm::types::int32>::max();

  corevm::types::int32 expected_result = std::abs(oprd);

  call_unary_op_and_assert_result<
    corevm::types::absolute, corevm::types::int32>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(absolute_operator_unittest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = -3.1415;

  corevm::types::decimal expected_result = std::abs(oprd);

  call_unary_op_and_assert_result<
    corevm::types::absolute, corevm::types::decimal>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

class sqrt_operator_unittest : public unary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(sqrt_operator_unittest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = std::numeric_limits<corevm::types::int64>::max();

  corevm::types::int64 expected_result = sqrt(oprd);

  call_unary_op_and_assert_result<
    corevm::types::sqrt, corevm::types::int64>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(sqrt_operator_unittest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = 3.1415;

  corevm::types::decimal expected_result = sqrt(oprd);

  call_unary_op_and_assert_result<
    corevm::types::sqrt, corevm::types::decimal>(oprd, expected_result);
}

// -----------------------------------------------------------------------------

class repr_operator_unittest : public unary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(repr_operator_unittest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = 123456789;

  corevm::types::string expected_result("123456789");

  call_typed_unary_op_and_assert_result<corevm::types::repr>(
    oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(repr_operator_unittest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = 3.141593;

  corevm::types::string expected_result("3.141593");

  call_typed_unary_op_and_assert_result<corevm::types::repr>(
    oprd, expected_result);
}

// -----------------------------------------------------------------------------

class logical_not_operator_unittest : public unary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(logical_not_operator_unittest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = 123456789;

  corevm::types::boolean expected_result = false;

  call_typed_unary_op_and_assert_result<corevm::types::logical_not>(
    oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(logical_not_operator_unittest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = 3.1415926;

  corevm::types::boolean expected_result = false;

  call_typed_unary_op_and_assert_result<corevm::types::logical_not>(
    oprd, expected_result);
}

// -----------------------------------------------------------------------------

class truthy_operator_unittest : public unary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(truthy_operator_unittest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = 123456789;

  corevm::types::boolean expected_result = true;

  call_typed_unary_op_and_assert_result<corevm::types::truthy>(
    oprd, expected_result);
}

// -----------------------------------------------------------------------------

TEST_F(truthy_operator_unittest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = 0.1415926;

  corevm::types::boolean expected_result = true;

  call_typed_unary_op_and_assert_result<corevm::types::truthy>(
    oprd, expected_result);
}

// -----------------------------------------------------------------------------

class hash_operator_unittest : public unary_operators_unittest_base {};

// -----------------------------------------------------------------------------

TEST_F(hash_operator_unittest, TestWithIntegeralOperand)
{
  corevm::types::int64 oprd = 123456789;

  ASSERT_NE(corevm::types::hash()(oprd), 0u);
}

// -----------------------------------------------------------------------------

TEST_F(hash_operator_unittest, TestWithFloatingPointOperand)
{
  corevm::types::decimal oprd = 0.1415926;

  ASSERT_NE(corevm::types::hash()(oprd), 0u);
}

// -----------------------------------------------------------------------------
