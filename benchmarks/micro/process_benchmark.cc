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

#include "runtime/process.h"
#include "types/native_type_handle.h"


// -----------------------------------------------------------------------------

static
void BenchmarkProcessCreateDyobj(benchmark::State& state)
{
  corevm::runtime::process process;

  while (state.KeepRunning())
  {
    process.create_dyobj();
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkProcessGetDyobj(benchmark::State& state)
{
  corevm::runtime::process process;

  auto id = process.create_dyobj();

  while (state.KeepRunning())
  {
    process.get_dyobj(id);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkProcessPushStack(benchmark::State& state)
{
  corevm::runtime::process process;

  corevm::dyobj::dyobj_id id = 1;

  while (state.KeepRunning())
  {
    process.push_stack(id);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkProcessInsertNtvHndl(benchmark::State& state)
{
  corevm::runtime::process process;

  corevm::types::native_type_handle hndl = corevm::types::string("Hello world");

  while (state.KeepRunning())
  {
    process.insert_ntvhndl(hndl);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkProcessInsertVector(benchmark::State& state)
{
  corevm::runtime::process process;

  // A vector of 100 instructions.
  corevm::runtime::vector vector {
    { .code=0xA, .oprd1=0xAA, .oprd2=0xAAA },
    { .code=0x1, .oprd1=0x11, .oprd2=0x111 },
    { .code=0x2, .oprd1=0x22, .oprd2=0x222 },
    { .code=0x3, .oprd1=0x33, .oprd2=0x333 },
    { .code=0x4, .oprd1=0x44, .oprd2=0x444 },
    { .code=0x5, .oprd1=0x55, .oprd2=0x555 },
    { .code=0x6, .oprd1=0x66, .oprd2=0x666 },
    { .code=0x7, .oprd1=0x77, .oprd2=0x777 },
    { .code=0x8, .oprd1=0x88, .oprd2=0x888 },
    { .code=0x9, .oprd1=0x99, .oprd2=0x999 },
    { .code=0xA, .oprd1=0xAA, .oprd2=0xAAA },
    { .code=0x1, .oprd1=0x11, .oprd2=0x111 },
    { .code=0x2, .oprd1=0x22, .oprd2=0x222 },
    { .code=0x3, .oprd1=0x33, .oprd2=0x333 },
    { .code=0x4, .oprd1=0x44, .oprd2=0x444 },
    { .code=0x5, .oprd1=0x55, .oprd2=0x555 },
    { .code=0x6, .oprd1=0x66, .oprd2=0x666 },
    { .code=0x7, .oprd1=0x77, .oprd2=0x777 },
    { .code=0x8, .oprd1=0x88, .oprd2=0x888 },
    { .code=0x9, .oprd1=0x99, .oprd2=0x999 },
    { .code=0xA, .oprd1=0xAA, .oprd2=0xAAA },
    { .code=0x1, .oprd1=0x11, .oprd2=0x111 },
    { .code=0x2, .oprd1=0x22, .oprd2=0x222 },
    { .code=0x3, .oprd1=0x33, .oprd2=0x333 },
    { .code=0x4, .oprd1=0x44, .oprd2=0x444 },
    { .code=0x5, .oprd1=0x55, .oprd2=0x555 },
    { .code=0x6, .oprd1=0x66, .oprd2=0x666 },
    { .code=0x7, .oprd1=0x77, .oprd2=0x777 },
    { .code=0x8, .oprd1=0x88, .oprd2=0x888 },
    { .code=0x9, .oprd1=0x99, .oprd2=0x999 },
    { .code=0xA, .oprd1=0xAA, .oprd2=0xAAA },
    { .code=0x1, .oprd1=0x11, .oprd2=0x111 },
    { .code=0x2, .oprd1=0x22, .oprd2=0x222 },
    { .code=0x3, .oprd1=0x33, .oprd2=0x333 },
    { .code=0x4, .oprd1=0x44, .oprd2=0x444 },
    { .code=0x5, .oprd1=0x55, .oprd2=0x555 },
    { .code=0x6, .oprd1=0x66, .oprd2=0x666 },
    { .code=0x7, .oprd1=0x77, .oprd2=0x777 },
    { .code=0x8, .oprd1=0x88, .oprd2=0x888 },
    { .code=0x9, .oprd1=0x99, .oprd2=0x999 },
    { .code=0xA, .oprd1=0xAA, .oprd2=0xAAA },
    { .code=0x1, .oprd1=0x11, .oprd2=0x111 },
    { .code=0x2, .oprd1=0x22, .oprd2=0x222 },
    { .code=0x3, .oprd1=0x33, .oprd2=0x333 },
    { .code=0x4, .oprd1=0x44, .oprd2=0x444 },
    { .code=0x5, .oprd1=0x55, .oprd2=0x555 },
    { .code=0x6, .oprd1=0x66, .oprd2=0x666 },
    { .code=0x7, .oprd1=0x77, .oprd2=0x777 },
    { .code=0x8, .oprd1=0x88, .oprd2=0x888 },
    { .code=0x9, .oprd1=0x99, .oprd2=0x999 },
    { .code=0xA, .oprd1=0xAA, .oprd2=0xAAA },
    { .code=0x1, .oprd1=0x11, .oprd2=0x111 },
    { .code=0x2, .oprd1=0x22, .oprd2=0x222 },
    { .code=0x3, .oprd1=0x33, .oprd2=0x333 },
    { .code=0x4, .oprd1=0x44, .oprd2=0x444 },
    { .code=0x5, .oprd1=0x55, .oprd2=0x555 },
    { .code=0x6, .oprd1=0x66, .oprd2=0x666 },
    { .code=0x7, .oprd1=0x77, .oprd2=0x777 },
    { .code=0x8, .oprd1=0x88, .oprd2=0x888 },
    { .code=0x9, .oprd1=0x99, .oprd2=0x999 },
    { .code=0xA, .oprd1=0xAA, .oprd2=0xAAA },
    { .code=0x1, .oprd1=0x11, .oprd2=0x111 },
    { .code=0x2, .oprd1=0x22, .oprd2=0x222 },
    { .code=0x3, .oprd1=0x33, .oprd2=0x333 },
    { .code=0x4, .oprd1=0x44, .oprd2=0x444 },
    { .code=0x5, .oprd1=0x55, .oprd2=0x555 },
    { .code=0x6, .oprd1=0x66, .oprd2=0x666 },
    { .code=0x7, .oprd1=0x77, .oprd2=0x777 },
    { .code=0x8, .oprd1=0x88, .oprd2=0x888 },
    { .code=0x9, .oprd1=0x99, .oprd2=0x999 },
    { .code=0xA, .oprd1=0xAA, .oprd2=0xAAA },
    { .code=0x1, .oprd1=0x11, .oprd2=0x111 },
    { .code=0x2, .oprd1=0x22, .oprd2=0x222 },
    { .code=0x3, .oprd1=0x33, .oprd2=0x333 },
    { .code=0x4, .oprd1=0x44, .oprd2=0x444 },
    { .code=0x5, .oprd1=0x55, .oprd2=0x555 },
    { .code=0x6, .oprd1=0x66, .oprd2=0x666 },
    { .code=0x7, .oprd1=0x77, .oprd2=0x777 },
    { .code=0x8, .oprd1=0x88, .oprd2=0x888 },
    { .code=0x9, .oprd1=0x99, .oprd2=0x999 },
    { .code=0xA, .oprd1=0xAA, .oprd2=0xAAA },
    { .code=0x1, .oprd1=0x11, .oprd2=0x111 },
    { .code=0x2, .oprd1=0x22, .oprd2=0x222 },
    { .code=0x3, .oprd1=0x33, .oprd2=0x333 },
    { .code=0x4, .oprd1=0x44, .oprd2=0x444 },
    { .code=0x5, .oprd1=0x55, .oprd2=0x555 },
    { .code=0x6, .oprd1=0x66, .oprd2=0x666 },
    { .code=0x7, .oprd1=0x77, .oprd2=0x777 },
    { .code=0x8, .oprd1=0x88, .oprd2=0x888 },
    { .code=0x9, .oprd1=0x99, .oprd2=0x999 },
    { .code=0xA, .oprd1=0xAA, .oprd2=0xAAA },
    { .code=0x1, .oprd1=0x11, .oprd2=0x111 },
    { .code=0x2, .oprd1=0x22, .oprd2=0x222 },
    { .code=0x3, .oprd1=0x33, .oprd2=0x333 },
    { .code=0x4, .oprd1=0x44, .oprd2=0x444 },
    { .code=0x5, .oprd1=0x55, .oprd2=0x555 },
    { .code=0x6, .oprd1=0x66, .oprd2=0x666 },
    { .code=0x7, .oprd1=0x77, .oprd2=0x777 },
    { .code=0x8, .oprd1=0x88, .oprd2=0x888 },
    { .code=0x9, .oprd1=0x99, .oprd2=0x999 },
  };

  while (state.KeepRunning())
  {
    process.insert_vector(vector);
  }
}

// -----------------------------------------------------------------------------

BENCHMARK(BenchmarkProcessCreateDyobj);
BENCHMARK(BenchmarkProcessGetDyobj);
BENCHMARK(BenchmarkProcessPushStack);
BENCHMARK(BenchmarkProcessInsertNtvHndl);
BENCHMARK(BenchmarkProcessInsertVector);

// -----------------------------------------------------------------------------
