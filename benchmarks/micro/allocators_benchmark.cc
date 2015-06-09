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

#include <memory>

// -----------------------------------------------------------------------------

const size_t CHUNK_SIZE = 8;

// -----------------------------------------------------------------------------

template <class allocation_scheme>
class coreVMAllocatorRunner
{
public:
  coreVMAllocatorRunner()
    :
    m_allocator(80000000)
  {
  }

  void run()
  {
    m_allocator.allocate(CHUNK_SIZE);
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
    m_allocator.allocate(CHUNK_SIZE);
  }

private:
  std::allocator<char> m_allocator;
};

// -----------------------------------------------------------------------------

class MallocRunner
{
public:
  void run()
  {
    malloc(CHUNK_SIZE);
  }
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

BENCHMARK_TEMPLATE(BenchmarkAllocator, MallocRunner);
BENCHMARK_TEMPLATE(BenchmarkAllocator, StdAllocatorRunner);
BENCHMARK_TEMPLATE(BenchmarkAllocator, coreVMAllocatorRunner<corevm::memory::first_fit_allocation_scheme>);
BENCHMARK_TEMPLATE(BenchmarkAllocator, coreVMAllocatorRunner<corevm::memory::next_fit_allocation_scheme>);
BENCHMARK_TEMPLATE(BenchmarkAllocator, coreVMAllocatorRunner<corevm::memory::best_fit_allocation_scheme>);
BENCHMARK_TEMPLATE(BenchmarkAllocator, coreVMAllocatorRunner<corevm::memory::worst_fit_allocation_scheme>);
BENCHMARK_TEMPLATE(BenchmarkAllocator, coreVMAllocatorRunner<corevm::memory::buddy_allocation_scheme>);

// -----------------------------------------------------------------------------
