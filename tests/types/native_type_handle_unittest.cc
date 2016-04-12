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
#include "types/native_type_handle.h"

#include <gtest/gtest.h>

#include <cmath>
#include <cstdint>


class NativeTypeHandleUnitTest : public ::testing::Test
{
public:
  template<typename T>
  void assert_handle_value(
    corevm::types::NativeTypeHandle& handle,
    T expected_value,
    bool decimal_value=false)
  {
    T actual_value = corevm::types::get_value_from_handle<T>(handle);

    if (!decimal_value)
    {
      ASSERT_EQ(expected_value, actual_value);
    }
    else
    {
      ASSERT_FLOAT_EQ(expected_value, actual_value);
    }
  }

  template<typename VisitorType, typename T>
  void apply_unary_visitor_and_check_result(
    corevm::types::NativeTypeHandle& handle,
    T expected_value,
    bool decimal_value=false,
    bool on_self=false)
  {
    typename corevm::types::NativeTypeHandle result =
      corevm::types::apply_unary_visitor<VisitorType>(handle);

    if (on_self)
    {
      assert_handle_value(handle, expected_value, decimal_value);
    }
    else
    {
      assert_handle_value(result, expected_value, decimal_value);
    }
  }

  template<typename VisitorType, typename T>
  void apply_binary_visitor_and_check_result(
    corevm::types::NativeTypeHandle& h1,
    corevm::types::NativeTypeHandle& h2,
    T expected_value,
    bool decimal_value=false)
  {
    typename corevm::types::NativeTypeHandle result =
      corevm::types::apply_binary_visitor<VisitorType>(h1, h2);

    assert_handle_value(result, expected_value, decimal_value);
  }

  template<class VisitorType, class ExceptionType>
  void apply_unary_visitor_and_check_exception(
    corevm::types::NativeTypeHandle& handle)
  {
    ASSERT_THROW(
      {
        corevm::types::apply_unary_visitor<VisitorType>(handle);
      },
      ExceptionType
    );
  }

  template<class VisitorType, class ExceptionType>
  void apply_binary_visitor_and_check_exception(
    corevm::types::NativeTypeHandle& lhs, corevm::types::NativeTypeHandle& rhs)
  {
    ASSERT_THROW(
      {
        corevm::types::apply_binary_visitor<VisitorType>(lhs, rhs);
      },
      ExceptionType
    );
  }
};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleUnitTest, TestGetValueFromHandle)
{
  corevm::types::native_string str = "Hello world";
  corevm::types::NativeTypeHandle hndl = str;

  corevm::types::native_string actual_string =
    corevm::types::get_value_from_handle<corevm::types::native_string>(hndl);

  ASSERT_EQ(actual_string, str);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleUnitTest, TestGetValueFromHandleWithInvalidType)
{
  corevm::types::native_string str = "Hello world";
  corevm::types::NativeTypeHandle hndl = str;

  ASSERT_THROW(
    {
      corevm::types::get_value_from_handle<corevm::types::native_map>(hndl);
    },
    corevm::types::ConversionError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleUnitTest, TestGetValueFromHandleWithCompatibleType)
{
  corevm::types::uint64 value = 10;
  corevm::types::NativeTypeHandle hndl = value;

  bool actual_value = corevm::types::get_value_from_handle<bool>(hndl);

  ASSERT_EQ(static_cast<bool>(value), actual_value);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleUnaryOperatorUnitTest : public NativeTypeHandleUnitTest {};

// -----------------------------------------------------------------------------

class NativeTypeHandlePositiveUnitTest : public NativeTypeHandleUnaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandlePositiveUnitTest, TestOnIntegerType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(-1);

  int8_t expected_value = -1;

  apply_unary_visitor_and_check_result<corevm::types::native_type_positive_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandlePositiveUnitTest, TestOnDecimalType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(-3.14);

  double expected_value = -3.14;

  apply_unary_visitor_and_check_result<corevm::types::native_type_positive_visitor>(
    h1,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandlePositiveUnitTest, TestOnBooleanType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(false);

  const bool expected_value = false;

  apply_unary_visitor_and_check_result<corevm::types::native_type_positive_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandlePositiveUnitTest, TestOnStringType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_positive_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandlePositiveUnitTest, TestOnArrayType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_positive_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandlePositiveUnitTest, TestOnMapType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_positive_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleNegationUnitTest : public NativeTypeHandleUnaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleNegationUnitTest, TestOnIntegerType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(-1);

  int8_t expected_value = -(-1);

  apply_unary_visitor_and_check_result<corevm::types::native_type_negation_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleNegationUnitTest, TestOnDecimalType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(-3.14);

  double expected_value = -(-3.14);

  apply_unary_visitor_and_check_result<corevm::types::native_type_negation_visitor>(
    h1,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleNegationUnitTest, TestOnBooleanType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(true);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(false);

  const bool expected_value = true;

  apply_unary_visitor_and_check_result<corevm::types::native_type_negation_visitor>(
    h1,
    expected_value
  );

  const bool expected_value2 = false;

  apply_unary_visitor_and_check_result<corevm::types::native_type_negation_visitor>(
    h2,
    expected_value2
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleNegationUnitTest, TestOnStringType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_negation_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleNegationUnitTest, TestOnArrayType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_negation_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleNegationUnitTest, TestOnMapType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_negation_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleIncrementUnitTest : public NativeTypeHandleUnaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleIncrementUnitTest, TestOnIntegerType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);

  int8_t expected_value = 2;

  apply_unary_visitor_and_check_result<corevm::types::native_type_increment_visitor>(
    h1,
    expected_value,
    false,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleIncrementUnitTest, TestOnDecimalType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(3.14);

  double expected_value = 4.14;

  apply_unary_visitor_and_check_result<corevm::types::native_type_increment_visitor>(
    h1,
    expected_value,
    true,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleIncrementUnitTest, TestOnBooleanType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(true);

  const bool expected_value = true;

  apply_unary_visitor_and_check_result<corevm::types::native_type_increment_visitor>(
    h1,
    expected_value,
    false,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleIncrementUnitTest, TestOnStringType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_increment_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleIncrementUnitTest, TestOnArrayType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_increment_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleIncrementUnitTest, TestOnMapType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_increment_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleDecrementUnitTest : public NativeTypeHandleUnaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleDecrementUnitTest, TestOnIntegerType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(-10);

  int8_t expected_value = -11;

  apply_unary_visitor_and_check_result<corevm::types::native_type_decrement_visitor>(
    h1,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleDecrementUnitTest, TestOnDecimalType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(3.14);

  double expected_value = 2.14;

  apply_unary_visitor_and_check_result<corevm::types::native_type_decrement_visitor>(
    h1,
    expected_value,
    true,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleDecrementUnitTest, TestOnBooleanType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(true);

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_decrement_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleDecrementUnitTest, TestOnStringType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_decrement_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleDecrementUnitTest, TestOnArrayType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_decrement_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleDecrementUnitTest, TestOnMapType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_decrement_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleLogicalNotUnitTest : public NativeTypeHandleUnaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalNotUnitTest, TestOnIntegerType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(99);

  int8_t expected_value = 0;

  apply_unary_visitor_and_check_result<corevm::types::native_type_logical_not_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalNotUnitTest, TestOnDecimalType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(3.14);

  double expected_value = 0.0;

  apply_unary_visitor_and_check_result<corevm::types::native_type_logical_not_visitor>(
    h1,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalNotUnitTest, TestOnBooleanType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(true);

  const bool expected_value = false;

  apply_unary_visitor_and_check_result<corevm::types::native_type_logical_not_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalNotUnitTest, TestOnStringType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_logical_not_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalNotUnitTest, TestOnArrayType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_logical_not_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalNotUnitTest, TestOnMapType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_logical_not_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleBitwiseNotUnitTest : public NativeTypeHandleUnaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseNotUnitTest, TestOnIntegerType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(9);

  int8_t expected_value = ~9;

  apply_unary_visitor_and_check_result<corevm::types::native_type_bitwise_not_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseNotUnitTest, TestOnDecimalType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(3.14);

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_not_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseNotUnitTest, TestOnBooleanType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(true);

  const bool expected_value = true;

  apply_unary_visitor_and_check_result<corevm::types::native_type_bitwise_not_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseNotUnitTest, TestOnStringType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_not_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseNotUnitTest, TestOnArrayType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_not_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseNotUnitTest, TestOnMapType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_not_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleAbsUnitTest : public NativeTypeHandleUnaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleAbsUnitTest, TestOnIntegerType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(-9);

  int8_t expected_value = 9;

  apply_unary_visitor_and_check_result<corevm::types::native_type_abs_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleAbsUnitTest, TestOnDecimalType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(-3.1415926);

  double expected_value = 3.1415926;

  apply_unary_visitor_and_check_result<corevm::types::native_type_abs_visitor>(
    h1,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleAbsUnitTest, TestOnBooleanType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(true);

  const bool expected_value = true;

  apply_unary_visitor_and_check_result<corevm::types::native_type_abs_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleAbsUnitTest, TestOnStringType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_abs_visitor, corevm::types::ConversionError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleAbsUnitTest, TestOnArrayType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_abs_visitor, corevm::types::ConversionError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleAbsUnitTest, TestOnMapType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_abs_visitor, corevm::types::ConversionError>(h1);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleSqrtUnitTest : public NativeTypeHandleUnaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleSqrtUnitTest, TestOnIntegerType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(9);

  int8_t expected_value = 3;

  apply_unary_visitor_and_check_result<corevm::types::native_type_sqrt_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleSqrtUnitTest, TestOnDecimalType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(9.869600);

  double expected_value = 3.1415926;

  apply_unary_visitor_and_check_result<corevm::types::native_type_sqrt_visitor>(
    h1,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleSqrtUnitTest, TestOnBooleanType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(true);

  const bool expected_value = true;

  apply_unary_visitor_and_check_result<corevm::types::native_type_sqrt_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleSqrtUnitTest, TestOnStringType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_sqrt_visitor, corevm::types::ConversionError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleSqrtUnitTest, TestOnArrayType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_sqrt_visitor, corevm::types::ConversionError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleSqrtUnitTest, TestOnMapType)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_sqrt_visitor, corevm::types::ConversionError>(h1);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleBinaryOperatorUnitTest : public NativeTypeHandleUnitTest {};

// -----------------------------------------------------------------------------

class NativeTypeHandleAdditionUnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleAdditionUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::uint8(2);

  uint8_t expected_value = 3;

  apply_binary_visitor_and_check_result<corevm::types::native_type_addition_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleAdditionUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(3.14159);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(2.22);

  const double expected_value = 3.14159 + 2.22;

  apply_binary_visitor_and_check_result<corevm::types::native_type_addition_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleAdditionUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(true);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(false);

  const bool expected_value = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_addition_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleAdditionUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello ");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("world!!!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_addition_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleAdditionUnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_addition_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleAdditionUnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_addition_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleAdditionUnitTest, TestAddtionBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(3.14);

  double expected_value = 4.14;

  apply_binary_visitor_and_check_result<corevm::types::native_type_addition_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  apply_binary_visitor_and_check_result<corevm::types::native_type_addition_visitor>(
    h2,
    h1,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleAdditionUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_addition_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleSubtractionUnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleSubtractionUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint8(100);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::uint8(12);

  uint8_t expected_value = 100 - 12; // I'm too lazy to calculate that...

  apply_binary_visitor_and_check_result<corevm::types::native_type_subtraction_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleSubtractionUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(3.14159);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(2.22);

  const double expected_value = 3.14159 - 2.22;

  apply_binary_visitor_and_check_result<corevm::types::native_type_subtraction_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleSubtractionUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(true);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(false);

  const bool expected_value = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_subtraction_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleSubtractionUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_subtraction_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleSubtractionUnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_subtraction_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleSubtractionUnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_subtraction_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleSubtractionUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int8(1);

  double expected_value = 2.14;

  apply_binary_visitor_and_check_result<corevm::types::native_type_subtraction_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  double expected_value2 = -2.14;

  apply_binary_visitor_and_check_result<corevm::types::native_type_subtraction_visitor>(
    h2,
    h1,
    expected_value2,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleSubtractionUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_subtraction_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleMultiplicationUnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleMultiplicationUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint16(33);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int32(-36);

  int32_t expected_value = 33 * (-36);

  apply_binary_visitor_and_check_result<corevm::types::native_type_multiplication_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleMultiplicationUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(3.14159);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(2.22);

  const double expected_value = 3.14159 * 2.22;

  apply_binary_visitor_and_check_result<corevm::types::native_type_multiplication_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleMultiplicationUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(true);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(false);

  const bool expected_value = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_multiplication_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleMultiplicationUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_multiplication_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleMultiplicationUnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_multiplication_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleMultiplicationUnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_multiplication_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleMultiplicationUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int8(2);

  double expected_value = 3.14 * 2;

  apply_binary_visitor_and_check_result<corevm::types::native_type_multiplication_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  double expected_value2 = 2 * 3.14;

  apply_binary_visitor_and_check_result<corevm::types::native_type_multiplication_visitor>(
    h2,
    h1,
    expected_value2,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleMultiplicationUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_multiplication_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleDivisionUnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleDivisionUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int32(-10);

  int32_t expected_value = 100 / (-10);

  apply_binary_visitor_and_check_result<corevm::types::native_type_division_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleDivisionUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(2.0);

  const double expected_value = 6.12 / 2.0;

  apply_binary_visitor_and_check_result<corevm::types::native_type_division_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleDivisionUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(true);

  const bool expected_value = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_division_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleDivisionUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_division_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleDivisionUnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_division_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleDivisionUnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_division_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleDivisionUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int8(2);

  double expected_value = 3.14 / 2;

  apply_binary_visitor_and_check_result<corevm::types::native_type_division_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  double expected_value2 = 2 / 3.14;

  apply_binary_visitor_and_check_result<corevm::types::native_type_division_visitor>(
    h2,
    h1,
    expected_value2,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleDivisionUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_division_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleModulusUnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleModulusUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int32(-10);

  int32_t expected_value = 100 % (-10);

  apply_binary_visitor_and_check_result<corevm::types::native_type_modulus_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleModulusUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 6 % 2;

  apply_binary_visitor_and_check_result<corevm::types::native_type_modulus_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleModulusUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(true);

  const int64_t expected_value = 0 % 1;

  apply_binary_visitor_and_check_result<corevm::types::native_type_modulus_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleModulusUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_modulus_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleModulusUnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_modulus_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleModulusUnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_modulus_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleModulusUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int8(2);

  int64_t expected_value = 3 % 2;

  apply_binary_visitor_and_check_result<corevm::types::native_type_modulus_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  int64_t expected_value2 = 2 % 3;

  apply_binary_visitor_and_check_result<corevm::types::native_type_modulus_visitor>(
    h2,
    h1,
    expected_value2,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleModulusUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_modulus_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeHandlePowUnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandlePowUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint16(2);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::uint32(4);

  double expected_value = pow(2, 4);

  apply_binary_visitor_and_check_result<corevm::types::native_type_pow_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandlePowUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(6);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(-1.2);

  const double expected_value = pow(6, -1.2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_pow_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandlePowUnitTest, TestBetweenIntegerAndDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint32(6);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(-1.2);

  const double expected_value = pow(6u, -1.2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_pow_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}
// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandlePowUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(true);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(false);

  const double expected_value = pow(1, 0);

  apply_binary_visitor_and_check_result<corevm::types::native_type_pow_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandlePowUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_pow_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandlePowUnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_pow_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandlePowUnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_pow_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandlePowUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int8(2);

  double expected_value = pow(3.14, 2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_pow_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  double expected_value2 = pow(2, 3.14);

  apply_binary_visitor_and_check_result<corevm::types::native_type_pow_visitor>(
    h2,
    h1,
    expected_value2,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandlePowUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_pow_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleLogicalAND_UnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalAND_UnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int32(-10);

  int32_t expected_value = 1; // (100) && (-10)

  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_and_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalAND_UnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 1; // (6.12) && (2.0)

  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_and_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalAND_UnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(true);

  const int64_t expected_value =  0; // 0 && 1

  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_and_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalAND_UnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_and_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalAND_UnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_and_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalAND_UnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_and_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalAND_UnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int8(2);

  int64_t expected_value = 1; // 3.14 && 2

  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_and_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  int64_t expected_value2 = 1; // 2 && 3.14

  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_and_visitor>(
    h2, h1, expected_value2, true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalAND_UnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_and_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleLogicalOR_UnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalOR_UnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int32(-10);

  int32_t expected_value = 1; // (100) || (-10)

  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_or_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalOR_UnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 1.0; // (6.12) || (2.0)

  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_or_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalOR_UnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(true);

  const int64_t expected_value = 1; // 0 || 1

  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_or_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalOR_UnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_or_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalOR_UnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_or_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalOR_UnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_or_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalOR_UnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int8(2);

  int64_t expected_value = 1; // 3.14 || 2

  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_or_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  int64_t expected_value2 = 1; // 2 || 3.14

  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_or_visitor>(
    h2,
    h1,
    expected_value2,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLogicalOR_UnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_or_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleBitwiseAND_UnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseAND_UnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int32(-10);

  int32_t expected_value = (100) & (-10);

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_and_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseAND_UnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6) & (2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_and_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseAND_UnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(true);

  const int64_t expected_value = (0) & (1);

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_and_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseAND_UnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_and_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseAND_UnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_and_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseAND_UnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_and_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseAND_UnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int8(2);

  int64_t expected_value = 3 & 2;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_and_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  int64_t expected_value2 = 2 & 3;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_and_visitor>(
    h2,
    h1,
    expected_value2,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseAND_UnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_and_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleBitwiseOR_UnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseOR_UnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int32(-10);

  int32_t expected_value = (100) | (-10);

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_or_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseOR_UnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 6 | 2;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_or_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseOR_UnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(true);

  const int64_t expected_value = 0 | 1;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_or_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseOR_UnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_or_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseOR_UnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_or_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseOR_UnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_or_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseOR_UnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int8(2);

  int64_t expected_value = 3 | 2;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_or_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  int64_t expected_value2 = 2 | 3;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_or_visitor>(
    h2,
    h1,
    expected_value2,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseOR_UnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_or_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleBitwiseXOR_UnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseXOR_UnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int32(-10);

  int32_t expected_value = (100) ^ (-10);

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_xor_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseXOR_UnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 6 ^ 2;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_xor_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseXOR_UnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(true);

  const int64_t expected_value = false ^ true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_xor_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseXOR_UnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_xor_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseXOR_UnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_xor_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseXOR_UnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_xor_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseXOR_UnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int8(2);

  int64_t expected_value = 3 ^ 2;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_xor_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  int64_t expected_value2 = 2 ^ 3;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_xor_visitor>(
    h2,
    h1,
    expected_value2,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseXOR_UnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_xor_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleBitwiseLeftShiftUnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseLeftShiftUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int32(2);

  int32_t expected_value = (100) << (2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_left_shift_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseLeftShiftUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 6 << 2;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_left_shift_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseLeftShiftUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(true);

  const int64_t expected_value = false << true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_left_shift_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseLeftShiftUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_left_shift_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseLeftShiftUnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_left_shift_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseLeftShiftUnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_left_shift_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseLeftShiftUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int8(2);

  int64_t expected_value = 3 << 2;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_left_shift_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  int64_t expected_value2 = 2 << 3;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_left_shift_visitor>(
    h2,
    h1,
    expected_value2,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseLeftShiftUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_left_shift_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleBitwiseRightShiftUnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseRightShiftUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int32(2);

  int32_t expected_value = (100) >> (2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_right_shift_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseRightShiftUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 6 >> 2;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_right_shift_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseRightShiftUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(true);

  const int64_t expected_value = false >> true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_right_shift_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseRightShiftUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_right_shift_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseRightShiftUnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_right_shift_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseRightShiftUnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_right_shift_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseRightShiftUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int8(2);

  int64_t expected_value = 3 >> 2;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_right_shift_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  int64_t expected_value2 = 2 >> 3;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_right_shift_visitor>(
    h2,
    h1,
    expected_value2,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleBitwiseRightShiftUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_right_shift_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleCmpUnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleCmpUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int32(2);

  int32_t expected_value = 1;

  apply_binary_visitor_and_check_result<corevm::types::native_type_cmp_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  expected_value = -1;

  apply_binary_visitor_and_check_result<corevm::types::native_type_cmp_visitor>(
    h2,
    h1,
    expected_value,
    true
  );

  h2 = corevm::types::int32(100);

  expected_value = 0;

  apply_binary_visitor_and_check_result<corevm::types::native_type_cmp_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleCmpUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(16.12);

  int32_t expected_value = -1;

  apply_binary_visitor_and_check_result<corevm::types::native_type_cmp_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  expected_value = 1;

  apply_binary_visitor_and_check_result<corevm::types::native_type_cmp_visitor>(
    h2,
    h1,
    expected_value,
    true
  );

  h1 = corevm::types::decimal2(16.12);

  expected_value = 0;

  apply_binary_visitor_and_check_result<corevm::types::native_type_cmp_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleCmpUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(true);

  int32_t expected_value = -1;

  apply_binary_visitor_and_check_result<corevm::types::native_type_cmp_visitor>(
    h1,
    h2,
    expected_value
  );

  expected_value = 1;

  apply_binary_visitor_and_check_result<corevm::types::native_type_cmp_visitor>(
    h2,
    h1,
    expected_value
  );

  h1 = h2;

  expected_value = 0;

  apply_binary_visitor_and_check_result<corevm::types::native_type_cmp_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleCmpUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello world!!!");

  int32_t expected_value = 0;

  apply_binary_visitor_and_check_result<corevm::types::native_type_cmp_visitor>(
    h1,
    h2,
    expected_value
  );

  h1 = corevm::types::string("Hello");

  expected_value = -1;

  apply_binary_visitor_and_check_result<corevm::types::native_type_cmp_visitor>(
    h1,
    h2,
    expected_value
  );

  expected_value = 1;

  apply_binary_visitor_and_check_result<corevm::types::native_type_cmp_visitor>(
    h2,
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleCmpUnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_cmp_visitor, corevm::types::RuntimeError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleCmpUnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_cmp_visitor, corevm::types::RuntimeError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleCmpUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int8(2);

  int32_t expected_value = 1;

  apply_binary_visitor_and_check_result<corevm::types::native_type_cmp_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  int32_t expected_value2 = -1;

  apply_binary_visitor_and_check_result<corevm::types::native_type_cmp_visitor>(
    h2,
    h1,
    expected_value2,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleCmpUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_cmp_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleEqUnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleEqUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int32(2);

  int32_t expected_value = (100 == 2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleEqUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6.12 == 2.0);

  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleEqUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(true);

  const int64_t expected_value = false == true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleEqUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello");
  typename corevm::types::NativeTypeHandle h3 = h2;

  const bool expected_value = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h1,
    h1,
    expected_value
  );

  const bool expected_value2 = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h1,
    h2,
    expected_value2
  );

  const bool expected_value3 = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h2,
    h3,
    expected_value3
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleEqUnitTest, TestBetweenArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h3 = h2;

  const bool expected_value = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h1,
    h1,
    expected_value
  );

  const bool expected_value2 = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h2,
    h3,
    expected_value2
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleEqUnitTest, TestBetweenMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h3 = h2;

  const bool expected_value = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h1,
    h1,
    expected_value
  );

  const bool expected_value2 = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h2,
    h3,
    expected_value2
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleEqUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int8(2);

  int64_t expected_value = (3 == 2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  int64_t expected_value2 = (2 == 3);

  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h2,
    h1,
    expected_value2,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleEqUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_eq_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleNeqUnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleNeqUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int32(2);

  int32_t expected_value = (100 != 2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleNeqUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6.12 != 2.0);

  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleNeqUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(true);

  const int64_t expected_value = false != true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleNeqUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello");
  typename corevm::types::NativeTypeHandle h3 = h2;

  const bool expected_value = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1,
    h1,
    expected_value
  );

  const bool expected_value2 = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1,
    h2,
    expected_value2
  );

  const bool expected_value3 = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h2,
    h3,
    expected_value3
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleNeqUnitTest, TestBetweenArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();

  const bool expected_value = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleNeqUnitTest, TestBetweenMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();

  const bool expected_value = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleNeqUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int8(2);

  int64_t expected_value = (3 != 2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  int64_t expected_value2 = (2 != 3);

  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h2,
    h1,
    expected_value2,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleNeqUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_neq_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleGtUnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleGtUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int32(2);

  int32_t expected_value = (100 > 2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleGtUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6.12 > 2.0);

  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleGtUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(true);

  const int64_t expected_value = false > true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleGtUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello");
  typename corevm::types::NativeTypeHandle h3 = h2;

  const bool expected_value = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h1,
    h1,
    expected_value
  );

  const bool expected_value2 = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h1,
    h2,
    expected_value2
  );

  const bool expected_value3 = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h2,
    h1,
    expected_value3
  );

  const bool expected_value4 = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h2,
    h3,
    expected_value4
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleGtUnitTest, TestBetweenArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();

  const bool expected_value = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleGtUnitTest, TestBetweenMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_gt_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleGtUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int8(2);

  int64_t expected_value = (3 > 2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  int64_t expected_value2 = (2 > 3);

  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h2,
    h1,
    expected_value2,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleGtUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_gt_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleLtUnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLtUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int32(2);

  int32_t expected_value = (100 < 2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLtUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6.12 < 2.0);

  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLtUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(true);

  const int64_t expected_value = false < true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLtUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello");
  typename corevm::types::NativeTypeHandle h3 = h2;

  const bool expected_value = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h1,
    h1,
    expected_value
  );

  const bool expected_value2 = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h1,
    h2,
    expected_value2
  );

  const bool expected_value3 = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h2,
    h1,
    expected_value3
  );

  const bool expected_value4 = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h2,
    h3,
    expected_value4
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLtUnitTest, TestBetweenArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();

  const bool expected_value = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLtUnitTest, TestBetweenMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_lt_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLtUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int8(2);

  int64_t expected_value = (3 < 2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  int64_t expected_value2 = (2 < 3);

  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h2,
    h1,
    expected_value2,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLtUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_lt_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleGteUnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleGteUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int32(2);

  int32_t expected_value = (100 >= 2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleGteUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6.12 >= 2.0);

  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleGteUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(true);

  const int64_t expected_value = false >= true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleGteUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello");
  typename corevm::types::NativeTypeHandle h3 = h2;

  const bool expected_value = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h1,
    h1,
    expected_value
  );

  const bool expected_value2 = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h1,
    h2,
    expected_value2
  );

  const bool expected_value3 = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h2,
    h1,
    expected_value3
  );

  const bool expected_value4 = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h2,
    h3,
    expected_value4
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleGteUnitTest, TestBetweenArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();

  const bool expected_value = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleGteUnitTest, TestBetweenMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_gte_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleGteUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int8(2);

  int64_t expected_value = (3 >= 2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  int64_t expected_value2 = (2 >= 3);

  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h2,
    h1,
    expected_value2,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleGteUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_gte_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeHandleLteUnitTest : public NativeTypeHandleBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLteUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int32(2);

  int32_t expected_value = (100 <= 2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLteUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6.12 <= 2.0);

  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLteUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::boolean(true);

  const int64_t expected_value = false <= true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLteUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello");
  typename corevm::types::NativeTypeHandle h3 = h2;

  const bool expected_value = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h1,
    h1,
    expected_value
  );

  const bool expected_value2 = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h1,
    h2,
    expected_value2
  );

  const bool expected_value3 = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h2,
    h1,
    expected_value3
  );

  const bool expected_value4 = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h2,
    h3,
    expected_value4
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLteUnitTest, TestBetweenArrayTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::array();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::array();

  const bool expected_value = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLteUnitTest, TestBetweenMapTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::map();
  typename corevm::types::NativeTypeHandle h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_lte_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLteUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::int8(2);

  int64_t expected_value = (3 <= 2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h1,
    h2,
    expected_value,
    true
  );

  int64_t expected_value2 = (2 <= 3);

  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h2,
    h1,
    expected_value2,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeHandleLteUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeHandle h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeHandle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_lte_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------
