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
#ifndef COREVM_DYOBJ_ID_H_
#define COREVM_DYOBJ_ID_H_

#if defined(__clang__) and __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif

#include <climits>
#include <cstdint>


namespace corevm {


namespace dyobj {


typedef uint64_t dyobj_id;


const uint64_t DYOBJ_LIMIT = ULLONG_MAX;


// -----------------------------------------------------------------------------

inline dyobj_id obj_ptr_to_id(void* ptr)
{
  return static_cast<dyobj_id>( (uint8_t*)(ptr) - (uint8_t*)(0) );
}

// -----------------------------------------------------------------------------

inline void* obj_id_to_ptr(dyobj_id id)
{
  return reinterpret_cast<void*>(id);
}

// -----------------------------------------------------------------------------


} /* end namespace dyobj */


} /* end namespace corevm */


#if defined(__clang__) and __clang__
  #pragma clang diagnostic pop
#endif


#endif /* COREVM_DYOBJ_ID_H_ */
