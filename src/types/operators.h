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

#include <boost/lexical_cast.hpp>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <functional>
#include <ios>


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
    // NOTES:
    //  (1) The curent approach has precision and accuracy issues.
    //  (2) Using `boost::lexical_cast` has ~ 3x performance gain over
    //      using `std::stringstream`, obeserved in micro benchmark.
    //
    // Benchmarks for `boost::lexical_cast` vs. `std::stringstream` vs. `sprintf`:
    // http://www.boost.org/doc/libs/1_58_0/doc/html/boost_lexical_cast/performance.html
    //
    // TODO: consider using `sprintf` here.
    return boost::lexical_cast<corevm::types::string::value_type>(handle.value);
  }
};

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::string::value_type
corevm::types::repr::operator()<corevm::types::decimal>(
  const corevm::types::decimal& handle)
{
  char buf[20];
  snprintf(buf, sizeof(buf), "%0.6f", handle.value);
  return std::move(corevm::types::string::value_type(buf));
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::string::value_type
corevm::types::repr::operator()<corevm::types::decimal2>(
  const corevm::types::decimal2& handle)
{
  char buf[20];
  snprintf(buf, sizeof(buf), "%0.6f", handle.value);
  return std::move(corevm::types::string::value_type(buf));
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::string::value_type
corevm::types::repr::operator()<corevm::types::string>(
  const corevm::types::string& handle)
{
  return handle.value;
}

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

class slice : public unary_op
{
public:
  slice(uint32_t start, uint32_t stop)
    :
    unary_op(),
    m_start(start),
    m_stop(stop)
  {
  }

  template<typename R, typename T>
  typename R::value_type operator()(const T& handle) const
  {
    THROW(corevm::types::runtime_error("Calling 'slice' operator on invalid type"));
  }

  uint32_t m_start;
  uint32_t m_stop;

private:
  template<typename T>
  T slice_impl(const T& val) const
  {
    if (m_start < m_stop)
    {
        auto begin = m_start >= val.size() ? val.end() : val.begin() + m_start;
        auto end = m_stop >= val.size() ? val.end() : val.begin() + m_stop;
        T res(begin, end);
        return res;
    }

    return T();
  }
};

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::string::value_type
corevm::types::slice::operator()<corevm::types::string>(
  const corevm::types::string& handle) const
{
  return slice_impl(handle.value);
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::array::value_type
corevm::types::slice::operator()<corevm::types::array>(
  const corevm::types::array& handle) const
{
  return slice_impl(handle.value);
}

// -----------------------------------------------------------------------------

class stride : public unary_op
{
public:
  explicit stride(int32_t stride)
    :
    unary_op(),
    m_stride(stride)
  {
  }

  template<typename R, typename T>
  typename R::value_type operator()(const T& handle) const
  {
    THROW(corevm::types::runtime_error("Calling 'stride' operator on invalid type"));
  }

  int32_t m_stride;

private:
  template<typename T>
  T stride_impl(const T& val) const
  {
    T res;

    if (m_stride > 0)
    {
      res.reserve(val.size());

      auto begin = val.begin();
      size_t size = 0;
      while (size < val.size())
      {
        auto itr = begin + size;
        res.push_back(*itr);
        size += m_stride;
      }
    }

    return res;
  }
};

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::string::value_type
corevm::types::stride::operator()<corevm::types::string>(
  const corevm::types::string& handle) const
{
  return stride_impl(handle.value);
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::array::value_type
corevm::types::stride::operator()<corevm::types::array>(
  const corevm::types::array& handle) const
{
  return stride_impl(handle.value);
}

// -----------------------------------------------------------------------------

class reverse : public unary_op
{
public:
  template<typename R, typename T>
  typename R::value_type operator()(const T& handle) const
  {
    THROW(corevm::types::runtime_error("Calling 'reverse' operator on invalid type"));
  }

private:
  template<typename T>
  T reverse_impl(const T& val) const
  {
    T res;
    res.resize(val.size());

    std::reverse_copy(val.begin(), val.end(), res.begin());

    return res;
  }
};

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::string::value_type
corevm::types::reverse::operator()<corevm::types::string>(
  const corevm::types::string& handle) const
{
  return reverse_impl(handle.value);
}

// -----------------------------------------------------------------------------

template<>
inline
typename corevm::types::array::value_type
corevm::types::reverse::operator()<corevm::types::array>(
  const corevm::types::array& handle) const
{
  return reverse_impl(handle.value);
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
    return fmod(
      static_cast<typename corevm::types::decimal2::value_type>(lhs.value),
      static_cast<typename corevm::types::decimal2::value_type>(rhs.value)
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
