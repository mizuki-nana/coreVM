#include <algorithm>
#include <iterator>
#include <string>
#include <unordered_map>
#include "../../include/frontend/schema_repository.h"

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
