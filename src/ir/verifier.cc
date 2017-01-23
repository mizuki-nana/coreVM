/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2017 Yanzheng Li

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
#include "verifier.h"
#include "format.h"
#include "ir_module_index.h"
#include <cstdio>
#include <unordered_set>


namespace corevm {
namespace ir {

// -----------------------------------------------------------------------------

Verifier::Verifier(const IRModule& module)
  :
  m_module(module),
  m_index(new IRModuleIndex(module)),
  m_msg()
{
}

// -----------------------------------------------------------------------------

bool
Verifier::run(std::string& msg)
{
  if (!check_type_decls() || !check_func_defs())
  {
    msg = std::move(m_msg);
    return false;
  }

  return true;
}

// -----------------------------------------------------------------------------

void
Verifier::get_index(std::unique_ptr<IRModuleIndex>& index)
{
  m_index.swap(index);
}

// -----------------------------------------------------------------------------

bool
Verifier::check_type_decls()
{
  std::unordered_set<std::string> type_name_set;
  for (const auto& type_decl : m_module.types)
  {
    auto itr = type_name_set.find(type_decl.name);
    if (itr == type_name_set.end())
    {
      type_name_set.insert(type_decl.name);
    }
    else
    {
      char buf[256] = {0};
      snprintf(buf, sizeof(buf),
        "Duplicate type definition of \"%s\"", type_decl.name.c_str());
      m_msg.assign(buf);
      return false;
    }
  }

  for (const auto& type_decl : m_module.types)
  {
    for (const auto& field : type_decl.fields)
    {
      if (!check_identifier_type(field.type))
      {
        char buf[256] = {0};
        snprintf(buf, sizeof(buf),
          "Unknown type used for type member %s::%s",
          type_decl.name.c_str(), field.identifier.c_str());
        m_msg.assign(buf);
        return false;
      }
    }
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_func_defs()
{
  // TODO: to be implemented.
  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_identifier_type(const IRIdentifierType& identifier_type)
{
  switch (identifier_type.type)
  {
  case IdentifierType_Identifier:
    {
      const std::string type_name = identifier_type.value.get_string();
      if (!check_type_string(type_name))
      {
        return false;
      }
    }
    break;
  case IdentifierType_Array:
    {
      const IRArrayType array_type = identifier_type.value.get_IRArrayType();
      if (!check_identifier_type(array_type.type))
      {
        return false;
      }
    }
    break;
  case IdentifierType_ValueType:
    // Do nothing.
    break;
  default:
    {
      char buf[256] = {0};
      snprintf(buf, sizeof(buf), "Invalid type field type: %u", identifier_type.type);
      m_msg.assign(buf);
      return false;
    }
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
Verifier::check_type_string(const std::string& type_name)
{
  return m_index->type_index.find(type_name) != m_index->type_index.end();
}

// -----------------------------------------------------------------------------

} /* end namespace ir */
} /* end namespace corevm */
