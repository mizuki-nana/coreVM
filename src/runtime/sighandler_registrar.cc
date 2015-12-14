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
#include "sighandler_registrar.h"

#include "process.h"
#include "sighandler.h"

#include <memory>
#include <string>


namespace corevm {


namespace runtime {


// -----------------------------------------------------------------------------

process* sighandler_registrar::process = nullptr;

// -----------------------------------------------------------------------------

bool sighandler_registrar::sig_raised = false;

// -----------------------------------------------------------------------------

static sigjmp_buf _env;

// -----------------------------------------------------------------------------

sigjmp_buf&
sighandler_registrar::get_sigjmp_env()
{
  return _env;
}

// -----------------------------------------------------------------------------

#if defined(__clang__) and __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc99-extensions"
#endif

const std::unordered_map<sig_atomic_t, sighandler_wrapper> \
  sighandler_registrar::handler_map
{

  /* --------------- Arithmetic and execution signals ---------------------- */

  { SIGFPE,     { .handler=std::make_shared<sighandler_SIGFPE>()    } },
  { SIGKILL,    { .handler=std::make_shared<sighandler_SIGILL>()    } },
  { SIGSEGV,    { .handler=std::make_shared<sighandler_SIGSEGV>()   } },
  { SIGBUS,     { .handler=std::make_shared<sighandler_SIGBUS>()    } },

  /* ----------------------- Termination signals ---------------------------- */

  { SIGABRT,    { .handler=std::make_shared<sighandler_SIGABRT>()   } },
  { SIGINT,     { .handler=std::make_shared<sighandler_SIGINT>()    } },
  { SIGTERM,    { .handler=std::make_shared<sighandler_SIGTERM>()   } },
  { SIGQUIT,    { .handler=std::make_shared<sighandler_SIGQUIT>()   } },

  /* ------------------------ Alarm signals --------------------------------- */

  { SIGALRM,    { .handler=std::make_shared<sighandler_SIGALRM>()   } },
  { SIGVTALRM,  { .handler=std::make_shared<sighandler_SIGVTALRM>() } },
  { SIGPROF,    { .handler=std::make_shared<sighandler_SIGPROF>()   } },

  /* --------------------- Operation error signals -------------------------- */

  { SIGPIPE,    { .handler=std::make_shared<sighandler_SIGPIPE>()   } },
  { SIGXCPU,    { .handler=std::make_shared<sighandler_SIGXCPU>()   } },
  { SIGXFSZ,    { .handler=std::make_shared<sighandler_SIGXFSZ>()   } },

  /* -------------------- Asynchronous I/O signals -------------------------- */

  { SIGIO,      { .handler=std::make_shared<sighandler_SIGIO>()     } },
  { SIGURG,     { .handler=std::make_shared<sighandler_SIGURG>()    } },

};

#if defined(__clang__) and __clang__
  #pragma clang diagnostic pop
#endif  /* #if defined(__clang__) and __clang__ */

// -----------------------------------------------------------------------------

const std::unordered_map<std::string, sig_atomic_t> \
  sighandler_registrar::sig_value_to_str_map
{

  /* ---------------- Arithmetic and execution signals ---------------------- */

  { "SIGFPE",     SIGFPE    },
  { "SIGKILL",    SIGKILL   },
  { "SIGSEGV",    SIGSEGV   },
  { "SIGBUS",     SIGBUS    },

  /* ----------------------- Termination signals ---------------------------- */

  { "SIGABRT",    SIGABRT   },
  { "SIGINT",     SIGINT    },
  { "SIGTERM",    SIGTERM   },
  { "SIGQUIT",    SIGQUIT   },

  /* ------------------------- Alarm signals -------------------------------- */

  { "SIGALRM",    SIGALRM   },
  { "SIGVTALRM",  SIGVTALRM },
  { "SIGPROF",    SIGPROF   },

  /* --------------------- Operation error signals -------------------------- */

  { "SIGPIPE",    SIGPIPE   },
  { "SIGXCPU",    SIGXCPU   },
  { "SIGXFSZ",    SIGXFSZ   },

  /* -------------------- Asynchronous I/O signals -------------------------- */

  { "SIGIO",      SIGIO     },
  { "SIGURG",     SIGURG    },

};

// -----------------------------------------------------------------------------

bool
sighandler_registrar::is_sig_raised()
{
  return sighandler_registrar::sig_raised;
}

// -----------------------------------------------------------------------------

void
sighandler_registrar::clear_sig_raised()
{
  sighandler_registrar::sig_raised = false;
}

// -----------------------------------------------------------------------------

void
sighandler_registrar::init(runtime::process* process_)
{
  sighandler_registrar::process = process_;

  for (
    auto itr = sighandler_registrar::handler_map.begin();
    itr != sighandler_registrar::handler_map.end();
    ++itr)
  {
    sig_atomic_t sig = itr->first;
    signal(sig, sighandler_registrar::handle_signal);
  }
}

// -----------------------------------------------------------------------------

void
sighandler_registrar::ignore(sig_atomic_t sig)
{
  signal(sig, SIG_IGN);
}

// -----------------------------------------------------------------------------

void
sighandler_registrar::handle_signal(int signum)
{
  auto itr = sighandler_registrar::handler_map.find(signum);

  sighandler *handler =
    itr != sighandler_registrar::handler_map.end() ?
    itr->second.handler.get() : nullptr;

  sighandler_registrar::process->handle_signal(
    signum,
    handler
  );

  sighandler_registrar::sig_raised = true;

  siglongjmp(sighandler_registrar::get_sigjmp_env(), 1);
}

// -----------------------------------------------------------------------------

sig_atomic_t
sighandler_registrar::get_sig_value_from_string(const std::string& str)
{
  return sig_value_to_str_map.at(str);
}

// -----------------------------------------------------------------------------


} /* end namespace runtime */


} /* end namespace corevm */
