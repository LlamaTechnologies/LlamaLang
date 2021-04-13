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
  {"VOID", PRIMITIVE_TYPE::VOID},
  {"BOOL", PRIMITIVE_TYPE::BOOL},
  /* SIGNED */
  {"INT8", PRIMITIVE_TYPE::INT8},
  {"INT16", PRIMITIVE_TYPE::INT16},
  {"INT32", PRIMITIVE_TYPE::INT32},
  {"INT64", PRIMITIVE_TYPE::INT64},
  /* CHARACTERS */
  {"SCHAR", PRIMITIVE_TYPE::SCHAR},
  {"CHAR", PRIMITIVE_TYPE::CHAR},
  {"WCHAR", PRIMITIVE_TYPE::WCHAR},
  {"UCHAR", PRIMITIVE_TYPE::UCHAR},
  /* UNSIGNED */
  {"BYTE",   PRIMITIVE_TYPE::BYTE},   // Alias for uint
  {"UINT8",  PRIMITIVE_TYPE::UINT8},
  {"UINT16", PRIMITIVE_TYPE::UINT16},
  {"UINT32", PRIMITIVE_TYPE::UINT32},
  {"UINT64", PRIMITIVE_TYPE::UINT64},
  /* FLOATING POINT */
  {"FLOAT32", PRIMITIVE_TYPE::FLOAT32},
  {"FLOAT64", PRIMITIVE_TYPE::FLOAT64}
};
static PrimitivesNameMap _primitivesNames = {
    {PRIMITIVE_TYPE::VOID   , "VOID"   },
    {PRIMITIVE_TYPE::BOOL   , "BOOL"   },
    {PRIMITIVE_TYPE::INT8   , "INT8"   },
    {PRIMITIVE_TYPE::INT16  , "INT16"  },
    {PRIMITIVE_TYPE::INT32  , "INT32"  },
    {PRIMITIVE_TYPE::INT64  , "INT64"  },
    {PRIMITIVE_TYPE::UINT8  , "UINT8"  },
    {PRIMITIVE_TYPE::UINT16 , "UINT16" },
    {PRIMITIVE_TYPE::UINT32 , "UINT32" },
    {PRIMITIVE_TYPE::UINT64 , "UINT64" },
    {PRIMITIVE_TYPE::FLOAT32, "FLOAT32"},
    {PRIMITIVE_TYPE::FLOAT64, "FLOAT64"}
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
