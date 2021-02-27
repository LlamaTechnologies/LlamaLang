#include "ConstantNode.hpp"
#include "../CppReflection.hpp"


#define PUSH_VAL(enum_type, value)\
contantTypes[size_t(value)] = GetEnumClassValueName<enum_type, value>()

namespace llang::ast
{
    static bool init = false;
    static auto contantTypes = std::vector<std::string>(size_t(CONSTANT_TYPE::_COUNT));

    std::string GetConstantTypeName(CONSTANT_TYPE type) {
        if( !init ) {
            init = true;
            PUSH_VAL(CONSTANT_TYPE, CONSTANT_TYPE::I8);
            PUSH_VAL(CONSTANT_TYPE, CONSTANT_TYPE::I16);
            PUSH_VAL(CONSTANT_TYPE, CONSTANT_TYPE::I32);
            PUSH_VAL(CONSTANT_TYPE, CONSTANT_TYPE::I64);
            PUSH_VAL(CONSTANT_TYPE, CONSTANT_TYPE::FLOAT);
            PUSH_VAL(CONSTANT_TYPE, CONSTANT_TYPE::DOUBLE);
            PUSH_VAL(CONSTANT_TYPE, CONSTANT_TYPE::STRING);
        }
        return contantTypes.at(size_t(type));
    }

    CONSTANT_TYPE GetResultType(BINARY_OPERATION op, std::shared_ptr<ConstantNode> rightNode, std::shared_ptr<ConstantNode> leftNode, bool &checkOverflow) {
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
