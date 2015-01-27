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
#include "../../include/runtime/process_runner.h"

#include "../../include/runtime/process.h"

#include <sneaker/threading/fixed_time_interval_daemon_service.h>


const int COREVM_PROCESS_DEFAULT_PAUSE_TIME = 10;


const int COREVM_PROCESS_DEFAULT_MAX_RUN_ITERATIONS = -1;


corevm::runtime::process_runner::process_runner(
  corevm::runtime::process& process
) :
  sneaker::threading::fixed_time_interval_daemon_service(
    COREVM_PROCESS_DEFAULT_PAUSE_TIME,
    corevm::runtime::process_runner::tick_handler,
    false,
    COREVM_PROCESS_DEFAULT_MAX_RUN_ITERATIONS
  ),
  m_process(process)
{
  // Do nothing here.
}

bool
corevm::runtime::process_runner::start()
{
  bool res = sneaker::threading::fixed_time_interval_daemon_service::start();

  if (res) {
    m_process.start();
  }

  return res;
}

void
corevm::runtime::process_runner::tick_handler(void* arg)
{
  assert(arg);
  corevm::runtime::process_runner* runner = static_cast<corevm::runtime::process_runner*>(arg);
  runner->gc();
}

void
corevm::runtime::process_runner::gc()
{
  m_process.maybe_gc();
}
