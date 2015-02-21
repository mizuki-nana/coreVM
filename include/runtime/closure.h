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
#ifndef COREVM_CLOSURE_H_
#define COREVM_CLOSURE_H_

#include "common.h"
#include "vector.h"

#include <limits>
#include <ostream>
#include <type_traits>
#include <vector>


namespace corevm {


namespace runtime {


typedef struct closure
{
  corevm::runtime::closure_id id;
  corevm::runtime::closure_id parent_id;
  corevm::runtime::vector vector;
} closure;

// -----------------------------------------------------------------------------

typedef std::vector<corevm::runtime::closure> closure_table;

// -----------------------------------------------------------------------------

static_assert(
  std::numeric_limits<closure_table::size_type>::max() >=
  std::numeric_limits<corevm::runtime::closure_id>::max(),
  "Closure ID incompatibility");

// -----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream&, const corevm::runtime::closure&);

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_CLOSURE_H_ */
