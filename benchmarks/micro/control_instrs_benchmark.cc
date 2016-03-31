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

#include "runtime/closure.h"
#include "runtime/common.h"
#include "runtime/compartment.h"
#include "runtime/instr.h"
#include "runtime/process.h"
#include "types/native_type_handle.h"

#include "instr_benchmarks_fixture.h"


// -----------------------------------------------------------------------------

using corevm::benchmarks::instr_benchmarks_fixture;

// -----------------------------------------------------------------------------

#ifdef __linux__
static
void BenchmarkInstrPINVK(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  auto id = fixture.process().create_dyobj();
  auto& obj = fixture.process().get_dyobj(id);

  corevm::runtime::closure_ctx ctx(0, 0);

  obj.set_closure_ctx(ctx);

  fixture.process().push_stack(id);

  corevm::runtime::instr_handler_pinvk handler;
  corevm::runtime::instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}
#endif

// -----------------------------------------------------------------------------

#ifdef __linux__
static
void BenchmarkInstrINVK(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::runtime::instr_handler_invk handler;
  corevm::runtime::instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}
#endif

// -----------------------------------------------------------------------------

// TODO: explore other options to enable this benchmark.
// This benchmark is currently disabled due to high cost associated with
// set up works.
/*
static
void BenchmarkInstrRTRN(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::runtime::instr_handler_rtrn handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

  corevm::runtime::closure* closure =
    fixture.process().top_frame().closure_ptr();

  ASSERT(closure);

  closure->vector.clear();

  corevm::runtime::compartment* compartment =
    fixture.process().top_frame().compartment_ptr();

  ASSERT(compartment);

  fixture.process().top_frame().set_return_addr(0);

  corevm::runtime::closure_ctx ctx {
    .compartment_id = 0,
    .closure_id = 0
  };

  for (size_t i = 0; i < 260000; ++i)
  {
    fixture.process().emplace_frame(
      ctx, compartment, closure, 0);

    fixture.process().emplace_invocation_ctx(
      ctx, compartment, closure);
  }

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process());
  }
}
*/

// -----------------------------------------------------------------------------

static
void BenchmarkInstrJMP(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::runtime::instr_handler_jmp handler;
  corevm::runtime::instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrJMPIF(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::runtime::instr_handler_jmpif handler;
  corevm::runtime::instr instr(0, 0, 0);

  corevm::types::native_type_handle hndl = corevm::types::boolean(true);
  fixture.process().top_frame().push_eval_stack(hndl);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrJMPR(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::runtime::instr_handler_jmpr handler;
  corevm::runtime::instr instr(0, 1, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

// TODO: explore other options to enable this benchmark.
// This benchmark is currently disabled due to high cost associated with
// set up works.
/*
static
void BenchmarkInstrEXC(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::runtime::closure* closure = fixture.process().top_frame().closure_ptr();
  ASSERT(closure);

  closure->catch_sites.push_back(
    corevm::runtime::catch_site {
      .from = 0,
      .to = 1,
      .dst = 1
    }
  );

  corevm::runtime::instr_handler_exc handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process());
  }
}
*/

// -----------------------------------------------------------------------------

static
void BenchmarkInstrEXCOBJ(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::runtime::instr_handler_excobj handler;
  corevm::runtime::instr instr(0, 0, 0);

  auto id = fixture.process().create_dyobj();
  fixture.process().top_frame().set_exc_obj(id);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrCLREXC(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::runtime::instr_handler_clrexc handler;
  corevm::runtime::instr instr(0, 0, 0);

  fixture.process().top_frame().set_exc_obj(1);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrJMPEXC(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::runtime::instr_handler_jmpexc handler;
  corevm::runtime::instr instr(0, 0, 1);

  auto id = fixture.process().create_dyobj();
  fixture.process().top_frame().set_exc_obj(id);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

#ifdef __linux__
BENCHMARK(BenchmarkInstrPINVK);
BENCHMARK(BenchmarkInstrINVK);
#endif
BENCHMARK(BenchmarkInstrJMP);
BENCHMARK(BenchmarkInstrJMPIF);
BENCHMARK(BenchmarkInstrJMPR);
BENCHMARK(BenchmarkInstrEXCOBJ);
BENCHMARK(BenchmarkInstrCLREXC);
BENCHMARK(BenchmarkInstrJMPEXC);

// Skipping these benchmarks.
//BENCHMARK(BenchmarkInstrRTRN);
//BENCHMARK(BenchmarkInstrEXC);

// -----------------------------------------------------------------------------
