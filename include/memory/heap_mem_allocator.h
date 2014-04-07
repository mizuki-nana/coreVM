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

  void* malloc_mem(size_t) noexcept;
  int free_mem(void*) noexcept;

private:
  size_t _total_size = 0;
  size_t _allocated_size = 0;
  void* _heap = nullptr;
  mem_allocation_scheme_type _allocator_scheme;
};


template<size_t N, class mem_allocation_scheme_type>
corevm::memory::heap_mem_allocator<N, mem_allocation_scheme_type>::heap_mem_allocator():
  _total_size(N),
  _allocated_size(0),
  _allocator_scheme(mem_allocation_scheme_type(_total_size))
{
  void* mem = malloc(_total_size);

  if(!mem) {
    throw std::bad_alloc();
  }

  _heap = mem;
}

template<size_t N, class mem_allocation_scheme_type>
corevm::memory::heap_mem_allocator<N, mem_allocation_scheme_type>::~heap_mem_allocator()
{
  if(_heap) {
    free(_heap);
    _heap = nullptr;
  }
}

template<size_t N, class mem_allocation_scheme_type>
void*
corevm::memory::heap_mem_allocator<N, mem_allocation_scheme_type>::malloc_mem(size_t size) noexcept
{
  void* ptr = nullptr;

  if(size > _total_size) {
    return ptr;
  }

  if(!(size > 0)) {
    return ptr;
  }

  ssize_t offset = _allocator_scheme.malloc(size);

  if(offset >= 0) {
    char* base = static_cast<char*>(_heap);
    ptr = base + static_cast<uint32_t>(offset);
    _allocated_size += size;
    assert(_allocated_size <= _total_size);
  }

  return ptr;
}

template<size_t N, class mem_allocation_scheme_type>
int
corevm::memory::heap_mem_allocator<N, mem_allocation_scheme_type>::free_mem(void* ptr) noexcept
{
  int res = -1;
  char* ptr_ = static_cast<char*>(ptr);
  char* heap_ = static_cast<char*>(_heap);

  size_t offset = ptr_ - heap_;
  ssize_t size = _allocator_scheme.free(offset);

  // found, free that memory
  if(size > 0) {
    memset(ptr, 0, static_cast<uint32_t>(size));
    _allocated_size -= size;
    assert(_allocated_size <= _total_size);
    res = 1;
  }

  return res;
}


} /* end namespace memory */


} /* end namespace corevm */


#endif /* COREVM_MEMORY_HEAP_MEM_ALLOCATOR_H_ */