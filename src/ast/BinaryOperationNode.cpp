#include "BinaryOperationNode.hpp"

using namespace llang;
using namespace ast;

void BinaryOperationNode::ToString(std::string &str, const int tabLevel) const
{
  auto tabs = GetTabs(tabLevel);
  str += tabs;
  str += GetBinaryOperationName(this->Op) + "(";
  this->Left->ToString(str, 0);
  str += ", ";
  this->Right->ToString(str, 0);
  str += ")";
}

#define GET_VAL(enum_type, value)\
GetEnumClassValueName<enum_type, value>()

static bool init = false;
static std::vector<std::string> binOpName = {
  GET_VAL(BINARY_OPERATION, BINARY_OPERATION::ADD),
  GET_VAL(BINARY_OPERATION, BINARY_OPERATION::SUB),
  GET_VAL(BINARY_OPERATION, BINARY_OPERATION::MUL),
  GET_VAL(BINARY_OPERATION, BINARY_OPERATION::DIV),
  GET_VAL(BINARY_OPERATION, BINARY_OPERATION::MOD),
  GET_VAL(BINARY_OPERATION, BINARY_OPERATION::BIT_AND),
  GET_VAL(BINARY_OPERATION, BINARY_OPERATION::BIT_OR),
};

std::string ast::GetBinaryOperationName(BINARY_OPERATION op)
{
  return binOpName.at(size_t(op));
}
