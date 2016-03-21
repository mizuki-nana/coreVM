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
#include "corevm/macros.h"

#include <cstdint>
#include <ostream>
#include <unordered_set>


#define PTR_TO_INT(p) (uint8_t*)( (p) ) - (uint8_t*)(NULL)


namespace corevm {


namespace memory {


/**
 * This is a generic container class responsible for storing and managing
 * objects allocated by an allocator. It creates instances of objects directly
 * from memory allocated by the allocator, as well as destroying instances and
 * deallocates their memory.
 *
 * In addition, it provides a set of iterator interfaces for the outside world
 * to iterate through all the contained objects.
 */
template<typename T, typename AllocatorType>
class object_container
{
public:

  typedef typename AllocatorType::value_type value_type;
  typedef typename AllocatorType::pointer pointer;
  typedef typename AllocatorType::const_pointer const_pointer;
  typedef typename AllocatorType::reference reference;
  typedef typename AllocatorType::const_reference const_reference;
  typedef typename AllocatorType::difference_type difference_type;
  typedef typename AllocatorType::size_type size_type;

  typedef typename AllocatorType::iterator iterator;
  typedef typename AllocatorType::const_iterator const_iterator;

  object_container();

  explicit object_container(uint64_t);

  /* Object containers should not be copyable. */
  object_container(const object_container&) = delete;
  object_container& operator=(const object_container&) = delete;

  iterator begin();
  iterator end();

  const_iterator cbegin() const;
  const_iterator cend() const;

  size_type size() const;

  size_type max_size() const;

  size_type total_size() const;

  pointer create();

  pointer create(size_t);

  pointer create(const_reference);

  pointer operator[](pointer);
  const_pointer operator[](const_pointer) const;

  pointer at(pointer) throw(corevm::memory::invalid_address_error);
  const_pointer at(const_pointer) const throw(corevm::memory::invalid_address_error);

  void destroy(pointer);

  void erase(iterator&);

private:
  using _HashSet = typename std::unordered_set<pointer>;

  bool check_ptr(pointer) const;

  AllocatorType m_allocator;
  _HashSet m_addrs;
};

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
object_container<T, AllocatorType>::object_container()
  :
  m_allocator()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
object_container<T, AllocatorType>::object_container(
  uint64_t total_size)
  :
  m_allocator(total_size)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
typename object_container<T, AllocatorType>::iterator
object_container<T, AllocatorType>::begin()
{
  return m_allocator.begin();
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
typename object_container<T, AllocatorType>::iterator
object_container<T, AllocatorType>::end()
{
  return m_allocator.end();
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
typename object_container<T, AllocatorType>::const_iterator
object_container<T, AllocatorType>::cbegin() const
{
  return m_allocator.cbegin();
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
typename object_container<T, AllocatorType>::const_iterator
object_container<T, AllocatorType>::cend() const
{
  return m_allocator.cend();
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
typename object_container<T, AllocatorType>::size_type
object_container<T, AllocatorType>::size() const
{
  return m_addrs.size();
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
typename object_container<T, AllocatorType>::size_type
object_container<T, AllocatorType>::max_size() const
{
  return m_allocator.max_size();
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
typename object_container<T, AllocatorType>::size_type
object_container<T, AllocatorType>::total_size() const
{
  return m_allocator.total_size();
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
typename object_container<T, AllocatorType>::pointer
object_container<T, AllocatorType>::create()
{
  pointer p = m_allocator.allocate(1, 0);

  if (!p)
  {
    return nullptr;
  }

  m_allocator.construct(p);

  m_addrs.insert(p);

  return p;
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
typename object_container<T, AllocatorType>::pointer
object_container<T, AllocatorType>::create(size_t n)
{
  pointer p = m_allocator.allocate(n, 0);

  if (!p)
  {
    return nullptr;
  }

  for (size_t i = 0; i < n; ++i)
  {
    m_allocator.construct(&p[i]);

    m_addrs.insert(&p[i]);
  }

  return p;
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
typename object_container<T, AllocatorType>::pointer
object_container<T, AllocatorType>::create(const_reference value)
{
  pointer p = m_allocator.allocate(1, 0);

  if (!p)
  {
    return nullptr;
  }

  m_allocator.construct(p, value);

  m_addrs.insert(p);

  return p;
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
bool
object_container<T, AllocatorType>::check_ptr(pointer p) const
{
  return m_addrs.find(p) != m_addrs.end();
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
typename object_container<T, AllocatorType>::pointer
object_container<T, AllocatorType>::operator[](pointer p)
{
  if (!check_ptr(p))
  {
    return nullptr;
  }

  return static_cast<pointer>((void*)p);
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
typename object_container<T, AllocatorType>::const_pointer
object_container<T, AllocatorType>::operator[](const_pointer p) const
{
  if (!check_ptr(const_cast<pointer>(p)))
  {
    return nullptr;
  }

  return static_cast<const_pointer>((void*)p);
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
typename object_container<T, AllocatorType>::pointer
object_container<T, AllocatorType>::at(pointer p)
  throw(corevm::memory::invalid_address_error)
{
  if (!check_ptr(p))
  {
    THROW(invalid_address_error(static_cast<uint64_t>(PTR_TO_INT(p))));
  }

  return static_cast<pointer>((void*)p);
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
typename object_container<T, AllocatorType>::const_pointer
object_container<T, AllocatorType>::at(const_pointer p) const
  throw(corevm::memory::invalid_address_error)
{
  if (!check_ptr(const_cast<pointer>(p)))
  {
    THROW(invalid_address_error(PTR_TO_INT(p)));
  }

  return static_cast<const_pointer>((void*)p);
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
void
object_container<T, AllocatorType>::destroy(pointer p)
{
  if (!check_ptr(p))
  {
    THROW(invalid_address_error(static_cast<uint64_t>(PTR_TO_INT(p))));
  }

  m_allocator.destroy(p);
  m_allocator.deallocate(p, 1);
  m_addrs.erase(p);
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
void
object_container<T, AllocatorType>::erase(iterator& itr)
{
  if (itr == end())
  {
    return;
  }

  pointer p = static_cast<pointer>(&(*itr));
  destroy(p);
}

// -----------------------------------------------------------------------------

template<typename T, typename AllocatorType>
std::ostream&
operator<<(
  std::ostream& ost,
  const object_container<T, AllocatorType>& container)
{
  ost << "object container: ";
  ost << container.size() << "/" << container.max_size();
  ost << " (" << container.total_size() << " bytes)";
  ost << std::endl << std::endl;

  ost << "-- BEGIN --" << std::endl;
  for (auto itr = container.cbegin(); itr != container.cend(); ++itr)
  {
    const T* t = itr.operator->();
    ost << t << std::endl;
  }

  ost << "-- END --" << std::endl;

  return ost;
}

// -----------------------------------------------------------------------------


} /* end namespace memory */


} /* end namespace corevm */


#endif /* COREVM_OBJECT_CONTAINER */
