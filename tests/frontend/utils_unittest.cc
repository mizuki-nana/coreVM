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
#include "../../include/frontend/utils.h"
#include "../../include/runtime/instr_block.h"
#include "../../include/runtime/instr.h"

#include <sneaker/testing/_unittest.h>
#include <sneaker/json/json_parser.h>
#include <sneaker/json/json.h>

#include <string>


using sneaker::json::JSON;


class utils_unittest : public ::testing::Test {};


TEST_F(utils_unittest, TestGetVectorFromJson)
{
  std::string vector_str = "["
    "[10, 11, 12],"
    "[1],"
    "[22, 33]"
  "]";

  const JSON vector_json = sneaker::json::parse(vector_str);

  corevm::runtime::instr_block vector = corevm::frontend::get_vector_from_json(vector_json);

  corevm::runtime::instr_block::block_type block = vector.block();

  corevm::runtime::instr instr1 = block[0];
  corevm::runtime::instr instr2 = block[1];
  corevm::runtime::instr instr3 = block[2];

  ASSERT_EQ(10, instr1.code);
  ASSERT_EQ(11, instr1.oprd1);
  ASSERT_EQ(12, instr1.oprd2);

  ASSERT_EQ(1, instr2.code);
  ASSERT_EQ(0, instr2.oprd1);
  ASSERT_EQ(0, instr2.oprd2);

  ASSERT_EQ(22, instr3.code);
  ASSERT_EQ(33, instr3.oprd1);
  ASSERT_EQ(0, instr3.oprd2);
}
