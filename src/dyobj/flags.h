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
#ifndef COREVM_DYOBJ_FLAGS_H_
#define COREVM_DYOBJ_FLAGS_H_

#include <cstdint>
#include <vector>


namespace corevm {


namespace dyobj {


/**
 * Dynamic object flags are represented by 32-bit unsigned integers.
 * These flag bits are defined so that the default value of 0 would be the
 * appropriate value for most objects.
 */
enum DynamicObjectFlagBits : uint32_t
{

  /* ------------ Bits that pertain to the lifespan of objects -------------- */

  DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE = 1,

  DYOBJ_IS_INDELIBLE,

  /* ------- Bits that pertain to the various attributes of objects --------- */

  DYOBJ_IS_NON_CALLABLE,

  DYOBJ_IS_IMMUTABLE,

  /* ------------------------ Max value allowed ----------------------------- */

  DYOBJ_MAX_VALUE = 32

};

// -----------------------------------------------------------------------------

bool is_valid_flag_bit(char);

// -----------------------------------------------------------------------------

const std::vector<const char*>
DYOBJ_FLAG_VALUES_ARRAY {
  "DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE",
  "DYOBJ_IS_INDELIBLE",
  "DYOBJ_IS_NON_CALLABLE",
  "DYOBJ_IS_IMMUTABLE",
};

// -----------------------------------------------------------------------------


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYOBJ_FLAGS_H_ */
