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
#include "ir/ir_module_index.h"
#include "ir/format.h"
#include "ir/ir_parser_driver.h"

#include <gtest/gtest.h>

// -----------------------------------------------------------------------------

class IRModuleIndexUnitTest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(IRModuleIndexUnitTest, TestSimpleInitialization)
{
  corevm::IRModule module;
  corevm::ir::IRModuleIndex index(module);
  ASSERT_EQ(&module, index.module_indexed);
}

// -----------------------------------------------------------------------------

TEST_F(IRModuleIndexUnitTest, TestInitializationCorrectness)
{
  const char* IR_STRING = 
    "type Person {"
    "    string name;"
    "}"
    ""
    "def void createPerson() {"
    "entry:"
        "%person = alloca [ auto ] Person;"
         "ret Person %person;"
    "}";

  corevm::ir::IRParserDriver driver;
  const int res = driver.parse_from_string(IR_STRING);

  ASSERT_EQ(0, res);

  const corevm::IRModule& module = driver.module();
  corevm::ir::IRModuleIndex index(module);

  // Test logic begins here.

  ASSERT_EQ(&module, index.module_indexed);

  ASSERT_EQ(1, module.types.size());
  const corevm::IRTypeDecl& type_decl = module.types.front();
  ASSERT_EQ(1, index.type_index.size());
  ASSERT_EQ(&type_decl, index.type_index[type_decl.name]);

  ASSERT_EQ(1, module.closures.size());
  const corevm::IRClosure& closure = module.closures.front();
  ASSERT_EQ(1, index.function_index.size());
  auto& func_index = index.function_index[closure.name];
  ASSERT_EQ(&closure, func_index.closure);

  ASSERT_EQ(1, closure.blocks.size());
  ASSERT_EQ(1, func_index.bb_index.size());
  const corevm::IRBasicBlock& bb = closure.blocks.front();
  auto& instr_index = func_index.bb_index[bb.label];
  ASSERT_EQ(1, instr_index.size());
  const corevm::IRInstruction& instr = bb.body.front();
  ASSERT_EQ(&instr, instr_index[instr.target.get_string()]);
}

// -----------------------------------------------------------------------------
