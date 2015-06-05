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
static void BenchmarkNtvhndlArithmeticUnaryInstrs(benchmark::State& state)
{
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };
  instr_handler_cls handler;

  corevm::types::native_type_handle oprd = corevm::types::uint32(666);

  instr_benchmarks_fixture fixture;
  fixture.process().top_frame().push_eval_stack(oprd);

  while (state.KeepRunning())
  {
      handler.execute(instr, fixture.process());
  }
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static void BenchmarkNtvhndlArithmeticBinaryInstrs(benchmark::State& state)
{
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };
  instr_handler_cls handler;

  corevm::types::native_type_handle oprd1 = corevm::types::int8(12);
  corevm::types::native_type_handle oprd2 = corevm::types::uint32(666);

  instr_benchmarks_fixture fixture;

  // TODO: Investigate state pause/resume timing affects benchmark timing result.
  while (state.KeepRunning())
  {
      state.PauseTiming();
      fixture.process().top_frame().push_eval_stack(oprd1);
      fixture.process().top_frame().push_eval_stack(oprd2);
      state.ResumeTiming();

      handler.execute(instr, fixture.process());

      state.PauseTiming();
      fixture.process().top_frame().pop_eval_stack();
      state.ResumeTiming();
  }
}

// -----------------------------------------------------------------------------

BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticUnaryInstrs, corevm::runtime::instr_handler_pos);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticUnaryInstrs, corevm::runtime::instr_handler_neg);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticUnaryInstrs, corevm::runtime::instr_handler_inc);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticUnaryInstrs, corevm::runtime::instr_handler_dec);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticBinaryInstrs, corevm::runtime::instr_handler_add);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticBinaryInstrs, corevm::runtime::instr_handler_sub);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticBinaryInstrs, corevm::runtime::instr_handler_mul);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticBinaryInstrs, corevm::runtime::instr_handler_div);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticBinaryInstrs, corevm::runtime::instr_handler_mod);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticBinaryInstrs, corevm::runtime::instr_handler_pow);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticUnaryInstrs, corevm::runtime::instr_handler_bnot);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticBinaryInstrs, corevm::runtime::instr_handler_band);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticBinaryInstrs, corevm::runtime::instr_handler_bor);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticBinaryInstrs, corevm::runtime::instr_handler_bxor);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticBinaryInstrs, corevm::runtime::instr_handler_bls);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticBinaryInstrs, corevm::runtime::instr_handler_brs);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticBinaryInstrs, corevm::runtime::instr_handler_eq);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticBinaryInstrs, corevm::runtime::instr_handler_neq);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticBinaryInstrs, corevm::runtime::instr_handler_gt);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticBinaryInstrs, corevm::runtime::instr_handler_lt);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticBinaryInstrs, corevm::runtime::instr_handler_gte);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticBinaryInstrs, corevm::runtime::instr_handler_lte);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticUnaryInstrs, corevm::runtime::instr_handler_lnot);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticBinaryInstrs, corevm::runtime::instr_handler_land);
BENCHMARK_TEMPLATE(BenchmarkNtvhndlArithmeticBinaryInstrs, corevm::runtime::instr_handler_lor);

// -----------------------------------------------------------------------------
