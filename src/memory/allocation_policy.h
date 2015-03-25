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
#ifndef COREVM_ALLOCATION_POLICY_H_
#define COREVM_ALLOCATION_POLICY_H_

#include "memory/allocator.h"

#include <sneaker/allocator/alloc_policy.h>

#include <cassert>
#include <cstdint>


namespace corevm {


namespace memory {


template<typename T, typename AllocationScheme>
class allocation_policy : public sneaker::allocator::standard_alloc_policy<T>
{
public:
  using value_type                             = typename sneaker::allocator::standard_alloc_policy<T>::value_type;
  using pointer                                = typename sneaker::allocator::standard_alloc_policy<T>::pointer;
  using const_pointer                          = typename sneaker::allocator::standard_alloc_policy<T>::const_pointer;
  using reference                              = typename sneaker::allocator::standard_alloc_policy<T>::reference;
  using const_reference                        = typename sneaker::allocator::standard_alloc_policy<T>::const_reference;
  using size_type                              = typename sneaker::allocator::standard_alloc_policy<T>::size_type;
  using difference_type                        = typename sneaker::allocator::standard_alloc_policy<T>::difference_type;
  using propagate_on_container_move_assignment = typename sneaker::allocator::standard_alloc_policy<T>::propagate_on_container_move_assignment;

  inline explicit allocation_policy(uint64_t);
  inline explicit allocation_policy(allocation_policy const&);
  inline ~allocation_policy();

  inline virtual pointer allocate(size_type, typename std::allocator<void>::const_pointer=0);
  inline virtual void deallocate(pointer, size_type);

  inline uint64_t base_addr() const;

  /**
   * The maximum number of bytes can be used.
   */
  inline uint64_t total_size() const;

  /**
   * The maximum number of elements can be allocated.
   */
  inline uint64_t max_size() const;

protected:
  corevm::memory::allocator<AllocationScheme> m_allocator;
};


// -----------------------------------------------------------------------------

namespace {


template<typename T, typename AllocationScheme>
using _MyType = typename corevm::memory::allocation_policy<T, AllocationScheme>;


} /* end namespace */

// -----------------------------------------------------------------------------

template<typename T, typename AllocationScheme>
corevm::memory::allocation_policy<T, AllocationScheme>::allocation_policy(
  uint64_t total_size)
  :
  m_allocator(total_size)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocationScheme>
corevm::memory::allocation_policy<T, AllocationScheme>::allocation_policy(
  allocation_policy const& other)
  :
  m_allocator(other.total_size())
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocationScheme>
corevm::memory::allocation_policy<T, AllocationScheme>::~allocation_policy()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocationScheme>
typename _MyType<T, AllocationScheme>::pointer
corevm::memory::allocation_policy<T, AllocationScheme>::allocate(
  typename allocation_policy<T, AllocationScheme>::size_type n,
  typename std::allocator<void>::const_pointer
)
{
  return reinterpret_cast<typename _MyType<T, AllocationScheme>::pointer>( m_allocator.allocate(n * sizeof(T)) );
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocationScheme>
void
corevm::memory::allocation_policy<T, AllocationScheme>::deallocate(
  typename allocation_policy<T, AllocationScheme>::pointer p,
  typename allocation_policy<T, AllocationScheme>::size_type
)
{
  int res = m_allocator.deallocate(p);
  assert(res == 1);
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocationScheme>
uint64_t
corevm::memory::allocation_policy<T, AllocationScheme>::base_addr() const
{
  return m_allocator.base_addr();
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocationScheme>
uint64_t
corevm::memory::allocation_policy<T, AllocationScheme>::total_size() const
{
  return m_allocator.total_size();
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocationScheme>
uint64_t
corevm::memory::allocation_policy<T, AllocationScheme>::max_size() const
{
  return total_size() / sizeof(T);
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocationScheme>
inline
bool operator==(
  allocation_policy<T, AllocationScheme> const& lhs,
  allocation_policy<T, AllocationScheme> const& rhs)
{
  return lhs.total_size() == rhs.total_size();
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocationScheme, typename U, typename OtherAllocationScheme>
inline
bool operator==(
  allocation_policy<T, AllocationScheme> const& lhs,
  allocation_policy<U, OtherAllocationScheme> const& rhs)
{
  return false;
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocationScheme, typename other_allocation_policy_type>
inline
bool operator==(
  allocation_policy<T, AllocationScheme> const& lhs,
  other_allocation_policy_type const& rhs)
{
  return false;
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocationScheme>
inline
bool operator!=(
  allocation_policy<T, AllocationScheme> const& lhs,
  allocation_policy<T, AllocationScheme> const& rhs)
{
  return !operator==(lhs, rhs);
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocationScheme, typename U, typename OtherAllocationScheme>
inline
bool operator!=(
  allocation_policy<T, AllocationScheme> const& lhs,
  allocation_policy<U, OtherAllocationScheme> const& rhs)
{
  return !operator==(lhs, rhs);
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocationScheme, typename other_allocation_policy_type>
inline
bool operator!=(
  allocation_policy<T, AllocationScheme> const& lhs, other_allocation_policy_type const& rhs)
{
  return !operator==(lhs, rhs);
}

// -----------------------------------------------------------------------------


} /* end namespace memory */


} /* end namespace corevm */


#endif /* COREVM_ALLOCATION_POLICY_H_ */
