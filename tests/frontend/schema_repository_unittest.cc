/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Yanzheng Li

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************/
#include "../../include/frontend/schema_repository.h"

#include <sneaker/testing/_unittest.h>

#include <cassert>


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
