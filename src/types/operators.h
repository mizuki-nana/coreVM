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
#ifndef COREVM_OPERATORS_H_
#define COREVM_OPERATORS_H_

#include "errors.h"
#include "types.h"
#include "corevm/macros.h"

#include <cmath>
#include <functional>
#include <ios>
#include <sstream>


namespace corevm {


namespace types {


class op {};

// -----------------------------------------------------------------------------

class unary_op : public op {};

// -----------------------------------------------------------------------------

class binary_op : public op {};

// -----------------------------------------------------------------------------

class positive : public unary_op
{
public:
  template<typename R, typename T>
  typename R::value_type operator()(const T& handle)
  {
    return static_cast<typename R::value_type>(+handle.value);
  }
};

// -----------------------------------------------------------------------------

class negation : public unary_op
{
public:
  template<typename R, typename T>
  typename R::value_type operator()(const T& handle)
  {
    return static_cast<typename R::value_type>(-handle.value);
  }
};

// -----------------------------------------------------------------------------

class increment : public unary_op
{
public:
  template<typename R, typename T>
  typename R::value_type operator()(const T& handle)
  {
    T& handle_ = const_cast<T&>(handle);
    return static_cast<typename R::value_type>(++handle_.value);
  }
};

// -----------------------------------------------------------------------------

class decrement : public unary_op
{
public:
  template<typename R, typename T>
  typename R::value_type operator()(const T& handle)
  {
    T& handle_ = const_cast<T&>(handle);
    --handle_.value;
    return handle.value;
  }
};

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::boolean::value_type
corevm::types::decrement::operator()<corevm::types::boolean>(
  const corevm::types::boolean& handle)
{
  THROW(corevm::types::invalid_operator_error("--", "boolean"));
}

// -----------------------------------------------------------------------------

class logical_not : public unary_op
{
public:
  template<typename R, typename T>
  typename R::value_type operator()(const T& handle)
  {
    return static_cast<typename R::value_type>(!handle.value);
  }
};

// -----------------------------------------------------------------------------

class bitwise_not : public unary_op
{
public:
  template<typename R, typename T>
  typename R::value_type operator()(const T& handle)
  {
    typename corevm::types::int64::value_type value =
      static_cast<typename corevm::types::int64::value_type>(handle.value);

    return ~value;
  }
};

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::string::value_type
corevm::types::bitwise_not::operator()<corevm::types::string>(
  const corevm::types::string& handle)
{
  return static_cast<typename corevm::types::string::value_type>(~handle.value);
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::array::value_type
corevm::types::bitwise_not::operator()<corevm::types::array>(
  const corevm::types::array& handle)
{
  return static_cast<typename corevm::types::array::value_type>(~handle.value);
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::map::value_type
corevm::types::bitwise_not::operator()<corevm::types::map>(
  const corevm::types::map& handle)
{
  return static_cast<typename corevm::types::map::value_type>(~handle.value);
}

// -----------------------------------------------------------------------------

class truthy : public unary_op
{
public:
  template<typename R, typename T>
  typename R::value_type operator()(const T& handle)
  {
    typename corevm::types::boolean::value_type value =
      static_cast<typename corevm::types::boolean::value_type>(handle.value);

    return value;
  }
};

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::boolean::value_type
corevm::types::truthy::operator()<corevm::types::boolean>(
  const corevm::types::string& handle)
{
  return !handle.value.empty();
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::boolean::value_type
corevm::types::truthy::operator()<corevm::types::boolean>(
  const corevm::types::array& handle)
{
  return !handle.value.empty();
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::boolean::value_type
corevm::types::truthy::operator()<corevm::types::boolean>(
  const corevm::types::map& handle)
{
  return !handle.value.empty();
}

// -----------------------------------------------------------------------------

class repr: public unary_op
{
public:
  template<typename R, typename T>
  typename corevm::types::string::value_type operator()(const T& handle)
  {
    // TODO: The current precision is not always accurate.
    std::stringstream ss;
    ss << std::fixed << handle.value;

    typename corevm::types::string::value_type value =
      static_cast<typename corevm::types::string::value_type>(ss.str());

    return value;
  }
};

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::string::value_type
corevm::types::repr::operator()<corevm::types::array>(
  const corevm::types::array& handle)
{
  return static_cast<corevm::types::string::value_type>("<array>");
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::string::value_type
corevm::types::repr::operator()<corevm::types::map>(
  const corevm::types::map& handle)
{
  return static_cast<corevm::types::string::value_type>("<map>");
}

// -----------------------------------------------------------------------------

class hash: public unary_op
{
public:
  template<typename R, typename T>
  typename corevm::types::int64::value_type operator()(const T& handle)
  {
    std::hash<typename T::value_type> hash_func;
    typename corevm::types::int64::value_type value = hash_func(handle.value);
    return value;
  }
};

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::int64::value_type
corevm::types::hash::operator()<corevm::types::string>(
  const corevm::types::string& handle)
{
  uint64_t res = 0;

  std::hash<corevm::types::native_string_base> hash_func;

  res = hash_func(handle.value);

  return static_cast<corevm::types::int64::value_type>(res);
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::int64::value_type
corevm::types::hash::operator()<corevm::types::array>(
  const corevm::types::array& handle)
{
  uint64_t res = 0;

  std::hash<corevm::types::native_array_element_type> element_hash;

  for (auto itr = handle.value.cbegin(); itr != handle.value.cend(); ++itr)
  {
    const auto& value = *itr;
    res += element_hash(value);
  }

  return static_cast<corevm::types::int64::value_type>(res);
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::int64::value_type
corevm::types::hash::operator()<corevm::types::map>(
  const corevm::types::map& handle)
{
  uint64_t res = 0;

  std::hash<corevm::types::native_map_key_type> key_hash;
  std::hash<corevm::types::native_map_mapped_type> value_hash;

  for (auto itr = handle.value.begin(); itr != handle.value.end(); ++itr)
  {
    const auto& key = itr->first;
    const auto& value = itr->second;

    res += key_hash(key);
    res += value_hash(value);
  }

  return static_cast<corevm::types::int64::value_type>(res);
}

// -----------------------------------------------------------------------------

class addition : public binary_op
{
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs)
  {
    return (
      static_cast<typename R::value_type>(lhs.value) +
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::string::value_type
corevm::types::addition::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(
    lhs.value + rhs.value);
}

// -----------------------------------------------------------------------------

class subtraction : public binary_op
{
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs)
  {
    return (
      static_cast<typename R::value_type>(lhs.value) -
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};

// -----------------------------------------------------------------------------

class multiplication : public binary_op
{
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs)
  {
    return (
      static_cast<typename R::value_type>(lhs.value) *
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};

// -----------------------------------------------------------------------------

class division : public binary_op
{
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs)
  {
    return (
      static_cast<typename R::value_type>(lhs.value) /
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};

// -----------------------------------------------------------------------------

class modulus : public binary_op
{
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs)
  {
    return (
      static_cast<typename corevm::types::int64::value_type>(lhs.value) %
      static_cast<typename corevm::types::int64::value_type>(rhs.value)
    );
  }
};

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::string::value_type
corevm::types::modulus::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(
    lhs.value % rhs.value);
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::array::value_type
corevm::types::modulus::operator()<corevm::types::array>(
  const corevm::types::array& lhs, const corevm::types::array& rhs)
{
  return static_cast<typename corevm::types::array::value_type>(
    lhs.value % rhs.value);
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::map::value_type
corevm::types::modulus::operator()<corevm::types::map>(
  const corevm::types::map& lhs, const corevm::types::map& rhs)
{
  return static_cast<typename corevm::types::map::value_type>(
    lhs.value % rhs.value);
}

// -----------------------------------------------------------------------------

class pow_op : public binary_op
{
public:
  typename corevm::types::decimal::value_type operator()(
    const corevm::types::decimal& lhs, const corevm::types::decimal& rhs)
  {
    return pow(
      static_cast<typename corevm::types::decimal::value_type>(lhs.value),
      static_cast<typename corevm::types::decimal::value_type>(rhs.value)
    );
  }

  typename corevm::types::decimal2::value_type operator()(
    const corevm::types::decimal2& lhs, const corevm::types::decimal2& rhs)
  {
    return pow(
      static_cast<typename corevm::types::decimal2::value_type>(lhs.value),
      static_cast<typename corevm::types::decimal2::value_type>(rhs.value)
    );
  }

  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs)
  {
    return pow(
      static_cast<typename corevm::types::decimal2::value_type>(lhs.value),
      static_cast<typename corevm::types::decimal2::value_type>(rhs.value)
    );
  }
};

// -----------------------------------------------------------------------------

class logical_and : public binary_op
{
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs)
  {
    return (
      static_cast<typename R::value_type>(lhs.value) &&
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};

// -----------------------------------------------------------------------------

class logical_or : public binary_op
{
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs)
  {
    return (
      static_cast<typename R::value_type>(lhs.value) ||
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};

// -----------------------------------------------------------------------------

class bitwise_and : public binary_op
{
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs)
  {
    return (
      static_cast<typename corevm::types::int64::value_type>(lhs.value) &
      static_cast<typename corevm::types::int64::value_type>(rhs.value)
    );
  }
};

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::string::value_type
corevm::types::bitwise_and::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(
    lhs.value & rhs.value);
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::array::value_type
corevm::types::bitwise_and::operator()<corevm::types::array>(
  const corevm::types::array& lhs, const corevm::types::array& rhs)
{
  return static_cast<typename corevm::types::array::value_type>(
    lhs.value & rhs.value);
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::map::value_type
corevm::types::bitwise_and::operator()<corevm::types::map>(
  const corevm::types::map& lhs, const corevm::types::map& rhs)
{
  return static_cast<typename corevm::types::map::value_type>(
    lhs.value & rhs.value);
}

// -----------------------------------------------------------------------------

class bitwise_or : public binary_op
{
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs)
  {
    return (
      static_cast<typename corevm::types::int64::value_type>(lhs.value) |
      static_cast<typename corevm::types::int64::value_type>(rhs.value)
    );
  }
};

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::string::value_type
corevm::types::bitwise_or::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(
    lhs.value | rhs.value);
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::array::value_type
corevm::types::bitwise_or::operator()<corevm::types::array>(
  const corevm::types::array& lhs, const corevm::types::array& rhs)
{
  return static_cast<typename corevm::types::array::value_type>(
    lhs.value | rhs.value);
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::map::value_type
corevm::types::bitwise_or::operator()<corevm::types::map>(
  const corevm::types::map& lhs, const corevm::types::map& rhs)
{
  return static_cast<typename corevm::types::map::value_type>(
    lhs.value | rhs.value);
}

// -----------------------------------------------------------------------------

class bitwise_xor : public binary_op
{
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs)
  {
    return (
      static_cast<typename corevm::types::int64::value_type>(lhs.value) ^
      static_cast<typename corevm::types::int64::value_type>(rhs.value)
    );
  }
};

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::string::value_type
corevm::types::bitwise_xor::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(
    lhs.value ^ rhs.value);
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::array::value_type
corevm::types::bitwise_xor::operator()<corevm::types::array>(
  const corevm::types::array& lhs, const corevm::types::array& rhs)
{
  return static_cast<typename corevm::types::array::value_type>(
    lhs.value ^ rhs.value);
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::map::value_type
corevm::types::bitwise_xor::operator()<corevm::types::map>(
  const corevm::types::map& lhs, const corevm::types::map& rhs)
{
  return static_cast<typename corevm::types::map::value_type>(
    lhs.value ^ rhs.value);
}

// -----------------------------------------------------------------------------

class bitwise_left_shift : public binary_op
{
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs)
  {
    return (
      static_cast<typename corevm::types::int64::value_type>(lhs.value) <<
      static_cast<typename corevm::types::int64::value_type>(rhs.value)
    );
  }
};

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::string::value_type
corevm::types::bitwise_left_shift::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(
    lhs.value << rhs.value);
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::array::value_type
corevm::types::bitwise_left_shift::operator()<corevm::types::array>(
  const corevm::types::array& lhs, const corevm::types::array& rhs)
{
  return static_cast<typename corevm::types::array::value_type>(
    lhs.value << rhs.value);
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::map::value_type
corevm::types::bitwise_left_shift::operator()<corevm::types::map>(
  const corevm::types::map& lhs, const corevm::types::map& rhs)
{
  return static_cast<typename corevm::types::map::value_type>(
    lhs.value << rhs.value);
}

// -----------------------------------------------------------------------------

class bitwise_right_shift : public binary_op
{
public:
  template<typename R, typename T, typename U>
  typename R::value_type operator()(const T& lhs, const U& rhs)
  {
    return (
      static_cast<typename corevm::types::int64::value_type>(lhs.value) >>
      static_cast<typename corevm::types::int64::value_type>(rhs.value)
    );
  }
};

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::string::value_type
corevm::types::bitwise_right_shift::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return static_cast<typename corevm::types::string::value_type>(
    lhs.value >> rhs.value);
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::array::value_type
corevm::types::bitwise_right_shift::operator()<corevm::types::array>(
  const corevm::types::array& lhs, const corevm::types::array& rhs)
{
  return static_cast<typename corevm::types::array::value_type>(
    lhs.value >> rhs.value);
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::map::value_type
corevm::types::bitwise_right_shift::operator()<corevm::types::map>(
  const corevm::types::map& lhs, const corevm::types::map& rhs)
{
  return static_cast<typename corevm::types::map::value_type>(
    lhs.value >> rhs.value);
}

// -----------------------------------------------------------------------------

class eq : public binary_op
{
public:
  template<typename R, typename T, typename U>
  bool operator()(const T& lhs, const U& rhs)
  {
    return (
      static_cast<typename R::value_type>(lhs.value) ==
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};

// -----------------------------------------------------------------------------

class neq : public binary_op
{
public:
  template<typename R, typename T, typename U>
  bool operator()(const T& lhs, const U& rhs)
  {
    return (
      static_cast<typename R::value_type>(lhs.value) !=
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};

// -----------------------------------------------------------------------------

class gt : public binary_op
{
public:
  template<typename R, typename T, typename U>
  bool operator()(const T& lhs, const U& rhs)
  {
    return (
      static_cast<typename R::value_type>(lhs.value) >
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};

// -----------------------------------------------------------------------------

class lt : public binary_op
{
public:
  template<typename R, typename T, typename U>
  bool operator()(const T& lhs, const U& rhs)
  {
    return (
      static_cast<typename R::value_type>(lhs.value) <
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};

// -----------------------------------------------------------------------------

class gte : public binary_op
{
public:
  template<typename R, typename T, typename U>
  bool operator()(const T& lhs, const U& rhs)
  {
    return (
      static_cast<typename R::value_type>(lhs.value) >=
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};

// -----------------------------------------------------------------------------

class lte : public binary_op
{
public:
  template<typename R, typename T, typename U>
  bool operator()(const T& lhs, const U& rhs)
  {
    return (
      static_cast<typename R::value_type>(lhs.value) <=
      static_cast<typename R::value_type>(rhs.value)
    );
  }
};

// -----------------------------------------------------------------------------


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_OPERATORS_H_ */
