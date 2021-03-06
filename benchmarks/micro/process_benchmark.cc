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

#include "corevm/macros.h"
#include "runtime/process.h"
#include "types/native_type_value.h"


// -----------------------------------------------------------------------------

#if !COREVM_USE_SMALL_ATTRIBUTE_TABLE
static
void BenchmarkProcessCreateDyobj(benchmark::State& state)
{
  corevm::runtime::Process::Options opts;

  opts.heap_alloc_size = 1024 * 1024 * 512;
  opts.pool_alloc_size = 1024 * 1024 * 512;

  corevm::runtime::Process process(opts);

  while (state.KeepRunning())
  {
    process.create_dyobj();
  }
}
#endif

// -----------------------------------------------------------------------------

static
void BenchmarkProcessGetDyobj(benchmark::State& state)
{
  corevm::runtime::Process process;

  auto obj = process.create_dyobj();
  auto id = obj->id();

  while (state.KeepRunning())
  {
    process.get_dyobj(id);
  }
}

// -----------------------------------------------------------------------------

#ifdef BUILD_BENCHMARKS_STRICT
static
void BenchmarkProcessPushStack(benchmark::State& state)
{
  corevm::runtime::Process process;

  auto obj = process.create_dyobj();

  while (state.KeepRunning())
  {
    process.push_stack(obj);
  }
}
#endif

// -----------------------------------------------------------------------------

#ifdef BUILD_BENCHMARKS_STRICT
static
void BenchmarkProcessInsertTypeValue(benchmark::State& state)
{
  corevm::runtime::Process::Options opts;

  opts.heap_alloc_size = 1024;
  opts.pool_alloc_size = 1024 * 1024 * 1024;

  corevm::runtime::Process process(opts);

  corevm::types::NativeTypeValue type_val = corevm::types::string("Hello world");

  while (state.KeepRunning())
  {
    process.insert_type_value(type_val);
  }
}
#endif

// -----------------------------------------------------------------------------

static
void BenchmarkProcessGetTypeValue(benchmark::State& state)
{
  corevm::runtime::Process process;

  corevm::types::NativeTypeValue type_val = corevm::types::string("Hello world");
  auto key = process.insert_type_value(type_val);

  while (state.KeepRunning())
  {
    process.get_type_value(key);
  }
}

// -----------------------------------------------------------------------------

#if !COREVM_USE_SMALL_ATTRIBUTE_TABLE
BENCHMARK(BenchmarkProcessCreateDyobj);
#endif
BENCHMARK(BenchmarkProcessGetDyobj);
BENCHMARK(BenchmarkProcessGetTypeValue);
#ifdef BUILD_BENCHMARKS_STRICT
BENCHMARK(BenchmarkProcessPushStack);
BENCHMARK(BenchmarkProcessInsertTypeValue);
#endif

// -----------------------------------------------------------------------------
