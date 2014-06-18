#include <stdexcept>
#include <sneaker/testing/_unittest.h>
#include "../test_helper.h"
#include "../../include/types/errors.h"
#include "../../include/types/native_map.h"


class native_map_unittest : public ::testing::Test {};


TEST_F(native_map_unittest, TestEmptyInitialization)
{
  const corevm::types::native_map map;

  ASSERT_EQ(true, map.empty());
  ASSERT_EQ(0, map.size());
}

TEST_F(native_map_unittest, TestCopyConstructor)
{
  corevm::types::native_map map1;

  map1[1] = 1;
  map1[2] = 2;
  map1[3] = 3;

  corevm::types::native_map map2 = map1;

  ASSERT_EQ(false, map1.empty());
  ASSERT_EQ(false, map2.empty());
  ASSERT_EQ(map1.size(), map2.size());

  ASSERT_EQ(1, map2[1]);
  ASSERT_EQ(2, map2[2]);
  ASSERT_EQ(3, map2[3]);
}

TEST_F(native_map_unittest, TestCopyConstructorOnIntegerType)
{
  _ASSERT_THROW(
    {const corevm::types::native_map map = 123;},
    corevm::types::corevm_native_type_conversion_error
  );
}

TEST_F(native_map_unittest, TestConvertingToIntegerType)
{
  _ASSERT_THROW(
    {int i = corevm::types::native_map(); i++;},
    corevm::types::corevm_native_type_conversion_error
  );
}

TEST_F(native_map_unittest, TestAssignmentOperator)
{
  corevm::types::native_map map1;
  corevm::types::native_map map2;

  map1[1] = 1;
  map1[2] = 2;
  map1[3] = 3;

  map2 = map1;

  ASSERT_EQ(false, map1.empty());
  ASSERT_EQ(false, map2.empty());
  ASSERT_EQ(map1.size(), map2.size());

  ASSERT_EQ(1, map2[1]);
  ASSERT_EQ(2, map2[2]);
  ASSERT_EQ(3, map2[3]);
}

TEST_F(native_map_unittest, TestEqualityBetweenEmptyInstances)
{
  const corevm::types::native_map map1;
  const corevm::types::native_map map2;

  ASSERT_TRUE(map1 == map1);
  ASSERT_TRUE(map2 == map2);

  ASSERT_TRUE(map1 == map2);
}

TEST_F(native_map_unittest, TestEqualityBetweenNonEmptyInstances)
{
  corevm::types::native_map map1;
  const corevm::types::native_map map2;

  map1[1] = 1;
  map1[2] = 2;
  map1[3] = 3;

  ASSERT_FALSE(map1 == map2);
  ASSERT_TRUE(map1 != map2);
}

TEST_F(native_map_unittest, TestEqualityBetweenIdenticalInstances)
{
  corevm::types::native_map map1;
  corevm::types::native_map map2;

  map1[1] = 1;
  map1[2] = 2;
  map1[3] = 3;

  map2[1] = 1;
  map2[2] = 2;
  map2[3] = 3;

  ASSERT_TRUE(map1 == map2);
  ASSERT_TRUE(map2 == map1);

  ASSERT_FALSE(map1 != map2);
}


class native_map_functionality_unittest : public native_map_unittest {};


TEST_F(native_map_functionality_unittest, TestPutAndGet)
{
  corevm::types::native_map map;

  map[1] = 1;
  map[2] = 2;
  map[3] = 3;

  ASSERT_EQ(3, map.size());

  ASSERT_EQ(1, map.at(1));
  ASSERT_EQ(2, map.at(2));
  ASSERT_EQ(3, map.at(3));
}


class native_map_operator_unittest : public native_map_unittest {};


class native_map_unary_operator_unittest : public native_map_operator_unittest {};


TEST_F(native_map_unary_operator_unittest, TestPositiveOperator)
{
  _ASSERT_THROW(
    {const corevm::types::native_map map; +map;},
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_map_unary_operator_unittest, TestNegationOperator)
{
  _ASSERT_THROW(
    {const corevm::types::native_map map; -map;},
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_map_unary_operator_unittest, TestIncrementOperator)
{
  _ASSERT_THROW(
    {const corevm::types::native_map map; ++map;},
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_map_unary_operator_unittest, TestDecrementOperator)
{
  _ASSERT_THROW(
    {const corevm::types::native_map map; --map;},
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_map_unary_operator_unittest, TestLogicalNOTOperator)
{
  _ASSERT_THROW(
    {const corevm::types::native_map map; !map;},
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_map_unary_operator_unittest, TestBitwiseNOTOperator)
{
  _ASSERT_THROW(
    {const corevm::types::native_map map; ~map;},
    corevm::types::corevm_native_type_invalid_operator_error
  );
}


class native_map_binary_operator_unittest : public native_map_operator_unittest {};


TEST_F(native_map_binary_operator_unittest, TestAdditionOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 + map2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_map_binary_operator_unittest, TestSubtractionOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 - map2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_map_binary_operator_unittest, TestMultiplicationOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 * map2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_map_binary_operator_unittest, TestDivisionOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 / map2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_map_binary_operator_unittest, TestLogicalANDOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 && map2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_map_binary_operator_unittest, TestLogicalOROperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 || map2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_map_binary_operator_unittest, TestBitwiseANDOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 & map2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_map_binary_operator_unittest, TestBitwiseOROperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 | map2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_map_binary_operator_unittest, TestBitwiseXOROperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 ^ map2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_map_binary_operator_unittest, TestBitwiseLeftShiftOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 << map2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_map_binary_operator_unittest, TestBitwiseRightShiftOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 >> map2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

void f(bool) {}

TEST_F(native_map_binary_operator_unittest, TestEQOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      f(map1 == map2);
    }
  );
}

TEST_F(native_map_binary_operator_unittest, TestNEQOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      f(map1 != map2);
    }
  );
}

TEST_F(native_map_binary_operator_unittest, TestGTOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 > map2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_map_binary_operator_unittest, TestLTOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 < map2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_map_binary_operator_unittest, TestGTEOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 >= map2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_map_binary_operator_unittest, TestLTEOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_map map1;
      const corevm::types::native_map map2;
      map1 <= map2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}
