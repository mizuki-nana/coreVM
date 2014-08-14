#ifndef COREVM_SIGHANDLER_REGISTRAR_H_
#define COREVM_SIGHANDLER_REGISTRAR_H_

#include <csignal>
#include <setjmp.h>
#include <unordered_map>
#include "process.h"
#include "sighandler.h"


namespace corevm {


namespace runtime {


typedef struct sighandler_wrapper {
  corevm::runtime::sighandler* handler;
} sighandler_wrapper;


class sighandler_registrar {
public:
  static void init(corevm::runtime::process*);

  static sigjmp_buf& get_sigjmp_env();
  static bool sig_raised;

  // Must be called after `init`.
  static void ignore(sig_atomic_t);

  static void handle_signal(int);

protected:
  static corevm::runtime::process* process;
  static const std::unordered_map<sig_atomic_t, sighandler_wrapper> handler_map;
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_SIGHANDLER_REGISTRAR_H_ */
