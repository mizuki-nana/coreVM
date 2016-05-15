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

#include "process.h"

#include <csignal>


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

ProcessRunner* ProcessRunner::registered_instance = nullptr;

// -----------------------------------------------------------------------------

ProcessRunner::ProcessRunner(Process& process, uint32_t interval_millisec)
  :
  m_process(process)
{
  init(interval_millisec);
}

// -----------------------------------------------------------------------------

ProcessRunner::~ProcessRunner()
{
  registered_instance = nullptr;
}

// -----------------------------------------------------------------------------

void
ProcessRunner::init(uint32_t interval_millisec)
{
  registered_instance = this;

  m_interval.it_interval.tv_usec = interval_millisec * 1000;
  m_interval.it_interval.tv_sec = 0;
  m_interval.it_value.tv_usec = m_interval.it_interval.tv_usec;
  m_interval.it_value.tv_sec = 0;
}

// -----------------------------------------------------------------------------

bool
ProcessRunner::run()
{
  bool res = true;

  res = signal(SIGALRM, tick_handler) == 0;

  if (!res)
  {
    return res;
  }

  res = set_timer();

  m_process.run();

  return res;
}

// -----------------------------------------------------------------------------

Process&
ProcessRunner::process()
{
  return m_process;
}

// -----------------------------------------------------------------------------

bool
ProcessRunner::set_timer()
{
  return setitimer(ITIMER_REAL, &m_interval, NULL) == 0;
}

// -----------------------------------------------------------------------------

void
ProcessRunner::tick_handler(int)
{
  if (ProcessRunner::registered_instance)
  {
    if (registered_instance->process().should_gc())
    {
      registered_instance->process().set_do_gc();
    }
  }
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */
