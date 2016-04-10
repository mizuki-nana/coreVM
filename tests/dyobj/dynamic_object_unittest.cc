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
#include "dyobj/flags.h"

#include <gtest/gtest.h>

#include <map>


class DummyDynamicObjectManager {};

// -----------------------------------------------------------------------------

class DynamicObjectUnitTest : public ::testing::Test
{
public:
  using dynamic_object_type = typename corevm::dyobj::DynamicObject<DummyDynamicObjectManager>;
};

// -----------------------------------------------------------------------------

TEST_F(DynamicObjectUnitTest, TestInitialization)
{
  dynamic_object_type obj;

  ASSERT_EQ(0, obj.flags());
  ASSERT_FALSE(obj.hasattr(0));
  ASSERT_FALSE(obj.hasattr(1));
  ASSERT_FALSE(obj.hasattr(123));
}

// -----------------------------------------------------------------------------

TEST_F(DynamicObjectUnitTest, TestGetAndSetFlags)
{
  dynamic_object_type obj;

  ASSERT_EQ(0, obj.flags());

  char flag1 = corevm::dyobj::DynamicObjectFlagBits::DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE;
  char flag2 = corevm::dyobj::DynamicObjectFlagBits::DYOBJ_IS_IMMUTABLE;

  ASSERT_FALSE(obj.get_flag(flag1));
  ASSERT_FALSE(obj.get_flag(flag2));

  obj.set_flag(flag1);
  obj.set_flag(flag2);

  ASSERT_TRUE(obj.get_flag(flag1));
  ASSERT_TRUE(obj.get_flag(flag2));

  obj.clear_flag(flag1);
  obj.clear_flag(flag2);

  ASSERT_FALSE(obj.get_flag(flag1));
  ASSERT_FALSE(obj.get_flag(flag2));

  char invalid_flag = corevm::dyobj::DynamicObjectFlagBits::DYOBJ_MAX_VALUE;

  ASSERT_THROW(
    {
      obj.set_flag(invalid_flag);
    },
    corevm::dyobj::InvalidFlagBitError
  );
}

// -----------------------------------------------------------------------------

TEST_F(DynamicObjectUnitTest, TestGetattr)
{
  dynamic_object_type obj;
  dynamic_object_type attr_obj;

  corevm::dyobj::attr_key_t attr_key = 1;

  ASSERT_FALSE(obj.hasattr(attr_key));

  obj.putattr(attr_key, &attr_obj);

  dynamic_object_type* actual_attr_obj = NULL;
  bool res = obj.getattr(attr_key, &actual_attr_obj);

  ASSERT_EQ(true, res);
  ASSERT_EQ(&attr_obj, actual_attr_obj);

  corevm::dyobj::attr_key_t invalid_attr_key = 2;
  ASSERT_FALSE(obj.hasattr(invalid_attr_key));

  dynamic_object_type* invalid_attr_obj = NULL;
  res = obj.getattr(invalid_attr_key, &invalid_attr_obj);

  ASSERT_EQ(false, res);
  ASSERT_EQ(NULL, invalid_attr_obj);
}

// -----------------------------------------------------------------------------

TEST_F(DynamicObjectUnitTest, TestGetAndSetAttrs)
{
  dynamic_object_type obj;

  corevm::dyobj::attr_key_t key1 = 123;
  corevm::dyobj::attr_key_t key2 = 456;
  corevm::dyobj::attr_key_t key3 = 789;

  dynamic_object_type obj1;
  dynamic_object_type obj2;
  dynamic_object_type obj3;

  std::map<corevm::dyobj::attr_key_t, dynamic_object_type*> mock_attrs {
    { key1, &obj1 },
    { key2, &obj2 },
    { key3, &obj3 }
  };

  ASSERT_FALSE(obj.hasattr(key1));
  ASSERT_FALSE(obj.hasattr(key2));
  ASSERT_FALSE(obj.hasattr(key3));

  ASSERT_THROW(
    {
      obj.delattr(key1);
    },
    corevm::dyobj::ObjectAttributeNotFoundError
  );

  ASSERT_THROW(
    {
      obj.delattr(key2);
    },
    corevm::dyobj::ObjectAttributeNotFoundError
  );

  ASSERT_THROW(
    {
      obj.delattr(key3);
    },
    corevm::dyobj::ObjectAttributeNotFoundError
  );

  obj.putattr(key1, mock_attrs[key1]);
  obj.putattr(key2, mock_attrs[key2]);
  obj.putattr(key3, mock_attrs[key3]);

  ASSERT_TRUE(obj.hasattr(key1));
  ASSERT_TRUE(obj.hasattr(key2));
  ASSERT_TRUE(obj.hasattr(key3));

  ASSERT_EQ(mock_attrs[key1], obj.getattr(key1));
  ASSERT_EQ(mock_attrs[key2], obj.getattr(key2));
  ASSERT_EQ(mock_attrs[key3], obj.getattr(key3));

  ASSERT_NO_THROW(
    {
      obj.delattr(key1);
    }
  );

  ASSERT_NO_THROW(
    {
      obj.delattr(key2);
    }
  );

  ASSERT_NO_THROW(
    {
      obj.delattr(key3);
    }
  );

  ASSERT_FALSE(obj.hasattr(key1));
  ASSERT_FALSE(obj.hasattr(key2));
  ASSERT_FALSE(obj.hasattr(key3));

  ASSERT_THROW(
    {
      obj.delattr(key1);
    },
    corevm::dyobj::ObjectAttributeNotFoundError
  );

  ASSERT_THROW(
    {
      obj.delattr(key2);
    },
    corevm::dyobj::ObjectAttributeNotFoundError
  );

  ASSERT_THROW(
    {
      obj.delattr(key3);
    },
    corevm::dyobj::ObjectAttributeNotFoundError
  );

  ASSERT_THROW(
    {
      obj.getattr(key1);
    },
    corevm::dyobj::ObjectAttributeNotFoundError
  );

  ASSERT_THROW(
    {
      obj.getattr(key2);
    },
    corevm::dyobj::ObjectAttributeNotFoundError
  );

  ASSERT_THROW(
    {
      obj.getattr(key3);
    },
    corevm::dyobj::ObjectAttributeNotFoundError
  );
}

// -----------------------------------------------------------------------------

TEST_F(DynamicObjectUnitTest, TestSetAndGetClosureCtx)
{
  dynamic_object_type obj;

  const corevm::runtime::ClosureCtx& ctx = obj.closure_ctx();

  ASSERT_EQ(corevm::runtime::NONESET_COMPARTMENT_ID, ctx.compartment_id);
  ASSERT_EQ(corevm::runtime::NONESET_CLOSURE_ID, ctx.closure_id);

  corevm::runtime::compartment_id_t compartment_id = 123;
  corevm::runtime::closure_id_t closure_id = 456;

  corevm::runtime::ClosureCtx expected_ctx(compartment_id, closure_id);

  obj.set_closure_ctx(expected_ctx);

  const corevm::runtime::ClosureCtx& ctx2 = obj.closure_ctx();

  ASSERT_EQ(expected_ctx.compartment_id, ctx2.compartment_id);
  ASSERT_EQ(expected_ctx.closure_id, ctx2.closure_id);
}

// -----------------------------------------------------------------------------

TEST_F(DynamicObjectUnitTest, TestCopyFrom)
{
  dynamic_object_type obj;

  corevm::runtime::compartment_id_t compartment_id = 123;
  corevm::runtime::closure_id_t closure_id = 456;

  corevm::runtime::ClosureCtx expected_ctx(compartment_id, closure_id);
  obj.set_closure_ctx(expected_ctx);

  corevm::dyobj::attr_key_t key1 = 123;
  corevm::dyobj::attr_key_t key2 = 456;
  corevm::dyobj::attr_key_t key3 = 789;

  dynamic_object_type attr_obj1;
  dynamic_object_type attr_obj2;
  dynamic_object_type attr_obj3;

  obj.putattr(key1, &attr_obj1);
  obj.putattr(key2, &attr_obj2);
  obj.putattr(key3, &attr_obj3);

  char flag1 = corevm::dyobj::DynamicObjectFlagBits::DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE;
  char flag2 = corevm::dyobj::DynamicObjectFlagBits::DYOBJ_IS_IMMUTABLE;

  obj.set_flag(flag1);
  obj.set_flag(flag2);

  dynamic_object_type dst;

  dst.copy_from(obj);

  ASSERT_EQ(obj.closure_ctx().compartment_id, dst.closure_ctx().compartment_id);
  ASSERT_EQ(obj.closure_ctx().closure_id, dst.closure_ctx().closure_id);

  ASSERT_EQ(obj.getattr(key1), dst.getattr(key1));
  ASSERT_EQ(obj.getattr(key2), dst.getattr(key2));
  ASSERT_EQ(obj.getattr(key3), dst.getattr(key3));

  ASSERT_EQ(obj.flags(), dst.flags());
}

// -----------------------------------------------------------------------------

TEST_F(DynamicObjectUnitTest, TestEquality)
{
  dynamic_object_type obj;

  ASSERT_TRUE(obj == obj);
}

// -----------------------------------------------------------------------------

TEST_F(DynamicObjectUnitTest, TestInequality)
{
  dynamic_object_type obj1;
  dynamic_object_type obj2;

  ASSERT_TRUE(obj1 != obj2);
}

// -----------------------------------------------------------------------------
