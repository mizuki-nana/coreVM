#include <map>
#include <sneaker/testing/_unittest.h>
#include "../../include/dyobj/dynamic_object.h"


class dummy_dynamic_object_manager {};


class dynamic_object_unittest : public ::testing::Test {};


TEST_F(dynamic_object_unittest, TestInitialization)
{
  corevm::dyobj::dyobj_id id = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();
  corevm::dyobj::dynamic_object<dummy_dynamic_object_manager> obj(id);

  ASSERT_EQ(0, obj.flags());
  ASSERT_FALSE(obj.hasattr(0));
  ASSERT_FALSE(obj.hasattr(1));
  ASSERT_FALSE(obj.hasattr(123));
}

TEST_F(dynamic_object_unittest, TestGetAndSetFlags)
{
  corevm::dyobj::dyobj_id id = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();
  corevm::dyobj::dynamic_object<dummy_dynamic_object_manager> obj(id);

  ASSERT_EQ(0, obj.flags());

  ASSERT_FALSE(obj.get_flag(1));
  ASSERT_FALSE(obj.get_flag(16));
  ASSERT_FALSE(obj.get_flag(32));

  obj.set_flag(1);
  obj.set_flag(16);
  obj.set_flag(32);

  ASSERT_TRUE(obj.get_flag(1));
  ASSERT_TRUE(obj.get_flag(16));
  ASSERT_TRUE(obj.get_flag(32));

  obj.clear_flag(1);
  obj.clear_flag(16);
  obj.clear_flag(32);

  ASSERT_FALSE(obj.get_flag(1));
  ASSERT_FALSE(obj.get_flag(16));
  ASSERT_FALSE(obj.get_flag(32));
}

TEST_F(dynamic_object_unittest, TestGetAndSetAttrs)
{
  corevm::dyobj::dyobj_id id = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();
  corevm::dyobj::dynamic_object<dummy_dynamic_object_manager> obj(id);

  corevm::dyobj::attr_key key1 = 123;
  corevm::dyobj::attr_key key2 = 456;
  corevm::dyobj::attr_key key3 = 789;

  corevm::dyobj::dyobj_id obj_id1 = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();
  corevm::dyobj::dyobj_id obj_id2 = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();
  corevm::dyobj::dyobj_id obj_id3 = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();

  std::map<corevm::dyobj::attr_key, corevm::dyobj::dyobj_id> mock_attrs = {
    {key1, obj_id1},
    {key2, obj_id2},
    {key3, obj_id3}
  };

  ASSERT_FALSE(obj.hasattr(key1));
  ASSERT_FALSE(obj.hasattr(key2));
  ASSERT_FALSE(obj.hasattr(key3));

#ifdef __APPLE__
  ASSERT_THROW(obj.delattr(key1), corevm::dyobj::dynamic_object_attribute_cannot_be_deleted_error);
  ASSERT_THROW(obj.delattr(key2), corevm::dyobj::dynamic_object_attribute_cannot_be_deleted_error);
  ASSERT_THROW(obj.delattr(key3), corevm::dyobj::dynamic_object_attribute_cannot_be_deleted_error);
#else
  ASSERT_ANY_THROW(obj.delattr(key1));
  ASSERT_ANY_THROW(obj.delattr(key2));
  ASSERT_ANY_THROW(obj.delattr(key3));
#endif

  obj.putattr(key1, mock_attrs[key1]);
  obj.putattr(key2, mock_attrs[key2]);
  obj.putattr(key3, mock_attrs[key3]);

  ASSERT_TRUE(obj.hasattr(key1));
  ASSERT_TRUE(obj.hasattr(key2));
  ASSERT_TRUE(obj.hasattr(key3));

  ASSERT_EQ(mock_attrs[key1], obj.getattr(key1));
  ASSERT_EQ(mock_attrs[key2], obj.getattr(key2));
  ASSERT_EQ(mock_attrs[key3], obj.getattr(key3));

  ASSERT_NO_THROW(obj.delattr(key1));
  ASSERT_NO_THROW(obj.delattr(key2));
  ASSERT_NO_THROW(obj.delattr(key3));

  ASSERT_FALSE(obj.hasattr(key1));
  ASSERT_FALSE(obj.hasattr(key2));
  ASSERT_FALSE(obj.hasattr(key3));

#ifdef __APPLE__
  ASSERT_THROW(obj.delattr(key1), corevm::dyobj::dynamic_object_attribute_cannot_be_deleted_error);
  ASSERT_THROW(obj.delattr(key2), corevm::dyobj::dynamic_object_attribute_cannot_be_deleted_error);
  ASSERT_THROW(obj.delattr(key3), corevm::dyobj::dynamic_object_attribute_cannot_be_deleted_error);

  ASSERT_THROW(obj.getattr(key1), corevm::dyobj::dynamic_object_attribute_not_found_error);
  ASSERT_THROW(obj.getattr(key2), corevm::dyobj::dynamic_object_attribute_not_found_error);
  ASSERT_THROW(obj.getattr(key3), corevm::dyobj::dynamic_object_attribute_not_found_error);
#else
  ASSERT_ANY_THROW(obj.delattr(key1));
  ASSERT_ANY_THROW(obj.delattr(key2));
  ASSERT_ANY_THROW(obj.delattr(key3));

  ASSERT_ANY_THROW(obj.getattr(key1));
  ASSERT_ANY_THROW(obj.getattr(key2));
  ASSERT_ANY_THROW(obj.getattr(key3));
#endif
}

TEST_F(dynamic_object_unittest, TestGetAndSetInstrBlockKey)
{
  corevm::dyobj::dyobj_id id = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();
  corevm::dyobj::dynamic_object<dummy_dynamic_object_manager> obj(id);

  ASSERT_EQ(corevm::dyobj::NONESET_INSTR_BLOCK_KEY, obj.get_instr_block_key());

  corevm::dyobj::instr_block_key key = 100;
  obj.set_instr_block_key(key);

  ASSERT_EQ(key, obj.get_instr_block_key());

  obj.clear_instr_block_key();

  ASSERT_EQ(corevm::dyobj::NONESET_INSTR_BLOCK_KEY, obj.get_instr_block_key());
}

TEST_F(dynamic_object_unittest, TestEquality)
{
  corevm::dyobj::dyobj_id id = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();
  corevm::dyobj::dynamic_object<dummy_dynamic_object_manager> obj(id);

  // ASSERT_EQ(obj, obj);
  ASSERT_TRUE(obj == obj);
}

TEST_F(dynamic_object_unittest, TestInequality)
{
  corevm::dyobj::dyobj_id id1 = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();
  corevm::dyobj::dynamic_object<dummy_dynamic_object_manager> obj1(id1);

  corevm::dyobj::dyobj_id id2 = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();
  corevm::dyobj::dynamic_object<dummy_dynamic_object_manager> obj2(id2);

  // ASSERT_NE(obj1, obj2);
  ASSERT_TRUE(obj1 != obj2);
  ASSERT_FALSE(obj1 == obj2);
}

// TEST_F(dynamic_object_unittest, TestSize)
// {
//   corevm::dyobj::dyobj_id id = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();
//   corevm::dyobj::dynamic_object<dummy_dynamic_object_manager> obj(id);

//   printf("Size of dyobj: %lu\n", sizeof(obj));
// }
