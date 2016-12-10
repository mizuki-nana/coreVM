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

#include "runtime/frame.h"
#include "types/native_type_value.h"


// -----------------------------------------------------------------------------

static
void BenchmarkPushEvalStack(benchmark::State& state)
{
  corevm::runtime::ClosureCtx ctx(corevm::runtime::NONESET_COMPARTMENT_ID,
    corevm::runtime::NONESET_CLOSURE_ID);

  corevm::runtime::Frame frame(ctx, NULL, NULL);

  corevm::types::NativeTypeValue type_val =
    corevm::types::native_string("hello world");

  while (state.KeepRunning())
  {
    frame.push_eval_stack(type_val);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkPushEvalStack2(benchmark::State& state)
{
  corevm::runtime::ClosureCtx ctx(corevm::runtime::NONESET_COMPARTMENT_ID,
    corevm::runtime::NONESET_CLOSURE_ID);

  corevm::runtime::Frame frame(ctx, NULL, NULL);

  corevm::types::NativeTypeValue type_val =
    corevm::types::native_string("hello world");

  while (state.KeepRunning())
  {
    frame.push_eval_stack(std::move(type_val));
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkPopEvalStack(benchmark::State& state)
{
  corevm::runtime::ClosureCtx ctx(corevm::runtime::NONESET_COMPARTMENT_ID,
    corevm::runtime::NONESET_CLOSURE_ID);

  corevm::runtime::Frame frame(ctx, NULL, NULL);

  corevm::types::NativeTypeValue type_val =
    corevm::types::native_string("hello world");

  for (size_t i = 0; i < state.max_iterations; ++i)
  {
    frame.push_eval_stack(type_val);
  }

  while (state.KeepRunning())
  {
    frame.pop_eval_stack();
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkGetVisibleVariable(benchmark::State& state)
{
  corevm::runtime::ClosureCtx ctx(corevm::runtime::NONESET_COMPARTMENT_ID,
    corevm::runtime::NONESET_CLOSURE_ID);

  corevm::runtime::Frame frame(ctx, NULL, NULL);

  corevm::runtime::variable_key_t key = 1;
  corevm::runtime::RuntimeTypes::dynamic_object_type obj;

  frame.set_visible_var(key, &obj);

  while (state.KeepRunning())
  {
    frame.get_visible_var(key);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkSwapEvalStack(benchmark::State& state)
{
  corevm::runtime::ClosureCtx ctx(corevm::runtime::NONESET_COMPARTMENT_ID,
    corevm::runtime::NONESET_CLOSURE_ID);

  corevm::runtime::Frame frame(ctx, NULL, NULL);

  corevm::types::NativeTypeValue type_val1((corevm::types::native_string("Hello world I'm FAT")));
  corevm::types::NativeTypeValue type_val2((corevm::types::native_map({
    { 1, 10 },
    { 2, 20 },
    { 3, 30 },
    { 4, 40 },
    { 5, 50 },
    { 6, 60 },
    { 7, 70 },
    { 8, 80 },
    { 9, 90 },
    { 10, 100 }
  })));

  frame.push_eval_stack(type_val1);
  frame.push_eval_stack(type_val2);

  while (state.KeepRunning())
  {
      frame.swap_eval_stack();
  }
}

// -----------------------------------------------------------------------------

BENCHMARK(BenchmarkPushEvalStack);
BENCHMARK(BenchmarkPushEvalStack2);
BENCHMARK(BenchmarkPopEvalStack);
BENCHMARK(BenchmarkGetVisibleVariable);
BENCHMARK(BenchmarkSwapEvalStack);

// -----------------------------------------------------------------------------
