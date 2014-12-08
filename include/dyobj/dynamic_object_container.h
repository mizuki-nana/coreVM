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
#ifndef COREVM_DYNAMIC_OBJECT_CONTAINER_H_
#define COREVM_DYNAMIC_OBJECT_CONTAINER_H_

#include <cassert>
#include <iterator>
#include <set>
#include "errors.h"
#include "heap_allocator.h"


namespace corevm {


namespace dyobj {


/*
 * This object is a container responsible for storing and managing dynamic
 * objects on the heap. It creates instances of dynamic objects directly from
 * memory allocated by the allocator, as well as destroying instances and
 * deallocates their memory.
 *
 * In addition, it provides a set of iterator interfaces for the outside world
 * to iterate through all the contained objects. To do so, it uses an instance
 * of `std::set` to store the addresses of the allocated objects. This way,
 * its iterator types are merely wrappers around the iterator types of the
 * address map, thus avoid the complexity associated with implementing custom
 * iterator types.
 */
template<typename T>
class dynamic_object_container {
public:

  using allocator_type = typename corevm::dyobj::heap_allocator<T>;

  typedef typename allocator_type::value_type value_type;
  typedef typename allocator_type::pointer pointer;
  typedef typename allocator_type::const_pointer const_pointer;
  typedef typename allocator_type::reference reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::difference_type difference_type;
  typedef typename allocator_type::size_type size_type;

private:
  using _HashSet = typename std::set<pointer>;

public:
  class iterator : public std::iterator<std::forward_iterator_tag, T> {
    private:
      using _ContainerType = dynamic_object_container<T>;
      typedef typename _HashSet::iterator _Inner;

    public:
      typedef typename allocator_type::value_type value_type;
      typedef typename allocator_type::reference reference;
      typedef typename allocator_type::const_reference const_reference;
      typedef typename allocator_type::difference_type difference_type;
      typedef typename allocator_type::size_type size_type;

      iterator(_ContainerType& container, _Inner inner):
        m_container(&container),
        m_inner(inner)
      {
      }

      iterator(const iterator& rhs) {
        m_container = rhs.m_container;
        m_inner = rhs.m_inner;
      }

      iterator& operator=(const iterator& rhs) {
        m_container = rhs.m_container;
        m_inner = rhs.m_inner;
        return *this;
      }

      bool operator==(const iterator& rhs) const {
        return m_inner == rhs.m_inner;
      }

      bool operator!=(const iterator& rhs) const {
        return m_inner != rhs.m_inner;
      }

      iterator& operator++() {
        m_inner++;
        return *this;
      }

      iterator operator++(int) {
        iterator tmp(*this);
        operator++();
        return tmp;
      }

      reference operator*() const {
        return *( (*m_container)[*m_inner] );
      }

      pointer operator->() const {
        return ( (*m_container)[*m_inner] );
      }

    private:
      _ContainerType* m_container;
      _Inner m_inner;

      friend class corevm::dyobj::dynamic_object_container<T>;
  };

  class const_iterator : public std::iterator<std::forward_iterator_tag, T> {
    private:
      using _ContainerType = const dynamic_object_container<T>;
      typedef typename _HashSet::const_iterator _Inner;

    public:
      typedef typename allocator_type::value_type value_type;
      typedef typename allocator_type::reference reference;
      typedef typename allocator_type::const_reference const_reference;
      typedef typename allocator_type::difference_type difference_type;
      typedef typename allocator_type::size_type size_type;

      const_iterator(_ContainerType& container, _Inner inner):
        m_container(&container),
        m_inner(inner)
      {
      }

      const_iterator(const const_iterator& rhs) {
        m_container = rhs.m_container;
        m_inner = rhs.m_inner;
      }

      const_iterator& operator=(const const_iterator& rhs) {
        m_container = rhs.m_container;
        m_inner = rhs.m_inner;
        return *this;
      }

      bool operator==(const const_iterator& rhs) {
        return m_inner == rhs.m_inner;
      }

      bool operator!=(const const_iterator& rhs) {
        return m_inner != rhs.m_inner;
      }

      const_iterator& operator++() {
        m_inner++;
        return *this;
      }

      const_iterator operator++(int) {
        const_iterator tmp(*this);
        operator++();
        return tmp;
      }

      const_reference operator*() const {
        return *( (*m_container)[*m_inner] );
      }

      const_pointer operator->() const {
        return ( (*m_container)[*m_inner] );
      }

    private:
      const _ContainerType* m_container;
      _Inner m_inner;

      friend class corevm::dyobj::dynamic_object_container<T>;
  };

  explicit dynamic_object_container();

  iterator begin();
  iterator end();

  const_iterator cbegin() const;
  const_iterator cend() const;

  size_type size() const;

  pointer create();

  pointer operator[](pointer);
  const pointer operator[](pointer) const;

  void destroy(pointer);

  void erase(iterator&);

  void erase(const_iterator&);

private:
  void check_ptr(pointer) const throw(corevm::dyobj::invalid_address_error);

  allocator_type m_allocator;
  _HashSet m_addrs;
};


template<typename T>
corevm::dyobj::dynamic_object_container<T>::dynamic_object_container()
{
  // Do nothing here.
}

template<typename T>
typename corevm::dyobj::dynamic_object_container<T>::iterator
corevm::dyobj::dynamic_object_container<T>::begin()
{
  return iterator(*this, m_addrs.begin());
}

template<typename T>
typename corevm::dyobj::dynamic_object_container<T>::iterator
corevm::dyobj::dynamic_object_container<T>::end()
{
  return iterator(*this, m_addrs.end());
}

template<typename T>
typename corevm::dyobj::dynamic_object_container<T>::const_iterator
corevm::dyobj::dynamic_object_container<T>::cbegin() const
{
  return const_iterator(*this, m_addrs.cbegin());
}

template<typename T>
typename corevm::dyobj::dynamic_object_container<T>::const_iterator
corevm::dyobj::dynamic_object_container<T>::cend() const
{
  return const_iterator(*this, m_addrs.cend());
}

template<typename T>
typename corevm::dyobj::dynamic_object_container<T>::size_type
corevm::dyobj::dynamic_object_container<T>::size() const
{
  return m_addrs.size();
}

template<typename T>
typename corevm::dyobj::dynamic_object_container<T>::pointer
corevm::dyobj::dynamic_object_container<T>::create()
{
  pointer p = m_allocator.allocate(1, 0);

  T t;
  m_allocator.construct(p, t);

  m_addrs.insert(p);

  return p;
}

template<typename T>
void
corevm::dyobj::dynamic_object_container<T>::check_ptr(pointer p) const
  throw(corevm::dyobj::invalid_address_error)
{
  assert(p);

  uint64_t base_addr = m_allocator.base_addr();
  uint64_t total_size = m_allocator.total_size();

  uint64_t addr = static_cast<uint64_t>( (char*)p - (char*)(0) );

  if(
    (addr < base_addr || addr >= base_addr + total_size) ||
    (m_addrs.find(p) == m_addrs.end())
  )
  {
    throw corevm::dyobj::invalid_address_error(addr);
  }
}

template<typename T>
typename corevm::dyobj::dynamic_object_container<T>::pointer
corevm::dyobj::dynamic_object_container<T>::operator[](pointer p)
{
  check_ptr(p);
  return static_cast<pointer>((void*)p);
}

template<typename T>
const typename corevm::dyobj::dynamic_object_container<T>::pointer
corevm::dyobj::dynamic_object_container<T>::operator[](pointer p) const
{
  check_ptr(p);
  return static_cast<const pointer>((void*)p);
}

template<typename T>
void
corevm::dyobj::dynamic_object_container<T>::destroy(pointer p)
{
  check_ptr(p);
  m_allocator.destroy(p);
  m_allocator.deallocate(p, 1);

  m_addrs.erase(p);
}

template<typename T>
void
corevm::dyobj::dynamic_object_container<T>::erase(iterator& itr)
{
  if(itr == end()) {
    return;
  }

  pointer p = static_cast<pointer>(*itr.m_inner);
  destroy(p);
}

template<typename T>
void
corevm::dyobj::dynamic_object_container<T>::erase(const_iterator& itr)
{
  if(itr == cend()) {
    return;
  }

  pointer p = static_cast<pointer>(*itr.m_inner);
  destroy(p);
}


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYNAMIC_OBJECT_CONTAINER */
