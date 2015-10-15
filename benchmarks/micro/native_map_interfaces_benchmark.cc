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

#include "types/interfaces.h"
#include "types/native_type_handle.h"


// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceMapSize(benchmark::State& state)
{
  corevm::types::native_type_handle hndl((corevm::types::native_map({
    { 1, 10 },
    { 2, 20 },
    { 3, 30 }
  })));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_map_size(hndl);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceMapEmpty(benchmark::State& state)
{
  corevm::types::native_type_handle hndl((corevm::types::native_map({
    { 1, 10 },
    { 2, 20 },
    { 3, 30 }
  })));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_map_empty(hndl);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceMapFind(benchmark::State& state)
{
  corevm::types::native_type_handle hndl((corevm::types::native_map({
    { 1, 10 },
    { 2, 20 },
    { 3, 30 }
  })));
  corevm::types::native_type_handle oprd2((corevm::types::uint32(2)));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_map_find(hndl, oprd2);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceMapAt(benchmark::State& state)
{
  corevm::types::native_type_handle hndl((corevm::types::native_map({
    { 1, 10 },
    { 2, 20 },
    { 3, 30 }
  })));
  corevm::types::native_type_handle oprd2((corevm::types::uint32(2)));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_map_at(hndl, oprd2);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceMapPut(benchmark::State& state)
{
  corevm::types::native_type_handle hndl((corevm::types::native_map({
    { 1, 10 },
    { 2, 20 },
    { 3, 30 }
  })));
  corevm::types::native_type_handle oprd2((corevm::types::uint32(2)));
  corevm::types::native_type_handle oprd3((corevm::types::uint32(200)));

  while (state.KeepRunning())
  {
    corevm::types::interface_map_put(hndl, oprd2, oprd3);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceMapErase(benchmark::State& state)
{
  corevm::types::native_type_handle hndl((corevm::types::native_map({
    { 1, 10 },
    { 2, 20 },
    { 3, 30 }
  })));
  corevm::types::native_type_handle oprd2((corevm::types::uint32(2)));

  while (state.KeepRunning())
  {
    corevm::types::interface_map_erase(hndl, oprd2);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceMapClear(benchmark::State& state)
{
  corevm::types::native_type_handle hndl((corevm::types::native_map({
    { 1, 10 },
    { 2, 20 },
    { 3, 30 }
  })));

  while (state.KeepRunning())
  {
    corevm::types::interface_map_clear(hndl);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceMapSwap(benchmark::State& state)
{
  corevm::types::native_type_handle hndl((corevm::types::native_map({
    { 1, 10 },
    { 2, 20 },
    { 3, 30 }
  })));

  corevm::types::native_type_handle oprd2((corevm::types::native_map({
    { 100, 1 },
    { 200, 2 },
    { 300, 3 }
  })));

  while (state.KeepRunning())
  {
    corevm::types::interface_map_swap(hndl, oprd2);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceMapKeys(benchmark::State& state)
{
  corevm::types::native_type_handle hndl((corevm::types::native_map({
    { 1, 10 },
    { 2, 20 },
    { 3, 30 }
  })));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_map_keys(hndl);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceMapVals(benchmark::State& state)
{
  corevm::types::native_type_handle hndl((corevm::types::native_map({
    { 1, 10 },
    { 2, 20 },
    { 3, 30 }
  })));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_map_vals(hndl);
  }
}

// -----------------------------------------------------------------------------

BENCHMARK(Benchmark_InterfaceMapSize);
BENCHMARK(Benchmark_InterfaceMapEmpty);
BENCHMARK(Benchmark_InterfaceMapFind);
BENCHMARK(Benchmark_InterfaceMapAt);
BENCHMARK(Benchmark_InterfaceMapPut);
BENCHMARK(Benchmark_InterfaceMapErase);
BENCHMARK(Benchmark_InterfaceMapClear);
BENCHMARK(Benchmark_InterfaceMapSwap);
BENCHMARK(Benchmark_InterfaceMapKeys);
BENCHMARK(Benchmark_InterfaceMapVals);

// -----------------------------------------------------------------------------
