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

#include "types/interfaces.h"
#include "types/native_type_value.h"


// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceStringGetSize(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_string("Hello world")));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_string_get_size(oprd);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceStringClear(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_string("Hello world")));

  while (state.KeepRunning())
  {
    corevm::types::interface_string_clear(oprd);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceStringAt(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_string("Hello world")));
  corevm::types::NativeTypeValue oprd2((3));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_string_at(oprd, oprd2);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceStringAt2(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_string("Hello world")));
  corevm::types::NativeTypeValue oprd2((3));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_string_at_2(oprd, oprd2);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceStringAppend(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_string("Hello world")));
  corevm::types::NativeTypeValue oprd2((corevm::types::native_string(" !!! ")));

  while (state.KeepRunning())
  {
    corevm::types::interface_string_append(oprd, oprd2);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceStringPushback(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_string("Hello world")));
  corevm::types::NativeTypeValue oprd2((corevm::types::uint32('c')));

  while (state.KeepRunning())
  {
    corevm::types::interface_string_pushback(oprd, oprd2);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceStringInsertStr(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_string("Hello world")));
  corevm::types::NativeTypeValue oprd2((corevm::types::uint32(5)));
  corevm::types::NativeTypeValue oprd3((corevm::types::native_string(" --- ")));

  while (state.KeepRunning())
  {
    corevm::types::interface_string_insert_str(oprd, oprd2, oprd3);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceStringInsertChar(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_string("Hello world")));
  corevm::types::NativeTypeValue oprd2((corevm::types::uint32(5)));
  corevm::types::NativeTypeValue oprd3((corevm::types::uint8('-')));

  while (state.KeepRunning())
  {
    corevm::types::interface_string_insert_char(oprd, oprd2, oprd3);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceStringErase(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd2((corevm::types::uint32(5)));

  while (state.KeepRunning())
  {
    corevm::types::NativeTypeValue oprd((corevm::types::native_string("Hello world")));
    corevm::types::interface_string_erase(oprd, oprd2);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceStringErase2(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd2((corevm::types::uint32(5)));
  corevm::types::NativeTypeValue oprd3((corevm::types::uint32(2)));

  while (state.KeepRunning())
  {
    corevm::types::NativeTypeValue oprd((corevm::types::native_string("Hello world")));
    corevm::types::interface_string_erase2(oprd, oprd2, oprd3);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceStringReplaceStr(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_string("Hello world")));
  corevm::types::NativeTypeValue oprd2((corevm::types::uint32(5)));
  corevm::types::NativeTypeValue oprd3((corevm::types::uint32(2)));
  corevm::types::NativeTypeValue oprd4((corevm::types::native_string("abc")));

  while (state.KeepRunning())
  {
    corevm::types::interface_string_replace_str(oprd, oprd2, oprd3, oprd4);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceStringSwap(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_string("Hello world")));
  corevm::types::NativeTypeValue oprd2((corevm::types::native_string("Byte world")));

  while (state.KeepRunning())
  {
    corevm::types::interface_string_swap(oprd, oprd2);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceStringSubstr(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_string("Hello world")));
  corevm::types::NativeTypeValue oprd2((corevm::types::uint32(5)));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_string_substr(oprd, oprd2);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceStringSubstr2(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_string("Hello world")));
  corevm::types::NativeTypeValue oprd2((corevm::types::uint32(5)));
  corevm::types::NativeTypeValue oprd3((corevm::types::uint32(2)));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_string_substr2(oprd, oprd2, oprd3);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceStringFind(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_string("Hello world")));
  corevm::types::NativeTypeValue oprd2((corevm::types::native_string("world")));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_string_find(oprd, oprd2);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceStringFind2(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_string("Hello world")));
  corevm::types::NativeTypeValue oprd2((corevm::types::native_string("world")));
  corevm::types::NativeTypeValue oprd3((corevm::types::uint32(2)));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_string_find2(oprd, oprd2, oprd3);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceStringRFind(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_string("Hello world")));
  corevm::types::NativeTypeValue oprd2((corevm::types::native_string("world")));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_string_rfind(oprd, oprd2);
  }
}

// -----------------------------------------------------------------------------

static
void Benchmark_InterfaceStringRFind2(benchmark::State& state)
{
  corevm::types::NativeTypeValue oprd((corevm::types::native_string("Hello world")));
  corevm::types::NativeTypeValue oprd2((corevm::types::native_string("world")));
  corevm::types::NativeTypeValue oprd3((corevm::types::uint32(2)));

  while (state.KeepRunning())
  {
    auto res = corevm::types::interface_string_rfind2(oprd, oprd2, oprd3);
  }
}

// -----------------------------------------------------------------------------

BENCHMARK(Benchmark_InterfaceStringGetSize);
BENCHMARK(Benchmark_InterfaceStringClear);
BENCHMARK(Benchmark_InterfaceStringAt);
BENCHMARK(Benchmark_InterfaceStringAt2);
BENCHMARK(Benchmark_InterfaceStringAppend);
BENCHMARK(Benchmark_InterfaceStringPushback);
BENCHMARK(Benchmark_InterfaceStringInsertStr);
BENCHMARK(Benchmark_InterfaceStringInsertChar);
BENCHMARK(Benchmark_InterfaceStringErase);
BENCHMARK(Benchmark_InterfaceStringErase2);
BENCHMARK(Benchmark_InterfaceStringReplaceStr);
BENCHMARK(Benchmark_InterfaceStringSwap);
BENCHMARK(Benchmark_InterfaceStringSubstr);
BENCHMARK(Benchmark_InterfaceStringSubstr2);
BENCHMARK(Benchmark_InterfaceStringFind);
BENCHMARK(Benchmark_InterfaceStringFind2);
BENCHMARK(Benchmark_InterfaceStringRFind);
BENCHMARK(Benchmark_InterfaceStringRFind2);

// -----------------------------------------------------------------------------
