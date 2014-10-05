#include <sneaker/testing/_unittest.h>
#include "../../include/dyobj/dynamic_object.h"
#include "../../include/dyobj/dynamic_object_heap.h"
#include "../../include/gc/garbage_collector.h"
#include "../../include/gc/garbage_collection_scheme.h"


class dummy_dynamic_object_manager {};


class dynamic_object_heap_unittest : public ::testing::Test {
protected:
  corevm::dyobj::dynamic_object_heap<dummy_dynamic_object_manager> _heap;
};


TEST_F(dynamic_object_heap_unittest, TestCreateDyobj)
{
  corevm::dyobj::dyobj_id id1 = _heap.create_dyobj();
  corevm::dyobj::dyobj_id id2 = _heap.create_dyobj();

  ASSERT_NE(0, id1);
  ASSERT_NE(0, id2);

  // This assumption is true for now, but might change in the future.
  ASSERT_LT(id1, id2);

  // Tests that we can get the objects by those ids and they are equivalent.
  corevm::dyobj::dynamic_object<dummy_dynamic_object_manager>& obj1 = _heap.at(id1);
  corevm::dyobj::dynamic_object<dummy_dynamic_object_manager>& obj2 = _heap.at(id2);

  ASSERT_EQ(id1, obj1.id());
  ASSERT_EQ(id2, obj2.id());
}

TEST_F(dynamic_object_heap_unittest, TestAtOnNonExistentKeys)
{
  corevm::dyobj::dyobj_id id1 = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();
  corevm::dyobj::dyobj_id id2 = corevm::dyobj::dyobj_id_helper::generate_dyobj_id();

  ASSERT_THROW(this->_heap.at(id1), std::runtime_error);
  ASSERT_THROW(this->_heap.at(id2), std::runtime_error);
}
