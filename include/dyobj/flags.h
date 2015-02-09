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


namespace corevm {


namespace dyobj {


/**
 * Dynamic object flags are represented by 8 bits unsigned integers.
 * These flag bits are defined so that the default value of 0 would be the
 * appropriate value for most objects.
 */
enum flags : uint8_t
{

  /* ------------ Bits that pertain to the lifespan of objects -------------- */

  IS_NOT_GARBAGE_COLLECTIBLE = 1,

  IS_WEAK_REF = 2,

  IS_INDELIBLE = 3,

  /* ------------- Bits that pertain to the scope of objects ---------------- */

  IS_GLOBAL_OBJ = 4,

  IS_SUPPLIED_OBJ = 5,

  IS_INVISIBLE_TO_USER = 6,

  /* ------- Bits that pertain to the various attributes of objects --------- */

  IS_CALLABLE = 7,

  IS_IMMUTABLE = 8,

  /* ------------------------ Max value allowed ----------------------------- */

  MAX_VALUE = 9,

  /* ---------- THE REMAINING BITS ARE RESERVED FOR FURTHER USE ------------- */

  LAST_PLACEHOLDER = 32,

};


bool is_valid_flag_bit(char);


}; /* end namespace dyobj */


}; /* end namespace corevm */


#endif /* COREVM_DYOBJ_FLAGS_H_ */
