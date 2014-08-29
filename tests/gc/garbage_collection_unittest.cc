#include <sneaker/testing/_unittest.h>
#include "../../include/dyobj/dynamic_object.h"
#include "../../include/dyobj/dynamic_object_heap.h"
#include "../../include/gc/garbage_collector.h"
#include "../../include/gc/garbage_collection_scheme.h"


template<class GarbageCollectionScheme>
class garbage_collection_unittest : public ::testing::Test {
public:
  using _GarbageCollectorType = typename corevm::gc::garbage_collector<GarbageCollectionScheme>;

  using _ObjectType = typename corevm::dyobj::dynamic_object<typename GarbageCollectionScheme::dynamic_object_manager>;

  void do_gc_and_check_results(uint32_t expected_size) {
    _GarbageCollectorType collector(_heap);
    collector.gc();

    ASSERT_EQ(expected_size, _heap.size());
  }

protected:
  corevm::dyobj::dyobj_id __create_obj() {
    corevm::dyobj::dyobj_id id = _heap.create_dyobj();
    //auto& obj = _heap.at(id);
    //obj.manager().on_create();
    return id;
  }

  void __setattr(corevm::dyobj::dyobj_id src_id, corevm::dyobj::dyobj_id dst_id) {
    auto& src_obj = _heap.at(src_id);
    auto& dst_obj = _heap.at(dst_id);
    src_obj.putattr(dst_id, dst_id);
    dst_obj.manager().on_setattr();
  }

  void __set_not_garbage_collectible(corevm::dyobj::dyobj_id id) {
    auto& obj = _heap.at(id);
    obj.set_flag(corevm::dyobj::flags::IS_NOT_GARBAGE_COLLECTIBLE);
  }

  corevm::dyobj::dynamic_object_heap<typename GarbageCollectionScheme::dynamic_object_manager> _heap; 
};


typedef ::testing::Types<
  corevm::gc::reference_count_garbage_collection_scheme
> GarbageCollectionSchemeTypes;


TYPED_TEST_CASE(garbage_collection_unittest, GarbageCollectionSchemeTypes);


TYPED_TEST(garbage_collection_unittest, TestOneObject)
{
  /*
   * Tests gc on the following object graph:
   *
   *  obj
   *
   * will result in 0 objects left on the heap.
   */
  this->__create_obj();

  this->do_gc_and_check_results(0);
}

TYPED_TEST(garbage_collection_unittest, TestSelfReferencedObject)
{
  /*
   * Tests gc on the following object graph:
   *
   * obj --->
   *  ^     |
   *  |_____|
   *
   * will result in 0 object left on the heap.
   */
  corevm::dyobj::dyobj_id id = this->__create_obj();
  this->__setattr(id, id);

  this->do_gc_and_check_results(0);
}

TYPED_TEST(garbage_collection_unittest, TestSelfReferenceOnNonGarbageCollectibleObject)
{
  /*
   * Tests gc on the following object graph:
   *
   * obj* -->
   *  ^     |
   *  |_____|
   *
   * will result in 1 object left on the heap.
   */
  corevm::dyobj::dyobj_id id = this->__create_obj();
  this->__setattr(id, id);
  this->__set_not_garbage_collectible(id);

  this->do_gc_and_check_results(1);
}

TYPED_TEST(garbage_collection_unittest, TestLinearChain)
{
  /*
   * Tests gc on the following object graph:
   *
   *  obj1 -> obj2 -> obj3 -> obj4
   *
   * will result in 0 objects left on the heap.
   */
  corevm::dyobj::dyobj_id id1 = this->__create_obj();
  corevm::dyobj::dyobj_id id2 = this->__create_obj();
  corevm::dyobj::dyobj_id id3 = this->__create_obj();
  corevm::dyobj::dyobj_id id4 = this->__create_obj();

  this->__setattr(id1, id2);
  this->__setattr(id2, id3);
  this->__setattr(id3, id4);

  this->do_gc_and_check_results(0);
}

TYPED_TEST(garbage_collection_unittest, TestLinearChainWithNonGarbageCollectibleObject)
{
  /*
   * Tests gc on the following object graph:
   *
   *  obj1* -> obj2 -> obj3 -> obj4
   *
   * will result in 4 objects left on the heap.
   */
  corevm::dyobj::dyobj_id id1 = this->__create_obj();
  corevm::dyobj::dyobj_id id2 = this->__create_obj();
  corevm::dyobj::dyobj_id id3 = this->__create_obj();
  corevm::dyobj::dyobj_id id4 = this->__create_obj();

  this->__setattr(id1, id2);
  this->__setattr(id2, id3);
  this->__setattr(id3, id4);

  this->__set_not_garbage_collectible(id1);

  this->do_gc_and_check_results(4);
}

TYPED_TEST(garbage_collection_unittest, TestLinearChainWithNonGarbageCollectibleObjects)
{
  /*
   * Tests gc on the following object graph:
   *
   *  obj1 -> obj2* -> obj3* -> obj4
   *
   * will result in 3 objects left on the heap.
   */
  corevm::dyobj::dyobj_id id1 = this->__create_obj();
  corevm::dyobj::dyobj_id id2 = this->__create_obj();
  corevm::dyobj::dyobj_id id3 = this->__create_obj();
  corevm::dyobj::dyobj_id id4 = this->__create_obj();

  this->__setattr(id1, id2);
  this->__setattr(id2, id3);
  this->__setattr(id3, id4);

  this->__set_not_garbage_collectible(id2);
  this->__set_not_garbage_collectible(id3);

  this->do_gc_and_check_results(3);
}

TYPED_TEST(garbage_collection_unittest, TestSingleCycle)
{
  /*
   * Tests gc on the following object graph:
   *
   * obj1 -> obj2 -> obj3 ->
   *  ^                    |
   *  |____________________|
   *
   * will result in 0 objects left on the heap.
   */
  corevm::dyobj::dyobj_id id1 = this->__create_obj();
  corevm::dyobj::dyobj_id id2 = this->__create_obj();
  corevm::dyobj::dyobj_id id3 = this->__create_obj();

  this->__setattr(id1, id2);
  this->__setattr(id2, id3);
  this->__setattr(id3, id1);

  this->do_gc_and_check_results(0);
}

TYPED_TEST(garbage_collection_unittest, TestMultipleObjectsPointToOne)
{
  /*
   * Tests gc on the following object graph
   *
   * obj1 --->\
   *           \
   * obj2 ------> obj4 
   *           /
   * obj3 --->/
   *
   * will result in 0 objects left on the heap.
   */
  corevm::dyobj::dyobj_id id1 = this->__create_obj();
  corevm::dyobj::dyobj_id id2 = this->__create_obj();
  corevm::dyobj::dyobj_id id3 = this->__create_obj();
  corevm::dyobj::dyobj_id id4 = this->__create_obj();

  this->__setattr(id1, id4);
  this->__setattr(id2, id4);
  this->__setattr(id3, id4);

  this->do_gc_and_check_results(0);
}

TYPED_TEST(garbage_collection_unittest, TestOnePointsToMultipleObjects)
{
  /*
   * Tests gc on the following object graph
   *
   *        ----> obj2
   *       /
   *      /
   * obj1 ------> obj3
   *      \
   *       \
   *        ----> obj4
   *
   * will result in 0 objects left on the heap.
   */
  corevm::dyobj::dyobj_id id1 = this->__create_obj();
  corevm::dyobj::dyobj_id id2 = this->__create_obj();
  corevm::dyobj::dyobj_id id3 = this->__create_obj();
  corevm::dyobj::dyobj_id id4 = this->__create_obj();

  this->__setattr(id1, id2);
  this->__setattr(id1, id3);
  this->__setattr(id1, id4);

  this->do_gc_and_check_results(0);
}

TYPED_TEST(garbage_collection_unittest, TestNonGarbageCollectibleObjectPointsToMultipleObjects)
{
  /*
   * Tests gc on the following object graph
   *
   *         ----> obj2
   *        /
   *       /
   * obj1* ------> obj3
   *       \
   *        \
   *         ----> obj4
   *
   * will result in 4 objects left on the heap.
   */
  corevm::dyobj::dyobj_id id1 = this->__create_obj();
  corevm::dyobj::dyobj_id id2 = this->__create_obj();
  corevm::dyobj::dyobj_id id3 = this->__create_obj();
  corevm::dyobj::dyobj_id id4 = this->__create_obj();

  this->__setattr(id1, id2);
  this->__setattr(id1, id3);
  this->__setattr(id1, id4);

  this->__set_not_garbage_collectible(id1);

  this->do_gc_and_check_results(4);
}

TYPED_TEST(garbage_collection_unittest, TestAdjacentCycles)
{
  /*
   * Tests gc on the following object graph
   *
   *        ----> obj2 --->
   *       /               \
   *      /                 \
   * obj1 ------> obj3 -----> obj1
   *      \                 /
   *       \               /
   *        ----> obj4 --->
   *
   * will result in 0 objects left on the heap.
   */
  corevm::dyobj::dyobj_id id1 = this->__create_obj();
  corevm::dyobj::dyobj_id id2 = this->__create_obj();
  corevm::dyobj::dyobj_id id3 = this->__create_obj();
  corevm::dyobj::dyobj_id id4 = this->__create_obj();

  this->__setattr(id1, id2);
  this->__setattr(id1, id3);
  this->__setattr(id1, id4);

  this->__setattr(id2, id1);
  this->__setattr(id3, id1);
  this->__setattr(id4, id1);

  this->do_gc_and_check_results(0);
}

TYPED_TEST(garbage_collection_unittest, TestTwoIsolatedCycles)
{
  /*
   * Tests gc on the following object graph
   *
   * obj1 -> obj2 -> obj3  obj4 -> obj5 -> obj6
   *  ^               |     ^                |
   *  |_______________|     |________________|
   *
   * will result in 0 objects left on the heap.
   */
  corevm::dyobj::dyobj_id id1 = this->__create_obj();
  corevm::dyobj::dyobj_id id2 = this->__create_obj();
  corevm::dyobj::dyobj_id id3 = this->__create_obj();
  corevm::dyobj::dyobj_id id4 = this->__create_obj();
  corevm::dyobj::dyobj_id id5 = this->__create_obj();
  corevm::dyobj::dyobj_id id6 = this->__create_obj();

  this->__setattr(id1, id2);
  this->__setattr(id2, id3);
  this->__setattr(id3, id1);

  this->__setattr(id4, id5);
  this->__setattr(id5, id6);
  this->__setattr(id6, id4);

  this->do_gc_and_check_results(0);
}

TYPED_TEST(garbage_collection_unittest, TestNestedCycles)
{
  /*
   * Tests gc on the following object graph
   *
   *          ------>------ obj2 ----->-----
   *         /                               \
   *        /                 --- obj5 -->    \
   *       /                /             \    \
   *      /                /               \    \
   * obj1 -->--- obj3 ----->----- obj6 -----> obj1
   *      \                \               /    /
   *       \                \             /    /
   *        \                 --- obj7 -->    /
   *         \                               /
   *          ------>------ obj4 ----->------
   *
   * will result in 0 objects left on the heap.
   */
  corevm::dyobj::dyobj_id id1 = this->__create_obj();
  corevm::dyobj::dyobj_id id2 = this->__create_obj();
  corevm::dyobj::dyobj_id id3 = this->__create_obj();
  corevm::dyobj::dyobj_id id4 = this->__create_obj();
  corevm::dyobj::dyobj_id id5 = this->__create_obj();
  corevm::dyobj::dyobj_id id6 = this->__create_obj();
  corevm::dyobj::dyobj_id id7 = this->__create_obj();

  this->__setattr(id1, id2);
  this->__setattr(id1, id3);
  this->__setattr(id1, id4);

  this->__setattr(id2, id1);
  this->__setattr(id4, id1);

  this->__setattr(id3, id5);
  this->__setattr(id3, id6);
  this->__setattr(id3, id7);

  this->__setattr(id5, id1);
  this->__setattr(id6, id1);
  this->__setattr(id7, id1);

  this->do_gc_and_check_results(0);
}

TYPED_TEST(garbage_collection_unittest, TestCycleWithInwardStub)
{
  /*
   * Tests gc on the following object graph
   *
   * obj1 -> obj2 -> obj3 <- obj4
   *  ^                |
   *  |________________|
   *
   * will result in 0 objects left on the heap.
   */
  corevm::dyobj::dyobj_id id1 = this->__create_obj();
  corevm::dyobj::dyobj_id id2 = this->__create_obj();
  corevm::dyobj::dyobj_id id3 = this->__create_obj();
  corevm::dyobj::dyobj_id id4 = this->__create_obj();

  this->__setattr(id1, id2);
  this->__setattr(id2, id3);
  this->__setattr(id3, id1);
  this->__setattr(id4, id3);

  this->do_gc_and_check_results(0);
}

TYPED_TEST(garbage_collection_unittest, TestCycleWithOutwardStub)
{
  /*
   * Tests gc on the following object graph
   *
   * obj1 -> obj2 -> obj3 -> obj4
   *  ^                |
   *  |________________|
   *
   * will result in 0 objects left on the heap.
   */
  corevm::dyobj::dyobj_id id1 = this->__create_obj();
  corevm::dyobj::dyobj_id id2 = this->__create_obj();
  corevm::dyobj::dyobj_id id3 = this->__create_obj();
  corevm::dyobj::dyobj_id id4 = this->__create_obj();

  this->__setattr(id1, id2);
  this->__setattr(id2, id3);
  this->__setattr(id3, id1);
  this->__setattr(id3, id4);

  this->do_gc_and_check_results(0);
}

TYPED_TEST(garbage_collection_unittest, TestCycleWithNonGarbageCollectibleInwardStub)
{
  /*
   * Tests gc on the following object graph
   *
   * obj1 -> obj2 -> obj3 <- obj4*
   *  ^                |
   *  |________________|
   *
   * will result in 4 objects left on the heap.
   */
  corevm::dyobj::dyobj_id id1 = this->__create_obj();
  corevm::dyobj::dyobj_id id2 = this->__create_obj();
  corevm::dyobj::dyobj_id id3 = this->__create_obj();
  corevm::dyobj::dyobj_id id4 = this->__create_obj();

  this->__setattr(id1, id2);
  this->__setattr(id2, id3);
  this->__setattr(id3, id1);
  this->__setattr(id4, id3);

  this->__set_not_garbage_collectible(id4);

  this->do_gc_and_check_results(4);
}

TYPED_TEST(garbage_collection_unittest, TestCycleWithTwoInwardStubs)
{
  /*
   * Tests gc on the following object graph
   *
   * obj5-> obj1 -> obj2 -> obj3 <- obj4*
   *         ^                |
   *         |________________|
   *
   * will result in 4 objects left on the heap.
   */
  corevm::dyobj::dyobj_id id1 = this->__create_obj();
  corevm::dyobj::dyobj_id id2 = this->__create_obj();
  corevm::dyobj::dyobj_id id3 = this->__create_obj();
  corevm::dyobj::dyobj_id id4 = this->__create_obj();
  corevm::dyobj::dyobj_id id5 = this->__create_obj();

  this->__setattr(id1, id2);
  this->__setattr(id2, id3);
  this->__setattr(id3, id1);
  this->__setattr(id4, id3);
  this->__setattr(id5, id1);

  this->__set_not_garbage_collectible(id4);

  this->do_gc_and_check_results(4);
}

TYPED_TEST(garbage_collection_unittest, TestCycleWithNonGarbageCollectibleOutwardStub)
{
  /*
   * Tests gc on the following object graph
   *
   * obj1 -> obj2 -> obj3 -> obj4*
   *  ^                |
   *  |________________|
   *
   * will result in 1 objects left on the heap.
   */
  corevm::dyobj::dyobj_id id1 = this->__create_obj();
  corevm::dyobj::dyobj_id id2 = this->__create_obj();
  corevm::dyobj::dyobj_id id3 = this->__create_obj();
  corevm::dyobj::dyobj_id id4 = this->__create_obj();

  this->__setattr(id1, id2);
  this->__setattr(id2, id3);
  this->__setattr(id3, id1);
  this->__setattr(id3, id4);

  this->__set_not_garbage_collectible(id4);

  this->do_gc_and_check_results(1);
}

TYPED_TEST(garbage_collection_unittest, TestCycleWithTwoOutwardStubs)
{
  /*
   * Tests gc on the following object graph
   *
   * obj5 <- obj1 -> obj2 -> obj3 -> obj4*
   *          ^                |
   *          |________________|
   *
   * will result in 1 objects left on the heap.
   */
  corevm::dyobj::dyobj_id id1 = this->__create_obj();
  corevm::dyobj::dyobj_id id2 = this->__create_obj();
  corevm::dyobj::dyobj_id id3 = this->__create_obj();
  corevm::dyobj::dyobj_id id4 = this->__create_obj();
  corevm::dyobj::dyobj_id id5 = this->__create_obj();

  this->__setattr(id1, id2);
  this->__setattr(id2, id3);
  this->__setattr(id3, id1);
  this->__setattr(id3, id4);
  this->__setattr(id1, id5);

  this->__set_not_garbage_collectible(id4);

  this->do_gc_and_check_results(1);
}

TYPED_TEST(garbage_collection_unittest, TestSingleCycleWithNonGarbageCollectibleObject)
{
  /*
   * Tests gc on the following object graph:
   *
   * obj1 -> obj2* -> obj3 ->
   *  ^                     |
   *  |_____________________|
   *
   * will result in 3 objects left on the heap.
   */
  corevm::dyobj::dyobj_id id1 = this->__create_obj();
  corevm::dyobj::dyobj_id id2 = this->__create_obj();
  corevm::dyobj::dyobj_id id3 = this->__create_obj();

  this->__setattr(id1, id2);
  this->__setattr(id2, id3);
  this->__setattr(id3, id1);

  this->__set_not_garbage_collectible(id2);

  this->do_gc_and_check_results(3);
}
