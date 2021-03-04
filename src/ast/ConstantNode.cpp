#include "ConstantNode.hpp"
#include "../CppReflection.hpp"
#include <unordered_map>

#define PUSH_NAME(enum_type, value)\
contantTypesNames[size_t(value)] = GetEnumClassValueName<enum_type, value>()

#define PUSH_VAL(enum_type, value)\
contantTypes.insert({GetEnumClassValueName<enum_type, value>(), value})

namespace llang::ast
{
    static bool initNames = false;
    static bool init = false;
    static auto contantTypesNames = std::vector<std::string>(size_t(CONSTANT_TYPE::_COUNT));
    static auto contantTypes = std::unordered_map<std::string, CONSTANT_TYPE>(size_t(CONSTANT_TYPE::_COUNT));

    static void FillConstantsTypes() {
        PUSH_VAL(CONSTANT_TYPE, CONSTANT_TYPE::I8);
        PUSH_VAL(CONSTANT_TYPE, CONSTANT_TYPE::I16);
        PUSH_VAL(CONSTANT_TYPE, CONSTANT_TYPE::I32);
        PUSH_VAL(CONSTANT_TYPE, CONSTANT_TYPE::I64);
        PUSH_VAL(CONSTANT_TYPE, CONSTANT_TYPE::FLOAT);
        PUSH_VAL(CONSTANT_TYPE, CONSTANT_TYPE::DOUBLE);
        PUSH_VAL(CONSTANT_TYPE, CONSTANT_TYPE::STRING);
    }

    std::string GetConstantTypeName(CONSTANT_TYPE type) {
        if( !initNames ) {
            initNames = true;
            PUSH_NAME(CONSTANT_TYPE, CONSTANT_TYPE::I8);
            PUSH_NAME(CONSTANT_TYPE, CONSTANT_TYPE::I16);
            PUSH_NAME(CONSTANT_TYPE, CONSTANT_TYPE::I32);
            PUSH_NAME(CONSTANT_TYPE, CONSTANT_TYPE::I64);
            PUSH_NAME(CONSTANT_TYPE, CONSTANT_TYPE::FLOAT);
            PUSH_NAME(CONSTANT_TYPE, CONSTANT_TYPE::DOUBLE);
            PUSH_NAME(CONSTANT_TYPE, CONSTANT_TYPE::STRING);
        }
        return contantTypesNames.at(size_t(type));
    }

    CONSTANT_TYPE GetConstantType(std::string name) {
        if( !init ) {
            init = true;
            FillConstantsTypes();
        }
        return contantTypes.at(name);
    }

    CONSTANT_TYPE GetConstantType(PRIMITIVE_TYPE primitive) {
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
