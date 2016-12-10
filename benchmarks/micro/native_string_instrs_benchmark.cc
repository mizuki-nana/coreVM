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

#include "runtime/instr.h"
#include "types/native_type_value.h"
#include "types/types.h"

#include "instr_benchmarks_fixture.h"


// -----------------------------------------------------------------------------

using corevm::benchmarks::InstrBenchmarksFixture;

// -----------------------------------------------------------------------------

static
void BenchmarkInstrSTRLEN(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeValue oprd =
    corevm::types::native_string("Hello world");

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(oprd);

    corevm::runtime::instr_handler_strlen(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrSTRCLR(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeValue oprd =
    corevm::types::native_string("Hello world");

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  frame->push_eval_stack(oprd);

  while (state.KeepRunning())
  {
    corevm::runtime::instr_handler_strclr(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrSTRAPD(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeValue oprd =
    corevm::types::native_string("Hello world");

  corevm::types::NativeTypeValue oprd2 =
    corevm::types::native_string("!!!");

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  frame->push_eval_stack(oprd2);
  frame->push_eval_stack(oprd);

  while (state.KeepRunning())
  {
    corevm::runtime::instr_handler_strapd(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrSTRPSH(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeValue oprd =
    corevm::types::native_string("Hello world");

  corevm::types::NativeTypeValue oprd2 =
    corevm::types::uint8(8);

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  frame->push_eval_stack(oprd2);
  frame->push_eval_stack(oprd);

  while (state.KeepRunning())
  {
    corevm::runtime::instr_handler_strpsh(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrSTRIST(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeValue oprd =
    corevm::types::native_string("Helloworld");

  corevm::types::NativeTypeValue oprd2 =
    corevm::types::uint8(4);

  corevm::types::NativeTypeValue oprd3 =
    corevm::types::native_string(" ");

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  frame->push_eval_stack(oprd3);
  frame->push_eval_stack(oprd2);
  frame->push_eval_stack(oprd);

  while (state.KeepRunning())
  {
    corevm::runtime::instr_handler_strist(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrSTRIST2(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeValue oprd =
    corevm::types::native_string("Hello world");

  corevm::types::NativeTypeValue oprd2 =
    corevm::types::uint8(10);

  corevm::types::NativeTypeValue oprd3 =
    corevm::types::uint8('!');

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  frame->push_eval_stack(oprd3);
  frame->push_eval_stack(oprd2);
  frame->push_eval_stack(oprd);

  while (state.KeepRunning())
  {
    corevm::runtime::instr_handler_strist2(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrSTRERS(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeValue oprd =
    corevm::types::native_string("Hello  world");

  corevm::types::NativeTypeValue oprd2 =
    corevm::types::uint8(4);

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  frame->push_eval_stack(oprd2);
  frame->push_eval_stack(oprd);

  while (state.KeepRunning())
  {
    corevm::runtime::instr_handler_strers(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

#ifdef BUILD_BENCHMARKS_STRICT
static
void BenchmarkInstrSTRERS2(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeValue oprd =
    corevm::types::native_string("Hello  world");

  corevm::types::NativeTypeValue oprd2 =
    corevm::types::uint8(4);

  corevm::types::NativeTypeValue oprd3 =
    corevm::types::uint8(1);

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(oprd3);
    frame->push_eval_stack(oprd2);
    frame->push_eval_stack(oprd);

    corevm::runtime::instr_handler_strers2(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}
#endif

// -----------------------------------------------------------------------------

static
void BenchmarkInstrSTRRPLC(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeValue oprd =
    corevm::types::native_string("Hello  world");

  corevm::types::NativeTypeValue oprd2 =
    corevm::types::uint8(4);

  corevm::types::NativeTypeValue oprd3 =
    corevm::types::uint8(1);

  corevm::types::NativeTypeValue oprd4 =
    corevm::types::native_string(" ");

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  frame->push_eval_stack(oprd4);
  frame->push_eval_stack(oprd3);
  frame->push_eval_stack(oprd2);
  frame->push_eval_stack(oprd);

  while (state.KeepRunning())
  {
    corevm::runtime::instr_handler_strrplc(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrSTRSWP(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeValue oprd =
    corevm::types::native_string("Hello world");

  corevm::types::NativeTypeValue oprd2 =
    corevm::types::native_string("Greetings!");

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  frame->push_eval_stack(oprd2);
  frame->push_eval_stack(oprd);

  while (state.KeepRunning())
  {
    corevm::runtime::instr_handler_strswp(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrSTRSUB(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeValue oprd =
    corevm::types::native_string("Hello world");

  corevm::types::NativeTypeValue oprd2 =
    corevm::types::uint8(4);

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(oprd2);
    frame->push_eval_stack(oprd);

    corevm::runtime::instr_handler_strsub(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

#ifdef BUILD_BENCHMARKS_STRICT
static
void BenchmarkInstrSTRSUB2(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeValue oprd =
    corevm::types::native_string("Hello world");

  corevm::types::NativeTypeValue oprd2 =
    corevm::types::uint8(4);

  corevm::types::NativeTypeValue oprd3 =
    corevm::types::uint8(5);

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(oprd3);
    frame->push_eval_stack(oprd2);
    frame->push_eval_stack(oprd);

    corevm::runtime::instr_handler_strsub2(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}
#endif

// -----------------------------------------------------------------------------

#ifdef BUILD_BENCHMARKS_STRICT
static
void BenchmarkInstrSTRFND(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeValue oprd =
    corevm::types::native_string("Hello world");

  corevm::types::NativeTypeValue oprd2 =
    corevm::types::native_string("world");

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(oprd2);
    frame->push_eval_stack(oprd);

    corevm::runtime::instr_handler_strfnd(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}
#endif

// -----------------------------------------------------------------------------

#ifdef BUILD_BENCHMARKS_STRICT
static
void BenchmarkInstrSTRFND2(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeValue oprd =
    corevm::types::native_string("Hello world");

  corevm::types::NativeTypeValue oprd2 =
    corevm::types::native_string("world");

  corevm::types::NativeTypeValue oprd3 =
    corevm::types::uint8(4);

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(oprd3);
    frame->push_eval_stack(oprd2);
    frame->push_eval_stack(oprd);

    corevm::runtime::instr_handler_strfnd2(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}
#endif

// -----------------------------------------------------------------------------

#ifdef BUILD_BENCHMARKS_STRICT
static
void BenchmarkInstrSTRRFND(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeValue oprd =
    corevm::types::native_string("Hello world");

  corevm::types::NativeTypeValue oprd2 =
    corevm::types::native_string("world");

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(oprd2);
    frame->push_eval_stack(oprd);

    corevm::runtime::instr_handler_strrfnd(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}
#endif

// -----------------------------------------------------------------------------

#ifdef BUILD_BENCHMARKS_STRICT
static
void BenchmarkInstrSTRRFND2(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::types::NativeTypeValue oprd =
    corevm::types::native_string("Hello world");

  corevm::types::NativeTypeValue oprd2 =
    corevm::types::native_string("hello");

  corevm::types::NativeTypeValue oprd3 =
    corevm::types::uint8(4);

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(oprd3);
    frame->push_eval_stack(oprd2);
    frame->push_eval_stack(oprd);

    corevm::runtime::instr_handler_strrfnd2(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}
#endif

// -----------------------------------------------------------------------------

BENCHMARK(BenchmarkInstrSTRLEN);
BENCHMARK(BenchmarkInstrSTRCLR);
BENCHMARK(BenchmarkInstrSTRAPD);
BENCHMARK(BenchmarkInstrSTRPSH);
BENCHMARK(BenchmarkInstrSTRIST);
BENCHMARK(BenchmarkInstrSTRIST2);
BENCHMARK(BenchmarkInstrSTRERS);
BENCHMARK(BenchmarkInstrSTRRPLC);
BENCHMARK(BenchmarkInstrSTRSWP);
BENCHMARK(BenchmarkInstrSTRSUB);

#ifdef BUILD_BENCHMARKS_STRICT
  BENCHMARK(BenchmarkInstrSTRERS2);
  BENCHMARK(BenchmarkInstrSTRSUB2);
  BENCHMARK(BenchmarkInstrSTRFND);
  BENCHMARK(BenchmarkInstrSTRFND2);
  BENCHMARK(BenchmarkInstrSTRRFND);
  BENCHMARK(BenchmarkInstrSTRRFND2);
#endif

// -----------------------------------------------------------------------------
