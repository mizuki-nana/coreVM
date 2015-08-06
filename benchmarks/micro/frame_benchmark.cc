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

static
void BenchmarkPopEvalStack(benchmark::State& state)
{
  corevm::runtime::closure_ctx ctx;

  corevm::runtime::frame frame(ctx, NULL, NULL);

  corevm::types::native_type_handle hndl =
    corevm::types::native_string("hello world");

  for (size_t i = 0; i < state.max_iterations; ++i)
  {
    frame.push_eval_stack(hndl);
  }

  while (state.KeepRunning())
  {
    frame.pop_eval_stack();
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkGetVisibleVariable1(benchmark::State& state)
{
  corevm::runtime::closure_ctx ctx;

  corevm::runtime::frame frame(ctx, NULL, NULL);

  corevm::runtime::variable_key key = 1;
  corevm::dyobj::dyobj_id id = 2;

  frame.set_visible_var(key, id);

  while (state.KeepRunning())
  {
    frame.get_visible_var(key);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkGetVisibleVariable2(benchmark::State& state)
{
  /**
   * Similar to `BenchmarkGetVisibleVariable1` above,
   * this benchmark demonstrates the speed of fetching a variable
   * when traversing the frames hierarchy.
   *
   * NOTE: based on the results observed, it shows that the run time
   * grows linearly as the number of frames have to traverse grows.
   */
  corevm::runtime::closure_ctx ctx;

  corevm::runtime::frame frame1(ctx, NULL, NULL);
  corevm::runtime::frame frame2(ctx, NULL, NULL);
  corevm::runtime::frame frame3(ctx, NULL, NULL);

  frame1.set_parent(&frame2);
  frame2.set_parent(&frame3);

  corevm::runtime::variable_key key = 1;
  corevm::dyobj::dyobj_id id = 2;

  frame1.set_visible_var(key + 1, id);
  frame2.set_visible_var(key + 2, id);
  frame3.set_visible_var(key, id);

  while (state.KeepRunning())
  {
    corevm::runtime::frame* frame = &frame1;

    while (!frame->has_visible_var(key))
    {
      frame = frame->parent();
    }

    frame->get_visible_var(key);
  }
}

// -----------------------------------------------------------------------------

BENCHMARK(BenchmarkPushEvalStack);
BENCHMARK(BenchmarkPushEvalStack2);
BENCHMARK(BenchmarkPopEvalStack);
BENCHMARK(BenchmarkGetVisibleVariable1);
BENCHMARK(BenchmarkGetVisibleVariable2);

// -----------------------------------------------------------------------------
