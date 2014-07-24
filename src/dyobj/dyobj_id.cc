#include <boost/lexical_cast.hpp>
#include "../../include/dyobj/dyobj_id.h"
#include "../../include/dyobj/errors.h"


sneaker::atomic::atomic_incrementor<
  corevm::dyobj::dyobj_id,
  corevm::dyobj::DYOBJ_LIMIT
> corevm::dyobj::dyobj_id_helper::atomic_value = 0;


corevm::dyobj::dyobj_id
corevm::dyobj::dyobj_id_helper::generate_dyobj_id()
{
  try {
    return ++corevm::dyobj::dyobj_id_helper::atomic_value;
  } catch (...) { // TODO: should catch std::overflow_error and std::underflow_error here.
    throw corevm::dyobj::dynamic_object_id_exceed_limit_error();
  }
}

std::string
corevm::dyobj::dyobj_id_helper::id_to_string(corevm::dyobj::dyobj_id id)
{
  return boost::lexical_cast<std::string>(id);
}
