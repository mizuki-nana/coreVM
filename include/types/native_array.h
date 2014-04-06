#ifndef COREVM_NATIVE_ARRAY_H_
#define COREVM_NATIVE_ARRAY_H_

#include <vector>
#include "errors.h"


namespace corevm {


namespace types {


typedef __int128_t element_type;


using std_vector_type = typename std::vector<element_type>;


// #define DEFAULT_INITIAL_ARRAY_SIZE 8


class native_array : public std_vector_type {
public:
  explicit native_array(const allocator_type& alloc = allocator_type()) : std_vector_type(alloc) {}

  /*
   * Cannot have this constructor because we need the implicit conversion to integer
   * types to make native_type_handle work:
   * explicit native_array(size_type n) : std_vector_type(n) {}
   */

  explicit native_array(
    size_type n, const value_type& val, const allocator_type& alloc = allocator_type()):
    std_vector_type(n, val, alloc) {}

  template <class InputIterator>
  native_array(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type()):
    std_vector_type(first, last, alloc) {}

  native_array(const std_vector_type& x) : std_vector_type(x) {}
  native_array(const std_vector_type& x, const allocator_type& alloc) : std_vector_type(x, alloc) {}
  native_array(std_vector_type&& x) : std_vector_type(x) {}
  native_array(std_vector_type&& x, const allocator_type& alloc) : std_vector_type(x, alloc) {}
  native_array(std::initializer_list<value_type> il,
    const allocator_type& alloc = allocator_type()) : std_vector_type(il, alloc) {}

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