#include <sneaker/testing/_unittest.h>
#include "../../include/types/interfaces.h"


class native_type_interfaces_test_base : public ::testing::Test {
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
    corevm::types::native_type_handle& operand, corevm::types::native_type_handle& other_operand, F func, T expected_result)
  {
    corevm::types::native_type_handle result;
    func(operand, other_operand, result);
    T actual_result = corevm::types::get_value_from_handle<T>(result);
    ASSERT_EQ(expected_result, actual_result);
  }

  template<typename T, typename F>
  void apply_interface_and_assert_result3(
    corevm::types::native_type_handle& operand, corevm::types::native_type_handle& operand2, corevm::types::native_type_handle& operand3, F func, T expected_result)
  {
    corevm::types::native_type_handle result;
    func(operand, operand2, operand3, result);
    T actual_result = corevm::types::get_value_from_handle<T>(result);
    ASSERT_EQ(expected_result, actual_result);
  }

  template<typename T, typename F>
  void apply_interface_and_assert_result4(
    corevm::types::native_type_handle& operand, corevm::types::native_type_handle& operand2,
    corevm::types::native_type_handle& operand3, corevm::types::native_type_handle& operand4, F func, T expected_result)
  {
    corevm::types::native_type_handle result;
    func(operand, operand2, operand3, operand4, result);
    T actual_result = corevm::types::get_value_from_handle<T>(result);
    ASSERT_EQ(expected_result, actual_result);
  }
};
