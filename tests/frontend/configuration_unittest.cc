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
#include "../../include/frontend/configuration.h"

#include <sneaker/testing/_unittest.h>

#include <fstream>
#include <string>


class configuration_unittest : public ::testing::Test
{
protected:
  static const char* PATH;

  virtual void SetUp()
  {
    std::ofstream f(PATH, std::ios::binary);
    f << this->content();
    f.close();
  }

  virtual void TearDown()
  {
    remove(PATH);
  }

  virtual const std::string content()
  {
    static const std::string content(
      "{"
        "\"alloc-size\": 1024,"
        "\"gc-interval\": 100"
      "}"
    );

    return content;
  }
};

// -----------------------------------------------------------------------------

const char* configuration_unittest::PATH = "./sample-config.config";

// -----------------------------------------------------------------------------

TEST_F(configuration_unittest, TestLoadSuccessful)
{
  corevm::frontend::configuration configuration(PATH);

  configuration.load_config();

  ASSERT_EQ(1024, configuration.alloc_size());
  ASSERT_EQ(100, configuration.gc_interval());
}

// -----------------------------------------------------------------------------

TEST_F(configuration_unittest, TestLoadFailsWithInvalidPath)
{
  corevm::frontend::configuration configuration("$%^some-invalid-path!@#");

  ASSERT_THROW(
    {
      configuration.load_config();
    },
    corevm::frontend::configuration_loading_error
  );
}

// -----------------------------------------------------------------------------
