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

static void BenchmarkNtvhndlArithmeticUnaryInstrs(benchmark::State& state,
  corevm::runtime::InstrHandler handler)
{
  corevm::runtime::Instr instr(0, 0, 0);

  corevm::types::NativeTypeHandle oprd = corevm::types::uint32(666);

  InstrBenchmarksFixture fixture;
  fixture.process().top_frame().push_eval_stack(oprd);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    handler(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

#define BENCHMARK_NTVHNDL_ARITHMETIC_UNARY_INSTR(name, handler)       \
static void                                                           \
Benchmark##name##Instr(benchmark::State& state)                       \
{                                                                     \
  BenchmarkNtvhndlArithmeticUnaryInstrs(state, (handler));            \
}                                                                     \
BENCHMARK(Benchmark##name##Instr)

// -----------------------------------------------------------------------------

static void
BenchmarkNtvhndlArithmeticBinaryInstrs(benchmark::State& state,
  corevm::runtime::InstrHandler handler)
{
  corevm::runtime::Instr instr(0, 0, 0);

  corevm::types::NativeTypeHandle oprd1 = corevm::types::int64(1);
  corevm::types::NativeTypeHandle oprd2 = corevm::types::uint32(1);

  InstrBenchmarksFixture fixture;

  fixture.process().top_frame().push_eval_stack(oprd1);
  fixture.process().top_frame().push_eval_stack(oprd2);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    handler(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

#define BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(name, handler)        \
static void                                                             \
Benchmark##name##Instr(benchmark::State& state)                         \
{                                                                       \
  BenchmarkNtvhndlArithmeticBinaryInstrs(state, (handler));             \
}                                                                       \
BENCHMARK(Benchmark##name##Instr)

// -----------------------------------------------------------------------------

BENCHMARK_NTVHNDL_ARITHMETIC_UNARY_INSTR(POS, corevm::runtime::instr_handler_pos);
BENCHMARK_NTVHNDL_ARITHMETIC_UNARY_INSTR(NEG, corevm::runtime::instr_handler_neg);
BENCHMARK_NTVHNDL_ARITHMETIC_UNARY_INSTR(INC, corevm::runtime::instr_handler_inc);
BENCHMARK_NTVHNDL_ARITHMETIC_UNARY_INSTR(DEC, corevm::runtime::instr_handler_dec);
BENCHMARK_NTVHNDL_ARITHMETIC_UNARY_INSTR(ABS, corevm::runtime::instr_handler_abs);
BENCHMARK_NTVHNDL_ARITHMETIC_UNARY_INSTR(SQRT, corevm::runtime::instr_handler_sqrt);
BENCHMARK_NTVHNDL_ARITHMETIC_UNARY_INSTR(BNOT, corevm::runtime::instr_handler_bnot);
BENCHMARK_NTVHNDL_ARITHMETIC_UNARY_INSTR(LNOT, corevm::runtime::instr_handler_lnot);
// TODO: enable test
//BENCHMARK_NTVHNDL_ARITHMETIC_UNARY_INSTR(MOD, corevm::runtime::instr_handler_mod);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(ADD, corevm::runtime::instr_handler_add);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(SUB, corevm::runtime::instr_handler_sub);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(MUL, corevm::runtime::instr_handler_mul);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(DIV, corevm::runtime::instr_handler_div);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(POW, corevm::runtime::instr_handler_pow);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(BAND, corevm::runtime::instr_handler_band);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(BOR, corevm::runtime::instr_handler_bor);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(BXOR, corevm::runtime::instr_handler_bxor);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(BLS, corevm::runtime::instr_handler_bls);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(BRS, corevm::runtime::instr_handler_brs);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(EQ, corevm::runtime::instr_handler_eq);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(NEQ, corevm::runtime::instr_handler_neq);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(GT, corevm::runtime::instr_handler_gt);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(LT, corevm::runtime::instr_handler_lt);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(GTE, corevm::runtime::instr_handler_gte);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(LTE, corevm::runtime::instr_handler_lte);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(CMP, corevm::runtime::instr_handler_cmp);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(LAND, corevm::runtime::instr_handler_land);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(LOR, corevm::runtime::instr_handler_lor);
BENCHMARK_NTVHNDL_ARITHMETIC_BINARY_INSTR(ROUND, corevm::runtime::instr_handler_round);

// -----------------------------------------------------------------------------
