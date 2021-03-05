#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "../ast/Node.hpp"

namespace llang::symbol_table
{
    struct Symbol;

    enum class SCOPE_TYPE {
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
    struct SymbolTableScope {
        using SymbolsVector = std::vector<std::shared_ptr<ast::Node>>;
        using ScopesVector = std::vector<std::shared_ptr<SymbolTableScope>>;
        
        SCOPE_TYPE ScopeType;
        std::shared_ptr<ast::Node> Data;
        std::shared_ptr<SymbolTableScope> Parent;
        std::unordered_map<std::string, SymbolsVector> Symbols;
        std::unordered_map<std::string, ScopesVector> children;
        
        SymbolTableScope(SCOPE_TYPE scopeType)
            : ScopeType(scopeType) {}


        void addSymbol(const std::string &name, std::shared_ptr<ast::Node> data);
        SymbolsVector findSymbol(const std::string& name, bool searchParentScopes);
        ScopesVector addChild(SCOPE_TYPE childType, const std::string& name, std::shared_ptr<ast::Node> data);

        void printScopeTree(int indentLevel = 0) {
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
