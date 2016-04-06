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
#include "types/errors.h"
#include "types/native_string.h"

#include <gtest/gtest.h>


class NativeStringUnitTest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnitTest, TestEmptyInitialization)
{
  const corevm::types::native_string str;

  ASSERT_EQ(true, str.empty());
  ASSERT_EQ(0, str.length());
  ASSERT_EQ(0, str.size());
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnitTest, TestInitializationWithCString)
{
  const char* s = "Hello world!";
  const corevm::types::native_string str(s);

  ASSERT_EQ(false, str.empty());
  ASSERT_EQ(strlen(s), str.length());
  ASSERT_EQ(strlen(s), str.size());

  ASSERT_STREQ(s, str.c_str());
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnitTest, TestInitialziationWithStdString)
{
  const std::string s("Hello world!");
  const corevm::types::native_string str(s);

  ASSERT_EQ(false, str.empty());
  ASSERT_EQ(s.length(), str.length());
  ASSERT_EQ(s.size(), str.size());

  ASSERT_STREQ(s.c_str(), str.c_str());
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnitTest, TestCopyConstructor)
{
  const corevm::types::native_string s("Hello world!");
  const corevm::types::native_string str = s;

  ASSERT_EQ(false, str.empty());
  ASSERT_EQ(s.length(), str.length());
  ASSERT_EQ(s.size(), str.size());

  ASSERT_STREQ(s.c_str(), str.c_str());
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnitTest, TestCopyConstructorOnStdString)
{
  const std::string s("Hello world!");
  const corevm::types::native_string str = s;

  ASSERT_EQ(false, str.empty());
  ASSERT_EQ(s.length(), str.length());
  ASSERT_EQ(s.size(), str.size());

  ASSERT_STREQ(s.c_str(), str.c_str());
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnitTest, TestCopyConstructorOnCString)
{
  const char* s = "Hello world!";
  const corevm::types::native_string str = s;

  ASSERT_EQ(false, str.empty());
  ASSERT_EQ(strlen(s), str.length());
  ASSERT_EQ(strlen(s), str.size());

  ASSERT_STREQ(s, str.c_str());
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnitTest, TestCopyConstructorOnIntegerType)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str = 123;
    },
    corevm::types::ConversionError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnitTest, TestConvertingToIntegerType)
{
  ASSERT_THROW(
    {
      int i = corevm::types::native_string("123");
      i++;
    },
    corevm::types::ConversionError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnitTest, TestAssignmentOperator)
{
  const corevm::types::native_string s("Hello world!");
  corevm::types::native_string str;
  str = s;

  ASSERT_EQ(false, str.empty());
  ASSERT_EQ(s.length(), str.length());
  ASSERT_EQ(s.size(), str.size());

  ASSERT_STREQ(s.c_str(), str.c_str());
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnitTest, TestAssignmentOperatorOnStdString)
{
  const std::string s("Hello world!");
  corevm::types::native_string str;
  str = s;

  ASSERT_EQ(false, str.empty());
  ASSERT_EQ(s.length(), str.length());
  ASSERT_EQ(s.size(), str.size());

  ASSERT_STREQ(s.c_str(), str.c_str());
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnitTest, TestAssignmentOperatorOnCString)
{
  const char* s ="Hello world!";
  corevm::types::native_string str;
  str = s;

  ASSERT_EQ(false, str.empty());
  ASSERT_EQ(strlen(s), str.length());

  ASSERT_STREQ(s, str.c_str());
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnitTest, TestEqualityBetweenEmptyStrings)
{
  corevm::types::native_string str1;
  corevm::types::native_string str2;

  ASSERT_TRUE(str1 == str1);
  ASSERT_TRUE(str2 == str2);

  ASSERT_TRUE(str1 == str2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnitTest, TestEqualityBetweenNonEmptyStrings)
{
  corevm::types::native_string str1("Hello world!");
  corevm::types::native_string str2("Hello");

  ASSERT_FALSE(str1 == str2);
  ASSERT_TRUE(str1 != str2);
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnitTest, TestEqualityBetweenIdenticalStrings)
{
  corevm::types::native_string str1("Hello world!");
  corevm::types::native_string str2("Hello world!");

  ASSERT_TRUE(str1 == str1);
  ASSERT_TRUE(str2 == str2);

  ASSERT_TRUE(str1 == str2);
}

// -----------------------------------------------------------------------------

class NativeStringOperatorUnitTest : public NativeStringUnitTest {};

// -----------------------------------------------------------------------------

class NativeStringUnaryOperatorUnitTest : public NativeStringOperatorUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnaryOperatorUnitTest, TestPositiveOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str("123");
      +str;
    },
    corevm::types::InvalidOperatorError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnaryOperatorUnitTest, TestNegationOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str("123");
      -str;
    },
    corevm::types::InvalidOperatorError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnaryOperatorUnitTest, TestIncrementOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str("123");
      ++str;
    },
    corevm::types::InvalidOperatorError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnaryOperatorUnitTest, TestDecrementOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str("123");
      --str;
    },
    corevm::types::InvalidOperatorError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnaryOperatorUnitTest, TestLogicalNotOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str("123");
      !str;
    },
    corevm::types::InvalidOperatorError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringUnaryOperatorUnitTest, TestBitwiseNotOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str("123");
      ~str;
    },
    corevm::types::InvalidOperatorError
  );
}

// -----------------------------------------------------------------------------

class NativeStringBinaryOperatorUnitTest : public NativeStringOperatorUnitTest
{
public:
  static void F(bool)
  {
  }
};

// -----------------------------------------------------------------------------

TEST_F(NativeStringBinaryOperatorUnitTest, TestAdditionOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 + str2;
    },
    corevm::types::InvalidOperatorError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringBinaryOperatorUnitTest, TestSubtractionOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 - str2;
    },
    corevm::types::InvalidOperatorError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringBinaryOperatorUnitTest, TestMultiplicationOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 * str2;
    },
    corevm::types::InvalidOperatorError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringBinaryOperatorUnitTest, TestDivisionOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 / str2;
    },
    corevm::types::InvalidOperatorError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringBinaryOperatorUnitTest, TestLogicalANDOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 && str2;
    },
    corevm::types::InvalidOperatorError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringBinaryOperatorUnitTest, TestLogicalOROperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 || str2;
    },
    corevm::types::InvalidOperatorError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringBinaryOperatorUnitTest, TestBitwiseANDOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 & str2;
    },
    corevm::types::InvalidOperatorError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringBinaryOperatorUnitTest, TestBitwiseOROperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 | str2;
    },
    corevm::types::InvalidOperatorError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringBinaryOperatorUnitTest, TestBitwiseXOROperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 ^ str2;
    },
    corevm::types::InvalidOperatorError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringBinaryOperatorUnitTest, TestBitwiseLeftShiftOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 << str2;
    },
    corevm::types::InvalidOperatorError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringBinaryOperatorUnitTest, TestBitwiseRightShiftOperator)
{
  ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 >> str2;
    },
    corevm::types::InvalidOperatorError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringBinaryOperatorUnitTest, TestEQOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      F(str1 == str2);
    }
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringBinaryOperatorUnitTest, TestNEQOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      F(str1 != str2);
    }
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringBinaryOperatorUnitTest, TestGTOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      F(str1 > str2);
    }
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringBinaryOperatorUnitTest, TestLTOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      F(str1 < str2);
    }
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringBinaryOperatorUnitTest, TestGTEOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      F(str1 >= str2);
    }
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringBinaryOperatorUnitTest, TestLTEOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      F(str1 <= str2);
    }
  );
}

// -----------------------------------------------------------------------------

class NativeStringFunctionalityUnitTest : public NativeStringUnitTest {};

// -----------------------------------------------------------------------------

TEST_F(NativeStringFunctionalityUnitTest, TestAtSuccessful)
{
  const corevm::types::native_string str("Hello world!");

  char expected_result = 'w';
  char actual_result = str.at(6);

  ASSERT_EQ(expected_result, actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringFunctionalityUnitTest, TestAtFailure)
{
  const corevm::types::native_string str("Hello world!");

  ASSERT_THROW(
    {
      str.at(100);
    },
    corevm::types::OutOfRangeError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringFunctionalityUnitTest, TestInsertSuccessful)
{
  corevm::types::native_string str1("Hello ");
  const corevm::types::native_string str2("world!");

  corevm::types::native_string& actual_result = str1.insert(6, str2);
  const corevm::types::native_string expected_result("Hello world!");

  ASSERT_EQ(expected_result, actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringFunctionalityUnitTest, TestInsertFailure)
{
  corevm::types::native_string str1("Hello ");
  const corevm::types::native_string str2("world!");

  ASSERT_THROW(
    {
      str1.insert(100, str2);
    },
    corevm::types::OutOfRangeError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringFunctionalityUnitTest, TestEraseSuccessful)
{
  corevm::types::native_string str("Hello world");

  corevm::types::native_string expected_result = "Hello";
  corevm::types::native_string actual_result = str.erase(5);

  ASSERT_EQ(expected_result, actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringFunctionalityUnitTest, TestEraseFailure)
{
  corevm::types::native_string str("Hello world");

  ASSERT_THROW(
    {
      str.erase(100);
    },
    corevm::types::OutOfRangeError
  );
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringFunctionalityUnitTest, TestReplaceSuccessful)
{
  corevm::types::native_string str1("this is a test string");
  corevm::types::native_string str2("n example");

  corevm::types::native_string expected_result = "this is an example string";
  corevm::types::native_string actual_result = str1.replace(9, 5, str2);

  ASSERT_EQ(expected_result, actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(NativeStringFunctionalityUnitTest, TestReplaceFailure)
{
  corevm::types::native_string str1("Hello world");
  corevm::types::native_string str2("!!!");

  ASSERT_THROW(
    {
      str1.replace(100, 1, str2);
    },
    corevm::types::OutOfRangeError
  );
}

// -----------------------------------------------------------------------------
