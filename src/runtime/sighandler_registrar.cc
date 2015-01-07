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
#include "../../include/runtime/sighandler_registrar.h"

#include "../../include/runtime/sighandler.h"


corevm::runtime::process* corevm::runtime::sighandler_registrar::process = nullptr;


bool corevm::runtime::sighandler_registrar::sig_raised = false;


sigjmp_buf _env;


sigjmp_buf&
corevm::runtime::sighandler_registrar::get_sigjmp_env()
{
  return _env;
}

const std::unordered_map<sig_atomic_t, corevm::runtime::sighandler_wrapper> \
  corevm::runtime::sighandler_registrar::handler_map
{
  //-----------------  Arithmetic and execution signals -----------------------/

  { SIGFPE,     { .handler=new sighandler_SIGFPE()    } },
  { SIGKILL,    { .handler=new sighandler_SIGILL()    } },
  { SIGSEGV,    { .handler=new sighandler_SIGSEGV()   } },
  { SIGBUS,     { .handler=new sighandler_SIGBUS()    } },

  //---------------------- Termination signals --------------------------------/

  { SIGABRT,    { .handler=new sighandler_SIGABRT()   } },
  { SIGINT,     { .handler=new sighandler_SIGINT()    } },
  { SIGTERM,    { .handler=new sighandler_SIGTERM()   } },
  { SIGQUIT,    { .handler=new sighandler_SIGQUIT()   } },

  //------------------------ Alarm signals ------------------------------------/

  { SIGALRM,    { .handler=new sighandler_SIGALRM()   } },
  { SIGVTALRM,  { .handler=new sighandler_SIGVTALRM() } },
  { SIGPROF,    { .handler=new sighandler_SIGPROF()   } },

  //---------------------- Operation error signals ----------------------------/

  { SIGPIPE,    { .handler=new sighandler_SIGPIPE()   } },
  { SIGXCPU,    { .handler=new sighandler_SIGXCPU()   } },
  { SIGXFSZ,    { .handler=new sighandler_SIGXFSZ()   } },

  //-------------------- Asynchronous I/O signals -----------------------------/

  { SIGIO,      { .handler=new sighandler_SIGIO()     } },
  { SIGURG,     { .handler=new sighandler_SIGURG()    } },
};


void
corevm::runtime::sighandler_registrar::init(corevm::runtime::process* process)
{
  corevm::runtime::sighandler_registrar::process = process;

  for(
    auto itr = corevm::runtime::sighandler_registrar::handler_map.begin();
    itr != corevm::runtime::sighandler_registrar::handler_map.end();
    ++itr
  ) {
    sig_atomic_t sig = itr->first;
    signal(sig, corevm::runtime::sighandler_registrar::handle_signal);
  }
}

void
corevm::runtime::sighandler_registrar::ignore(sig_atomic_t sig)
{
  signal(sig, SIG_IGN);
}

void
corevm::runtime::sighandler_registrar::handle_signal(int signum)
{
  auto itr = corevm::runtime::sighandler_registrar::handler_map.find(signum);

  corevm::runtime::sighandler *handler =
    itr != corevm::runtime::sighandler_registrar::handler_map.end() ?
    itr->second.handler : nullptr;

  corevm::runtime::sighandler_registrar::process->handle_signal(
    signum,
    handler
  );

  corevm::runtime::sighandler_registrar::sig_raised = true;

  siglongjmp(corevm::runtime::sighandler_registrar::get_sigjmp_env(), 1);
}
