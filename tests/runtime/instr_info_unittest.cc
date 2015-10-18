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
#include "runtime/common.h"
#include "runtime/instr_info.h"

#include <sneaker/testing/_unittest.h>


// -----------------------------------------------------------------------------

class instr_info_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(instr_info_unittest, TestInstrInfo)
{
  corevm::runtime::instr_code code = corevm::runtime::instr_enum::LDOBJ;
  const corevm::runtime::instr_info& info = corevm::runtime::instr_set_info::instr_infos[code];

  ASSERT_STREQ("ldobj", info.name);
}

// -----------------------------------------------------------------------------
