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
#ifndef COREVM_SIGHANDLER_H_
#define COREVM_SIGHANDLER_H_

#include <signal.h>


namespace corevm {


namespace runtime {


/* Forward declaration of `Process` */
class Process;

// -----------------------------------------------------------------------------

class SigHandler
{
public:
  virtual void handle_signal(sig_atomic_t, Process&) = 0;

  virtual ~SigHandler();
};

// -----------------------------------------------------------------------------


/* ------------------- Arithmetic and execution signals --------------------- */


// -----------------------------------------------------------------------------

class SigHandler_SIGFPE : public SigHandler
{
public:
  virtual void handle_signal(sig_atomic_t, Process&);
};

// -----------------------------------------------------------------------------

class SigHandler_SIGILL : public SigHandler
{
public:
  virtual void handle_signal(sig_atomic_t, Process&);
};

// -----------------------------------------------------------------------------

class SigHandler_SIGSEGV : public SigHandler
{
public:
  virtual void handle_signal(sig_atomic_t, Process&);
};

// -----------------------------------------------------------------------------

class SigHandler_SIGBUS : public SigHandler
{
public:
  virtual void handle_signal(sig_atomic_t, Process&);
};

// -----------------------------------------------------------------------------


/* ------------------------ Termination signals ----------------------------- */


// -----------------------------------------------------------------------------

class SigHandler_SIGABRT : public SigHandler
{
public:
  virtual void handle_signal(sig_atomic_t, Process&);
};

// -----------------------------------------------------------------------------

class SigHandler_SIGINT : public SigHandler
{
public:
  virtual void handle_signal(sig_atomic_t, Process&);
};

// -----------------------------------------------------------------------------

class SigHandler_SIGTERM : public SigHandler
{
public:
  virtual void handle_signal(sig_atomic_t, Process&);
};

// -----------------------------------------------------------------------------

class SigHandler_SIGQUIT : public SigHandler
{
public:
  virtual void handle_signal(sig_atomic_t, Process&);
};

// -----------------------------------------------------------------------------


/* ---------------------------- Alarm signals --------------------------------*/


// -----------------------------------------------------------------------------

class SigHandler_SIGALRM : public SigHandler
{
public:
  virtual void handle_signal(sig_atomic_t, Process&);
};

// -----------------------------------------------------------------------------

class SigHandler_SIGVTALRM : public SigHandler
{
public:
  virtual void handle_signal(sig_atomic_t, Process&);
};

// -----------------------------------------------------------------------------

class SigHandler_SIGPROF : public SigHandler
{
public:
  virtual void handle_signal(sig_atomic_t, Process&);
};

// -----------------------------------------------------------------------------


/* ---------------------- Operation error signals --------------------------- */


// -----------------------------------------------------------------------------

class SigHandler_SIGPIPE : public SigHandler
{
public:
  virtual void handle_signal(sig_atomic_t, Process&);
};

// -----------------------------------------------------------------------------

class SigHandler_SIGLOST : public SigHandler
{
public:
  virtual void handle_signal(sig_atomic_t, Process&);
};

// -----------------------------------------------------------------------------

class SigHandler_SIGXCPU : public SigHandler
{
public:
  virtual void handle_signal(sig_atomic_t, Process&);
};

// -----------------------------------------------------------------------------

class SigHandler_SIGXFSZ : public SigHandler
{
public:
  virtual void handle_signal(sig_atomic_t, Process&);
};

// -----------------------------------------------------------------------------


/* ----------------------- Asynchronous I/O signals ------------------------- */


// -----------------------------------------------------------------------------

class SigHandler_SIGIO : public SigHandler
{
public:
  virtual void handle_signal(sig_atomic_t, Process&);
};

// -----------------------------------------------------------------------------

class SigHandler_SIGURG : public SigHandler
{
public:
  virtual void handle_signal(sig_atomic_t, Process&);
};

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_SIGHANDLER_H_ */
