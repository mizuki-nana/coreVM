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
#include "types/native_map.h"

#include <gtest/gtest.h>

#include <stdexcept>


class NativeMapUnitTest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(NativeMapUnitTest, TestEmptyInitialization)
{
  const corevm::types::native_map map;

  ASSERT_EQ(true, map.empty());
  ASSERT_EQ(0, map.size());
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapUnitTest, TestCopyConstructor)
{
  corevm::types::native_map map1 {
    { 1, 1 },
    { 2, 2 },
    { 3, 3 },
  };

  corevm::types::native_map map2 = map1;

  ASSERT_EQ(false, map1.empty());
  ASSERT_EQ(false, map2.empty());
  ASSERT_EQ(map1.size(), map2.size());

  ASSERT_EQ(1, map2[1]);
  ASSERT_EQ(2, map2[2]);
  ASSERT_EQ(3, map2[3]);
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapUnitTest, TestCopyConstructorOnIntegerType)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map = 123;
    },
    corevm::types::conversion_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapUnitTest, TestConvertingToIntegerType)
{
  ASSERT_THROW(
    {
      int i = corevm::types::native_map(); i++;
    },
    corevm::types::conversion_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapUnitTest, TestAssignmentOperator)
{
  corevm::types::native_map map1 {
    { 1, 1 },
    { 2, 2 },
    { 3, 3 },
  };
  corevm::types::native_map map2;

  map2 = map1;

  ASSERT_EQ(false, map1.empty());
  ASSERT_EQ(false, map2.empty());
  ASSERT_EQ(map1.size(), map2.size());

  ASSERT_EQ(1, map2[1]);
  ASSERT_EQ(2, map2[2]);
  ASSERT_EQ(3, map2[3]);
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapUnitTest, TestEqualityBetweenEmptyInstances)
{
  const corevm::types::native_map map1;
  const corevm::types::native_map map2;

  ASSERT_TRUE(map1 == map1);
  ASSERT_TRUE(map2 == map2);

  ASSERT_TRUE(map1 == map2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapUnitTest, TestEqualityBetweenNonEmptyInstances)
{
  corevm::types::native_map map1 {
    { 1, 1 },
    { 2, 2 },
    { 3, 3 },
  };
  const corevm::types::native_map map2;

  ASSERT_FALSE(map1 == map2);
  ASSERT_TRUE(map1 != map2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapUnitTest, TestEqualityBetweenIdenticalInstances)
{
  corevm::types::native_map map1 {
    { 1, 1 },
    { 2, 2 },
    { 3, 3 },
  };
  corevm::types::native_map map2 {
    { 1, 1 },
    { 2, 2 },
    { 3, 3 },
  };

  ASSERT_TRUE(map1 == map2);
  ASSERT_TRUE(map2 == map1);

  ASSERT_FALSE(map1 != map2);
}

// -----------------------------------------------------------------------------

class NativeMapFunctionalityUnitTest : public NativeMapUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeMapFunctionalityUnitTest, TestAtSuccessful)
{
  corevm::types::native_map map {
    { 1, 1 }
  };

  ASSERT_EQ(1, map.at(1));
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapFunctionalityUnitTest, TestAtFailure)
{
  corevm::types::native_map map;

  ASSERT_THROW(
    {
      map.at(1);
    },
    corevm::types::out_of_range_error
  );
}

// -----------------------------------------------------------------------------

class NativeMapOperatorUnitTest : public NativeMapUnitTest {};

// -----------------------------------------------------------------------------

class NativeMapUnaryOperatorUnitTest : public NativeMapOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeMapUnaryOperatorUnitTest, TestPositiveOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map;
      +map;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapUnaryOperatorUnitTest, TestNegationOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map;
      -map;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapUnaryOperatorUnitTest, TestIncrementOperator)
{
  ASSERT_THROW(
    {
      corevm::types::native_map map;
      ++map;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapUnaryOperatorUnitTest, TestDecrementOperator)
{
  ASSERT_THROW(
    {
      corevm::types::native_map map;
      --map;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapUnaryOperatorUnitTest, TestLogicalNOTOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map;
      !map;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapUnaryOperatorUnitTest, TestBitwiseNOTOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map;
      ~map;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

class NativeMapBinaryOperatorUnitTest : public NativeMapOperatorUnitTest
{
public:
  static void F(bool)
  {
  }
};

// -----------------------------------------------------------------------------

TEST_F(NativeMapBinaryOperatorUnitTest, TestAdditionOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 + map2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapBinaryOperatorUnitTest, TestSubtractionOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 - map2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapBinaryOperatorUnitTest, TestMultiplicationOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 * map2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapBinaryOperatorUnitTest, TestDivisionOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 / map2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapBinaryOperatorUnitTest, TestLogicalANDOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 && map2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapBinaryOperatorUnitTest, TestLogicalOROperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 || map2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapBinaryOperatorUnitTest, TestBitwiseANDOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 & map2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapBinaryOperatorUnitTest, TestBitwiseOROperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 | map2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapBinaryOperatorUnitTest, TestBitwiseXOROperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 ^ map2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapBinaryOperatorUnitTest, TestBitwiseLeftShiftOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 << map2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapBinaryOperatorUnitTest, TestBitwiseRightShiftOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 >> map2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapBinaryOperatorUnitTest, TestEQOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      F(map1 == map2);
    }
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapBinaryOperatorUnitTest, TestNEQOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      F(map1 != map2);
    }
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapBinaryOperatorUnitTest, TestGTOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 > map2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapBinaryOperatorUnitTest, TestLTOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 < map2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapBinaryOperatorUnitTest, TestGTEOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 >= map2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeMapBinaryOperatorUnitTest, TestLTEOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 <= map2;
    },
    corevm::types::invalid_operator_error
  );
}

// -----------------------------------------------------------------------------
