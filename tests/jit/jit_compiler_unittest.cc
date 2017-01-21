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
#include "jit/jit_compiler.h"
#include "jit/jit_scheme_llvmmcjit.h"

#include <gtest/gtest.h>


// -----------------------------------------------------------------------------

class JITCompilerUnitTest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(JITCompilerUnitTest, TestCompilation)
{
  corevm::jit::JITCompiler<corevm::jit::JITSchemeLLVMMCJIT> compiler;
  corevm::IRModule input_module;
  std::string func_name("hello_world");
  std::string msg;
  const bool res = compiler.run(input_module, func_name, msg);
  ASSERT_EQ(false, res);
  ASSERT_EQ("Target module lowering failed.", msg);
  ASSERT_EQ(false, compiler.compiled());

  std::vector<corevm::jit::RuntimeValue> args;
  corevm::jit::RuntimeValue return_val;
  ASSERT_EQ(false, compiler.eval_func(args, return_val));
}

// -----------------------------------------------------------------------------
