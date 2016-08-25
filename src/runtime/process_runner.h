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
#ifndef COREVM_PROCESS_RUNNER_H_
#define COREVM_PROCESS_RUNNER_H_

#include "common.h"
#include "corevm/macros.h"

#include <cstdint>

#include <sys/time.h>


namespace corevm {
namespace runtime {

/** Forward declaration of `corevm::runtime::Process` */
class Process;


class ProcessRunner
{
public:
  explicit ProcessRunner(Process& process);

  ProcessRunner(Process& process,
    uint32_t interval_millisec = COREVM_DEFAULT_GC_INTERVAL_MILLISECOND);

  ~ProcessRunner();

  bool run();

  Process& process();

private:
  void init(uint32_t interval_millisec);

  static ProcessRunner* registered_instance;

  bool set_timer();

  static void tick_handler(int);

  Process& m_process;
  struct itimerval m_interval;
};

} /* end namespace runtime */
} /* end namespace corevm */


#endif /* COREVM_PROCESS_RUNNER_H_ */
