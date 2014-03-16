#include <boost/functional/hash.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "../../include/dyobj/dyobj_id.h"


corevm::dyobj::dyobj_id corevm::dyobj::generate_dyobj_id()
{
  return boost::uuids::random_generator()();
}

size_t corevm::dyobj::id_to_hash(corevm::dyobj::dyobj_id id)
{
  boost::hash<boost::uuids::uuid> uuid_hasher;
  return static_cast<std::size_t>( uuid_hasher(id) );
}

std::string corevm::dyobj::id_to_string(corevm::dyobj::dyobj_id id)
{
  return boost::lexical_cast<std::string>(id);
}