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
#include "sighandler.h"

#include "errors.h"
#include "process.h"
#include "corevm/macros.h"


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

/* virtual */
SigHandler::~SigHandler()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

void
SigHandler_SIGFPE::handle_signal(
  sig_atomic_t /* signum */, Process& /* process */)
{
  THROW(ExecutionSignalError("SIGFPE"));
}

// -----------------------------------------------------------------------------

void
SigHandler_SIGILL::handle_signal(
  sig_atomic_t /* signum */, Process& /* process */)
{
  THROW(ExecutionSignalError("SIGILL"));
}

// -----------------------------------------------------------------------------

void
SigHandler_SIGSEGV::handle_signal(
  sig_atomic_t /* signum */, Process& /* process */)
{
  THROW(ExecutionSignalError("SIGSEGV"));
}

// -----------------------------------------------------------------------------

void
SigHandler_SIGBUS::handle_signal(
  sig_atomic_t /* signum */, Process& /* process */)
{
  THROW(ExecutionSignalError("SIGBUS"));
}

// -----------------------------------------------------------------------------

void
SigHandler_SIGABRT::handle_signal(
  sig_atomic_t /* signum */, Process& /* process */)
{
  THROW(TerminationSignalError("SIGABRT"));
}

// -----------------------------------------------------------------------------

void
SigHandler_SIGINT::handle_signal(
  sig_atomic_t /* signum */, Process& /* process */)
{
  THROW(TerminationSignalError("SIGINT"));
}

// -----------------------------------------------------------------------------

void
SigHandler_SIGTERM::handle_signal(
  sig_atomic_t /* signum */, Process& /* process */)
{
  THROW(TerminationSignalError("SIGTERM"));
}

// -----------------------------------------------------------------------------

void
SigHandler_SIGQUIT::handle_signal(
  sig_atomic_t /* signum */, Process& /* process */)
{
  THROW(TerminationSignalError("SIGQUIT"));
}

// -----------------------------------------------------------------------------

void
SigHandler_SIGALRM::handle_signal(
  sig_atomic_t /* signum */, Process& /* process */)
{
  // Do nothing.
}

// -----------------------------------------------------------------------------

void
SigHandler_SIGVTALRM::handle_signal(
  sig_atomic_t /* signum */, Process& /* process */)
{
  // Do nothing.
}

// -----------------------------------------------------------------------------

void
SigHandler_SIGPROF::handle_signal(
  sig_atomic_t /* signum */, Process& /* process */)
{
  // Do nothing.
}

// -----------------------------------------------------------------------------

void
SigHandler_SIGPIPE::handle_signal(
  sig_atomic_t /* signum */, Process& /* process */)
{
  THROW(OperationSignalError("SIGPIPE"));
}

// -----------------------------------------------------------------------------

void
SigHandler_SIGLOST::handle_signal(
  sig_atomic_t /* signum */, Process& /* process */)
{
  THROW(OperationSignalError("SIGLOST"));
}

// -----------------------------------------------------------------------------

void
SigHandler_SIGXCPU::handle_signal(
  sig_atomic_t /* signum */, Process& /* process */)
{
  THROW(OperationSignalError("SIGXCPU"));
}

// -----------------------------------------------------------------------------

void
SigHandler_SIGXFSZ::handle_signal(
  sig_atomic_t /* signum */, Process& /* process */)
{
  THROW(OperationSignalError("SIGXFSZ"));
}

// -----------------------------------------------------------------------------

void
SigHandler_SIGIO::handle_signal(
  sig_atomic_t /* signum */, Process& /* process */)
{
  THROW(IoSignalError("SIGIO"));
}

// -----------------------------------------------------------------------------

void
SigHandler_SIGURG::handle_signal(
  sig_atomic_t /* signum */, Process& /* process */)
{
  THROW(IoSignalError("SIGURG"));
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */
