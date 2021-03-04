#include "SymbolTableScope.hpp"

using namespace llang;
using namespace symbol_table;

void SymbolTableScope::addSymbol(const std::string &name, std::shared_ptr<ast::Node> data) {
    Symbols.insert({ name, data });
}

std::shared_ptr<ast::Node> SymbolTableScope::findSymbol(const std::string &name) {
    if( Symbols.find(name) != Symbols.end() ) {
        return Symbols.at(name);
    }

    auto current = Parent;
    while( current ) {
        if( current->Symbols.find(name) != current->Symbols.end() )
            return current->Symbols.at(name);
        current = current->Parent;
    }
    return nullptr;
}


std::shared_ptr<SymbolTableScope> SymbolTableScope::addChild(SCOPE_TYPE childType, const std::string &name, std::shared_ptr<ast::Node> data) {
    addSymbol(name, data);

    Data = data;
    auto child = std::make_shared<SymbolTableScope>(childType);
    children.insert({ name, child });

    return child;
}
