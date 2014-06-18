#ifndef COREVM_NATIVE_MAP_H_
#define COREVM_NATIVE_MAP_H_

#include <functional>
#include <unordered_map>
#include <sneaker/functional/function.h>
#include "errors.h"


namespace corevm {


namespace types {


typedef __uint128_t key_type;
typedef __uint128_t mapped_type;


using std_map_type = typename std::unordered_map<
  key_type,
  mapped_type,
  key_type(*)(key_type)
>;


class native_map : public std_map_type {
public:
  //explicit native_map(const allocator_type& alloc=allocator_type()) : std_map_type(alloc) {}

  explicit native_map() : std_map_type(
    10,
    [](key_type key) -> key_type {
      return key;
    }
  ) {}

  /*
  explicit native_map(
    size_type n,
    const hasher& hf=hasher(),
    const key_equal& eql=key_equal(),
    const allocator_type& alloc=allocator_type()
  ) : std_map_type(n, hf, eql, alloc) {};
  */

  native_map(const std_map_type& x) : std_map_type(x) {}
  native_map(std_map_type&& x) : std_map_type(x) {}

  // TODO: resolve OS X macro
#ifdef __APPLE__
  // native_map(const std_map_type& x, const allocator_type& alloc) : std_map_type(x, alloc) {}
  // native_map(std_map_type&& x, const allocator_type& alloc) : std_map_type(x, alloc) {}
#endif

  native_map(int8_t) {
    throw corevm::types::corevm_native_type_conversion_error("int8", "map");
  }

  operator int8_t() const {
    throw corevm::types::corevm_native_type_conversion_error("map", "int8");
  }

  native_map& operator+() const {
    throw corevm::types::corevm_native_type_invalid_operator_error("+", "map");
  }

  native_map& operator-() const {
    throw corevm::types::corevm_native_type_invalid_operator_error("-", "map");
  }

  native_map& operator++() const {
    throw corevm::types::corevm_native_type_invalid_operator_error("++", "map");
  }

  native_map& operator--() const {
    throw corevm::types::corevm_native_type_invalid_operator_error("--", "map");
  }

  native_map& operator!() const {
    throw corevm::types::corevm_native_type_invalid_operator_error("!", "map");
  }

  native_map& operator~() const {
    throw corevm::types::corevm_native_type_invalid_operator_error("~", "map");
  }

  native_map& operator+(const native_map&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("+", "map");
  }

  native_map& operator-(const native_map&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("-", "map");
  }

  native_map& operator*(const native_map&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("*", "map");
  }

  native_map& operator/(const native_map&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("/", "map");
  }

  native_map& operator%(const native_map&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("%", "map");
  }

  native_map& operator&&(const native_map&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("&&", "map");
  }

  native_map& operator||(const native_map&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("||", "map");
  }

  native_map& operator&(const native_map&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("&", "map");
  }

  native_map& operator|(const native_map&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("|", "map");
  }

  native_map& operator^(const native_map&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("^", "map");
  }

  native_map& operator<<(const native_map&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("<<", "map");
  }

  native_map& operator>>(const native_map&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error(">>", "map");
  }

  /*
  native_map& operator<(const native_map&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error("<", "map");
  }

  native_map& operator>(const native_map&) const {
    throw corevm::types::corevm_native_type_invalid_operator_error(">", "map");
  } */
};


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_NATIVE_MAP_H_ */
