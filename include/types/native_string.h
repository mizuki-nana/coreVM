#ifndef COREVM_NATIVE_STRING_H_
#define COREVM_NATIVE_STRING_H_

#include "errors.h"


namespace corevm {


namespace types {


class native_string : public std::string {
public:
  native_string(): std::string() {}
  native_string(const char* s): std::string(s) {}
  native_string(const char* s, size_t n) : std::string(s, n) {}
  native_string(size_t n, char c) : std::string(n, c) {}
  native_string(const std::string& str, size_t pos, size_t len=std::string::npos) : std::string(str, pos, len) {}
  native_string(const std::string& str) : std::string(str) {}

  template<class InputIterator>
  native_string(InputIterator first, InputIterator last) : std::string(first, last) {}
  native_string(std::string&& str) : std::string(str) {}

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