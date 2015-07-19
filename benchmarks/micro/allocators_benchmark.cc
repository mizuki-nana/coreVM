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
#include <benchmark/benchmark.h>

#include "memory/allocator.h"
#include "memory/sequential_allocation_scheme.h"

#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool.hpp>
#include <boost/pool/pool_alloc.hpp>

#include <memory>
#include <vector>

/**
 * A benchmark for simple sequential allocations and deallocations of
 * chunks of memories of the same size.
 *
 * Note that this benchmark does not necessarily reflects the relative
 * performances of these allocators in real-world scenarios in an accurate
 * manner, as real-world allocations and deallocations happen in a much more
 * randomized manner.
 *
 * References:
 *
 * Background on memory pool:
 * http://www.boost.org/doc/libs/1_58_0/libs/pool/doc/html/boost_pool/pool/pooling.html
 *
 * General guide on Boost.Pool:
 * http://theboostcpplibraries.com/boost.pool
 *
 * Boost Pool Interfaces:
 * http://www.boost.org/doc/libs/1_58_0/libs/pool/doc/html/boost_pool/pool/interfaces.html
 */

// -----------------------------------------------------------------------------

typedef uint8_t T;

// -----------------------------------------------------------------------------

const size_t CHUNK_SIZE = sizeof(T);

// -----------------------------------------------------------------------------

const size_t LOOP_COUNT = 80;

// -----------------------------------------------------------------------------

const size_t N = 10;

// -----------------------------------------------------------------------------

template <class allocation_scheme>
class coreVMAllocatorRunner
{
public:
  coreVMAllocatorRunner()
    :
    m_allocator(CHUNK_SIZE * LOOP_COUNT)
  {
  }

  void run()
  {
    void* mem[LOOP_COUNT];

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      mem[i] = m_allocator.allocate(CHUNK_SIZE);
    }

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      m_allocator.deallocate(mem[i]);
    }
  }

  void run_bulk()
  {
    void* mem[LOOP_COUNT * N];
    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      mem[i] = m_allocator.allocate_n(N, CHUNK_SIZE);
    }

    for (size_t i = 0; i < LOOP_COUNT * N; ++i)
    {
      m_allocator.deallocate(mem[i]);
    }
  }

private:
  corevm::memory::allocator<allocation_scheme> m_allocator;
};

// -----------------------------------------------------------------------------

class StdAllocatorRunner
{
public:
  void run()
  {
    T* mem[LOOP_COUNT];

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      mem[i] = m_allocator.allocate(CHUNK_SIZE);
    }

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      m_allocator.deallocate(mem[i], CHUNK_SIZE);
    }
  }

private:
  std::allocator<T> m_allocator;
};

// -----------------------------------------------------------------------------

class MallocRunner
{
public:
  void run()
  {
    void* mem[LOOP_COUNT];

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      mem[i] = malloc(CHUNK_SIZE);
    }

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      free(mem[i]);
    }
  }

  void run_bulk()
  {
    void* mem[LOOP_COUNT];

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      mem[i] = calloc(N, CHUNK_SIZE);
    }

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      free(mem[i]);
    }
  }
};

// -----------------------------------------------------------------------------

/**
 * A simple Object Usage interface with Null Return.
 * Extends and generalizes the framework provided by the
 * Simple Segregated Storage solution.
 */
template <class UserAllocator>
class BoostPoolRunner
{
public:
  BoostPoolRunner()
    :
    m_allocator(LOOP_COUNT * CHUNK_SIZE)
  {
  }

  void run()
  {
    void* mem[LOOP_COUNT];

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      mem[i] = m_allocator.malloc();
    }

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      m_allocator.free(mem[i], CHUNK_SIZE);
    }
  }

private:
  boost::pool<UserAllocator> m_allocator;
};

// -----------------------------------------------------------------------------

/*
 * Singleton Usage interface with Exceptions. It is built on the singleton_pool
 * interface, and provides a Standard Allocator-compliant class
 * (for use in containers, etc.).
 *
 * http://www.boost.org/doc/libs/1_58_0/libs/pool/doc/html/boost_pool/pool/interfaces.html#boost_pool.pool.interfaces.interfaces.pool_allocator
 */
template <class UserAllocator>
class BoostPoolAllocatorRunner
{
public:
  void run()
  {
    T* mem[LOOP_COUNT];

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      mem[i] = m_allocator.allocate(CHUNK_SIZE);
    }

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      m_allocator.deallocate(mem[i], CHUNK_SIZE);
    }
  }

private:
  boost::pool_allocator<T, UserAllocator> m_allocator;
};

// -----------------------------------------------------------------------------

template <class UserAllocator>
class BoostFastPoolAllocatorRunner
{
public:
  void run()
  {
    T* mem[LOOP_COUNT];

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      mem[i] = m_allocator.allocate(CHUNK_SIZE);
    }

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      m_allocator.deallocate(mem[i], CHUNK_SIZE);
    }
  }

private:
  boost::fast_pool_allocator<T, UserAllocator> m_allocator;
};

// -----------------------------------------------------------------------------

/*
 * Object Usage interface with Null Return, but is aware of the type of the
 * object for which it is allocating chunks. On destruction, any chunks that
 * have been allocated from that object_pool will have their destructors called.
 *
 * http://www.boost.org/doc/libs/1_58_0/libs/pool/doc/html/boost_pool/pool/interfaces.html#boost_pool.pool.interfaces.interfaces.object_pool
 */
template <class UserAllocator>
class BoostObjectPoolRunner
{
public:
  void run()
  {
    T* mem[LOOP_COUNT];

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      mem[i] = m_allocator.malloc();
    }

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      m_allocator.free(mem[i]);
    }
  }

private:
  boost::object_pool<T, UserAllocator> m_allocator;
};

// -----------------------------------------------------------------------------

/**
 * A Singleton Usage interface with Null Return.
 * It's just the same as the pool interface but with Singleton Usage instead.
 *
 * http://www.boost.org/doc/libs/1_58_0/libs/pool/doc/html/boost/singleton_pool.html
 */
template <class UserAllocator>
class BoostSingletonPoolRunner
{
public:
  void run()
  {
    void* mem[LOOP_COUNT];

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      mem[i] = boost::singleton_pool<T, sizeof(T), UserAllocator>::malloc();
    }

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      boost::singleton_pool<T, sizeof(T), UserAllocator>::free(mem[i]);
    }
  }
};

// -----------------------------------------------------------------------------

class BoostSimpleSegregatedStorageRunner
{
public:
  void run()
  {
    std::vector<char> v(1024);

    T* mem[LOOP_COUNT];
    m_allocator.add_block(&v.front(), v.size(), sizeof(T) * 8);

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      mem[i] = (T*)m_allocator.malloc();
    }

    for (size_t i = 0; i < LOOP_COUNT; ++i)
    {
      m_allocator.free(mem[i]);
    }
  }

private:
  boost::simple_segregated_storage<std::size_t> m_allocator;
};

// -----------------------------------------------------------------------------

template <class runner_cls>
static void BenchmarkAllocator(benchmark::State& state)
{
  runner_cls runner;

  while (state.KeepRunning())
  {
    runner.run();
  }
}

// -----------------------------------------------------------------------------

template <class runner_cls>
static void BenchmarkAllocatorBulkAllocation(benchmark::State& state)
{
  runner_cls runner;

  while (state.KeepRunning())
  {
    runner.run_bulk();
  }
}

// -----------------------------------------------------------------------------

BENCHMARK_TEMPLATE(BenchmarkAllocator, MallocRunner);
BENCHMARK_TEMPLATE(BenchmarkAllocator, StdAllocatorRunner);
BENCHMARK_TEMPLATE(BenchmarkAllocator, BoostPoolRunner<boost::default_user_allocator_malloc_free>);
BENCHMARK_TEMPLATE(BenchmarkAllocator, BoostPoolRunner<boost::default_user_allocator_new_delete>);
BENCHMARK_TEMPLATE(BenchmarkAllocator, BoostPoolAllocatorRunner<boost::default_user_allocator_malloc_free>);
BENCHMARK_TEMPLATE(BenchmarkAllocator, BoostPoolAllocatorRunner<boost::default_user_allocator_new_delete>);
BENCHMARK_TEMPLATE(BenchmarkAllocator, BoostFastPoolAllocatorRunner<boost::default_user_allocator_malloc_free>);
BENCHMARK_TEMPLATE(BenchmarkAllocator, BoostFastPoolAllocatorRunner<boost::default_user_allocator_new_delete>);
BENCHMARK_TEMPLATE(BenchmarkAllocator, BoostObjectPoolRunner<boost::default_user_allocator_malloc_free>);
BENCHMARK_TEMPLATE(BenchmarkAllocator, BoostObjectPoolRunner<boost::default_user_allocator_new_delete>);
BENCHMARK_TEMPLATE(BenchmarkAllocator, BoostSingletonPoolRunner<boost::default_user_allocator_malloc_free>);
BENCHMARK_TEMPLATE(BenchmarkAllocator, BoostSingletonPoolRunner<boost::default_user_allocator_new_delete>);
BENCHMARK_TEMPLATE(BenchmarkAllocator, BoostSimpleSegregatedStorageRunner);
BENCHMARK_TEMPLATE(BenchmarkAllocator, coreVMAllocatorRunner<corevm::memory::first_fit_allocation_scheme>);
BENCHMARK_TEMPLATE(BenchmarkAllocator, coreVMAllocatorRunner<corevm::memory::next_fit_allocation_scheme>);
BENCHMARK_TEMPLATE(BenchmarkAllocator, coreVMAllocatorRunner<corevm::memory::best_fit_allocation_scheme>);
BENCHMARK_TEMPLATE(BenchmarkAllocator, coreVMAllocatorRunner<corevm::memory::worst_fit_allocation_scheme>);
BENCHMARK_TEMPLATE(BenchmarkAllocator, coreVMAllocatorRunner<corevm::memory::buddy_allocation_scheme>);

// -----------------------------------------------------------------------------

BENCHMARK_TEMPLATE(BenchmarkAllocatorBulkAllocation, MallocRunner);
BENCHMARK_TEMPLATE(BenchmarkAllocatorBulkAllocation, coreVMAllocatorRunner<corevm::memory::first_fit_allocation_scheme>);
BENCHMARK_TEMPLATE(BenchmarkAllocatorBulkAllocation, coreVMAllocatorRunner<corevm::memory::next_fit_allocation_scheme>);
BENCHMARK_TEMPLATE(BenchmarkAllocatorBulkAllocation, coreVMAllocatorRunner<corevm::memory::best_fit_allocation_scheme>);
BENCHMARK_TEMPLATE(BenchmarkAllocatorBulkAllocation, coreVMAllocatorRunner<corevm::memory::worst_fit_allocation_scheme>);

// -----------------------------------------------------------------------------
