#include <stdio.h>
#include <algorithm>
#include <sneaker/libc/math.h>
#include "../../include/memory/sequential_allocation_scheme.h"


/* typedefs to make things easier to read. */
typedef corevm::memory::sequential_allocation_scheme::iterator iterator_type;
typedef corevm::memory::sequential_block_descriptor block_descriptor_type;


corevm::memory::sequential_allocation_scheme::sequential_allocation_scheme(size_t total_size)
{
  this->_blocks.push_back(
    block_descriptor_type{
      .size = total_size,
      .offset = 0,
      .free = true
    }
  );
}

iterator_type
corevm::memory::sequential_allocation_scheme::begin() noexcept
{
  return _blocks.begin();
}

iterator_type
corevm::memory::sequential_allocation_scheme::end() noexcept
{
  return _blocks.end();
}

#ifdef __DEBUG__
void
corevm::memory::sequential_allocation_scheme::debug_print() noexcept
{
  printf("==============================\n");
  iterator_type itr;
  for(itr = begin(); itr != end(); itr++) {
    block_descriptor_type descriptor = static_cast<block_descriptor_type>(*itr);
    printf("Size [%lu] - Offset - [%d] - Free[ %d]\n",
      descriptor.size,
      descriptor.offset,
      descriptor.free
    );
  }
  printf("==============================\n");
}
#endif

void
corevm::memory::sequential_allocation_scheme::_split(
  iterator_type itr, size_t size, uint64_t offset) noexcept
{
  iterator_type itr_pos = itr;
  ++itr_pos;
  block_descriptor_type descriptor = {
    .size = size,
    .offset = offset,
    .free = true
  };
  this->_blocks.insert(itr_pos, descriptor);
}

void
corevm::memory::sequential_allocation_scheme::_combine_free_blocks() noexcept
{
  iterator_type itr = this->end();

  while(itr != this->begin()) {
    iterator_type current = itr;
    iterator_type prev = --itr;

    if(prev != this->end() && prev != this->begin()) {
      block_descriptor_type current_block = static_cast<block_descriptor_type>(*current);
      block_descriptor_type prev_block = static_cast<block_descriptor_type>(*prev);

      // merge
      if(current_block.free && prev_block.free) {
        prev_block.size += current_block.size;
        current_block.size = 0;
        *current = current_block;
        *prev = prev_block;
      }
    }
  } /* end of while loop */

  std::remove_if(
    this->begin(),
    this->end(),
    [](const block_descriptor_type& block) -> bool {
      return block.free == true && block.size == 0;
    }
  );
}

ssize_t
corevm::memory::sequential_allocation_scheme::malloc(size_t size) noexcept
{
  ssize_t res = -1;
  iterator_type itr;

  itr = this->_find_fit(size);

  if(itr != this->end()) {
    block_descriptor_type block_found = static_cast<block_descriptor_type>(*itr);
    block_found.free = false;
    *itr = block_found;

    if(block_found.size > size) {
      this->_split(itr, block_found.size - size, static_cast<uint64_t>(block_found.offset + size));
      block_found.size = size;
    }

    res = static_cast<ssize_t>(block_found.offset);

    *itr = block_found;
  }

  return res;
}

ssize_t
corevm::memory::sequential_allocation_scheme::free(size_t offset) noexcept
{
  ssize_t size_freed = -1;
  iterator_type itr;

  itr = std::find_if(
    this->begin(),
    this->end(),
    [this, offset](block_descriptor_type block) -> bool {
      return block.offset == offset && block.free == false;
    }
  );

  if(itr != this->end()) {
    block_descriptor_type block_found = static_cast<block_descriptor_type>(*itr);
    block_found.free = true;

    *itr = block_found;

    size_freed = static_cast<ssize_t>(block_found.size);
    this->_combine_free_blocks();
  }

  return size_freed;
}

/*********** BEGIN OF corevm::memory::first_fit_allocation_scheme **************/

iterator_type
corevm::memory::first_fit_allocation_scheme::_find_fit(size_t size) noexcept
{
  return std::find_if(
    this->begin(),
    this->end(),
    [this, size](block_descriptor_type block) -> bool {
      return block.size >= size && block.free == true;
    }
  );
}

/************ END OF corevm::memory::first_fit_allocation_scheme **************/


/*********** BEGIN OF corevm::memory::best_fit_allocation_scheme **************/

iterator_type
corevm::memory::best_fit_allocation_scheme::_find_fit(size_t size) noexcept
{
  iterator_type itr = std::min_element(
    this->begin(),
    this->end(),
    [size](block_descriptor_type block_a, block_descriptor_type block_b) -> bool {
      if(block_a.free == false) {
        return false;
      }

      if(block_b.free == false) {
        return true;
      }

      return block_a.size <= block_b.size && block_a.size >= size;
    }
  );

  if(itr == this->end()) {
    return itr;
  }

  block_descriptor_type block_found = static_cast<block_descriptor_type>(*itr);

  if(! (block_found.free == true && block_found.size >= size) ) {
    itr = this->end();
  }

  return itr;
}

/************ END OF corevm::memory::best_fit_allocation_scheme ***************/


/*********** BEGIN OF corevm::memory::worst_fit_allocation_scheme *************/

iterator_type
corevm::memory::worst_fit_allocation_scheme::_find_fit(size_t size) noexcept
{
  iterator_type itr = std::max_element(
    this->begin(),
    this->end(),
    [size](block_descriptor_type block_a, block_descriptor_type block_b) -> bool {
      if(block_b.free == false) {
        return false;
      }

      if(block_a.free == false) {
        return true;
      }

      return block_a.size <= block_b.size && block_a.size >= size;
    }
  );

  if(itr == this->end()) {
    return itr;
  }

  block_descriptor_type block_found = static_cast<block_descriptor_type>(*itr);

  if(! (block_found.free == true && block_found.size >= size) ) {
    itr = this->end();
  }

  return itr;
}

/************ END OF corevm::memory::worst_fit_allocation_scheme **************/


/*********** BEGIN OF corevm::memory::next_fit_allocation_scheme **************/

iterator_type
corevm::memory::next_fit_allocation_scheme::_find_fit(size_t size) noexcept
{
  iterator_type last_itr = this->_last_itr;
  iterator_type itr = end();

  auto criterion = [this, size](block_descriptor_type block) -> bool {
    return block.size >= size && block.free == true;
  };

  itr = std::find_if(last_itr, this->end(), criterion);

  if(itr != end()) {
    this->_last_itr = itr;
    return itr;
  }

  itr = std::find_if(this->begin(), itr, criterion);

  if(itr != end()) {
    this->_last_itr = itr;
  }

  return itr;
}

/************ END OF corevm::memory::next_fit_allocation_scheme ***************/


/************* BEGIN OF corevm::memory::buddy_allocation_scheme ***************/

iterator_type
corevm::memory::buddy_allocation_scheme::_find_fit(size_t size) noexcept
{
  size_t nearest_power_of_2 = static_cast<size_t>(nearest_exp2_ceil(size));

  iterator_type itr = this->end();

  while(1) {
    itr = std::find_if(
      this->begin(),
      this->end(),
      [this, nearest_power_of_2](block_descriptor_type block) -> bool {
        return block.size >= nearest_power_of_2 && block.free == true;
      }
    );

    if(itr == this->end()) {
      break;  
    }

    block_descriptor_type block_found = static_cast<block_descriptor_type>(*itr);

    if(block_found.size / 2 > size) {
      // split
      block_found.size = block_found.size / 2;
      *itr = block_found;
      this->_split(itr, block_found.size, static_cast<uint64_t>(block_found.offset + block_found.size));
      *itr = block_found;
    } else {
      // bingo! use this block
      block_found.free = false;
      block_found.size = block_found.size; // size stay the same
      *itr = block_found;
      break;
    }
  }

  return itr;
}

void
corevm::memory::buddy_allocation_scheme::_combine_free_blocks() noexcept
{
  /* We want to combine two free blocks only if they were originally splitted
   * from a larger block. */

  bool has_freed_blocks = true;

  while(has_freed_blocks) {

    has_freed_blocks = false;

    size_t i = 1;
    for(iterator_type itr = begin(); itr != end(); itr++, i++) {
      iterator_type current_itr = itr;
      iterator_type next_itr = current_itr;
      ++next_itr;

      if(itr != end() && next_itr != end() && i % 2 == 1) {

        block_descriptor_type current_block = static_cast<block_descriptor_type>(*itr);
        block_descriptor_type next_block = static_cast<block_descriptor_type>(*next_itr);

        if(current_block.free && next_block.free) {

          block_descriptor_type combined_block = {
            .size = current_block.size + next_block.size,
            .offset = current_block.offset,
            .free = true
          };

          *itr = combined_block;

          _blocks.erase(next_itr);

          has_freed_blocks = true;

        } /* end of if-stmt */

      } /* end of if-stmt */

    } /* end of for-loop */

  } /* end of while-loop */
}

/*************** END OF corevm::memory::buddy_allocation_scheme ***************/
