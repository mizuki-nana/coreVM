#ifndef COREVM_DYOBJ_ID_HELPER_H_
#define COREVM_DYOBJ_ID_HELPER_H_

#include <string>
#include <sneaker/atomic/atomic_incrementor.h>
#include "dyobj_id.h"


namespace corevm {


namespace dyobj {


class dyobj_id_helper {
public:
  static dyobj_id generate_dyobj_id();
  static std::string id_to_string(corevm::dyobj::dyobj_id);
private:
  static sneaker::atomic::atomic_incrementor<dyobj_id, DYOBJ_LIMIT> atomic_value;
};


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYOBJ_ID_HELPER_H_ */
