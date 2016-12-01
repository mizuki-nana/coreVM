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
#include "corevm/program.h"

#include <gtest/gtest.h>

#include <array>
#include <iostream>


class ProgramUnitTest : public ::testing::Test {};

// NOTE: This test makes Xcode emit error even on success.
#ifndef __APPLE__
TEST_F(ProgramUnitTest, TestRun)
{
  corevm::Program program;

  int argc = 2;
  std::array<char*, 4> argv {{
     (char*)"coreVM",
     (char*)"./sample.core",
     (char*)"--config",
     (char*)"./sample.config"
  }};

  int res = program.run(argc, argv.data());

  ASSERT_EQ(-1, res);

  std::cout << "(error here expected)" << std::endl;
}
#endif
