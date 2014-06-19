#ifndef COREVM_NATIVE_STRING_H_
#define COREVM_NATIVE_STRING_H_

#include <string>
#include "errors.h"


namespace corevm {


namespace types {


using native_string_base = typename std::string;


class native_string : public native_string_base {
public:
  explicit native_string(): native_string_base() {}
  native_string(const char* s): native_string_base(s) {}
  native_string(const native_string_base& str) : native_string_base(str) {}

  native_string(native_string_base&& str) : native_string_base(str) {}

  native_string(int8_t) {
    throw corevm::types::corevm_native_type_conversion_error("int8", "string");
  }

  operator int8_t() const {
    throw corevm::types::corevm_native_type_conversion_error("string", "int8");
  }

  native_string& operator+() const {
    throw corevm::types::corevm_native_type_invalid_operator_error("+", "string");
  }

  native_string& operator-() const {
    throw corevm::types::corevm_native_type_invalid_operator_error("-", "string");
  }

  native_string& operator++() const {
    throw corevm::types::corevm_native_type_invalid_operator_error("++", "string");
  }

  native_string& operator--() const {
    throw corevm::types::corevm_native_type_invalid_operator_error("--", "string");
  }

  native_string& operator!() const {
    throw corevm::types::corevm_native_type_invalid_operator_error("!", "string");
  }

  native_string& operator~() const {
    throw corevm::types::corevm_native_type_invalid_operator_error("~", "string");
  }

  native_string& operator+(const native_string&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("+", "string");
  }

  native_string& operator-(const native_string&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("-", "string");
  }

  native_string& operator*(const native_string&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("*", "string");
  }

  native_string& operator/(const native_string&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("/", "string");
  }

  native_string& operator%(const native_string&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("%", "string");
  }

  native_string& operator&&(const native_string&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("&&", "string");
  }

  native_string& operator||(const native_string&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("||", "string");
  }

  native_string& operator&(const native_string&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("&", "string");
  }

  native_string& operator|(const native_string&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("|", "string");
  }

  native_string& operator^(const native_string&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("^", "string");
  }

  native_string& operator<<(const native_string&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("<<", "string");
  }

  native_string& operator>>(const native_string&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error(">>", "string");
  }
};


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_NATIVE_STRING_H_ */
