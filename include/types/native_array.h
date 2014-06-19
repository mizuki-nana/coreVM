#ifndef COREVM_NATIVE_ARRAY_H_
#define COREVM_NATIVE_ARRAY_H_

#include <vector>
#include "errors.h"


namespace corevm {


namespace types {


typedef __uint128_t element_type;


using native_array_base = typename std::vector<element_type>;


class native_array : public native_array_base {
public:
  explicit native_array() : native_array_base() {}

  native_array(const native_array_base& x) : native_array_base(x) {}
  native_array(native_array_base&& x) : native_array_base(x) {}

  native_array(std::initializer_list<value_type> il) : native_array_base(il) {}

  native_array(int8_t) {
    throw corevm::types::corevm_native_type_conversion_error("int8", "array");
  }

  operator int8_t() const {
    throw corevm::types::corevm_native_type_conversion_error("array", "int8");
  }

  native_array& operator+() const {
    throw corevm::types::corevm_native_type_invalid_operator_error("+", "array");
  }

  native_array& operator-() const {
    throw corevm::types::corevm_native_type_invalid_operator_error("-", "array");
  }

  native_array& operator++() const {
    throw corevm::types::corevm_native_type_invalid_operator_error("++", "array");
  }

  native_array& operator--() const {
    throw corevm::types::corevm_native_type_invalid_operator_error("--", "array");
  }

  native_array& operator!() const {
    throw corevm::types::corevm_native_type_invalid_operator_error("!", "array");
  }

  native_array& operator~() const {
    throw corevm::types::corevm_native_type_invalid_operator_error("~", "array");
  }

  native_array& operator+(const native_array&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("+", "array");
  }

  native_array& operator-(const native_array&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("-", "array");
  }

  native_array& operator*(const native_array&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("*", "array");
  }

  native_array& operator/(const native_array&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("/", "array");
  }

  native_array& operator%(const native_array&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("%", "array");
  }

  native_array& operator&&(const native_array&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("&&", "array");
  }

  native_array& operator||(const native_array&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("||", "array");
  }

  native_array& operator&(const native_array&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("&", "array");
  }

  native_array& operator|(const native_array&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("|", "array");
  }

  native_array& operator^(const native_array&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("^", "array");
  }

  native_array& operator<<(const native_array&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("<<", "array");
  }

  native_array& operator>>(const native_array&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error(">>", "array");
  }
};


}; /* end namespace types */


}; /* end namespace corevm */


#endif /* COREVM_NATIVE_ARRAY_H_ */
