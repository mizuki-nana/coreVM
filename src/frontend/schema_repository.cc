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
#include "../../include/frontend/schema_repository.h"

#include <algorithm>
#include <iterator>
#include <string>
#include <unordered_map>


const std::unordered_map<std::string, std::string>
corevm::frontend::schema_repository::format_version_to_schema_map {
  {
    "0.1",
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"format\": {"
          "\"type\": \"string\""
        "},"
        "\"format-version\": {"
          "\"type\": \"string\""
        "},"
        "\"target-version\": {"
          "\"type\": \"string\""
        "},"
        "\"path\": {"
          "\"type\": \"string\""
        "},"
        "\"timestamp\": {"
          "\"type\": \"string\","
          "\"format\": \"date-time\""
        "},"
        "\"encoding\": {"
          "\"type\": \"string\","
          "\"enum\": ["
            "\"utf8\""
          "]"
        "},"
        "\"author\": {"
          "\"type\": \"string\""
        "},"
        "\"encoding_map\": {"
          "\"type\": \"object\","
          "\"additionalProperties\": true"
        "},"
        "\"__MAIN__\": {"
          "\"$ref\": \"#/definitions/object\""
        "}"
      "},"
      "\"additionalProperties\": false,"
      "\"required\": ["
        "\"format\","
        "\"format-version\","
        "\"target-version\","
        "\"encoding\","
        "\"encoding_map\","
        "\"__MAIN__\""
      "],"
      "\"definitions\": {"
        "\"instr\": {"
          "\"type\": \"integer\","
          "\"minimum\": 0,"
          "\"maximum\": 4294967295"
        "},"
        "\"vector\": {"
          "\"type\": \"array\","
          "\"items\": {"
            "\"type\": \"object\","
            "\"properties\": {"
              "\"__name__\": {"
                "\"type\": \"string\""
              "},"
              "\"__inner__\": {"
                "\"type:\": \"array\","
                "\"items\": {"
                  "\"type:\": \"array\","
                  "\"items\": ["
                    "{"
                      "\"$ref\": \"#/definitions/instr\""
                    "},"
                    "{"
                      "\"$ref\": \"#/definitions/instr\""
                    "},"
                    "{"
                      "\"$ref\": \"#/definitions/instr\""
                    "}"
                  "],"
                  "\"minItems\": 1,"
                  "\"maxItems\": 3,"
                  "\"additionalItems\": false"
                "}"
              "},"
              "\"__parent__\": {"
                "\"type\": \"string\""
              "}"
            "},"
            "\"required\": ["
              "\"__name__\","
              "\"__inner__\""
            "]"
          "}"
        "},"
        "\"attributes\": {"
          "\"type\": \"object\","
          "\"properties\": {"
            "\"is_garbage_collectible\": {"
              "\"type\": \"boolean\""
            "}"
          "},"
          "\"additionalProperties\": false"
        "},"
        "\"object\": {"
          "\"type\": \"object\","
          "\"properties\": {"
            "\"attributes\": {"
              "\"$ref\": \"#/definitions/attributes\""
            "},"
            "\"vector\": {"
              "\"$ref\": \"#/definitions/vector\""
            "}"
          "},"
          "\"required\": ["
            "\"attributes\","
            "\"vector\""
          "]"
        "}"
      "}"
    "}"
  }, /* end 0.1 */
};


struct key_retriever
{
  template<typename T>
  typename T::first_type operator()(T pair) const {
    return pair.first;
  }
};


const std::set<std::string>
corevm::frontend::schema_repository::list_supported_versions()
{
  std::set<std::string> supported_versions;

  std::transform(
    format_version_to_schema_map.begin(),
    format_version_to_schema_map.end(),
    std::inserter(supported_versions, supported_versions.begin()),
    key_retriever()
  );

  return supported_versions;
}

const std::string
corevm::frontend::schema_repository::load_by_format_version(
  const std::string& format_version)
{
  auto itr = format_version_to_schema_map.find(format_version);

  if(itr == format_version_to_schema_map.end()) {
    return std::string();
  }

  std::string schema_str = itr->second;

  return schema_str;
}
