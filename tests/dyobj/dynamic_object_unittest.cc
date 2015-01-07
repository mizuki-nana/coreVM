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
#include "../../include/dyobj/dynamic_object.h"
#include "../../include/dyobj/flags.h"

#include <sneaker/testing/_unittest.h>

#include <map>


class dummy_dynamic_object_manager {};


class dynamic_object_unittest : public ::testing::Test {
public:
  using dynamic_object_type = typename corevm::dyobj::dynamic_object<dummy_dynamic_object_manager>;
};


TEST_F(dynamic_object_unittest, TestInitialization)
{
  dynamic_object_type obj;

  ASSERT_EQ(0, obj.flags());
  ASSERT_FALSE(obj.hasattr(0));
  ASSERT_FALSE(obj.hasattr(1));
  ASSERT_FALSE(obj.hasattr(123));
}

TEST_F(dynamic_object_unittest, TestGetAndSetFlags)
{
  dynamic_object_type obj;

  ASSERT_EQ(0, obj.flags());

  char flag1 = corevm::dyobj::flags::IS_NOT_GARBAGE_COLLECTIBLE;
  char flag2 = corevm::dyobj::flags::IS_MUTABLE;

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

  char invalid_flag = corevm::dyobj::flags::MAX_VALUE;

  ASSERT_THROW(
    {
      obj.set_flag(invalid_flag);
    },
    corevm::dyobj::invalid_flag_bit_error
  );
}

TEST_F(dynamic_object_unittest, TestGetAndSetAttrs)
{
  dynamic_object_type obj;

  corevm::dyobj::attr_key key1 = 123;
  corevm::dyobj::attr_key key2 = 456;
  corevm::dyobj::attr_key key3 = 789;

  corevm::dyobj::dyobj_id obj_id1 = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();
  corevm::dyobj::dyobj_id obj_id2 = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();
  corevm::dyobj::dyobj_id obj_id3 = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();

  std::map<corevm::dyobj::attr_key, corevm::dyobj::dyobj_id> mock_attrs = {
    { key1, obj_id1 },
    { key2, obj_id2 },
    { key3, obj_id3 }
  };

  ASSERT_FALSE(obj.hasattr(key1));
  ASSERT_FALSE(obj.hasattr(key2));
  ASSERT_FALSE(obj.hasattr(key3));

  ASSERT_THROW(
    {
      obj.delattr(key1);
    },
    corevm::dyobj::object_attribute_deletion_error
  );

  ASSERT_THROW(
    {
      obj.delattr(key2);
    },
    corevm::dyobj::object_attribute_deletion_error
  );

  ASSERT_THROW(
    {
      obj.delattr(key3);
    },
    corevm::dyobj::object_attribute_deletion_error
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
    corevm::dyobj::object_attribute_deletion_error
  );

  ASSERT_THROW(
    {
      obj.delattr(key2);
    },
    corevm::dyobj::object_attribute_deletion_error
  );

  ASSERT_THROW(
    {
      obj.delattr(key3);
    },
    corevm::dyobj::object_attribute_deletion_error
  );

  ASSERT_THROW(
    {
      obj.getattr(key1);
    },
    corevm::dyobj::object_attribute_not_found_error
  );

  ASSERT_THROW(
    {
      obj.getattr(key2);
    },
    corevm::dyobj::object_attribute_not_found_error
  );

  ASSERT_THROW(
    {
      obj.getattr(key3);
    },
    corevm::dyobj::object_attribute_not_found_error
  );
}

TEST_F(dynamic_object_unittest, TestGetAndSetInstrBlockKey)
{
  dynamic_object_type obj;

  ASSERT_EQ(corevm::dyobj::NONESET_INSTR_BLOCK_KEY, obj.get_instr_block_key());

  corevm::dyobj::instr_block_key key = 100;
  obj.set_instr_block_key(key);

  ASSERT_EQ(key, obj.get_instr_block_key());

  obj.clear_instr_block_key();

  ASSERT_EQ(corevm::dyobj::NONESET_INSTR_BLOCK_KEY, obj.get_instr_block_key());
}

TEST_F(dynamic_object_unittest, TestEquality)
{
  dynamic_object_type obj;

  ASSERT_TRUE(obj == obj);
}

TEST_F(dynamic_object_unittest, TestInequality)
{
  dynamic_object_type obj1;
  dynamic_object_type obj2;

  ASSERT_TRUE(obj1 != obj2);
  ASSERT_FALSE(obj1 == obj2);
}
