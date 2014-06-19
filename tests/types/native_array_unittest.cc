#include <stdexcept>
#include <sneaker/testing/_unittest.h>
#include "../test_helper.h"
#include "../../include/types/errors.h"
#include "../../include/types/native_array.h"
#include "../../include/types/types.h"


class native_array_unittest : public ::testing::Test {
protected:
  const corevm::types::int64::value_type fixture_element1 = static_cast<corevm::types::int64::value_type>(1);
  const corevm::types::int64::value_type fixture_element2 = static_cast<corevm::types::int64::value_type>(2);
  const corevm::types::int64::value_type fixture_element3 = static_cast<corevm::types::int64::value_type>(3);
};


TEST_F(native_array_unittest, TestEmptyInitialization)
{
  const corevm::types::native_array array;

  ASSERT_EQ(true, array.empty());
  ASSERT_EQ(0, array.size());
}

TEST_F(native_array_unittest, TestCopyConstructor)
{
  const corevm::types::native_array array1 = {fixture_element1, fixture_element2, fixture_element3};
  const corevm::types::native_array array2 = array1;

  ASSERT_EQ(false, array1.empty());
  ASSERT_EQ(false, array2.empty());
  ASSERT_EQ(array1.size(), array2.size());

  ASSERT_EQ(fixture_element1, array2[0]);
  ASSERT_EQ(fixture_element2, array2[1]);
  ASSERT_EQ(fixture_element3, array2[2]);
}

TEST_F(native_array_unittest, TestCopyConstructorOnIntegerType)
{
  _ASSERT_THROW(
    {const corevm::types::native_array array = 123;},
    corevm::types::corevm_native_type_conversion_error
  );
}

TEST_F(native_array_unittest, TestConvertingToIntegerType)
{
  _ASSERT_THROW(
    {int i = corevm::types::native_array(); i++;},
    corevm::types::corevm_native_type_conversion_error
  );
}

TEST_F(native_array_unittest, TestAssignmentOperator)
{
  const corevm::types::native_array array1 = {fixture_element1, fixture_element2, fixture_element3};
  corevm::types::native_array array2;
  array2 = array1;

  ASSERT_EQ(false, array1.empty());
  ASSERT_EQ(false, array2.empty());
  ASSERT_EQ(array1.size(), array2.size());

  ASSERT_EQ(fixture_element1, array2[0]);
  ASSERT_EQ(fixture_element2, array2[1]);
  ASSERT_EQ(fixture_element3, array2[2]);
}

TEST_F(native_array_unittest, TestEqualityBetweenEmptyArrays)
{
  const corevm::types::native_array array1;
  const corevm::types::native_array array2;

  ASSERT_TRUE(array1 == array1);
  ASSERT_TRUE(array2 == array2);

  ASSERT_TRUE(array1 == array2);
}

TEST_F(native_array_unittest, TestEqualityBetweenNonEmptyArrays)
{
  const corevm::types::native_array array1 = {fixture_element1, fixture_element2, fixture_element3};
  const corevm::types::native_array array2;

  ASSERT_FALSE(array1 == array2);
  ASSERT_TRUE(array1 != array2);
}

TEST_F(native_array_unittest, TestEqualityBetweenIdenticalArrays)
{
  const corevm::types::native_array array1 = {fixture_element1, fixture_element2, fixture_element3};
  const corevm::types::native_array array2 = {fixture_element1, fixture_element2, fixture_element3};

  ASSERT_TRUE(array1 == array1);
  ASSERT_TRUE(array2 == array2);

  ASSERT_TRUE(array1 == array2);
}


class native_array_functionality_unittest : public native_array_unittest {};


TEST_F(native_array_functionality_unittest, TestPushBack)
{
  corevm::types::native_array array;

  array.push_back(this->fixture_element1);
  array.push_back(this->fixture_element2);
  array.push_back(this->fixture_element3);

  ASSERT_EQ(false, array.empty());
  ASSERT_EQ(3, array.size());

  ASSERT_EQ(fixture_element1, array[0]);
  ASSERT_EQ(fixture_element2, array[1]);
  ASSERT_EQ(fixture_element3, array[2]);

  ASSERT_EQ(fixture_element1, array.at(0));
  ASSERT_EQ(fixture_element2, array.at(1));
  ASSERT_EQ(fixture_element3, array.at(2));


  _ASSERT_THROW(
    { array.at(3); },
    std::out_of_range
  );
}


class native_array_operator_unittest : public native_array_unittest {};

class native_array_unary_operator_unittest : public native_array_operator_unittest {};


TEST_F(native_array_unary_operator_unittest, TestPositiveOperator)
{
  _ASSERT_THROW(
    {const corevm::types::native_array array; +array;},
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_array_unary_operator_unittest, TestNegationOperator)
{
  _ASSERT_THROW(
    {const corevm::types::native_array array; -array;},
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_array_unary_operator_unittest, TestIncrementOperator)
{
  _ASSERT_THROW(
    {const corevm::types::native_array array; ++array;},
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_array_unary_operator_unittest, TestDecrementOperator)
{
  _ASSERT_THROW(
    {const corevm::types::native_array array; --array;},
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_array_unary_operator_unittest, TestLogicalNotOperator)
{
  _ASSERT_THROW(
    {const corevm::types::native_array array; !array;},
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_array_unary_operator_unittest, TestBitwiseNotOperator)
{
  _ASSERT_THROW(
    {const corevm::types::native_array array; ~array;},
    corevm::types::corevm_native_type_invalid_operator_error
  );
}


class native_array_binary_operator_unittest : public native_array_operator_unittest {};


TEST_F(native_array_binary_operator_unittest, TestAdditionOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 + array2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_array_binary_operator_unittest, TestSubtractionOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 - array2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_array_binary_operator_unittest, TestMultiplicationOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 * array2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_array_binary_operator_unittest, TestDivisionOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 / array2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_array_binary_operator_unittest, TestLogicalANDOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 && array2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_array_binary_operator_unittest, TestLogicalOROperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 || array2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_array_binary_operator_unittest, TestBitwiseANDOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 & array2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_array_binary_operator_unittest, TestBitwiseOROperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 | array2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_array_binary_operator_unittest, TestBitwiseXOROperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 ^ array2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_array_binary_operator_unittest, TestBitwiseLeftShiftOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 << array2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_array_binary_operator_unittest, TestBitwiseRightShiftOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 >> array2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

void f(bool) {}

TEST_F(native_array_binary_operator_unittest, TestEQOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      f(array1 == array2);
    }
  );
}

TEST_F(native_array_binary_operator_unittest, TestNEQOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      f(array1 != array2);
    }
  );
}

TEST_F(native_array_binary_operator_unittest, TestGTOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      f(array1 > array2);
    }
  );
}

TEST_F(native_array_binary_operator_unittest, TestLTOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      f(array1 < array2);
    }
  );
}

TEST_F(native_array_binary_operator_unittest, TestGTEOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      f(array1 >= array2);
    }
  );
}

TEST_F(native_array_binary_operator_unittest, TestLTEOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      f(array1 <= array2);
    }
  );
}
