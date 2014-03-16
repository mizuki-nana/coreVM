#ifndef COREVM_DYOBJ_ID_H_
#define COREVM_DYOBJ_ID_H_

#include <string>
#include <boost/uuid/uuid.hpp>


namespace corevm {


namespace dyobj {


typedef boost::uuids::uuid dyobj_id;


corevm::dyobj::dyobj_id generate_dyobj_id();


size_t id_to_hash(corevm::dyobj::dyobj_id);


std::string id_to_string(corevm::dyobj::dyobj_id);


} /* end namespace dyobj */


} /* end namespace corevm */


#endif /* COREVM_DYOBJ_ID_H_ */