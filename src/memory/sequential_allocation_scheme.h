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
#ifndef COREVM_SEQUENTIAL_ALLOCATION_SCHEME_H_
#define COREVM_SEQUENTIAL_ALLOCATION_SCHEME_H_

#include "allocation_scheme.h"

#include <cstdint>
#include <list>


namespace corevm {


namespace memory {


// -----------------------------------------------------------------------------

typedef struct __SequentialBlockDescriptor
{
  __SequentialBlockDescriptor()
    :
    size(0u),
    actual_size(0u),
    offset(0u),
    flags(0u)
  {
  }

  __SequentialBlockDescriptor(
    uint64_t size_,
    uint64_t actual_size_,
    uint64_t offset_,
    uint8_t flags_)
    :
    size(size_),
    actual_size(actual_size_),
    offset(offset_),
    flags(flags_)
  {
  }

  uint64_t size;
  uint64_t actual_size;
  uint64_t offset;
  uint8_t flags;
} SequentialBlockDescriptor;

// -----------------------------------------------------------------------------

class SequentialAllocationScheme : public AllocationScheme
{
public:
  explicit SequentialAllocationScheme(size_t);

private:
  using list_type = typename std::list<SequentialBlockDescriptor>;

public:
  using iterator = list_type::iterator;
  using const_iterator = list_type::const_iterator;

  virtual ssize_t malloc(size_t) noexcept;
  virtual ssize_t calloc(size_t, size_t) noexcept;
  virtual ssize_t free(size_t) noexcept;

  void debug_print(uint32_t) const noexcept;

  static const size_t LINEAR_SEARCH_BLOCK_COUNT_THRESHOLD;

  // Updatable for unit tests.
  static bool SUPPRESS_LINEAR_SEARCH;

protected:
  virtual SequentialBlockDescriptor default_block() const noexcept;
  virtual iterator find_fit(size_t) noexcept = 0;
  virtual void split(iterator, size_t, uint64_t) noexcept;
  virtual void combine_free_blocks() noexcept;

  size_t m_total_size;
  std::list<SequentialBlockDescriptor> m_blocks;
};

// -----------------------------------------------------------------------------

class FirstFitAllocationScheme : public SequentialAllocationScheme
{
public:
  explicit FirstFitAllocationScheme(size_t total_size);

protected:
  virtual iterator find_fit(size_t) noexcept;
};

// -----------------------------------------------------------------------------

class BestFitAllocationScheme : public SequentialAllocationScheme
{
public:
  explicit BestFitAllocationScheme(size_t total_size);

protected:
  virtual iterator find_fit(size_t) noexcept;
};

// -----------------------------------------------------------------------------

class WorstFitAllocationScheme : public SequentialAllocationScheme
{
public:
  explicit WorstFitAllocationScheme(size_t total_size);

protected:
  virtual iterator find_fit(size_t) noexcept;
};

// -----------------------------------------------------------------------------

class NextFitAllocationScheme : public SequentialAllocationScheme
{
public:
  explicit NextFitAllocationScheme(size_t total_size);

  virtual ssize_t free(size_t) noexcept;

protected:
  virtual iterator find_fit(size_t) noexcept;

  iterator m_last_itr;
};

// -----------------------------------------------------------------------------

class BuddyAllocationScheme : public SequentialAllocationScheme
{
public:
  explicit BuddyAllocationScheme(size_t total_size);

  virtual ssize_t malloc(size_t) noexcept;
  virtual ssize_t calloc(size_t, size_t) noexcept;

protected:
  virtual SequentialBlockDescriptor default_block() const noexcept;
  virtual iterator find_fit(size_t) noexcept;
  virtual void combine_free_blocks() noexcept;

  static const uint8_t FLAG_SPLIT;
  static const uint8_t FLAG_PARENT_SPLIT;
};

// -----------------------------------------------------------------------------


} /* end namespace memory */


} /* end namespace corevm */


#endif /* COREVM_SEQUENTIAL_ALLOCATION_SCHEME_H_ */
