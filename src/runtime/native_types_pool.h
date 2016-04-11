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
#ifndef COREVM_NATIVE_TYPES_POOL_H_
#define COREVM_NATIVE_TYPES_POOL_H_

#include "common.h"
#include "errors.h"
#include "dyobj/common.h"
#include "memory/allocator.h"
#include "memory/allocation_policy.h"
#include "memory/block_allocator.h"
#include "memory/object_container.h"
#include "types/fwd.h"

#include <sneaker/allocator/object_traits.h>

#include <cstdint>
#include <iosfwd>
#include <string>
#include <type_traits>


namespace corevm {


namespace runtime {


using sneaker::allocator::object_traits;


class NativeTypesPool
{
public:
  typedef types::NativeTypeHandle value_type;

  template<typename T>
  class allocator : public memory::AllocationPolicy<T, memory::BlockAllocator<value_type>>, public object_traits<T>
  {
    private:
      using AllocationPolicyType = typename memory::AllocationPolicy<T, memory::BlockAllocator<value_type>>;

    public:
      using value_type      = typename AllocationPolicyType::value_type;
      using pointer         = typename AllocationPolicyType::pointer;
      using const_pointer   = typename AllocationPolicyType::const_pointer;
      using reference       = typename AllocationPolicyType::reference;
      using const_reference = typename AllocationPolicyType::const_reference;
      using size_type       = typename AllocationPolicyType::size_type;
      using difference_type = typename AllocationPolicyType::difference_type;

      explicit allocator(uint64_t total_size)
        :
        AllocationPolicyType(total_size)
      {
      }
  };

  typedef allocator<value_type> allocator_type;

  using container_type = typename memory::ObjectContainer<types::NativeTypeHandle, allocator_type>;

  typedef value_type& reference;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;

  using size_type = typename container_type::size_type;

  NativeTypesPool();
  explicit NativeTypesPool(uint64_t);

  /* Native type pools should not be copyable. */
  NativeTypesPool(const NativeTypesPool&) = delete;
  NativeTypesPool& operator=(const NativeTypesPool&) = delete;

  size_type size() const;

  size_type max_size() const;

  size_type total_size() const;

  reference at(const_pointer);

  types::NativeTypeHandle* create();

  types::NativeTypeHandle* create(const types::NativeTypeHandle& hndl);

  void erase(pointer);

  friend std::ostream& operator<<(std::ostream&, const NativeTypesPool&);

private:
  container_type m_container;
};

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_NATIVE_TYPES_POOL_H_ */
