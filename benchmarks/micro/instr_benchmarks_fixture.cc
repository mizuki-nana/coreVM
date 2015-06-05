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
#include "instr_benchmarks_fixture.h"

#include "runtime/closure.h"
#include "runtime/common.h"
#include "runtime/compartment.h"
#include "runtime/frame.h"
#include "runtime/invocation_ctx.h"
#include "runtime/process.h"
#include "runtime/vector.h"


// -----------------------------------------------------------------------------

const char* DUMMY_PATH = "./example.core";

// -----------------------------------------------------------------------------

const uint64_t BENCHMARKS_PROCESS_HEAP_ALLOC_SIZE = 2048;
const uint64_t BENCHMARKS_PROCESS_POOL_ALLOC_SIZE = 2048;

// -----------------------------------------------------------------------------

corevm::benchmarks::instr_benchmarks_fixture::instr_benchmarks_fixture()
  :
  m_process(BENCHMARKS_PROCESS_HEAP_ALLOC_SIZE, BENCHMARKS_PROCESS_POOL_ALLOC_SIZE)
{
  init();
}

// -----------------------------------------------------------------------------

void
corevm::benchmarks::instr_benchmarks_fixture::init()
{
  corevm::runtime::compartment compartment(DUMMY_PATH);

  corevm::runtime::vector vector;

  corevm::runtime::closure closure {
    .id = 0,
    .parent_id = corevm::runtime::NONESET_CLOSURE_ID,
    .vector = vector
  };

  corevm::runtime::closure_table closure_table { closure };
  compartment.set_closure_table(std::move(closure_table));

  auto compartment_id = m_process.insert_compartment(compartment);

  corevm::runtime::closure_ctx ctx {
    .compartment_id = compartment_id,
    .closure_id = closure.id
  };

  m_process.emplace_frame(ctx, &compartment, &closure_table.front());
  m_process.emplace_invocation_ctx(ctx, &compartment, &closure_table.front());
}

// -----------------------------------------------------------------------------

corevm::runtime::process&
corevm::benchmarks::instr_benchmarks_fixture::process()
{
  return m_process;
}

// -----------------------------------------------------------------------------
