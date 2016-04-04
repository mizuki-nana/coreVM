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
#include "types/interfaces.h"
#include "types/native_type_handle.h"

#include <gtest/gtest.h>


class NativeTypeInterfacesTestBase : public ::testing::Test
{
public:
  template<typename T, typename F>
  void apply_interface_on_single_operand_and_assert_result(
    corevm::types::NativeTypeHandle& operand, F func, T expected_value)
  {
    corevm::types::NativeTypeHandle result = func(operand);
    T actual_result = corevm::types::get_value_from_handle<T>(result);
    ASSERT_EQ(expected_value, actual_result);
  }

  template<typename T, typename F>
  void apply_interface_on_single_operand_in_place_and_assert_result(
    corevm::types::NativeTypeHandle& operand, F func, T expected_value)
  {
    func(operand);
    T& actual_result = corevm::types::get_value_ref_from_handle<T>(operand);
    ASSERT_EQ(expected_value, actual_result);
  }

  template<typename T, typename F>
  void apply_interface_on_two_operands_and_assert_result(
    corevm::types::NativeTypeHandle& operand,
    corevm::types::NativeTypeHandle& other_operand,
    F func,
    T expected_result)
  {
    corevm::types::NativeTypeHandle result = func(operand, other_operand);
    T actual_result = corevm::types::get_value_from_handle<T>(result);
    ASSERT_EQ(expected_result, actual_result);
  }

  template<typename T, typename F>
  void apply_interface_on_two_operands_in_place_and_assert_result(
    corevm::types::NativeTypeHandle& operand,
    corevm::types::NativeTypeHandle& other_operand,
    F func,
    T expected_result)
  {
    func(operand, other_operand);
    const T& actual_result = corevm::types::get_value_ref_from_handle<T>(operand);
    ASSERT_EQ(expected_result, actual_result);
  }

  template<typename T, typename F>
  void apply_interface_on_three_operands_and_assert_result(
    corevm::types::NativeTypeHandle& operand,
    corevm::types::NativeTypeHandle& operand2,
    corevm::types::NativeTypeHandle& operand3,
    F func,
    T expected_result)
  {
    corevm::types::NativeTypeHandle result =
      func(operand, operand2, operand3);
    T actual_result = corevm::types::get_value_from_handle<T>(result);
    ASSERT_EQ(expected_result, actual_result);
  }

  template<typename T, typename F>
  void apply_interface_on_three_operands_in_place_and_assert_result(
    corevm::types::NativeTypeHandle& operand,
    corevm::types::NativeTypeHandle& operand2,
    corevm::types::NativeTypeHandle& operand3,
    F func,
    T expected_result)
  {
    func(operand, operand2, operand3);
    T& actual_result = corevm::types::get_value_ref_from_handle<T>(operand);
    ASSERT_EQ(expected_result, actual_result);
  }

  template<typename T, typename F>
  void apply_interface_on_four_operands_and_assert_result(
    corevm::types::NativeTypeHandle& operand,
    corevm::types::NativeTypeHandle& operand2,
    corevm::types::NativeTypeHandle& operand3,
    corevm::types::NativeTypeHandle& operand4,
    F func,
    T expected_result)
  {
    corevm::types::NativeTypeHandle result =
      func(operand, operand2, operand3, operand4);
    T actual_result = corevm::types::get_value_from_handle<T>(result);
    ASSERT_EQ(expected_result, actual_result);
  }

  template<typename T, typename F>
  void apply_interface_on_four_operands_in_place_and_assert_result(
    corevm::types::NativeTypeHandle& operand,
    corevm::types::NativeTypeHandle& operand2,
    corevm::types::NativeTypeHandle& operand3,
    corevm::types::NativeTypeHandle& operand4,
    F func,
    T expected_result)
  {
    func(operand, operand2, operand3, operand4);
    T& actual_result = corevm::types::get_value_ref_from_handle<T>(operand);
    ASSERT_EQ(expected_result, actual_result);
  }
};
