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


class native_string_type_interfaces_test : public native_type_interfaces_test_base
{
public:
  template<typename F>
  void apply_interface_and_assert_string_result(
    corevm::types::native_type_handle& operand, F func, corevm::types::native_string& expected_result)
  {
    corevm::types::native_type_handle result;
    func(operand, result);
    corevm::types::native_string actual_result = corevm::types::get_value_from_handle<corevm::types::native_string>(result);

    // Calling .c_str() to get raw C-string is only permitted in tests.
    const char* expected_string = expected_result.c_str();
    const char* actual_string = actual_result.c_str();
    ASSERT_STREQ(expected_string, actual_string);
  }
};

// -----------------------------------------------------------------------------

TEST_F(native_string_type_interfaces_test, TestGetSize)
{
  corevm::types::native_type_handle operand = corevm::types::string("Hello world!");

  size_t expected_value = strlen("Hello world!");

  this->apply_interface_on_single_operand_and_assert_result<size_t>(
    operand,
    corevm::types::interface_string_get_size,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_string_type_interfaces_test, TestClear)
{
  corevm::types::native_type_handle operand = corevm::types::string("Hello world!");

  corevm::types::native_string expected_result = "";

  this->apply_interface_on_single_operand_and_assert_result(
    operand,
    corevm::types::interface_string_clear,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_string_type_interfaces_test, TestAt)
{
  corevm::types::native_type_handle operand = corevm::types::string("Hello world!");
  corevm::types::native_type_handle other_operand = corevm::types::uint32(4);

  char expected_value = 'o';

  this->apply_interface_on_two_operands_and_assert_result<size_t>(
    operand,
    other_operand,
    corevm::types::interface_string_at,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_string_type_interfaces_test, TestAppend)
{
  corevm::types::native_type_handle operand = corevm::types::string("Hello ");
  corevm::types::native_type_handle str = corevm::types::string("world!");

  corevm::types::native_string expected_result = "Hello world!";

  this->apply_interface_on_two_operands_and_assert_result<corevm::types::native_string>(
    operand,
    str,
    corevm::types::interface_string_append,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_string_type_interfaces_test, TestPushBack)
{
  corevm::types::native_type_handle operand = corevm::types::string("Hello world");
  corevm::types::native_type_handle c = corevm::types::int8('!');

  corevm::types::native_string expected_result = "Hello world!";

  this->apply_interface_on_two_operands_and_assert_result<corevm::types::native_string>(
    operand,
    c,
    corevm::types::interface_string_pushback,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_string_type_interfaces_test, TestInsertStr)
{
  corevm::types::native_type_handle operand = corevm::types::string("Helloworld!");
  corevm::types::native_type_handle pos = corevm::types::uint32(5);
  corevm::types::native_type_handle str = corevm::types::string(" ");

  corevm::types::native_string expected_result = "Hello world!";

  this->apply_interface_on_three_operands_and_assert_result<corevm::types::native_string>(
    operand,
    pos,
    str,
    corevm::types::interface_string_insert_str,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_string_type_interfaces_test, TestInsertChar)
{
  corevm::types::native_type_handle operand = corevm::types::string("Helloworld!");
  corevm::types::native_type_handle pos = corevm::types::uint32(5);
  corevm::types::native_type_handle c = corevm::types::int8(' ');

  corevm::types::native_string expected_result = "Hello world!";

  this->apply_interface_on_three_operands_and_assert_result<corevm::types::native_string>(
    operand,
    pos,
    c,
    corevm::types::interface_string_insert_char,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_string_type_interfaces_test, TestErase)
{
  corevm::types::native_type_handle operand = corevm::types::string("Hello  world!");
  corevm::types::native_type_handle pos = corevm::types::uint32(5);

  corevm::types::native_string expected_result = "Hello";

  this->apply_interface_on_two_operands_and_assert_result<corevm::types::native_string>(
    operand,
    pos,
    corevm::types::interface_string_erase,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_string_type_interfaces_test, TestErase2)
{
  corevm::types::native_type_handle operand = corevm::types::string("Hello  world!");
  corevm::types::native_type_handle pos = corevm::types::uint32(5);
  corevm::types::native_type_handle len = corevm::types::uint32(1);

  corevm::types::native_string expected_result = "Hello world!";

  this->apply_interface_on_three_operands_and_assert_result<corevm::types::native_string>(
    operand,
    pos,
    len,
    corevm::types::interface_string_erase2,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_string_type_interfaces_test, TestReplaceStr)
{
  corevm::types::native_type_handle operand = corevm::types::string("Hello world!");
  corevm::types::native_type_handle pos = corevm::types::uint32(5);
  corevm::types::native_type_handle len = corevm::types::uint32(1);
  corevm::types::native_type_handle str = corevm::types::string("-");

  corevm::types::native_string expected_result = "Hello-world!";

  this->apply_interface_on_four_operands_and_assert_result<corevm::types::native_string>(
    operand,
    pos,
    len,
    str,
    corevm::types::interface_string_replace_str,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_string_type_interfaces_test, TestSwap)
{
  corevm::types::native_type_handle operand = corevm::types::string("Hello world!");
  corevm::types::native_type_handle str = corevm::types::string("Nice to meet you!");

  corevm::types::native_string expected_result = "Nice to meet you!";

  this->apply_interface_on_two_operands_and_assert_result<corevm::types::native_string>(
    operand,
    str,
    corevm::types::interface_string_swap,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_string_type_interfaces_test, TestSubstr)
{
  corevm::types::native_type_handle operand = corevm::types::string("Hello world!");
  corevm::types::native_type_handle pos = corevm::types::uint32(6);

  corevm::types::native_string expected_result = "world!";

  this->apply_interface_on_two_operands_and_assert_result<corevm::types::native_string>(
    operand,
    pos,
    corevm::types::interface_string_substr,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_string_type_interfaces_test, TestSubstr2)
{
  corevm::types::native_type_handle operand = corevm::types::string("Hello world!");
  corevm::types::native_type_handle pos = corevm::types::uint32(6);
  corevm::types::native_type_handle len = corevm::types::uint32(5);

  corevm::types::native_string expected_result = "world";

  this->apply_interface_on_three_operands_and_assert_result<corevm::types::native_string>(
    operand,
    pos,
    len,
    corevm::types::interface_string_substr2,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_string_type_interfaces_test, TestFind)
{
  corevm::types::native_type_handle operand = corevm::types::string("Hello world!");
  corevm::types::native_type_handle str = corevm::types::string("world");

  size_t expected_result = 6;

  this->apply_interface_on_two_operands_and_assert_result<size_t>(
    operand,
    str,
    corevm::types::interface_string_find,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_string_type_interfaces_test, TestFind2)
{
  corevm::types::native_type_handle operand = corevm::types::string("Hello world!");
  corevm::types::native_type_handle str = corevm::types::string("o");
  corevm::types::native_type_handle pos = corevm::types::uint32(5);

  size_t expected_result = 7;

  this->apply_interface_on_three_operands_and_assert_result<size_t>(
    operand,
    str,
    pos,
    corevm::types::interface_string_find2,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_string_type_interfaces_test, TestRFind)
{
  corevm::types::native_type_handle operand = corevm::types::string("Hello world!");
  corevm::types::native_type_handle str = corevm::types::string("world");

  size_t expected_result = 6;

  this->apply_interface_on_two_operands_and_assert_result<size_t>(
    operand,
    str,
    corevm::types::interface_string_rfind,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(native_string_type_interfaces_test, TestRFind2)
{
  corevm::types::native_type_handle operand = corevm::types::string("Hello world!");
  corevm::types::native_type_handle str = corevm::types::string("o");
  corevm::types::native_type_handle pos = corevm::types::uint32(5);

  size_t expected_result = 4;

  this->apply_interface_on_three_operands_and_assert_result<size_t>(
    operand,
    str,
    pos,
    corevm::types::interface_string_rfind2,
    expected_result
  );
}

// -----------------------------------------------------------------------------
