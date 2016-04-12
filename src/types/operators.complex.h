/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Yanzheng Li

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
#ifndef COREVM_OPERATORS_COMPLEX_H_
#define COREVM_OPERATORS_COMPLEX_H_

#include "errors.h"
#include "operators.base.h"
#include "types.h"
#include "corevm/macros.h"

#include <boost/lexical_cast.hpp>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <functional>
#include <ios>

#if defined(__clang__) and __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wconversion"
  #pragma clang diagnostic ignored "-Wsign-conversion"
#endif


namespace corevm {


namespace types {


// -----------------------------------------------------------------------------

class absolute : public op<unary_op_tag>
{
public:
  template<typename T>
  T operator()(const T& handle)
  {
    decimal2 value = static_cast<decimal2>(handle);
    return std::abs(value);
  }
};

// -----------------------------------------------------------------------------

template<>
inline
string
absolute::operator()(const string& handle)
{
  return string(
    std::abs(static_cast<int64>(handle)));
}

// -----------------------------------------------------------------------------

template<>
inline
array
absolute::operator()(const array& handle)
{
  return array(
    std::abs(static_cast<int64>(handle)));
}

// -----------------------------------------------------------------------------

template<>
inline
map
absolute::operator()(const map& handle)
{
  return map(
    std::abs(static_cast<int64>(handle)));
}

// -----------------------------------------------------------------------------

class sqrt : public op<unary_op_tag>
{
public:
  template<typename T>
  T operator()(const T& handle)
  {
    decimal2 value = static_cast<decimal2>(handle);
    return std::sqrt(value);
  }
};

// -----------------------------------------------------------------------------

template<>
inline
string
sqrt::operator()(const string& handle)
{
  return string(
    std::sqrt(static_cast<int64>(handle)));
}

// -----------------------------------------------------------------------------

template<>
inline
array
sqrt::operator()(const array& handle)
{
  return array(
    std::sqrt(static_cast<int64>(handle)));
}

// -----------------------------------------------------------------------------

template<>
inline
map
sqrt::operator()(const map& handle)
{
  return map(
    std::sqrt(static_cast<int64>(handle)));
}

// -----------------------------------------------------------------------------

class truthy : public op<typed_unary_op_tag, boolean>
{
public:
  template<typename T>
  result_type operator()(const T& handle)
  {
    boolean value = static_cast<boolean>(handle);
    return value;
  }
};

// -----------------------------------------------------------------------------

template<>
inline
truthy::result_type
truthy::operator()(const string& handle)
{
  return !handle.empty();
}

// -----------------------------------------------------------------------------

template<>
inline
truthy::result_type
truthy::operator()(const array& handle)
{
  return !handle.empty();
}

// -----------------------------------------------------------------------------

template<>
inline
truthy::result_type
truthy::operator()(const map& handle)
{
  return !handle.empty();
}

// -----------------------------------------------------------------------------

class repr: public op<typed_unary_op_tag, string>
{
public:
  template<typename T>
  result_type operator()(const T& handle)
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
    return boost::lexical_cast<string>(handle);
  }
};

// -----------------------------------------------------------------------------

template<>
inline
repr::result_type
repr::operator()(const decimal& handle)
{
  char buf[20];
  snprintf(buf, sizeof(buf), "%0.6f", handle);
  return std::move(string(buf));
}

// -----------------------------------------------------------------------------

template<>
inline
repr::result_type
repr::operator()(const decimal2& handle)
{
  char buf[20];
  snprintf(buf, sizeof(buf), "%0.6f", handle);
  return std::move(string(buf));
}

// -----------------------------------------------------------------------------

template<>
inline
repr::result_type
repr::operator()(const string& handle)
{
  return handle;
}

// -----------------------------------------------------------------------------

template<>
inline
repr::result_type
repr::operator()(const array& /* handle */)
{
  return static_cast<string>("<array>");
}

// -----------------------------------------------------------------------------

template<>
inline
repr::result_type
repr::operator()(const map& /* handle */)
{
  return static_cast<string>("<map>");
}

// -----------------------------------------------------------------------------

class hash: public op<typed_unary_op_tag, int64>
{
public:
  template<typename T>
  result_type operator()(const T& handle)
  {
    std::hash<T> hash_func;
    uint64 value = hash_func(handle);
    return static_cast<result_type>(value);
  }
};

// -----------------------------------------------------------------------------

template<>
inline
hash::result_type
hash::operator()(const string& handle)
{
  uint64_t res = 0;

  std::hash<native_string_base> hash_func;

  res = hash_func(handle);

  return static_cast<int64>(res);
}

// -----------------------------------------------------------------------------

template<>
inline
hash::result_type
hash::operator()(const array& handle)
{
  uint64_t res = 0;

  std::hash<native_array_element_type> element_hash;

  for (auto itr = handle.cbegin(); itr != handle.cend(); ++itr)
  {
    const auto& value = *itr;
    res += element_hash(value);
  }

  return static_cast<int64>(res);
}

// -----------------------------------------------------------------------------

template<>
inline
hash::result_type
hash::operator()(const map& handle)
{
  uint64_t res = 0;

  std::hash<native_map_key_type> key_hash;
  std::hash<native_map_mapped_type> value_hash;

  for (auto itr = handle.begin(); itr != handle.end(); ++itr)
  {
    const auto& key = itr->first;
    const auto& value = itr->second;

    res += key_hash(key);
    res += value_hash(value);
  }

  return static_cast<int64>(res);
}

// -----------------------------------------------------------------------------

class slice : public op<unary_op_tag>
{
public:
  slice(uint32_t start, uint32_t stop)
    :
    m_start(start),
    m_stop(stop)
  {
  }

  template<typename T>
  T operator()(const T& /* handle */) const
  {
    THROW(RuntimeError("Calling 'slice' operator on invalid type"));
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
string
slice::operator()(const string& handle) const
{
  return slice_impl(handle);
}

// -----------------------------------------------------------------------------

template<>
inline
array
slice::operator()(const array& handle) const
{
  return slice_impl(handle);
}

// -----------------------------------------------------------------------------

class stride : public op<unary_op_tag>
{
public:
  explicit stride(int32_t stride)
    :
    m_stride(stride)
  {
  }

  template<typename T>
  T operator()(const T& /* handle */) const
  {
    THROW(RuntimeError("Calling 'stride' operator on invalid type"));
  }

private:
  template<typename T>
  T stride_impl(const T& val) const
  {
    T res;

    if (m_stride > 0)
    {
      res.reserve(val.size());

      size_t stride_val = static_cast<size_t>(m_stride);

      auto begin = val.begin();
      size_t stepped_index = 0;
      while (stepped_index < val.size())
      {
        auto itr = begin;
        std::advance(itr, stepped_index);
        res.push_back(*itr);
        stepped_index += stride_val;
      }
    }

    return res;
  }

  int32_t m_stride;
};

// -----------------------------------------------------------------------------

template<>
inline
string
stride::operator()(const string& handle) const
{
  return stride_impl(handle);
}

// -----------------------------------------------------------------------------

template<>
inline
array
stride::operator()(const array& handle) const
{
  return stride_impl(handle);
}

// -----------------------------------------------------------------------------

class reverse : public op<unary_op_tag>
{
public:
  template<typename T>
  T operator()(const T& /* handle */) const
  {
    THROW(RuntimeError("Calling 'reverse' operator on invalid type"));
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
string
reverse::operator()(const string& handle) const
{
  return reverse_impl(handle);
}

// -----------------------------------------------------------------------------

template<>
inline
array
reverse::operator()(const array& handle) const
{
  return reverse_impl(handle);
}

// -----------------------------------------------------------------------------

class cmp : public op<typed_binary_op_tag, int32>
{
public:
  template<typename DominantType, typename T, typename U>
  result_type operator()(const T& lhs, const U& rhs)
  {
    const auto& lhs_val = static_cast<DominantType>(lhs);
    const auto& rhs_val = static_cast<DominantType>(rhs);

    if (lhs_val < rhs_val)
    {
      return -1;
    }
    else if (lhs_val == rhs_val)
    {
      return 0;
    }
    else
    {
      return 1;
    }
  }
};

// -----------------------------------------------------------------------------

// TODO: Move this to sneaker or somewhere else.
template<typename T>
T clamp(T val, T lower, T upper)
{
  return std::max(lower, std::min(val, upper));
}

// -----------------------------------------------------------------------------

template<>
inline
cmp::result_type
cmp::operator()<string>(
  const string& lhs, const string& rhs)
{
  return clamp<int32_t>(lhs.compare(rhs), -1, 1);
}

// -----------------------------------------------------------------------------

template<>
inline
cmp::result_type
cmp::operator()<array>(
  const array& /* lhs */, const array& /* rhs */)
{
  THROW(RuntimeError("Calling 'cmp' operator on invalid type"));
}

// -----------------------------------------------------------------------------

template<>
inline
cmp::result_type
cmp::operator()<map>(
  const map& /* lhs */, const map& /* rhs */)
{
  THROW(RuntimeError("Calling 'cmp' operator on invalid type"));
}

// -----------------------------------------------------------------------------


} /* end namespace types */


} /* end namespace corevm */


#if defined(__clang__) and __clang__
  #pragma clang diagnostic pop
#endif


#endif /* COREVM_OPERATORS_COMPLEX_H_ */
