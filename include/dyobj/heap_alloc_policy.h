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
#ifndef COREVM_HEAP_ALLOC_POLICY_H_
#define COREVM_HEAP_ALLOC_POLICY_H_

#include <sneaker/allocator/alloc_policy.h>
#include "../memory/allocator.h"
#include "../memory/sequential_allocation_scheme.h"
#include "common.h"


namespace corevm {


namespace dyobj {


template<typename T, size_t N=COREVM_DEFAULT_HEAP_SIZE>
class heap_alloc_policy : public sneaker::allocator::standard_alloc_policy<T> {
public:
  using value_type                             = typename sneaker::allocator::standard_alloc_policy<T>::value_type;
  using pointer                                = typename sneaker::allocator::standard_alloc_policy<T>::pointer;
  using const_pointer                          = typename sneaker::allocator::standard_alloc_policy<T>::const_pointer;
  using reference                              = typename sneaker::allocator::standard_alloc_policy<T>::reference;
  using const_reference                        = typename sneaker::allocator::standard_alloc_policy<T>::const_reference;
  using size_type                              = typename sneaker::allocator::standard_alloc_policy<T>::size_type;
  using difference_type                        = typename sneaker::allocator::standard_alloc_policy<T>::difference_type;
  using propagate_on_container_move_assignment = typename sneaker::allocator::standard_alloc_policy<T>::propagate_on_container_move_assignment;

  inline explicit heap_alloc_policy();
  inline explicit heap_alloc_policy(heap_alloc_policy const&);
  inline ~heap_alloc_policy();

  inline virtual pointer allocate(size_type, typename std::allocator<void>::const_pointer=0);
  inline virtual void deallocate(pointer, size_type);

  inline uint64_t base_addr() const;

  inline uint64_t total_size() const;

protected:
  corevm::memory::allocator<
    N,
    corevm::memory::buddy_allocation_scheme
  > m_allocator;
};


template<typename T, size_t N>
using _MyType = typename corevm::dyobj::heap_alloc_policy<T, N>;


template<typename T, size_t N>
corevm::dyobj::heap_alloc_policy<T, N>::heap_alloc_policy():
  sneaker::allocator::standard_alloc_policy<T>(),
  m_allocator(
    corevm::memory::allocator<N, corevm::memory::buddy_allocation_scheme>()
  )
{
  // Do nothing here.
}

template<typename T, size_t N>
corevm::dyobj::heap_alloc_policy<T, N>::heap_alloc_policy(
  heap_alloc_policy const&)
{
  // Do nothing here.
}

template<typename T, size_t N>
corevm::dyobj::heap_alloc_policy<T, N>::~heap_alloc_policy()
{
  // Do nothing here.
}

template<typename T, size_t N>
typename _MyType<T, N>::pointer
corevm::dyobj::heap_alloc_policy<T, N>::allocate(
  typename heap_alloc_policy<T, N>::size_type n,
  typename std::allocator<void>::const_pointer
)
{
  return reinterpret_cast<typename _MyType<T, N>::pointer>( m_allocator.allocate(n * sizeof(T)) );
}

template<typename T, size_t N>
void
corevm::dyobj::heap_alloc_policy<T, N>::deallocate(
  typename heap_alloc_policy<T, N>::pointer p,
  typename heap_alloc_policy<T, N>::size_type
)
{
  m_allocator.deallocate(p);
}

template<typename T, size_t N>
uint64_t
corevm::dyobj::heap_alloc_policy<T, N>::base_addr() const
{
  return m_allocator.base_addr();
}

template<typename T, size_t N>
uint64_t
corevm::dyobj::heap_alloc_policy<T, N>::total_size() const
{
  return m_allocator.total_size();
}


template<typename T, typename U>
inline
bool operator==(heap_alloc_policy<T> const& lhs, heap_alloc_policy<U> const& rhs)
{
  return true;
}

template<typename T, typename other_allocator_policy_type>
inline
bool operator==(heap_alloc_policy<T> const& lhs, other_allocator_policy_type const& rhs)
{
  return false;
}

template<typename T, typename U>
inline
bool operator!=(heap_alloc_policy<T> const& lhs, heap_alloc_policy<U> const& rhs)
{
  return !operator==(lhs, rhs);
}

template<typename T, typename other_allocator_policy_type>
inline
bool operator!=(heap_alloc_policy<T> const& lhs, other_allocator_policy_type const& rhs)
{
  return !operator==(lhs, rhs);
}


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_HEAP_ALLOC_POLICY_H_ */