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

#include "types/variant/static_visitor.h"
#include "types/variant/variant.h"

#include <boost/variant/get.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/variant/variant.hpp>

#include <cstdio>
#include <sstream>
#include <string>
#include <vector>


// -----------------------------------------------------------------------------

using BoostVariant = typename boost::variant<int, double, std::string, std::vector<int>>;
using CorevmVariant = typename corevm::types::variant::variant<int, double, std::string, std::vector<int>>;

// -----------------------------------------------------------------------------

template <typename VariantType>
static void BenchmarkVariantEmptyInitialization(benchmark::State& state)
{
  while (state.KeepRunning())
  {
    VariantType v;
  }
}

// -----------------------------------------------------------------------------

template <typename VariantType>
static void BenchmarkVariantCopyInitialization(benchmark::State& state)
{
  VariantType v0 = std::vector<int> {1, 2, 3, 4, 5};

  while (state.KeepRunning())
  {
    VariantType v(v0);
  }
}

// -----------------------------------------------------------------------------

template <typename VariantType>
static void BenchmarkVariantMoveInitialization(benchmark::State& state)
{
  VariantType v0 = std::vector<int> {1, 2, 3, 4, 5};

  while (state.KeepRunning())
  {
    VariantType v(std::move(v0));
  }
}

// -----------------------------------------------------------------------------

template <typename VariantType>
static void BenchmarkVariantMoveValueInitialization(benchmark::State& state)
{
  const std::string s = "Hello world";

  while (state.KeepRunning())
  {
    VariantType v(std::move(s));
  }
}

// -----------------------------------------------------------------------------

template <typename VariantType>
static void BenchmarkVariantAssignmentInitialization(benchmark::State& state)
{
  VariantType v0 = std::vector<int> {1, 2, 3, 4, 5};

  while (state.KeepRunning())
  {
    VariantType v = v0;
  }
}

// -----------------------------------------------------------------------------

template <typename VariantType>
static void BenchmarkVariantValueAssignmentInitialization(benchmark::State& state)
{
  while (state.KeepRunning())
  {
    VariantType v = "Hello world";
  }
}

// -----------------------------------------------------------------------------

template <typename VariantType>
static void BenchmarkVariantMoveValueAssignmentInitialization(benchmark::State& state)
{
  const std::string s = "Hello world";

  while (state.KeepRunning())
  {
    VariantType v = std::move(s);
  }
}

// -----------------------------------------------------------------------------

template <typename VariantType>
static void BenchmarkVariantEqualityComparison(benchmark::State& state)
{
  VariantType v1 = "Hello";
  VariantType v2 = "Greetings";

  bool res;

  while (state.KeepRunning())
  {
    res = v1 == v2;
  }
}

// -----------------------------------------------------------------------------

static void BenchmarkVariantGet_Boost(benchmark::State& state)
{
  BoostVariant v = std::vector<int> { 1, 2, 3, 4, 5 };

  while (state.KeepRunning())
  {
    boost::get<std::vector<int>>(v);
  }
}

// -----------------------------------------------------------------------------

static void BenchmarkVariantGet_coreVM(benchmark::State& state)
{
  CorevmVariant v = std::vector<int> { 1, 2, 3, 4, 5 };

  while (state.KeepRunning())
  {
    v.get<std::vector<int>>();
  }
}

// -----------------------------------------------------------------------------

template <class VisitorType>
struct tostring_visitor : public VisitorType
{
  std::string operator()(const int& val) const
  {
    return std::to_string(val);
  }

  std::string operator()(const double& val) const
  {
    char buf[5];
    sprintf(buf, "%.2f", val);

    return std::string(buf);
  }

  std::string operator()(const std::string& val) const
  {
    return val;
  }

  std::string operator()(const std::vector<int>& val) const
  {
    std::stringstream ss;

    ss << '[';
    for (auto itr = val.cbegin(); itr != val.cend(); ++itr)
    {
      ss << *itr;

      auto itr_ = itr;
      if (++itr_ != val.cend())
      {
        ss << ", ";
      }
    }
    ss << ']';

    return ss.str();
  }
};

// -----------------------------------------------------------------------------

static void BenchmarkVariantUnaryVisitor_Boost(benchmark::State& state)
{
  BoostVariant v1 = (int)1;
  BoostVariant v2 = (double)3.14;
  BoostVariant v3 = std::string("Hello world");
  BoostVariant v4 = std::vector<int> { 1, 2, 3 };

  while (state.KeepRunning())
  {
    boost::apply_visitor(tostring_visitor<boost::static_visitor<std::string>>(), v1);
    boost::apply_visitor(tostring_visitor<boost::static_visitor<std::string>>(), v2);
    boost::apply_visitor(tostring_visitor<boost::static_visitor<std::string>>(), v3);
    boost::apply_visitor(tostring_visitor<boost::static_visitor<std::string>>(), v4);
  }
}

// -----------------------------------------------------------------------------

static void BenchmarkVariantUnaryVisitor_coreVM(benchmark::State& state)
{
  CorevmVariant v1 = (int)1;
  CorevmVariant v2 = (double)3.14;
  CorevmVariant v3 = std::string("Hello world");
  CorevmVariant v4 = std::vector<int> { 1, 2, 3 };

  while (state.KeepRunning())
  {
    corevm::types::variant::apply_visitor(tostring_visitor<corevm::types::variant::static_visitor<std::string>>(), v1);
    corevm::types::variant::apply_visitor(tostring_visitor<corevm::types::variant::static_visitor<std::string>>(), v2);
    corevm::types::variant::apply_visitor(tostring_visitor<corevm::types::variant::static_visitor<std::string>>(), v3);
    corevm::types::variant::apply_visitor(tostring_visitor<corevm::types::variant::static_visitor<std::string>>(), v4);
  }
}

// -----------------------------------------------------------------------------

template <typename VisitorType>
struct equality_visitor : public VisitorType
{
  template <typename T>
  bool operator() (const T& lhs, const T& rhs) const
  {
    return lhs == rhs;
  }

  template <typename T, typename T2>
  bool operator() (const T& lhs, const T2& rhs) const
  {
    return false;
  }
};

// -----------------------------------------------------------------------------

static void BenchmarkVariantBinaryVisitor_Boost(benchmark::State& state)
{
  BoostVariant v1 = (int)1;
  BoostVariant v2 = std::vector<int> { 1, 2, 3 };

  while (state.KeepRunning())
  {
    boost::apply_visitor(equality_visitor<boost::static_visitor<bool>>(), v1, v2);
  }
}

// -----------------------------------------------------------------------------

static void BenchmarkVariantBinaryVisitor_coreVM(benchmark::State& state)
{
  CorevmVariant v1 = (int)1;
  CorevmVariant v2 = std::vector<int> { 1, 2, 3 };

  while (state.KeepRunning())
  {
    corevm::types::variant::apply_visitor(
      equality_visitor<corevm::types::variant::static_visitor<bool>>(), v1, v2);
  }
}

// -----------------------------------------------------------------------------

BENCHMARK_TEMPLATE(BenchmarkVariantEmptyInitialization, BoostVariant);
BENCHMARK_TEMPLATE(BenchmarkVariantEmptyInitialization, CorevmVariant);

BENCHMARK_TEMPLATE(BenchmarkVariantCopyInitialization, BoostVariant);
BENCHMARK_TEMPLATE(BenchmarkVariantCopyInitialization, CorevmVariant);

BENCHMARK_TEMPLATE(BenchmarkVariantMoveInitialization, BoostVariant);
BENCHMARK_TEMPLATE(BenchmarkVariantMoveInitialization, CorevmVariant);

BENCHMARK_TEMPLATE(BenchmarkVariantMoveValueInitialization, BoostVariant);
BENCHMARK_TEMPLATE(BenchmarkVariantMoveValueInitialization, CorevmVariant);

BENCHMARK_TEMPLATE(BenchmarkVariantAssignmentInitialization, BoostVariant);
BENCHMARK_TEMPLATE(BenchmarkVariantAssignmentInitialization, CorevmVariant);

BENCHMARK_TEMPLATE(BenchmarkVariantValueAssignmentInitialization, BoostVariant);
BENCHMARK_TEMPLATE(BenchmarkVariantValueAssignmentInitialization, CorevmVariant);

BENCHMARK_TEMPLATE(BenchmarkVariantMoveValueAssignmentInitialization, BoostVariant);
BENCHMARK_TEMPLATE(BenchmarkVariantMoveValueAssignmentInitialization, CorevmVariant);

BENCHMARK_TEMPLATE(BenchmarkVariantEqualityComparison, BoostVariant);
BENCHMARK_TEMPLATE(BenchmarkVariantEqualityComparison, CorevmVariant);

BENCHMARK(BenchmarkVariantGet_Boost);
BENCHMARK(BenchmarkVariantGet_coreVM);

BENCHMARK(BenchmarkVariantUnaryVisitor_Boost);
BENCHMARK(BenchmarkVariantUnaryVisitor_coreVM);

BENCHMARK(BenchmarkVariantBinaryVisitor_Boost);
BENCHMARK(BenchmarkVariantBinaryVisitor_coreVM);

// -----------------------------------------------------------------------------
