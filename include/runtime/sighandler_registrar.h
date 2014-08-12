#ifndef COREVM_SIGHANDLER_REGISTRAR_H_
#define COREVM_SIGHANDLER_REGISTRAR_H_

#include <csignal>
#include <unordered_map>
#include "process.h"


namespace corevm {


namespace runtime {


class sighandler_registrar {
public:
  static void init(corevm::runtime::process*);

  // Must be called after `init`.
  static void ignore(sig_atomic_t);

  /************************** HANDLER FUNCTIONS *******************************/
  // Arithmetic and execution signals
  static void handle_SIGFPE(int);
  static void handle_SIGILL(int);
  static void handle_SIGSEGV(int);
  // Termination signals
  static void handle_SIGABRT(int);
  static void handle_SIGINT(int);
  static void handle_SIGTERM(int);
  static void handle_SIGQUIT(int);
  // Alarm signals
  static void handle_SIGALRM(int);
  static void handle_SIGVTALRM(int);
  static void handle_SIGPROF(int);
  // Operation error signals
  static void handle_SIGPIPE(int);
  static void handle_SIGLOST(int);
  static void handle_SIGXCPU(int);
  static void handle_SIGXFSZ(int);
  // Asynchronous I/O signals
  static void handle_SIGIO(int);
  static void handle_SIGURG(int);

protected:
  static corevm::runtime::process* process;
  static const std::unordered_map<sig_atomic_t, sighandler_t> handler_map;
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_SIGHANDLER_REGISTRAR_H_ */
