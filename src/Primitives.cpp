#include "Primitives.hpp"
#include "CppReflection.hpp"
#include "ast/ConstantNode.hpp"
#include <unordered_map>
#include <algorithm>


#define ENUM_VALUE(enum_type, enum_val)\
{GetEnumClassValueName<enum_type, enum_val>(), enum_val}

#define NAME_VALUE(enum_type, enum_val)\
{enum_val, GetName(GetEnumClassValueName<enum_type, enum_val>())}

static std::string GetSearchName(const std::string &name)
{
  auto searchName = name;
  std::transform(searchName.begin(), searchName.end(), searchName.begin(), ::toupper);
  return searchName;
}

static std::string GetName(const std::string &name)
{
  auto searchName = name;
  std::transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);
  return searchName;
}

namespace llang
{
using PrimitivesMap = std::unordered_map<std::string, PRIMITIVE_TYPE>;
using PrimitivesNameMap = std::unordered_map<PRIMITIVE_TYPE, std::string>;
static PrimitivesMap _primitives = {
  /* SPECIALS */
  ENUM_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::VOID),
  ENUM_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::BOOL),
  /* SIGNED */
  ENUM_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::INT8),
  ENUM_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::INT16),
  ENUM_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::INT32),
  ENUM_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::INT64),
  /* CHARACTERS */
  ENUM_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::SCHAR),
  ENUM_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::CHAR),
  ENUM_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::WCHAR),
  ENUM_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::UCHAR),
  /* UNSIGNED */
  ENUM_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::BYTE),   // Alias for uint
  ENUM_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::UINT8),
  ENUM_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::UINT16),
  ENUM_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::UINT32),
  ENUM_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::UINT64),
  /* FLOATING POINT */
  ENUM_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::FLOAT32),
  ENUM_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::FLOAT64)
};
static PrimitivesNameMap _primitivesNames = {
  /* SPECIALS */
  NAME_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::VOID),
  NAME_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::BOOL),
  /* SIGNED */
  NAME_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::INT8),
  NAME_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::INT16),
  NAME_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::INT32),
  NAME_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::INT64),
  /* CHARACTERS */
  NAME_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::SCHAR),
  NAME_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::CHAR),
  NAME_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::WCHAR),
  NAME_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::UCHAR),
  /* UNSIGNED */
  NAME_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::BYTE),   // Alias for uint
  NAME_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::UINT8),
  NAME_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::UINT16),
  NAME_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::UINT32),
  NAME_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::UINT64),
  /* FLOATING POINT */
  NAME_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::FLOAT32),
  NAME_VALUE(PRIMITIVE_TYPE, PRIMITIVE_TYPE::FLOAT64)
};
namespace Primitives
{
bool Exists(const std::string &name)
{
  return _primitives.find(GetSearchName(name)) != _primitives.end();
}
PRIMITIVE_TYPE Get(const std::string &name)
{
  return _primitives[GetSearchName(name)];
}

std::string GetName(PRIMITIVE_TYPE primitive)
{
  return _primitivesNames[primitive];
}

std::string GetName(ast::CONSTANT_TYPE constType)
{
  PRIMITIVE_TYPE result;

  switch (constType) {
  case llang::ast::CONSTANT_TYPE::I8:
    result = PRIMITIVE_TYPE::INT8;
    break;

  case llang::ast::CONSTANT_TYPE::I16:
    result = PRIMITIVE_TYPE::INT16;
    break;

  case llang::ast::CONSTANT_TYPE::I32:
    result = PRIMITIVE_TYPE::INT32;
    break;

  case llang::ast::CONSTANT_TYPE::I64:
    result = PRIMITIVE_TYPE::INT64;
    break;

  case llang::ast::CONSTANT_TYPE::FLOAT:
    result = PRIMITIVE_TYPE::FLOAT32;
    break;

  case llang::ast::CONSTANT_TYPE::DOUBLE:
    result = PRIMITIVE_TYPE::FLOAT64;
    break;

  case llang::ast::CONSTANT_TYPE::STRING:
  case llang::ast::CONSTANT_TYPE::_COUNT:
  default:
    result = PRIMITIVE_TYPE::VOID;
  }

  return _primitivesNames[result];
}
}
}
