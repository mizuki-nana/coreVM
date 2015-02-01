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
#include "../../include/types/interfaces.h"

#include <sneaker/testing/_unittest.h>


class native_type_interfaces_test_base : public ::testing::Test
{
public:
  template<typename T, typename F>
  void apply_interface_and_assert_result(
    corevm::types::native_type_handle& operand, F func, T expected_value)
  {
    corevm::types::native_type_handle result;
    func(operand, result);
    T actual_result = corevm::types::get_value_from_handle<T>(result);
    ASSERT_EQ(expected_value, actual_result);
  }

  template<typename T, typename F>
  void apply_interface_and_assert_result2(
    corevm::types::native_type_handle& operand,
    corevm::types::native_type_handle& other_operand,
    F func,
    T expected_result)
  {
    corevm::types::native_type_handle result;
    func(operand, other_operand, result);
    T actual_result = corevm::types::get_value_from_handle<T>(result);
    ASSERT_EQ(expected_result, actual_result);
  }

  template<typename T, typename F>
  void apply_interface_and_assert_result3(
    corevm::types::native_type_handle& operand,
    corevm::types::native_type_handle& operand2,
    corevm::types::native_type_handle& operand3,
    F func,
    T expected_result)
  {
    corevm::types::native_type_handle result;
    func(operand, operand2, operand3, result);
    T actual_result = corevm::types::get_value_from_handle<T>(result);
    ASSERT_EQ(expected_result, actual_result);
  }

  template<typename T, typename F>
  void apply_interface_and_assert_result4(
    corevm::types::native_type_handle& operand,
    corevm::types::native_type_handle& operand2,
    corevm::types::native_type_handle& operand3,
    corevm::types::native_type_handle& operand4,
    F func,
    T expected_result)
  {
    corevm::types::native_type_handle result;
    func(operand, operand2, operand3, operand4, result);
    T actual_result = corevm::types::get_value_from_handle<T>(result);
    ASSERT_EQ(expected_result, actual_result);
  }
};
