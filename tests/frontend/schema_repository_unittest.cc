#include <cassert>
#include <sneaker/testing/_unittest.h>
#include "../../include/frontend/schema_repository.h"

using corevm::frontend::schema_repository;


class schema_repository_unittest : public ::testing::Test {};


TEST_F(schema_repository_unittest, TestListSupportedVersions)
{
  auto supported_versions = schema_repository::list_supported_versions();

  ASSERT_EQ(false, supported_versions.empty());
}

TEST_F(schema_repository_unittest, TestLoadByValidFormatVersion)
{
  auto supported_versions = schema_repository::list_supported_versions();
  auto itr = supported_versions.begin();
  std::string version = static_cast<std::string>(*itr);

  std::string schema = schema_repository::load_by_format_version(version);

  ASSERT_EQ(false, schema.empty());
}

TEST_F(schema_repository_unittest, TestLoadByInvalidFormatVersion)
{
  std::string invalid_version = "!@#";
  auto supported_versions = schema_repository::list_supported_versions();

  assert(supported_versions.find(invalid_version) == supported_versions.end());

  std::string schema = schema_repository::load_by_format_version(invalid_version);

  ASSERT_EQ(true, schema.empty());
}
