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

using corevm::benchmarks::instr_benchmarks_fixture;

// -----------------------------------------------------------------------------

static
void BenchmarkInstrSTRLEN(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl =
    corevm::types::native_string("Hello world");

  corevm::runtime::instr_handler_strlen handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

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
void BenchmarkInstrSTRCLR(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl =
    corevm::types::native_string("Hello world");

  corevm::runtime::instr_handler_strclr handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  frame->push_eval_stack(hndl);

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrSTRAPD(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl =
    corevm::types::native_string("Hello world");

  corevm::types::native_type_handle hndl2 =
    corevm::types::native_string("!!!");

  corevm::runtime::instr_handler_strapd handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

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
void BenchmarkInstrSTRPSH(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl =
    corevm::types::native_string("Hello world");

  corevm::types::native_type_handle hndl2 =
    corevm::types::uint8(8);

  corevm::runtime::instr_handler_strpsh handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

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
void BenchmarkInstrSTRIST(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl =
    corevm::types::native_string("Helloworld");

  corevm::types::native_type_handle hndl2 =
    corevm::types::uint8(4);

  corevm::types::native_type_handle hndl3 =
    corevm::types::native_string(" ");

  corevm::runtime::instr_handler_strist handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

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
void BenchmarkInstrSTRIST2(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl =
    corevm::types::native_string("Hello world");

  corevm::types::native_type_handle hndl2 =
    corevm::types::uint8(10);

  corevm::types::native_type_handle hndl3 =
    corevm::types::uint8('!');

  corevm::runtime::instr_handler_strist2 handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

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
void BenchmarkInstrSTRERS(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl =
    corevm::types::native_string("Hello  world");

  corevm::types::native_type_handle hndl2 =
    corevm::types::uint8(4);

  corevm::runtime::instr_handler_strers handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

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
void BenchmarkInstrSTRERS2(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl =
    corevm::types::native_string("Hello  world");

  corevm::types::native_type_handle hndl2 =
    corevm::types::uint8(4);

  corevm::types::native_type_handle hndl3 =
    corevm::types::uint8(1);

  corevm::runtime::instr_handler_strers2 handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(hndl3);
    frame->push_eval_stack(hndl2);
    frame->push_eval_stack(hndl);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrSTRRPLC(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl =
    corevm::types::native_string("Hello  world");

  corevm::types::native_type_handle hndl2 =
    corevm::types::uint8(4);

  corevm::types::native_type_handle hndl3 =
    corevm::types::uint8(1);

  corevm::types::native_type_handle hndl4 =
    corevm::types::native_string(" ");

  corevm::runtime::instr_handler_strrplc handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  frame->push_eval_stack(hndl4);
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
void BenchmarkInstrSTRSWP(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl =
    corevm::types::native_string("Hello world");

  corevm::types::native_type_handle hndl2 =
    corevm::types::native_string("Greetings!");

  corevm::runtime::instr_handler_strswp handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

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
void BenchmarkInstrSTRSUB(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl =
    corevm::types::native_string("Hello world");

  corevm::types::native_type_handle hndl2 =
    corevm::types::uint8(4);

  corevm::runtime::instr_handler_strsub handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

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
void BenchmarkInstrSTRSUB2(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl =
    corevm::types::native_string("Hello world");

  corevm::types::native_type_handle hndl2 =
    corevm::types::uint8(4);

  corevm::types::native_type_handle hndl3 =
    corevm::types::uint8(5);

  corevm::runtime::instr_handler_strsub2 handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(hndl3);
    frame->push_eval_stack(hndl2);
    frame->push_eval_stack(hndl);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrSTRFND(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl =
    corevm::types::native_string("Hello world");

  corevm::types::native_type_handle hndl2 =
    corevm::types::native_string("world");

  corevm::runtime::instr_handler_strfnd handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

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
void BenchmarkInstrSTRFND2(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl =
    corevm::types::native_string("Hello world");

  corevm::types::native_type_handle hndl2 =
    corevm::types::native_string("world");

  corevm::types::native_type_handle hndl3 =
    corevm::types::uint8(4);

  corevm::runtime::instr_handler_strfnd2 handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(hndl3);
    frame->push_eval_stack(hndl2);
    frame->push_eval_stack(hndl);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrSTRRFND(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl =
    corevm::types::native_string("Hello world");

  corevm::types::native_type_handle hndl2 =
    corevm::types::native_string("world");

  corevm::runtime::instr_handler_strrfnd handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

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
void BenchmarkInstrSTRRFND2(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl =
    corevm::types::native_string("Hello world");

  corevm::types::native_type_handle hndl2 =
    corevm::types::native_string("hello");

  corevm::types::native_type_handle hndl3 =
    corevm::types::uint8(4);

  corevm::runtime::instr_handler_strrfnd2 handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(hndl3);
    frame->push_eval_stack(hndl2);
    frame->push_eval_stack(hndl);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

BENCHMARK(BenchmarkInstrSTRLEN);
BENCHMARK(BenchmarkInstrSTRCLR);
BENCHMARK(BenchmarkInstrSTRAPD);
BENCHMARK(BenchmarkInstrSTRPSH);
BENCHMARK(BenchmarkInstrSTRIST);
BENCHMARK(BenchmarkInstrSTRIST2);
BENCHMARK(BenchmarkInstrSTRERS);
BENCHMARK(BenchmarkInstrSTRERS2);
BENCHMARK(BenchmarkInstrSTRRPLC);
BENCHMARK(BenchmarkInstrSTRSWP);
BENCHMARK(BenchmarkInstrSTRSUB);
BENCHMARK(BenchmarkInstrSTRSUB2);
BENCHMARK(BenchmarkInstrSTRFND);
BENCHMARK(BenchmarkInstrSTRFND2);
BENCHMARK(BenchmarkInstrSTRRFND);
BENCHMARK(BenchmarkInstrSTRRFND2);

// -----------------------------------------------------------------------------
