#include <sneaker/testing/_unittest.h>
#include "../test_helper.h"
#include "../../include/types/errors.h"
#include "../../include/types/native_string.h"


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
  _ASSERT_THROW(
    {const corevm::types::native_string str = 123;},
    corevm::types::corevm_native_type_conversion_error
  );
}

TEST_F(native_string_unittest, TestConvertingToIntegerType)
{
  _ASSERT_THROW(
    {int i = corevm::types::native_string("123"); i++;},
    corevm::types::corevm_native_type_conversion_error
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
  _ASSERT_THROW(
    {const corevm::types::native_string str("123"); +str;},
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_string_unary_operator_unittest, TestNegationOperator)
{
  _ASSERT_THROW(
    {const corevm::types::native_string str("123"); -str;},
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_string_unary_operator_unittest, TestIncrementOperator)
{
  _ASSERT_THROW(
    {const corevm::types::native_string str("123"); ++str;},
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_string_unary_operator_unittest, TestDecrementOperator)
{
  _ASSERT_THROW(
    {const corevm::types::native_string str("123"); --str;},
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_string_unary_operator_unittest, TestLogicalNotOperator)
{
  _ASSERT_THROW(
    {const corevm::types::native_string str("123"); !str;},
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_string_unary_operator_unittest, TestBitwiseNotOperator)
{
  _ASSERT_THROW(
    {const corevm::types::native_string str("123"); ~str;},
    corevm::types::corevm_native_type_invalid_operator_error
  );
}


class native_string_binary_operator_unittest : public native_string_operator_unittest {};


TEST_F(native_string_binary_operator_unittest, TestAdditionOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 + str2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestSubtractionOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 - str2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestMultiplicationOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 * str2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestDivisionOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 / str2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestLogicalANDOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 && str2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestLogicalOROperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 || str2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestBitwiseANDOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 & str2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestBitwiseOROperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 | str2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestBitwiseXOROperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 ^ str2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestBitwiseLeftShiftOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 << str2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

TEST_F(native_string_binary_operator_unittest, TestBitwiseRightShiftOperator)
{
  _ASSERT_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      str1 >> str2;
    },
    corevm::types::corevm_native_type_invalid_operator_error
  );
}

void f(bool) {}

TEST_F(native_string_binary_operator_unittest, TestEQOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      f(str1 == str2);
    }
  );
}

TEST_F(native_string_binary_operator_unittest, TestNEQOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      f(str1 != str2);
    }
  );
}

TEST_F(native_string_binary_operator_unittest, TestGTOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      f(str1 > str2);
    }
  );
}

TEST_F(native_string_binary_operator_unittest, TestLTOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      f(str1 < str2);
    }
  );
}

TEST_F(native_string_binary_operator_unittest, TestGTEOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      f(str1 >= str2);
    }
  );
}

TEST_F(native_string_binary_operator_unittest, TestLTEOperator)
{
  ASSERT_NO_THROW(
    {
      const corevm::types::native_string str1("Hello ");
      const corevm::types::native_string str2("world!");
      f(str1 <= str2);
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

  _ASSERT_THROW(
    { str.at(100); },
    corevm::types::corevm_type_native_type_out_of_range_error
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

  _ASSERT_THROW(
    { str1.insert(100, str2); },
    corevm::types::corevm_type_native_type_out_of_range_error
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

  _ASSERT_THROW(
    { str.erase(100); },
    corevm::types::corevm_type_native_type_out_of_range_error
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

  _ASSERT_THROW(
    { str1.replace(100, 1, str2); },
    corevm::types::corevm_type_native_type_out_of_range_error
  );
}
