#ifndef COREVM_TYPES_H_
#define COREVM_TYPES_H_


#include "native_array.h"
#include "native_string.h"


namespace corevm {


namespace types {


enum native_types_enum {
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
  ARRAY     = 0x13
};


class native_type_wrapper {
public:
  virtual size_t size() const noexcept=0;  
};


class int8 : public native_type_wrapper {
public:
  typedef int8_t value_type;

  int8() : value(0) {}
  int8(value_type value) : value(value) {}

  operator native_string() const {
    throw 1;
  }

  virtual size_t size() const noexcept { return sizeof(value_type); }

  const native_types_enum type = native_types_enum::INT8;
  value_type value;
};


class uint8 : public native_type_wrapper {
public:
  typedef uint8_t value_type;

  uint8() : value(0) {}
  uint8(value_type value) : value(value) {}

  virtual size_t size() const noexcept { return sizeof(value_type); }

  const native_types_enum type = native_types_enum::UINT8;
  value_type value;
};


class int16 : public native_type_wrapper {
public:
  typedef int16_t value_type;

  int16() : value(0) {}
  int16(value_type value) : value(value) {}

  virtual size_t size() const noexcept { return sizeof(value_type); }

  const native_types_enum type = native_types_enum::INT16;
  value_type value;
};


class uint16 : public native_type_wrapper {
public:
  typedef uint16_t value_type;

  uint16() : value(0) {}
  uint16(value_type value) : value(value) {}

  virtual size_t size() const noexcept { return sizeof(value_type); }

  const native_types_enum type = native_types_enum::UINT16;
  value_type value;
};


class int32 : public native_type_wrapper {
public:
  typedef int32_t value_type;

  int32() : value(0) {}
  int32(value_type value) : value(value) {}

  virtual size_t size() const noexcept { return sizeof(value_type); }

  const native_types_enum type = native_types_enum::INT32;
  value_type value;
};


class uint32 : public native_type_wrapper {
public:
  typedef uint32_t value_type;

  uint32() : value(0) {}
  uint32(value_type value) : value(value) {}

  virtual size_t size() const noexcept { return sizeof(value_type); }

  const native_types_enum type = native_types_enum::UINT32;
  value_type value;
};


class int64 : public native_type_wrapper {
public:
  typedef int64_t value_type;

  int64() : value(0) {}
  int64(value_type value) : value(value) {}

  virtual size_t size() const noexcept { return sizeof(value_type); }

  const native_types_enum type = native_types_enum::INT64;
  value_type value;
};


class uint64 : public native_type_wrapper {
public:
  typedef uint64_t value_type;

  uint64() : value(0) {}
  uint64(value_type value) : value(value) {}

  virtual size_t size() const noexcept { return sizeof(value_type); }

  const native_types_enum type = native_types_enum::UINT64;
  value_type value;
};


class boolean : public native_type_wrapper {
public:
  typedef bool value_type;

  boolean() : value(true) {}
  boolean(value_type value) : value(value) {}

  virtual size_t size() const noexcept { return sizeof(value_type); }

  const native_types_enum type = native_types_enum::BOOLEAN;
  value_type value;
};


class decimal : public native_type_wrapper {
public:
  typedef float value_type;

  decimal() : value(static_cast<value_type>(0.0)) {}
  decimal(value_type value) : value(value) {}

  virtual size_t size() const noexcept { return sizeof(value_type); }

  const native_types_enum type = native_types_enum::DECIMAL;
  value_type value;
};


class decimal2 : public native_type_wrapper {
public:
  typedef double value_type;

  decimal2() : value(static_cast<value_type>(0.0)) {}
  decimal2(value_type value) : value(value) {}

  virtual size_t size() const noexcept { return sizeof(value_type); }

  const native_types_enum type = native_types_enum::DECIMAL2;
  value_type value;
};


class string : public native_type_wrapper {
public:
  typedef native_string value_type;

  string() {}
  string(value_type value) : value(value) {}

  virtual size_t size() const noexcept { return sizeof(value_type); }

  const native_types_enum type = native_types_enum::STRING;
  value_type value;
};


class array : public native_type_wrapper {
public:
  typedef native_array value_type;

  array() {}
  array(value_type value) : value(value) {}

  virtual size_t size() const noexcept { return sizeof(value_type); }

  const native_types_enum type = native_types_enum::ARRAY;
  value_type value;
};


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_TYPES_H_ */