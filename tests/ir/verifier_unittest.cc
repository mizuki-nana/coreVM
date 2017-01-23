/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2017 Yanzheng Li

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
#include "ir/verifier.h"
#include "ir/format.h"
#include "ir/ir_module_index.h"
#include "ir/ir_parser_driver.h"

#include <gtest/gtest.h>


// -----------------------------------------------------------------------------

class VerifierUnitTest : public ::testing::Test
{
protected:
  void check_verification(const char* IR_STRING, const char* expected_msg = NULL)
  {
    corevm::ir::IRParserDriver driver;
    const int res = driver.parse_from_string(IR_STRING);

    ASSERT_EQ(0, res);

    const corevm::IRModule& module = driver.module();

    corevm::ir::Verifier verifier(module);

    std::string msg;
    if (!expected_msg)
    {
      ASSERT_EQ(true, verifier.run(msg));
    }
    else
    {
      ASSERT_EQ(false, verifier.run(msg));
      ASSERT_STREQ(expected_msg, msg.c_str());
    }
  }
};

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestInitialization)
{
  corevm::IRModule module;
  corevm::ir::Verifier verifier(module);
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithSingleValidTypeDefinition)
{
  const char* IR_STRING = 
    "type Person {"
    "    string name;"
    "}";

  check_verification(IR_STRING);
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithSingleInvalidTypeDefinition)
{
  const char* IR_STRING = 
    "type Person {"
    "    Address address;"
    "}";

  check_verification(IR_STRING,
    "Unknown type used for type member Person::address");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithTwoValidTypeDefinitions)
{
  const char* IR_STRING = 
    "type Person {"
    "    string name;"
    "    Location location;"
    "}"
    ""
    "type Location {"
    "    string address;"
    "    dpf lat;"
    "    dpf lon;"
    "}";

  check_verification(IR_STRING);
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneInvalidTypeDefinition)
{
  const char* IR_STRING = 
    "type Person {"
    "    string name;"
    "    Location location;"
    "}"
    ""
    "type Location {"
    "    string address;"
    "    dpf lat;"
    "    dpf lon;"
    "    Zipcode zipcode;"
    "}";

  check_verification(IR_STRING,
    "Unknown type used for type member Location::zipcode");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestWithOneInvalidTypeDefinitionWithRecursiveInvalidTypeUsage)
{
  const char* IR_STRING = 
    "type Person {"
    "    string name;"
    "    Location location;"
    "}"
    ""
    "type Location {"
    "    string address;"
    "    dpf lat;"
    "    dpf lon;"
    "    array [ 10 * Address ] addresses;"
    "}"
    ""
    "type Address {"
    "    string street;"
    "    string city;"
    "    string state;"
    "    string country;"
    "    array [ 2 * Zipcode ] zipcodes;"
    "}";

  check_verification(IR_STRING,
    "Unknown type used for type member Address::zipcodes");
}

// -----------------------------------------------------------------------------

TEST_F(VerifierUnitTest, TestGetIndex)
{
  const char* IR_STRING = 
    "type Person {"
    "    string name;"
    "}";

  corevm::ir::IRParserDriver driver;
  const int res = driver.parse_from_string(IR_STRING);
  ASSERT_EQ(0, res);
  const corevm::IRModule& module = driver.module();
  corevm::ir::Verifier verifier(module);

  std::unique_ptr<corevm::ir::IRModuleIndex> index;
  ASSERT_EQ(nullptr, index);

  verifier.get_index(index);

  ASSERT_NE(nullptr, index);

  ASSERT_EQ(1, index->type_index.size());
}

// -----------------------------------------------------------------------------
