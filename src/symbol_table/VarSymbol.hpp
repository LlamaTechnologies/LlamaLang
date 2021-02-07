#pragma once
#include <string>
#include <memory>
#include "TypeSymbol.hpp"

namespace llang::symbol_table
{
    /**
     * Represents a variable declaration symbol in the SymbolTableScope
     **/
    struct VarSymbol : public Symbol
    {
        std::shared_ptr<TypeSymbol> VarType;

        VarSymbol(std::string name, std::shared_ptr<TypeSymbol> varType)
            : Symbol(name, SYMBOL_TYPE::VAR), VarType(varType)
        {}
    };
}