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
#include "native_type_interfaces_test_base.h"

#include <string>


// -----------------------------------------------------------------------------

class NativeArrayTypeInterfacesTest : public NativeTypeInterfacesTestBase {};

// -----------------------------------------------------------------------------

TEST_F(NativeArrayTypeInterfacesTest, TestGetSize)
{
  corevm::types::native_array array {1, 2, 3};
  corevm::types::NativeTypeHandle operand = array;

  size_t expected_result = 3;

  this->apply_interface_on_single_operand_and_assert_result<size_t>(
    operand,
    corevm::types::interface_array_size,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayTypeInterfacesTest, TestEmpty)
{
  corevm::types::native_array array {1, 2, 3};
  corevm::types::NativeTypeHandle operand = array;

  bool expected_result = false;

  this->apply_interface_on_single_operand_and_assert_result<bool>(
    operand,
    corevm::types::interface_array_empty,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayTypeInterfacesTest, TestAt)
{
  corevm::types::native_array array {1, 2, 3};
  corevm::types::NativeTypeHandle operand = array;
  corevm::types::NativeTypeHandle index = corevm::types::uint32(1);

  corevm::types::native_array::value_type expected_result = 2;

  this->apply_interface_on_two_operands_and_assert_result<corevm::types::native_array::value_type>(
    operand,
    index,
    corevm::types::interface_array_at,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayTypeInterfacesTest, TestFront)
{
  corevm::types::native_array array {1, 2, 3};
  corevm::types::NativeTypeHandle operand = array;

  corevm::types::native_array::value_type expected_result = 1;

  this->apply_interface_on_single_operand_and_assert_result<corevm::types::native_array::value_type>(
    operand,
    corevm::types::interface_array_front,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayTypeInterfacesTest, TestBack)
{
  corevm::types::native_array array {1, 2, 3};
  corevm::types::NativeTypeHandle operand = array;

  corevm::types::native_array::value_type expected_result = 3;

  this->apply_interface_on_single_operand_and_assert_result<corevm::types::native_array::value_type>(
    operand,
    corevm::types::interface_array_back,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayTypeInterfacesTest, TestPut)
{
  corevm::types::native_array array {1, 2, 3};
  corevm::types::NativeTypeHandle operand = array;
  corevm::types::NativeTypeHandle index = corevm::types::uint32(2);
  corevm::types::NativeTypeHandle value = corevm::types::uint32(5);

  corevm::types::native_array expected_result { 1, 2, 5 };

  apply_interface_on_three_operands_in_place_and_assert_result<corevm::types::native_array>(
    operand,
    index,
    value,
    corevm::types::interface_array_put,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayTypeInterfacesTest, TestAppend)
{
  corevm::types::native_array array {1, 2, 3};
  corevm::types::NativeTypeHandle operand = array;
  corevm::types::NativeTypeHandle data = corevm::types::uint64(4);

  corevm::types::native_array expected_result {1, 2, 3, 4};

  apply_interface_on_two_operands_in_place_and_assert_result<corevm::types::native_array>(
    operand,
    data,
    corevm::types::interface_array_append,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayTypeInterfacesTest, TestErase)
{
  corevm::types::native_array array {1, 2, 3};
  corevm::types::NativeTypeHandle operand = array;
  corevm::types::NativeTypeHandle index = corevm::types::uint64(1);

  corevm::types::native_array expected_result {1, 3};

  apply_interface_on_two_operands_in_place_and_assert_result<corevm::types::native_array>(
    operand,
    index,
    corevm::types::interface_array_erase,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayTypeInterfacesTest, TestEraseOnInvalidIndex)
{
  corevm::types::native_array array {1, 2, 3};
  corevm::types::NativeTypeHandle operand = array;
  corevm::types::NativeTypeHandle index = corevm::types::uint64(3);

  ASSERT_THROW(
    {
      corevm::types::interface_array_erase(operand, index);
    },
    corevm::types::OutOfRangeError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayTypeInterfacesTest, TestPop)
{
  corevm::types::native_array array {1, 2, 3};
  corevm::types::NativeTypeHandle operand = array;

  corevm::types::native_array expected_result {1, 2};

  this->apply_interface_on_single_operand_and_assert_result<corevm::types::native_array>(
    operand,
    corevm::types::interface_array_pop,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayTypeInterfacesTest, TestSwap)
{
  corevm::types::native_array array {1, 2, 3};
  corevm::types::NativeTypeHandle operand = array;
  corevm::types::native_array other_array {4, 5, 6};
  corevm::types::NativeTypeHandle other_operand = other_array;

  corevm::types::native_array expected_result {4, 5, 6};

  apply_interface_on_two_operands_in_place_and_assert_result<corevm::types::native_array>(
    operand,
    other_operand,
    corevm::types::interface_array_swap,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeArrayTypeInterfacesTest, TestClear)
{
  corevm::types::native_array array {1, 2, 3};
  corevm::types::NativeTypeHandle operand = array;

  corevm::types::native_array expected_result;

  apply_interface_on_single_operand_in_place_and_assert_result<corevm::types::native_array>(
    operand,
    corevm::types::interface_array_clear,
    expected_result
  );
}

// -----------------------------------------------------------------------------
