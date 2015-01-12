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
#include "../../include/runtime/sighandler.h"

#include "../../include/runtime/errors.h"
#include "../../include/runtime/process.h"


//------------------- Arithmetic and execution signals ------------------------/

void
corevm::runtime::sighandler_SIGFPE::handle_signal(
  sig_atomic_t signum, corevm::runtime::process& process)
{
  throw corevm::runtime::execution_signal_error("SIGFPE");
}

void
corevm::runtime::sighandler_SIGILL::handle_signal(
  sig_atomic_t signum, corevm::runtime::process& process)
{
  throw corevm::runtime::execution_signal_error("SIGILL");
}

void
corevm::runtime::sighandler_SIGSEGV::handle_signal(
  sig_atomic_t signum, corevm::runtime::process& process)
{
  throw corevm::runtime::execution_signal_error("SIGSEGV");
}

void
corevm::runtime::sighandler_SIGBUS::handle_signal(
  sig_atomic_t signum, corevm::runtime::process& process)
{
  throw corevm::runtime::execution_signal_error("SIGBUS");
}

//----------------------- Termination signals ---------------------------------/

void
corevm::runtime::sighandler_SIGABRT::handle_signal(
  sig_atomic_t signum, corevm::runtime::process& process)
{
  throw corevm::runtime::termination_signal_error("SIGABRT");
}

void
corevm::runtime::sighandler_SIGINT::handle_signal(
  sig_atomic_t signum, corevm::runtime::process& process)
{
  throw corevm::runtime::termination_signal_error("SIGINT");
}

void
corevm::runtime::sighandler_SIGTERM::handle_signal(
  sig_atomic_t signum, corevm::runtime::process& process)
{
  throw corevm::runtime::termination_signal_error("SIGTERM");
}

void
corevm::runtime::sighandler_SIGQUIT::handle_signal(
  sig_atomic_t signum, corevm::runtime::process& process)
{
  throw corevm::runtime::termination_signal_error("SIGQUIT");
}

//-------------------------- Alarm signals ------------------------------------/

void
corevm::runtime::sighandler_SIGALRM::handle_signal(
  sig_atomic_t signum, corevm::runtime::process& process)
{
  // Do nothing.
}

void
corevm::runtime::sighandler_SIGVTALRM::handle_signal(
  sig_atomic_t signum, corevm::runtime::process& process)
{
  // Do nothing.
}

void
corevm::runtime::sighandler_SIGPROF::handle_signal(
  sig_atomic_t signum, corevm::runtime::process& process)
{
  // Do nothing.
}

//------------------------ Operation error signals ----------------------------/

void
corevm::runtime::sighandler_SIGPIPE::handle_signal(
  sig_atomic_t signum, corevm::runtime::process& process)
{
  throw corevm::runtime::operation_signal_error("SIGPIPE");
}

void
corevm::runtime::sighandler_SIGLOST::handle_signal(
  sig_atomic_t signum, corevm::runtime::process& process)
{
  throw corevm::runtime::operation_signal_error("SIGLOST");
}

void
corevm::runtime::sighandler_SIGXCPU::handle_signal(
  sig_atomic_t signum, corevm::runtime::process& process)
{
  throw corevm::runtime::operation_signal_error("SIGXCPU");
}

void
corevm::runtime::sighandler_SIGXFSZ::handle_signal(
  sig_atomic_t signum, corevm::runtime::process& process)
{
  throw corevm::runtime::operation_signal_error("SIGXFSZ");
}

//---------------------- Asynchronous I/O signals -----------------------------/

void
corevm::runtime::sighandler_SIGIO::handle_signal(
  sig_atomic_t signum, corevm::runtime::process& process)
{
  throw corevm::runtime::io_signal_error("SIGIO");
}

void
corevm::runtime::sighandler_SIGURG::handle_signal(
  sig_atomic_t signum, corevm::runtime::process& process)
{
  throw corevm::runtime::io_signal_error("SIGURG");
}
