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
#ifndef COREVM_BLOCK_ALLOCATOR_H_
#define COREVM_BLOCK_ALLOCATOR_H_

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <vector>

#include "corevm/macros.h"


namespace corevm {


namespace memory {


// -----------------------------------------------------------------------------

namespace {


#if defined(__clang__) and __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wunneeded-member-function"
#endif

typedef struct FreeListDescriptor
{
  FreeListDescriptor(
    uint32_t start_index_,
    uint32_t end_index_,
    uint32_t current_index_)
    :
    start_index(start_index_),
    end_index(end_index_),
    current_index(current_index_)
  {
  }

  uint32_t start_index;
  uint32_t end_index;
  uint32_t current_index; // index points to the first free block in the list.
} FreeListDescriptor;

#if defined(__clang__) and __clang__
  #pragma clang diagnostic pop
#endif

// -----------------------------------------------------------------------------

/**
 * Determines if the given freelist is large enough to hold
 * `n` blocks.
 *
 * `n` has to be greater than 0.
 */
inline bool
is_free_list_available(const FreeListDescriptor& descriptor, size_t n)
{
  return descriptor.current_index + (n - 1) <= descriptor.end_index;
}

// -----------------------------------------------------------------------------

/**
 * Determines if the given index has been allocated in the freelist provided.
 */
inline bool
is_allocated_in_free_list(const FreeListDescriptor& descriptor, uint32_t index)
{
  return index >= descriptor.start_index && index < descriptor.current_index;
}

// -----------------------------------------------------------------------------

/**
 * Determines if the given freelist is empty.
 */
inline bool
is_empty_free_list(const FreeListDescriptor& descriptor)
{
  return descriptor.current_index == descriptor.start_index;
}

// -----------------------------------------------------------------------------

} /* end anonymous namespace */

// -----------------------------------------------------------------------------

template<class T>
class BlockAllocator
{
public:
  explicit BlockAllocator(uint64_t total_size);

  ~BlockAllocator();

  /* Allocators should not be copyable. */
  BlockAllocator(const BlockAllocator&) = delete;
  BlockAllocator& operator=(const BlockAllocator&) = delete;

  void* allocate();

  void* allocate_n(size_t);

  /**
   * Allocate `N` objects, each of `size` bytes.
   * The `size` parameter is actually not needed here, but added in order to
   * conform to the same interface as `corevm::memory::allocator`.
   */
  void* allocate_n(size_t N, size_t /* size */);

  int deallocate(void*);

  uint64_t base_addr() const noexcept;

  uint64_t total_size() const noexcept;

private:
  void combine_empty_freelists(size_t i);

  void* m_heap;
  const uint64_t m_total_size;
  size_t m_empty_lists_count;
  std::vector<FreeListDescriptor> m_free_lists;

private:

  /**
   * An implementation of forward iterator for block allocator, with
   * flattened inheritance hierarchy.
   */
  class iterator_t : public std::iterator<std::forward_iterator_tag, T>
  {
  public:
    iterator_t(BlockAllocator<T>& parent, int64_t descriptor_index,
      int64_t slot_index)
      :
      m_parent(parent),
      m_descriptor_index(descriptor_index),
      m_slot_index(slot_index)
    {
    }

    iterator_t(const iterator_t& other)
      :
      m_parent(other.m_parent),
      m_descriptor_index(other.m_descriptor_index),
      m_slot_index(other.m_slot_index)
    {
    }

    iterator_t& operator=(const iterator_t& other)
    {
      m_descriptor_index = other.m_descriptor_index;
      m_slot_index = other.m_slot_index;
      return *this;
    }

    typename std::iterator<std::forward_iterator_tag, T>::reference operator*()
    {
      const auto desp_index = static_cast<size_t>(m_descriptor_index);
      const auto start_index = m_parent.m_free_lists[desp_index].start_index;
      const uint64_t slot_index = start_index + static_cast<uint64_t>(m_slot_index);
      T* heap_ = reinterpret_cast<T*>(m_parent.m_heap);
      return heap_[slot_index];
    }

    typename std::iterator<std::forward_iterator_tag, T>::value_type operator*() const
    {
      return const_cast<iterator_t>(this)->operator*();
    }

    typename std::iterator<std::forward_iterator_tag, T>::pointer operator->()
    {
      return &(this->operator*());
    }

    friend bool operator==(const iterator_t& lhs, const iterator_t& rhs)
    {
      return &lhs.m_parent == &rhs.m_parent &&
        lhs.m_descriptor_index == rhs.m_descriptor_index &&
        lhs.m_slot_index == rhs.m_slot_index;
    }

    friend bool operator!=(const iterator_t& lhs, const iterator_t& rhs)
    {
      return !(operator==(lhs, rhs));
    }

    /** Prefix increment. */
    iterator_t& operator++()
    {
      while (*this != m_parent.end())
      {
        // Invalid case.
        if (m_descriptor_index < 0 ||
            static_cast<size_t>(m_descriptor_index) >= m_parent.m_free_lists.size())
        {
          *this = m_parent.end();
        }
        else
        {
          const auto& descriptor =
            m_parent.m_free_lists[static_cast<size_t>(m_descriptor_index)];

          if (descriptor.start_index + m_slot_index + 1 < descriptor.current_index)
          {
            ++m_slot_index;
            break;
          }
          else
          {
            ++m_descriptor_index;
            m_slot_index = 0;

            // Skip empty lists.
            while (static_cast<size_t>(m_descriptor_index) < m_parent.m_free_lists.size())
            {
              const auto& descriptor_ =
                m_parent.m_free_lists[static_cast<size_t>(m_descriptor_index)];

              if (!is_empty_free_list(descriptor_))
              {
                  break;
              }

              ++m_descriptor_index;
              m_slot_index = 0;
            }
          }
          if (static_cast<size_t>(m_descriptor_index) >= m_parent.m_free_lists.size())
          {
            *this = m_parent.end();
          }
          else
          {
            break;
          }
        }
      }

      return *this;
    }

    /** Postfix iterator. */
    iterator_t operator++(int)
    {
      auto copy(*this);
      operator++();
      return copy;
    }

  protected:
    BlockAllocator<T>& m_parent;
    int64_t m_descriptor_index;
    int64_t m_slot_index;
  };

public:
  typedef iterator_t iterator;
  typedef iterator_t const_iterator;

  iterator begin();
  iterator end();

  const_iterator cbegin() const;
  const_iterator cend() const;

  void* find(void*) const;
};

// -----------------------------------------------------------------------------

template<class T>
BlockAllocator<T>::BlockAllocator(
  uint64_t total_size)
  :
  m_heap(nullptr),
  m_total_size(total_size),
  m_empty_lists_count(0u),
  m_free_lists()
{
  void* mem = malloc(m_total_size);

  if (!mem)
  {
    throw std::bad_alloc();
  }

  m_heap = mem;

  if (m_total_size)
  {
    uint64_t total_blocks = m_total_size / sizeof(T);

    if (total_blocks)
    {
      m_free_lists.reserve(10u);
      m_free_lists.emplace_back(0u, static_cast<uint32_t>(total_blocks - 1), 0u);
      m_empty_lists_count = 1u;
    }
  }
}

// -----------------------------------------------------------------------------

template<class T>
BlockAllocator<T>::~BlockAllocator()
{
  if (m_heap)
  {
    free(m_heap);
    m_heap = nullptr;
  }

  m_free_lists.clear();
}

// -----------------------------------------------------------------------------

template<class T>
void*
BlockAllocator<T>::allocate()
{
  return allocate_n(1);
}

// -----------------------------------------------------------------------------

template<class T>
void*
BlockAllocator<T>::allocate_n(size_t n)
{
  void* ptr = nullptr;

  if (n == 0)
  {
    return ptr;
  }

  for (size_t i = 0; i < m_free_lists.size(); ++i)
  {
    FreeListDescriptor& descriptor = m_free_lists[i];

    if (is_free_list_available(descriptor, n))
    {
#if __DEBUG__
      ASSERT(descriptor.current_index <= descriptor.end_index);
#endif

      if (is_empty_free_list(descriptor))
      {
        --m_empty_lists_count;
      }

      T* mem = reinterpret_cast<T*>(m_heap);
      ptr = reinterpret_cast<void*>(&mem[descriptor.current_index]);

#if __DEBUG__
      ASSERT(ptr);
#endif

      descriptor.current_index += n;

#if __DEBUG__
      ASSERT(descriptor.current_index <= descriptor.end_index + 1);
#endif

      break;
    }
  }

  return ptr;
}

// -----------------------------------------------------------------------------

template<class T>
void*
BlockAllocator<T>::allocate_n(size_t n, size_t)
{
  return allocate_n(n);
}

// -----------------------------------------------------------------------------

template<class T>
int
BlockAllocator<T>::deallocate(void* ptr)
{
  int res = -1;

  if (ptr == nullptr)
  {
    return res;
  }

  uint64_t ptr_ = reinterpret_cast<uint64_t>(ptr);
  uint64_t heap_ = reinterpret_cast<uint64_t>(m_heap);

  if (ptr_ < heap_ || ptr_ > heap_ + m_total_size)
  {
    return res;
  }

  uint64_t range = ptr_ - heap_;

  uint32_t index = (uint32_t)( range / sizeof(T) );

  size_t i = 0;
  for (auto itr = m_free_lists.begin(); itr != m_free_lists.end(); ++itr, ++i)
  {
    FreeListDescriptor& descriptor = *itr;

    if (is_allocated_in_free_list(descriptor, index))
    {
      /**
       * Free a block in 2 cases:
       *
       *  1) at end of allocated blocks
       *    - decrement the current index
       *  2) at beginning or middle of list
       *    - split the current list into two, with the left list
       *      having the free block at `index` at the end.
       */

      if (index == descriptor.current_index - 1)
      {
        /**
         * Free at end of block.
         *
         */
        --descriptor.current_index;

        if (is_empty_free_list(descriptor))
        {
          ++m_empty_lists_count;
          if (m_empty_lists_count > 1)
          {
            combine_empty_freelists(i);
          }
        }
      }
      else
      {
        /**
         * Free at beginning or middle of the list.
         *
         */

        if (index == descriptor.start_index and i > 0 and is_empty_free_list(m_free_lists[i-1]))
        {
          /**
           * Free at the beginning of a list and there is a previous empty list.
           *
           * Just expand the previous empty list.
           */
          auto& previous_descriptor = m_free_lists[i-1];
          previous_descriptor.end_index = index;

          descriptor.start_index = index + 1;
        }
        else
        {
          /**
           * Mark the current list as one element less than full, and insert
           * a new list after it.
           */

          FreeListDescriptor new_descriptor(
            /* start_index */ index + 1,
            /* end_index */ descriptor.end_index,
            /* current_index */ descriptor.current_index);

          descriptor.end_index = index;
          descriptor.current_index = index;

          auto itr_ = itr;
          ++itr_;
          m_free_lists.insert(itr_, new_descriptor);

          if (is_empty_free_list(descriptor))
          {
            ++m_empty_lists_count;
            if (m_empty_lists_count > 1)
            {
              combine_empty_freelists(i);
            }
          }
        }
      }

      res = 1;
      break;
    }
  }

  return res;
}

// -----------------------------------------------------------------------------

template<class T>
void*
BlockAllocator<T>::find(void* ptr) const
{
  uint64_t ptr_ = reinterpret_cast<uint64_t>(ptr);
  uint64_t heap_ = reinterpret_cast<uint64_t>(m_heap);

  if (ptr_ < heap_ || ptr_ > heap_ + m_total_size)
  {
    return NULL;
  }

  const uint64_t range = ptr_ - heap_;

  const uint32_t index = (uint32_t)( range / sizeof(T) );

  for (auto itr = m_free_lists.begin(); itr != m_free_lists.end(); ++itr)
  {
    const FreeListDescriptor& descriptor = *itr;

    if (is_allocated_in_free_list(descriptor, index))
    {
      return ptr;
    }
  }

  return NULL;
}

// -----------------------------------------------------------------------------

template<class T>
void
BlockAllocator<T>::combine_empty_freelists(size_t i)
{
  const bool combine_with_previous = i > 0 and is_empty_free_list(m_free_lists[i - 1]);

#if __DEBUG__
  ASSERT(is_empty_free_list(m_free_lists[i]));
#endif

  if (combine_with_previous)
  {
    auto& previous_descriptor = m_free_lists[i - 1];
    auto& descriptor = m_free_lists[i];

    previous_descriptor.end_index = descriptor.end_index;
    auto itr = m_free_lists.begin();
    std::advance(itr, i);
    m_free_lists.erase(itr);
    --i;
    --m_empty_lists_count;
  }

  const bool combine_with_next = i < (m_free_lists.size() - 1) and is_empty_free_list(m_free_lists[i + 1]);

#if __DEBUG__
  ASSERT(is_empty_free_list(m_free_lists[i]));
#endif

  if (combine_with_next)
  {
    auto& descriptor = m_free_lists[i];
    auto& next_descriptor = m_free_lists[i + 1];

    descriptor.end_index = next_descriptor.end_index;
    auto itr = m_free_lists.begin();
    std::advance(itr, i + 1);
    m_free_lists.erase(itr);
    --m_empty_lists_count;
  }
}

// -----------------------------------------------------------------------------

template<class T>
uint64_t
BlockAllocator<T>::base_addr() const noexcept
{
  return static_cast<uint64_t>((uint8_t*)m_heap - (uint8_t*)NULL);
}

// -----------------------------------------------------------------------------

template<class T>
uint64_t
BlockAllocator<T>::total_size() const noexcept
{
  return m_total_size;
}

// -----------------------------------------------------------------------------

template<class T>
typename BlockAllocator<T>::iterator
BlockAllocator<T>::begin()
{
  int64_t descriptor_index = -1;
  int64_t slot_index = -1;
  size_t i = 0;

  for (auto itr = m_free_lists.begin(); itr != m_free_lists.end(); ++itr, ++i)
  {
    FreeListDescriptor& descriptor = *itr;

    if (descriptor.current_index > descriptor.start_index)
    {
      descriptor_index = static_cast<int64_t>(i);
      slot_index = 0;
      break;
    }
  }

  return BlockAllocator<T>::iterator(*this, descriptor_index, slot_index);
}

// -----------------------------------------------------------------------------

template<class T>
typename BlockAllocator<T>::iterator
BlockAllocator<T>::end()
{
  return BlockAllocator<T>::iterator(*this, -1, -1);
}

// -----------------------------------------------------------------------------

template<class T>
typename BlockAllocator<T>::const_iterator
BlockAllocator<T>::cbegin() const
{
  return const_cast<BlockAllocator<T>*>(this)->begin();
}

// -----------------------------------------------------------------------------

template<class T>
typename BlockAllocator<T>::const_iterator
BlockAllocator<T>::cend() const
{
  return const_cast<BlockAllocator<T>*>(this)->end();
}

// -----------------------------------------------------------------------------


} /* end namespace memory */


} /* end namespace corevm */


#endif /* COREVM_BLOCK_ALLOCATOR_H_ */
