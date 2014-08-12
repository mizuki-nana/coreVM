#include "../../include/runtime/sighandler_registrar.h"


corevm::runtime::process* corevm::runtime::sighandler_registrar::process = nullptr;

const std::unordered_map<sig_atomic_t, sighandler_t> corevm::runtime::sighandler_registrar::handler_map {
  // Arithmetic and execution signals
  {SIGFPE, handle_SIGFPE},
  {SIGKILL, handle_SIGILL},
  {SIGSEGV, handle_SIGSEGV},
  // Termination signals
  {SIGABRT, handle_SIGABRT},
  {SIGINT, handle_SIGINT},
  {SIGTERM, handle_SIGTERM},
  {SIGQUIT, handle_SIGQUIT},
  // Alarm signals
  {SIGALRM, handle_SIGALRM},
  {SIGVTALRM, handle_SIGVTALRM},
  {SIGPROF, handle_SIGPROF},
  // Operation error signals
  {SIGPIPE, handle_SIGPIPE},
  {SIGXCPU, handle_SIGXCPU},
  {SIGXFSZ, handle_SIGXFSZ},
  // Asynchronous I/O signals
  {SIGIO, handle_SIGIO},
  {SIGURG, handle_SIGURG},
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
    sighandler_t handler = itr->second;
    signal(sig, handler);
  }
}

void
corevm::runtime::sighandler_registrar::ignore(sig_atomic_t sig)
{
  signal(sig, SIG_IGN);
}

void
corevm::runtime::sighandler_registrar::handle_SIGFPE(int signum)
{
  corevm::runtime::sighandler_registrar::process->handle_SIGFPE();
}

void
corevm::runtime::sighandler_registrar::handle_SIGILL(int signum)
{
  corevm::runtime::sighandler_registrar::process->handle_SIGILL();
}

void
corevm::runtime::sighandler_registrar::handle_SIGSEGV(int signum)
{
  corevm::runtime::sighandler_registrar::process->handle_SIGSEGV();
}

void
corevm::runtime::sighandler_registrar::handle_SIGABRT(int signum)
{
  corevm::runtime::sighandler_registrar::process->handle_SIGABRT();
}

void
corevm::runtime::sighandler_registrar::handle_SIGINT(int signum)
{
  corevm::runtime::sighandler_registrar::process->handle_SIGINT();
}

void
corevm::runtime::sighandler_registrar::handle_SIGTERM(int signum)
{
  corevm::runtime::sighandler_registrar::process->handle_SIGTERM();
}

void
corevm::runtime::sighandler_registrar::handle_SIGQUIT(int signum)
{
  corevm::runtime::sighandler_registrar::process->handle_SIGQUIT();
}

void
corevm::runtime::sighandler_registrar::handle_SIGALRM(int signum)
{
  corevm::runtime::sighandler_registrar::process->handle_SIGALRM();
}

void
corevm::runtime::sighandler_registrar::handle_SIGVTALRM(int signum)
{
  corevm::runtime::sighandler_registrar::process->handle_SIGVTALRM();
}

void
corevm::runtime::sighandler_registrar::handle_SIGPROF(int signum)
{
  corevm::runtime::sighandler_registrar::process->handle_SIGPROF();
}

void
corevm::runtime::sighandler_registrar::handle_SIGPIPE(int signum)
{
  corevm::runtime::sighandler_registrar::process->handle_SIGPIPE();
}

void
corevm::runtime::sighandler_registrar::handle_SIGXCPU(int signum)
{
  corevm::runtime::sighandler_registrar::process->handle_SIGXCPU();
}

void
corevm::runtime::sighandler_registrar::handle_SIGXFSZ(int signum)
{
  corevm::runtime::sighandler_registrar::process->handle_SIGXFSZ();
}

void
corevm::runtime::sighandler_registrar::handle_SIGIO(int signum)
{
  corevm::runtime::sighandler_registrar::process->handle_SIGIO();
}

void
corevm::runtime::sighandler_registrar::handle_SIGURG(int signum)
{
  corevm::runtime::sighandler_registrar::process->handle_SIGURG();
}
