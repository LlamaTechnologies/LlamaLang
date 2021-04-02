#include "PreprocessorListener.hpp"

using namespace llang;

void ModuleRetriever::enterValidSource(ModuleRetrieverParser::ValidSourceContext* ctx)
{
    IsValid = ctx->moduleDirective() != nullptr;
}

void ModuleRetriever::enterModuleDirective(ModuleRetrieverParser::ModuleDirectiveContext* ctx)
{
    auto id = ctx->IDENTIFIER();
    IsValid = id ? id->getText() == moduleName : false;
}


