#pragma once
#include <string>
#include "Symbol.hpp"

namespace llang::symbol_table
{
    struct TypeSymbol : public Symbol
    {
        TypeSymbol(std::string name, SYMBOL_TYPE type = SYMBOL_TYPE::TYPE)
            : Symbol(name, type)
        {}
    }
}
