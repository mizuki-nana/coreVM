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
#include "process_runner.h"

#include "common.h"
#include "process.h"
#include "signal_handler.h"
#include "corevm/macros.h"

#include <sneaker/threading/fixed_time_interval_daemon_service.h>

#include <cstdint>


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

const int COREVM_PROCESS_DEFAULT_MAX_RUN_ITERATIONS = -1;

// -----------------------------------------------------------------------------

ProcessRunner::ProcessRunner(Process& process)
  :
  sneaker::threading::fixed_time_interval_daemon_service(
    COREVM_DEFAULT_GC_INTERVAL,
    ProcessRunner::tick_handler,
    false,
    COREVM_PROCESS_DEFAULT_MAX_RUN_ITERATIONS),
  m_process(process)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

ProcessRunner::ProcessRunner(Process& process, uint32_t gc_interval)
  :
  sneaker::threading::fixed_time_interval_daemon_service(
    gc_interval,
    ProcessRunner::tick_handler,
    false,
    COREVM_PROCESS_DEFAULT_MAX_RUN_ITERATIONS),
  m_process(process)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

/* virtual */
ProcessRunner::~ProcessRunner()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

bool
ProcessRunner::start()
{
  bool res = sneaker::threading::fixed_time_interval_daemon_service::start();

  if (res)
  {
    SignalHandler::register_process(&m_process);
    m_process.start();
  }

  return res;
}

// -----------------------------------------------------------------------------

void
ProcessRunner::tick_handler(void* arg)
{
#if __DEBUG__
  ASSERT(arg);
#endif

  ProcessRunner* runner =
    static_cast<ProcessRunner*>(arg);

  runner->gc();
}

// -----------------------------------------------------------------------------

void
ProcessRunner::gc()
{
  if (m_process.should_gc())
  {
    m_process.set_do_gc();
  }
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */
