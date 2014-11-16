#ifndef COREVM_SIGHANDLER_H_
#define COREVM_SIGHANDLER_H_


namespace corevm {


namespace runtime {


class process;


class sighandler {
public:
  virtual void handle_signal(sig_atomic_t, corevm::runtime::process&) = 0;
};


// ------------------- Arithmetic and execution signals -----------------------/


class sighandler_SIGFPE : public sighandler {
public:
  virtual void handle_signal(sig_atomic_t, corevm::runtime::process&);
};


class sighandler_SIGILL : public sighandler {
public:
  virtual void handle_signal(sig_atomic_t, corevm::runtime::process&);
};


class sighandler_SIGSEGV : public sighandler {
public:
  virtual void handle_signal(sig_atomic_t, corevm::runtime::process&);
};


class sighandler_SIGBUS : public sighandler {
public:
  virtual void handle_signal(sig_atomic_t, corevm::runtime::process&);
};


// ------------------------ Termination signals -------------------------------/


class sighandler_SIGABRT : public sighandler {
public:
  virtual void handle_signal(sig_atomic_t, corevm::runtime::process&);
};


class sighandler_SIGINT : public sighandler {
public:
  virtual void handle_signal(sig_atomic_t, corevm::runtime::process&);
};


class sighandler_SIGTERM : public sighandler {
public:
  virtual void handle_signal(sig_atomic_t, corevm::runtime::process&);
};


class sighandler_SIGQUIT : public sighandler {
public:
  virtual void handle_signal(sig_atomic_t, corevm::runtime::process&);
};


// ---------------------------- Alarm signals ---------------------------------/


class sighandler_SIGALRM : public sighandler {
public:
  virtual void handle_signal(sig_atomic_t, corevm::runtime::process&);
};


class sighandler_SIGVTALRM : public sighandler {
public:
  virtual void handle_signal(sig_atomic_t, corevm::runtime::process&);
};


class sighandler_SIGPROF : public sighandler {
public:
  virtual void handle_signal(sig_atomic_t, corevm::runtime::process&);
};


// ---------------------- Operation error signals -----------------------------/


class sighandler_SIGPIPE : public sighandler {
public:
  virtual void handle_signal(sig_atomic_t, corevm::runtime::process&);
};


class sighandler_SIGLOST : public sighandler {
public:
  virtual void handle_signal(sig_atomic_t, corevm::runtime::process&);
};


class sighandler_SIGXCPU : public sighandler {
public:
  virtual void handle_signal(sig_atomic_t, corevm::runtime::process&);
};


class sighandler_SIGXFSZ : public sighandler {
public:
  virtual void handle_signal(sig_atomic_t, corevm::runtime::process&);
};


// ----------------------- Asynchronous I/O signals ---------------------------/


class sighandler_SIGIO : public sighandler {
public:
  virtual void handle_signal(sig_atomic_t, corevm::runtime::process&);
};


class sighandler_SIGURG : public sighandler {
public:
  virtual void handle_signal(sig_atomic_t, corevm::runtime::process&);
};


}; /* end namespace runtime */


}; /* end namespace corevm */


#endif /* COREVM_SIGHANDLER_H_ */
