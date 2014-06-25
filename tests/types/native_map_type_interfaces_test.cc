#include <string>
#include "native_type_interfaces_test_base.h"


class native_map_type_interfaces_test : public native_type_interfaces_test_base {};


TEST_F(native_map_type_interfaces_test, TestSize)
{
  corevm::types::native_map map;
  map[1] = 1;
  map[2] = 2;
  map[3] = 3;

  corevm::types::native_type_handle operand = map;

  size_t expected_result = 3;
  this->apply_interface_and_assert_result<size_t>(operand, corevm::types::interface_map_size, expected_result);
}

TEST_F(native_map_type_interfaces_test, TestEmpty)
{
  corevm::types::native_map map;
  map[1] = 1;

  corevm::types::native_type_handle operand = map;

  bool expected_result = false;
  this->apply_interface_and_assert_result<bool>(operand, corevm::types::interface_map_empty, expected_result);
}

TEST_F(native_map_type_interfaces_test, TestAt)
{
  corevm::types::native_map map;
  map[1] = 101;

  corevm::types::native_type_handle operand = map;
  corevm::types::native_type_handle key = corevm::types::uint64(1);

  corevm::types::native_map::mapped_type expected_result = 101;
  this->apply_interface_and_assert_result2<corevm::types::native_map::mapped_type>(operand, key, corevm::types::interface_map_at, expected_result);
}

TEST_F(native_map_type_interfaces_test, TestPut)
{
  corevm::types::native_map map;
  corevm::types::native_type_handle operand = map;
  corevm::types::native_type_handle key = corevm::types::uint64(1);
  corevm::types::native_type_handle value = corevm::types::uint64(101);

  corevm::types::native_map expected_result;
  expected_result[1] = 101;
  this->apply_interface_and_assert_result3<corevm::types::native_map>(operand, key, value, corevm::types::interface_map_put, expected_result);
}

TEST_F(native_map_type_interfaces_test, TestErase)
{
  corevm::types::native_map map;
  map[1] = 101;
  map[2] = 2002;

  corevm::types::native_type_handle operand = map;
  corevm::types::native_type_handle key = corevm::types::uint64(1);
  corevm::types::native_map expected_result;
  expected_result[2] = 2002;
  this->apply_interface_and_assert_result2<corevm::types::native_map>(operand, key, corevm::types::interface_map_erase, expected_result); 
}

TEST_F(native_map_type_interfaces_test, TestClear)
{
  corevm::types::native_map map;
  map[1] = 101;
  map[2] = 2002;

  corevm::types::native_type_handle operand = map;
  corevm::types::native_map expected_result;
  this->apply_interface_and_assert_result<corevm::types::native_map>(operand, corevm::types::interface_map_clear, expected_result);
}

TEST_F(native_map_type_interfaces_test, TestSwap)
{
  corevm::types::native_map map;
  corevm::types::native_map other_map;
  other_map[1001] = 2002;

  corevm::types::native_type_handle operand = map;
  corevm::types::native_type_handle other_operand = other_map;
  corevm::types::native_map expected_result = other_map;
  this->apply_interface_and_assert_result2<corevm::types::native_map>(operand, other_operand, corevm::types::interface_map_swap, expected_result); 
}
