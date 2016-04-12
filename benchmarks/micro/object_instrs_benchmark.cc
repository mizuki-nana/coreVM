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
#include "dyobj/util.h"
#include "runtime/instr.h"
#include "runtime/process.h"
#include "types/native_type_handle.h"

#include "instr_benchmarks_fixture.h"

#include <cassert>


#ifdef BUILD_BENCHMARKS_STRICT


// -----------------------------------------------------------------------------

#define STATE_RUN_LOOP()                                               \
  while (state.KeepRunning())                                          \
  {                                                                    \
      handler.execute(instr, fixture.process(), &frame, &invk_ctx);    \
  }

// -----------------------------------------------------------------------------

using corevm::benchmarks::InstrBenchmarksFixture;

// -----------------------------------------------------------------------------

static
void set_encoding_pair(
  corevm::runtime::Process& process,
  const corevm::runtime::EncodingMap& encoding_map)
{
  corevm::runtime::compartment_id_t compartment_id = 0;
  corevm::runtime::Compartment* compartment = nullptr;
  process.get_compartment(compartment_id, &compartment);

  assert(compartment);

  compartment->set_encoding_map(encoding_map);
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrs(benchmark::State& state)
{
  corevm::runtime::Instr instr(0, 0, 0);
  instr_handler_cls handler;

  InstrBenchmarksFixture fixture;

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  STATE_RUN_LOOP();
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsInstrWithOneObjectInVisibleScope(benchmark::State& state)
{
  corevm::runtime::variable_key_t var_key = 1;
  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(var_key), 0);

  instr_handler_cls handler;

  InstrBenchmarksFixture fixture;

  auto id = fixture.process().create_dyobj();
  fixture.process().top_frame().set_visible_var(var_key, id);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  STATE_RUN_LOOP();
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsInstrWithOneObjectInInvisibleScope(benchmark::State& state)
{
  corevm::runtime::variable_key_t var_key = 1;
  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(var_key), 0);

  corevm::runtime::instr_handler_ldobj2 handler;

  InstrBenchmarksFixture fixture;

  auto id = fixture.process().create_dyobj();
  fixture.process().top_frame().set_invisible_var(var_key, id);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  STATE_RUN_LOOP();
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsWithOneObjectOnStack(benchmark::State& state)
{
  corevm::runtime::Instr instr(0, 0, 0);
  instr_handler_cls handler;

  InstrBenchmarksFixture fixture;

  auto obj = fixture.process().create_dyobj();
  corevm::types::NativeTypeHandle hndl = corevm::types::uint32(1);
  obj->set_ntvhndl(fixture.process().insert_ntvhndl(hndl));

  for (size_t i = 0; i < state.max_iterations; ++i)
  {
    fixture.process().push_stack(obj);
  }

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  STATE_RUN_LOOP();
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsWithOneObjectOnStackWithAttr(benchmark::State& state)
{
  const std::string attr_str = "hello_world";
  auto encoding_key = 0;

  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(encoding_key), 0);

  instr_handler_cls handler;

  InstrBenchmarksFixture fixture;

  corevm::runtime::EncodingMap encoding_map { attr_str };
  set_encoding_pair(fixture.process(), encoding_map);

  auto obj = fixture.process().create_dyobj();
  auto obj2 = fixture.process().create_dyobj();

  auto attr_key = corevm::dyobj::hash_attr_str(attr_str);
  obj->putattr(attr_key, obj2);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    fixture.process().push_stack(obj);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsWithOneObjectOnStackWithAttrPerIteration(benchmark::State& state)
{
  corevm::dyobj::attr_key_t attr_key = 0;
  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(attr_key), 0);

  instr_handler_cls handler;

  InstrBenchmarksFixture fixture;

  const std::string attr_str = "hello_world";

  corevm::runtime::EncodingMap encoding_map { attr_str };
  set_encoding_pair(fixture.process(), encoding_map);

  auto obj = fixture.process().create_dyobj();
  auto obj2 = fixture.process().create_dyobj();

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    fixture.process().push_stack(obj);
    obj->putattr(attr_key, obj2);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsWithOneObjectOnStackWithNtvhndl(benchmark::State& state)
{
  corevm::runtime::Instr instr(0, 0, 0);
  instr_handler_cls handler;

  InstrBenchmarksFixture fixture;

  auto id = fixture.process().create_dyobj();
  corevm::types::NativeTypeHandle hndl = corevm::types::uint32(1);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    fixture.process().push_stack(id);
    fixture.process().top_frame().push_eval_stack(hndl);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsWithOneObjectOnStackWithNtvhndlPerIteration(benchmark::State& state)
{
  corevm::runtime::Instr instr(0, 0, 0);
  instr_handler_cls handler;

  InstrBenchmarksFixture fixture;

  auto obj = fixture.process().create_dyobj();
  corevm::types::NativeTypeHandle hndl = corevm::types::uint32(1);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    fixture.process().push_stack(obj);
    obj->set_ntvhndl(fixture.process().insert_ntvhndl(hndl));

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsWithTwoObjectsOnStack(benchmark::State& state)
{
  corevm::runtime::Instr instr(0, 0, 0);
  instr_handler_cls handler;

  InstrBenchmarksFixture fixture;

  auto obj = fixture.process().create_dyobj();
  auto obj2 = fixture.process().create_dyobj();

  corevm::types::NativeTypeHandle hndl = corevm::types::uint32(1);
  obj2->set_ntvhndl(fixture.process().insert_ntvhndl(hndl));

  for (size_t i = 0; i < state.max_iterations; ++i)
  {
    fixture.process().push_stack(obj);
    fixture.process().push_stack(obj2);
  }

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  STATE_RUN_LOOP();
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsInstrWithOneObjectInVisibleScopePerIteration(benchmark::State& state)
{
  corevm::runtime::variable_key_t var_key = 1;
  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(var_key), 0);

  instr_handler_cls handler;

  InstrBenchmarksFixture fixture;

  auto id = fixture.process().create_dyobj();

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    fixture.process().top_frame().set_visible_var(var_key, id);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsInstrWithOneObjectInInvisibleScopePerIteration(benchmark::State& state)
{
  corevm::runtime::variable_key_t var_key = 1;
  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(var_key), 0);

  corevm::runtime::instr_handler_ldobj2 handler;

  InstrBenchmarksFixture fixture;

  auto id = fixture.process().create_dyobj();
  fixture.process().top_frame().set_invisible_var(var_key, id);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  while (state.KeepRunning())
  {
    fixture.process().top_frame().set_visible_var(var_key, id);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsInstrWithTwoObjectsInVisibleScope(benchmark::State& state)
{
  corevm::runtime::variable_key_t var_key0 = 0;
  corevm::runtime::variable_key_t var_key = 1;
  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(var_key), 0);

  instr_handler_cls handler;

  InstrBenchmarksFixture fixture;

  auto id = fixture.process().create_dyobj();
  auto id2 = fixture.process().create_dyobj();

  fixture.process().top_frame().set_visible_var(var_key0, id);
  fixture.process().top_frame().set_visible_var(var_key, id2);

  corevm::types::NativeTypeHandle hndl = corevm::types::uint32(1);

  for (size_t i = 0; i < state.max_iterations; ++i)
  {
    fixture.process().top_frame().push_eval_stack(hndl);
  }

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  STATE_RUN_LOOP();
}

// -----------------------------------------------------------------------------

static
void BenchmarkSWAPInstr(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  auto obj = fixture.process().create_dyobj();
  auto obj2 = fixture.process().create_dyobj();

  corevm::types::NativeTypeHandle hndl = corevm::types::uint32(1);
  obj2->set_ntvhndl(fixture.process().insert_ntvhndl(hndl));

  fixture.process().push_stack(obj);
  fixture.process().push_stack(obj2);

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  corevm::runtime::Instr instr(0, 0, 0);
  corevm::runtime::instr_handler_swap handler;

  STATE_RUN_LOOP();
}

// -----------------------------------------------------------------------------

static
void
BenchmarkHASATTR2Instr(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  const std::string attr_str = "hello_world";
  const corevm::dyobj::attr_key_t attr_key = corevm::dyobj::hash_attr_str(attr_str);

  corevm::types::NativeTypeHandle hndl( (corevm::types::native_string(attr_str)) );

  auto obj = fixture.process().create_dyobj();
  auto obj2 = fixture.process().create_dyobj();

  obj->putattr(attr_key, obj2);
  fixture.process().push_stack(obj);

  corevm::runtime::Instr instr(0, 0, 0);
  corevm::runtime::instr_handler_hasattr2 handler;

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
void
BenchmarkGETATTR2Instr(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  const std::string attr_str = "hello_world";
  const corevm::dyobj::attr_key_t attr_key = corevm::dyobj::hash_attr_str(attr_str);

  corevm::types::NativeTypeHandle hndl( (corevm::types::native_string(attr_str)) );

  auto obj = fixture.process().create_dyobj();
  auto obj2 = fixture.process().create_dyobj();

  obj->putattr(attr_key, obj2);
  fixture.process().push_stack(obj);

  corevm::runtime::Instr instr(0, 0, 0);
  corevm::runtime::instr_handler_getattr2 handler;

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  frame->push_eval_stack(hndl);

  while (state.KeepRunning())
  {
    fixture.process().push_stack(obj);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void
BenchmarkSETATTR2Instr(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  const std::string attr_str = "hello_world";
  corevm::types::NativeTypeHandle hndl( (corevm::types::native_string(attr_str)) );

  auto obj1 = fixture.process().create_dyobj();
  auto obj2 = fixture.process().create_dyobj();

  corevm::runtime::Instr instr(0, 0, 0);
  corevm::runtime::instr_handler_setattr2 handler;

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  frame->push_eval_stack(hndl);

  while (state.KeepRunning())
  {
    fixture.process().push_stack(obj1);
    fixture.process().push_stack(obj2);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void
BenchmarkDELATTR2Instr(benchmark::State& state)
{
  InstrBenchmarksFixture fixture;

  const std::string attr_str = "hello_world";
  const corevm::dyobj::attr_key_t attr_key = corevm::dyobj::hash_attr_str(attr_str);

  corevm::types::NativeTypeHandle hndl( (corevm::types::native_string(attr_str)) );

  auto obj = fixture.process().create_dyobj();
  auto obj2 = fixture.process().create_dyobj();

  fixture.process().push_stack(obj);

  corevm::runtime::Instr instr(0, 0, 0);
  corevm::runtime::instr_handler_delattr2 handler;

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  frame->push_eval_stack(hndl);

  while (state.KeepRunning())
  {
    obj->putattr(attr_key, obj2);

    handler.execute(instr, fixture.process(), &frame, &invk_ctx);
  }
}

// -----------------------------------------------------------------------------

static
void
BenchmarkSETATTRSInstr(benchmark::State& state)
{
  corevm::runtime::Instr instr(0, 1, 1);
  corevm::runtime::instr_handler_setattrs handler;

  InstrBenchmarksFixture fixture;

  auto obj = fixture.process().create_dyobj();
  auto obj2 = fixture.process().create_dyobj();

  corevm::types::native_map attr_map {
    { 1, obj2->id() }
  };

  corevm::types::NativeTypeHandle hndl = attr_map;
  obj->set_ntvhndl(fixture.process().insert_ntvhndl(hndl));

  corevm::runtime::ClosureCtx ctx(0, 0);

  obj->set_closure_ctx(ctx);

  fixture.process().push_stack(obj2);

  for (size_t i = 0; i < state.max_iterations; ++i)
  {
    fixture.process().push_stack(obj);
  }

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  STATE_RUN_LOOP();
}

// -----------------------------------------------------------------------------

static
void
BenchmarkRSETATTRSInstr(benchmark::State& state)
{
  const std::string attr_str = "hello_world";
  auto attr_key = 0;

  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(attr_key), 0);

  corevm::runtime::instr_handler_rsetattrs handler;

  InstrBenchmarksFixture fixture;

  corevm::runtime::EncodingMap encoding_map { attr_str };
  set_encoding_pair(fixture.process(), encoding_map);

  auto obj = fixture.process().create_dyobj();
  auto obj2 = fixture.process().create_dyobj();

  corevm::types::native_map attr_map {
    { attr_key, obj2->id() }
  };

  corevm::types::NativeTypeHandle hndl = attr_map;

  fixture.process().push_stack(obj);

  for (size_t i = 0; i < state.max_iterations; ++i)
  {
    fixture.process().top_frame().push_eval_stack(hndl);
  }

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  STATE_RUN_LOOP();
}

// -----------------------------------------------------------------------------

static
void
BenchmarkSETATTRS2Instr(benchmark::State& state)
{
  const std::string attr_str = "hello_world";
  auto attr_key = 0;

  corevm::runtime::Instr instr(
    0, static_cast<corevm::runtime::instr_oprd_t>(attr_key), 0);

  corevm::runtime::instr_handler_setattrs2 handler;

  InstrBenchmarksFixture fixture;

  auto obj = fixture.process().create_dyobj();
  auto obj2 = fixture.process().create_dyobj();

  corevm::runtime::ClosureCtx ctx(0, 0);

  obj->set_closure_ctx(ctx);

  auto attr_obj1 = fixture.process().create_dyobj();
  auto attr_obj2 = fixture.process().create_dyobj();
  auto attr_obj3 = fixture.process().create_dyobj();

  corevm::dyobj::attr_key_t attr_key1 = 1;
  corevm::dyobj::attr_key_t attr_key2 = 2;
  corevm::dyobj::attr_key_t attr_key3 = 3;

  const std::string attr_str1 = "attr1";
  const std::string attr_str2 = "attr2";
  const std::string attr_str3 = "attr3";

  corevm::runtime::EncodingMap encoding_map {
    attr_str,
    attr_str1,
    attr_str2,
    attr_str3,
  };

  set_encoding_pair(fixture.process(), encoding_map);

  obj->putattr(attr_key1, attr_obj1);
  obj->putattr(attr_key2, attr_obj2);
  obj->putattr(attr_key3, attr_obj3);

  fixture.process().push_stack(obj2);

  for (size_t i = 0; i < state.max_iterations; ++i)
  {
    fixture.process().push_stack(obj);
  }

  auto frame = &fixture.process().top_frame();
  auto invk_ctx = &fixture.process().top_invocation_ctx();

  STATE_RUN_LOOP();
}

// -----------------------------------------------------------------------------

BENCHMARK_TEMPLATE(BenchmarkObjectInstrs, corevm::runtime::instr_handler_new);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsInstrWithOneObjectInVisibleScope, corevm::runtime::instr_handler_ldobj);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStack, corevm::runtime::instr_handler_stobj);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStackWithAttr, corevm::runtime::instr_handler_getattr);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithTwoObjectsOnStack, corevm::runtime::instr_handler_setattr);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStackWithAttrPerIteration, corevm::runtime::instr_handler_delattr);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsInstrWithOneObjectInInvisibleScope, corevm::runtime::instr_handler_ldobj2);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStack, corevm::runtime::instr_handler_stobj2);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsInstrWithOneObjectInVisibleScopePerIteration, corevm::runtime::instr_handler_delobj);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsInstrWithOneObjectInInvisibleScopePerIteration, corevm::runtime::instr_handler_delobj2);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStack, corevm::runtime::instr_handler_gethndl);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStackWithNtvhndl, corevm::runtime::instr_handler_sethndl);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithTwoObjectsOnStack, corevm::runtime::instr_handler_cpyhndl);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithTwoObjectsOnStack, corevm::runtime::instr_handler_cpyrepr);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStack, corevm::runtime::instr_handler_istruthy);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStackWithNtvhndlPerIteration, corevm::runtime::instr_handler_clrhndl);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithTwoObjectsOnStack, corevm::runtime::instr_handler_objeq);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithTwoObjectsOnStack, corevm::runtime::instr_handler_objneq);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStack, corevm::runtime::instr_handler_setctx);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsInstrWithTwoObjectsInVisibleScope, corevm::runtime::instr_handler_cldobj);
BENCHMARK(BenchmarkSWAPInstr);
BENCHMARK(BenchmarkHASATTR2Instr);
BENCHMARK(BenchmarkGETATTR2Instr);
BENCHMARK(BenchmarkSETATTR2Instr);
BENCHMARK(BenchmarkDELATTR2Instr);
BENCHMARK(BenchmarkSETATTRSInstr);
BENCHMARK(BenchmarkRSETATTRSInstr);
BENCHMARK(BenchmarkSETATTRS2Instr);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStackWithNtvhndl, corevm::runtime::instr_handler_putobj);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStackWithNtvhndl, corevm::runtime::instr_handler_getobj);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStack, corevm::runtime::instr_handler_setflgc);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStack, corevm::runtime::instr_handler_setfldel);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStack, corevm::runtime::instr_handler_setflcall);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStack, corevm::runtime::instr_handler_setflmute);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStack, corevm::runtime::instr_handler_pop);

#endif /* #ifdef BUILD_BENCHMARKS_STRICT */

// -----------------------------------------------------------------------------
