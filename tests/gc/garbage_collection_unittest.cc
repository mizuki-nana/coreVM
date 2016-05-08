/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Yanzheng Li

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
#include "corevm/macros.h"
#include "dyobj/dynamic_object.h"
#include "dyobj/dynamic_object_heap.h"
#include "gc/garbage_collector.h"
#include "gc/refcount_gc_scheme.h"

#include <gtest/gtest.h>

#include <vector>


template<class GarbageCollectionScheme>
class GarbageCollectionUnitTest : public ::testing::Test
{
protected:
  using _GarbageCollectorType = typename corevm::gc::GarbageCollector<GarbageCollectionScheme>;

  using _ObjectType = typename corevm::dyobj::DynamicObject<
    typename GarbageCollectionScheme::DynamicObjectManager>;

  void do_gc_and_check_results(const std::vector<_ObjectType*>& objs)
  {
    _GarbageCollectorType collector(m_heap);
    collector.gc();

    ASSERT_EQ(m_heap.size(), objs.size());

    for (const auto obj : objs)
    {
      ASSERT_NO_THROW(
        {
          m_heap.at(obj->id());
        }
      );
    }
  }

  void set_on_attached(_ObjectType* obj)
  {
    obj->manager().on_setattr();
    obj->manager().dec_ref_count();
  }

  _ObjectType* help_create_obj()
  {
    auto obj = m_heap.create_dyobj();
    set_on_attached(obj);
    return obj;
  }

  void help_setattr(_ObjectType* src_obj, _ObjectType* dst_obj)
  {
    src_obj->putattr(static_cast<corevm::dyobj::attr_key_t>(dst_obj->id()), dst_obj);
    dst_obj->manager().on_setattr();
  }

  void help_set_as_non_garbage_collectible(_ObjectType* obj)
  {
    obj->set_flag(corevm::dyobj::DynamicObjectFlagBits::DYOBJ_IS_NOT_GARBAGE_COLLECTIBLE);
  }

  corevm::dyobj::DynamicObjectHeap<
    typename GarbageCollectionScheme::DynamicObjectManager> m_heap;
};

// -----------------------------------------------------------------------------

typedef ::testing::Types<
  corevm::gc::RefCountGarbageCollectionScheme
> GarbageCollectionSchemeTypes;

// -----------------------------------------------------------------------------

TYPED_TEST_CASE(GarbageCollectionUnitTest, GarbageCollectionSchemeTypes);

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestOneObject)
{
  /**
   * Tests GC on the following object graph:
   *
   *  obj
   *
   * will result in 0 objects left on the heap.
   */
  this->help_create_obj();

  this->do_gc_and_check_results({});
}

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestSelfReferencedObject)
{
  /**
   * Tests GC on the following object graph:
   *
   * obj --->
   *  ^     |
   *  |_____|
   *
   * will result in 0 object left on the heap.
   */
  auto obj = this->help_create_obj();

  this->help_setattr(obj, obj);

  this->do_gc_and_check_results({});
}

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestSelfReferenceOnNonGarbageCollectibleObject)
{
  /**
   * Tests GC on the following object graph:
   *
   * obj* -->
   *  ^     |
   *  |_____|
   *
   * will result in 1 object left on the heap.
   */
  auto obj = this->help_create_obj();

  this->help_setattr(obj, obj);
  this->help_set_as_non_garbage_collectible(obj);

  this->do_gc_and_check_results({obj});
}

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestLinearChain)
{
  /**
   * Tests GC on the following object graph:
   *
   *  obj1 -> obj2 -> obj3 -> obj4
   *
   * will result in 0 objects left on the heap.
   */
  auto obj1 = this->help_create_obj();
  auto obj2 = this->help_create_obj();
  auto obj3 = this->help_create_obj();
  auto obj4 = this->help_create_obj();

  this->help_setattr(obj1, obj2);
  this->help_setattr(obj2, obj3);
  this->help_setattr(obj3, obj4);

  this->do_gc_and_check_results({});
}

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestLinearChainWithNonGarbageCollectibleObject)
{
  /**
   * Tests GC on the following object graph:
   *
   *  obj1* -> obj2 -> obj3 -> obj4
   *
   * will result in 4 objects left on the heap.
   */
  auto obj1 = this->help_create_obj();
  auto obj2 = this->help_create_obj();
  auto obj3 = this->help_create_obj();
  auto obj4 = this->help_create_obj();

  this->help_setattr(obj1, obj2);
  this->help_setattr(obj2, obj3);
  this->help_setattr(obj3, obj4);

  this->help_set_as_non_garbage_collectible(obj1);

  this->do_gc_and_check_results({obj1, obj2, obj3, obj4});
}

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestLinearChainWithNonGarbageCollectibleObjects)
{
  /**
   * Tests GC on the following object graph:
   *
   *  obj1 -> obj2* -> obj3* -> obj4
   *
   * will result in 3 objects left on the heap.
   */
  auto obj1 = this->help_create_obj();
  auto obj2 = this->help_create_obj();
  auto obj3 = this->help_create_obj();
  auto obj4 = this->help_create_obj();

  this->help_setattr(obj1, obj2);
  this->help_setattr(obj2, obj3);
  this->help_setattr(obj3, obj4);

  this->help_set_as_non_garbage_collectible(obj2);
  this->help_set_as_non_garbage_collectible(obj3);

  this->do_gc_and_check_results({obj2, obj3, obj4});
}

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestSingleCycle)
{
  /**
   * Tests GC on the following object graph:
   *
   * obj1 -> obj2 -> obj3 ->
   *  ^                    |
   *  |____________________|
   *
   * will result in 0 objects left on the heap.
   */
  auto obj1 = this->help_create_obj();
  auto obj2 = this->help_create_obj();
  auto obj3 = this->help_create_obj();

  this->help_setattr(obj1, obj2);
  this->help_setattr(obj2, obj3);
  this->help_setattr(obj3, obj1);

  this->do_gc_and_check_results({});
}

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestMultipleObjectsPointToOne)
{
  /**
   * Tests GC on the following object graph
   *
   * obj1 --->\
   *           \
   * obj2 ------> obj4
   *           /
   * obj3 --->/
   *
   * will result in 0 objects left on the heap.
   */
  auto obj1 = this->help_create_obj();
  auto obj2 = this->help_create_obj();
  auto obj3 = this->help_create_obj();
  auto obj4 = this->help_create_obj();

  this->help_setattr(obj1, obj4);
  this->help_setattr(obj2, obj4);
  this->help_setattr(obj3, obj4);

  this->do_gc_and_check_results({});
}

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestOnePointsToMultipleObjects)
{
  /**
   * Tests GC on the following object graph
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
  auto obj1 = this->help_create_obj();
  auto obj2 = this->help_create_obj();
  auto obj3 = this->help_create_obj();
  auto obj4 = this->help_create_obj();

  this->help_setattr(obj1, obj2);
  this->help_setattr(obj1, obj3);
  this->help_setattr(obj1, obj4);

  this->do_gc_and_check_results({});
}

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestNonGarbageCollectibleObjectPointsToMultipleObjects)
{
  /**
   * Tests GC on the following object graph
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
  auto obj1 = this->help_create_obj();
  auto obj2 = this->help_create_obj();
  auto obj3 = this->help_create_obj();
  auto obj4 = this->help_create_obj();

  this->help_setattr(obj1, obj2);
  this->help_setattr(obj1, obj3);
  this->help_setattr(obj1, obj4);

  this->help_set_as_non_garbage_collectible(obj1);

  this->do_gc_and_check_results({obj1, obj2, obj3, obj4});
}

// -----------------------------------------------------------------------------

#ifndef COREVM_485
TYPED_TEST(GarbageCollectionUnitTest, TestAdjacentCycles)
{
  /**
   * Tests GC on the following object graph
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
  auto obj1 = this->help_create_obj();
  auto obj2 = this->help_create_obj();
  auto obj3 = this->help_create_obj();
  auto obj4 = this->help_create_obj();

  this->help_setattr(obj1, obj2);
  this->help_setattr(obj1, obj3);
  this->help_setattr(obj1, obj4);

  this->help_setattr(obj2, obj1);
  this->help_setattr(obj3, obj1);
  this->help_setattr(obj4, obj1);

  this->do_gc_and_check_results({});
}
#endif

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestTwoIsolatedCycles)
{
  /**
   * Tests GC on the following object graph
   *
   * obj1 -> obj2 -> obj3  obj4 -> obj5 -> obj6
   *  ^               |     ^                |
   *  |_______________|     |________________|
   *
   * will result in 0 objects left on the heap.
   */
  auto obj1 = this->help_create_obj();
  auto obj2 = this->help_create_obj();
  auto obj3 = this->help_create_obj();
  auto obj4 = this->help_create_obj();
  auto obj5 = this->help_create_obj();
  auto obj6 = this->help_create_obj();

  this->help_setattr(obj1, obj2);
  this->help_setattr(obj2, obj3);
  this->help_setattr(obj3, obj1);

  this->help_setattr(obj4, obj5);
  this->help_setattr(obj5, obj6);
  this->help_setattr(obj6, obj4);

  this->do_gc_and_check_results({});
}

// -----------------------------------------------------------------------------

#ifndef COREVM_485
TYPED_TEST(GarbageCollectionUnitTest, TestNestedCycles)
{
  /*
   * Tests GC on the following object graph
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
  auto obj1 = this->help_create_obj();
  auto obj2 = this->help_create_obj();
  auto obj3 = this->help_create_obj();
  auto obj4 = this->help_create_obj();
  auto obj5 = this->help_create_obj();
  auto obj6 = this->help_create_obj();
  auto obj7 = this->help_create_obj();

  this->help_setattr(obj1, obj2);
  this->help_setattr(obj1, obj3);
  this->help_setattr(obj1, obj4);

  this->help_setattr(obj2, obj1);
  this->help_setattr(obj4, obj1);

  this->help_setattr(obj3, obj5);
  this->help_setattr(obj3, obj6);
  this->help_setattr(obj3, obj7);

  this->help_setattr(obj5, obj1);
  this->help_setattr(obj6, obj1);
  this->help_setattr(obj7, obj1);

  this->do_gc_and_check_results({});
}
#endif

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestCycleWithInwardStub)
{
  /**
   * Tests GC on the following object graph
   *
   * obj1 -> obj2 -> obj3 <- obj4
   *  ^                |
   *  |________________|
   *
   * will result in 0 objects left on the heap.
   */
  auto obj1 = this->help_create_obj();
  auto obj2 = this->help_create_obj();
  auto obj3 = this->help_create_obj();
  auto obj4 = this->help_create_obj();

  this->help_setattr(obj1, obj2);
  this->help_setattr(obj2, obj3);
  this->help_setattr(obj3, obj1);
  this->help_setattr(obj4, obj3);

  this->do_gc_and_check_results({});
}

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestCycleWithOutwardStub)
{
  /**
   * Tests GC on the following object graph
   *
   * obj1 -> obj2 -> obj3 -> obj4
   *  ^                |
   *  |________________|
   *
   * will result in 0 objects left on the heap.
   */
  auto obj1 = this->help_create_obj();
  auto obj2 = this->help_create_obj();
  auto obj3 = this->help_create_obj();
  auto obj4 = this->help_create_obj();

  this->help_setattr(obj1, obj2);
  this->help_setattr(obj2, obj3);
  this->help_setattr(obj3, obj1);
  this->help_setattr(obj3, obj4);

  this->do_gc_and_check_results({});
}

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestCycleWithStronglyReferencedInwardStub)
{
  /**
   * Tests GC on the following object graph
   *
   * obj1 -> obj2 -> obj3 <- obj4**
   *  ^                |
   *  |________________|
   *
   * will result in 4 objects left on the heap.
   */
  auto obj1 = this->help_create_obj();
  auto obj2 = this->help_create_obj();
  auto obj3 = this->help_create_obj();
  auto obj4 = this->help_create_obj();

  this->help_setattr(obj1, obj2);
  this->help_setattr(obj2, obj3);
  this->help_setattr(obj3, obj1);
  this->help_setattr(obj4, obj3);

  obj4->manager().inc_ref_count();

  this->do_gc_and_check_results({obj1, obj2, obj3, obj4});
}

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestCycleWithStronglyReferencedOutwardStub)
{
  /**
   * Tests GC on the following object graph
   *
   * obj1 -> obj2 -> obj3 -> obj4**
   *  ^                |
   *  |________________|
   *
   * will result in 1 objects left on the heap.
   */
  auto obj1 = this->help_create_obj();
  auto obj2 = this->help_create_obj();
  auto obj3 = this->help_create_obj();
  auto obj4 = this->help_create_obj();

  this->help_setattr(obj1, obj2);
  this->help_setattr(obj2, obj3);
  this->help_setattr(obj3, obj1);
  this->help_setattr(obj3, obj4);

  obj4->manager().inc_ref_count();

  this->do_gc_and_check_results({obj4});
}

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestCycleWithNonGarbageCollectibleInwardStub)
{
  /**
   * Tests GC on the following object graph
   *
   * obj1 -> obj2 -> obj3 <- obj4*
   *  ^                |
   *  |________________|
   *
   * will result in 4 objects left on the heap.
   */
  auto obj1 = this->help_create_obj();
  auto obj2 = this->help_create_obj();
  auto obj3 = this->help_create_obj();
  auto obj4 = this->help_create_obj();

  this->help_setattr(obj1, obj2);
  this->help_setattr(obj2, obj3);
  this->help_setattr(obj3, obj1);
  this->help_setattr(obj4, obj3);

  this->help_set_as_non_garbage_collectible(obj4);

  this->do_gc_and_check_results({obj1, obj2, obj3, obj4});
}

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestCycleWithTwoInwardStubs)
{
  /**
   * Tests GC on the following object graph
   *
   * obj5-> obj1 -> obj2 -> obj3 <- obj4*
   *         ^                |
   *         |________________|
   *
   * will result in 4 objects left on the heap.
   */
  auto obj1 = this->help_create_obj();
  auto obj2 = this->help_create_obj();
  auto obj3 = this->help_create_obj();
  auto obj4 = this->help_create_obj();
  auto obj5 = this->help_create_obj();

  this->help_setattr(obj1, obj2);
  this->help_setattr(obj2, obj3);
  this->help_setattr(obj3, obj1);
  this->help_setattr(obj4, obj3);
  this->help_setattr(obj5, obj1);

  this->help_set_as_non_garbage_collectible(obj4);

  this->do_gc_and_check_results({obj1, obj2, obj3, obj4});
}

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestCycleWithNonGarbageCollectibleOutwardStub)
{
  /**
   * Tests GC on the following object graph
   *
   * obj1 -> obj2 -> obj3 -> obj4*
   *  ^                |
   *  |________________|
   *
   * will result in 1 object left on the heap.
   */
  auto obj1 = this->help_create_obj();
  auto obj2 = this->help_create_obj();
  auto obj3 = this->help_create_obj();
  auto obj4 = this->help_create_obj();

  this->help_setattr(obj1, obj2);
  this->help_setattr(obj2, obj3);
  this->help_setattr(obj3, obj1);
  this->help_setattr(obj3, obj4);

  this->help_set_as_non_garbage_collectible(obj4);

  this->do_gc_and_check_results({obj4});
}

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestCycleWithTwoOutwardStubs)
{
  /**
   * Tests GC on the following object graph
   *
   * obj5 <- obj1 -> obj2 -> obj3 -> obj4*
   *          ^                |
   *          |________________|
   *
   * will result in 1 object left on the heap.
   */
  auto obj1 = this->help_create_obj();
  auto obj2 = this->help_create_obj();
  auto obj3 = this->help_create_obj();
  auto obj4 = this->help_create_obj();
  auto obj5 = this->help_create_obj();

  this->help_setattr(obj1, obj2);
  this->help_setattr(obj2, obj3);
  this->help_setattr(obj3, obj1);
  this->help_setattr(obj3, obj4);
  this->help_setattr(obj1, obj5);

  this->help_set_as_non_garbage_collectible(obj4);

  this->do_gc_and_check_results({obj4});
}

// -----------------------------------------------------------------------------

TYPED_TEST(GarbageCollectionUnitTest, TestSingleCycleWithNonGarbageCollectibleObject)
{
  /**
   * Tests GC on the following object graph:
   *
   * obj1 -> obj2* -> obj3 ->
   *  ^                     |
   *  |_____________________|
   *
   * will result in 3 objects left on the heap.
   */
  auto obj1 = this->help_create_obj();
  auto obj2 = this->help_create_obj();
  auto obj3 = this->help_create_obj();

  this->help_setattr(obj1, obj2);
  this->help_setattr(obj2, obj3);
  this->help_setattr(obj3, obj1);

  this->help_set_as_non_garbage_collectible(obj2);

  this->do_gc_and_check_results({obj1, obj2, obj3});
}

// -----------------------------------------------------------------------------
