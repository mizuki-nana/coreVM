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
#include "runtime/native_types_pool.h"
#include "types/native_type_value.h"

#include <gtest/gtest.h>

#include <cstdint>
#include <sstream>


class NativeTypesPoolUnitTest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(NativeTypesPoolUnitTest, TestInitialization)
{
  corevm::runtime::NativeTypesPool pool;

  ASSERT_EQ(0, pool.size());
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypesPoolUnitTest, TestCreateAndAccess)
{
  corevm::runtime::NativeTypesPool pool;

  auto created_type_val = pool.create();

  ASSERT_NE(nullptr, created_type_val);

  ASSERT_EQ(1, pool.size());

  corevm::types::NativeTypeValue& type_val = pool.at(created_type_val);

  int value = 8;
  type_val = corevm::types::int8(value);

  corevm::types::NativeTypeValue& type_val2 = pool.at(created_type_val);

  int actual_value = corevm::types::get_intrinsic_value_from_type_value<int8_t>(type_val2);

  ASSERT_EQ(value, actual_value);
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypesPoolUnitTest, TestInvalidAccess)
{
  corevm::runtime::NativeTypesPool pool;

  corevm::types::NativeTypeValue type_val;

  ASSERT_THROW(
    {
      pool.at(&type_val);
    },
    corevm::runtime::NativeTypeValueNotFoundError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypesPoolUnitTest, TestCreateAndErase)
{
  corevm::runtime::NativeTypesPool pool;

  auto created_type_val = pool.create();

  ASSERT_NE(nullptr, created_type_val);

  ASSERT_EQ(1, pool.size());

  pool.erase(created_type_val);

  ASSERT_EQ(0, pool.size());

  // Second time calling `.erase`
  ASSERT_THROW(
    {
      pool.erase(created_type_val);
    },
    corevm::runtime::NativeTypeValueNotFoundError
  );

  ASSERT_THROW(
    {
      pool.at(created_type_val);
    },
    corevm::runtime::NativeTypeValueNotFoundError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeTypesPoolUnitTest, TestOutputStream)
{
  corevm::runtime::NativeTypesPool pool;
  pool.create();
  pool.create();
  pool.create();

  ASSERT_EQ(3, pool.size());

  std::stringstream ss;
  ss << pool;

  ASSERT_NE(0, ss.str().size());
}

// -----------------------------------------------------------------------------
