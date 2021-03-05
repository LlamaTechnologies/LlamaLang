#include "SymbolTableScope.hpp"

using namespace llang;
using namespace symbol_table;

void SymbolTableScope::addSymbol(const std::string &name, std::shared_ptr<ast::Node> data) {
    if( Symbols.find(name) == Symbols.end() ) {
        SymbolsVector vec;
        vec.push_back(data);
        Symbols.insert({ name, vec});
    }
    else
        Symbols.at(name).push_back(data);
}

SymbolTableScope::SymbolsVector SymbolTableScope::findSymbol(const std::string &name, bool searchParentScopes) {
    if( Symbols.find(name) != Symbols.end() ) {
        return Symbols.at(name);
    }

    auto current = Parent;
    while( current ) {
        if( current->Symbols.find(name) != current->Symbols.end() )
            return current->Symbols.at(name);
        
        if( searchParentScopes )
            current = current->Parent;
        else
            current = nullptr;
    }
    return SymbolTableScope::SymbolsVector();
}


SymbolTableScope::ScopesVector SymbolTableScope::addChild(SCOPE_TYPE childType, const std::string &name, std::shared_ptr<ast::Node> data) {
    addSymbol(name, data);
    
    auto child = std::make_shared<SymbolTableScope>(childType);
    child->Data = data;

    if( children.find(name) == children.end() ) {
        ScopesVector vec;
        vec.push_back(child);
        children.insert({ name, vec });
        return children.at(name);
    } else {
        auto vec = children.at(name);
        vec.push_back(child);
        return vec;
    }
}
