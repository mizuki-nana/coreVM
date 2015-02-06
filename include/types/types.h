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
#ifndef COREVM_TYPES_H_
#define COREVM_TYPES_H_

#include "native_array.h"
#include "native_map.h"
#include "native_string.h"

#include <cstdint>


namespace corevm {


namespace types {


// -----------------------------------------------------------------------------

enum native_types_enum
{
  INT8      = 0x01,
  UINT8     = 0x02,
  INT16     = 0x03,
  UINT16    = 0x04,
  INT32     = 0x05,
  UINT32    = 0x06,
  INT64     = 0x07,
  UINT64    = 0x08,
  BOOLEAN   = 0x09,
  DECIMAL   = 0x10,
  DECIMAL2  = 0x11,
  STRING    = 0x12,
  ARRAY     = 0x13,
  MAP       = 0x14
};

// -----------------------------------------------------------------------------

class native_type_wrapper {};

// -----------------------------------------------------------------------------

class int8 : public native_type_wrapper
{
public:
  typedef int8_t value_type;

  int8() : value(0) {}
  int8(value_type value) : value(value) {}

  value_type value;
};

// -----------------------------------------------------------------------------

class uint8 : public native_type_wrapper
{
public:
  typedef uint8_t value_type;

  uint8() : value(0) {}
  uint8(value_type value) : value(value) {}

  value_type value;
};

// -----------------------------------------------------------------------------

class int16 : public native_type_wrapper
{
public:
  typedef int16_t value_type;

  int16() : value(0) {}
  int16(value_type value) : value(value) {}

  value_type value;
};

// -----------------------------------------------------------------------------

class uint16 : public native_type_wrapper
{
public:
  typedef uint16_t value_type;

  uint16() : value(0) {}
  uint16(value_type value) : value(value) {}

  value_type value;
};

// -----------------------------------------------------------------------------

class int32 : public native_type_wrapper
{
public:
  typedef int32_t value_type;

  int32() : value(0) {}
  int32(value_type value) : value(value) {}

  value_type value;
};

// -----------------------------------------------------------------------------

class uint32 : public native_type_wrapper
{
public:
  typedef uint32_t value_type;

  uint32() : value(0) {}
  uint32(value_type value) : value(value) {}

  value_type value;
};

// -----------------------------------------------------------------------------

class int64 : public native_type_wrapper
{
public:
  typedef int64_t value_type;

  int64() : value(0) {}
  int64(value_type value) : value(value) {}

  value_type value;
};

// -----------------------------------------------------------------------------

class uint64 : public native_type_wrapper
{
public:
  typedef uint64_t value_type;

  uint64() : value(0) {}
  uint64(value_type value) : value(value) {}

  value_type value;
};

// -----------------------------------------------------------------------------

class boolean : public native_type_wrapper
{
public:
  typedef bool value_type;

  boolean() : value(true) {}
  boolean(value_type value) : value(value) {}

  value_type value;
};

// -----------------------------------------------------------------------------

class decimal : public native_type_wrapper
{
public:
  typedef float value_type;

  decimal() : value(static_cast<value_type>(0.0)) {}
  decimal(value_type value) : value(value) {}

  value_type value;
};

// -----------------------------------------------------------------------------

class decimal2 : public native_type_wrapper
{
public:
  typedef double value_type;

  decimal2() : value(static_cast<value_type>(0.0)) {}
  decimal2(value_type value) : value(value) {}

  value_type value;
};

// -----------------------------------------------------------------------------

class string : public native_type_wrapper
{
public:
  typedef corevm::types::native_string value_type;

  string() {}
  string(value_type value) : value(value) {}

  value_type value;
};

// -----------------------------------------------------------------------------

class array : public native_type_wrapper
{
public:
  typedef corevm::types::native_array value_type;

  array() {}
  array(value_type value) : value(value) {}

  value_type value;
};

// -----------------------------------------------------------------------------

class map : public native_type_wrapper
{
public:
  typedef corevm::types::native_map value_type;

  map() {}
  map(value_type value) : value(value) {}

  value_type value;
};

// -----------------------------------------------------------------------------


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_TYPES_H_ */
