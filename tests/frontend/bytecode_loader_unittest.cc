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
#include "../../include/frontend/bytecode_loader.h"

#include "../../include/runtime/process.h"

#include <sneaker/testing/_unittest.h>

#include <cassert>
#include <fstream>
#include <string>


class bytecode_loader_unittest : public ::testing::Test {
protected:
  static const char* PATH;

  virtual void SetUp() {
    std::ofstream f(PATH, std::ios::binary);
    f << this->bytecode();
    f.close();
  }

  virtual void TearDown() {
    remove(PATH);
  }

  virtual const char* bytecode() {
    return "";
  }
};


const char* bytecode_loader_unittest::PATH = "./example.corevm";


class bytecode_loader_v0_1_unittest : public bytecode_loader_unittest {
protected:
  virtual const char* bytecode() {
    return \
    "{"
      "\"format\": \"corevm\","
      "\"format-version\": \"0.1\","
      "\"target-version\": \"0.1\","
      "\"path\": \"./example.corevm\","
      "\"timestamp\": \"2014-10-12T15:33:30\","
      "\"encoding\": \"utf8\","
      "\"author\": \"Yanzheng Li\","
      "\"encoding_map\": {"
        "\"name\": 111"
      "},"
      "\"__MAIN__\": ["
        "{"
          "\"__name__\": \"hello_world\","
          "\"__vector__\": ["
            "[701, 702, 703],"
            "[801, 802, 803],"
            "[901, 902, 903]"
          "],"
          "\"__parent__\": \"__main__\""
        "},"
        "{"
          "\"__name__\": \"__main__\","
          "\"__vector__\": ["
            "[701, 702, 703],"
            "[801, 802, 803],"
            "[901, 902, 903]"
          "]"
        "}"
      "]"
    "}";
  }
};


TEST_F(bytecode_loader_unittest, TestLoadFailsWithInvalidPath)
{
  std::string invalid_path("some/random/path/example.core");
  corevm::runtime::process process;

  ASSERT_THROW(
    {
      corevm::frontend::load(invalid_path, process);
    },
    corevm::frontend::file_loading_error
  );
}


TEST_F(bytecode_loader_v0_1_unittest, TestLoadSuccessful)
{
  corevm::runtime::process process;

  ASSERT_NO_THROW(
    {
      corevm::frontend::load(PATH, process);
    }
  );
}
