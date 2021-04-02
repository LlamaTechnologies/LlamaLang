#include "DirectiveNodes.hpp"
#include <unordered_map>

using namespace llang;
using namespace ast;


static std::unordered_map<DIRECTIVE_TYPE, std::string> directiveTypeNames = {
    {DIRECTIVE_TYPE::MODULE, "MODULE"},
    {DIRECTIVE_TYPE::INCLUDE, "INCLUDE"},
    {DIRECTIVE_TYPE::EXTERNAL, "EXTERNAL"},
    {DIRECTIVE_TYPE::MAIN, "MAIN"}
};

std::string GetDirectiveTypeName(DIRECTIVE_TYPE type) {
    return directiveTypeNames[type];
}
