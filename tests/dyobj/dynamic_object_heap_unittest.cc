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

#include <sneaker/testing/_unittest.h>

#include <sstream>
#include <vector>


class dummy_dynamic_object_manager : public corevm::dyobj::dynamic_object_manager
{
public:
  virtual bool garbage_collectible() const noexcept { return true; }
  virtual void on_create() noexcept {}
  virtual void on_setattr() noexcept {}
  virtual void on_delattr() noexcept {}
  virtual void on_delete() noexcept {}
  virtual void on_exit() noexcept {}
};

// -----------------------------------------------------------------------------

class dynamic_object_heap_unittest : public ::testing::Test
{
protected:
  dynamic_object_heap_unittest()
    :
    m_heap(1024)
  {
  }

  corevm::dyobj::dynamic_object_heap<dummy_dynamic_object_manager> m_heap;
};

// -----------------------------------------------------------------------------

TEST_F(dynamic_object_heap_unittest, TestCreateDyobj)
{
  corevm::dyobj::dyobj_id id1 = m_heap.create_dyobj();
  corevm::dyobj::dyobj_id id2 = m_heap.create_dyobj();

  ASSERT_NE(0, id1);
  ASSERT_NE(0, id2);

  // This assumption is true for now, but might change in the future.
  ASSERT_LT(id1, id2);

  // Tests that we can get the objects by those ids and they are equivalent.
  corevm::dyobj::dynamic_object<dummy_dynamic_object_manager>& obj1 = m_heap.at(id1);
  corevm::dyobj::dynamic_object<dummy_dynamic_object_manager>& obj2 = m_heap.at(id2);

  ASSERT_EQ(id1, obj1.id());
  ASSERT_EQ(id2, obj2.id());

  // Clean up.
  m_heap.erase(id1);
  m_heap.erase(id2);
}

// -----------------------------------------------------------------------------

TEST_F(dynamic_object_heap_unittest, TestBulkCreate)
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
    m_heap.erase(p[i].id());
  }
}

// -----------------------------------------------------------------------------

TEST_F(dynamic_object_heap_unittest, TestAtOnNonExistentKeys)
{
  corevm::dyobj::dyobj_id id1 = 0;
  corevm::dyobj::dyobj_id id2 = 1;

  ASSERT_THROW(m_heap.at(id1), corevm::dyobj::object_not_found_error);
  ASSERT_THROW(m_heap.at(id2), corevm::dyobj::object_not_found_error);
}

// -----------------------------------------------------------------------------

TEST_F(dynamic_object_heap_unittest, TestAllocationOverMaxSize)
{
  /* NOTE: This test may not work if using buddy allocation for the heap. */

  auto max_size = m_heap.max_size();
  std::vector<corevm::dyobj::dyobj_id> ids;

  for (auto i = 1; i <= max_size; ++i)
  {
    ids.push_back(m_heap.create_dyobj());
  }

  ASSERT_THROW(
    {
      m_heap.create_dyobj();
    },
    corevm::dyobj::object_creation_error
  );

  // Clean up.
  for (auto i = 0; i < ids.size(); ++i)
  {
    m_heap.erase(ids[i]);
  }
}

// -----------------------------------------------------------------------------

TEST_F(dynamic_object_heap_unittest, TestOutputStream)
{
  corevm::dyobj::dyobj_id id1 = m_heap.create_dyobj();
  corevm::dyobj::dyobj_id id2 = m_heap.create_dyobj();
  corevm::dyobj::dyobj_id id3 = m_heap.create_dyobj();

  ASSERT_NE(0, id1);
  ASSERT_NE(0, id2);
  ASSERT_NE(0, id3);

  std::stringstream ss;
  ss << m_heap;

  ASSERT_NE(0, ss.str().size());

  m_heap.erase(id1);
  m_heap.erase(id2);
  m_heap.erase(id3);
}

// -----------------------------------------------------------------------------
