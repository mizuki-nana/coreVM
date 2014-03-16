#include <map>
#include <sneaker/testing/_unittest.h>
#include "../../include/dyobj/dynamic_object.h"


class dummy_dynamic_object_manager {};


class dynamic_object_unit_test : public ::testing::Test {
protected:
  corevm::dyobj::dynamic_object<dummy_dynamic_object_manager> _obj;
};


TEST_F(dynamic_object_unit_test, TestInitialization)
{
  ASSERT_EQ(0, this->_obj.flags());
  ASSERT_FALSE(this->_obj.hasattr(0));
  ASSERT_FALSE(this->_obj.hasattr(1));
  ASSERT_FALSE(this->_obj.hasattr(123));
}

TEST_F(dynamic_object_unit_test, TestGetAndSetFlags)
{
  ASSERT_EQ(0, this->_obj.flags());

  ASSERT_FALSE(this->_obj.get_flag(1));
  ASSERT_FALSE(this->_obj.get_flag(16));
  ASSERT_FALSE(this->_obj.get_flag(32));

  this->_obj.set_flag(1);
  this->_obj.set_flag(16);
  this->_obj.set_flag(32);

  ASSERT_TRUE(this->_obj.get_flag(1));
  ASSERT_TRUE(this->_obj.get_flag(16));
  ASSERT_TRUE(this->_obj.get_flag(32));

  this->_obj.clear_flag(1);
  this->_obj.clear_flag(16);
  this->_obj.clear_flag(32);

  ASSERT_FALSE(this->_obj.get_flag(1));
  ASSERT_FALSE(this->_obj.get_flag(16));
  ASSERT_FALSE(this->_obj.get_flag(32));
}

TEST_F(dynamic_object_unit_test, TestGetAndSetAttrs)
{
  corevm::dyobj::attr_key key1 = 123;
  corevm::dyobj::attr_key key2 = 456;
  corevm::dyobj::attr_key key3 = 789;

  corevm::dyobj::dyobj_id obj_id1 = corevm::dyobj::generate_dyobj_id();
  corevm::dyobj::dyobj_id obj_id2 = corevm::dyobj::generate_dyobj_id();
  corevm::dyobj::dyobj_id obj_id3 = corevm::dyobj::generate_dyobj_id();

  std::map<corevm::dyobj::attr_key, corevm::dyobj::dyobj_id> mock_attrs = {
    {key1, obj_id1},
    {key2, obj_id2},
    {key3, obj_id3}
  };

  ASSERT_FALSE(this->_obj.hasattr(key1));
  ASSERT_FALSE(this->_obj.hasattr(key2));
  ASSERT_FALSE(this->_obj.hasattr(key3));

  ASSERT_THROW(this->_obj.delattr(key1), corevm::dyobj::dynamic_object_attribute_cannot_be_deleted_error);
  ASSERT_THROW(this->_obj.delattr(key2), corevm::dyobj::dynamic_object_attribute_cannot_be_deleted_error);
  ASSERT_THROW(this->_obj.delattr(key3), corevm::dyobj::dynamic_object_attribute_cannot_be_deleted_error);

  this->_obj.putattr(key1, mock_attrs[key1]);
  this->_obj.putattr(key2, mock_attrs[key2]);
  this->_obj.putattr(key3, mock_attrs[key3]);

  ASSERT_TRUE(this->_obj.hasattr(key1));
  ASSERT_TRUE(this->_obj.hasattr(key2));
  ASSERT_TRUE(this->_obj.hasattr(key3));

  ASSERT_EQ(mock_attrs[key1], this->_obj.getattr(key1));
  ASSERT_EQ(mock_attrs[key2], this->_obj.getattr(key2));
  ASSERT_EQ(mock_attrs[key3], this->_obj.getattr(key3));

  ASSERT_NO_THROW(this->_obj.delattr(key1));
  ASSERT_NO_THROW(this->_obj.delattr(key2));
  ASSERT_NO_THROW(this->_obj.delattr(key3));

  ASSERT_FALSE(this->_obj.hasattr(key1));
  ASSERT_FALSE(this->_obj.hasattr(key2));
  ASSERT_FALSE(this->_obj.hasattr(key3));

  ASSERT_THROW(this->_obj.delattr(key1), corevm::dyobj::dynamic_object_attribute_cannot_be_deleted_error);
  ASSERT_THROW(this->_obj.delattr(key2), corevm::dyobj::dynamic_object_attribute_cannot_be_deleted_error);
  ASSERT_THROW(this->_obj.delattr(key3), corevm::dyobj::dynamic_object_attribute_cannot_be_deleted_error);

  ASSERT_THROW(this->_obj.getattr(key1), corevm::dyobj::dynamic_object_not_found_error);
  ASSERT_THROW(this->_obj.getattr(key2), corevm::dyobj::dynamic_object_not_found_error);
  ASSERT_THROW(this->_obj.getattr(key3), corevm::dyobj::dynamic_object_not_found_error);
}

TEST_F(dynamic_object_unit_test, TestEquality)
{
  // ASSERT_EQ(this->_obj, this->_obj);
  ASSERT_TRUE(this->_obj == this->_obj);
}

TEST_F(dynamic_object_unit_test, TestInequality)
{
  corevm::dyobj::dynamic_object<dummy_dynamic_object_manager> obj1;
  corevm::dyobj::dynamic_object<dummy_dynamic_object_manager> obj2;

  // ASSERT_NE(obj1, obj2);
  ASSERT_TRUE(obj1 != obj2);
  ASSERT_FALSE(obj1 == obj2);
}