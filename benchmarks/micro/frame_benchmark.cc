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

#include "runtime/frame.h"
#include "types/native_type_handle.h"


// -----------------------------------------------------------------------------

static
void BenchmarkPushEvalStack(benchmark::State& state)
{
  corevm::runtime::closure_ctx ctx;

  corevm::runtime::frame frame(ctx, NULL, NULL);

  corevm::types::native_type_handle hndl =
    corevm::types::native_string("hello world");

  while (state.KeepRunning())
  {
    frame.push_eval_stack(hndl);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkPushEvalStack2(benchmark::State& state)
{
  corevm::runtime::closure_ctx ctx;

  corevm::runtime::frame frame(ctx, NULL, NULL);

  corevm::types::native_type_handle hndl =
    corevm::types::native_string("hello world");

  while (state.KeepRunning())
  {
    frame.push_eval_stack(std::move(hndl));
  }
}

// -----------------------------------------------------------------------------

BENCHMARK(BenchmarkPushEvalStack);
BENCHMARK(BenchmarkPushEvalStack2);

// -----------------------------------------------------------------------------
