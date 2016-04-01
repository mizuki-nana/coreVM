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

#include "corevm/macros.h"

#include <sneaker/allocator/alloc_policy.h>

#include <cstdint>


namespace corevm {


namespace memory {


template<typename T, typename CoreAllocatorType>
class AllocationPolicy : public sneaker::allocator::standard_alloc_policy<T>
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

  using iterator        = typename CoreAllocatorType::iterator;
  using const_iterator  = typename CoreAllocatorType::const_iterator;

  inline explicit AllocationPolicy(uint64_t);
  inline explicit AllocationPolicy(AllocationPolicy const&);
  inline virtual ~AllocationPolicy();

  inline virtual pointer allocate(
    size_type, typename std::allocator<void>::const_pointer=0);

  inline virtual void deallocate(pointer, size_type);

  inline uint64_t base_addr() const;

  iterator begin();
  iterator end();

  const_iterator cbegin() const;
  const_iterator cend() const;

  pointer find(pointer) const;

  /**
   * The maximum number of bytes can be used.
   */
  inline uint64_t total_size() const;

  /**
   * The maximum number of elements can be allocated.
   */
  inline uint64_t max_size() const;

protected:
  CoreAllocatorType m_allocator;
};


// -----------------------------------------------------------------------------

namespace {


template<typename T, typename CoreAllocatorType>
using _MyType = typename corevm::memory::AllocationPolicy<T, CoreAllocatorType>;


} /* end namespace */

// -----------------------------------------------------------------------------

template<typename T, typename CoreAllocatorType>
AllocationPolicy<T, CoreAllocatorType>::AllocationPolicy(uint64_t total_size)
  :
  m_allocator(total_size)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<typename T, typename CoreAllocatorType>
AllocationPolicy<T, CoreAllocatorType>::AllocationPolicy(
  AllocationPolicy const& other)
  :
  m_allocator(other.total_size())
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

/* virtual */
template<typename T, typename CoreAllocatorType>
AllocationPolicy<T, CoreAllocatorType>::~AllocationPolicy()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<typename T, typename CoreAllocatorType>
typename _MyType<T, CoreAllocatorType>::pointer
AllocationPolicy<T, CoreAllocatorType>::allocate(
  typename AllocationPolicy<T, CoreAllocatorType>::size_type n,
  typename std::allocator<void>::const_pointer
)
{
  return reinterpret_cast<typename _MyType<T, CoreAllocatorType>::pointer>(
    m_allocator.allocate_n(n, sizeof(T)));
}

// -----------------------------------------------------------------------------

template<typename T, typename CoreAllocatorType>
void
AllocationPolicy<T, CoreAllocatorType>::deallocate(
  typename AllocationPolicy<T, CoreAllocatorType>::pointer p,
  typename AllocationPolicy<T, CoreAllocatorType>::size_type
)
{
#if __DEBUG__
  int res = m_allocator.deallocate(p);
  ASSERT(res == 1);
#else
  m_allocator.deallocate(p);
#endif
}

// -----------------------------------------------------------------------------

template<typename T, typename CoreAllocatorType>
uint64_t
AllocationPolicy<T, CoreAllocatorType>::base_addr() const
{
  return m_allocator.base_addr();
}

// -----------------------------------------------------------------------------

template<typename T, typename CoreAllocatorType>
uint64_t
AllocationPolicy<T, CoreAllocatorType>::total_size() const
{
  return m_allocator.total_size();
}

// -----------------------------------------------------------------------------

template<typename T, typename CoreAllocatorType>
uint64_t
AllocationPolicy<T, CoreAllocatorType>::max_size() const
{
  return total_size() / sizeof(T);
}

// -----------------------------------------------------------------------------

template<typename T, typename CoreAllocatorType>
typename AllocationPolicy<T, CoreAllocatorType>::iterator
AllocationPolicy<T, CoreAllocatorType>::begin()
{
  return m_allocator.begin();
}

// -----------------------------------------------------------------------------

template<typename T, typename CoreAllocatorType>
typename AllocationPolicy<T, CoreAllocatorType>::iterator
AllocationPolicy<T, CoreAllocatorType>::end()
{
  return m_allocator.end();
}

// -----------------------------------------------------------------------------

template<typename T, typename CoreAllocatorType>
typename AllocationPolicy<T, CoreAllocatorType>::const_iterator
AllocationPolicy<T, CoreAllocatorType>::cbegin() const
{
  return m_allocator.cbegin();
}

// -----------------------------------------------------------------------------

template<typename T, typename CoreAllocatorType>
typename AllocationPolicy<T, CoreAllocatorType>::const_iterator
AllocationPolicy<T, CoreAllocatorType>::cend() const
{
  return m_allocator.cend();
}

// -----------------------------------------------------------------------------

template<typename T, typename CoreAllocatorType>
typename AllocationPolicy<T, CoreAllocatorType>::pointer
AllocationPolicy<T, CoreAllocatorType>::find(
  typename AllocationPolicy<T, CoreAllocatorType>::pointer p) const
{
  return reinterpret_cast<typename AllocationPolicy<T, CoreAllocatorType>::pointer>(
    m_allocator.find(reinterpret_cast<void*>(p)));
}

// -----------------------------------------------------------------------------

template<typename T, typename CoreAllocatorType>
inline
bool operator==(
  AllocationPolicy<T, CoreAllocatorType> const& lhs,
  AllocationPolicy<T, CoreAllocatorType> const& rhs)
{
  return lhs.total_size() == rhs.total_size();
}

// -----------------------------------------------------------------------------

template<
  typename T,
  typename CoreAllocatorType,
  typename U,
  typename OtherCoreAllocatorType
>
inline
bool operator==(
  AllocationPolicy<T, CoreAllocatorType> const& /* lhs */,
  AllocationPolicy<U, OtherCoreAllocatorType> const& /* rhs */)
{
  return false;
}

// -----------------------------------------------------------------------------

template<
  typename T,
  typename CoreAllocatorType,
  typename OtherAllocationPolicyType
>
inline
bool operator==(
  AllocationPolicy<T, CoreAllocatorType> const& /* lhs */,
  OtherAllocationPolicyType const& /* rhs */)
{
  return false;
}

// -----------------------------------------------------------------------------

template<typename T, typename CoreAllocatorType>
inline
bool operator!=(
  AllocationPolicy<T, CoreAllocatorType> const& lhs,
  AllocationPolicy<T, CoreAllocatorType> const& rhs)
{
  return !operator==(lhs, rhs);
}

// -----------------------------------------------------------------------------

template<
  typename T,
  typename CoreAllocatorType,
  typename U,
  typename OtherCoreAllocatorType
>
inline
bool operator!=(
  AllocationPolicy<T, CoreAllocatorType> const& lhs,
  AllocationPolicy<U, OtherCoreAllocatorType> const& rhs)
{
  return !operator==(lhs, rhs);
}

// -----------------------------------------------------------------------------

template<
  typename T,
  typename CoreAllocatorType,
  typename OtherAllocationPolicyType
>
inline
bool operator!=(
  AllocationPolicy<T, CoreAllocatorType> const& lhs,
  OtherAllocationPolicyType const& rhs)
{
  return !operator==(lhs, rhs);
}

// -----------------------------------------------------------------------------


} /* end namespace memory */


} /* end namespace corevm */


#endif /* COREVM_ALLOCATION_POLICY_H_ */
