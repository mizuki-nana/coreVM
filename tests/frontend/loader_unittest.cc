/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2014 Yanzheng Li

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
#include <cassert>
#include <fstream>
#include <string>
#include <sneaker/testing/_unittest.h>
#include "../../include/frontend/loader.h"


class loader_unittest : public ::testing::Test {
protected:
  static const char* PATH;
  static const std::string BYTECODE;

  virtual void SetUp() {
    std::ofstream f(PATH, std::ios::binary);
    f << loader_unittest::BYTECODE;
    f.close();
  }

  virtual void TearDown() {
    remove(PATH);
  }
};

const char* loader_unittest::PATH = "./example.corevm";

const std::string loader_unittest::BYTECODE = \
  "{"
    "\"format\": \"corevm\","
    "\"format-version\": \"0.1\","
    "\"target-version\": \"0.1\","
    "\"path\": \"./example.corevm\","
    "\"timestamp\": \"2014-10-12T15:33:30\","
    "\"encoding\": \"utf8\","
    "\"author\": \"Yanzheng Li\","
    "\"encoding_map\": \"{}\","
    "\"__MAIN__\": {"
      "\"attributes\": {"
        "\"is_garbage_collectible\": false"
      "},"
      "\"vector\": ["
        "{"
          "\"__name__\": \"__main__\","
          "\"__inner__\": ["
            "[101, 102, 103],"
            "[201, 202, 203],"
            "[301, 302, 303]"
          "]"
        "},"
        "{"
          "\"__name__\": \"hello_world\","
          "\"__inner__\": ["
            "[701, 702, 703],"
            "[801, 802, 803],"
            "[901, 902, 903]"
          "],"
          "\"__parent__\": \"__main__\""
        "}"
      "]"
    "}"
  "}";


TEST_F(loader_unittest, TestLoadSuccessful)
{
  corevm::frontend::loader loader(PATH);

  ASSERT_NO_THROW(
    loader.load()
  );
}

TEST_F(loader_unittest, TestLoadFailsWithInvalidPath)
{
  corevm::frontend::loader loader("some/random/path/example.core");

  ASSERT_THROW(
    loader.load(),
    corevm::frontend::file_loading_error
  );
}
