#ifndef COREVM_ALLOCATION_SCHEME_H_
#define COREVM_ALLOCATION_SCHEME_H_


namespace corevm {


namespace memory {


class allocation_scheme {
public:
  virtual ssize_t malloc(size_t) noexcept = 0;
  virtual ssize_t free(size_t) noexcept = 0;
};


} /* end namespace memory */


} /* end namespace corevm */


#endif /* COREVM_ALLOCATION_SCHEME_H_ */