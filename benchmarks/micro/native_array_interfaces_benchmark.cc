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
#include <benchmark/benchmark.h>

#include "types/interfaces.h"
#include "types/native_type_value.h"


// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceArraySize(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_array({ 1, 2, 3 })));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_array_size(oprd);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceArrayEmpty(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_array({ 1, 2, 3 })));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_array_empty(oprd);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceArrayAt(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_array({ 1, 2, 3 })));
  corevm::types::NativeTypeValue oprd2((2));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_array_at(oprd, oprd2);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceArrayFront(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_array({ 1, 2, 3 })));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_array_front(oprd);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceArrayBack(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_array({ 1, 2, 3 })));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_array_back(oprd);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceArrayPut(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_array({ 1, 2, 3 })));
  corevm::types::NativeTypeValue oprd2((2));
  corevm::types::NativeTypeValue oprd3((0));

  while (state.KeepRunning())
  {
    corevm::types::interface_array_put(oprd, oprd2, oprd3);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceArrayAppend(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_array({ 1, 2, 3 })));
  corevm::types::NativeTypeValue oprd2((4));

  while (state.KeepRunning())
  {
    corevm::types::interface_array_append(oprd, oprd2);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceArrayErase(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_array({ 1, 2, 3 })));
  corevm::types::NativeTypeValue oprd2((2));

  while (state.KeepRunning())
  {
    corevm::types::interface_array_erase(oprd, oprd2);
    corevm::types::interface_array_append(oprd, oprd2);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceArrayPop(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_array({ 1, 2, 3 })));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_array_pop(oprd);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceArraySwap(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_array({ 1, 2, 3 })));
  corevm::types::NativeTypeValue oprd2((corevm::types::native_array({ 4, 5, 6 })));

  while (state.KeepRunning())
  {
    corevm::types::interface_array_swap(oprd, oprd2);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceArrayClear(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_array({ 1, 2, 3 })));

  while (state.KeepRunning())
  {
    corevm::types::interface_array_clear(oprd);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceArrayMerge(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_array({ 1, 2, 3 })));
  corevm::types::NativeTypeValue oprd2((corevm::types::native_array({ 4, 5, 6 })));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_array_merge(oprd, oprd2);
  }
}

// -----------------------------------------------------------------------------

BENCHMARK(Benchmark_InterfaceArraySize);
BENCHMARK(Benchmark_InterfaceArrayEmpty);
BENCHMARK(Benchmark_InterfaceArrayAt);
BENCHMARK(Benchmark_InterfaceArrayFront);
BENCHMARK(Benchmark_InterfaceArrayBack);
BENCHMARK(Benchmark_InterfaceArrayPut);
BENCHMARK(Benchmark_InterfaceArrayAppend);
BENCHMARK(Benchmark_InterfaceArrayErase);
BENCHMARK(Benchmark_InterfaceArrayPop);
BENCHMARK(Benchmark_InterfaceArraySwap);
BENCHMARK(Benchmark_InterfaceArrayClear);
BENCHMARK(Benchmark_InterfaceArrayMerge);

// -----------------------------------------------------------------------------
