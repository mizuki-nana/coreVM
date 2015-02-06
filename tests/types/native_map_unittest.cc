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
#include "../../include/types/native_map.h"

#include <sneaker/testing/_unittest.h>

#include <stdexcept>


class native_map_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(native_map_unittest, TestEmptyInitialization)
{
  const corevm::types::native_map map;

  ASSERT_EQ(true, map.empty());
  ASSERT_EQ(0, map.size());
}

// -----------------------------------------------------------------------------

TEST_F(native_map_unittest, TestCopyConstructor)
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

TEST_F(native_map_unittest, TestCopyConstructorOnIntegerType)
{
  ASSERT_THROW(
    {
      const corevm::types::native_map map = 123;
    },
    corevm::types::conversion_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_map_unittest, TestConvertingToIntegerType)
{
  ASSERT_THROW(
    {
      int i = corevm::types::native_map(); i++;
    },
    corevm::types::conversion_error
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_map_unittest, TestAssignmentOperator)
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

TEST_F(native_map_unittest, TestEqualityBetweenEmptyInstances)
{
  const corevm::types::native_map map1;
  const corevm::types::native_map map2;

  ASSERT_TRUE(map1 == map1);
  ASSERT_TRUE(map2 == map2);

  ASSERT_TRUE(map1 == map2);
}

// -----------------------------------------------------------------------------

TEST_F(native_map_unittest, TestEqualityBetweenNonEmptyInstances)
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

TEST_F(native_map_unittest, TestEqualityBetweenIdenticalInstances)
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

class native_map_functionality_unittest : public native_map_unittest {};

// -----------------------------------------------------------------------------

TEST_F(native_map_functionality_unittest, TestAtSuccessful)
{
  corevm::types::native_map map {
    { 1, 1 }
  };

  ASSERT_EQ(1, map.at(1));
}

// -----------------------------------------------------------------------------

TEST_F(native_map_functionality_unittest, TestAtFailure)
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

class native_map_operator_unittest : public native_map_unittest {};

// -----------------------------------------------------------------------------

class native_map_unary_operator_unittest : public native_map_operator_unittest {};

// -----------------------------------------------------------------------------

TEST_F(native_map_unary_operator_unittest, TestPositiveOperator)
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

TEST_F(native_map_unary_operator_unittest, TestNegationOperator)
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

TEST_F(native_map_unary_operator_unittest, TestIncrementOperator)
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

TEST_F(native_map_unary_operator_unittest, TestDecrementOperator)
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

TEST_F(native_map_unary_operator_unittest, TestLogicalNOTOperator)
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

TEST_F(native_map_unary_operator_unittest, TestBitwiseNOTOperator)
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

class native_map_binary_operator_unittest : public native_map_operator_unittest
{
public:
  static void F(bool)
  {
  }
};

// -----------------------------------------------------------------------------

TEST_F(native_map_binary_operator_unittest, TestAdditionOperator)
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

TEST_F(native_map_binary_operator_unittest, TestSubtractionOperator)
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

TEST_F(native_map_binary_operator_unittest, TestMultiplicationOperator)
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

TEST_F(native_map_binary_operator_unittest, TestDivisionOperator)
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

TEST_F(native_map_binary_operator_unittest, TestLogicalANDOperator)
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

TEST_F(native_map_binary_operator_unittest, TestLogicalOROperator)
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

TEST_F(native_map_binary_operator_unittest, TestBitwiseANDOperator)
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

TEST_F(native_map_binary_operator_unittest, TestBitwiseOROperator)
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

TEST_F(native_map_binary_operator_unittest, TestBitwiseXOROperator)
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

TEST_F(native_map_binary_operator_unittest, TestBitwiseLeftShiftOperator)
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

TEST_F(native_map_binary_operator_unittest, TestBitwiseRightShiftOperator)
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

TEST_F(native_map_binary_operator_unittest, TestEQOperator)
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

TEST_F(native_map_binary_operator_unittest, TestNEQOperator)
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

TEST_F(native_map_binary_operator_unittest, TestGTOperator)
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

TEST_F(native_map_binary_operator_unittest, TestLTOperator)
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

TEST_F(native_map_binary_operator_unittest, TestGTEOperator)
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

TEST_F(native_map_binary_operator_unittest, TestLTEOperator)
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
