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
#include "dyobj/common.h"
#include "dyobj/dynamic_object.h"
#include "dyobj/dynamic_object_heap.h"
#include "dyobj/dynamic_object_manager.h"
#include "gc/garbage_collector.h"
#include "gc/garbage_collection_scheme.h"

#include <gtest/gtest.h>

#include <sstream>
#include <vector>


// -----------------------------------------------------------------------------

class DummyDynamicObjectManager : public corevm::dyobj::DynamicObjectManager
{
public:
  virtual bool garbage_collectible() const noexcept { return true; }
  virtual void on_create() noexcept;
  virtual void on_setattr() noexcept {}
  virtual void on_delattr() noexcept {}
  virtual void on_delete() noexcept {}
  virtual void on_exit() noexcept {}
};

// -----------------------------------------------------------------------------

/* virtual */
void
DummyDynamicObjectManager::on_create() noexcept
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

class DynamicObjectHeapUnitTest : public ::testing::Test
{
protected:
  DynamicObjectHeapUnitTest()
    :
    m_heap(1024)
  {
  }

  typedef corevm::dyobj::DynamicObjectHeap<DummyDynamicObjectManager> heap_type;
  typedef heap_type::dynamic_object_type dyobj_type;

  heap_type m_heap;
};

// -----------------------------------------------------------------------------

TEST_F(DynamicObjectHeapUnitTest, TestCreateDyobj)
{
  auto obj1_ptr = m_heap.create_dyobj();
  auto obj2_ptr = m_heap.create_dyobj();

  ASSERT_NE(nullptr, obj1_ptr);
  ASSERT_NE(nullptr, obj2_ptr);

  // This assumption is true for now, but might change in the future.
  ASSERT_LT(obj1_ptr, obj2_ptr);

  // Tests that we can get the objects by those ids and they are equivalent.
  corevm::dyobj::DynamicObject<DummyDynamicObjectManager>& obj1 = m_heap.at(obj1_ptr->id());
  corevm::dyobj::DynamicObject<DummyDynamicObjectManager>& obj2 = m_heap.at(obj2_ptr->id());

  ASSERT_EQ(obj1_ptr->id(), obj1.id());
  ASSERT_EQ(obj2_ptr->id(), obj2.id());

  // Clean up.
  m_heap.erase(obj1_ptr);
  m_heap.erase(obj2_ptr);
}

// -----------------------------------------------------------------------------

TEST_F(DynamicObjectHeapUnitTest, TestBulkCreate)
{
  const size_t N = 8;

  auto * p = m_heap.create_dyobjs(N);
  ASSERT_NE(nullptr, p);

  for (size_t i = 0; i < N; ++i)
  {
    auto id = p[i].id();

    ASSERT_NE(0, id);
    auto & obj = m_heap.at(id);

    ASSERT_EQ(id, obj.id());
  }

  // Clean up.
  for (size_t i = 0; i < N; ++i)
  {
    m_heap.erase(&p[i]);
  }
}

// -----------------------------------------------------------------------------

TEST_F(DynamicObjectHeapUnitTest, TestAtOnNonExistentKeys)
{
  corevm::dyobj::dyobj_id id1 = 0;
  corevm::dyobj::dyobj_id id2 = 1;

  ASSERT_THROW(m_heap.at(id1), corevm::dyobj::ObjectNotFoundError);
  ASSERT_THROW(m_heap.at(id2), corevm::dyobj::ObjectNotFoundError);
}

// -----------------------------------------------------------------------------

TEST_F(DynamicObjectHeapUnitTest, TestAllocationOverMaxSize)
{
  /* NOTE: This test may not work if using buddy allocation for the heap. */

  auto max_size = m_heap.max_size();
  std::vector<dyobj_type*> objs;

  for (size_t i = 1; i <= max_size; ++i)
  {
    objs.push_back(m_heap.create_dyobj());
  }

  ASSERT_THROW(
    {
      m_heap.create_dyobj();
    },
    corevm::dyobj::ObjectCreationError
  );

  // Clean up.
  for (size_t i = 0; i < objs.size(); ++i)
  {
    m_heap.erase(objs[i]);
  }
}

// -----------------------------------------------------------------------------

TEST_F(DynamicObjectHeapUnitTest, TestOutputStream)
{
  auto obj1 = m_heap.create_dyobj();
  auto obj2 = m_heap.create_dyobj();
  auto obj3 = m_heap.create_dyobj();

  ASSERT_NE(nullptr, obj1);
  ASSERT_NE(nullptr, obj2);
  ASSERT_NE(nullptr, obj3);

  std::stringstream ss;
  ss << m_heap;

  ASSERT_NE(0, ss.str().size());

  m_heap.erase(obj1);
  m_heap.erase(obj2);
  m_heap.erase(obj3);
}

// -----------------------------------------------------------------------------
