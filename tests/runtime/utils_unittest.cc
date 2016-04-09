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
#include "dyobj/dynamic_object.h"
#include "runtime/compartment.h"
#include "runtime/utils.h"

#include <gtest/gtest.h>


// -----------------------------------------------------------------------------

class DummyDynamicObjectManager {};

// -----------------------------------------------------------------------------

class RuntimeUtilsUnitTest : public ::testing::Test
{
public:
  using DynamicObjectType = typename corevm::dyobj::DynamicObject<DummyDynamicObjectManager>;
};

// -----------------------------------------------------------------------------

static const std::string DUMMY_PATH("");

// -----------------------------------------------------------------------------

TEST_F(RuntimeUtilsUnitTest, TestGetAttrKey)
{
  corevm::runtime::Compartment compartment(DUMMY_PATH);

  const std::string attr_name("__str__");

  corevm::runtime::EncodingMap encoding_map;
  encoding_map.emplace_back(attr_name);

  compartment.set_encoding_map(std::move(encoding_map));

  corevm::runtime::encoding_key attr_key = 0;

  attr_key = corevm::runtime::get_attr_key(
    &compartment, attr_key);

  ASSERT_NE(0, attr_key);
}

// -----------------------------------------------------------------------------

TEST_F(RuntimeUtilsUnitTest, TestGetAttrKey2)
{
  corevm::runtime::Compartment compartment(DUMMY_PATH);

  const std::string attr_name("__str__");

  corevm::runtime::EncodingMap encoding_map;
  encoding_map.emplace_back(attr_name);

  compartment.set_encoding_map(std::move(encoding_map));

  corevm::runtime::encoding_key attr_key = 0;

  std::string actual_attr_name;

  attr_key = corevm::runtime::get_attr_key(
    &compartment, attr_key, &actual_attr_name);

  ASSERT_NE(0, attr_key);
  ASSERT_EQ(attr_name, actual_attr_name);
}

// -----------------------------------------------------------------------------

TEST_F(RuntimeUtilsUnitTest, TestGetattr)
{
  const std::string attr_name("__str__");

  corevm::dyobj::attr_key attr_key = corevm::dyobj::hash_attr_str(attr_name);

  DynamicObjectType obj;
  DynamicObjectType attr_obj;
  obj.putattr(attr_key, &attr_obj);

  ASSERT_EQ(true, obj.hasattr(attr_key));

  auto actual_attr_obj = corevm::runtime::getattr(&obj, attr_name);

  ASSERT_EQ(&attr_obj, actual_attr_obj);
}

// -----------------------------------------------------------------------------

TEST_F(RuntimeUtilsUnitTest, TestGetattr2)
{
  corevm::runtime::Compartment compartment(DUMMY_PATH);

  const std::string attr_name("__str__");

  corevm::runtime::EncodingMap encoding_map;
  encoding_map.emplace_back(attr_name);

  compartment.set_encoding_map(std::move(encoding_map));

  corevm::runtime::encoding_key attr_encoding_key = 0;

  corevm::dyobj::attr_key attr_key = corevm::dyobj::hash_attr_str(attr_name);

  DynamicObjectType obj;
  DynamicObjectType attr_obj;
  obj.putattr(attr_key, &attr_obj);

  ASSERT_EQ(true, obj.hasattr(attr_key));

  auto actual_attr_obj = corevm::runtime::getattr(
    &obj, &compartment, attr_encoding_key);

  ASSERT_EQ(&attr_obj, actual_attr_obj);
}

// -----------------------------------------------------------------------------
