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
#ifndef COREVM_NATIVE_TYPES_POOL_H_
#define COREVM_NATIVE_TYPES_POOL_H_

#include <string> // TODO: [COREVM-90] Remove extraneous header in native_types_pool.h
#include <limits> // TODO: [COREVM-90] Remove extraneous header in native_types_pool.h
#include <sneaker/allocator/alloc_policy.h>
#include <sneaker/allocator/allocator.h>
#include "../dyobj/common.h"
#include "../memory/allocator.h"
#include "../memory/object_container.h"
#include "../memory/sequential_allocation_scheme.h"
#include "../types/native_type_handle.h"
#include "common.h"
#include "errors.h"


namespace corevm {


namespace runtime {


// TODO: [COREVM-91] Combine dynamic object heap and native types pool allocator types
template<typename T, size_t N>
class alloc_policy : public sneaker::allocator::standard_alloc_policy<T> {
public:
  using value_type                             = typename sneaker::allocator::standard_alloc_policy<T>::value_type;
  using pointer                                = typename sneaker::allocator::standard_alloc_policy<T>::pointer;
  using const_pointer                          = typename sneaker::allocator::standard_alloc_policy<T>::const_pointer;
  using reference                              = typename sneaker::allocator::standard_alloc_policy<T>::reference;
  using const_reference                        = typename sneaker::allocator::standard_alloc_policy<T>::const_reference;
  using size_type                              = typename sneaker::allocator::standard_alloc_policy<T>::size_type;
  using difference_type                        = typename sneaker::allocator::standard_alloc_policy<T>::difference_type;
  using propagate_on_container_move_assignment = typename sneaker::allocator::standard_alloc_policy<T>::propagate_on_container_move_assignment;

  inline virtual pointer allocate(size_type n, typename std::allocator<void>::const_pointer=0)
  {
    return reinterpret_cast<pointer>( m_allocator.allocate(n * sizeof(T)) );
  }

  inline virtual void deallocate(pointer p, size_type)
  {
    m_allocator.deallocate(p);
  }

private:
  corevm::memory::allocator<N, corevm::memory::first_fit_allocation_scheme> m_allocator;
};


// TODO: [COREVM-91] Combine dynamic object heap and native types pool allocator types
template<typename T, size_t N>
class allocator : public sneaker::allocator::allocator<T, alloc_policy<T, N>> {
public:
  using _BaseType = typename sneaker::allocator::allocator<T, alloc_policy<T, N> >;

  using value_type      = typename _BaseType::value_type;
  using pointer         = typename _BaseType::pointer;
  using const_pointer   = typename _BaseType::const_pointer;
  using reference       = typename _BaseType::reference;
  using const_reference = typename _BaseType::const_reference;
  using size_type       = typename _BaseType::size_type;
  using difference_type = typename _BaseType::difference_type;
};


class native_types_pool {
public:
  typedef corevm::types::native_type_handle value_type;

  typedef allocator<value_type, COREVM_DEFAULT_NATIVE_TYPES_POOL_SIZE> allocator_type;
  using container_type = typename corevm::memory::object_container<corevm::types::native_type_handle, allocator_type>;

  typedef value_type& reference;
  typedef value_type* pointer;

  using size_type = typename container_type::size_type;

  size_type size() const;

  size_type max_size() const;

  reference at(const corevm::dyobj::ntvhndl_key&)
    throw(corevm::runtime::native_type_handle_not_found_error);

  corevm::dyobj::ntvhndl_key create()
    throw(corevm::runtime::native_type_handle_insertion_error);

  void erase(const corevm::dyobj::ntvhndl_key&)
    throw(corevm::runtime::native_type_handle_not_found_error);

private:
  container_type m_container;
};


}; /* end namespace runtime */


}; /* end namespace corevm */


#endif /* COREVM_NATIVE_TYPES_POOL_H_ */
