#ifndef COREVM_SCHEMA_REPOSITORY_H_
#define COREVM_SCHEMA_REPOSITORY_H_

#include <set>
#include <string>
#include <unordered_map>


namespace corevm {


namespace frontend {


class schema_repository {
public:
  static const std::string load_by_format_version(const std::string&);
  static const std::set<std::string> list_supported_versions();
private:
  static const std::unordered_map<std::string, std::string> format_version_to_schema_map;
};


} /* end namespace frontend */


} /* end namespace corevm */


#endif /* COREVM_SCHEMA_REPOSITORY_H_ */
