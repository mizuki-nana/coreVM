#ifndef COREVM_NATIVE_MAP_H_
#define COREVM_NATIVE_MAP_H_

#include <unordered_map>
#include "errors.h"


namespace corevm {


namespace types {


typedef uint64_t native_map_key_type;
typedef uint64_t native_map_mapped_type;


using native_map_base = typename std::unordered_map<native_map_key_type, native_map_mapped_type>;

#ifndef DEFAULT_NATIVE_MAP_INITIAL_CAPACITY
  #define DEFAULT_NATIVE_MAP_INITIAL_CAPACITY 10
#endif

class native_map : public native_map_base {
public:
  explicit native_map() : native_map_base(DEFAULT_NATIVE_MAP_INITIAL_CAPACITY) {}

  native_map(const native_map_base& x) : native_map_base(x) {}
  native_map(native_map_base&& x) : native_map_base(x) {}

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
};


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_NATIVE_MAP_H_ */
