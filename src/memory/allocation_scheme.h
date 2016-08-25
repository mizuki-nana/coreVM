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
#ifndef COREVM_ALLOCATION_SCHEME_H_
#define COREVM_ALLOCATION_SCHEME_H_

#include <cstdlib>
#include <sys/types.h>


namespace corevm {
namespace memory {

class AllocationScheme
{
public:
  virtual ssize_t malloc(size_t) noexcept = 0;
  virtual ssize_t calloc(size_t, size_t) noexcept = 0;
  virtual ssize_t free(size_t) noexcept = 0;

protected:
  virtual ~AllocationScheme();
};

} /* end namespace memory */
} /* end namespace corevm */


#endif /* COREVM_ALLOCATION_SCHEME_H_ */
