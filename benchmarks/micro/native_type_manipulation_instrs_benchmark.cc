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
#include "types/native_type_value.h"
#include "types/types.h"

#include "instr_benchmarks_fixture.h"


using corevm::benchmarks::InstrBenchmarksFixture;

// -----------------------------------------------------------------------------

struct instr_handler_truthy_wrapper
{
  static corevm::runtime::InstrHandler& handler()
  {
    return corevm::runtime::instr_handler_truthy;
  }
};

// -----------------------------------------------------------------------------

struct instr_handler_repr_wrapper
{
  static corevm::runtime::InstrHandler& handler()
  {
    return corevm::runtime::instr_handler_repr;
  }
};

// -----------------------------------------------------------------------------

struct instr_handler_hash_wrapper
{
  static corevm::runtime::InstrHandler& handler()
  {
    return corevm::runtime::instr_handler_hash;
  }
};

// -----------------------------------------------------------------------------

struct instr_handler_reverse_wrapper
{
  static corevm::runtime::InstrHandler& handler()
  {
    return corevm::runtime::instr_handler_reverse;
  }
};

// -----------------------------------------------------------------------------

template <typename handler_wrapper_cls, class IntrinsicType=corevm::types::int64>
static void BenchmarkNativeTypeValueManipulationInstrs(benchmark::State& state)
{
  corevm::runtime::Instr instr(0, 0, 0);

  corevm::types::NativeTypeValue oprd = IntrinsicType();

  InstrBenchmarksFixture fixture;
  fixture.process().top_frame().push_eval_stack(oprd);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  auto& handler = handler_wrapper_cls::handler();

  while (state.KeepRunning())
  {
    handler(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

BENCHMARK_TEMPLATE(BenchmarkNativeTypeValueManipulationInstrs, instr_handler_truthy_wrapper);
BENCHMARK_TEMPLATE(BenchmarkNativeTypeValueManipulationInstrs, instr_handler_truthy_wrapper, corevm::types::boolean);
BENCHMARK_TEMPLATE(BenchmarkNativeTypeValueManipulationInstrs, instr_handler_truthy_wrapper, corevm::types::decimal2);

// -----------------------------------------------------------------------------

BENCHMARK_TEMPLATE(BenchmarkNativeTypeValueManipulationInstrs, instr_handler_repr_wrapper);
BENCHMARK_TEMPLATE(BenchmarkNativeTypeValueManipulationInstrs, instr_handler_repr_wrapper, corevm::types::boolean);
BENCHMARK_TEMPLATE(BenchmarkNativeTypeValueManipulationInstrs, instr_handler_repr_wrapper, corevm::types::decimal);
BENCHMARK_TEMPLATE(BenchmarkNativeTypeValueManipulationInstrs, instr_handler_repr_wrapper, corevm::types::decimal2);
BENCHMARK_TEMPLATE(BenchmarkNativeTypeValueManipulationInstrs, instr_handler_repr_wrapper, corevm::types::native_string);
BENCHMARK_TEMPLATE(BenchmarkNativeTypeValueManipulationInstrs, instr_handler_repr_wrapper, corevm::types::native_array);
BENCHMARK_TEMPLATE(BenchmarkNativeTypeValueManipulationInstrs, instr_handler_repr_wrapper, corevm::types::native_map);

// -----------------------------------------------------------------------------

BENCHMARK_TEMPLATE(BenchmarkNativeTypeValueManipulationInstrs, instr_handler_hash_wrapper);
BENCHMARK_TEMPLATE(BenchmarkNativeTypeValueManipulationInstrs, instr_handler_hash_wrapper, corevm::types::native_string);
BENCHMARK_TEMPLATE(BenchmarkNativeTypeValueManipulationInstrs, instr_handler_hash_wrapper, corevm::types::native_array);
BENCHMARK_TEMPLATE(BenchmarkNativeTypeValueManipulationInstrs, instr_handler_hash_wrapper, corevm::types::native_map);

// -----------------------------------------------------------------------------

BENCHMARK_TEMPLATE(BenchmarkNativeTypeValueManipulationInstrs, instr_handler_reverse_wrapper, corevm::types::native_array);
BENCHMARK_TEMPLATE(BenchmarkNativeTypeValueManipulationInstrs, instr_handler_reverse_wrapper, corevm::types::native_string);

// -----------------------------------------------------------------------------
