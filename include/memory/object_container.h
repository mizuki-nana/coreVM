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
#ifndef COREVM_OBJECT_CONTAINER_H_
#define COREVM_OBJECT_CONTAINER_H_

#include "errors.h"

#include <cstdint>
#include <iterator>
#include <set>


namespace corevm {


namespace memory {


/* This is a generic container class responsible for storing and managing
 * objects allocated by an allocator. It creates instances of objects directly
 * from memory allocated by the allocator, as well as destroying instances and
 * deallocates their memory.
 *
 * In addition, it provides a set of iterator interfaces for the outside world
 * to iterate through all the contained objects. To do so, it uses an instance
 * of `std::set` to store the addresses of the allocated objects. This way,
 * its iterator types are merely wrappers around the iterator types of the
 * address map, thus avoid the complexity associated with implementing custom
 * iterator types.
 * */
template<typename T, typename AllocatorType=std::allocator<T>>
class object_container {
public:

  typedef typename AllocatorType::value_type value_type;
  typedef typename AllocatorType::pointer pointer;
  typedef typename AllocatorType::const_pointer const_pointer;
  typedef typename AllocatorType::reference reference;
  typedef typename AllocatorType::const_reference const_reference;
  typedef typename AllocatorType::difference_type difference_type;
  typedef typename AllocatorType::size_type size_type;

private:
  using _HashSet = typename std::set<pointer>;

public:
  class iterator : public std::iterator<std::forward_iterator_tag, T> {
    private:
      using _ContainerType = object_container<T, AllocatorType>;
      typedef typename _HashSet::iterator _Inner;

    public:
      typedef typename AllocatorType::value_type value_type;
      typedef typename AllocatorType::reference reference;
      typedef typename AllocatorType::const_reference const_reference;
      typedef typename AllocatorType::difference_type difference_type;
      typedef typename AllocatorType::size_type size_type;

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

      friend class corevm::memory::object_container<T, AllocatorType>;
  };

  class const_iterator : public std::iterator<std::forward_iterator_tag, T> {
    private:
      using _ContainerType = const object_container<T, AllocatorType>;
      typedef typename _HashSet::const_iterator _Inner;

    public:
      typedef typename AllocatorType::value_type value_type;
      typedef typename AllocatorType::reference reference;
      typedef typename AllocatorType::const_reference const_reference;
      typedef typename AllocatorType::difference_type difference_type;
      typedef typename AllocatorType::size_type size_type;

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

      friend class corevm::memory::object_container<T, AllocatorType>;
  };

  explicit object_container();

  iterator begin();
  iterator end();

  const_iterator cbegin() const;
  const_iterator cend() const;

  size_type size() const;

  size_type max_size() const;

  pointer create();

  pointer operator[](pointer);
  const pointer operator[](pointer) const;

  void destroy(pointer);

  void erase(iterator&);

  void erase(const_iterator&);

private:
  void check_ptr(pointer) const throw(corevm::memory::invalid_address_error);

  AllocatorType m_allocator;
  _HashSet m_addrs;
};


template<typename T, typename AllocatorType>
corevm::memory::object_container<T, AllocatorType>::object_container()
{
  // Do nothing here.
}

template<typename T, typename AllocatorType>
typename corevm::memory::object_container<T, AllocatorType>::iterator
corevm::memory::object_container<T, AllocatorType>::begin()
{
  return iterator(*this, m_addrs.begin());
}

template<typename T, typename AllocatorType>
typename corevm::memory::object_container<T, AllocatorType>::iterator
corevm::memory::object_container<T, AllocatorType>::end()
{
  return iterator(*this, m_addrs.end());
}

template<typename T, typename AllocatorType>
typename corevm::memory::object_container<T, AllocatorType>::const_iterator
corevm::memory::object_container<T, AllocatorType>::cbegin() const
{
  return const_iterator(*this, m_addrs.cbegin());
}

template<typename T, typename AllocatorType>
typename corevm::memory::object_container<T, AllocatorType>::const_iterator
corevm::memory::object_container<T, AllocatorType>::cend() const
{
  return const_iterator(*this, m_addrs.cend());
}

template<typename T, typename AllocatorType>
typename corevm::memory::object_container<T, AllocatorType>::size_type
corevm::memory::object_container<T, AllocatorType>::size() const
{
  return m_addrs.size();
}

template<typename T, typename AllocatorType>
typename corevm::memory::object_container<T, AllocatorType>::size_type
corevm::memory::object_container<T, AllocatorType>::max_size() const
{
  return m_addrs.max_size();
}

template<typename T, typename AllocatorType>
typename corevm::memory::object_container<T, AllocatorType>::pointer
corevm::memory::object_container<T, AllocatorType>::create()
{
  pointer p = m_allocator.allocate(1, 0);

  if (!p) {
    return nullptr;
  }

  m_allocator.construct(p);

  m_addrs.insert(p);

  return p;
}

template<typename T, typename AllocatorType>
void
corevm::memory::object_container<T, AllocatorType>::check_ptr(pointer p) const
  throw(corevm::memory::invalid_address_error)
{
  uint64_t addr = static_cast<uint64_t>( (char*)p - (char*)(0) );

  if (m_addrs.find(p) == m_addrs.end()) {
    throw corevm::memory::invalid_address_error(addr);
  }
}

template<typename T, typename AllocatorType>
typename corevm::memory::object_container<T, AllocatorType>::pointer
corevm::memory::object_container<T, AllocatorType>::operator[](pointer p)
{
  check_ptr(p);
  return static_cast<pointer>((void*)p);
}

template<typename T, typename AllocatorType>
const typename corevm::memory::object_container<T, AllocatorType>::pointer
corevm::memory::object_container<T, AllocatorType>::operator[](pointer p) const
{
  check_ptr(p);
  return static_cast<const pointer>((void*)p);
}

template<typename T, typename AllocatorType>
void
corevm::memory::object_container<T, AllocatorType>::destroy(pointer p)
{
  check_ptr(p);
  m_allocator.destroy(p);
  m_allocator.deallocate(p, 1);
  m_addrs.erase(p);
}

template<typename T, typename AllocatorType>
void
corevm::memory::object_container<T, AllocatorType>::erase(iterator& itr)
{
  if (itr == end()) {
    return;
  }

  pointer p = static_cast<pointer>(*itr.m_inner);
  destroy(p);
}

template<typename T, typename AllocatorType>
void
corevm::memory::object_container<T, AllocatorType>::erase(const_iterator& itr)
{
  if (itr == cend()) {
    return;
  }

  pointer p = static_cast<pointer>(*itr.m_inner);
  destroy(p);
}


} /* end namespace memory */


} /* end namespace corevm */


#endif /* COREVM_OBJECT_CONTAINER */
