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
#ifndef COREVM_LLVM_SMALLVECTOR_H_
#define COREVM_LLVM_SMALLVECTOR_H_

/**
 * Magic used to include LLVM's SmallVector.h (v3.4.0) hussle-free.
 */

#ifndef __STDC_LIMIT_MACROS
  #define __STDC_LIMIT_MACROS 1
#endif
#ifndef __STDC_CONSTANT_MACROS
  #define __STDC_CONSTANT_MACROS 1
#endif

#include <llvm/ADT/SmallVector.h>

#ifdef __STDC_LIMIT_MACROS
  #undef __STDC_LIMIT_MACROS
#endif
#ifdef __STDC_CONSTANT_MACROS
  #undef __STDC_CONSTANT_MACROS
#endif


#endif /* COREVM_LLVM_SMALLVECTOR_H_ */
