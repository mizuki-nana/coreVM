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


namespace corevm {


namespace types {


// -----------------------------------------------------------------------------

class absolute : public op<unary_op_tag>
{
public:
  template<typename T>
  T operator()(const T& handle)
  {
    corevm::types::decimal2 value = static_cast<corevm::types::decimal2>(handle);
    return std::abs(value);
  }
};

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::string
corevm::types::absolute::operator()(const corevm::types::string& handle)
{
  return corevm::types::string(
    std::abs(static_cast<corevm::types::int64>(handle)));
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::array
corevm::types::absolute::operator()(const corevm::types::array& handle)
{
  return corevm::types::array(
    std::abs(static_cast<corevm::types::int64>(handle)));
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::map
corevm::types::absolute::operator()(const corevm::types::map& handle)
{
  return corevm::types::map(
    std::abs(static_cast<corevm::types::int64>(handle)));
}

// -----------------------------------------------------------------------------

class sqrt : public op<unary_op_tag>
{
public:
  template<typename T>
  T operator()(const T& handle)
  {
    corevm::types::decimal2 value = static_cast<corevm::types::decimal2>(handle);
    return std::sqrt(value);
  }
};

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::string
corevm::types::sqrt::operator()(const corevm::types::string& handle)
{
  return corevm::types::string(
    std::sqrt(static_cast<corevm::types::int64>(handle)));
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::array
corevm::types::sqrt::operator()(const corevm::types::array& handle)
{
  return corevm::types::array(
    std::sqrt(static_cast<corevm::types::int64>(handle)));
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::map
corevm::types::sqrt::operator()(const corevm::types::map& handle)
{
  return corevm::types::map(
    std::sqrt(static_cast<corevm::types::int64>(handle)));
}

// -----------------------------------------------------------------------------

class truthy : public op<typed_unary_op_tag, corevm::types::boolean>
{
public:
  template<typename T>
  result_type operator()(const T& handle)
  {
    corevm::types::boolean value = static_cast<corevm::types::boolean>(handle);
    return value;
  }
};

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::truthy::result_type
corevm::types::truthy::operator()(const corevm::types::string& handle)
{
  return !handle.empty();
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::truthy::result_type
corevm::types::truthy::operator()(const corevm::types::array& handle)
{
  return !handle.empty();
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::truthy::result_type
corevm::types::truthy::operator()(const corevm::types::map& handle)
{
  return !handle.empty();
}

// -----------------------------------------------------------------------------

class repr: public op<typed_unary_op_tag, corevm::types::string>
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
    return boost::lexical_cast<corevm::types::string>(handle);
  }
};

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::repr::result_type
corevm::types::repr::operator()(const corevm::types::decimal& handle)
{
  char buf[20];
  snprintf(buf, sizeof(buf), "%0.6f", handle);
  return std::move(corevm::types::string(buf));
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::repr::result_type
corevm::types::repr::operator()(const corevm::types::decimal2& handle)
{
  char buf[20];
  snprintf(buf, sizeof(buf), "%0.6f", handle);
  return std::move(corevm::types::string(buf));
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::repr::result_type
corevm::types::repr::operator()(const corevm::types::string& handle)
{
  return handle;
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::repr::result_type
corevm::types::repr::operator()(const corevm::types::array& handle)
{
  return static_cast<corevm::types::string>("<array>");
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::repr::result_type
corevm::types::repr::operator()(const corevm::types::map& handle)
{
  return static_cast<corevm::types::string>("<map>");
}

// -----------------------------------------------------------------------------

class hash: public op<typed_unary_op_tag, corevm::types::int64>
{
public:
  template<typename T>
  result_type operator()(const T& handle)
  {
    std::hash<T> hash_func;
    corevm::types::int64 value = hash_func(handle);
    return value;
  }
};

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::hash::result_type
corevm::types::hash::operator()(const corevm::types::string& handle)
{
  uint64_t res = 0;

  std::hash<corevm::types::native_string_base> hash_func;

  res = hash_func(handle);

  return static_cast<corevm::types::int64>(res);
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::hash::result_type
corevm::types::hash::operator()(const corevm::types::array& handle)
{
  uint64_t res = 0;

  std::hash<corevm::types::native_array_element_type> element_hash;

  for (auto itr = handle.cbegin(); itr != handle.cend(); ++itr)
  {
    const auto& value = *itr;
    res += element_hash(value);
  }

  return static_cast<corevm::types::int64>(res);
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::hash::result_type
corevm::types::hash::operator()(const corevm::types::map& handle)
{
  uint64_t res = 0;

  std::hash<corevm::types::native_map_key_type> key_hash;
  std::hash<corevm::types::native_map_mapped_type> value_hash;

  for (auto itr = handle.begin(); itr != handle.end(); ++itr)
  {
    const auto& key = itr->first;
    const auto& value = itr->second;

    res += key_hash(key);
    res += value_hash(value);
  }

  return static_cast<corevm::types::int64>(res);
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
  T operator()(const T& handle) const
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
corevm::types::string
corevm::types::slice::operator()(const corevm::types::string& handle) const
{
  return slice_impl(handle);
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::array
corevm::types::slice::operator()(const corevm::types::array& handle) const
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
  T operator()(const T& handle) const
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
corevm::types::string
corevm::types::stride::operator()(const corevm::types::string& handle) const
{
  return stride_impl(handle);
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::array
corevm::types::stride::operator()(const corevm::types::array& handle) const
{
  return stride_impl(handle);
}

// -----------------------------------------------------------------------------

class reverse : public op<unary_op_tag>
{
public:
  template<typename T>
  T operator()(const T& handle) const
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
corevm::types::string
corevm::types::reverse::operator()(const corevm::types::string& handle) const
{
  return reverse_impl(handle);
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::array
corevm::types::reverse::operator()(const corevm::types::array& handle) const
{
  return reverse_impl(handle);
}

// -----------------------------------------------------------------------------

class cmp : public op<typed_binary_op_tag, corevm::types::int32>
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
corevm::types::cmp::result_type
corevm::types::cmp::operator()<corevm::types::string>(
  const corevm::types::string& lhs, const corevm::types::string& rhs)
{
  return clamp<int32_t>(lhs.compare(rhs), -1, 1);
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::cmp::result_type
corevm::types::cmp::operator()<corevm::types::array>(
  const corevm::types::array& lhs, const corevm::types::array& rhs)
{
  THROW(corevm::types::runtime_error("Calling 'cmp' operator on invalid type"));
}

// -----------------------------------------------------------------------------

template<>
inline
corevm::types::cmp::result_type
corevm::types::cmp::operator()<corevm::types::map>(
  const corevm::types::map& lhs, const corevm::types::map& rhs)
{
  THROW(corevm::types::runtime_error("Calling 'cmp' operator on invalid type"));
}

// -----------------------------------------------------------------------------


} /* end namespace types */


} /* end namespace corevm */


#endif /* COREVM_OPERATORS_COMPLEX_H_ */
