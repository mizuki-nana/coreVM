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
#include "native_type_interfaces_test_base.h"

#include <string>


// -----------------------------------------------------------------------------

class NativeStringTypeInterfacesTest : public NativeTypeInterfacesTestBase
{
public:
  template<typename F>
  void apply_interface_and_assert_string_result(
    corevm::types::NativeTypeHandle& operand,
    F func,
    corevm::types::native_string& expected_result)
  {
    corevm::types::NativeTypeHandle result = func(operand);
    corevm::types::native_string actual_result =
      corevm::types::get_value_from_handle<corevm::types::native_string>(result);

    // Calling .c_str() to get raw C-string is only permitted in tests.
    const char* expected_string = expected_result.c_str();
    const char* actual_string = actual_result.c_str();
    ASSERT_STREQ(expected_string, actual_string);
  }
};

// -----------------------------------------------------------------------------

TEST_F(NativeStringTypeInterfacesTest, TestGetSize)
{
  corevm::types::NativeTypeHandle operand = corevm::types::string("Hello world!");

  size_t expected_value = strlen("Hello world!");

  this->apply_interface_on_single_operand_and_assert_result<size_t>(
    operand,
    corevm::types::interface_string_get_size,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringTypeInterfacesTest, TestClear)
{
  corevm::types::NativeTypeHandle operand = corevm::types::string("Hello world!");

  corevm::types::native_string expected_result = "";

  apply_interface_on_single_operand_in_place_and_assert_result(
    operand,
    corevm::types::interface_string_clear,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringTypeInterfacesTest, TestAt)
{
  corevm::types::NativeTypeHandle operand = corevm::types::string("Hello world!");
  corevm::types::NativeTypeHandle other_operand = corevm::types::uint32(4);

  char expected_value = 'o';

  this->apply_interface_on_two_operands_and_assert_result<char>(
    operand,
    other_operand,
    corevm::types::interface_string_at,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringTypeInterfacesTest, TestAt2)
{
  corevm::types::NativeTypeHandle operand = corevm::types::string("Hello world!");
  corevm::types::NativeTypeHandle other_operand = corevm::types::uint32(6);

  corevm::types::string expected_value("w");

  this->apply_interface_on_two_operands_and_assert_result<corevm::types::native_string>(
    operand,
    other_operand,
    corevm::types::interface_string_at_2,
    expected_value
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringTypeInterfacesTest, TestAppend)
{
  corevm::types::NativeTypeHandle operand = corevm::types::string("Hello ");
  corevm::types::NativeTypeHandle str = corevm::types::string("world!");

  corevm::types::native_string expected_result = "Hello world!";

  apply_interface_on_two_operands_in_place_and_assert_result<corevm::types::native_string>(
    operand,
    str,
    corevm::types::interface_string_append,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringTypeInterfacesTest, TestPushBack)
{
  corevm::types::NativeTypeHandle operand = corevm::types::string("Hello world");
  corevm::types::NativeTypeHandle c = corevm::types::int8('!');

  corevm::types::native_string expected_result = "Hello world!";

  apply_interface_on_two_operands_in_place_and_assert_result<corevm::types::native_string>(
    operand,
    c,
    corevm::types::interface_string_pushback,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringTypeInterfacesTest, TestInsertStr)
{
  corevm::types::NativeTypeHandle operand = corevm::types::string("Helloworld!");
  corevm::types::NativeTypeHandle pos = corevm::types::uint32(5);
  corevm::types::NativeTypeHandle str = corevm::types::string(" ");

  corevm::types::native_string expected_result = "Hello world!";

  this->apply_interface_on_three_operands_in_place_and_assert_result<corevm::types::native_string>(
    operand,
    pos,
    str,
    corevm::types::interface_string_insert_str,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringTypeInterfacesTest, TestInsertChar)
{
  corevm::types::NativeTypeHandle operand = corevm::types::string("Helloworld!");
  corevm::types::NativeTypeHandle pos = corevm::types::uint32(5);
  corevm::types::NativeTypeHandle c = corevm::types::int8(' ');

  corevm::types::native_string expected_result = "Hello world!";

  apply_interface_on_three_operands_in_place_and_assert_result<corevm::types::native_string>(
    operand,
    pos,
    c,
    corevm::types::interface_string_insert_char,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringTypeInterfacesTest, TestErase)
{
  corevm::types::NativeTypeHandle operand = corevm::types::string("Hello  world!");
  corevm::types::NativeTypeHandle pos = corevm::types::uint32(5);

  corevm::types::native_string expected_result = "Hello";

  apply_interface_on_two_operands_in_place_and_assert_result<corevm::types::native_string>(
    operand,
    pos,
    corevm::types::interface_string_erase,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringTypeInterfacesTest, TestErase2)
{
  corevm::types::NativeTypeHandle operand = corevm::types::string("Hello  world!");
  corevm::types::NativeTypeHandle pos = corevm::types::uint32(5);
  corevm::types::NativeTypeHandle len = corevm::types::uint32(1);

  corevm::types::native_string expected_result = "Hello world!";

  apply_interface_on_three_operands_in_place_and_assert_result<corevm::types::native_string>(
    operand,
    pos,
    len,
    corevm::types::interface_string_erase2,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringTypeInterfacesTest, TestReplaceStr)
{
  corevm::types::NativeTypeHandle operand = corevm::types::string("Hello world!");
  corevm::types::NativeTypeHandle pos = corevm::types::uint32(5);
  corevm::types::NativeTypeHandle len = corevm::types::uint32(1);
  corevm::types::NativeTypeHandle str = corevm::types::string("-");

  corevm::types::native_string expected_result = "Hello-world!";

  apply_interface_on_four_operands_in_place_and_assert_result<corevm::types::native_string>(
    operand,
    pos,
    len,
    str,
    corevm::types::interface_string_replace_str,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringTypeInterfacesTest, TestSwap)
{
  corevm::types::NativeTypeHandle operand = corevm::types::string("Hello world!");
  corevm::types::NativeTypeHandle str = corevm::types::string("Nice to meet you!");

  corevm::types::native_string expected_result = "Nice to meet you!";

  apply_interface_on_two_operands_in_place_and_assert_result<corevm::types::native_string>(
    operand,
    str,
    corevm::types::interface_string_swap,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringTypeInterfacesTest, TestSubstr)
{
  corevm::types::NativeTypeHandle operand = corevm::types::string("Hello world!");
  corevm::types::NativeTypeHandle pos = corevm::types::uint32(6);

  corevm::types::native_string expected_result = "world!";

  this->apply_interface_on_two_operands_and_assert_result<corevm::types::native_string>(
    operand,
    pos,
    corevm::types::interface_string_substr,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringTypeInterfacesTest, TestSubstr2)
{
  corevm::types::NativeTypeHandle operand = corevm::types::string("Hello world!");
  corevm::types::NativeTypeHandle pos = corevm::types::uint32(6);
  corevm::types::NativeTypeHandle len = corevm::types::uint32(5);

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

TEST_F(NativeStringTypeInterfacesTest, TestFind)
{
  corevm::types::NativeTypeHandle operand = corevm::types::string("Hello world!");
  corevm::types::NativeTypeHandle str = corevm::types::string("world");

  size_t expected_result = 6;

  this->apply_interface_on_two_operands_and_assert_result<size_t>(
    operand,
    str,
    corevm::types::interface_string_find,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringTypeInterfacesTest, TestFind2)
{
  corevm::types::NativeTypeHandle operand = corevm::types::string("Hello world!");
  corevm::types::NativeTypeHandle str = corevm::types::string("o");
  corevm::types::NativeTypeHandle pos = corevm::types::uint32(5);

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

TEST_F(NativeStringTypeInterfacesTest, TestRFind)
{
  corevm::types::NativeTypeHandle operand = corevm::types::string("Hello world!");
  corevm::types::NativeTypeHandle str = corevm::types::string("world");

  size_t expected_result = 6;

  this->apply_interface_on_two_operands_and_assert_result<size_t>(
    operand,
    str,
    corevm::types::interface_string_rfind,
    expected_result
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringTypeInterfacesTest, TestRFind2)
{
  corevm::types::NativeTypeHandle operand = corevm::types::string("Hello world!");
  corevm::types::NativeTypeHandle str = corevm::types::string("o");
  corevm::types::NativeTypeHandle pos = corevm::types::uint32(5);

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
