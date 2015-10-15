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
void BenchmarkInstrMAPLEN(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl = corevm::types::native_map {
    { 1, 11 },
    { 2, 22 },
    { 3, 33 },
  };

  corevm::runtime::instr_handler_maplen handler;
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
void BenchmarkInstrMAPEMP(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl = corevm::types::native_map {
    { 1, 11 },
    { 2, 22 },
    { 3, 33 },
  };

  corevm::runtime::instr_handler_mapemp handler;
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
void BenchmarkInstrMAPFIND(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl = corevm::types::native_map {
    { 1, 11 },
    { 2, 22 },
    { 3, 33 },
  };

  corevm::types::native_type_handle hndl2 =
    corevm::types::uint32(2);

  corevm::runtime::instr_handler_mapfind handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(hndl);
    frame->push_eval_stack(hndl2);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrMAPAT(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl = corevm::types::native_map {
    { 1, 11 },
    { 2, 22 },
    { 3, 33 },
  };

  corevm::types::native_type_handle hndl2 =
    corevm::types::uint32(2);

  corevm::runtime::instr_handler_mapat handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(hndl);
    frame->push_eval_stack(hndl2);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrMAPPUT(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl = corevm::types::native_map {
    { 1, 11 },
    { 2, 22 },
    { 3, 33 },
  };

  corevm::types::native_type_handle hndl2 =
    corevm::types::uint32(2);

  corevm::types::native_type_handle hndl3 =
    corevm::types::uint32(222);

  corevm::runtime::instr_handler_mapput handler;
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
void BenchmarkInstrMAPSET(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl = corevm::types::native_map {
    { 1, 11 },
    { 2, 22 },
    { 3, 33 },
  };

  corevm::runtime::variable_key key = 4;
  corevm::dyobj::dyobj_id id = 44;

  fixture.process().push_stack(id);

  corevm::runtime::instr_handler_mapset handler;
  corevm::runtime::instr instr {
    .code=0,
    .oprd1=static_cast<corevm::runtime::instr_oprd>(key),
    .oprd2=0
  };

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
void BenchmarkInstrMAPERS(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl = corevm::types::native_map {
    { 1, 11 },
    { 2, 22 },
    { 3, 33 },
  };

  corevm::types::native_type_handle hndl2 =
    corevm::types::uint32(2);

  corevm::runtime::instr_handler_mapers handler;
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
void BenchmarkInstrMAPCLR(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl = corevm::types::native_map {
    { 1, 11 },
    { 2, 22 },
    { 3, 33 },
  };

  corevm::runtime::instr_handler_mapclr handler;
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
void BenchmarkInstrMAPSWP(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl = corevm::types::native_map {
    { 1, 11 },
    { 2, 22 },
    { 3, 33 },
  };

  corevm::types::native_type_handle hndl2 = corevm::types::native_map {
    { 11, 1 },
    { 22, 2 },
    { 33, 3 },
  };

  corevm::runtime::instr_handler_mapswp handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  frame->push_eval_stack(hndl);
  frame->push_eval_stack(hndl2);

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrMAPKEYS(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl = corevm::types::native_map {
    { 1, 11 },
    { 2, 22 },
    { 3, 33 },
  };

  corevm::runtime::instr_handler_mapkeys handler;
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
void BenchmarkInstrMAPVALS(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl = corevm::types::native_map {
    { 1, 11 },
    { 2, 22 },
    { 3, 33 },
  };

  corevm::runtime::instr_handler_mapvals handler;
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
void BenchmarkInstrMAPMRG(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::types::native_type_handle hndl = corevm::types::native_map {
    { 1, 11 },
    { 2, 22 },
    { 3, 33 },
  };

  corevm::types::native_type_handle hndl2 = corevm::types::native_map {
    { 11, 1 },
    { 22, 2 },
    { 33, 3 },
  };

  corevm::runtime::instr_handler_mapmrg handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    frame->push_eval_stack(hndl);
    frame->push_eval_stack(hndl2);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

BENCHMARK(BenchmarkInstrMAPLEN);
BENCHMARK(BenchmarkInstrMAPEMP);
BENCHMARK(BenchmarkInstrMAPFIND);
BENCHMARK(BenchmarkInstrMAPAT);
BENCHMARK(BenchmarkInstrMAPPUT);
BENCHMARK(BenchmarkInstrMAPSET);
BENCHMARK(BenchmarkInstrMAPERS);
BENCHMARK(BenchmarkInstrMAPCLR);
BENCHMARK(BenchmarkInstrMAPSWP);
BENCHMARK(BenchmarkInstrMAPKEYS);
BENCHMARK(BenchmarkInstrMAPVALS);
BENCHMARK(BenchmarkInstrMAPMRG);

// -----------------------------------------------------------------------------
