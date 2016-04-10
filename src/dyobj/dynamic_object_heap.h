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
#ifndef COREVM_DYNAMIC_OBJECT_HEAP_H_
#define COREVM_DYNAMIC_OBJECT_HEAP_H_

#include "corevm/macros.h"
#include "dyobj/common.h"
#include "dyobj/dynamic_object.h"
#include "dyobj/errors.h"
#include "dyobj/heap_allocator.h"
#include "memory/block_allocator.h"
#include "memory/errors.h"
#include "memory/object_container.h"

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <type_traits>


namespace corevm {


namespace dyobj {


template<class DynamicObjectManager>
class DynamicObjectHeap
{
public:

  typedef dyobj_id_t dynamic_object_id_type;
  using dynamic_object_type = typename dyobj::DynamicObject<DynamicObjectManager>;
  using allocator_type = typename dyobj::HeapAllocator<dynamic_object_type, corevm::memory::BlockAllocator<dynamic_object_type>>;
  using dynamic_object_container_type = typename corevm::memory::ObjectContainer<dynamic_object_type, allocator_type>;

  static_assert(
    std::numeric_limits<typename dynamic_object_container_type::size_type>::max() >=
    std::numeric_limits<dyobj_id_t>::max(),
    "Dynamic object heap incompatibility"
  );

  using reference           = typename dynamic_object_container_type::reference;
  using const_reference     = typename dynamic_object_container_type::const_reference;
  using pointer             = typename dynamic_object_container_type::pointer;
  using const_pointer       = typename dynamic_object_container_type::const_pointer;
  using iterator            = typename dynamic_object_container_type::iterator;
  using const_iterator      = typename dynamic_object_container_type::const_iterator;
  using size_type           = typename dynamic_object_container_type::size_type;
  using difference_type     = typename dynamic_object_container_type::difference_type;

  DynamicObjectHeap();
  explicit DynamicObjectHeap(uint64_t);
  ~DynamicObjectHeap();

  /* Dynamic object heap should not be copyable. */
  DynamicObjectHeap(const DynamicObjectHeap&) = delete;
  DynamicObjectHeap& operator=(const DynamicObjectHeap&) = delete;

  size_type size() const noexcept;

  size_type max_size() const noexcept;

  size_type total_size() const noexcept;

  size_type active_size() const noexcept;

  void erase(iterator);

  void erase(dynamic_object_type*);

  iterator begin() noexcept;
  const_iterator cbegin() const noexcept;

  iterator end() noexcept;
  const_iterator cend() const noexcept;

  template<typename Function>
  void iterate(Function) noexcept;

  dynamic_object_type& at(const dynamic_object_id_type);

  dynamic_object_type* create_dyobj();

  dynamic_object_type* create_dyobjs(size_t n);

private:
  dynamic_object_container_type m_container;
};

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
DynamicObjectHeap<DynamicObjectManager>::DynamicObjectHeap()
  :
  m_container(COREVM_DEFAULT_HEAP_SIZE)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
DynamicObjectHeap<DynamicObjectManager>::DynamicObjectHeap(
  uint64_t total_size)
  :
  m_container(total_size)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
DynamicObjectHeap<DynamicObjectManager>::~DynamicObjectHeap()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
typename DynamicObjectHeap<DynamicObjectManager>::size_type
DynamicObjectHeap<DynamicObjectManager>::size() const noexcept
{
  return m_container.size();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
typename DynamicObjectHeap<DynamicObjectManager>::size_type
DynamicObjectHeap<DynamicObjectManager>::max_size() const noexcept
{
  return m_container.max_size();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
typename DynamicObjectHeap<DynamicObjectManager>::size_type
DynamicObjectHeap<DynamicObjectManager>::total_size() const noexcept
{
  return m_container.total_size();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
typename DynamicObjectHeap<DynamicObjectManager>::size_type
DynamicObjectHeap<DynamicObjectManager>::active_size() const noexcept
{
  typedef typename DynamicObjectHeap<DynamicObjectManager>::size_type active_size_type;

  return static_cast<active_size_type>(std::count_if(cbegin(), cend(),
    [](const dynamic_object_type& obj) {
      return !obj.is_garbage_collectible();
    }));
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
void
DynamicObjectHeap<DynamicObjectManager>::erase(iterator pos)
{
  m_container.erase(pos);
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
void
DynamicObjectHeap<DynamicObjectManager>::erase(dynamic_object_type* ptr)
{
  ptr = m_container.at(ptr);

  m_container.destroy(ptr);
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
typename DynamicObjectHeap<DynamicObjectManager>::iterator
DynamicObjectHeap<DynamicObjectManager>::begin() noexcept
{
  return m_container.begin();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
typename DynamicObjectHeap<DynamicObjectManager>::const_iterator
DynamicObjectHeap<DynamicObjectManager>::cbegin() const noexcept
{
  return m_container.cbegin();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
typename DynamicObjectHeap<DynamicObjectManager>::iterator
DynamicObjectHeap<DynamicObjectManager>::end() noexcept
{
  return m_container.end();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
typename DynamicObjectHeap<DynamicObjectManager>::const_iterator
DynamicObjectHeap<DynamicObjectManager>::cend() const noexcept
{
  return m_container.cend();
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
template<typename Function>
void
DynamicObjectHeap<DynamicObjectManager>::iterate(Function func) noexcept
{
  std::for_each(
    begin(),
    end(),
    [&func](dynamic_object_type& obj) {
      func(obj.id(), obj);
    }
  );
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
typename DynamicObjectHeap<DynamicObjectManager>::dynamic_object_type&
DynamicObjectHeap<DynamicObjectManager>::at(
  const DynamicObjectHeap<DynamicObjectManager>::dynamic_object_id_type id)
{
  void* raw_ptr = obj_id_to_ptr(id);
  dynamic_object_type* ptr = static_cast<dynamic_object_type*>(raw_ptr);

  ptr = m_container[ptr];

  if (ptr == nullptr)
  {
    THROW(ObjectNotFoundError(id));
  }

  return *ptr;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
typename DynamicObjectHeap<DynamicObjectManager>::dynamic_object_type*
DynamicObjectHeap<DynamicObjectManager>::create_dyobj()
{
  auto obj_ptr = m_container.create();

  if (obj_ptr == nullptr)
  {
    THROW(ObjectCreationError());
  }

  obj_ptr->manager().on_create();

  return obj_ptr;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
typename DynamicObjectHeap<DynamicObjectManager>::dynamic_object_type*
DynamicObjectHeap<DynamicObjectManager>::create_dyobjs(size_t n)
{
  auto ptr = m_container.create(n);

  if (ptr == nullptr)
  {
    THROW(ObjectCreationError());
  }

  for (size_t i = 0; i < n; ++i)
  {
    ptr[i].manager().on_create();
  }

  return ptr;
}

// -----------------------------------------------------------------------------

template<class DynamicObjectManager>
std::ostream&
operator<<(std::ostream& ost, const DynamicObjectHeap<DynamicObjectManager>& heap)
{
  using T = typename DynamicObjectHeap<DynamicObjectManager>::dynamic_object_type;

  ost << "Dynamic object heap: ";
  ost << heap.size() << "/" << heap.max_size();
  ost << " (" << heap.total_size() << " bytes)";
  ost << std::endl << std::endl;

  ost << "-- BEGIN --" << std::endl;
  for (auto itr = heap.cbegin(); itr != heap.cend(); ++itr)
  {
    const T* t = itr.operator->();
    ost << t << "  ";
    ost << "id[" << std::setw(4) << std::hex << std::showbase << t->id() << std::noshowbase << std::dec << "] ";
    ost << "flags[" << std::setw(4) << std::hex << std::showbase << t->flags() << std::noshowbase << std::dec << "] ";
    ost << "attrs[" << std::setw(4) << t->attr_count() << "] ";
    ost << "garbage-collectible[" << std::setw(1) << t->is_garbage_collectible() << "] ";
    ost << "ntvhndl[" << std::setw(4) << std::hex << std::showbase << t->ntvhndl_key() << std::noshowbase << std::dec << "] ";
    ost << "closure[" << std::setw(8) << t->closure_ctx().compartment_id << ":" << t->closure_ctx().closure_id << "] ";
    ost << std::endl;
  }

  ost << "-- END --" << std::endl;

  return ost;
}

// -----------------------------------------------------------------------------


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYNAMIC_OBJECT_HEAP_H_ */
