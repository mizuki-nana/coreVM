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
#ifndef COREVM_MEMORY_HEAP_MEM_ALLOCATOR_H_
#define COREVM_MEMORY_HEAP_MEM_ALLOCATOR_H_


#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <new>
#include <sys/types.h>


namespace corevm {


namespace memory {


template<size_t N, class mem_allocation_scheme_type>
class heap_mem_allocator {
public:
  heap_mem_allocator();
  ~heap_mem_allocator();

  void* allocate(size_t) noexcept;
  int deallocate(void*) noexcept;

private:
  size_t m_total_size = 0;
  size_t m_allocated_size = 0;
  void* m_heap = nullptr;
  mem_allocation_scheme_type m_allocator_scheme;
};


template<size_t N, class mem_allocation_scheme_type>
corevm::memory::heap_mem_allocator<N, mem_allocation_scheme_type>::heap_mem_allocator():
  m_total_size(N),
  m_allocated_size(0),
  m_allocator_scheme(mem_allocation_scheme_type(m_total_size))
{
  void* mem = malloc(m_total_size);

  if(!mem) {
    throw std::bad_alloc();
  }

  m_heap = mem;
}

template<size_t N, class mem_allocation_scheme_type>
corevm::memory::heap_mem_allocator<N, mem_allocation_scheme_type>::~heap_mem_allocator()
{
  if(m_heap) {
    free(m_heap);
    m_heap = nullptr;
  }
}

template<size_t N, class mem_allocation_scheme_type>
void*
corevm::memory::heap_mem_allocator<N, mem_allocation_scheme_type>::allocate(size_t size) noexcept
{
  void* ptr = nullptr;

  if(size > m_total_size) {
    return ptr;
  }

  if(!(size > 0)) {
    return ptr;
  }

  ssize_t offset = m_allocator_scheme.malloc(size);

  if(offset >= 0) {
    char* base = static_cast<char*>(m_heap);
    ptr = base + static_cast<uint32_t>(offset);
    m_allocated_size += size;
    assert(m_allocated_size <= m_total_size);
  }

  return ptr;
}

template<size_t N, class mem_allocation_scheme_type>
int
corevm::memory::heap_mem_allocator<N, mem_allocation_scheme_type>::deallocate(void* ptr) noexcept
{
  int res = -1;
  char* ptr_ = static_cast<char*>(ptr);
  char* heap_ = static_cast<char*>(m_heap);

  size_t offset = ptr_ - heap_;
  ssize_t size = m_allocator_scheme.free(offset);

  if(size > 0) {
    memset(ptr, 0, static_cast<uint32_t>(size));
    m_allocated_size -= size;
    assert(m_allocated_size <= m_total_size);
    res = 1;
  }

  return res;
}


} /* end namespace memory */


} /* end namespace corevm */


#endif /* COREVM_MEMORY_HEAP_MEM_ALLOCATOR_H_ */
