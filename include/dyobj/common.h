/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2014 Yanzheng Li

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
#ifndef COREVM_DYOBJ_COMMON_H_
#define COREVM_DYOBJ_COMMON_H_

#include <climits>
#include <cstdint>


// 64 MB
#define COREVM_DEFAULT_HEAP_SIZE 1024 * 1024 * 64


namespace corevm {


namespace dyobj {


typedef uint32_t attr_key;


typedef int64_t ntvhndl_key;


typedef uint32_t flag;


typedef uint32_t instr_block_key;


const ntvhndl_key NONESET_NTVHNDL_KEY = -1;


const int64_t NTVHNDL_LIMIT = LLONG_MAX;


const instr_block_key NONESET_INSTR_BLOCK_KEY = -1;


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYOBJ_COMMON_H_ */
