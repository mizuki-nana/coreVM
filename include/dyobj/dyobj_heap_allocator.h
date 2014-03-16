#ifndef COREVM_DYOBJ_HEAP_ALLOCATOR_H_
#define COREVM_DYOBJ_HEAP_ALLOCATOR_H_

#include <sneaker/allocator/allocator.h>
#include "common.h"
#include "dyobj_heap_alloc_policy.h"


namespace corevm {


namespace dyobj {


template<typename T, size_t N=COREVM_DEFAULT_HEAP_SIZE>
class dyobj_heap_allocator : public sneaker::allocator::allocator<T, corevm::dyobj::dyobj_heap_alloc_policy<T, N>> {
public:
  using _BaseType = typename sneaker::allocator::allocator<T, corevm::dyobj::dyobj_heap_alloc_policy<T, N> >;

  using value_type      = typename _BaseType::value_type;
  using pointer         = typename _BaseType::pointer;
  using const_pointer   = typename _BaseType::const_pointer;
  using reference       = typename _BaseType::reference;
  using const_reference = typename _BaseType::const_reference;
  using size_type       = typename _BaseType::size_type;
  using difference_type = typename _BaseType::difference_type;

  inline explicit dyobj_heap_allocator();
  inline ~dyobj_heap_allocator();

  inline dyobj_heap_allocator(dyobj_heap_allocator const&);

  template<typename U>
  inline dyobj_heap_allocator(dyobj_heap_allocator<U> const&);

  template<typename U, size_t M>
  inline dyobj_heap_allocator(dyobj_heap_allocator<U, M> const&);
};


template<typename T, size_t N>
corevm::dyobj::dyobj_heap_allocator<T, N>::dyobj_heap_allocator()
{
  // Do nothing here.
}

template<typename T, size_t N>
corevm::dyobj::dyobj_heap_allocator<T, N>::~dyobj_heap_allocator()
{
  // Do nothing here.
}

template<typename T, size_t N>
corevm::dyobj::dyobj_heap_allocator<T, N>::dyobj_heap_allocator(dyobj_heap_allocator<T, N> const&)
{
  // Do nothing here.
}

template<typename T, size_t N>
template<typename U, size_t M>
corevm::dyobj::dyobj_heap_allocator<T, N>::dyobj_heap_allocator(dyobj_heap_allocator<U, M> const&)
{
  // Do nothing here.
}


template<typename T, size_t N>
inline
bool operator==(dyobj_heap_allocator<T, N> const& lhs, dyobj_heap_allocator<T, N> const& rhs)
{
  return operator==(
    static_cast<corevm::dyobj::dyobj_heap_alloc_policy<T, N>>(lhs),
    static_cast<corevm::dyobj::dyobj_heap_alloc_policy<T, N>>(rhs)
  );
}

template<typename T, size_t N, typename U, size_t M>
inline
bool operator==(dyobj_heap_allocator<T, N> const& lhs, dyobj_heap_allocator<U, M> const& rhs)
{
  return operator==(
    static_cast<corevm::dyobj::dyobj_heap_alloc_policy<T, N>>(lhs),
    static_cast<corevm::dyobj::dyobj_heap_alloc_policy<U, M>>(rhs)
  );
}

template<typename T, size_t N, typename other_allocator_type>
inline
bool operator==(dyobj_heap_allocator<T, N> const& lhs, other_allocator_type const& rhs)
{
  return operator==(
    static_cast<corevm::dyobj::dyobj_heap_alloc_policy<T, N>>(lhs),
    rhs
  );
}

template<typename T, size_t N>
inline
bool operator!=(dyobj_heap_allocator<T, N> const& lhs, dyobj_heap_allocator<T, N> const& rhs)
{
  return !operator==(lhs, rhs);
}

template<typename T, size_t N, typename U, size_t M>
inline
bool operator!=(dyobj_heap_allocator<T, N> const& lhs, dyobj_heap_allocator<U, M> const& rhs)
{
  return !operator==(lhs, rhs);
}

template<typename T, size_t N, typename other_allocator_type>
inline
bool operator!=(dyobj_heap_allocator<T, N> const& lhs, other_allocator_type const& rhs)
{
  return !operator==(lhs, rhs);
}


}; /* end namespace dyobj */


}; /* end namespace corevm */


#endif /* COREVM_DYOBJ_HEAP_ALLOCATOR_H_ */