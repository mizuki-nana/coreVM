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
#include "instr_benchmarks_fixture.h"

#include "runtime/catch_site.h"
#include "runtime/closure.h"
#include "runtime/common.h"
#include "runtime/compartment.h"
#include "runtime/frame.h"
#include "runtime/invocation_ctx.h"
#include "runtime/loc_info.h"
#include "runtime/process.h"
#include "runtime/vector.h"

#include <cassert>


namespace corevm {


namespace benchmarks {


// -----------------------------------------------------------------------------

static const char* DUMMY_PATH = "./example.core";

// -----------------------------------------------------------------------------

const uint64_t BENCHMARK_PROCESS_HEAP_SIZE = 1024 * 1024 * 512;
const uint64_t BENCHMARK_PROCESS_POOL_SIZE = 1024 * 1024 * 512;

// -----------------------------------------------------------------------------

InstrBenchmarksFixture::InstrBenchmarksFixture()
  :
  m_process(BENCHMARK_PROCESS_HEAP_SIZE, BENCHMARK_PROCESS_POOL_SIZE)
{
  init();
}

// -----------------------------------------------------------------------------

InstrBenchmarksFixture::InstrBenchmarksFixture(
  const runtime::Process::Options& opts)
  :
  m_process(opts)
{
  init();
}

// -----------------------------------------------------------------------------

void
InstrBenchmarksFixture::init()
{
  corevm::runtime::Compartment compartment(DUMMY_PATH);

  // Please note that setting this vector too long can cause runtime errors
  // in certain benchmarks.
  corevm::runtime::Vector vector {
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
    corevm::runtime::Instr(0, 0, 0),
  };

  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::Closure closure(
    "__main__",
    0,
    corevm::runtime::NONESET_CLOSURE_ID,
    vector,
    locs,
    catch_sites);

  corevm::runtime::ClosureTable closure_table { closure };
  compartment.set_closure_table(std::move(closure_table));

  auto compartment_id = m_process.insert_compartment(compartment);

  // TODO: consolidate this code with `process::pre_start()`.
  corevm::runtime::ClosureCtx ctx(compartment_id, closure.id);

  corevm::runtime::Compartment* compartment_ptr = nullptr;
  m_process.get_compartment(compartment_id, &compartment_ptr);
  assert(compartment_ptr);

  m_process.emplace_frame(ctx, compartment_ptr, &closure_table.front());
  m_process.emplace_invocation_ctx(ctx, compartment_ptr, &closure_table.front());

  m_process.set_pc(0);
}

// -----------------------------------------------------------------------------

corevm::runtime::Process&
InstrBenchmarksFixture::process()
{
  return m_process;
}

// -----------------------------------------------------------------------------


} /* endnamespace benchmarks */


} /* end namespace corevm */
