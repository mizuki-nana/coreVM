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

#include "dyobj/util.h"
#include "runtime/instr.h"
#include "runtime/process.h"

#include "instr_benchmarks_fixture.h"

#include <cassert>


// -----------------------------------------------------------------------------

#define STATE_RUN_LOOP()                            \
  while (state.KeepRunning())                       \
  {                                                 \
      handler.execute(instr, fixture.process());    \
  }

// -----------------------------------------------------------------------------

using corevm::benchmarks::instr_benchmarks_fixture;

// -----------------------------------------------------------------------------

static
void set_encoding_pair(
  corevm::runtime::process& process,
  corevm::runtime::encoding_key key, const std::string& val)
{
  corevm::runtime::compartment_id compartment_id = 0;
  corevm::runtime::compartment* compartment = nullptr;
  process.get_compartment(compartment_id, &compartment);

  assert(compartment);

  corevm::runtime::encoding_map encoding_map { { key, val } };
  compartment->set_encoding_map(encoding_map);
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrs(benchmark::State& state)
{
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };
  instr_handler_cls handler;

  instr_benchmarks_fixture fixture;

  STATE_RUN_LOOP();
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsInstrWithOneObjectInVisibleScope(benchmark::State& state)
{
  corevm::runtime::variable_key var_key = 1;
  corevm::runtime::instr instr {
    .code=0,
    .oprd1=static_cast<corevm::runtime::instr_oprd>(var_key),
    .oprd2=0
  };

  instr_handler_cls handler;

  instr_benchmarks_fixture fixture;

  auto id = fixture.process().create_dyobj();
  fixture.process().top_frame().set_visible_var(var_key, id);

  STATE_RUN_LOOP();
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsInstrWithOneObjectInInvisibleScope(benchmark::State& state)
{
  corevm::runtime::variable_key var_key = 1;
  corevm::runtime::instr instr {
    .code=0,
    .oprd1=static_cast<corevm::runtime::instr_oprd>(var_key),
    .oprd2=0
  };

  corevm::runtime::instr_handler_ldobj2 handler;

  instr_benchmarks_fixture fixture;

  auto id = fixture.process().create_dyobj();
  fixture.process().top_frame().set_invisible_var(var_key, id);

  STATE_RUN_LOOP();
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsWithOneObjectOnStack(benchmark::State& state)
{
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };
  instr_handler_cls handler;

  instr_benchmarks_fixture fixture;

  auto id = fixture.process().create_dyobj();
  auto& obj = fixture.process().get_dyobj(id);
  corevm::types::native_type_handle hndl = corevm::types::uint32(1);
  obj.set_ntvhndl_key(fixture.process().insert_ntvhndl(hndl));

  for (size_t i = 0; i < state.max_iterations; ++i)
  {
    fixture.process().push_stack(id);
  }

  STATE_RUN_LOOP();
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsWithOneObjectOnStackWithAttr(benchmark::State& state)
{
  const std::string attr_str = "hello_world";
  auto attr_key = corevm::dyobj::hash_attr_str(attr_str);

  corevm::runtime::instr instr {
    .code=0,
    .oprd1=static_cast<corevm::runtime::instr_oprd>(attr_key),
    .oprd2=0
  };

  instr_handler_cls handler;

  instr_benchmarks_fixture fixture;

  set_encoding_pair(fixture.process(), attr_key, attr_str);

  auto id = fixture.process().create_dyobj();
  auto id2 = fixture.process().create_dyobj();
  auto& obj = fixture.process().get_dyobj(id);

  obj.putattr(attr_key, id2);

  while (state.KeepRunning())
  {
    fixture.process().push_stack(id);

    handler.execute(instr, fixture.process());
  }
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsWithOneObjectOnStackWithAttrPerIteration(benchmark::State& state)
{
  auto attr_key = 1;
  corevm::runtime::instr instr {
    .code=0,
    .oprd1=static_cast<corevm::runtime::instr_oprd>(attr_key),
    .oprd2=0
  };

  instr_handler_cls handler;

  instr_benchmarks_fixture fixture;

  const std::string attr_str = "hello_world";
  set_encoding_pair(fixture.process(), attr_key, attr_str);

  auto id = fixture.process().create_dyobj();
  auto id2 = fixture.process().create_dyobj();
  auto& obj = fixture.process().get_dyobj(id);

  while (state.KeepRunning())
  {
    fixture.process().push_stack(id);
    obj.putattr(attr_key, id2);

    handler.execute(instr, fixture.process());
  }
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsWithOneObjectOnStackWithNtvhndl(benchmark::State& state)
{
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };
  instr_handler_cls handler;

  instr_benchmarks_fixture fixture;

  auto id = fixture.process().create_dyobj();
  corevm::types::native_type_handle hndl = corevm::types::uint32(1);

  while (state.KeepRunning())
  {
    fixture.process().push_stack(id);
    fixture.process().top_frame().push_eval_stack(hndl);

    handler.execute(instr, fixture.process());
  }
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsWithOneObjectOnStackWithNtvhndlPerIteration(benchmark::State& state)
{
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };
  instr_handler_cls handler;

  instr_benchmarks_fixture fixture;

  auto id = fixture.process().create_dyobj();
  auto& obj = fixture.process().get_dyobj(id);
  corevm::types::native_type_handle hndl = corevm::types::uint32(1);

  while (state.KeepRunning())
  {
    fixture.process().push_stack(id);
    obj.set_ntvhndl_key(fixture.process().insert_ntvhndl(hndl));

    handler.execute(instr, fixture.process());
  }
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsWithTwoObjectsOnStack(benchmark::State& state)
{
  corevm::runtime::instr instr { .code=0, .oprd1=0, .oprd2=0 };
  instr_handler_cls handler;

  instr_benchmarks_fixture fixture;

  auto id = fixture.process().create_dyobj();
  auto id2 = fixture.process().create_dyobj();

  auto& obj2 = fixture.process().get_dyobj(id2);
  corevm::types::native_type_handle hndl = corevm::types::uint32(1);
  obj2.set_ntvhndl_key(fixture.process().insert_ntvhndl(hndl));

  for (size_t i = 0; i < state.max_iterations; ++i)
  {
    fixture.process().push_stack(id);
    fixture.process().push_stack(id2);
  }

  STATE_RUN_LOOP();
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsInstrWithOneObjectInVisibleScopePerIteration(benchmark::State& state)
{
  corevm::runtime::variable_key var_key = 1;
  corevm::runtime::instr instr {
    .code=0,
    .oprd1=static_cast<corevm::runtime::instr_oprd>(var_key),
    .oprd2=0
  };

  instr_handler_cls handler;

  instr_benchmarks_fixture fixture;

  auto id = fixture.process().create_dyobj();

  while (state.KeepRunning())
  {
    fixture.process().top_frame().set_visible_var(var_key, id);

    handler.execute(instr, fixture.process());
  }
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsInstrWithOneObjectInInvisibleScopePerIteration(benchmark::State& state)
{
  corevm::runtime::variable_key var_key = 1;
  corevm::runtime::instr instr {
    .code=0,
    .oprd1=static_cast<corevm::runtime::instr_oprd>(var_key),
    .oprd2=0
  };

  corevm::runtime::instr_handler_ldobj2 handler;

  instr_benchmarks_fixture fixture;

  auto id = fixture.process().create_dyobj();
  fixture.process().top_frame().set_invisible_var(var_key, id);

  while (state.KeepRunning())
  {
    fixture.process().top_frame().set_visible_var(var_key, id);

    handler.execute(instr, fixture.process());
  }
}

// -----------------------------------------------------------------------------

template <class instr_handler_cls>
static
void BenchmarkObjectInstrsInstrWithTwoObjectsInVisibleScope(benchmark::State& state)
{
  corevm::runtime::variable_key var_key0 = 0;
  corevm::runtime::variable_key var_key = 1;
  corevm::runtime::instr instr {
    .code=0,
    .oprd1=static_cast<corevm::runtime::instr_oprd>(var_key),
    .oprd2=0
  };

  instr_handler_cls handler;

  instr_benchmarks_fixture fixture;

  auto id = fixture.process().create_dyobj();
  auto id2 = fixture.process().create_dyobj();

  fixture.process().top_frame().set_visible_var(var_key0, id);
  fixture.process().top_frame().set_visible_var(var_key, id2);

  corevm::types::native_type_handle hndl = corevm::types::uint32(1);

  for (size_t i = 0; i < state.max_iterations; ++i)
  {
    fixture.process().top_frame().push_eval_stack(hndl);
  }

  STATE_RUN_LOOP();
}

// -----------------------------------------------------------------------------

static
void
BenchmarkSETATTRSInstr(benchmark::State& state)
{
  corevm::runtime::instr instr { .code=0, .oprd1=1, .oprd2=1 };
  corevm::runtime::instr_handler_setattrs handler;

  instr_benchmarks_fixture fixture;

  auto id = fixture.process().create_dyobj();
  auto id2 = fixture.process().create_dyobj();

  corevm::types::native_map attr_map {
    { 1, id2 }
  };

  corevm::types::native_type_handle hndl = attr_map;
  auto& obj = fixture.process().get_dyobj(id);
  obj.set_ntvhndl_key(fixture.process().insert_ntvhndl(hndl));

  corevm::runtime::closure_ctx ctx {
    .compartment_id = 0,
    .closure_id = 0
  };

  obj.set_closure_ctx(ctx);

  fixture.process().push_stack(id2);

  for (size_t i = 0; i < state.max_iterations; ++i)
  {
    fixture.process().push_stack(id);
  }

  STATE_RUN_LOOP();
}

// -----------------------------------------------------------------------------

static
void
BenchmarkRSETATTRSInstr(benchmark::State& state)
{
  const std::string attr_str = "hello_world";
  auto attr_key = corevm::dyobj::hash_attr_str(attr_str);

  corevm::runtime::instr instr { .code=0, .oprd1=attr_key, .oprd2=0 };
  corevm::runtime::instr_handler_rsetattrs handler;

  instr_benchmarks_fixture fixture;

  set_encoding_pair(fixture.process(), attr_key, attr_str);

  auto id = fixture.process().create_dyobj();
  auto id2 = fixture.process().create_dyobj();

  corevm::types::native_map attr_map {
    { attr_key, id2 }
  };

  corevm::types::native_type_handle hndl = attr_map;

  fixture.process().push_stack(id);

  for (size_t i = 0; i < state.max_iterations; ++i)
  {
    fixture.process().top_frame().push_eval_stack(hndl);
  }

  STATE_RUN_LOOP();
}

// -----------------------------------------------------------------------------

BENCHMARK_TEMPLATE(BenchmarkObjectInstrs, corevm::runtime::instr_handler_new);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsInstrWithOneObjectInVisibleScope, corevm::runtime::instr_handler_ldobj);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStack, corevm::runtime::instr_handler_stobj);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStackWithAttr, corevm::runtime::instr_handler_getattr);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithTwoObjectsOnStack, corevm::runtime::instr_handler_setattr);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStackWithAttrPerIteration, corevm::runtime::instr_handler_delattr);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStack, corevm::runtime::instr_handler_pop);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsInstrWithOneObjectInInvisibleScope, corevm::runtime::instr_handler_ldobj2);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStack, corevm::runtime::instr_handler_stobj2);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsInstrWithOneObjectInVisibleScopePerIteration, corevm::runtime::instr_handler_delobj);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsInstrWithOneObjectInInvisibleScopePerIteration, corevm::runtime::instr_handler_delobj2);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStack, corevm::runtime::instr_handler_gethndl);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStackWithNtvhndl, corevm::runtime::instr_handler_sethndl);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithTwoObjectsOnStack, corevm::runtime::instr_handler_cpyhndl);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithTwoObjectsOnStack, corevm::runtime::instr_handler_cpyrepr);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStackWithNtvhndlPerIteration, corevm::runtime::instr_handler_clrhndl);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithTwoObjectsOnStack, corevm::runtime::instr_handler_objeq);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithTwoObjectsOnStack, corevm::runtime::instr_handler_objneq);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStack, corevm::runtime::instr_handler_setctx);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsInstrWithTwoObjectsInVisibleScope, corevm::runtime::instr_handler_cldobj);
BENCHMARK(BenchmarkSETATTRSInstr);
BENCHMARK(BenchmarkRSETATTRSInstr);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStackWithNtvhndl, corevm::runtime::instr_handler_putobj);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStackWithNtvhndl, corevm::runtime::instr_handler_getobj);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithTwoObjectsOnStack, corevm::runtime::instr_handler_swap);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStack, corevm::runtime::instr_handler_setflgc);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStack, corevm::runtime::instr_handler_setfldel);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStack, corevm::runtime::instr_handler_setflcall);
BENCHMARK_TEMPLATE(BenchmarkObjectInstrsWithOneObjectOnStack, corevm::runtime::instr_handler_setflmute);

// -----------------------------------------------------------------------------
