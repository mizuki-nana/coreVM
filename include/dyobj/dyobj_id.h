#ifndef COREVM_DYOBJ_ID_H_
#define COREVM_DYOBJ_ID_H_

#include <climits>
#include <string>
#include <sneaker/atomic/atomic_incrementor.h>


namespace corevm {


namespace dyobj {


typedef uint64_t dyobj_id;


const uint64_t DYOBJ_LIMIT = ULLONG_MAX;


class dyobj_id_helper {
public:
  static dyobj_id generate_dyobj_id();
  static std::string id_to_string(corevm::dyobj::dyobj_id);
private:
  static sneaker::atomic::atomic_incrementor<dyobj_id, DYOBJ_LIMIT> atomic_value;
};


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYOBJ_ID_H_ */
