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
#include "types/native_type_value.h"

#include <gtest/gtest.h>

#include <cmath>
#include <cstdint>


class NativeTypeValueUnitTest : public ::testing::Test
{
public:
  template<typename T>
  void assert_native_type_value(
    corevm::types::NativeTypeValue& type_value,
    T expected_value,
    bool decimal_value=false)
  {
    T actual_value = corevm::types::get_intrinsic_value_from_type_value<T>(type_value);

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
    corevm::types::NativeTypeValue& type_value,
    T expected_value,
    bool decimal_value=false,
    bool on_self=false)
  {
    typename corevm::types::NativeTypeValue result =
      corevm::types::apply_unary_visitor<VisitorType>(type_value);

    if (on_self)
    {
      assert_native_type_value(type_value, expected_value, decimal_value);
    }
    else
    {
      assert_native_type_value(result, expected_value, decimal_value);
    }
  }

  template<typename VisitorType, typename T>
  void apply_binary_visitor_and_check_result(
    corevm::types::NativeTypeValue& h1,
    corevm::types::NativeTypeValue& h2,
    T expected_value,
    bool decimal_value=false)
  {
    typename corevm::types::NativeTypeValue result =
      corevm::types::apply_binary_visitor<VisitorType>(h1, h2);

    assert_native_type_value(result, expected_value, decimal_value);
  }

  template<class VisitorType, class ExceptionType>
  void apply_unary_visitor_and_check_exception(
    corevm::types::NativeTypeValue& type_value)
  {
    ASSERT_THROW(
      {
        corevm::types::apply_unary_visitor<VisitorType>(type_value);
      },
      ExceptionType
    );
  }

  template<class VisitorType, class ExceptionType>
  void apply_binary_visitor_and_check_exception(
    corevm::types::NativeTypeValue& lhs, corevm::types::NativeTypeValue& rhs)
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

TEST_F(NativeTypeValueUnitTest, TestGetIntrinsicValueFromTypeValue)
{
  corevm::types::native_string str = "Hello world";
  corevm::types::NativeTypeValue type_val = str;

  corevm::types::native_string actual_string =
    corevm::types::get_intrinsic_value_from_type_value<corevm::types::native_string>(type_val);

  ASSERT_EQ(actual_string, str);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueUnitTest, TestGetIntrinsicValueFromTypeValueWithInvalidType)
{
  corevm::types::native_string str = "Hello world";
  corevm::types::NativeTypeValue type_val = str;

  ASSERT_THROW(
    {
      corevm::types::get_intrinsic_value_from_type_value<corevm::types::native_map>(type_val);
    },
    corevm::types::ConversionError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueUnitTest, TestGetIntrinsicValueFromTypeValueWithCompatibleType)
{
  corevm::types::uint64 value = 10;
  corevm::types::NativeTypeValue type_val = value;

  bool actual_value = corevm::types::get_intrinsic_value_from_type_value<bool>(type_val);

  ASSERT_EQ(static_cast<bool>(value), actual_value);
}

// -----------------------------------------------------------------------------

class NativeTypeValueUnaryOperatorUnitTest : public NativeTypeValueUnitTest {};

// -----------------------------------------------------------------------------

class NativeTypeValuePositiveUnitTest : public NativeTypeValueUnaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValuePositiveUnitTest, TestOnIntegerType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(-1);

  int8_t expected_value = -1;

  apply_unary_visitor_and_check_result<corevm::types::native_type_positive_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValuePositiveUnitTest, TestOnDecimalType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(-3.14);

  double expected_value = -3.14;

  apply_unary_visitor_and_check_result<corevm::types::native_type_positive_visitor>(
    h1,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValuePositiveUnitTest, TestOnBooleanType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(false);

  const bool expected_value = false;

  apply_unary_visitor_and_check_result<corevm::types::native_type_positive_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValuePositiveUnitTest, TestOnStringType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_positive_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValuePositiveUnitTest, TestOnArrayType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_positive_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValuePositiveUnitTest, TestOnMapType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_positive_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

class NativeTypeValueNegationUnitTest : public NativeTypeValueUnaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueNegationUnitTest, TestOnIntegerType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(-1);

  int8_t expected_value = -(-1);

  apply_unary_visitor_and_check_result<corevm::types::native_type_negation_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueNegationUnitTest, TestOnDecimalType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(-3.14);

  double expected_value = -(-3.14);

  apply_unary_visitor_and_check_result<corevm::types::native_type_negation_visitor>(
    h1,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueNegationUnitTest, TestOnBooleanType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(true);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(false);

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

TEST_F(NativeTypeValueNegationUnitTest, TestOnStringType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_negation_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueNegationUnitTest, TestOnArrayType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_negation_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueNegationUnitTest, TestOnMapType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_negation_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

class NativeTypeValueIncrementUnitTest : public NativeTypeValueUnaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueIncrementUnitTest, TestOnIntegerType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);

  int8_t expected_value = 2;

  apply_unary_visitor_and_check_result<corevm::types::native_type_increment_visitor>(
    h1,
    expected_value,
    false,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueIncrementUnitTest, TestOnDecimalType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(3.14);

  double expected_value = 4.14;

  apply_unary_visitor_and_check_result<corevm::types::native_type_increment_visitor>(
    h1,
    expected_value,
    true,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueIncrementUnitTest, TestOnBooleanType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(true);

  const bool expected_value = true;

  apply_unary_visitor_and_check_result<corevm::types::native_type_increment_visitor>(
    h1,
    expected_value,
    false,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueIncrementUnitTest, TestOnStringType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_increment_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueIncrementUnitTest, TestOnArrayType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_increment_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueIncrementUnitTest, TestOnMapType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_increment_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

class NativeTypeValueDecrementUnitTest : public NativeTypeValueUnaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueDecrementUnitTest, TestOnIntegerType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(-10);

  int8_t expected_value = -11;

  apply_unary_visitor_and_check_result<corevm::types::native_type_decrement_visitor>(
    h1,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueDecrementUnitTest, TestOnDecimalType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(3.14);

  double expected_value = 2.14;

  apply_unary_visitor_and_check_result<corevm::types::native_type_decrement_visitor>(
    h1,
    expected_value,
    true,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueDecrementUnitTest, TestOnBooleanType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(true);

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_decrement_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueDecrementUnitTest, TestOnStringType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_decrement_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueDecrementUnitTest, TestOnArrayType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_decrement_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueDecrementUnitTest, TestOnMapType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_decrement_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

class NativeTypeValueLogicalNotUnitTest : public NativeTypeValueUnaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalNotUnitTest, TestOnIntegerType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(99);

  int8_t expected_value = 0;

  apply_unary_visitor_and_check_result<corevm::types::native_type_logical_not_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalNotUnitTest, TestOnDecimalType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(3.14);

  double expected_value = 0.0;

  apply_unary_visitor_and_check_result<corevm::types::native_type_logical_not_visitor>(
    h1,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalNotUnitTest, TestOnBooleanType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(true);

  const bool expected_value = false;

  apply_unary_visitor_and_check_result<corevm::types::native_type_logical_not_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalNotUnitTest, TestOnStringType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_logical_not_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalNotUnitTest, TestOnArrayType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_logical_not_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalNotUnitTest, TestOnMapType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_logical_not_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

class NativeTypeValueBitwiseNotUnitTest : public NativeTypeValueUnaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseNotUnitTest, TestOnIntegerType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(9);

  int8_t expected_value = ~9;

  apply_unary_visitor_and_check_result<corevm::types::native_type_bitwise_not_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseNotUnitTest, TestOnDecimalType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(3.14);

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_not_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseNotUnitTest, TestOnBooleanType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(true);

  const bool expected_value = true;

  apply_unary_visitor_and_check_result<corevm::types::native_type_bitwise_not_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseNotUnitTest, TestOnStringType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_not_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseNotUnitTest, TestOnArrayType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_not_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseNotUnitTest, TestOnMapType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_not_visitor, corevm::types::InvalidOperatorError>(h1);
}

// -----------------------------------------------------------------------------

class NativeTypeValueAbsUnitTest : public NativeTypeValueUnaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueAbsUnitTest, TestOnIntegerType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(-9);

  int8_t expected_value = 9;

  apply_unary_visitor_and_check_result<corevm::types::native_type_abs_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueAbsUnitTest, TestOnDecimalType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(-3.1415926);

  double expected_value = 3.1415926;

  apply_unary_visitor_and_check_result<corevm::types::native_type_abs_visitor>(
    h1,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueAbsUnitTest, TestOnBooleanType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(true);

  const bool expected_value = true;

  apply_unary_visitor_and_check_result<corevm::types::native_type_abs_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueAbsUnitTest, TestOnStringType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_abs_visitor, corevm::types::ConversionError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueAbsUnitTest, TestOnArrayType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_abs_visitor, corevm::types::ConversionError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueAbsUnitTest, TestOnMapType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_abs_visitor, corevm::types::ConversionError>(h1);
}

// -----------------------------------------------------------------------------

class NativeTypeValueSqrtUnitTest : public NativeTypeValueUnaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueSqrtUnitTest, TestOnIntegerType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(9);

  int8_t expected_value = 3;

  apply_unary_visitor_and_check_result<corevm::types::native_type_sqrt_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueSqrtUnitTest, TestOnDecimalType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(9.869600);

  double expected_value = 3.1415926;

  apply_unary_visitor_and_check_result<corevm::types::native_type_sqrt_visitor>(
    h1,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueSqrtUnitTest, TestOnBooleanType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(true);

  const bool expected_value = true;

  apply_unary_visitor_and_check_result<corevm::types::native_type_sqrt_visitor>(
    h1,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueSqrtUnitTest, TestOnStringType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_sqrt_visitor, corevm::types::ConversionError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueSqrtUnitTest, TestOnArrayType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_sqrt_visitor, corevm::types::ConversionError>(h1);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueSqrtUnitTest, TestOnMapType)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_sqrt_visitor, corevm::types::ConversionError>(h1);
}

// -----------------------------------------------------------------------------

class NativeTypeValueBinaryOperatorUnitTest : public NativeTypeValueUnitTest {};

// -----------------------------------------------------------------------------

class NativeTypeValueAdditionUnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueAdditionUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::uint8(2);

  uint8_t expected_value = 3;

  apply_binary_visitor_and_check_result<corevm::types::native_type_addition_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueAdditionUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(3.14159);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(2.22);

  const double expected_value = 3.14159 + 2.22;

  apply_binary_visitor_and_check_result<corevm::types::native_type_addition_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueAdditionUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(true);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(false);

  const bool expected_value = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_addition_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueAdditionUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello ");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("world!!!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_addition_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueAdditionUnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_addition_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueAdditionUnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_addition_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueAdditionUnitTest, TestAddtionBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(3.14);

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

TEST_F(NativeTypeValueAdditionUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_addition_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeValueSubtractionUnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueSubtractionUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint8(100);
  typename corevm::types::NativeTypeValue h2 = corevm::types::uint8(12);

  uint8_t expected_value = 100 - 12; // I'm too lazy to calculate that...

  apply_binary_visitor_and_check_result<corevm::types::native_type_subtraction_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueSubtractionUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(3.14159);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(2.22);

  const double expected_value = 3.14159 - 2.22;

  apply_binary_visitor_and_check_result<corevm::types::native_type_subtraction_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueSubtractionUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(true);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(false);

  const bool expected_value = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_subtraction_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueSubtractionUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_subtraction_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueSubtractionUnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_subtraction_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueSubtractionUnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_subtraction_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueSubtractionUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int8(1);

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

TEST_F(NativeTypeValueSubtractionUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_subtraction_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeValueMultiplicationUnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueMultiplicationUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint16(33);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int32(-36);

  int32_t expected_value = 33 * (-36);

  apply_binary_visitor_and_check_result<corevm::types::native_type_multiplication_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueMultiplicationUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(3.14159);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(2.22);

  const double expected_value = 3.14159 * 2.22;

  apply_binary_visitor_and_check_result<corevm::types::native_type_multiplication_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueMultiplicationUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(true);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(false);

  const bool expected_value = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_multiplication_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueMultiplicationUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_multiplication_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueMultiplicationUnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_multiplication_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueMultiplicationUnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_multiplication_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueMultiplicationUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int8(2);

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

TEST_F(NativeTypeValueMultiplicationUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_multiplication_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeValueDivisionUnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueDivisionUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int32(-10);

  int32_t expected_value = 100 / (-10);

  apply_binary_visitor_and_check_result<corevm::types::native_type_division_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueDivisionUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(2.0);

  const double expected_value = 6.12 / 2.0;

  apply_binary_visitor_and_check_result<corevm::types::native_type_division_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueDivisionUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(true);

  const bool expected_value = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_division_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueDivisionUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_division_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueDivisionUnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_division_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueDivisionUnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_division_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueDivisionUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int8(2);

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

TEST_F(NativeTypeValueDivisionUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_division_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeValueModulusUnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueModulusUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int32(-10);

  int32_t expected_value = 100 % (-10);

  apply_binary_visitor_and_check_result<corevm::types::native_type_modulus_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueModulusUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 6 % 2;

  apply_binary_visitor_and_check_result<corevm::types::native_type_modulus_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueModulusUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(true);

  const int64_t expected_value = 0 % 1;

  apply_binary_visitor_and_check_result<corevm::types::native_type_modulus_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueModulusUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_modulus_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueModulusUnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_modulus_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueModulusUnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_modulus_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueModulusUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int8(2);

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

TEST_F(NativeTypeValueModulusUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_modulus_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeValuePowUnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValuePowUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint16(2);
  typename corevm::types::NativeTypeValue h2 = corevm::types::uint32(4);

  double expected_value = pow(2, 4);

  apply_binary_visitor_and_check_result<corevm::types::native_type_pow_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValuePowUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(6);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(-1.2);

  const double expected_value = pow(6, -1.2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_pow_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValuePowUnitTest, TestBetweenIntegerAndDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint32(6);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(-1.2);

  const double expected_value = pow(6u, -1.2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_pow_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}
// -----------------------------------------------------------------------------

TEST_F(NativeTypeValuePowUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(true);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(false);

  const double expected_value = pow(1, 0);

  apply_binary_visitor_and_check_result<corevm::types::native_type_pow_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValuePowUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_pow_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValuePowUnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_pow_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValuePowUnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_pow_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValuePowUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int8(2);

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

TEST_F(NativeTypeValuePowUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_pow_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeValueLogicalAND_UnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalAND_UnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int32(-10);

  int32_t expected_value = 1; // (100) && (-10)

  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_and_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalAND_UnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 1; // (6.12) && (2.0)

  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_and_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalAND_UnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(true);

  const int64_t expected_value =  0; // 0 && 1

  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_and_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalAND_UnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_and_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalAND_UnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_and_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalAND_UnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_and_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalAND_UnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int8(2);

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

TEST_F(NativeTypeValueLogicalAND_UnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_and_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeValueLogicalOR_UnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalOR_UnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int32(-10);

  int32_t expected_value = 1; // (100) || (-10)

  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_or_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalOR_UnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 1.0; // (6.12) || (2.0)

  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_or_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalOR_UnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(true);

  const int64_t expected_value = 1; // 0 || 1

  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_or_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalOR_UnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_or_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalOR_UnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_or_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalOR_UnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_or_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLogicalOR_UnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int8(2);

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

TEST_F(NativeTypeValueLogicalOR_UnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_or_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeValueBitwiseAND_UnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseAND_UnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int32(-10);

  int32_t expected_value = (100) & (-10);

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_and_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseAND_UnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6) & (2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_and_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseAND_UnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(true);

  const int64_t expected_value = (0) & (1);

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_and_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseAND_UnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_and_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseAND_UnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_and_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseAND_UnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_and_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseAND_UnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int8(2);

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

TEST_F(NativeTypeValueBitwiseAND_UnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_and_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeValueBitwiseOR_UnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseOR_UnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int32(-10);

  int32_t expected_value = (100) | (-10);

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_or_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseOR_UnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 6 | 2;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_or_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseOR_UnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(true);

  const int64_t expected_value = 0 | 1;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_or_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseOR_UnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_or_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseOR_UnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_or_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseOR_UnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_or_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseOR_UnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int8(2);

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

TEST_F(NativeTypeValueBitwiseOR_UnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_or_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeValueBitwiseXOR_UnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseXOR_UnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int32(-10);

  int32_t expected_value = (100) ^ (-10);

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_xor_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseXOR_UnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 6 ^ 2;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_xor_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseXOR_UnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(true);

  const int64_t expected_value = false ^ true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_xor_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseXOR_UnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_xor_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseXOR_UnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_xor_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseXOR_UnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_xor_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseXOR_UnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int8(2);

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

TEST_F(NativeTypeValueBitwiseXOR_UnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_xor_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeValueBitwiseLeftShiftUnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseLeftShiftUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int32(2);

  int32_t expected_value = (100) << (2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_left_shift_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseLeftShiftUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 6 << 2;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_left_shift_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseLeftShiftUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(true);

  const int64_t expected_value = false << true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_left_shift_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseLeftShiftUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_left_shift_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseLeftShiftUnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_left_shift_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseLeftShiftUnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_left_shift_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseLeftShiftUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int8(2);

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

TEST_F(NativeTypeValueBitwiseLeftShiftUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_left_shift_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeValueBitwiseRightShiftUnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseRightShiftUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int32(2);

  int32_t expected_value = (100) >> (2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_right_shift_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseRightShiftUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 6 >> 2;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_right_shift_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseRightShiftUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(true);

  const int64_t expected_value = false >> true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_right_shift_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseRightShiftUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_right_shift_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseRightShiftUnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_right_shift_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseRightShiftUnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_right_shift_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueBitwiseRightShiftUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int8(2);

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

TEST_F(NativeTypeValueBitwiseRightShiftUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_right_shift_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeValueCmpUnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueCmpUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int32(2);

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

TEST_F(NativeTypeValueCmpUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(16.12);

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

TEST_F(NativeTypeValueCmpUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(true);

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

TEST_F(NativeTypeValueCmpUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello world!!!");

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

TEST_F(NativeTypeValueCmpUnitTest, TestWithArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_cmp_visitor, corevm::RuntimeError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueCmpUnitTest, TestWithMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_cmp_visitor, corevm::RuntimeError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueCmpUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int8(2);

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

TEST_F(NativeTypeValueCmpUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_cmp_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeValueEqUnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueEqUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int32(2);

  int32_t expected_value = (100 == 2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueEqUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6.12 == 2.0);

  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueEqUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(true);

  const int64_t expected_value = false == true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueEqUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello");
  typename corevm::types::NativeTypeValue h3 = h2;

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

TEST_F(NativeTypeValueEqUnitTest, TestBetweenArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();
  typename corevm::types::NativeTypeValue h3 = h2;

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

TEST_F(NativeTypeValueEqUnitTest, TestBetweenMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();
  typename corevm::types::NativeTypeValue h3 = h2;

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

TEST_F(NativeTypeValueEqUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int8(2);

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

TEST_F(NativeTypeValueEqUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_eq_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeValueNeqUnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueNeqUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int32(2);

  int32_t expected_value = (100 != 2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueNeqUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6.12 != 2.0);

  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueNeqUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(true);

  const int64_t expected_value = false != true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueNeqUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello");
  typename corevm::types::NativeTypeValue h3 = h2;

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

TEST_F(NativeTypeValueNeqUnitTest, TestBetweenArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();

  const bool expected_value = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueNeqUnitTest, TestBetweenMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();

  const bool expected_value = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueNeqUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int8(2);

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

TEST_F(NativeTypeValueNeqUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_neq_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeValueGtUnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueGtUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int32(2);

  int32_t expected_value = (100 > 2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueGtUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6.12 > 2.0);

  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueGtUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(true);

  const int64_t expected_value = false > true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueGtUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello");
  typename corevm::types::NativeTypeValue h3 = h2;

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

TEST_F(NativeTypeValueGtUnitTest, TestBetweenArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();

  const bool expected_value = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueGtUnitTest, TestBetweenMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_gt_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueGtUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int8(2);

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

TEST_F(NativeTypeValueGtUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_gt_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeValueLtUnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLtUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int32(2);

  int32_t expected_value = (100 < 2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLtUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6.12 < 2.0);

  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLtUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(true);

  const int64_t expected_value = false < true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLtUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello");
  typename corevm::types::NativeTypeValue h3 = h2;

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

TEST_F(NativeTypeValueLtUnitTest, TestBetweenArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();

  const bool expected_value = false;

  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLtUnitTest, TestBetweenMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_lt_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLtUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int8(2);

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

TEST_F(NativeTypeValueLtUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_lt_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeValueGteUnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueGteUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int32(2);

  int32_t expected_value = (100 >= 2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueGteUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6.12 >= 2.0);

  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueGteUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(true);

  const int64_t expected_value = false >= true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueGteUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello");
  typename corevm::types::NativeTypeValue h3 = h2;

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

TEST_F(NativeTypeValueGteUnitTest, TestBetweenArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();

  const bool expected_value = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueGteUnitTest, TestBetweenMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_gte_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueGteUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int8(2);

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

TEST_F(NativeTypeValueGteUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_gte_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------

class NativeTypeValueLteUnitTest : public NativeTypeValueBinaryOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLteUnitTest, TestBetweenIntegerTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::uint16(100);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int32(2);

  int32_t expected_value = (100 <= 2);

  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLteUnitTest, TestBetweenDecimalTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal(6.12);
  typename corevm::types::NativeTypeValue h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6.12 <= 2.0);

  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h1,
    h2,
    expected_value,
    true
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLteUnitTest, TestBetweenBooleanTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::boolean(false);
  typename corevm::types::NativeTypeValue h2 = corevm::types::boolean(true);

  const int64_t expected_value = false <= true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLteUnitTest, TestWithStringTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello");
  typename corevm::types::NativeTypeValue h3 = h2;

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

TEST_F(NativeTypeValueLteUnitTest, TestBetweenArrayTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::array();
  typename corevm::types::NativeTypeValue h2 = corevm::types::array();

  const bool expected_value = true;

  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h1,
    h2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLteUnitTest, TestBetweenMapTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::map();
  typename corevm::types::NativeTypeValue h2 = corevm::types::map();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_lte_visitor, corevm::types::InvalidOperatorError>(h1, h2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypeValueLteUnitTest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::decimal2(3.14);
  typename corevm::types::NativeTypeValue h2 = corevm::types::int8(2);

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

TEST_F(NativeTypeValueLteUnitTest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::NativeTypeValue h1 = corevm::types::int8(1);
  typename corevm::types::NativeTypeValue h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_lte_visitor, corevm::types::ConversionError>(h1, h2);
}

// -----------------------------------------------------------------------------
