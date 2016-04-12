/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Yanzheng Li

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
#include "sequential_allocation_scheme.h"

#include <sneaker/libc/math.h>
#include <sneaker/libc/utils.h>

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


namespace corevm {


namespace memory {


// -----------------------------------------------------------------------------

const size_t SequentialAllocationScheme::LINEAR_SEARCH_BLOCK_COUNT_THRESHOLD = 10;

// -----------------------------------------------------------------------------

bool SequentialAllocationScheme::SUPPRESS_LINEAR_SEARCH = false;

// -----------------------------------------------------------------------------

typedef SequentialAllocationScheme::iterator iterator_type;
typedef SequentialAllocationScheme::const_iterator const_iterator_type;
typedef SequentialBlockDescriptor block_descriptor_type;

// -----------------------------------------------------------------------------

SequentialAllocationScheme::SequentialAllocationScheme(size_t total_size)
  :
  m_total_size(total_size)
{
}

// -----------------------------------------------------------------------------

void
SequentialAllocationScheme::debug_print(uint32_t base) const noexcept
{
  const std::string LINE        = "------------------------------------------------------------------------------------------";
  const std::string BLANK_SPACE = "|                                                                                        |";

  std::stringstream ss;

  ss << LINE << std::endl;
  ss << "| Heap debug print (starting at " << std::setw(10)
    << std::hex << std::showbase << base << std::noshowbase << std::dec
    << ")                                              |" << std::endl;
  ss << BLANK_SPACE << std::endl;

  for (auto itr = m_blocks.cbegin(); itr != m_blocks.cend(); ++itr)
  {
    block_descriptor_type descriptor = static_cast<block_descriptor_type>(*itr);

    ss << "| ";
    ss << std::left << std::setw(10) << std::hex << std::showbase
      << base + descriptor.offset << std::noshowbase << std::dec << " " << std::right;
    ss << "BlockSize[" << std::setw(10) << descriptor.size << "] ";
    ss << "ActualSize[" << std::setw(10) << descriptor.actual_size << "] ";
    ss << "Offset[" << std::setw(10) << descriptor.offset << "] ";
    ss << "Flags[" << std::setw(4) << std::hex << std::showbase
      << (uint64_t)descriptor.flags << "]" << std::noshowbase << std::dec;
    ss << " |" << std::endl;
  }
  ss << LINE << std::endl;

  std::cout << ss.str();
}

// -----------------------------------------------------------------------------

block_descriptor_type
SequentialAllocationScheme::default_block() const noexcept
{
  return block_descriptor_type(m_total_size, 0u, 0u, 0u);
}

// -----------------------------------------------------------------------------

void
SequentialAllocationScheme::split(
  iterator_type itr, size_t size, uint64_t offset) noexcept
{
  block_descriptor_type descriptor(size, 0u, offset, 0u);
  this->m_blocks.insert(++itr, descriptor);
}

// -----------------------------------------------------------------------------

void
SequentialAllocationScheme::combine_free_blocks() noexcept
{
  iterator_type itr = m_blocks.begin();

  while (itr != m_blocks.end())
  {
    iterator_type current = itr;
    iterator_type next = ++itr;

    if (next != m_blocks.end())
    {
      block_descriptor_type current_block = static_cast<block_descriptor_type>(*current);
      block_descriptor_type next_block = static_cast<block_descriptor_type>(*next);

      if (current_block.actual_size == 0 && next_block.actual_size == 0)
      {
        current_block.size += next_block.size;
        next_block.size = 0;
        *current = current_block;
        *next = next_block;
      }
    }
  }

  auto new_end_itr = std::remove_if(
    m_blocks.begin(),
    m_blocks.end(),
    [](const block_descriptor_type& block) -> bool {
      return block.actual_size == 0u && block.size == 0u;
    }
  );
  m_blocks.erase(new_end_itr, m_blocks.end());
}

// -----------------------------------------------------------------------------

ssize_t
SequentialAllocationScheme::malloc(size_t size) noexcept
{
  ssize_t res = -1;
  iterator_type itr;

  itr = this->find_fit(size);

  if (itr != m_blocks.end())
  {
    block_descriptor_type block_found = static_cast<block_descriptor_type>(*itr);
    block_found.actual_size = size;

    if (block_found.size > size)
    {
      this->split(
        itr, block_found.size - size,
        static_cast<uint64_t>(block_found.offset + size));

      block_found.size = size;
    }

    res = static_cast<ssize_t>(block_found.offset);

    *itr = block_found;
  }

  return res;
}

// -----------------------------------------------------------------------------

ssize_t
SequentialAllocationScheme::calloc(size_t num, size_t size) noexcept
{
  // TODO: [COREVM-282] Optimize allocator
  ssize_t res = -1;
  size_t alloc_size = size * num;

  iterator_type itr = this->find_fit(alloc_size);

  if (itr != m_blocks.end())
  {
    block_descriptor_type block_found = static_cast<block_descriptor_type>(*itr);

    if (block_found.size > alloc_size)
    {
      this->split(
        itr, block_found.size - alloc_size,
        static_cast<uint64_t>(block_found.offset + alloc_size));
    }

    block_found.size = size;
    block_found.actual_size = size;
    *itr = block_found;

    res = static_cast<ssize_t>(block_found.offset);

    if (num > 1)
    {
      uint64_t offset = block_found.offset + (uint64_t)size;
      std::vector<block_descriptor_type> descriptors(num - 1);

      for (size_t i = 0; i < descriptors.size(); ++i)
      {
        descriptors[i].size = size;
        descriptors[i].actual_size = size;
        descriptors[i].offset = offset;

        offset += (uint64_t)size;
      }
      this->m_blocks.insert(++itr, descriptors.begin(), descriptors.end());
    }
  }

  return res;
}

// -----------------------------------------------------------------------------

// TODO: Move this to sneaker or somewhere else.
template<class ForwardIterator, class T, class Compare>
ForwardIterator binary_find(
  ForwardIterator first, ForwardIterator last, const T& val, Compare comp)
{
  first = std::lower_bound(first, last, val, comp);

  if (first != last && !(comp(val, *first)))
  {
    return first;
  }
  else
  {
    return last;
  }
}

// -----------------------------------------------------------------------------

// TODO: Move this to sneaker or somewhere else.
template<class ForwardIterator, class T, class Compare, class Predicate>
ForwardIterator binary_find_if(
  ForwardIterator first, ForwardIterator last,
  const T& val, Compare comp, Predicate pred)
{
  first = binary_find(first, last, val, comp);

  if (first == last)
  {
    return last;
  }

  return pred(*first) ? first : last;
}

// -----------------------------------------------------------------------------

ssize_t
SequentialAllocationScheme::free(size_t offset) noexcept
{
  ssize_t size_freed = -1;
  iterator_type itr;

  auto pred = [offset](block_descriptor_type block) -> bool {
    return block.offset == offset && block.actual_size != 0;
  };

  itr = std::find_if(m_blocks.begin(), m_blocks.end(), pred);

  /**
   * TODO: Investigate benefits of binary search instead of linear search.
   *
  if (m_blocks.size() <= LINEAR_SEARCH_BLOCK_COUNT_THRESHOLD &&
      !SUPPRESS_LINEAR_SEARCH)
  {
    itr = std::find_if(m_blocks.begin(), m_blocks.end(), pred);
  }
  else
  {
    auto comp = [](
      const block_descriptor_type& lhs,
      const block_descriptor_type& rhs) -> bool
    {
      return lhs.offset < rhs.offset;
    };

    block_descriptor_type ref { .offset = offset };
    itr = binary_find_if(m_blocks.begin(), m_blocks.end(), ref, comp, pred);
  }
  *
  **/

  if (itr != m_blocks.end())
  {
    block_descriptor_type block_found = static_cast<block_descriptor_type>(*itr);

    size_freed = static_cast<ssize_t>(block_found.actual_size);

    block_found.actual_size = 0;
    *itr = block_found;

    this->combine_free_blocks();
  }

  return size_freed;
}

// -----------------------------------------------------------------------------


/*                         FirstFitAllocationScheme                           */


// -----------------------------------------------------------------------------

FirstFitAllocationScheme::FirstFitAllocationScheme(size_t total_size)
  :
  SequentialAllocationScheme(total_size)
{
  this->m_blocks.push_back(this->default_block());
}

// -----------------------------------------------------------------------------

iterator_type
FirstFitAllocationScheme::find_fit(size_t size) noexcept
{
  return std::find_if(
    m_blocks.begin(),
    m_blocks.end(),
    [this, size](block_descriptor_type block) -> bool {
      return block.size >= size && block.actual_size == 0;
    }
  );
}

// -----------------------------------------------------------------------------


/* -------------- BestFitAllocationScheme ---------------- */


// -----------------------------------------------------------------------------

BestFitAllocationScheme::BestFitAllocationScheme(size_t total_size)
  :
  SequentialAllocationScheme(total_size)
{
  this->m_blocks.push_back(this->default_block());
}

// -----------------------------------------------------------------------------

iterator_type
BestFitAllocationScheme::find_fit(size_t size) noexcept
{
  iterator_type itr = std::min_element(
    m_blocks.begin(),
    m_blocks.end(),
    [size](
      block_descriptor_type block_a, block_descriptor_type block_b) -> bool {
      if (block_a.actual_size != 0)
      {
        return false;
      }

      if (block_b.actual_size != 0)
      {
        return true;
      }

      return block_a.size <= block_b.size && block_a.size >= size;
    }
  );

  if (itr == m_blocks.end())
  {
    return itr;
  }

  block_descriptor_type block_found = static_cast<block_descriptor_type>(*itr);

  if (! (block_found.actual_size == 0 && block_found.size >= size) )
  {
    itr = m_blocks.end();
  }

  return itr;
}

// -----------------------------------------------------------------------------


/*                          WorstFitAllocationScheme                          */


// -----------------------------------------------------------------------------

WorstFitAllocationScheme::WorstFitAllocationScheme(
  size_t total_size)
  :
  SequentialAllocationScheme(total_size)
{
  this->m_blocks.push_back(this->default_block());
}

// -----------------------------------------------------------------------------

iterator_type
WorstFitAllocationScheme::find_fit(size_t size) noexcept
{
  iterator_type itr = std::max_element(
    m_blocks.begin(),
    m_blocks.end(),
    [size](
      block_descriptor_type block_a, block_descriptor_type block_b) -> bool {
      if (block_b.actual_size != 0)
      {
        return false;
      }

      if (block_a.actual_size != 0)
      {
        return true;
      }

      return block_a.size <= block_b.size && block_a.size >= size;
    }
  );

  if (itr == m_blocks.end())
  {
    return itr;
  }

  block_descriptor_type block_found = static_cast<block_descriptor_type>(*itr);

  if (! (block_found.actual_size == 0 && block_found.size >= size) )
  {
    itr = m_blocks.end();
  }

  return itr;
}

// -----------------------------------------------------------------------------


/*                            NextFitAllocationScheme                         */


// -----------------------------------------------------------------------------

NextFitAllocationScheme::NextFitAllocationScheme(size_t total_size)
  :
  SequentialAllocationScheme(total_size)
{
  m_last_itr = m_blocks.begin();
  this->m_blocks.push_back(this->default_block());
}

// -----------------------------------------------------------------------------

iterator_type
NextFitAllocationScheme::find_fit(size_t size) noexcept
{
  auto criterion = [size](block_descriptor_type block) -> bool {
    return block.size >= size && block.actual_size == 0;
  };

  iterator_type itr = std::find_if(m_last_itr, m_blocks.end(), criterion);

  if (itr != m_blocks.end())
  {
    this->m_last_itr = itr;
    return itr;
  }

  itr = std::find_if(m_blocks.begin(), m_blocks.end(), criterion);

  if (itr != m_blocks.end())
  {
    m_last_itr = itr;
  }

  return itr;
}

// -----------------------------------------------------------------------------

ssize_t
NextFitAllocationScheme::free(size_t offset) noexcept
{
  if (m_last_itr != m_blocks.end())
  {
    const block_descriptor_type& block = *m_last_itr;
    if (block.offset == offset)
    {
      m_last_itr = m_blocks.end();
    }
  }

  return SequentialAllocationScheme::free(offset);
}

// -----------------------------------------------------------------------------


/*                            BuddyAllocationScheme                           */


// -----------------------------------------------------------------------------

const uint8_t BuddyAllocationScheme::FLAG_SPLIT = 1;

// -----------------------------------------------------------------------------

const uint8_t BuddyAllocationScheme::FLAG_PARENT_SPLIT = 2;

// -----------------------------------------------------------------------------

BuddyAllocationScheme::BuddyAllocationScheme(size_t total_size)
  :
  SequentialAllocationScheme(total_size)
{
  this->m_blocks.push_back(this->default_block());
}

// -----------------------------------------------------------------------------

block_descriptor_type
BuddyAllocationScheme::default_block() const noexcept
{
  return block_descriptor_type(
    m_total_size,
    0u,
    0u,
    0x01 << (FLAG_PARENT_SPLIT - 1));
}

// -----------------------------------------------------------------------------

ssize_t
BuddyAllocationScheme::malloc(size_t size) noexcept
{
  ssize_t res = -1;
  iterator_type itr;

  itr = this->find_fit(size);

  if (itr != m_blocks.end())
  {
    block_descriptor_type block_found = static_cast<block_descriptor_type>(*itr);
    block_found.actual_size = size;
    *itr = block_found;

    res = static_cast<ssize_t>(block_found.offset);

    *itr = block_found;
  }

  return res;
}

// -----------------------------------------------------------------------------

ssize_t
BuddyAllocationScheme::calloc(size_t /* num */, size_t /* size */) noexcept
{
  // Buddy allocation scheme does not currently support `calloc`.
  return -1;
}

// -----------------------------------------------------------------------------

iterator_type
BuddyAllocationScheme::find_fit(size_t size) noexcept
{
  size_t nearest_power_of_2 = static_cast<size_t>(
    nearest_exp2_ceil(static_cast<uint32_t>(size)));

  iterator_type itr = m_blocks.end();

  while (1)
  {
    itr = std::find_if(
      m_blocks.begin(),
      m_blocks.end(),
      [this, nearest_power_of_2](block_descriptor_type block) -> bool {
        return block.size >= nearest_power_of_2 && block.actual_size == 0;
      }
    );

    if (itr == m_blocks.end())
    {
      break;
    }

    block_descriptor_type block_found = static_cast<block_descriptor_type>(*itr);

    if (block_found.size / 2 >= size) // split
    {
      block_found.size = block_found.size / 2;
      set_nth_bit_uint8(&block_found.flags, FLAG_SPLIT);
      *itr = block_found;
      this->split(
        itr,
        block_found.size,
        static_cast<uint64_t>(block_found.offset + block_found.size)
      );
    }
    else // bingo! use this block
    {
      block_found.size = block_found.size; // size stay the same
      block_found.actual_size = size;
      *itr = block_found;
      break;
    }
  }

  return itr;
}

// -----------------------------------------------------------------------------

void
BuddyAllocationScheme::combine_free_blocks() noexcept
{
  /*
   * We want to combine two free blocks only if they were originally splitted
   * from a larger block.
   */

  bool has_freed_blocks = true;

  while (has_freed_blocks)
  {
    has_freed_blocks = false;

    for (iterator_type itr = m_blocks.begin(); itr != m_blocks.end(); ++itr)
    {
      iterator_type current_itr = itr;
      iterator_type next_itr = current_itr;
      ++next_itr;

      block_descriptor_type current_block =
        static_cast<block_descriptor_type>(*itr);

      if (itr != m_blocks.end() && next_itr != m_blocks.end())
      {
        block_descriptor_type next_block =
          static_cast<block_descriptor_type>(*next_itr);

        bool is_split = is_bit_set_uint8(current_block.flags, FLAG_SPLIT) &&
          !is_bit_set_uint8(next_block.flags, FLAG_SPLIT);

        bool is_parent_split = is_bit_set_uint8(
          current_block.flags, FLAG_PARENT_SPLIT);

        if (is_split && current_block.actual_size == 0 &&
            next_block.actual_size == 0)
        {
          uint8_t flags = 0;

          if (is_parent_split)
          {
            set_nth_bit_uint8(&flags, FLAG_PARENT_SPLIT);
            set_nth_bit_uint8(&flags, FLAG_SPLIT);
          }

          block_descriptor_type combined_block(
            current_block.size + next_block.size, 0u, current_block.offset, flags);

          *itr = combined_block;

          m_blocks.erase(next_itr);

          has_freed_blocks = true;

        } /* end of if-stmt */

      } /* end of if-stmt */

    } /* end of for-loop */

  } /* end of while-loop */
}

// -----------------------------------------------------------------------------


} /* end namespace memory */


} /* end namespace corevm */
