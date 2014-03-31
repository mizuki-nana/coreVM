#ifndef COREVM_OPERATORS_H_
#define COREVM_OPERATORS_H_

#include "types.h"


namespace corevm {


namespace types {


class op {};

class unary_op : public op {};
class binary_op : public op {};


class positive : public unary_op {
public:
  template<typename R, typename T>
  typename R::value_type operator()(const T& handle) {
    return static_cast<typename R::value_type>(+handle.value);
  }
};


class negation : public unary_op {
public:
  template<typename R, typename T>
  typename R::value_type operator()(const T& handle) {
    return static_cast<typename R::value_type>(-handle.value);
  }
};


class increment : public unary_op {
public:
  template<typename R, typename T>
  typename R::value_type operator()(const T& handle) {
    T& handle_ = const_cast<T&>(handle);
    return static_cast<typename R::value_type>(++handle_.value);
  }
};


// TODO: not sure if it's right to convert operand types to int64_t here...
class decrement : public unary_op {
public:
  template<typename R, typename T>
  typename R::value_type operator()(const T& handle) {
    T& handle_ = const_cast<T&>(handle);
    typename T::value_type value = handle.value;
    typename corevm::types::int64::value_type value_ =
      static_cast<typename corevm::types::int64::value_type>(value);

    --value_;

    handle_.value = value_;

    return handle.value;
  }
};
template<>
typename corevm::types::string::value_type
decrement::operator()<corevm::types::string>(const corevm::types::string& handle) {
  return static_cast<typename corevm::types::string::value_type>(--handle.value);
}


class logical_not : public unary_op {
public:
  template<typename R, typename T>
  typename R::value_type operator()(const T& handle) {
    return static_cast<typename R::value_type>(!handle.value);
  }
};


class bitwise_not : public unary_op {
public:
  template<typename R, typename T>
  typename R::value_type operator()(const T& handle) {
    typename corevm::types::int64::value_type value =
      static_cast<typename corevm::types::int64::value_type>(handle.value);

    return ~value;
  }
};
template<>
typename corevm::types::string::value_type
bitwise_not::operator()<corevm::types::string>(const corevm::types::string& handle) {
  return static_cast<typename corevm::types::string::value_type>(~handle.value);
}


class addition : public binary_op {
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs) {
    return (
      static_cast<typename R::value_type>(lhs.value) +
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};
template<>
typename corevm::types::string::value_type
addition::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value + rhs.value);
}


class subtraction : public binary_op {
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs) {
    return (
      static_cast<typename R::value_type>(lhs.value) -
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};
template<>
typename corevm::types::string::value_type
subtraction::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value - rhs.value);
}


class multiplication : public binary_op {
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs) {
    return (
      static_cast<typename R::value_type>(lhs.value) *
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};
template<>
typename corevm::types::string::value_type
multiplication::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value * rhs.value);
}


class division : public binary_op {
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs) {
    return (
      static_cast<typename R::value_type>(lhs.value) /
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};
template<>
typename corevm::types::string::value_type
division::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value / rhs.value);
}


// TODO: not sure if it's right to convert operand types to int64_t here...
class modulus : public binary_op {
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs) {
    return (
      static_cast<typename corevm::types::int64::value_type>(lhs.value) %
      static_cast<typename corevm::types::int64::value_type>(rhs.value)
    );
  }
};
template<>
typename corevm::types::string::value_type
modulus::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value % rhs.value);
}


class logical_and : public binary_op {
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs) {
    return (
      static_cast<typename R::value_type>(lhs.value) &&
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};
template<>
typename corevm::types::string::value_type
logical_and::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value && rhs.value);
}


class logical_or : public binary_op {
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs) {
    return (
      static_cast<typename R::value_type>(lhs.value) ||
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};
template<>
typename corevm::types::string::value_type
logical_or::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value || rhs.value);
}


class bitwise_and : public binary_op {
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs) {
    return (
      static_cast<typename corevm::types::int64::value_type>(lhs.value) &
      static_cast<typename corevm::types::int64::value_type>(rhs.value)
    );
  }
};
template<>
typename corevm::types::string::value_type
bitwise_and::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value & rhs.value);
}



class bitwise_or : public binary_op {
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs) {
    return (
      static_cast<typename corevm::types::int64::value_type>(lhs.value) |
      static_cast<typename corevm::types::int64::value_type>(rhs.value)
    );
  }
};
template<>
typename corevm::types::string::value_type
bitwise_or::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value | rhs.value);
}


class bitwise_xor : public binary_op {
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs) {
    return (
      static_cast<typename corevm::types::int64::value_type>(lhs.value) ^
      static_cast<typename corevm::types::int64::value_type>(rhs.value)
    );
  }
};
template<>
typename corevm::types::string::value_type
bitwise_xor::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value ^ rhs.value);
}


class bitwise_left_shift : public binary_op {
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs) {
    return (
      static_cast<typename corevm::types::int64::value_type>(lhs.value) <<
      static_cast<typename corevm::types::int64::value_type>(rhs.value)
    );
  }
};
template<>
typename corevm::types::string::value_type
bitwise_left_shift::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value << rhs.value);
}


class bitwise_right_shift : public binary_op {
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs) {
    return (
      static_cast<typename corevm::types::int64::value_type>(lhs.value) >>
      static_cast<typename corevm::types::int64::value_type>(rhs.value)
    );
  }
};
template<>
typename corevm::types::string::value_type
bitwise_right_shift::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(lhs.value >> rhs.value);
}


class eq : public binary_op {
public:
  template<typename R, typename T, typename U>
  bool operator()(const T& lhs, const U& rhs) {
    return (
      static_cast<typename R::value_type>(lhs.value) ==
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};


class neq : public binary_op {
public:
  template<typename R, typename T, typename U>
  bool operator()(const T& lhs, const U& rhs) {
    return (
      static_cast<typename R::value_type>(lhs.value) !=
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};


class gt : public binary_op {
public:
  template<typename R, typename T, typename U>
  bool operator()(const T& lhs, const U& rhs) {
    return (
      static_cast<typename R::value_type>(lhs.value) >
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};


class lt : public binary_op {
public:
  template<typename R, typename T, typename U>
  bool operator()(const T& lhs, const U& rhs) {
    return (
      static_cast<typename R::value_type>(lhs.value) <
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};


class gte : public binary_op {
public:
  template<typename R, typename T, typename U>
  bool operator()(const T& lhs, const U& rhs) {
    return (
      static_cast<typename R::value_type>(lhs.value) >=
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};


class lte : public binary_op {
public:
  template<typename R, typename T, typename U>
  bool operator()(const T& lhs, const U& rhs) {
    return (
      static_cast<typename R::value_type>(lhs.value) <=
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_OPERATORS_H_ */