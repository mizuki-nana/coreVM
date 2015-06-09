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

BENCHMARK(BenchmarkProcessCreateDyobj);
BENCHMARK(BenchmarkProcessGetDyobj);
BENCHMARK(BenchmarkProcessPushStack);
BENCHMARK(BenchmarkProcessInsertNtvHndl);

// -----------------------------------------------------------------------------
