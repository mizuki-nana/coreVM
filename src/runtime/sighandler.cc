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
sighandler::~sighandler()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

void
sighandler_SIGFPE::handle_signal(
  sig_atomic_t /* signum */, process& /* process */)
{
  THROW(execution_signal_error("SIGFPE"));
}

// -----------------------------------------------------------------------------

void
sighandler_SIGILL::handle_signal(
  sig_atomic_t /* signum */, process& /* process */)
{
  THROW(execution_signal_error("SIGILL"));
}

// -----------------------------------------------------------------------------

void
sighandler_SIGSEGV::handle_signal(
  sig_atomic_t /* signum */, process& /* process */)
{
  THROW(execution_signal_error("SIGSEGV"));
}

// -----------------------------------------------------------------------------

void
sighandler_SIGBUS::handle_signal(
  sig_atomic_t /* signum */, process& /* process */)
{
  THROW(execution_signal_error("SIGBUS"));
}

// -----------------------------------------------------------------------------

void
sighandler_SIGABRT::handle_signal(
  sig_atomic_t /* signum */, process& /* process */)
{
  THROW(termination_signal_error("SIGABRT"));
}

// -----------------------------------------------------------------------------

void
sighandler_SIGINT::handle_signal(
  sig_atomic_t /* signum */, process& /* process */)
{
  THROW(termination_signal_error("SIGINT"));
}

// -----------------------------------------------------------------------------

void
sighandler_SIGTERM::handle_signal(
  sig_atomic_t /* signum */, process& /* process */)
{
  THROW(termination_signal_error("SIGTERM"));
}

// -----------------------------------------------------------------------------

void
sighandler_SIGQUIT::handle_signal(
  sig_atomic_t /* signum */, process& /* process */)
{
  THROW(termination_signal_error("SIGQUIT"));
}

// -----------------------------------------------------------------------------

void
sighandler_SIGALRM::handle_signal(
  sig_atomic_t /* signum */, process& /* process */)
{
  // Do nothing.
}

// -----------------------------------------------------------------------------

void
sighandler_SIGVTALRM::handle_signal(
  sig_atomic_t /* signum */, process& /* process */)
{
  // Do nothing.
}

// -----------------------------------------------------------------------------

void
sighandler_SIGPROF::handle_signal(
  sig_atomic_t /* signum */, process& /* process */)
{
  // Do nothing.
}

// -----------------------------------------------------------------------------

void
sighandler_SIGPIPE::handle_signal(
  sig_atomic_t /* signum */, process& /* process */)
{
  THROW(operation_signal_error("SIGPIPE"));
}

// -----------------------------------------------------------------------------

void
sighandler_SIGLOST::handle_signal(
  sig_atomic_t /* signum */, process& /* process */)
{
  THROW(operation_signal_error("SIGLOST"));
}

// -----------------------------------------------------------------------------

void
sighandler_SIGXCPU::handle_signal(
  sig_atomic_t /* signum */, process& /* process */)
{
  THROW(operation_signal_error("SIGXCPU"));
}

// -----------------------------------------------------------------------------

void
sighandler_SIGXFSZ::handle_signal(
  sig_atomic_t /* signum */, process& /* process */)
{
  THROW(operation_signal_error("SIGXFSZ"));
}

// -----------------------------------------------------------------------------

void
sighandler_SIGIO::handle_signal(
  sig_atomic_t /* signum */, process& /* process */)
{
  THROW(io_signal_error("SIGIO"));
}

// -----------------------------------------------------------------------------

void
sighandler_SIGURG::handle_signal(
  sig_atomic_t /* signum */, process& /* process */)
{
  THROW(io_signal_error("SIGURG"));
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */
