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
#include "../../include/types/errors.h"
#include "../../include/types/native_string.h"

#include <sneaker/testing/_unittest.h>


class native_string_unittest : public ::testing::Test {};


TEST_F(native_string_unittest, TestEmptyInitialization)
{
  const corevm::types::native_string str;

  ASSERT_EQ(true, str.empty());
  ASSERT_EQ(0, str.length());
  ASSERT_EQ(0, str.size());
}

TEST_F(native_string_unittest, TestInitializationWithCString)
{
  const char* s = "Hello world!";
  const corevm::types::native_string str(s);

  ASSERT_EQ(false, str.empty());
  ASSERT_EQ(strlen(s), str.length());
  ASSERT_EQ(strlen(s), str.size());

  ASSERT_STREQ(s, str.c_str());
}

TEST_F(native_string_unittest, TestInitialziationWithStdString)
{
  const std::string s("Hello world!");
  const corevm::types::native_string str(s);

  ASSERT_EQ(false, str.empty());
  ASSERT_EQ(s.length(), str.length());
  ASSERT_EQ(s.size(), str.size());

  ASSERT_STREQ(s.c_str(), str.c_str());
}

TEST_F(native_string_unittest, TestCopyConstructor)
{
  const corevm::types::native_string s("Hello world!");
  const corevm::types::native_string str = s;

  ASSERT_EQ(false, str.empty());
  ASSERT_EQ(s.length(), str.length());
  ASSERT_EQ(s.size(), str.size());

  ASSERT_STREQ(s.c_str(), str.c_str());
}

TEST_F(native_string_unittest, TestCopyConstructorOnStdString)
{
  const std::string s("Hello world!");
  const corevm::types::native_string str = s;

  ASSERT_EQ(false, str.empty());
  ASSERT_EQ(s.length(), str.length());
  ASSERT_EQ(s.size(), str.size());

  ASSERT_STREQ(s.c_str(), str.c_str());
}

TEST_F(native_string_unittest, TestCopyConstructorOnCString)
{
  const char* s = "Hello world!";
  const corevm::types::native_string str = s;

  ASSERT_EQ(false, str.empty());
  ASSERT_EQ(strlen(s), str.length());
  ASSERT_EQ(strlen(s), str.size());

  ASSERT_STREQ(s, str.c_str());
}

TEST_F(native_string_unittest, TestCopyConstructorOnIntegerType)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str = 123;
    },
    corevm::types::conversion_error
  );
}

TEST_F(native_string_unittest, TestConvertingToIntegerType)
{
  ASSERT_THROW(
    {
      int i = corevm::types::native_string("123");
      i++;
    },
    corevm::types::conversion_error
  );
}

TEST_F(native_string_unittest, TestAssignmentOperator)
{
  const corevm::types::native_string s("Hello world!");
  corevm::types::native_string str;
  str = s;

  ASSERT_EQ(false, str.empty());
  ASSERT_EQ(s.length(), str.length());
  ASSERT_EQ(s.size(), str.size());

  ASSERT_STREQ(s.c_str(), str.c_str());
}

TEST_F(native_string_unittest, TestAssignmentOperatorOnStdString)
{
  const std::string s("Hello world!");
  corevm::types::native_string str;
  str = s;

  ASSERT_EQ(false, str.empty());
  ASSERT_EQ(s.length(), str.length());
  ASSERT_EQ(s.size(), str.size());

  ASSERT_STREQ(s.c_str(), str.c_str());
}

TEST_F(native_string_unittest, TestAssignmentOperatorOnCString)
{
  const char* s ="Hello world!";
  corevm::types::native_string str;
  str = s;

  ASSERT_EQ(false, str.empty());
  ASSERT_EQ(strlen(s), str.length());

  ASSERT_STREQ(s, str.c_str());
}

TEST_F(native_string_unittest, TestEqualityBetweenEmptyStrings)
{
  corevm::types::native_string str1;
  corevm::types::native_string str2;

  ASSERT_TRUE(str1 == str1);
  ASSERT_TRUE(str2 == str2);

  ASSERT_TRUE(str1 == str2);
}

TEST_F(native_string_unittest, TestEqualityBetweenNonEmptyStrings)
{
  corevm::types::native_string str1("Hello world!");
  corevm::types::native_string str2("Hello");

  ASSERT_FALSE(str1 == str2);
  ASSERT_TRUE(str1 != str2);
}

TEST_F(native_string_unittest, TestEqualityBetweenIdenticalStrings)
{
  corevm::types::native_string str1("Hello world!");
  corevm::types::native_string str2("Hello world!");

  ASSERT_TRUE(str1 == str1);
  ASSERT_TRUE(str2 == str2);

  ASSERT_TRUE(str1 == str2);
}


class native_string_operator_unittest : public native_string_unittest {};


class native_string_unary_operator_unittest : public native_string_operator_unittest {};


TEST_F(native_string_unary_operator_unittest, TestPositiveOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str("123");
      +str;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_string_unary_operator_unittest, TestNegationOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str("123");
      -str;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_string_unary_operator_unittest, TestIncrementOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str("123");
      ++str;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_string_unary_operator_unittest, TestDecrementOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str("123");
      --str;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_string_unary_operator_unittest, TestLogicalNotOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str("123");
      !str;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_string_unary_operator_unittest, TestBitwiseNotOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str("123");
      ~str;
    },
    corevm::types::invalid_operator_error
  );
}


class native_string_binary_operator_unittest : public native_string_operator_unittest
{
public:
  static void F(bool)
  {
  }
};


TEST_F(native_string_binary_operator_unittest, TestAdditionOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 + str2;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestSubtractionOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 - str2;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestMultiplicationOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 * str2;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestDivisionOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 / str2;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestLogicalANDOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 && str2;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestLogicalOROperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 || str2;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestBitwiseANDOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 & str2;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestBitwiseOROperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 | str2;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestBitwiseXOROperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 ^ str2;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestBitwiseLeftShiftOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 << str2;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestBitwiseRightShiftOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 >> str2;
    },
    corevm::types::invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestEQOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      F(str1 == str2);
    }
  );
}

TEST_F(native_string_binary_operator_unittest, TestNEQOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      F(str1 != str2);
    }
  );
}

TEST_F(native_string_binary_operator_unittest, TestGTOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      F(str1 > str2);
    }
  );
}

TEST_F(native_string_binary_operator_unittest, TestLTOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      F(str1 < str2);
    }
  );
}

TEST_F(native_string_binary_operator_unittest, TestGTEOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      F(str1 >= str2);
    }
  );
}

TEST_F(native_string_binary_operator_unittest, TestLTEOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      F(str1 <= str2);
    }
  );
}


class native_string_functionality_unittest : public native_string_unittest {};


TEST_F(native_string_functionality_unittest, TestAtSuccessful)
{
  const corevm::types::native_string str("Hello world!");

  char expected_result = 'w';
  char actual_result = str.at(6);

  ASSERT_EQ(expected_result, actual_result);
}

TEST_F(native_string_functionality_unittest, TestAtFailure)
{
  const corevm::types::native_string str("Hello world!");

  ASSERT_THROW(
    {
      str.at(100);
    },
    corevm::types::out_of_range_error
  );
}

TEST_F(native_string_functionality_unittest, TestInsertSuccessful)
{
  corevm::types::native_string str1("Hello ");
  const corevm::types::native_string str2("world!");

  corevm::types::native_string& actual_result = str1.insert(6, str2);
  const corevm::types::native_string expected_result("Hello world!");

  ASSERT_EQ(expected_result, actual_result);
}

TEST_F(native_string_functionality_unittest, TestInsertFailure)
{
  corevm::types::native_string str1("Hello ");
  const corevm::types::native_string str2("world!");

  ASSERT_THROW(
    {
      str1.insert(100, str2);
    },
    corevm::types::out_of_range_error
  );
}

TEST_F(native_string_functionality_unittest, TestEraseSuccessful)
{
  corevm::types::native_string str("Hello world");

  corevm::types::native_string expected_result = "Hello";
  corevm::types::native_string actual_result = str.erase(5);

  ASSERT_EQ(expected_result, actual_result);
}

TEST_F(native_string_functionality_unittest, TestEraseFailure)
{
  corevm::types::native_string str("Hello world");

  ASSERT_THROW(
    {
      str.erase(100);
    },
    corevm::types::out_of_range_error
  );
}

TEST_F(native_string_functionality_unittest, TestReplaceSuccessful)
{
  corevm::types::native_string str1("this is a test string");
  corevm::types::native_string str2("n example");

  corevm::types::native_string expected_result = "this is an example string";
  corevm::types::native_string actual_result = str1.replace(9, 5, str2);

  ASSERT_EQ(expected_result, actual_result);
}

TEST_F(native_string_functionality_unittest, TestReplaceFailure)
{
  corevm::types::native_string str1("Hello world");
  corevm::types::native_string str2("!!!");

  ASSERT_THROW(
    {
      str1.replace(100, 1, str2);
    },
    corevm::types::out_of_range_error
  );
}
