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

#include "runtime/compartment.h"
#include "runtime/instr.h"

#include "instr_benchmarks_fixture.h"


using corevm::benchmarks::InstrBenchmarksFixture;

// -----------------------------------------------------------------------------

static void
BenchmarkNtvhndlCreationInstrs(benchmark::State& state,
  corevm::runtime::InstrHandler handler)
{
  corevm::runtime::Instr instr(0, 0, 0);

  InstrBenchmarksFixture fixture;
  auto compartment = fixture.process().top_frame().compartment();
  corevm::runtime::FptLiteralTable fpt_literal_table { 3.1415 };
  compartment->set_fpt_literal_table(fpt_literal_table);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    handler(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

#define BENCHMARK_NTVHNDL_CREATION_INSTR(name, handler)        \
static void                                                    \
Benchmark##name##Instr(benchmark::State& state)                \
{                                                              \
  BenchmarkNtvhndlCreationInstrs(state, (handler));            \
}                                                              \
BENCHMARK(Benchmark##name##Instr)

// -----------------------------------------------------------------------------

BENCHMARK_NTVHNDL_CREATION_INSTR(INT8, corevm::runtime::instr_handler_int8);
BENCHMARK_NTVHNDL_CREATION_INSTR(UINT8, corevm::runtime::instr_handler_uint8);
BENCHMARK_NTVHNDL_CREATION_INSTR(INT16, corevm::runtime::instr_handler_int16);
BENCHMARK_NTVHNDL_CREATION_INSTR(UINT16, corevm::runtime::instr_handler_uint16);
BENCHMARK_NTVHNDL_CREATION_INSTR(INT32, corevm::runtime::instr_handler_int32);
BENCHMARK_NTVHNDL_CREATION_INSTR(UINT32, corevm::runtime::instr_handler_uint32);
BENCHMARK_NTVHNDL_CREATION_INSTR(INT64, corevm::runtime::instr_handler_int64);
BENCHMARK_NTVHNDL_CREATION_INSTR(UINT64, corevm::runtime::instr_handler_uint64);
BENCHMARK_NTVHNDL_CREATION_INSTR(BOOL, corevm::runtime::instr_handler_bool);
BENCHMARK_NTVHNDL_CREATION_INSTR(DEC1, corevm::runtime::instr_handler_dec1);
BENCHMARK_NTVHNDL_CREATION_INSTR(DEC2, corevm::runtime::instr_handler_dec2);
BENCHMARK_NTVHNDL_CREATION_INSTR(STR, corevm::runtime::instr_handler_str);
BENCHMARK_NTVHNDL_CREATION_INSTR(ARY, corevm::runtime::instr_handler_ary);
BENCHMARK_NTVHNDL_CREATION_INSTR(MAP, corevm::runtime::instr_handler_map);

// -----------------------------------------------------------------------------
