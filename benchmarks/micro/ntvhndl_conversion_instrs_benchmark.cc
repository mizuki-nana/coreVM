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
#include "types/native_type_handle.h"
#include "types/types.h"

#include "instr_benchmarks_fixture.h"


using corevm::benchmarks::InstrBenchmarksFixture;

// -----------------------------------------------------------------------------

static void
BenchmarkNtvhndlConversionInstrs(benchmark::State& state,
  corevm::runtime::InstrHandler handler)
{
  corevm::runtime::Instr instr(0, 0, 0);

  corevm::types::NativeTypeHandle hndl(corevm::types::int64(1));

  InstrBenchmarksFixture fixture;
  fixture.process().top_frame().push_eval_stack(hndl);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    handler(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

#define BENCHMARK_NTVHNDL_CONVERSION_INSTR(name, handler)      \
static void                                                    \
Benchmark##name##Instr(benchmark::State& state)                \
{                                                              \
  BenchmarkNtvhndlConversionInstrs(state, (handler));          \
}                                                              \
BENCHMARK(Benchmark##name##Instr)

// -----------------------------------------------------------------------------

BENCHMARK_NTVHNDL_CONVERSION_INSTR(2INT8, corevm::runtime::instr_handler_2int8);
BENCHMARK_NTVHNDL_CONVERSION_INSTR(2UINT8, corevm::runtime::instr_handler_2uint8);
BENCHMARK_NTVHNDL_CONVERSION_INSTR(2INT16, corevm::runtime::instr_handler_2int16);
BENCHMARK_NTVHNDL_CONVERSION_INSTR(2UINT16, corevm::runtime::instr_handler_2uint16);
BENCHMARK_NTVHNDL_CONVERSION_INSTR(2INT32, corevm::runtime::instr_handler_2int32);
BENCHMARK_NTVHNDL_CONVERSION_INSTR(2UINT32, corevm::runtime::instr_handler_2uint32);
BENCHMARK_NTVHNDL_CONVERSION_INSTR(2INT64, corevm::runtime::instr_handler_2int64);
BENCHMARK_NTVHNDL_CONVERSION_INSTR(2UINT64, corevm::runtime::instr_handler_2uint64);
BENCHMARK_NTVHNDL_CONVERSION_INSTR(2BOOL, corevm::runtime::instr_handler_2bool);

// -----------------------------------------------------------------------------
