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
#include "../../include/types/errors.h"
#include "../../include/types/native_array.h"
#include "../../include/types/types.h"

#include <sneaker/testing/_unittest.h>

#include <stdexcept>


class native_array_unittest : public ::testing::Test {
protected:
  const corevm::types::uint64::value_type fixture_element1 = static_cast<corevm::types::uint64::value_type>(1);
  const corevm::types::uint64::value_type fixture_element2 = static_cast<corevm::types::uint64::value_type>(2);
  const corevm::types::uint64::value_type fixture_element3 = static_cast<corevm::types::uint64::value_type>(3);
};


TEST_F(native_array_unittest, TestEmptyInitialization)
{
  const corevm::types::native_array array;

  ASSERT_EQ(true, array.empty());
  ASSERT_EQ(0, array.size());
}

TEST_F(native_array_unittest, TestCopyConstructor)
{
  const corevm::types::native_array array1 = {
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

TEST_F(native_array_unittest, TestCopyConstructorOnIntegerType)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array = 123;
    },
    corevm::types::conversion_error
  );
}

TEST_F(native_array_unittest, TestConvertingToIntegerType)
{
  ASSERT_THROW(
    {
      int i = corevm::types::native_array();
      i++;
    },
    corevm::types::conversion_error
  );
}

TEST_F(native_array_unittest, TestAssignmentOperator)
{
  const corevm::types::native_array array1 = {
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
  const corevm::types::native_array array1 = {
    fixture_element1,
    fixture_element2,
    fixture_element3
  };
  const corevm::types::native_array array2;

  ASSERT_FALSE(array1 == array2);
  ASSERT_TRUE(array1 != array2);
}

TEST_F(native_array_unittest, TestEqualityBetweenIdenticalArrays)
{
  const corevm::types::native_array array1 = {
    fixture_element1,
    fixture_element2,
    fixture_element3
  };
  const corevm::types::native_array array2 = {
    fixture_element1,
    fixture_element2,
    fixture_element3
  };

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

  ASSERT_THROW(
    {
      array.at(3);
    },
    corevm::types::out_of_range_error
  );
}

TEST_F(native_array_functionality_unittest, TestAtSuccessful)
{
  const corevm::types::native_array array = {1, 2, 3};

  corevm::types::native_array::value_type expected_result = 3;
  corevm::types::native_array::value_type actual_result = array.at(2);

  ASSERT_EQ(expected_result, actual_result);
}

TEST_F(native_array_functionality_unittest, TestAtFailure)
{
  const corevm::types::native_array array = {1, 2, 3};

  ASSERT_THROW(
    {
      array.at(100);
    },
    corevm::types::out_of_range_error
  );
}


class native_array_operator_unittest : public native_array_unittest {};


class native_array_unary_operator_unittest : public native_array_operator_unittest {};


TEST_F(native_array_unary_operator_unittest, TestPositiveOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array;
      +array;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_array_unary_operator_unittest, TestNegationOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array;
      -array;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_array_unary_operator_unittest, TestIncrementOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array;
      ++array;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_array_unary_operator_unittest, TestDecrementOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array;
      --array;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_array_unary_operator_unittest, TestLogicalNotOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array;
      !array;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_array_unary_operator_unittest, TestBitwiseNotOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_array array;
      ~array;
    },
    corevm::types::invalid_operator_error
  );
}


class native_array_binary_operator_unittest : public native_array_operator_unittest {
public:
  static void F(bool) {}
};


TEST_F(native_array_binary_operator_unittest, TestAdditionOperator)
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

TEST_F(native_array_binary_operator_unittest, TestSubtractionOperator)
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

TEST_F(native_array_binary_operator_unittest, TestMultiplicationOperator)
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

TEST_F(native_array_binary_operator_unittest, TestDivisionOperator)
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

TEST_F(native_array_binary_operator_unittest, TestLogicalANDOperator)
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

TEST_F(native_array_binary_operator_unittest, TestLogicalOROperator)
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

TEST_F(native_array_binary_operator_unittest, TestBitwiseANDOperator)
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

TEST_F(native_array_binary_operator_unittest, TestBitwiseOROperator)
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

TEST_F(native_array_binary_operator_unittest, TestBitwiseXOROperator)
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

TEST_F(native_array_binary_operator_unittest, TestBitwiseLeftShiftOperator)
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

TEST_F(native_array_binary_operator_unittest, TestBitwiseRightShiftOperator)
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

TEST_F(native_array_binary_operator_unittest, TestEQOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      F(array1 == array2);
    }
  );
}

TEST_F(native_array_binary_operator_unittest, TestNEQOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      F(array1 != array2);
    }
  );
}

TEST_F(native_array_binary_operator_unittest, TestGTOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      F(array1 > array2);
    }
  );
}

TEST_F(native_array_binary_operator_unittest, TestLTOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      F(array1 < array2);
    }
  );
}

TEST_F(native_array_binary_operator_unittest, TestGTEOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      F(array1 >= array2);
    }
  );
}

TEST_F(native_array_binary_operator_unittest, TestLTEOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_array array1;
      const corevm::types::native_array array2;
      F(array1 <= array2);
    }
  );
}
