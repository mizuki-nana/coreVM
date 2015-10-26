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
#ifndef COREVM_RUNTIME_UTILS_H_
#define COREVM_RUNTIME_UTILS_H_

#include "dyobj/common.h"
#include "common.h"

#include <cstdint>


namespace corevm {


namespace runtime {


/** Forward declaration of `corevm::runtime::compartment`. */
class compartment;


using corevm::dyobj::ntvhndl_key;

// -----------------------------------------------------------------------------

inline ntvhndl_key ptr_to_ntvhndl_key(void* ptr)
{
  return static_cast<ntvhndl_key>( (uint8_t*)(ptr) - (uint8_t*)(NULL) );
}

// -----------------------------------------------------------------------------

inline void* ntvhndl_key_to_ptr(const ntvhndl_key& key)
{
  return reinterpret_cast<void*>(key);
}

// -----------------------------------------------------------------------------

corevm::dyobj::attr_key
get_attr_key(
  corevm::runtime::compartment* compartment,
  corevm::runtime::encoding_key str_key);

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_RUNTIME_UTILS_H_ */
