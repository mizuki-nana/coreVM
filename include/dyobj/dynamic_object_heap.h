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
#ifndef COREVM_DYNAMIC_OBJECT_HEAP_H_
#define COREVM_DYNAMIC_OBJECT_HEAP_H_

#include <algorithm>
#include <stdexcept>
#include "common.h"
#include "dyobj_id.h"
#include "dynamic_object.h"
#include "dynamic_object_container.h"
#include "errors.h"
#include "heap_allocator.h"


namespace corevm {


namespace dyobj {


template<class dynamic_object_manager>
class dynamic_object_heap {
public:

  typedef corevm::dyobj::dyobj_id dynamic_object_id_type;
  using dynamic_object_type = typename corevm::dyobj::dynamic_object<dynamic_object_manager>;

  using dynamic_object_container_type = typename corevm::dyobj::dynamic_object_container<dynamic_object_type>;

  using allocator_type      = typename dynamic_object_container_type::allocator_type;
  using reference           = typename dynamic_object_container_type::reference;
  using const_reference     = typename dynamic_object_container_type::const_reference;
  using pointer             = typename dynamic_object_container_type::pointer;
  using const_pointer       = typename dynamic_object_container_type::const_pointer;
  using iterator            = typename dynamic_object_container_type::iterator;
  using const_iterator      = typename dynamic_object_container_type::const_iterator;
  using size_type           = typename dynamic_object_container_type::size_type;
  using difference_type     = typename dynamic_object_container_type::difference_type;

  explicit dynamic_object_heap();
  ~dynamic_object_heap();

  size_type size() const noexcept;

  size_type max_size() const noexcept;

  size_type active_size() const noexcept;

  void erase(iterator);

  void erase(dynamic_object_id_type id);

  iterator begin() noexcept;
  const_iterator cbegin() const noexcept;

  iterator end() noexcept;
  const_iterator cend() const noexcept;

  template<typename Function>
  void iterate(Function) noexcept;

  dynamic_object_type& at(const dynamic_object_id_type)
    throw(corevm::dyobj::object_not_found_error);

  dynamic_object_id_type create_dyobj()
    throw(corevm::dyobj::object_heap_insertion_failed_error);

private:
  dynamic_object_container_type m_container;
};


template<class dynamic_object_manager>
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::dynamic_object_heap()
{
  // Do nothing here.
}

template<class dynamic_object_manager>
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::~dynamic_object_heap()
{
  // Do nothing here.
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::size_type
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::size() const noexcept
{
  return m_container.size();
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::size_type
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::max_size() const noexcept
{
  return m_container.max_size();
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::size_type
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::active_size() const noexcept
{
  return std::count_if(
    cbegin(),
    cend(),
    [](const dynamic_object_type& obj) {
      return !obj.is_garbage_collectible();
    }
  );
}

template<class dynamic_object_manager>
void
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::erase(iterator pos)
{
  m_container.erase(pos);
}

template<class dynamic_object_manager>
void
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::erase(dynamic_object_id_type id)
{
  auto itr = m_container.find(id);

  if(itr != m_container.end()) {
    erase(itr);
  }
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::iterator
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::begin() noexcept
{
  return m_container.begin();
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::const_iterator
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::cbegin() const noexcept
{
  return m_container.cbegin();
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::iterator
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::end() noexcept
{
  return m_container.end();
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::const_iterator
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::cend() const noexcept
{
  return m_container.cend();
}

template<class dynamic_object_manager>
template<typename Function>
void
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::iterate(Function func) noexcept
{
  std::for_each(
    begin(),
    end(),
    [&func](dynamic_object_type& obj) {
      func(obj.id(), obj);
    }
  );
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::dynamic_object_type&
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::at(
  const corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::dynamic_object_id_type id)
  throw(corevm::dyobj::object_not_found_error)
{
  void* raw_ptr = corevm::dyobj::obj_id_to_ptr(id);
  dynamic_object_type* ptr = static_cast<dynamic_object_type*>(raw_ptr);

  ptr = m_container[ptr];

  if(ptr == nullptr) {
    throw corevm::dyobj::object_not_found_error(id);
  }

  return *ptr;
}

template<class dynamic_object_manager>
typename corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::dynamic_object_id_type
corevm::dyobj::dynamic_object_heap<dynamic_object_manager>::create_dyobj()
  throw(corevm::dyobj::object_heap_insertion_failed_error)
{
  auto obj_ptr = m_container.create();

  if (obj_ptr == nullptr) {
    throw corevm::dyobj::object_heap_insertion_failed_error();
  }

  auto id = corevm::dyobj::obj_ptr_to_id(obj_ptr);

  obj_ptr->set_id(id);

  return id;
}


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYNAMIC_OBJECT_HEAP_H_ */
