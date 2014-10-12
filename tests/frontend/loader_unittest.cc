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
