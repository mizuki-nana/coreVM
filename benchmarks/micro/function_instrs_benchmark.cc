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

#include "dyobj/dyobj_id.h"
#include "runtime/common.h"
#include "runtime/instr.h"
#include "runtime/process.h"
#include "types/native_type_handle.h"

#include "instr_benchmarks_fixture.h"


// -----------------------------------------------------------------------------

using corevm::benchmarks::instr_benchmarks_fixture;

// -----------------------------------------------------------------------------

static
void BenchmarkInstrPUTARG(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::dyobj::dyobj_id id = 1;

  for (size_t i = 0; i < 10000000; ++i)
  {
    fixture.process().push_stack(id);
  }

  corevm::runtime::instr_handler_putarg handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrPUTKWARG(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::dyobj::dyobj_id id = 1;

  for (size_t i = 0; i < 10000000; ++i)
  {
    fixture.process().push_stack(id);
  }

  corevm::runtime::instr_handler_putkwarg handler;
  corevm::runtime::instr instr { .code=0, .oprd1=1, .oprd2=0 };

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrPUTARGS(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  auto id = fixture.process().create_dyobj();
  auto& obj = fixture.process().get_dyobj(id);

  for (size_t i = 0; i < 10000000; ++i)
  {
    fixture.process().push_stack(id);
  }

  corevm::types::native_type_handle hndl = corevm::types::native_array {1, 2, 3};
  auto ntvhndl_key = fixture.process().insert_ntvhndl(hndl);

  obj.set_ntvhndl_key(ntvhndl_key);

  corevm::runtime::instr_handler_putargs handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrPUTKWARGS(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  auto id = fixture.process().create_dyobj();
  auto& obj = fixture.process().get_dyobj(id);

  for (size_t i = 0; i < 10000000; ++i)
  {
    fixture.process().push_stack(id);
  }

  corevm::types::native_type_handle hndl = corevm::types::native_map {
    { 1, 11 },
    { 2, 22 },
    { 3, 33 },
  };
  auto ntvhndl_key = fixture.process().insert_ntvhndl(hndl);

  obj.set_ntvhndl_key(ntvhndl_key);

  corevm::runtime::instr_handler_putkwargs handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrGETARG(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::dyobj::dyobj_id id = 1;

  for (size_t i = 0; i < 20000000; ++i)
  {
    fixture.process().top_invocation_ctx().put_param(id);
  }

  corevm::runtime::instr_handler_getarg handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrGETKWARG(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::runtime::variable_key key = 1;
  corevm::dyobj::dyobj_id id = 1;

  corevm::runtime::instr_handler_getkwarg handler;
  corevm::runtime::instr instr {
    .code=0,
    .oprd1=static_cast<corevm::runtime::instr_oprd>(key),
    .oprd2=0
  };

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    invk_ctx->put_param_value_pair(key, id);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrGETARGS(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::dyobj::dyobj_id id = 1;

  corevm::runtime::instr_handler_getargs handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    invk_ctx->put_param(id);
    invk_ctx->put_param(id);
    invk_ctx->put_param(id);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrGETKWARGS(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  corevm::runtime::variable_key key = 1;
  corevm::dyobj::dyobj_id id = 1;

  corevm::runtime::instr_handler_getkwargs handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    invk_ctx->put_param_value_pair(key, id);
    invk_ctx->put_param_value_pair(key, id);
    invk_ctx->put_param_value_pair(key, id);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrHASARGS(benchmark::State& state)
{
  instr_benchmarks_fixture fixture;

  auto invk_ctx = &fixture.process().top_invocation_ctx();

  corevm::dyobj::dyobj_id id = 1;
  invk_ctx->put_param(id);

  corevm::runtime::instr_handler_hasargs handler;
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };

  auto frame = &fixture.process().top_frame();

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

BENCHMARK(BenchmarkInstrPUTARG);
BENCHMARK(BenchmarkInstrPUTKWARG);
BENCHMARK(BenchmarkInstrPUTARGS);
BENCHMARK(BenchmarkInstrPUTKWARGS);
BENCHMARK(BenchmarkInstrGETARG);
BENCHMARK(BenchmarkInstrGETKWARG);
BENCHMARK(BenchmarkInstrGETARGS);
BENCHMARK(BenchmarkInstrGETKWARGS);
BENCHMARK(BenchmarkInstrHASARGS);

// -----------------------------------------------------------------------------
