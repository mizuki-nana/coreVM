#include <sneaker/testing/_unittest.h>
#include "../../include/dyobj/dynamic_object.h"
#include "../../include/dyobj/dynamic_object_heap.h"

#include "../../include/gc/garbage_collector.h"
#include "../../include/gc/garbage_collection_scheme.h"


class dummy_dynamic_object_manager {};


class dynamic_object_heap_unit_test : public ::testing::Test {
protected:
  corevm::dyobj::dynamic_object_heap<dummy_dynamic_object_manager> _heap;
};


TEST_F(dynamic_object_heap_unit_test, TestAtOnNonExistentKeys)
{
  corevm::dyobj::dyobj_id id1 = corevm::dyobj::generate_dyobj_id();
  corevm::dyobj::dyobj_id id2 = corevm::dyobj::generate_dyobj_id();

  ASSERT_THROW(this->_heap.at(id1), std::runtime_error);
  ASSERT_THROW(this->_heap.at(id2), std::runtime_error);
}