#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "../../include/dyobj/dyobj_id.h"


corevm::dyobj::dyobj_id corevm::dyobj::generate_dyobj_id()
{
  boost::uuids::uuid uuid = boost::uuids::random_generator()();
  __uint128_t id;
  memcpy(&id, static_cast<unsigned char*>(uuid.data), uuid.size());
  return id;
}

size_t corevm::dyobj::id_to_hash(corevm::dyobj::dyobj_id id)
{
  return static_cast<std::size_t>(id);
}

std::string corevm::dyobj::id_to_string(corevm::dyobj::dyobj_id id)
{
  uint64_t normalized_id = static_cast<uint64_t>(id);
  return boost::lexical_cast<std::string>(normalized_id);
}