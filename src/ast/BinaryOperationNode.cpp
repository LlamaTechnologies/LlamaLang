#include "BinaryOperationNode.hpp"

using namespace llang;

void ast::BinaryOperationNode::ToString(std::string& str, const int tabLevel) const
{
    auto tabs = GetTabs(tabLevel);
    str += tabs;
    str += GetBinaryOperationName(this->Op) + "(";
    this->Left->ToString(str, 0);
    str += ", ";
    this->Right->ToString(str, 0);
    str += ")";
}

#define PUSH_VAL(enum_type, value)\
binOpName[size_t(value)] = GetEnumClassValueName<enum_type, value>()

static bool init = false;
static auto binOpName = std::vector<std::string>(size_t(ast::BINARY_OPERATION::_COUNT));

std::string llang::ast::GetBinaryOperationName(BINARY_OPERATION op)
{
    if (!init) {
        init = true;
        PUSH_VAL(BINARY_OPERATION, BINARY_OPERATION::ADD);
        PUSH_VAL(BINARY_OPERATION, BINARY_OPERATION::BIT_AND);
        PUSH_VAL(BINARY_OPERATION, BINARY_OPERATION::BIT_OR);
        PUSH_VAL(BINARY_OPERATION, BINARY_OPERATION::DIV);
        PUSH_VAL(BINARY_OPERATION, BINARY_OPERATION::MOD);
        PUSH_VAL(BINARY_OPERATION, BINARY_OPERATION::MUL);
        PUSH_VAL(BINARY_OPERATION, BINARY_OPERATION::SUB);
    }
    return binOpName.at(size_t(op));
}
