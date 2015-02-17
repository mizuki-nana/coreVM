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
#include "../../include/memory/sequential_allocation_scheme.h"

#include <sneaker/libc/math.h>

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>


typedef corevm::memory::sequential_allocation_scheme::iterator iterator_type;
typedef corevm::memory::sequential_allocation_scheme::const_iterator const_iterator_type;
typedef corevm::memory::sequential_block_descriptor block_descriptor_type;

// -----------------------------------------------------------------------------

corevm::memory::sequential_allocation_scheme::sequential_allocation_scheme(
  size_t total_size
):
  m_total_size(total_size)
{
}

// -----------------------------------------------------------------------------

void
corevm::memory::sequential_allocation_scheme::debug_print(uint32_t base) const noexcept
{
  const std::string LINE        = "------------------------------------------------------------------------------------------";
  const std::string BLANK_SPACE = "|                                                                                        |";

  std::stringstream ss;

  ss << LINE << std::endl;
  ss << "| Heap debug print (starting at " << std::setw(10) \
    << std::hex << std::showbase << base << std::noshowbase << std::dec \
    << ")                                              |" << std::endl;
  ss << BLANK_SPACE << std::endl;

  for (auto itr = cbegin(); itr != cend(); itr++)
  {
    block_descriptor_type descriptor = static_cast<block_descriptor_type>(*itr);

    ss << "| ";
    ss << std::left << std::setw(10) << std::hex << std::showbase \
      << base + descriptor.offset << std::noshowbase << std::dec << " " << std::right;
    ss << "BlockSize[" << std::setw(10) << descriptor.size << "] ";
    ss << "ActualSize[" << std::setw(10) << descriptor.actual_size << "] ";
    ss << "Offset[" << std::setw(10) << descriptor.offset << "] ";
    ss << "Flags[" << std::setw(4) << std::hex << std::showbase \
      << (uint64_t)descriptor.flags << "]" << std::noshowbase << std::dec;
    ss << " |" << std::endl;
  }
  ss << LINE << std::endl;

  std::cout << ss.str();
}

// -----------------------------------------------------------------------------

block_descriptor_type
corevm::memory::sequential_allocation_scheme::default_block() const noexcept
{
  return block_descriptor_type {
    .size = m_total_size,
    .actual_size = 0,
    .offset = 0,
    .flags = 0,
  };
}

// -----------------------------------------------------------------------------

iterator_type
corevm::memory::sequential_allocation_scheme::begin() noexcept
{
  return m_blocks.begin();
}

// -----------------------------------------------------------------------------

iterator_type
corevm::memory::sequential_allocation_scheme::end() noexcept
{
  return m_blocks.end();
}

// -----------------------------------------------------------------------------

const_iterator_type
corevm::memory::sequential_allocation_scheme::cbegin() const noexcept
{
  return m_blocks.cbegin();
}

// -----------------------------------------------------------------------------

const_iterator_type
corevm::memory::sequential_allocation_scheme::cend() const noexcept
{
  return m_blocks.cend();
}

// -----------------------------------------------------------------------------

void
corevm::memory::sequential_allocation_scheme::split(
  iterator_type itr, size_t size, uint64_t offset) noexcept
{
  iterator_type itr_pos = itr;
  ++itr_pos;
  block_descriptor_type descriptor {
    .size = size,
    .actual_size = 0,
    .offset = offset,
    .flags = 0
  };
  this->m_blocks.insert(itr_pos, descriptor);
}

// -----------------------------------------------------------------------------

void
corevm::memory::sequential_allocation_scheme::combine_free_blocks() noexcept
{
  iterator_type itr = this->begin();

  while (itr != this->end())
  {
    iterator_type current = itr;
    iterator_type next = ++itr;

    if (next != this->end())
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

  std::remove_if(
    this->begin(),
    this->end(),
    [](const block_descriptor_type& block) -> bool {
      return block.actual_size == 0 && block.size == 0;
    }
  );
}

// -----------------------------------------------------------------------------

ssize_t
corevm::memory::sequential_allocation_scheme::malloc(size_t size) noexcept
{
  ssize_t res = -1;
  iterator_type itr;

  itr = this->find_fit(size);

  if (itr != this->end())
  {
    block_descriptor_type block_found = static_cast<block_descriptor_type>(*itr);
    block_found.actual_size = size;
    *itr = block_found;

    if (block_found.size > size)
    {
      this->split(itr, block_found.size - size, static_cast<uint64_t>(block_found.offset + size));
      block_found.size = size;
    }

    res = static_cast<ssize_t>(block_found.offset);

    *itr = block_found;
  }

  return res;
}

// -----------------------------------------------------------------------------

ssize_t
corevm::memory::sequential_allocation_scheme::free(size_t offset) noexcept
{
  ssize_t size_freed = -1;
  iterator_type itr;

  itr = std::find_if(
    this->begin(),
    this->end(),
    [this, offset](block_descriptor_type block) -> bool {
      return block.offset == offset && block.actual_size != 0;
    }
  );

  if (itr != this->end())
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


/* -------------- corevm::memory::first_fit_allocation_scheme --------------- */


// -----------------------------------------------------------------------------

corevm::memory::first_fit_allocation_scheme::first_fit_allocation_scheme(
  size_t total_size
):
  corevm::memory::sequential_allocation_scheme(total_size)
{
  this->m_blocks.push_back(this->default_block());
}

iterator_type
corevm::memory::first_fit_allocation_scheme::find_fit(size_t size) noexcept
{
  return std::find_if(
    this->begin(),
    this->end(),
    [this, size](block_descriptor_type block) -> bool {
      return block.size >= size && block.actual_size == 0;
    }
  );
}

// -----------------------------------------------------------------------------


/* -------------- corevm::memory::best_fit_allocation_scheme ---------------- */


// -----------------------------------------------------------------------------

corevm::memory::best_fit_allocation_scheme::best_fit_allocation_scheme(
  size_t total_size
):
  corevm::memory::sequential_allocation_scheme(total_size)
{
  this->m_blocks.push_back(this->default_block());
}

// -----------------------------------------------------------------------------

iterator_type
corevm::memory::best_fit_allocation_scheme::find_fit(size_t size) noexcept
{
  iterator_type itr = std::min_element(
    this->begin(),
    this->end(),
    [size](block_descriptor_type block_a, block_descriptor_type block_b) -> bool {
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

  if (itr == this->end())
  {
    return itr;
  }

  block_descriptor_type block_found = static_cast<block_descriptor_type>(*itr);

  if (! (block_found.actual_size == 0 && block_found.size >= size) )
  {
    itr = this->end();
  }

  return itr;
}

// -----------------------------------------------------------------------------


/* ------------- corevm::memory::worst_fit_allocation_scheme ---------------- */


// -----------------------------------------------------------------------------

corevm::memory::worst_fit_allocation_scheme::worst_fit_allocation_scheme(
  size_t total_size
):
  corevm::memory::sequential_allocation_scheme(total_size)
{
  this->m_blocks.push_back(this->default_block());
}

// -----------------------------------------------------------------------------

iterator_type
corevm::memory::worst_fit_allocation_scheme::find_fit(size_t size) noexcept
{
  iterator_type itr = std::max_element(
    this->begin(),
    this->end(),
    [size](block_descriptor_type block_a, block_descriptor_type block_b) -> bool {
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

  if (itr == this->end())
  {
    return itr;
  }

  block_descriptor_type block_found = static_cast<block_descriptor_type>(*itr);

  if (! (block_found.actual_size == 0 && block_found.size >= size) )
  {
    itr = this->end();
  }

  return itr;
}

// -----------------------------------------------------------------------------


/* -------------- corevm::memory::next_fit_allocation_scheme ---------------- */


// -----------------------------------------------------------------------------

corevm::memory::next_fit_allocation_scheme::next_fit_allocation_scheme(
  size_t total_size
):
  corevm::memory::sequential_allocation_scheme(total_size)
{
  m_last_itr = this->begin();
  this->m_blocks.push_back(this->default_block());
}

// -----------------------------------------------------------------------------

iterator_type
corevm::memory::next_fit_allocation_scheme::find_fit(size_t size) noexcept
{
  iterator_type last_itr = this->m_last_itr;
  iterator_type itr = end();

  auto criterion = [this, size](block_descriptor_type block) -> bool {
    return block.size >= size && block.actual_size == 0;
  };

  itr = std::find_if(last_itr, this->end(), criterion);

  if (itr != end())
  {
    this->m_last_itr = itr;
    return itr;
  }

  itr = std::find_if(this->begin(), itr, criterion);

  if (itr != end())
  {
    this->m_last_itr = itr;
  }

  return itr;
}

// -----------------------------------------------------------------------------


/* ---------------- corevm::memory::buddy_allocation_scheme ----------------- */


// -----------------------------------------------------------------------------

// Helper functions
// TODO: [COREVM-77] Move helper functions away
void
set_nth_bit_uint8(uint8_t *val, char bit)
{
  uint8_t b = (1 << (bit - 1));
  *val = *val | b;
}

// -----------------------------------------------------------------------------

int
is_bit_set_uint8(uint32_t val, char bit)
{
  uint8_t v = val;
  uint8_t b = (1 << (bit - 1));
  v = v & b;
  return (v >> (bit - 1)) == 1;
}

// -----------------------------------------------------------------------------

const uint8_t corevm::memory::buddy_allocation_scheme::FLAG_SPLIT = 1;

// -----------------------------------------------------------------------------

const uint8_t corevm::memory::buddy_allocation_scheme::FLAG_PARENT_SPLIT = 2;

// -----------------------------------------------------------------------------

corevm::memory::buddy_allocation_scheme::buddy_allocation_scheme(
  size_t total_size
):
  corevm::memory::sequential_allocation_scheme(total_size)
{
  this->m_blocks.push_back(this->default_block());
}

// -----------------------------------------------------------------------------

block_descriptor_type
corevm::memory::buddy_allocation_scheme::default_block() const noexcept
{
  return block_descriptor_type {
    .size = m_total_size,
    .offset = 0,
    .flags = ( 0x01 << (FLAG_PARENT_SPLIT - 1) )
  };
}

// -----------------------------------------------------------------------------

ssize_t
corevm::memory::buddy_allocation_scheme::malloc(size_t size) noexcept
{
  ssize_t res = -1;
  iterator_type itr;

  itr = this->find_fit(size);

  if (itr != this->end())
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

iterator_type
corevm::memory::buddy_allocation_scheme::find_fit(size_t size) noexcept
{
  size_t nearest_power_of_2 = static_cast<size_t>(nearest_exp2_ceil(size));

  iterator_type itr = this->end();

  while (1)
  {
    itr = std::find_if(
      this->begin(),
      this->end(),
      [this, nearest_power_of_2](block_descriptor_type block) -> bool {
        return block.size >= nearest_power_of_2 && block.actual_size == 0;
      }
    );

    if (itr == this->end())
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
corevm::memory::buddy_allocation_scheme::combine_free_blocks() noexcept
{
  /*
   * We want to combine two free blocks only if they were originally splitted
   * from a larger block.
   */

  bool has_freed_blocks = true;

  while (has_freed_blocks)
  {
    has_freed_blocks = false;

    for (iterator_type itr = begin(); itr != end(); itr++)
    {
      iterator_type current_itr = itr;
      iterator_type next_itr = current_itr;
      ++next_itr;

      block_descriptor_type current_block = static_cast<block_descriptor_type>(*itr);

      if (itr != end() && next_itr != end())
      {
        block_descriptor_type next_block = static_cast<block_descriptor_type>(*next_itr);

        bool is_split = is_bit_set_uint8(current_block.flags, FLAG_SPLIT) &&
          !is_bit_set_uint8(next_block.flags, FLAG_SPLIT);

        bool is_parent_split = is_bit_set_uint8(current_block.flags, FLAG_PARENT_SPLIT);

        if (is_split && current_block.actual_size == 0 && next_block.actual_size == 0)
        {
          uint8_t flags = 0;

          if (is_parent_split)
          {
            set_nth_bit_uint8(&flags, FLAG_PARENT_SPLIT);
            set_nth_bit_uint8(&flags, FLAG_SPLIT);
          }

          block_descriptor_type combined_block {
            .size = current_block.size + next_block.size,
            .actual_size = 0,
            .offset = current_block.offset,
            .flags = flags
          };

          *itr = combined_block;

          m_blocks.erase(next_itr);

          has_freed_blocks = true;

        } /* end of if-stmt */

      } /* end of if-stmt */

    } /* end of for-loop */

  } /* end of while-loop */
}

// -----------------------------------------------------------------------------
