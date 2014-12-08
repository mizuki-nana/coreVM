/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2014 Yanzheng Li

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
#ifndef COREVM_HEAP_ALLOCATOR_H_
#define COREVM_HEAP_ALLOCATOR_H_

#include <sneaker/allocator/allocator.h>
#include "common.h"
#include "heap_alloc_policy.h"


namespace corevm {


namespace dyobj {


template<typename T, size_t N=COREVM_DEFAULT_HEAP_SIZE>
class heap_allocator : public sneaker::allocator::allocator<T, corevm::dyobj::heap_alloc_policy<T, N>> {
public:
  using _BaseType = typename sneaker::allocator::allocator<T, corevm::dyobj::heap_alloc_policy<T, N> >;

  using value_type      = typename _BaseType::value_type;
  using pointer         = typename _BaseType::pointer;
  using const_pointer   = typename _BaseType::const_pointer;
  using reference       = typename _BaseType::reference;
  using const_reference = typename _BaseType::const_reference;
  using size_type       = typename _BaseType::size_type;
  using difference_type = typename _BaseType::difference_type;

  inline explicit heap_allocator();
  inline ~heap_allocator();

  inline heap_allocator(heap_allocator const&);

  template<typename U>
  inline heap_allocator(heap_allocator<U> const&);

  template<typename U, size_t M>
  inline heap_allocator(heap_allocator<U, M> const&);
};


template<typename T, size_t N>
corevm::dyobj::heap_allocator<T, N>::heap_allocator()
{
  // Do nothing here.
}

template<typename T, size_t N>
corevm::dyobj::heap_allocator<T, N>::~heap_allocator()
{
  // Do nothing here.
}

template<typename T, size_t N>
corevm::dyobj::heap_allocator<T, N>::heap_allocator(heap_allocator<T, N> const&)
{
  // Do nothing here.
}

template<typename T, size_t N>
template<typename U, size_t M>
corevm::dyobj::heap_allocator<T, N>::heap_allocator(heap_allocator<U, M> const&)
{
  // Do nothing here.
}


template<typename T, size_t N>
inline
bool operator==(heap_allocator<T, N> const& lhs, heap_allocator<T, N> const& rhs)
{
  return operator==(
    static_cast<corevm::dyobj::heap_alloc_policy<T, N>>(lhs),
    static_cast<corevm::dyobj::heap_alloc_policy<T, N>>(rhs)
  );
}

template<typename T, size_t N, typename U, size_t M>
inline
bool operator==(heap_allocator<T, N> const& lhs, heap_allocator<U, M> const& rhs)
{
  return operator==(
    static_cast<corevm::dyobj::heap_alloc_policy<T, N>>(lhs),
    static_cast<corevm::dyobj::heap_alloc_policy<U, M>>(rhs)
  );
}

template<typename T, size_t N, typename other_allocator_type>
inline
bool operator==(heap_allocator<T, N> const& lhs, other_allocator_type const& rhs)
{
  return operator==(
    static_cast<corevm::dyobj::heap_alloc_policy<T, N>>(lhs),
    rhs
  );
}

template<typename T, size_t N>
inline
bool operator!=(heap_allocator<T, N> const& lhs, heap_allocator<T, N> const& rhs)
{
  return !operator==(lhs, rhs);
}

template<typename T, size_t N, typename U, size_t M>
inline
bool operator!=(heap_allocator<T, N> const& lhs, heap_allocator<U, M> const& rhs)
{
  return !operator==(lhs, rhs);
}

template<typename T, size_t N, typename other_allocator_type>
inline
bool operator!=(heap_allocator<T, N> const& lhs, other_allocator_type const& rhs)
{
  return !operator==(lhs, rhs);
}


}; /* end namespace dyobj */


}; /* end namespace corevm */


#endif /* COREVM_HEAP_ALLOCATOR_H_ */
