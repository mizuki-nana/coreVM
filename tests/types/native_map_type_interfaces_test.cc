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


class native_map_type_interfaces_test : public native_type_interfaces_test_base {};

// -----------------------------------------------------------------------------

TEST_F(native_map_type_interfaces_test, TestSize)
{
  corevm::types::native_map map {
    { 1, 1 },
    { 2, 2 },
    { 3, 3 },
  };

  corevm::types::native_type_handle operand = map;

  size_t expected_result = 3;

  this->apply_interface_on_single_operand_and_assert_result<size_t>(
    operand,
    corevm::types::interface_map_size,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_map_type_interfaces_test, TestEmpty)
{
  corevm::types::native_map map {
    { 1, 1 }
  };

  corevm::types::native_type_handle operand = map;

  bool expected_result = false;

  this->apply_interface_on_single_operand_and_assert_result<bool>(
    operand,
    corevm::types::interface_map_empty,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_map_type_interfaces_test, TestAt)
{
  corevm::types::native_map map {
    { 1, 101 }
  };

  corevm::types::native_type_handle operand = map;
  corevm::types::native_type_handle key = corevm::types::uint64(1);

  corevm::types::native_map::mapped_type expected_result = 101;

  this->apply_interface_on_two_operands_and_assert_result<corevm::types::native_map::mapped_type>(
    operand,
    key,
    corevm::types::interface_map_at,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_map_type_interfaces_test, TestPut)
{
  corevm::types::native_map map;
  corevm::types::native_type_handle operand = map;
  corevm::types::native_type_handle key = corevm::types::uint64(1);
  corevm::types::native_type_handle value = corevm::types::uint64(101);

  corevm::types::native_map expected_result {
    { 1, 101 }
  };

  this->apply_interface_on_three_operands_in_place_and_assert_result<corevm::types::native_map>(
    operand,
    key,
    value,
    corevm::types::interface_map_put,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_map_type_interfaces_test, TestErase)
{
  corevm::types::native_map map {
    { 1, 101 },
    { 2, 2002 },
  };

  corevm::types::native_type_handle operand = map;
  corevm::types::native_type_handle key = corevm::types::uint64(1);
  corevm::types::native_map expected_result {
    { 2, 2002 }
  };

  apply_interface_on_two_operands_in_place_and_assert_result<corevm::types::native_map>(
    operand,
    key,
    corevm::types::interface_map_erase,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_map_type_interfaces_test, TestClear)
{
  corevm::types::native_map map {
    { 1, 101 },
    { 2, 2002 },
  };

  corevm::types::native_type_handle operand = map;
  corevm::types::native_map expected_result;

  apply_interface_on_single_operand_in_place_and_assert_result<corevm::types::native_map>(
    operand,
    corevm::types::interface_map_clear,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_map_type_interfaces_test, TestSwap)
{
  corevm::types::native_map map;
  corevm::types::native_map other_map {
    { 1001, 2002 }
  };

  corevm::types::native_type_handle operand = map;
  corevm::types::native_type_handle other_operand = other_map;
  corevm::types::native_map expected_result = other_map;

  this->apply_interface_on_two_operands_in_place_and_assert_result<corevm::types::native_map>(
    operand,
    other_operand,
    corevm::types::interface_map_swap,
    expected_result
  );
}

// -----------------------------------------------------------------------------
