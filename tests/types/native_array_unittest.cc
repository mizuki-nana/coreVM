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
#include "types/errors.h"
#include "types/native_array.h"
#include "types/types.h"

#include <gtest/gtest.h>

#include <stdexcept>


class NativeArrayUnitTest : public ::testing::Test
{
protected:
  const corevm::types::uint64 fixture_element1 = static_cast<corevm::types::uint64>(1);
  const corevm::types::uint64 fixture_element2 = static_cast<corevm::types::uint64>(2);
  const corevm::types::uint64 fixture_element3 = static_cast<corevm::types::uint64>(3);
};

// -----------------------------------------------------------------------------

TEST_F(NativeArrayUnitTest, TestEmptyInitialization)
{
  const corevm::types::native_array array;

  ASSERT_EQ(true, array.empty());
  ASSERT_EQ(0, array.size());
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayUnitTest, TestCopyConstructor)
{
  const corevm::types::native_array array1 {
    fixture_element1,
    fixture_element2,
    fixture_element3
  };
  const corevm::types::native_array array2 = array1;

  ASSERT_EQ(false, array1.empty());
  ASSERT_EQ(false, array2.empty());
  ASSERT_EQ(array1.size(), array2.size());

  ASSERT_EQ(fixture_element1, array2[0]);
  ASSERT_EQ(fixture_element2, array2[1]);
  ASSERT_EQ(fixture_element3, array2[2]);
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayUnitTest, TestCopyConstructorOnIntegerType)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array = 123;
    },
    corevm::types::conversion_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayUnitTest, TestConvertingToIntegerType)
{
  ASSERT_THROW(
    {
      int i = corevm::types::native_array();
      i++;
    },
    corevm::types::conversion_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayUnitTest, TestAssignmentOperator)
{
  const corevm::types::native_array array1 {
    fixture_element1,
    fixture_element2,
    fixture_element3
  };
  corevm::types::native_array array2;
  array2 = array1;

  ASSERT_EQ(false, array1.empty());
  ASSERT_EQ(false, array2.empty());
  ASSERT_EQ(array1.size(), array2.size());

  ASSERT_EQ(fixture_element1, array2[0]);
  ASSERT_EQ(fixture_element2, array2[1]);
  ASSERT_EQ(fixture_element3, array2[2]);
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayUnitTest, TestEqualityBetweenEmptyArrays)
{
  const corevm::types::native_array array1;
  const corevm::types::native_array array2;

  ASSERT_TRUE(array1 == array1);
  ASSERT_TRUE(array2 == array2);

  ASSERT_TRUE(array1 == array2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayUnitTest, TestEqualityBetweenNonEmptyArrays)
{
  const corevm::types::native_array array1 {
    fixture_element1,
    fixture_element2,
    fixture_element3
  };
  const corevm::types::native_array array2;

  ASSERT_FALSE(array1 == array2);
  ASSERT_TRUE(array1 != array2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayUnitTest, TestEqualityBetweenIdenticalArrays)
{
  const corevm::types::native_array array1 {
    fixture_element1,
    fixture_element2,
    fixture_element3
  };
  const corevm::types::native_array array2 {
    fixture_element1,
    fixture_element2,
    fixture_element3
  };

  ASSERT_TRUE(array1 == array1);
  ASSERT_TRUE(array2 == array2);

  ASSERT_TRUE(array1 == array2);
}

// -----------------------------------------------------------------------------

class NativeArrayFunctionalityUnitTest : public NativeArrayUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeArrayFunctionalityUnitTest, TestPushBack)
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

  ASSERT_THROW(
    {
      array.at(3);
    },
    corevm::types::out_of_range_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayFunctionalityUnitTest, TestAtSuccessful)
{
  const corevm::types::native_array array {1, 2, 3};

  corevm::types::native_array::value_type expected_result = 3;
  corevm::types::native_array::value_type actual_result = array.at(2);

  ASSERT_EQ(expected_result, actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayFunctionalityUnitTest, TestAtFailure)
{
  const corevm::types::native_array array {1, 2, 3};

  ASSERT_THROW(
    {
      array.at(100);
    },
    corevm::types::out_of_range_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayFunctionalityUnitTest, TestEraseSuccessful)
{
  corevm::types::native_array array {1, 2, 3, 4, 5};

  corevm::types::native_array expected_result = {1, 2, 3, 5};

  array.erase(3);

  ASSERT_EQ(expected_result.size(), array.size());
  ASSERT_EQ(expected_result[0], array[0]);
  ASSERT_EQ(expected_result[1], array[1]);
  ASSERT_EQ(expected_result[2], array[2]);
  ASSERT_EQ(expected_result[3], array[3]);
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayFunctionalityUnitTest, TestEraseFailure)
{
  corevm::types::native_array array {1, 2, 3, 4, 5};

  ASSERT_THROW(
    {
      array.erase(5);
    },
    corevm::types::out_of_range_error
  );
}

// -----------------------------------------------------------------------------

class NativeArrayOperatorUnitTest : public NativeArrayUnitTest {};

// -----------------------------------------------------------------------------

class NativeArrayUnaryOperatorUnitTest : public NativeArrayOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeArrayUnaryOperatorUnitTest, TestPositiveOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array;
      +array;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayUnaryOperatorUnitTest, TestNegationOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array;
      -array;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayUnaryOperatorUnitTest, TestIncrementOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array;
      ++array;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayUnaryOperatorUnitTest, TestDecrementOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array;
      --array;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayUnaryOperatorUnitTest, TestLogicalNotOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array;
      !array;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayUnaryOperatorUnitTest, TestBitwiseNotOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array;
      ~array;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

class NativeArrayBinaryOperatorUnitTest : public NativeArrayOperatorUnitTest
{
public:
  static void F(bool)
  {
  }
};

// -----------------------------------------------------------------------------

TEST_F(NativeArrayBinaryOperatorUnitTest, TestAdditionOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 + array2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayBinaryOperatorUnitTest, TestSubtractionOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 - array2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayBinaryOperatorUnitTest, TestMultiplicationOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 * array2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayBinaryOperatorUnitTest, TestDivisionOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 / array2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayBinaryOperatorUnitTest, TestLogicalANDOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 && array2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayBinaryOperatorUnitTest, TestLogicalOROperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 || array2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayBinaryOperatorUnitTest, TestBitwiseANDOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 & array2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayBinaryOperatorUnitTest, TestBitwiseOROperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 | array2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayBinaryOperatorUnitTest, TestBitwiseXOROperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 ^ array2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayBinaryOperatorUnitTest, TestBitwiseLeftShiftOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 << array2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayBinaryOperatorUnitTest, TestBitwiseRightShiftOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      array1 >> array2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayBinaryOperatorUnitTest, TestEQOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      F(array1 == array2);
    }
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayBinaryOperatorUnitTest, TestNEQOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      F(array1 != array2);
    }
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayBinaryOperatorUnitTest, TestGTOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      F(array1 > array2);
    }
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayBinaryOperatorUnitTest, TestLTOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      F(array1 < array2);
    }
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayBinaryOperatorUnitTest, TestGTEOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      F(array1 >= array2);
    }
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayBinaryOperatorUnitTest, TestLTEOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      F(array1 <= array2);
    }
  );
}

// -----------------------------------------------------------------------------
