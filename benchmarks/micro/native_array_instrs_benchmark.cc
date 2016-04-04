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

#include "runtime/instr.h"
#include "types/native_type_handle.h"
#include "types/types.h"

#include "instr_benchmarks_fixture.h"


// -----------------------------------------------------------------------------

using corevm::benchmarks::InstrBenchmarksFixture;

// -----------------------------------------------------------------------------

static
void BenchmarkInstrARYLEN(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeHandle hndl =
    corevm::types::native_array { 1, 2, 3 };

  corevm::runtime::instr_handler_arylen handler;
  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(hndl);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrARYEMP(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeHandle hndl =
    corevm::types::native_array { 1, 2, 3 };

  corevm::runtime::instr_handler_aryemp handler;
  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(hndl);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrARYAT(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeHandle hndl =
    corevm::types::native_array { 1, 2, 3 };

  corevm::types::NativeTypeHandle hndl2 =
    corevm::types::uint32(2);

  corevm::runtime::instr_handler_aryat handler;
  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(hndl2);
    frame->push_eval_stack(hndl);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrARYFRT(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeHandle hndl =
    corevm::types::native_array { 1, 2, 3 };

  corevm::runtime::instr_handler_aryfrt handler;
  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(hndl);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrARYBAK(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeHandle hndl =
    corevm::types::native_array { 1, 2, 3 };

  corevm::runtime::instr_handler_arybak handler;
  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(hndl);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrARYPUT(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeHandle hndl =
    corevm::types::native_array { 1, 2, 3 };

  corevm::types::NativeTypeHandle hndl2 =
    corevm::types::uint32(2);

  corevm::types::NativeTypeHandle hndl3 =
    corevm::types::uint32(10);

  corevm::runtime::instr_handler_aryput handler;
  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  frame->push_eval_stack(hndl3);
  frame->push_eval_stack(hndl2);
  frame->push_eval_stack(hndl);

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrARYAPND(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeHandle hndl =
    corevm::types::native_array { 1, 2, 3 };

  corevm::types::NativeTypeHandle hndl2 =
    corevm::types::uint32(2);

  corevm::runtime::instr_handler_aryapnd handler;
  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  frame->push_eval_stack(hndl2);
  frame->push_eval_stack(hndl);

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrARYERS(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeHandle hndl =
    corevm::types::native_array { 1, 2, 3 };

  corevm::types::NativeTypeHandle hndl2 =
    corevm::types::uint32(2);

  corevm::runtime::instr_handler_aryers handler;
  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(hndl2);
    frame->push_eval_stack(hndl);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrARYPOP(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeHandle hndl =
    corevm::types::native_array { 1, 2, 3 };

  corevm::runtime::instr_handler_arypop handler;
  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(hndl);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrARYSWP(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeHandle hndl =
    corevm::types::native_array { 1, 2, 3 };

  corevm::types::NativeTypeHandle hndl2 =
    corevm::types::native_array { 3, 2, 1 };

  corevm::runtime::instr_handler_aryswp handler;
  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  frame->push_eval_stack(hndl2);
  frame->push_eval_stack(hndl);

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrARYCLR(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeHandle hndl =
    corevm::types::native_array { 1, 2, 3 };

  corevm::runtime::instr_handler_aryclr handler;
  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  frame->push_eval_stack(hndl);

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

#ifdef BUILD_BENCHMARKS_STRICT
static
void BenchmarkInstrARYMRG(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeHandle hndl =
    corevm::types::native_array { 1, 2, 3 };

  corevm::types::NativeTypeHandle hndl2 =
    corevm::types::native_array { 4, 5, 6 };

  corevm::runtime::instr_handler_arymrg handler;
  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(hndl2);
    frame->push_eval_stack(hndl);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}
#endif

// -----------------------------------------------------------------------------

BENCHMARK(BenchmarkInstrARYLEN);
BENCHMARK(BenchmarkInstrARYEMP);
BENCHMARK(BenchmarkInstrARYAT);
BENCHMARK(BenchmarkInstrARYFRT);
BENCHMARK(BenchmarkInstrARYBAK);
BENCHMARK(BenchmarkInstrARYPUT);
BENCHMARK(BenchmarkInstrARYAPND);
BENCHMARK(BenchmarkInstrARYERS);
BENCHMARK(BenchmarkInstrARYPOP);
BENCHMARK(BenchmarkInstrARYSWP);
BENCHMARK(BenchmarkInstrARYCLR);
#ifdef BUILD_BENCHMARKS_STRICT
  BENCHMARK(BenchmarkInstrARYMRG);
#endif

// -----------------------------------------------------------------------------
