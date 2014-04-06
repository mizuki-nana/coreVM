#include <sneaker/testing/_unittest.h>
#include "../../include/types/native_type_handle.h"


class native_type_handle_unittest : public ::testing::Test {
public:
  template<typename T>
  void assert_handle_value(
    corevm::types::native_type_handle& handle,
    T expected_value,
    bool decimal_value=false
  ) {
    T actual_value = corevm::types::get_value_from_handle<T>(handle);

    if(!decimal_value) {
      ASSERT_EQ(expected_value, actual_value);
    } else {
      ASSERT_FLOAT_EQ(expected_value, actual_value);
    }
  }

  template<typename visitor_type, typename T>
  void apply_unary_visitor_and_check_result(
    corevm::types::native_type_handle& handle,
    T expected_value,
    bool decimal_value=false,
    bool on_self=false
  )
  {
    typename corevm::types::native_type_handle result =
      corevm::types::apply_unary_visitor<visitor_type>(handle);

    if(on_self) {
      assert_handle_value(handle, expected_value, decimal_value);
    } else {
      assert_handle_value(result, expected_value, decimal_value);
    }
  }

  template<typename visitor_type, typename T>
  void apply_binary_visitor_and_check_result(
    corevm::types::native_type_handle& h1,
    corevm::types::native_type_handle& h2,
    T expected_value,
    bool decimal_value=false
  )
  {
    typename corevm::types::native_type_handle result =
      corevm::types::apply_binary_visitor<visitor_type>(h1, h2);

    assert_handle_value(result, expected_value, decimal_value);
  }

  template<class visitor_type, class exception_type>
  void apply_unary_visitor_and_check_exception(
    corevm::types::native_type_handle& handle)
  {
    ASSERT_THROW(
      corevm::types::apply_unary_visitor<visitor_type>(handle),
      exception_type
    );
  }

  template<class visitor_type, class exception_type>
  void apply_binary_visitor_and_check_exception(
    corevm::types::native_type_handle& lhs, corevm::types::native_type_handle& rhs)
  {
    ASSERT_THROW(
      corevm::types::apply_binary_visitor<visitor_type>(lhs, rhs),
      exception_type
    );
  }
};


class native_type_handle_unary_operator_unittest : public native_type_handle_unittest {};


class native_type_handle_positive_unittest : public native_type_handle_unary_operator_unittest {};


TEST_F(native_type_handle_positive_unittest, TestOnIntegerType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(-1);

  int8_t expected_value = -1;
  apply_unary_visitor_and_check_result<corevm::types::native_type_positive_visitor>(
    h1, expected_value
  );
}

TEST_F(native_type_handle_positive_unittest, TestOnDecimalType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(-3.14);

  double expected_value = -3.14;
  apply_unary_visitor_and_check_result<corevm::types::native_type_positive_visitor>(
    h1, expected_value, true
  );
}

TEST_F(native_type_handle_positive_unittest, TestOnBooleanType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(false);

  const bool expected_value = false;
  apply_unary_visitor_and_check_result<corevm::types::native_type_positive_visitor>(
    h1, expected_value
  );
}

TEST_F(native_type_handle_positive_unittest, TestOnStringType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_positive_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1);
}

TEST_F(native_type_handle_positive_unittest, TestOnArrayType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_positive_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1);
}


class native_type_handle_negation_unittest : public native_type_handle_unary_operator_unittest {};


TEST_F(native_type_handle_negation_unittest, TestOnIntegerType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(-1);

  int8_t expected_value = -(-1);
  apply_unary_visitor_and_check_result<corevm::types::native_type_negation_visitor>(
    h1, expected_value
  );
}

TEST_F(native_type_handle_negation_unittest, TestOnDecimalType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(-3.14);

  double expected_value = -(-3.14);
  apply_unary_visitor_and_check_result<corevm::types::native_type_negation_visitor>(
    h1, expected_value, true
  );
}

TEST_F(native_type_handle_negation_unittest, TestOnBooleanType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(true);
  typename corevm::types::native_type_handle h2 = corevm::types::boolean(false);

  const bool expected_value = true;
  apply_unary_visitor_and_check_result<corevm::types::native_type_negation_visitor>(
    h1, expected_value
  );

  const bool expected_value2 = false;
  apply_unary_visitor_and_check_result<corevm::types::native_type_negation_visitor>(
    h2, expected_value2
  );
}

TEST_F(native_type_handle_negation_unittest, TestOnStringType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_negation_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1);
}

TEST_F(native_type_handle_negation_unittest, TestOnArrayType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_negation_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1);
}


class native_type_handle_increment_unittest : public native_type_handle_unary_operator_unittest {};


TEST_F(native_type_handle_increment_unittest, TestOnIntegerType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);

  int8_t expected_value = 2;
  apply_unary_visitor_and_check_result<corevm::types::native_type_increment_visitor>(
    h1, expected_value, false, true
  );
}

TEST_F(native_type_handle_increment_unittest, TestOnDecimalType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(3.14);

  double expected_value = 4.14;
  apply_unary_visitor_and_check_result<corevm::types::native_type_increment_visitor>(
    h1, expected_value, true, true
  );
}

TEST_F(native_type_handle_increment_unittest, TestOnBooleanType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(true);

  const bool expected_value = true;
  apply_unary_visitor_and_check_result<corevm::types::native_type_increment_visitor>(
    h1, expected_value, false, true
  );
}

TEST_F(native_type_handle_increment_unittest, TestOnStringType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_increment_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1);
}

TEST_F(native_type_handle_increment_unittest, TestOnArrayType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_increment_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1);
}


class native_type_handle_decrement_unittest : public native_type_handle_unary_operator_unittest {};


TEST_F(native_type_handle_decrement_unittest, TestOnIntegerType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(-10);

  int8_t expected_value = -11;
  apply_unary_visitor_and_check_result<corevm::types::native_type_decrement_visitor>(
    h1, expected_value, true
  );
}

TEST_F(native_type_handle_decrement_unittest, TestOnDecimalType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(3.14);

  double expected_value = 2;
  apply_unary_visitor_and_check_result<corevm::types::native_type_decrement_visitor>(
    h1, expected_value, true, true
  );
}

TEST_F(native_type_handle_decrement_unittest, TestOnBooleanType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(true);

  const bool expected_value = false;
  apply_unary_visitor_and_check_result<corevm::types::native_type_decrement_visitor>(
    h1, expected_value, false, true
  );
}

TEST_F(native_type_handle_decrement_unittest, TestOnStringType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_decrement_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1);
}

TEST_F(native_type_handle_decrement_unittest, TestOnArrayType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_decrement_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1);
}


class native_type_handle_logical_not_unittest : public native_type_handle_unary_operator_unittest {};


TEST_F(native_type_handle_logical_not_unittest, TestOnIntegerType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(99);

  int8_t expected_value = 0;
  apply_unary_visitor_and_check_result<corevm::types::native_type_logical_not_visitor>(
    h1, expected_value
  );
}

TEST_F(native_type_handle_logical_not_unittest, TestOnDecimalType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(3.14);

  double expected_value = 0.0;
  apply_unary_visitor_and_check_result<corevm::types::native_type_logical_not_visitor>(
    h1, expected_value, true
  );
}

TEST_F(native_type_handle_logical_not_unittest, TestOnBooleanType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(true);

  const bool expected_value = false;
  apply_unary_visitor_and_check_result<corevm::types::native_type_logical_not_visitor>(
    h1, expected_value
  );
}

TEST_F(native_type_handle_logical_not_unittest, TestOnStringType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_logical_not_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1);
}

TEST_F(native_type_handle_logical_not_unittest, TestOnArrayType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_logical_not_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1);
}


class native_type_handle_bitwise_not_unittest : public native_type_handle_unary_operator_unittest {};


TEST_F(native_type_handle_bitwise_not_unittest, TestOnIntegerType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(9);

  int8_t expected_value = ~9;
  apply_unary_visitor_and_check_result<corevm::types::native_type_bitwise_not_visitor>(
    h1, expected_value
  );
}

TEST_F(native_type_handle_bitwise_not_unittest, TestOnDecimalType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(3.14);

  double expected_value = ~(3);
  apply_unary_visitor_and_check_result<corevm::types::native_type_bitwise_not_visitor>(
    h1, expected_value, true
  );
}

TEST_F(native_type_handle_bitwise_not_unittest, TestOnBooleanType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(true);

  const bool expected_value = true;
  apply_unary_visitor_and_check_result<corevm::types::native_type_bitwise_not_visitor>(
    h1, expected_value
  );
}

TEST_F(native_type_handle_bitwise_not_unittest, TestOnStringType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_not_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1);
}

TEST_F(native_type_handle_bitwise_not_unittest, TestOnArrayType)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();

  apply_unary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_not_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1);
}


class native_type_handle_binary_operator_unittest : public native_type_handle_unittest {};


class native_type_handle_addition_unittest : public native_type_handle_binary_operator_unittest {};


TEST_F(native_type_handle_addition_unittest, TestBetweenIntegerTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::uint8(2);

  uint8_t expected_value = 3;
  apply_binary_visitor_and_check_result<corevm::types::native_type_addition_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_addition_unittest, TestBetweenDecimalTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(3.14159);
  typename corevm::types::native_type_handle h2 = corevm::types::decimal2(2.22);

  const double expected_value = 3.14159 + 2.22;
  apply_binary_visitor_and_check_result<corevm::types::native_type_addition_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_addition_unittest, TestBetweenBooleanTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(true);
  typename corevm::types::native_type_handle h2 = corevm::types::boolean(false);

  const bool expected_value = true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_addition_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_addition_unittest, TestWithStringTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello ");
  typename corevm::types::native_type_handle h2 = corevm::types::string("world!!!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_addition_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_addition_unittest, TestWithArrayTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();
  typename corevm::types::native_type_handle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_addition_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_addition_unittest, TestAddtionBetweenTypesOfDifferentSizes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::decimal2(3.14);

  double expected_value = 4.14;
  apply_binary_visitor_and_check_result<corevm::types::native_type_addition_visitor>(
    h1, h2, expected_value, true
  );

  apply_binary_visitor_and_check_result<corevm::types::native_type_addition_visitor>(
    h2, h1, expected_value, true
  );
}

TEST_F(native_type_handle_addition_unittest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_addition_visitor, corevm::types::corevm_native_type_conversion_error>(h1, h2);
}


class native_type_handle_subtraction_unittest : public native_type_handle_binary_operator_unittest {};


TEST_F(native_type_handle_subtraction_unittest, TestBetweenIntegerTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::uint8(100);
  typename corevm::types::native_type_handle h2 = corevm::types::uint8(12);

  uint8_t expected_value = 100 - 12; // I'm too lazy to calculate that...
  apply_binary_visitor_and_check_result<corevm::types::native_type_subtraction_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_subtraction_unittest, TestBetweenDecimalTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(3.14159);
  typename corevm::types::native_type_handle h2 = corevm::types::decimal2(2.22);

  const double expected_value = 3.14159 - 2.22;
  apply_binary_visitor_and_check_result<corevm::types::native_type_subtraction_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_subtraction_unittest, TestBetweenBooleanTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(true);
  typename corevm::types::native_type_handle h2 = corevm::types::boolean(false);

  const bool expected_value = true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_subtraction_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_subtraction_unittest, TestWithStringTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_subtraction_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_subtraction_unittest, TestWithArrayTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();
  typename corevm::types::native_type_handle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_subtraction_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_subtraction_unittest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::native_type_handle h2 = corevm::types::int8(1);

  double expected_value = 2.14;
  apply_binary_visitor_and_check_result<corevm::types::native_type_subtraction_visitor>(
    h1, h2, expected_value, true
  );

  double expected_value2 = -2.14;
  apply_binary_visitor_and_check_result<corevm::types::native_type_subtraction_visitor>(
    h2, h1, expected_value2, true
  );
}

TEST_F(native_type_handle_subtraction_unittest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_subtraction_visitor, corevm::types::corevm_native_type_conversion_error>(h1, h2);
}


class native_type_handle_multiplication_unittest : public native_type_handle_binary_operator_unittest {};


TEST_F(native_type_handle_multiplication_unittest, TestBetweenIntegerTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::uint16(33);
  typename corevm::types::native_type_handle h2 = corevm::types::int32(-36);

  int32_t expected_value = 33 * (-36); // I'm too lazy to calculate that...
  apply_binary_visitor_and_check_result<corevm::types::native_type_multiplication_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_multiplication_unittest, TestBetweenDecimalTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(3.14159);
  typename corevm::types::native_type_handle h2 = corevm::types::decimal2(2.22);

  const double expected_value = 3.14159 * 2.22;
  apply_binary_visitor_and_check_result<corevm::types::native_type_multiplication_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_multiplication_unittest, TestBetweenBooleanTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(true);
  typename corevm::types::native_type_handle h2 = corevm::types::boolean(false);

  const bool expected_value = false;
  apply_binary_visitor_and_check_result<corevm::types::native_type_multiplication_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_multiplication_unittest, TestWithStringTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_multiplication_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_multiplication_unittest, TestWithArrayTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();
  typename corevm::types::native_type_handle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_multiplication_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_multiplication_unittest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::native_type_handle h2 = corevm::types::int8(2);

  double expected_value = 3.14 * 2;
  apply_binary_visitor_and_check_result<corevm::types::native_type_multiplication_visitor>(
    h1, h2, expected_value, true
  );

  double expected_value2 = 2 * 3.14;
  apply_binary_visitor_and_check_result<corevm::types::native_type_multiplication_visitor>(
    h2, h1, expected_value2, true
  );
}

TEST_F(native_type_handle_multiplication_unittest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_multiplication_visitor, corevm::types::corevm_native_type_conversion_error>(h1, h2);
}


class native_type_handle_division_unittest : public native_type_handle_binary_operator_unittest {};


TEST_F(native_type_handle_division_unittest, TestBetweenIntegerTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::uint16(100);
  typename corevm::types::native_type_handle h2 = corevm::types::int32(-10);

  int32_t expected_value = 100 / (-10); // I'm too lazy to calculate that...
  apply_binary_visitor_and_check_result<corevm::types::native_type_division_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_division_unittest, TestBetweenDecimalTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(6.12);
  typename corevm::types::native_type_handle h2 = corevm::types::decimal2(2.0);

  const double expected_value = 6.12 / 2.0;
  apply_binary_visitor_and_check_result<corevm::types::native_type_division_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_division_unittest, TestBetweenBooleanTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(false);
  typename corevm::types::native_type_handle h2 = corevm::types::boolean(true);

  const bool expected_value = false;
  apply_binary_visitor_and_check_result<corevm::types::native_type_division_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_division_unittest, TestWithStringTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_division_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_division_unittest, TestWithArrayTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();
  typename corevm::types::native_type_handle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_division_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_division_unittest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::native_type_handle h2 = corevm::types::int8(2);

  double expected_value = 3.14 / 2;
  apply_binary_visitor_and_check_result<corevm::types::native_type_division_visitor>(
    h1, h2, expected_value, true
  );

  double expected_value2 = 2 / 3.14;
  apply_binary_visitor_and_check_result<corevm::types::native_type_division_visitor>(
    h2, h1, expected_value2, true
  );
}

TEST_F(native_type_handle_division_unittest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_division_visitor, corevm::types::corevm_native_type_conversion_error>(h1, h2);
}


class native_type_handle_modulus_unittest : public native_type_handle_binary_operator_unittest {};


TEST_F(native_type_handle_modulus_unittest, TestBetweenIntegerTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::uint16(100);
  typename corevm::types::native_type_handle h2 = corevm::types::int32(-10);

  int32_t expected_value = 100 % (-10); // I'm too lazy to calculate that...
  apply_binary_visitor_and_check_result<corevm::types::native_type_modulus_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_modulus_unittest, TestBetweenDecimalTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(6.12);
  typename corevm::types::native_type_handle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 6 % 2;
  apply_binary_visitor_and_check_result<corevm::types::native_type_modulus_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_modulus_unittest, TestBetweenBooleanTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(false);
  typename corevm::types::native_type_handle h2 = corevm::types::boolean(true);

  const int64_t expected_value = 0 % 1;
  apply_binary_visitor_and_check_result<corevm::types::native_type_modulus_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_modulus_unittest, TestWithStringTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_modulus_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_modulus_unittest, TestWithArrayTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();
  typename corevm::types::native_type_handle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_modulus_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_modulus_unittest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::native_type_handle h2 = corevm::types::int8(2);

  int64_t expected_value = 3 % 2;
  apply_binary_visitor_and_check_result<corevm::types::native_type_modulus_visitor>(
    h1, h2, expected_value, true
  );

  int64_t expected_value2 = 2 % 3;
  apply_binary_visitor_and_check_result<corevm::types::native_type_modulus_visitor>(
    h2, h1, expected_value2, true
  );
}

TEST_F(native_type_handle_modulus_unittest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_modulus_visitor, corevm::types::corevm_native_type_conversion_error>(h1, h2);
}


class native_type_handle_logical_and_unittest : public native_type_handle_binary_operator_unittest {};


TEST_F(native_type_handle_logical_and_unittest, TestBetweenIntegerTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::uint16(100);
  typename corevm::types::native_type_handle h2 = corevm::types::int32(-10);

  int32_t expected_value = 1; // (100) && (-10)
  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_and_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_logical_and_unittest, TestBetweenDecimalTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(6.12);
  typename corevm::types::native_type_handle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 1.0; // (6.12) && (2.0)
  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_and_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_logical_and_unittest, TestBetweenBooleanTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(false);
  typename corevm::types::native_type_handle h2 = corevm::types::boolean(true);

  const int64_t expected_value = 0; // 0 && 1
  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_and_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_logical_and_unittest, TestWithStringTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_and_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_logical_and_unittest, TestWithArrayTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();
  typename corevm::types::native_type_handle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_and_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_logical_and_unittest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::native_type_handle h2 = corevm::types::int8(2);

  int64_t expected_value = 1; // 3.14 && 2
  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_and_visitor>(
    h1, h2, expected_value, true
  );

  int64_t expected_value2 = 1; // 2 && 3.14
  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_and_visitor>(
    h2, h1, expected_value2, true
  );
}

TEST_F(native_type_handle_logical_and_unittest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_and_visitor, corevm::types::corevm_native_type_conversion_error>(h1, h2);
}


class native_type_handle_logical_or_unittest : public native_type_handle_binary_operator_unittest {};


TEST_F(native_type_handle_logical_or_unittest, TestBetweenIntegerTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::uint16(100);
  typename corevm::types::native_type_handle h2 = corevm::types::int32(-10);

  int32_t expected_value = 1; // (100) || (-10)
  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_or_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_logical_or_unittest, TestBetweenDecimalTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(6.12);
  typename corevm::types::native_type_handle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 1.0; // (6.12) || (2.0)
  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_or_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_logical_or_unittest, TestBetweenBooleanTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(false);
  typename corevm::types::native_type_handle h2 = corevm::types::boolean(true);

  const int64_t expected_value = 1; // 0 || 1
  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_or_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_logical_or_unittest, TestWithStringTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_or_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_logical_or_unittest, TestWithArrayTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();
  typename corevm::types::native_type_handle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_or_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_logical_or_unittest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::native_type_handle h2 = corevm::types::int8(2);

  int64_t expected_value = 1; // 3.14 || 2
  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_or_visitor>(
    h1, h2, expected_value, true
  );

  int64_t expected_value2 = 1; // 2 || 3.14
  apply_binary_visitor_and_check_result<corevm::types::native_type_logical_or_visitor>(
    h2, h1, expected_value2, true
  );
}

TEST_F(native_type_handle_logical_or_unittest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_logical_or_visitor, corevm::types::corevm_native_type_conversion_error>(h1, h2);
}


class native_type_handle_bitwise_and_unittest : public native_type_handle_binary_operator_unittest {};


TEST_F(native_type_handle_bitwise_and_unittest, TestBetweenIntegerTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::uint16(100);
  typename corevm::types::native_type_handle h2 = corevm::types::int32(-10);

  int32_t expected_value = (100) & (-10);
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_and_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_bitwise_and_unittest, TestBetweenDecimalTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(6.12);
  typename corevm::types::native_type_handle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6) & (2);
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_and_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_bitwise_and_unittest, TestBetweenBooleanTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(false);
  typename corevm::types::native_type_handle h2 = corevm::types::boolean(true);

  const int64_t expected_value = (0) & (1);
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_and_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_bitwise_and_unittest, TestWithStringTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_and_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_bitwise_and_unittest, TestWithArrayTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();
  typename corevm::types::native_type_handle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_and_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_bitwise_and_unittest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::native_type_handle h2 = corevm::types::int8(2);

  int64_t expected_value = 3 & 2;
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_and_visitor>(
    h1, h2, expected_value, true
  );

  int64_t expected_value2 = 2 & 3;
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_and_visitor>(
    h2, h1, expected_value2, true
  );
}

TEST_F(native_type_handle_bitwise_and_unittest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_and_visitor, corevm::types::corevm_native_type_conversion_error>(h1, h2);
}


class native_type_handle_bitwise_or_unittest : public native_type_handle_binary_operator_unittest {};


TEST_F(native_type_handle_bitwise_or_unittest, TestBetweenIntegerTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::uint16(100);
  typename corevm::types::native_type_handle h2 = corevm::types::int32(-10);

  int32_t expected_value = (100) | (-10);
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_or_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_bitwise_or_unittest, TestBetweenDecimalTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(6.12);
  typename corevm::types::native_type_handle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 6 | 2;
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_or_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_bitwise_or_unittest, TestBetweenBooleanTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(false);
  typename corevm::types::native_type_handle h2 = corevm::types::boolean(true);

  const int64_t expected_value = 0 | 1;
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_or_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_bitwise_or_unittest, TestWithStringTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_or_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_bitwise_or_unittest, TestWithArrayTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();
  typename corevm::types::native_type_handle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_or_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_bitwise_or_unittest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::native_type_handle h2 = corevm::types::int8(2);

  int64_t expected_value = 3 | 2;
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_or_visitor>(
    h1, h2, expected_value, true
  );

  int64_t expected_value2 = 2 | 3;
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_or_visitor>(
    h2, h1, expected_value2, true
  );
}

TEST_F(native_type_handle_bitwise_or_unittest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_or_visitor, corevm::types::corevm_native_type_conversion_error>(h1, h2);
}


class native_type_handle_bitwise_xor_unittest : public native_type_handle_binary_operator_unittest {};


TEST_F(native_type_handle_bitwise_xor_unittest, TestBetweenIntegerTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::uint16(100);
  typename corevm::types::native_type_handle h2 = corevm::types::int32(-10);

  int32_t expected_value = (100) ^ (-10);
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_xor_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_bitwise_xor_unittest, TestBetweenDecimalTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(6.12);
  typename corevm::types::native_type_handle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 6 ^ 2;
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_xor_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_bitwise_xor_unittest, TestBetweenBooleanTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(false);
  typename corevm::types::native_type_handle h2 = corevm::types::boolean(true);

  const int64_t expected_value = false ^ true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_xor_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_bitwise_xor_unittest, TestWithStringTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_xor_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_bitwise_xor_unittest, TestWithArrayTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();
  typename corevm::types::native_type_handle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_xor_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_bitwise_xor_unittest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::native_type_handle h2 = corevm::types::int8(2);

  int64_t expected_value = 3 ^ 2;
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_xor_visitor>(
    h1, h2, expected_value, true
  );

  int64_t expected_value2 = 2 ^ 3;
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_xor_visitor>(
    h2, h1, expected_value2, true
  );
}

TEST_F(native_type_handle_bitwise_xor_unittest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_xor_visitor, corevm::types::corevm_native_type_conversion_error>(h1, h2);
}


class native_type_handle_bitwise_left_shift_unittest : public native_type_handle_binary_operator_unittest {};


TEST_F(native_type_handle_bitwise_left_shift_unittest, TestBetweenIntegerTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::uint16(100);
  typename corevm::types::native_type_handle h2 = corevm::types::int32(2);

  int32_t expected_value = (100) << (2);
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_left_shift_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_bitwise_left_shift_unittest, TestBetweenDecimalTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(6.12);
  typename corevm::types::native_type_handle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 6 << 2;
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_left_shift_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_bitwise_left_shift_unittest, TestBetweenBooleanTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(false);
  typename corevm::types::native_type_handle h2 = corevm::types::boolean(true);

  const int64_t expected_value = false << true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_left_shift_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_bitwise_left_shift_unittest, TestWithStringTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_left_shift_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_bitwise_left_shift_unittest, TestWithArrayTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();
  typename corevm::types::native_type_handle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_left_shift_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_bitwise_left_shift_unittest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::native_type_handle h2 = corevm::types::int8(2);

  int64_t expected_value = 3 << 2;
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_left_shift_visitor>(
    h1, h2, expected_value, true
  );

  int64_t expected_value2 = 2 << 3;
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_left_shift_visitor>(
    h2, h1, expected_value2, true
  );
}

TEST_F(native_type_handle_bitwise_left_shift_unittest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_left_shift_visitor, corevm::types::corevm_native_type_conversion_error>(h1, h2);
}


class native_type_handle_bitwise_right_shift_unittest : public native_type_handle_binary_operator_unittest {};


TEST_F(native_type_handle_bitwise_right_shift_unittest, TestBetweenIntegerTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::uint16(100);
  typename corevm::types::native_type_handle h2 = corevm::types::int32(2);

  int32_t expected_value = (100) >> (2);
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_right_shift_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_bitwise_right_shift_unittest, TestBetweenDecimalTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(6.12);
  typename corevm::types::native_type_handle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = 6 >> 2;
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_right_shift_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_bitwise_right_shift_unittest, TestBetweenBooleanTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(false);
  typename corevm::types::native_type_handle h2 = corevm::types::boolean(true);

  const int64_t expected_value = false >> true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_right_shift_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_bitwise_right_shift_unittest, TestWithStringTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_right_shift_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_bitwise_right_shift_unittest, TestWithArrayTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();
  typename corevm::types::native_type_handle h2 = corevm::types::array();

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_right_shift_visitor, corevm::types::corevm_native_type_invalid_operator_error>(h1, h2);
}

TEST_F(native_type_handle_bitwise_right_shift_unittest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::native_type_handle h2 = corevm::types::int8(2);

  int64_t expected_value = 3 >> 2;
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_right_shift_visitor>(
    h1, h2, expected_value, true
  );

  int64_t expected_value2 = 2 >> 3;
  apply_binary_visitor_and_check_result<corevm::types::native_type_bitwise_right_shift_visitor>(
    h2, h1, expected_value2, true
  );
}

TEST_F(native_type_handle_bitwise_right_shift_unittest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_bitwise_right_shift_visitor, corevm::types::corevm_native_type_conversion_error>(h1, h2);
}


class native_type_handle_eq_unittest : public native_type_handle_binary_operator_unittest {};


TEST_F(native_type_handle_eq_unittest, TestBetweenIntegerTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::uint16(100);
  typename corevm::types::native_type_handle h2 = corevm::types::int32(2);

  int32_t expected_value = (100 == 2);
  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_eq_unittest, TestBetweenDecimalTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(6.12);
  typename corevm::types::native_type_handle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6.12 == 2.0);
  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_eq_unittest, TestBetweenBooleanTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(false);
  typename corevm::types::native_type_handle h2 = corevm::types::boolean(true);

  const int64_t expected_value = false == true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_eq_unittest, TestWithStringTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello");
  typename corevm::types::native_type_handle h3 = h2;

  const bool expected_value = true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h1, h1, expected_value
  );

  const bool expected_value2 = false;
  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h1, h2, expected_value2
  );

  const bool expected_value3 = true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h2, h3, expected_value3
  );
}

TEST_F(native_type_handle_eq_unittest, TestBetweenArrayTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();
  typename corevm::types::native_type_handle h2 = corevm::types::array();
  typename corevm::types::native_type_handle h3 = h2;

  const bool expected_value = true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h1, h1, expected_value
  );

  const bool expected_value2 = true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h2, h3, expected_value2
  );
}

TEST_F(native_type_handle_eq_unittest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::native_type_handle h2 = corevm::types::int8(2);

  int64_t expected_value = (3 == 2);
  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h1, h2, expected_value, true
  );

  int64_t expected_value2 = (2 == 3);
  apply_binary_visitor_and_check_result<corevm::types::native_type_eq_visitor>(
    h2, h1, expected_value2, true
  );
}

TEST_F(native_type_handle_eq_unittest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_eq_visitor, corevm::types::corevm_native_type_conversion_error>(h1, h2);
}


class native_type_handle_neq_unittest : public native_type_handle_binary_operator_unittest {};


TEST_F(native_type_handle_neq_unittest, TestBetweenIntegerTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::uint16(100);
  typename corevm::types::native_type_handle h2 = corevm::types::int32(2);

  int32_t expected_value = (100 != 2);
  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_neq_unittest, TestBetweenDecimalTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(6.12);
  typename corevm::types::native_type_handle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6.12 != 2.0);
  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_neq_unittest, TestBetweenBooleanTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(false);
  typename corevm::types::native_type_handle h2 = corevm::types::boolean(true);

  const int64_t expected_value = false != true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_neq_unittest, TestWithStringTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello");
  typename corevm::types::native_type_handle h3 = h2;

  const bool expected_value = false;
  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1, h1, expected_value
  );

  const bool expected_value2 = true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1, h2, expected_value2
  );

  const bool expected_value3 = false;
  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h2, h3, expected_value3
  );
}

TEST_F(native_type_handle_neq_unittest, TestBetweenArrayTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();
  typename corevm::types::native_type_handle h2 = corevm::types::array();

  const bool expected_value = false;
  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_neq_unittest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::native_type_handle h2 = corevm::types::int8(2);

  int64_t expected_value = (3 != 2);
  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h1, h2, expected_value, true
  );

  int64_t expected_value2 = (2 != 3);
  apply_binary_visitor_and_check_result<corevm::types::native_type_neq_visitor>(
    h2, h1, expected_value2, true
  );
}

TEST_F(native_type_handle_neq_unittest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_neq_visitor, corevm::types::corevm_native_type_conversion_error>(h1, h2);
}


class native_type_handle_gt_unittest : public native_type_handle_binary_operator_unittest {};


TEST_F(native_type_handle_gt_unittest, TestBetweenIntegerTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::uint16(100);
  typename corevm::types::native_type_handle h2 = corevm::types::int32(2);

  int32_t expected_value = (100 > 2);
  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_gt_unittest, TestBetweenDecimalTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(6.12);
  typename corevm::types::native_type_handle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6.12 > 2.0);
  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_gt_unittest, TestBetweenBooleanTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(false);
  typename corevm::types::native_type_handle h2 = corevm::types::boolean(true);

  const int64_t expected_value = false > true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_gt_unittest, TestWithStringTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello");
  typename corevm::types::native_type_handle h3 = h2;

  const bool expected_value = false;
  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h1, h1, expected_value
  );

  const bool expected_value2 = true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h1, h2, expected_value2
  );

  const bool expected_value3 = false;
  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h2, h1, expected_value3
  );

  const bool expected_value4 = false;
  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h2, h3, expected_value4
  );
}

TEST_F(native_type_handle_gt_unittest, TestBetweenArrayTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();
  typename corevm::types::native_type_handle h2 = corevm::types::array();

  const bool expected_value = false;
  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_gt_unittest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::native_type_handle h2 = corevm::types::int8(2);

  int64_t expected_value = (3 > 2);
  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h1, h2, expected_value, true
  );

  int64_t expected_value2 = (2 > 3);
  apply_binary_visitor_and_check_result<corevm::types::native_type_gt_visitor>(
    h2, h1, expected_value2, true
  );
}

TEST_F(native_type_handle_gt_unittest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_gt_visitor, corevm::types::corevm_native_type_conversion_error>(h1, h2);
}


class native_type_handle_lt_unittest : public native_type_handle_binary_operator_unittest {};


TEST_F(native_type_handle_lt_unittest, TestBetweenIntegerTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::uint16(100);
  typename corevm::types::native_type_handle h2 = corevm::types::int32(2);

  int32_t expected_value = (100 < 2);
  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_lt_unittest, TestBetweenDecimalTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(6.12);
  typename corevm::types::native_type_handle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6.12 < 2.0);
  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_lt_unittest, TestBetweenBooleanTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(false);
  typename corevm::types::native_type_handle h2 = corevm::types::boolean(true);

  const int64_t expected_value = false < true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_lt_unittest, TestWithStringTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello");
  typename corevm::types::native_type_handle h3 = h2;

  const bool expected_value = false;
  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h1, h1, expected_value
  );

  const bool expected_value2 = false;
  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h1, h2, expected_value2
  );

  const bool expected_value3 = true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h2, h1, expected_value3
  );

  const bool expected_value4 = false;
  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h2, h3, expected_value4
  );
}

TEST_F(native_type_handle_lt_unittest, TestBetweenArrayTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();
  typename corevm::types::native_type_handle h2 = corevm::types::array();

  const bool expected_value = false;
  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_lt_unittest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::native_type_handle h2 = corevm::types::int8(2);

  int64_t expected_value = (3 < 2);
  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h1, h2, expected_value, true
  );

  int64_t expected_value2 = (2 < 3);
  apply_binary_visitor_and_check_result<corevm::types::native_type_lt_visitor>(
    h2, h1, expected_value2, true
  );

}

TEST_F(native_type_handle_lt_unittest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_lt_visitor, corevm::types::corevm_native_type_conversion_error>(h1, h2);
}


class native_type_handle_gte_unittest : public native_type_handle_binary_operator_unittest {};


TEST_F(native_type_handle_gte_unittest, TestBetweenIntegerTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::uint16(100);
  typename corevm::types::native_type_handle h2 = corevm::types::int32(2);

  int32_t expected_value = (100 >= 2);
  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_gte_unittest, TestBetweenDecimalTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(6.12);
  typename corevm::types::native_type_handle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6.12 >= 2.0);
  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_gte_unittest, TestBetweenBooleanTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(false);
  typename corevm::types::native_type_handle h2 = corevm::types::boolean(true);

  const int64_t expected_value = false >= true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_gte_unittest, TestWithStringTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello");
  typename corevm::types::native_type_handle h3 = h2;

  const bool expected_value = true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h1, h1, expected_value
  );

  const bool expected_value2 = true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h1, h2, expected_value2
  );

  const bool expected_value3 = false;
  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h2, h1, expected_value3
  );

  const bool expected_value4 = true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h2, h3, expected_value4
  );
}

TEST_F(native_type_handle_gte_unittest, TestBetweenArrayTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();
  typename corevm::types::native_type_handle h2 = corevm::types::array();

  const bool expected_value = true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_gte_unittest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::native_type_handle h2 = corevm::types::int8(2);

  int64_t expected_value = (3 >= 2);
  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h1, h2, expected_value, true
  );

  int64_t expected_value2 = (2 >= 3);
  apply_binary_visitor_and_check_result<corevm::types::native_type_gte_visitor>(
    h2, h1, expected_value2, true
  );
}

TEST_F(native_type_handle_gte_unittest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_gte_visitor, corevm::types::corevm_native_type_conversion_error>(h1, h2);
}


class native_type_handle_lte_unittest : public native_type_handle_binary_operator_unittest {};


TEST_F(native_type_handle_lte_unittest, TestBetweenIntegerTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::uint16(100);
  typename corevm::types::native_type_handle h2 = corevm::types::int32(2);

  int32_t expected_value = (100 <= 2);
  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_lte_unittest, TestBetweenDecimalTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal(6.12);
  typename corevm::types::native_type_handle h2 = corevm::types::decimal2(2.0);

  const int64_t expected_value = (6.12 <= 2.0);
  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h1, h2, expected_value, true
  );
}

TEST_F(native_type_handle_lte_unittest, TestBetweenBooleanTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::boolean(false);
  typename corevm::types::native_type_handle h2 = corevm::types::boolean(true);

  const int64_t expected_value = false <= true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_lte_unittest, TestWithStringTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::string("Hello world!!!");
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello");
  typename corevm::types::native_type_handle h3 = h2;

  const bool expected_value = true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h1, h1, expected_value
  );

  const bool expected_value2 = false;
  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h1, h2, expected_value2
  );

  const bool expected_value3 = true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h2, h1, expected_value3
  );

  const bool expected_value4 = true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h2, h3, expected_value4
  );
}

TEST_F(native_type_handle_lte_unittest, TestBetweenArrayTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::array();
  typename corevm::types::native_type_handle h2 = corevm::types::array();

  const bool expected_value = true;
  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h1, h2, expected_value
  );
}

TEST_F(native_type_handle_lte_unittest, TestBetweenTypesOfDifferentSizes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::decimal2(3.14);
  typename corevm::types::native_type_handle h2 = corevm::types::int8(2);

  int64_t expected_value = (3 <= 2);
  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h1, h2, expected_value, true
  );

  int64_t expected_value2 = (2 <= 3);
  apply_binary_visitor_and_check_result<corevm::types::native_type_lte_visitor>(
    h2, h1, expected_value2, true
  );
}

TEST_F(native_type_handle_lte_unittest, TestBetweenIncompatibleTypes)
{
  typename corevm::types::native_type_handle h1 = corevm::types::int8(1);
  typename corevm::types::native_type_handle h2 = corevm::types::string("Hello World!");

  apply_binary_visitor_and_check_exception<
    corevm::types::native_type_lte_visitor, corevm::types::corevm_native_type_conversion_error>(h1, h2);
}