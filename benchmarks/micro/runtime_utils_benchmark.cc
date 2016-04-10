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

#include "dyobj/dynamic_object.h"
#include "dyobj/util.h"
#include "runtime/common.h"
#include "runtime/compartment.h"
#include "runtime/utils.h"


// -----------------------------------------------------------------------------

namespace {

class DummyDynamicObjectManager {};

using DynamicObjectType = typename corevm::dyobj::DynamicObject<DummyDynamicObjectManager>;

} /* end anonymous namespace */

// -----------------------------------------------------------------------------

static
void BenchmarkGetAttrKey(benchmark::State& state)
{
  corevm::runtime::encoding_key_t key = 0;
  corevm::runtime::EncodingMap encoding_table { "hello_world" };

  corevm::runtime::Compartment compartment("");
  compartment.set_encoding_map(encoding_table);

  while (state.KeepRunning())
  {
    corevm::runtime::get_attr_key(&compartment, key);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkHashAttrStr(benchmark::State& state)
{
  const std::string attr_str("hello_world");

  while (state.KeepRunning())
  {
    corevm::dyobj::hash_attr_str(attr_str);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkGetattr(benchmark::State& state)
{
  const std::string attr_name("__str__");

  corevm::dyobj::attr_key_t attr_key = corevm::dyobj::hash_attr_str(attr_name);

  DynamicObjectType obj;
  DynamicObjectType attr_obj;
  obj.putattr(attr_key, &attr_obj);

  while (state.KeepRunning())
  {
    corevm::runtime::getattr(&obj, attr_name);
  }
}

// -----------------------------------------------------------------------------

static
void BenchmarkGetattr2(benchmark::State& state)
{
  corevm::runtime::Compartment compartment("");

  const std::string attr_name("__str__");

  corevm::runtime::EncodingMap encoding_map;
  encoding_map.emplace_back(attr_name);

  compartment.set_encoding_map(std::move(encoding_map));

  corevm::runtime::encoding_key_t attr_encoding_key = 0;

  corevm::dyobj::attr_key_t attr_key = corevm::dyobj::hash_attr_str(attr_name);

  DynamicObjectType obj;
  DynamicObjectType attr_obj;
  obj.putattr(attr_key, &attr_obj);

  while (state.KeepRunning())
  {
    corevm::runtime::getattr(&obj, &compartment, attr_encoding_key);
  }
}

// -----------------------------------------------------------------------------

BENCHMARK(BenchmarkGetAttrKey);
BENCHMARK(BenchmarkHashAttrStr);
BENCHMARK(BenchmarkGetattr);
BENCHMARK(BenchmarkGetattr2);

// -----------------------------------------------------------------------------
