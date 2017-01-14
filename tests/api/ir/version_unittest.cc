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
#include "api/ir/version.h"

#include <gtest/gtest.h>


// -----------------------------------------------------------------------------

class IRVersionUnitTest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(IRVersionUnitTest, TestVersion)
{
  ASSERT_GT(corevm::api::ir::IR_VERSION, 0ULL);
}

// -----------------------------------------------------------------------------

TEST_F(IRVersionUnitTest, TestVersionStringRepresentation)
{
  const auto major_version = corevm::api::ir::IR_VERSION / 100000;
  const auto minor_version = corevm::api::ir::IR_VERSION / 100 % 1000;
  const auto patch_version = corevm::api::ir::IR_VERSION % 100;

  char buf[32] = {0};
  snprintf(buf, sizeof(buf), "%u.%u.%u",
    major_version, minor_version, patch_version);

  ASSERT_STREQ("0.1.0", buf);
}

// -----------------------------------------------------------------------------
