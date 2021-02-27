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
            PUSH_VAL(CONSTANT_TYPE, CONSTANT_TYPE::INTEGER);
            PUSH_VAL(CONSTANT_TYPE, CONSTANT_TYPE::FLOAT);
            PUSH_VAL(CONSTANT_TYPE, CONSTANT_TYPE::CHAR);
            PUSH_VAL(CONSTANT_TYPE, CONSTANT_TYPE::STRING);
        }
        return contantTypes.at(size_t(type));
    }
}
