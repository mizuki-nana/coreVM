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
#include "frontend/utils.h"
#include "runtime/instr.h"
#include "runtime/vector.h"

#include <sneaker/testing/_unittest.h>
#include <sneaker/json/json.h>
#include <sneaker/json/json_parser.h>
#include <sneaker/json/json_schema.h>

#include <string>


using sneaker::json::JSON;

// -----------------------------------------------------------------------------

class utils_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(utils_unittest, TestGetVectorFromJson)
{
  std::string vector_str = "["
    "[10, 11, 12],"
    "[1,  10,  0],"
    "[22, 33,  0]"
  "]";

  const JSON vector_json = sneaker::json::parse(vector_str);

  corevm::runtime::vector vector = corevm::frontend::get_vector_from_json(vector_json);

  corevm::runtime::instr instr1 = vector[0];
  corevm::runtime::instr instr2 = vector[1];
  corevm::runtime::instr instr3 = vector[2];

  ASSERT_EQ(10, instr1.code);
  ASSERT_EQ(11, instr1.oprd1);
  ASSERT_EQ(12, instr1.oprd2);

  ASSERT_EQ(1, instr2.code);
  ASSERT_EQ(10, instr2.oprd1);
  ASSERT_EQ(0, instr2.oprd2);

  ASSERT_EQ(22, instr3.code);
  ASSERT_EQ(33, instr3.oprd1);
  ASSERT_EQ(0, instr3.oprd2);
}

// -----------------------------------------------------------------------------

TEST_F(utils_unittest, TestValidateInvalidVectorFails)
{
  // Invalid instruction code.
  const std::string vector_str = "["
    "[700, 0, 0]"
  "]";

  const JSON vector_json = sneaker::json::parse(vector_str);
  const JSON schema_json = sneaker::json::parse(
    corevm::frontend::get_v0_1_vector_schema_definition()
  );

  ASSERT_THROW(
    {
      sneaker::json::json_schema::validate(vector_json, schema_json);
    },
    sneaker::json::json_validation_error
  );
}

// -----------------------------------------------------------------------------
