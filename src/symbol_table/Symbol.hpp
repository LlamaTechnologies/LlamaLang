#pragma once
#include <string>
#include "../Primitives.hpp"

namespace llang::symbol_table
{
    enum class SYMBOL_TYPE
    {
        PRIMITIVE,
        FUNC,
        TYPE,
        VAR,
        COMPLEX,
    };

    /**
     * Represents a Symbol in the SymbolTableScope
     **/
    struct Symbol
    {
        std::string Name;
        SYMBOL_TYPE Type;

        Symbol(std::string name, SYMBOL_TYPE type = SYMBOL_TYPE::PRIMITIVE)
        : Name(name), Type(type)
        {}

        static bool IsPrimitive(std::string name)
        {
            return Primitives::Exists(name);
        }
    };
}
