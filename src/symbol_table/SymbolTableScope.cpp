#include "SymbolTableScope.hpp"

using namespace llang;
using namespace symbol_table;

void SymbolTableScope::addSymbol(const std::string &name, Symbol data) {
    if( Symbols.find(name) == Symbols.end() ) {
        Symbols.insert({ name, data});
        SymbolsCounter.insert({ name, 1 });
    }
    else {
        SymbolsCounter.at(name)++;
    }
}

size_t SymbolTableScope::getSymbolCount(const std::string& name, bool searchParentScopes) {
    if (SymbolsCounter.find(name) != SymbolsCounter.end()) {
        return SymbolsCounter.at(name);
    }

    auto current = Parent;
    while (current) {
        if (current->SymbolsCounter.find(name) != current->SymbolsCounter.end()) {
            return SymbolsCounter.at(name);
        }

        if (searchParentScopes)
            current = current->Parent;
        else
            current = nullptr;
    }
    return 0;
}

SymbolTableScope::Symbol SymbolTableScope::findSymbol(const std::string &name, bool searchParentScopes) {
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
    return nullptr;
}


SymbolTableScope::ScopesVector SymbolTableScope::addChild(SCOPE_TYPE childType, const std::string &name, Symbol data) {
    addSymbol(name, data);
    
    auto child = std::make_shared<SymbolTableScope>(childType);
    child->Data = data;
    child->Parent = std::shared_ptr<SymbolTableScope>(this);

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
