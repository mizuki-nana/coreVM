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

#include "memory/object_container.h"
#include "memory/sequential_allocation_scheme.h"
#include "dyobj/heap_allocator.h"

#include <memory>


// -----------------------------------------------------------------------------

typedef uint32_t T;

// -----------------------------------------------------------------------------

static
void BenchmarkObjectContainerAllocateAndConstructObject(benchmark::State& state)
{
  std::allocator<T> allocator;
  T val = 123;

  while (state.KeepRunning())
  {
    T* p = allocator.allocate(sizeof(T));
    allocator.construct(p, val);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkObjectContainerWithStdAllocator(benchmark::State& state)
{
  corevm::memory::object_container<T, std::allocator<T>> object_container;

  while (state.KeepRunning())
  {
    object_container.create();
  }
}

// -----------------------------------------------------------------------------

template <class AllocatorType>
static void BenchmarkObjectContainer(benchmark::State& state)
{
  corevm::memory::object_container<T, AllocatorType> object_container(1000000000);

  while (state.KeepRunning())
  {
    object_container.create();
  }
}

// -----------------------------------------------------------------------------

BENCHMARK(BenchmarkObjectContainerAllocateAndConstructObject);
BENCHMARK(BenchmarkObjectContainerWithStdAllocator);
BENCHMARK_TEMPLATE(BenchmarkObjectContainer, corevm::dyobj::heap_allocator<T, corevm::memory::next_fit_allocation_scheme>);

// -----------------------------------------------------------------------------
