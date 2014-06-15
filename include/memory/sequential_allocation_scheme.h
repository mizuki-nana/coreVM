#ifndef COREVM_SEQUENTIAL_ALLOCATION_SCHEME_H_
#define COREVM_SEQUENTIAL_ALLOCATION_SCHEME_H_

#include <list>
#include "allocation_scheme.h"


namespace corevm {


namespace memory {


typedef struct __sequential_block_descriptor {
  size_t size;
  uint64_t offset;
  bool free;
} sequential_block_descriptor;


class sequential_allocation_scheme : public corevm::memory::allocation_scheme {
public:
  sequential_allocation_scheme(size_t);

  using iterator = typename std::list<sequential_block_descriptor>::iterator;

  iterator begin() noexcept;
  iterator end() noexcept;

  virtual ssize_t malloc(size_t) noexcept;
  virtual ssize_t free(size_t) noexcept;

#ifdef __DEBUG__
  void debug_print() noexcept;
#endif
protected:
  virtual iterator _find_fit(size_t) noexcept = 0;
  virtual void _split(iterator, size_t, uint64_t) noexcept;
  virtual void _combine_free_blocks() noexcept;

  std::list<sequential_block_descriptor> _blocks;
};


class first_fit_allocation_scheme : public corevm::memory::sequential_allocation_scheme {
public:
  first_fit_allocation_scheme(size_t total_size) : corevm::memory::sequential_allocation_scheme(total_size) {}
protected:
  virtual iterator _find_fit(size_t) noexcept;
};


class best_fit_allocation_scheme : public corevm::memory::sequential_allocation_scheme {
public:
  best_fit_allocation_scheme(size_t total_size) : corevm::memory::sequential_allocation_scheme(total_size) {}
protected:
  virtual iterator _find_fit(size_t) noexcept;
};


class worst_fit_allocation_scheme : public corevm::memory::sequential_allocation_scheme {
public:
  worst_fit_allocation_scheme(size_t total_size) : corevm::memory::sequential_allocation_scheme(total_size) {}
protected:
  virtual iterator _find_fit(size_t) noexcept;
};


class next_fit_allocation_scheme : public corevm::memory::sequential_allocation_scheme {
public:
  next_fit_allocation_scheme(size_t total_size):
    corevm::memory::sequential_allocation_scheme(total_size)
  {
    _last_itr = this->begin();
  }
protected:
  virtual iterator _find_fit(size_t) noexcept;
  iterator _last_itr;
};


class buddy_allocation_scheme : public corevm::memory::sequential_allocation_scheme {
public:
  buddy_allocation_scheme(size_t total_size) : corevm::memory::sequential_allocation_scheme(total_size) {}
protected:
  virtual iterator _find_fit(size_t) noexcept;
  virtual void _combine_free_blocks() noexcept;
};


} /* end namespace memory */


} /* end namespace corevm */


#endif /* COREVM_SEQUENTIAL_ALLOCATION_SCHEME_H_ */
