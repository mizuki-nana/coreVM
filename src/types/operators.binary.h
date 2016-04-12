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
#ifndef COREVM_OPERATORS_BINARY_H_
#define COREVM_OPERATORS_BINARY_H_

#include "operators.base.h"
#include "types.h"

#if defined(__clang__) and __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wconversion"
  #pragma clang diagnostic ignored "-Wsign-conversion"
#endif


namespace corevm {


namespace types {


// -----------------------------------------------------------------------------

class addition : public op<binary_op_tag>
{
public:
  template<typename DominantType, typename T, typename U>
  DominantType operator()(const T& lhs, const U& rhs)
  {
    return ( static_cast<DominantType>(lhs) + static_cast<DominantType>(rhs) );
  }
};

// -----------------------------------------------------------------------------

template<>
inline
string
addition::operator()<string>(
  const string& lhs, const string& rhs)
{
  return static_cast<string>(lhs + rhs);
}

// -----------------------------------------------------------------------------

class subtraction : public op<binary_op_tag>
{
public:
  template<typename DominantType, typename T, typename U>
  DominantType operator()(const T& lhs, const U& rhs)
  {
    return ( static_cast<DominantType>(lhs) - static_cast<DominantType>(rhs) );
  }
};

// -----------------------------------------------------------------------------

class multiplication : public op<binary_op_tag>
{
public:
  template<typename DominantType, typename T, typename U>
  DominantType operator()(const T& lhs, const U& rhs)
  {
    return ( static_cast<DominantType>(lhs) * static_cast<DominantType>(rhs) );
  }
};

// -----------------------------------------------------------------------------

class division : public op<binary_op_tag>
{
public:
  template<typename DominantType, typename T, typename U>
  DominantType operator()(const T& lhs, const U& rhs)
  {
    return ( static_cast<DominantType>(lhs) / static_cast<DominantType>(rhs) );
  }
};

// -----------------------------------------------------------------------------

class modulus : public op<binary_op_tag>
{
public:
  template<typename DominantType, typename T, typename U>
  DominantType operator()(const T& lhs, const U& rhs)
  {
    return modulus_impl<DominantType>(
      static_cast<decimal2>(lhs),
      static_cast<decimal2>(rhs)
    );
  }

private:
  template<typename R>
  R modulus_impl(
      const decimal2& lhs,
      const decimal2& rhs)
  {
    double res = fmod(lhs, rhs);

    const int lhs_int = static_cast<int>(lhs);
    const int rhs_int = static_cast<int>(rhs);

    /**
     * If the remainder is non-zero and the two operands are off different
     * signs, we want to add the denominator to the remainder.
     *
     * [COREVM-318] Modulo expressions do not match CPython results
     */
    if (res != 0.0 && (lhs_int ^ rhs_int) < 0)
    {
        res = res + rhs;
    }

    return res;
  }
};

// -----------------------------------------------------------------------------

template<>
inline
string
modulus::operator()<string>(
  const string& lhs, const string& rhs)
{
  return static_cast<string>(lhs % rhs);
}

// -----------------------------------------------------------------------------

template<>
inline
array
modulus::operator()<array>(
  const array& lhs, const array& rhs)
{
  return static_cast<array>(lhs % rhs);
}

// -----------------------------------------------------------------------------

template<>
inline
map
modulus::operator()<map>(
  const map& lhs, const map& rhs)
{
  return static_cast<map>(lhs % rhs);
}

// -----------------------------------------------------------------------------

class pow_op : public op<binary_op_tag>
{
public:
  decimal operator()(
    const decimal& lhs, const decimal& rhs)
  {
    return pow(
      static_cast<decimal>(lhs),
      static_cast<decimal>(rhs)
    );
  }

  template<typename DominantType, typename T, typename U>
  DominantType operator()(const T& lhs, const U& rhs)
  {
    return pow(
      static_cast<decimal2>(lhs),
      static_cast<decimal2>(rhs)
    );
  }
};

// -----------------------------------------------------------------------------

class logical_and : public op<typed_binary_op_tag, boolean>
{
public:
  template<typename DominantType, typename T, typename U>
  result_type operator()(const T& lhs, const U& rhs)
  {
    return ( static_cast<DominantType>(lhs) && static_cast<DominantType>(rhs) );
  }
};

// -----------------------------------------------------------------------------

class logical_or : public op<typed_binary_op_tag, boolean>
{
public:
  template<typename DominantType, typename T, typename U>
  result_type operator()(const T& lhs, const U& rhs)
  {
    return ( static_cast<DominantType>(lhs) || static_cast<DominantType>(rhs) );
  }
};

// -----------------------------------------------------------------------------

class bitwise_and : public op<typed_cast_binary_op_tag, int64>
{
public:
  typedef result_type cast_type;

  template<typename T, typename U>
  result_type operator()(const T& lhs, const U& rhs)
  {
    return ( static_cast<cast_type>(lhs) & static_cast<cast_type>(rhs) );
  }
};

// -----------------------------------------------------------------------------

class bitwise_or : public op<typed_cast_binary_op_tag, int64>
{
public:
  typedef result_type cast_type;

  template<typename T, typename U>
  result_type operator()(const T& lhs, const U& rhs)
  {
    return ( static_cast<cast_type>(lhs) | static_cast<cast_type>(rhs) );
  }
};

// -----------------------------------------------------------------------------

class bitwise_xor : public op<typed_cast_binary_op_tag, int64>
{
public:
  typedef result_type cast_type;

  template<typename T, typename U>
  result_type operator()(const T& lhs, const U& rhs)
  {
    return ( static_cast<cast_type>(lhs) ^ static_cast<cast_type>(rhs) );
  }
};

// -----------------------------------------------------------------------------

class bitwise_left_shift : public op<typed_cast_binary_op_tag, int64>
{
public:
  typedef result_type cast_type;

  template<typename T, typename U>
  result_type operator()(const T& lhs, const U& rhs)
  {
    return ( static_cast<cast_type>(lhs) << static_cast<cast_type>(rhs) );
  }
};

// -----------------------------------------------------------------------------

class bitwise_right_shift : public op<typed_cast_binary_op_tag, int64>
{
public:
  typedef result_type cast_type;

  template<typename T, typename U>
  result_type operator()(const T& lhs, const U& rhs)
  {
    return ( static_cast<cast_type>(lhs) >> static_cast<cast_type>(rhs) );
  }
};

// -----------------------------------------------------------------------------

class eq : public op<typed_binary_op_tag, boolean>
{
public:
  template<typename DominantType, typename T, typename U>
  result_type operator()(const T& lhs, const U& rhs)
  {
    return ( static_cast<DominantType>(lhs) == static_cast<DominantType>(rhs) );
  }
};

// -----------------------------------------------------------------------------

class neq : public op<typed_binary_op_tag, boolean>
{
public:
  template<typename DominantType, typename T, typename U>
  result_type operator()(const T& lhs, const U& rhs)
  {
    return ( static_cast<DominantType>(lhs) != static_cast<DominantType>(rhs) );
  }
};

// -----------------------------------------------------------------------------

class gt : public op<typed_binary_op_tag, boolean>
{
public:
  template<typename DominantType, typename T, typename U>
  result_type operator()(const T& lhs, const U& rhs)
  {
    return ( static_cast<DominantType>(lhs) > static_cast<DominantType>(rhs) );
  }
};

// -----------------------------------------------------------------------------

class lt : public op<typed_binary_op_tag, boolean>
{
public:
  template<typename DominantType, typename T, typename U>
  result_type operator()(const T& lhs, const U& rhs)
  {
    return ( static_cast<DominantType>(lhs) < static_cast<DominantType>(rhs) );
  }
};

// -----------------------------------------------------------------------------

class gte : public op<typed_binary_op_tag, boolean>
{
public:
  template<typename DominantType, typename T, typename U>
  result_type operator()(const T& lhs, const U& rhs)
  {
    return ( static_cast<DominantType>(lhs) >= static_cast<DominantType>(rhs) );
  }
};

// -----------------------------------------------------------------------------

class lte : public op<typed_binary_op_tag, boolean>
{
public:
  template<typename DominantType, typename T, typename U>
  result_type operator()(const T& lhs, const U& rhs)
  {
    return ( static_cast<DominantType>(lhs) <= static_cast<DominantType>(rhs) );
  }
};

// -----------------------------------------------------------------------------


} /* end namespace types */


} /* end namespace corevm */


#if defined(__clang__) and __clang__
  #pragma clang diagnostic pop
#endif


#endif /* COREVM_OPERATORS_BINARY_H_ */
