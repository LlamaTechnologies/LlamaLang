#pragma once
#include <string>
#include <vector>
#include "Symbol.hpp"

namespace llang::symbol_table 
{
    /**
     * Represents a function in the SymbolTableScope
     */
    struct FunctionSymbol : public Symbol
    {
        unsigned int ParamCount;
        unsigned int RequiredParamCount;
        std::vector<std::string> ParamList;
        std::vector<std::string> RequiredParamList;

        FunctionSymbol(std::string name, std::vector<std::string> &paramList, std::vector<std::string> &requiredParamList)
            : Symbol(name, SYMBOL_TYPE::FUNC), ParamList(paramList), RequiredParamList(requiredParamList)
        {
            ParamCount = paramList.size();
            RequiredParamCount = requiredParamList.size() > 0 
                ? requiredParamList.size() : ParamCount;          
        }
    };
}
