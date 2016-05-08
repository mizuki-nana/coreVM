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
#include "runtime/catch_site.h"
#include "runtime/compartment.h"
#include "runtime/closure.h"
#include "runtime/loc_info.h"
#include "runtime/vector.h"

#include <gtest/gtest.h>


class CompartmentUnitTest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(CompartmentUnitTest, TestGetClosureByID)
{
  corevm::runtime::Compartment compartment("./example.core");

  corevm::runtime::Vector vector;
  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::Closure closure(
    /* name */ "__main__",
    /* id */ 0,
    /* parent_id */ corevm::runtime::NONESET_CLOSURE_ID,
    /* vector */ vector,
    /* locs */ locs,
    /* catch_sites */ catch_sites);

  corevm::runtime::ClosureTable closure_table {
    closure
  };

  compartment.set_closure_table(std::move(closure_table));

  corevm::runtime::Closure* result = nullptr;

  compartment.get_closure_by_id(closure.id, &result);

  ASSERT_NE(nullptr, result);

  ASSERT_EQ(closure.id, result->id);
  ASSERT_EQ(closure.parent_id, result->parent_id);

  corevm::runtime::Closure* result2 = nullptr;
  corevm::runtime::closure_id_t invalid_id = 0xfff;

  compartment.get_closure_by_id(invalid_id, &result2);

  ASSERT_EQ(nullptr, result2);
}

// -----------------------------------------------------------------------------

TEST_F(CompartmentUnitTest, TestGetClosureByID2)
{
  corevm::runtime::Compartment compartment("./example.core");

  corevm::runtime::Vector vector;
  corevm::runtime::LocTable locs;
  corevm::runtime::CatchSiteList catch_sites;

  corevm::runtime::Closure closure(
    /* name */ "__main__",
    /* id */ 0,
    /* parent_id */ corevm::runtime::NONESET_CLOSURE_ID,
    /* vector */ vector,
    /* locs */ locs,
    /* catch_sites */ catch_sites);

  corevm::runtime::ClosureTable closure_table {
    closure
  };

  compartment.set_closure_table(std::move(closure_table));

  corevm::runtime::Closure result = compartment.get_closure_by_id(closure.id);

  ASSERT_EQ(closure.id, result.id);
  ASSERT_EQ(closure.parent_id, result.parent_id);

  corevm::runtime::closure_id_t invalid_id = 0xfff;

  ASSERT_THROW(
    {
      compartment.get_closure_by_id(invalid_id);
    },
    corevm::runtime::ClosureNotFoundError
  );
}

// -----------------------------------------------------------------------------

TEST_F(CompartmentUnitTest, TestGetFptLiteral)
{
  corevm::runtime::Compartment compartment("./example.core");
  const corevm::runtime::encoding_key_t encoding_key = 0;

  ASSERT_THROW(
    {
      compartment.get_fpt_literal(encoding_key);
    },
    corevm::runtime::FptLiteralNotFoundError
  );

  corevm::runtime::FptLiteralTable fpt_literal_table;
  const double fpt_literal = 3.14;
  fpt_literal_table.push_back(fpt_literal);

  compartment.set_fpt_literal_table(fpt_literal_table);

  const double actual_fpt_literal = compartment.get_fpt_literal(encoding_key);

  ASSERT_EQ(fpt_literal, actual_fpt_literal);

  const corevm::runtime::encoding_key_t invalid_encoding_key = encoding_key + 1;

  ASSERT_THROW(
    {
      compartment.get_fpt_literal(invalid_encoding_key);
    },
    corevm::runtime::FptLiteralNotFoundError
  );
}

// -----------------------------------------------------------------------------
