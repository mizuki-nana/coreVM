#ifndef COREVM_NATIVE_STRING_H_
#define COREVM_NATIVE_STRING_H_

#include <stdexcept>
#include <string>
#include "errors.h"


namespace corevm {


namespace types {


typedef std::string native_string_base;


class native_string : public native_string_base {
public:
  explicit native_string(): native_string_base() {}
  native_string(const char* s): native_string_base(s) {}
  native_string(const native_string_base& str) : native_string_base(str) {}

  native_string(native_string_base&& str) : native_string_base(str) {}

  native_string(int8_t) {
    throw corevm::types::conversion_error("int8", "string");
  }

  operator int8_t() const {
    throw corevm::types::conversion_error("string", "int8");
  }

  native_string& operator+() const {
    throw corevm::types::invalid_operator_error("+", "string");
  }

  native_string& operator-() const {
    throw corevm::types::invalid_operator_error("-", "string");
  }

  native_string& operator++() const {
    throw corevm::types::invalid_operator_error("++", "string");
  }

  native_string& operator--() const {
    throw corevm::types::invalid_operator_error("--", "string");
  }

  native_string& operator!() const {
    throw corevm::types::invalid_operator_error("!", "string");
  }

  native_string& operator~() const {
    throw corevm::types::invalid_operator_error("~", "string");
  }

  native_string& operator+(const native_string&) const {
    throw corevm::types::invalid_operator_error("+", "string");
  }

  native_string& operator-(const native_string&) const {
    throw corevm::types::invalid_operator_error("-", "string");
  }

  native_string& operator*(const native_string&) const {
    throw corevm::types::invalid_operator_error("*", "string");
  }

  native_string& operator/(const native_string&) const {
    throw corevm::types::invalid_operator_error("/", "string");
  }

  native_string& operator%(const native_string&) const {
    throw corevm::types::invalid_operator_error("%", "string");
  }

  native_string& operator&&(const native_string&) const {
    throw corevm::types::invalid_operator_error("&&", "string");
  }

  native_string& operator||(const native_string&) const {
    throw corevm::types::invalid_operator_error("||", "string");
  }

  native_string& operator&(const native_string&) const {
    throw corevm::types::invalid_operator_error("&", "string");
  }

  native_string& operator|(const native_string&) const {
    throw corevm::types::invalid_operator_error("|", "string");
  }

  native_string& operator^(const native_string&) const {
    throw corevm::types::invalid_operator_error("^", "string");
  }

  native_string& operator<<(const native_string&) const {
    throw corevm::types::invalid_operator_error("<<", "string");
  }

  native_string& operator>>(const native_string&) const {
    throw corevm::types::invalid_operator_error(">>", "string");
  }

  reference at(size_type n) throw(corevm::types::out_of_range_error) {
    try {
      return native_string_base::at(n);
    } catch (const std::out_of_range&) {
      throw corevm::types::out_of_range_error("String index out of range");
    }
  }

  const_reference at(size_type n) const throw(corevm::types::out_of_range_error) {
    try {
      return native_string_base::at(n);
    } catch (const std::out_of_range&) {
      throw corevm::types::out_of_range_error("String index out of range");
    }
  }

  corevm::types::native_string& insert(size_type pos, const corevm::types::native_string& str) throw(corevm::types::out_of_range_error) {
    try {
      return static_cast<corevm::types::native_string&>(native_string_base::insert(pos, str));
    } catch (const std::out_of_range&) {
      throw corevm::types::out_of_range_error("String index out of range");
    }
  }

  corevm::types::native_string& insert(size_type pos, size_type n, value_type c) throw(corevm::types::out_of_range_error) {
    try {
      return static_cast<corevm::types::native_string&>(native_string_base::insert(pos, n, c));
    } catch (const std::out_of_range&) {
      throw corevm::types::out_of_range_error("String index out of range");
    }
  }

  corevm::types::native_string& erase(size_type pos) throw(corevm::types::out_of_range_error) {
    try {
      return static_cast<corevm::types::native_string&>(native_string_base::erase(pos));
    } catch (const std::out_of_range&) {
      throw corevm::types::out_of_range_error("String index out of range");
    }
  }

  corevm::types::native_string& erase(size_type pos, size_type len) throw(corevm::types::out_of_range_error) {
    try {
      return static_cast<corevm::types::native_string&>(native_string_base::erase(pos, len));
    } catch (const std::out_of_range&) {
      throw corevm::types::out_of_range_error("String index out of range");
    }
  }

  corevm::types::native_string& replace(
    size_type pos, size_type len, const corevm::types::native_string& str) throw(corevm::types::out_of_range_error)
  {
    try {
      return static_cast<corevm::types::native_string&>(native_string_base::replace(pos, len, str));
    } catch (const std::out_of_range&) {
      throw corevm::types::out_of_range_error("String index out of range");
    }
  }
};


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_NATIVE_STRING_H_ */
