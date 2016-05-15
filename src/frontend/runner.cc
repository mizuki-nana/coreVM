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
#include "runner.h"

#include "errors.h"
#include "binary_bytecode_loader.h"
#include "configuration.h"
#include "corevm/macros.h"
#include "dyobj/errors.h"
#include "runtime/process.h"
#include "runtime/process_runner.h"

#include <boost/format.hpp>
#include <sneaker/utility/stack_trace.h>

#include <cerrno>
#include <cstring>
#include <iostream>


namespace corevm {


namespace frontend {


// -----------------------------------------------------------------------------

#if __DEBUG__
static void
print_stack_trace()
{
  // TODO: [SNEAKER-86] Print stack trace not working well

  static const unsigned int DEFAULT_STACK_LEVEL = 5;
  sneaker::utility::stack_trace::print_stack_trace(std::cerr, DEFAULT_STACK_LEVEL);
}
#endif

// -----------------------------------------------------------------------------

Runner::Runner(const std::string& path, Configuration& configuration)
  :
  m_path(path),
  m_configuration(configuration)
{
}

// -----------------------------------------------------------------------------

int
Runner::run() const noexcept
{
  const uint32_t gc_interval = m_configuration.gc_interval() ? \
    m_configuration.gc_interval() : corevm::runtime::COREVM_DEFAULT_GC_INTERVAL;

  runtime::Process::Options options;

  if (m_configuration.heap_alloc_size())
  {
    options.heap_alloc_size = m_configuration.heap_alloc_size();
  }

  if (m_configuration.pool_alloc_size())
  {
    options.pool_alloc_size = m_configuration.pool_alloc_size();
  }

  if (m_configuration.has_gc_flag())
  {
    options.gc_flag = m_configuration.gc_flag();
  }

  Logger logger(log_mode_to_scheme(m_configuration.log_mode()));

  runtime::Process process(options);
  process.set_logger(&logger);

  BinaryBytecodeLoader loader;

  try
  {
    loader.load(m_path, process);

    const bool res = runtime::ProcessRunner(process, gc_interval).run();

    if (!res)
    {
      std::cerr << "Run failed: " << strerror(errno) << std::endl;

#if __DEBUG__
      print_stack_trace();
#endif

      runtime::Process::unwind_stack(process);

      return -1;
    }
  }
  catch (const corevm::RuntimeError& ex)
  {
    std::cerr << "Runtime error: " << ex.what() << std::endl;
    std::cerr << "Abort" << std::endl;

#if __DEBUG__
    print_stack_trace();
#endif

    runtime::Process::unwind_stack(process);

    return -1;
  }
  catch (const std::exception& ex)
  {
    std::cerr << "Error: " << ex.what() << std::endl;
    std::cerr << "Abort" << std::endl;

#if __DEBUG__
    print_stack_trace();
#endif

    runtime::Process::unwind_stack(process);

    return -1;
  }
  catch (...)
  {
    std::cerr << "Unknown error" << std::endl;
    std::cerr << "Abort" << std::endl;

#if __DEBUG__
    print_stack_trace();
#endif

    runtime::Process::unwind_stack(process);

    return -1;
  }

  return 0;
}

// -----------------------------------------------------------------------------


} /* end namespace frontend */


} /* end namespace corevm */
