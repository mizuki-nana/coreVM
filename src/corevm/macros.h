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
#ifndef COREVM_MACROS_H_
#define COREVM_MACROS_H_

#include <cstdlib>
#include <iostream>


// -----------------------------------------------------------------------------

// Debug mode is off by default.
// Use compiler option to turn on debug mode.
#ifndef __DEBUG__
  #define __DEBUG__ 0
#endif

// -----------------------------------------------------------------------------

#define ASSERT(expr)                                    \
  if (! (expr) )                                        \
  {                                                     \
    std::cerr << "Assertion failed: " << #expr <<       \
      " (" << __FILE__ << ")" << " [" << __FUNCTION__   \
      << "() line " << __LINE__ << "]" << std::endl;    \
    abort();                                            \
  }

// -----------------------------------------------------------------------------

// Special exception throw mode. Off by default.
// To be turned on by compiler option when needed.
#ifndef __THROW__
  #define __THROW__ 0
#endif

// -----------------------------------------------------------------------------

#if __DEBUG__ && __THROW__
  #define THROW(expr)                                   \
    std::cerr << #expr << " (" << __FILE__ << ")" <<    \
      "[" << __FUNCTION__ << "() line " << __LINE__ <<  \
      "]" << std::endl;                                 \
    throw (expr)
#else
  #define THROW(expr) throw (expr)
#endif

// -----------------------------------------------------------------------------

/**
 * Measure cumulative and average runtime of each instruction handler.
 */
#ifndef INSTRUMENTAL_MEASUREMENT
  #define INSTRUMENTAL_MEASUREMENT 0
#endif

#if __DEBUG__ && INSTRUMENTAL_MEASUREMENT
  #define __MEASURE_INSTRS__ 1
#else
  #define __MEASURE_INSTRS__ 0
#endif

// -----------------------------------------------------------------------------

/**
 * TODO: [COREVM-463] Design and implement forward iterator for sequential allocator
 */
#define COREVM_463 0

// -----------------------------------------------------------------------------

#define COREVM_USE_SMALL_ATTRIBUTE_TABLE 1

// -----------------------------------------------------------------------------

#define COREVM_USE_LINEAR_VARIABLE_TABLE 1

// -----------------------------------------------------------------------------

#endif /* COREVM_MACROS_H_ */
