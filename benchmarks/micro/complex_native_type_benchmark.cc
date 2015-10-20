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
#include "types/native_array.h"
#include "types/native_map.h"
#include "types/native_string.h"
#include "types/variant/variant.h"

#include <benchmark/benchmark.h>

#include <utility>


// -----------------------------------------------------------------------------

using ComplexNativeType = typename corevm::types::variant::variant<
  corevm::types::native_string,
  corevm::types::native_array,
  corevm::types::native_map
>;

// -----------------------------------------------------------------------------

static void
BenchmarkNativeTypeEmptyDefaultInitialization(benchmark::State& state)
{
  while (state.KeepRunning())
  {
    ComplexNativeType v;
  }
}

// -----------------------------------------------------------------------------

template <typename NativeType>
static void
BenchmarkNativeTypeDefaultInitialization(benchmark::State& state)
{
  while (state.KeepRunning())
  {
    ComplexNativeType v( (NativeType()) );
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeStringValueInitialization(benchmark::State& state)
{
  const corevm::types::native_string value("Hello world");

  while (state.KeepRunning())
  {
    ComplexNativeType v( value );
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeArrayValueInitialization(benchmark::State& state)
{
  const corevm::types::native_array value({ 1, 2, 3, 4, 5 });

  while (state.KeepRunning())
  {
    ComplexNativeType v( value );
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeMapValueInitialization(benchmark::State& state)
{
  const corevm::types::native_map value( { { 1, 11 }, { 2, 22 } } );

  while (state.KeepRunning())
  {
    ComplexNativeType v( value );
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeStringCopyInitialization(benchmark::State& state)
{
  const ComplexNativeType value( corevm::types::native_string("Hello world") );

  while (state.KeepRunning())
  {
    ComplexNativeType v( value );
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeArrayCopyInitialization(benchmark::State& state)
{
  const ComplexNativeType value( corevm::types::native_array({ 1, 2, 3, 4, 5 }) );

  while (state.KeepRunning())
  {
    ComplexNativeType v( value );
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeMapCopyInitialization(benchmark::State& state)
{
  const ComplexNativeType value( corevm::types::native_map({ { 1, 11 }, { 2, 22 } }) );

  while (state.KeepRunning())
  {
    ComplexNativeType v( value );
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeStringValueMoveInitialization(benchmark::State& state)
{
  while (state.KeepRunning())
  {
    corevm::types::native_string value("Hello world");
    ComplexNativeType v(std::move(value));
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeArrayValueMoveInitialization(benchmark::State& state)
{
  while (state.KeepRunning())
  {
    corevm::types::native_array value({ 1, 2, 3, 4, 5 });
    ComplexNativeType v(std::move(value));
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeMapValueMoveInitialization(benchmark::State& state)
{
  while (state.KeepRunning())
  {
    corevm::types::native_map value({ { 1, 11 }, { 2, 22 } });
    ComplexNativeType v(std::move(value));
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeStringMoveInitialization(benchmark::State& state)
{
  while (state.KeepRunning())
  {
    ComplexNativeType value( corevm::types::native_string("Hello world") );
    ComplexNativeType v(std::move(value));
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeArrayMoveInitialization(benchmark::State& state)
{
  while (state.KeepRunning())
  {
    ComplexNativeType value( corevm::types::native_array({ 1, 2, 3, 4, 5 }) );
    ComplexNativeType v(std::move(value));
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeMapMoveInitialization(benchmark::State& state)
{
  while (state.KeepRunning())
  {
    ComplexNativeType value( corevm::types::native_map({ { 1, 11 }, { 2, 22 } }) );
    ComplexNativeType v(std::move(value));
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeStringValueAssignment(benchmark::State& state)
{
  const corevm::types::native_string value("Hello world");
  ComplexNativeType v;

  while (state.KeepRunning())
  {
    v = value;
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeArrayValueAssignment(benchmark::State& state)
{
  const corevm::types::native_array value({ 1, 2, 3, 4, 5 });
  ComplexNativeType v;

  while (state.KeepRunning())
  {
    v = value;
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeMapValueAssignment(benchmark::State& state)
{
  const corevm::types::native_map value({ { 1, 11 }, { 2, 22 } });
  ComplexNativeType v;

  while (state.KeepRunning())
  {
    v = value;
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeStringMoveAssignment(benchmark::State& state)
{
  while (state.KeepRunning())
  {
    corevm::types::native_string value("Hello world");
    ComplexNativeType v = std::move(value);
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeArrayMoveAssignment(benchmark::State& state)
{
  while (state.KeepRunning())
  {
    corevm::types::native_array value({ 1, 2, 3, 4, 5 });
    ComplexNativeType v = std::move(value);
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeMapMoveAssignment(benchmark::State& state)
{
  while (state.KeepRunning())
  {
    corevm::types::native_map value({ { 1, 11 }, { 2, 22 } });
    ComplexNativeType v = std::move(value);
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeStringCopyAssignment(benchmark::State& state)
{
  const ComplexNativeType value( corevm::types::native_string("Hello world") );

  while (state.KeepRunning())
  {
    ComplexNativeType v = value;
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeArrayCopyAssignment(benchmark::State& state)
{
  const corevm::types::native_array value({ 1, 2, 3, 4, 5 });

  while (state.KeepRunning())
  {
    ComplexNativeType v = value;
  }
}

// -----------------------------------------------------------------------------

static void
BenchmarkNativeMapCopyAssignment(benchmark::State& state)
{
  const corevm::types::native_map value({ { 1, 11 }, { 2, 22 } });

  while (state.KeepRunning())
  {
    ComplexNativeType v = value;
  }
}

// -----------------------------------------------------------------------------

BENCHMARK(BenchmarkNativeTypeEmptyDefaultInitialization);

BENCHMARK_TEMPLATE(BenchmarkNativeTypeDefaultInitialization, corevm::types::native_string);
BENCHMARK_TEMPLATE(BenchmarkNativeTypeDefaultInitialization, corevm::types::native_array);
BENCHMARK_TEMPLATE(BenchmarkNativeTypeDefaultInitialization, corevm::types::native_map);

BENCHMARK(BenchmarkNativeStringValueInitialization);
BENCHMARK(BenchmarkNativeArrayValueInitialization);
BENCHMARK(BenchmarkNativeMapValueInitialization);

BENCHMARK(BenchmarkNativeStringCopyInitialization);
BENCHMARK(BenchmarkNativeArrayCopyInitialization);
BENCHMARK(BenchmarkNativeMapCopyInitialization);

BENCHMARK(BenchmarkNativeStringValueMoveInitialization);
BENCHMARK(BenchmarkNativeArrayValueMoveInitialization);
BENCHMARK(BenchmarkNativeMapValueMoveInitialization);

BENCHMARK(BenchmarkNativeStringMoveInitialization);
BENCHMARK(BenchmarkNativeArrayMoveInitialization);
BENCHMARK(BenchmarkNativeMapMoveInitialization);

BENCHMARK(BenchmarkNativeStringValueAssignment);
BENCHMARK(BenchmarkNativeArrayValueAssignment);
BENCHMARK(BenchmarkNativeMapValueAssignment);

BENCHMARK(BenchmarkNativeStringMoveAssignment);
BENCHMARK(BenchmarkNativeArrayMoveAssignment);
BENCHMARK(BenchmarkNativeMapMoveAssignment);

BENCHMARK(BenchmarkNativeStringCopyAssignment);
BENCHMARK(BenchmarkNativeArrayCopyAssignment);
BENCHMARK(BenchmarkNativeMapCopyAssignment);

// -----------------------------------------------------------------------------
