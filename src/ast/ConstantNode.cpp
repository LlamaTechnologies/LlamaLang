#include "ConstantNode.hpp"
#include "../CppReflection.hpp"
#include <unordered_map>

#define GET_NAME(enum_type, value)\
GetEnumClassValueName<enum_type, value>()

#define SET_VAL(enum_type, value)\
{GetEnumClassValueName<enum_type, value>(), value}

namespace llang::ast
{
static bool initNames = false;
static bool init = false;
static std::vector<std::string> contantTypesNames = {
  GET_NAME(CONSTANT_TYPE, CONSTANT_TYPE::I8),
  GET_NAME(CONSTANT_TYPE, CONSTANT_TYPE::I16),
  GET_NAME(CONSTANT_TYPE, CONSTANT_TYPE::I32),
  GET_NAME(CONSTANT_TYPE, CONSTANT_TYPE::I64),
  GET_NAME(CONSTANT_TYPE, CONSTANT_TYPE::FLOAT),
  GET_NAME(CONSTANT_TYPE, CONSTANT_TYPE::DOUBLE),
  GET_NAME(CONSTANT_TYPE, CONSTANT_TYPE::STRING)
};
static std::unordered_map<std::string, CONSTANT_TYPE> contantTypes = {
  SET_VAL(CONSTANT_TYPE, CONSTANT_TYPE::I8),
  SET_VAL(CONSTANT_TYPE, CONSTANT_TYPE::I16),
  SET_VAL(CONSTANT_TYPE, CONSTANT_TYPE::I32),
  SET_VAL(CONSTANT_TYPE, CONSTANT_TYPE::I64),
  SET_VAL(CONSTANT_TYPE, CONSTANT_TYPE::FLOAT),
  SET_VAL(CONSTANT_TYPE, CONSTANT_TYPE::DOUBLE),
  SET_VAL(CONSTANT_TYPE, CONSTANT_TYPE::STRING)
};

std::string GetConstantTypeName(CONSTANT_TYPE type)
{
  return contantTypesNames.at(size_t(type));
}

CONSTANT_TYPE GetConstantType(std::string name)
{
  return contantTypes.at(name);
}

CONSTANT_TYPE GetConstantType(PRIMITIVE_TYPE primitive)
{
  switch( primitive ) {
  case llang::PRIMITIVE_TYPE::VOID:
    return CONSTANT_TYPE::_COUNT;

  case llang::PRIMITIVE_TYPE::BOOL:
  case llang::PRIMITIVE_TYPE::SCHAR:
  case llang::PRIMITIVE_TYPE::CHAR:
  case llang::PRIMITIVE_TYPE::BYTE:
  case llang::PRIMITIVE_TYPE::UINT8:
  case llang::PRIMITIVE_TYPE::INT8:
    return CONSTANT_TYPE::I8;

  case llang::PRIMITIVE_TYPE::WCHAR:
  case llang::PRIMITIVE_TYPE::UINT16:
  case llang::PRIMITIVE_TYPE::INT16:
    return CONSTANT_TYPE::I16;

  case llang::PRIMITIVE_TYPE::UCHAR:
  case llang::PRIMITIVE_TYPE::INT32:
  case llang::PRIMITIVE_TYPE::UINT32:
    return CONSTANT_TYPE::I32;

  case llang::PRIMITIVE_TYPE::INT64:
  case llang::PRIMITIVE_TYPE::UINT64:
    return CONSTANT_TYPE::I64;

  case llang::PRIMITIVE_TYPE::FLOAT32:
    return CONSTANT_TYPE::FLOAT;

  case llang::PRIMITIVE_TYPE::FLOAT64:
  default:
    return CONSTANT_TYPE::DOUBLE;
  }
}

CONSTANT_TYPE GetResultType(BINARY_OPERATION op, std::shared_ptr<ConstantNode> rightNode, std::shared_ptr<ConstantNode> leftNode, bool &checkOverflow)
{
  checkOverflow = rightNode->ConstType == leftNode->ConstType && (op == BINARY_OPERATION::ADD || op == BINARY_OPERATION::MUL );

  auto returnType = CONSTANT_TYPE::I8;

  if( rightNode->ConstType == returnType && leftNode->ConstType == returnType ) {
    return returnType;
  }

  returnType = CONSTANT_TYPE::I16;

  if( rightNode->ConstType <= returnType && leftNode->ConstType <= returnType ) {
    return returnType;
  }

  returnType = CONSTANT_TYPE::I32;

  if( rightNode->ConstType <= returnType && leftNode->ConstType <= returnType ) {
    return returnType;
  }

  returnType = CONSTANT_TYPE::I64;

  if( rightNode->ConstType <= returnType && leftNode->ConstType <= returnType ) {
    return returnType;
  }

  returnType = CONSTANT_TYPE::FLOAT;

  if( rightNode->ConstType == returnType && leftNode->ConstType == returnType ) {
    return returnType;
  }

  returnType = CONSTANT_TYPE::DOUBLE;

  if( (rightNode->ConstType == returnType || rightNode->ConstType == CONSTANT_TYPE::FLOAT) && ( leftNode->ConstType == returnType || leftNode->ConstType == CONSTANT_TYPE::FLOAT ) ) {
    return returnType;
  }

  return CONSTANT_TYPE::DOUBLE;
}
}
