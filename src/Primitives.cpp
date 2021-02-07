#include "Primitives.hpp"
#include "CppReflection.hpp"

#define ENUM_VALUE(enum_type, enum_val)\
{GetEnumClassValueName<enum_type, enum_value>(), enum_val}


namespace llang
{
    using PrimitivesMap = std::unordered_map<const std::string, PRIMITIVE_TYPE>;
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

    namespace Primitives
    {
        bool Exists(std::string &name) {
            return _primitives.find(name) != _primitives.end();
        }

        PRIMITIVE_TYPE Get(std::string &name);
        {
            return _primitives[name];
        }
    }
}
