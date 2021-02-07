#pragma once
#include <string>
#include <vector>
#include <memory>
#include "TypeSymbol.hpp"
#include "VarSymbol.hpp"
#include "FunctionSymbol.hpp"

namespace llang::symbol_table
{
    /**
     * Represnts a complex type like Prot or Union in the SymbolTableScope
     **/
    struct ComplexTypeSymbol : public TypeSymbol
    {
        using MemberList = std::vector<std::shared_ptr<VarSymbol>>;
        using MethodList =std::vector<std::shared_ptr<FunctionSymbol>>;
        MemberList Members;
        MethodList Methods;

        ComplexTypeSymbol(std::string name, MemberList &members, MethodList &methods)
            : TypeSymbol(name, SYMBOL_TYPE::COMPLEX), Members(members), Methods(methods)
        {}
    };
}
