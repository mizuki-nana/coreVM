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


using corevm::benchmarks::instr_benchmarks_fixture;

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static void BenchmarkNtvhndlConversionInstrs(benchmark::State& state)
{
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };
  instr_handler_cls handler;

  corevm::types::native_type_handle hndl = corevm::types::int64(64);

  instr_benchmarks_fixture fixture;
  fixture.process().top_frame().push_eval_stack(hndl);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

BENCHMARK_TEMPLATE(BenchmarkNtvhndlConversionInstrs, corevm::runtime::instr_handler_2int8);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlConversionInstrs, corevm::runtime::instr_handler_2uint8);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlConversionInstrs, corevm::runtime::instr_handler_2int16);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlConversionInstrs, corevm::runtime::instr_handler_2uint16);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlConversionInstrs, corevm::runtime::instr_handler_2int32);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlConversionInstrs, corevm::runtime::instr_handler_2uint32);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlConversionInstrs, corevm::runtime::instr_handler_2int64);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlConversionInstrs, corevm::runtime::instr_handler_2uint64);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlConversionInstrs, corevm::runtime::instr_handler_2bool);

// -----------------------------------------------------------------------------
