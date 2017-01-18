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
#include "ir/ir_parser_driver.h"

#include <gtest/gtest.h>

#include <fstream>


// -----------------------------------------------------------------------------

class IRParserDriverUnitTest : public ::testing::Test
{
protected:
  static const char* IR_STRING;

  void validate_parsed_module(const corevm::IRModule& module)
  {
    ASSERT_EQ("Dummy_IR", module.meta.name); 
    ASSERT_EQ(1, module.types.size());
    ASSERT_EQ("Person", module.types.front().name);
    ASSERT_EQ(1, module.closures.size());
    ASSERT_EQ("doNothing", module.closures.front().name);
  }
};

const char* IRParserDriverUnitTest::IR_STRING = 
  "\"module name\" : \"Dummy_IR\""
  "type Person {"
  "string name;"
  "}"
  ""
  "def void doNothing() {"
  "}";

// -----------------------------------------------------------------------------

class IRParserDriverFileBasedUnitTest : public IRParserDriverUnitTest
{
protected:
  static const char* PATH;

  virtual void SetUp()
  {
    std::ofstream f(PATH, std::ios::binary);
    f << IRParserDriverUnitTest::IR_STRING;
    f.close();
  }

  virtual void TearDown()
  {
    remove(PATH);
  }
};

const char* IRParserDriverFileBasedUnitTest::PATH = "Dummy_IR.txt";

// -----------------------------------------------------------------------------

TEST_F(IRParserDriverUnitTest, TestParseFromString)
{
  corevm::ir::IRParserDriver driver;
  const int res = driver.parse_from_string(IR_STRING);

  ASSERT_EQ(0, res);

  const corevm::IRModule& module = driver.module();
  validate_parsed_module(module); 
}

// -----------------------------------------------------------------------------

TEST_F(IRParserDriverFileBasedUnitTest, TestParseFromFile)
{
  corevm::ir::IRParserDriver driver;
  const int res = driver.parse_from_file(PATH);
  ASSERT_EQ(0, res);

  const corevm::IRModule& module = driver.module();
  validate_parsed_module(module); 
}

// -----------------------------------------------------------------------------
