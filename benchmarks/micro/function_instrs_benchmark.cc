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

#include "dyobj/common.h"
#include "runtime/common.h"
#include "runtime/instr.h"
#include "runtime/process.h"
#include "types/native_type_value.h"

#include "instr_benchmarks_fixture.h"


// -----------------------------------------------------------------------------

using corevm::benchmarks::InstrBenchmarksFixture;

// -----------------------------------------------------------------------------

// TODO: [COREVM-460] Re-enable functional instruction micro benchmarks
#if 0
static
void BenchmarkInstrPUTARG(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  for (size_t i = 0; i < state.max_iterations; ++i)
  {
    corevm::dyobj::dyobj_id_t id = fixture.process().create_dyobj();
    fixture.process().push_stack(id);
  }

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    corevm::runtime::instr_handler_putarg(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}
#endif

// -----------------------------------------------------------------------------

// TODO: [COREVM-460] Re-enable functional instruction micro benchmarks
#if 0
static
void BenchmarkInstrPUTKWARG(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::dyobj::dyobj_id_t id = 1;

  for (size_t i = 0; i < state.max_iterations; ++i)
  {
    fixture.process().push_stack(id);
  }

  corevm::runtime::Instr instr(0, 1, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    corevm::runtime::instr_handler_putkwarg(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}
#endif

// -----------------------------------------------------------------------------

// TODO: [COREVM-460] Re-enable functional instruction micro benchmarks
#if 0
static
void BenchmarkInstrPUTARGS(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  auto id = fixture.process().create_dyobj();
  auto& obj = fixture.process().get_dyobj(id);

  for (size_t i = 0; i < state.max_iterations; ++i)
  {
    fixture.process().push_stack(id);
  }

  corevm::types::native_type_value type_val = corevm::types::native_array {1, 2, 3};
  auto type_value_ptr = fixture.process().insert_type_value(type_val);

  obj.set_type_value(type_value_ptr);

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    corevm::runtime::instr_handler_putargs(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}
#endif

// -----------------------------------------------------------------------------

// TODO: [COREVM-460] Re-enable functional instruction micro benchmarks
#if 0
static
void BenchmarkInstrPUTKWARGS(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  auto id = fixture.process().create_dyobj();
  auto& obj = fixture.process().get_dyobj(id);

  for (size_t i = 0; i < state.max_iterations; ++i)
  {
    fixture.process().push_stack(id);
  }

  corevm::types::native_type_value type_val = corevm::types::native_map {
    { 1, 11 },
    { 2, 22 },
    { 3, 33 },
  };
  auto type_value_ptr = fixture.process().insert_type_value(type_val);

  obj.set_type_value(type_value_ptr);

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    corevm::runtime::instr_handler_putkwargs(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}
#endif

// -----------------------------------------------------------------------------

static
void BenchmarkInstrGETARG(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  auto obj = fixture.process().create_dyobj();

  for (size_t i = 0; i < state.max_iterations; ++i)
  {
    fixture.process().top_invocation_ctx().put_param(obj);
  }

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    corevm::runtime::instr_handler_getarg(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrGETKWARG(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::runtime::variable_key_t key = 1;
  auto obj = fixture.process().create_dyobj();

  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(key), 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    invk_ctx->put_param_value_pair(key, obj);

    corevm::runtime::instr_handler_getkwarg(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrGETARGS(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  auto obj = fixture.process().create_dyobj();

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    invk_ctx->put_param(obj);
    invk_ctx->put_param(obj);
    invk_ctx->put_param(obj);

    corevm::runtime::instr_handler_getargs(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrGETKWARGS(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  corevm::runtime::variable_key_t key = 1;
  auto obj = fixture.process().create_dyobj();

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    invk_ctx->put_param_value_pair(key, obj);
    invk_ctx->put_param_value_pair(key, obj);
    invk_ctx->put_param_value_pair(key, obj);

    corevm::runtime::instr_handler_getkwargs(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkInstrHASARGS(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  auto invk_ctx = &fixture.process().top_invocation_ctx();

  auto obj = fixture.process().create_dyobj();
  invk_ctx->put_param(obj);

  corevm::runtime::Instr instr(0, 0, 0);

  auto frame = &fixture.process().top_frame();

  while (state.KeepRunning())
  {
    corevm::runtime::instr_handler_hasargs(
      instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

// TODO: [COREVM-460] Re-enable functional instruction micro benchmarks
//BENCHMARK(BenchmarkInstrPUTARG);
//BENCHMARK(BenchmarkInstrPUTKWARG);
//BENCHMARK(BenchmarkInstrPUTARGS);
//BENCHMARK(BenchmarkInstrPUTKWARGS);

BENCHMARK(BenchmarkInstrGETARG);
BENCHMARK(BenchmarkInstrGETKWARG);
BENCHMARK(BenchmarkInstrGETARGS);
BENCHMARK(BenchmarkInstrGETKWARGS);
BENCHMARK(BenchmarkInstrHASARGS);

// -----------------------------------------------------------------------------
