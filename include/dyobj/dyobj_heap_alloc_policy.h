#ifndef COREVM_DYOBJ_HEAP_ALLOC_POLICY_H_
#define COREVM_DYOBJ_HEAP_ALLOC_POLICY_H_

#include <sneaker/allocator/alloc_policy.h>
#include "../memory/heap_mem_allocator.h"
#include "../memory/sequential_allocation_scheme.h"
#include "common.h"


namespace corevm {


namespace dyobj {


template<typename T, size_t N=COREVM_DEFAULT_HEAP_SIZE>
class dyobj_heap_alloc_policy : public sneaker::allocator::standard_alloc_policy<T> {
public:
  using pointer = typename sneaker::allocator::standard_alloc_policy<T>::pointer;
  using const_pointer = typename sneaker::allocator::standard_alloc_policy<T>::const_pointer;
  using reference = typename sneaker::allocator::standard_alloc_policy<T>::reference;
  using const_reference = typename sneaker::allocator::standard_alloc_policy<T>::const_reference;
  using size_type = typename sneaker::allocator::standard_alloc_policy<T>::size_type;
  using difference_type = typename sneaker::allocator::standard_alloc_policy<T>::difference_type;
  using propagate_on_container_move_assignment = typename sneaker::allocator::standard_alloc_policy<T>::propagate_on_container_move_assignment;

  inline explicit dyobj_heap_alloc_policy();
  inline explicit dyobj_heap_alloc_policy(dyobj_heap_alloc_policy const&);
  inline ~dyobj_heap_alloc_policy();

  inline virtual pointer allocate(size_type, typename std::allocator<void>::const_pointer=0);
  inline virtual void deallocate(pointer, size_type);

protected:
  corevm::memory::heap_mem_allocator<
    N,
    corevm::memory::buddy_allocation_scheme
  > _heap;
};


template<typename T, size_t N>
using _MyType = typename corevm::dyobj::dyobj_heap_alloc_policy<T, N>;


template<typename T, size_t N>
corevm::dyobj::dyobj_heap_alloc_policy<T, N>::dyobj_heap_alloc_policy():
  sneaker::allocator::standard_alloc_policy<T>(),
  _heap(
    corevm::memory::heap_mem_allocator<N, corevm::memory::buddy_allocation_scheme>()
  )
{
  // Do nothing here.
}

template<typename T, size_t N>
corevm::dyobj::dyobj_heap_alloc_policy<T, N>::dyobj_heap_alloc_policy(
  dyobj_heap_alloc_policy const&)
{
  // Do nothing here.
}

template<typename T, size_t N>
corevm::dyobj::dyobj_heap_alloc_policy<T, N>::~dyobj_heap_alloc_policy()
{
  // Do nothing here.
}

template<typename T, size_t N>
typename _MyType<T, N>::pointer
corevm::dyobj::dyobj_heap_alloc_policy<T, N>::allocate(
  typename dyobj_heap_alloc_policy<T, N>::size_type n,
  typename std::allocator<void>::const_pointer
)
{
  return reinterpret_cast<_MyType<T, N>::pointer>( _heap.malloc_mem(n * sizeof(T)) );
}

template<typename T, size_t N>
void
corevm::dyobj::dyobj_heap_alloc_policy<T, N>::deallocate(
  typename dyobj_heap_alloc_policy<T, N>::pointer p,
  typename dyobj_heap_alloc_policy<T, N>::size_type
)
{
  _heap.free_mem(p);
}


template<typename T, typename U>
inline
bool operator==(dyobj_heap_alloc_policy<T> const& lhs, dyobj_heap_alloc_policy<U> const& rhs)
{
  return true;
}

template<typename T, typename other_allocator_policy_type>
inline
bool operator==(dyobj_heap_alloc_policy<T> const& lhs, other_allocator_policy_type const& rhs)
{
  return false;
}

template<typename T, typename U>
inline
bool operator!=(dyobj_heap_alloc_policy<T> const& lhs, dyobj_heap_alloc_policy<U> const& rhs)
{
  return !operator==(lhs, rhs);
}

template<typename T, typename other_allocator_policy_type>
inline
bool operator!=(dyobj_heap_alloc_policy<T> const& lhs, other_allocator_policy_type const& rhs)
{
  return !operator==(lhs, rhs);
}


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYOBJ_HEAP_ALLOC_POLICY_H_ */