#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Symbol.hpp"


namespace llang::symbol_table
{
    enum class SCOPE_TYPE
    {
        GLOBAL,     // program gloabl scope
        FUNC,       // function scope
        METHOD,     // same as func but has access to members
        NAMESPACE   // namespace scope
    };

    /**
     * Represents a scope in the program code
     * It could be:
     *    - global
     *    - function
     *    - method
     *    - namespace.
     **/
    struct SymbolTableScope
    {
        SCOPE_TYPE ScopeType;

        std::shared_ptr<SymbolTableScope> Parent;

        std::vector<std::shared_ptr<Symbol>> Symbols;

        std::vector<std::shared_ptr<SymbolTableScope>> children;

        SymbolTableScope(SCOPE_TYPE scopeType)
        : ScopeType(scopeType) {}

        void printScopeTree(int indentLevel = 0)
        {
            /*
            string identation = new String('\t', indentLevel);
            Console.WriteLine(identation + ScopeType.ToString() + " {");

            ForEachSymbol((symbol) =>
            {
                Console.WriteLine(identation + "\t" + symbol.Name + ": " + symbol.Type);
            });

            ForEachChild((child) =>
            {
                child.printScopeTree(indentLevel + 1);
            });
            Console.WriteLine(identation + "}");
            */
        }
    };
}
