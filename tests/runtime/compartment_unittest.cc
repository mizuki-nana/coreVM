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
#include "../../include/runtime/compartment.h"
#include "../../include/runtime/closure.h"
#include "../../include/runtime/vector.h"

#include <sneaker/testing/_unittest.h>

#include <sstream>


class compartment_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(compartment_unittest, TestOutputStream)
{
  corevm::runtime::compartment compartment("./example.core");

  corevm::runtime::vector vector {
    { .code=6, .oprd1=421, .oprd2=523 },
    { .code=5, .oprd1=532, .oprd2=0   },
    { .code=2, .oprd1=72,  .oprd2=0   },
  };

  corevm::runtime::closure closure {
    .id=2,
    .parent_id=1,
    .vector=vector
  };

  corevm::runtime::closure_table closure_table {
    closure
  };

  compartment.set_closure_table(closure_table);

  std::stringstream ss;
  ss << compartment;

  ASSERT_NE(0, ss.str().size());
}

// -----------------------------------------------------------------------------
