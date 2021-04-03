#include "PreprocessorListener.hpp"
using namespace llang;

void ModuleRetriever::enterValidSource(ModuleRetrieverParser::ValidSourceContext* ctx) {
    BelongToModule = ctx->moduleDirective() != nullptr;
}

void ModuleRetriever::enterModuleDirective(ModuleRetrieverParser::ModuleDirectiveContext* ctx) {
    auto id = ctx->IDENTIFIER();
    BelongToModule = id ? id->getText() == moduleName : false;
}

void ModuleRetriever::enterMainDirective(ModuleRetrieverParser::MainDirectiveContext* ctx) {
    const size_t bufferSize = 1024;
    static char mainBuffer[bufferSize];

    if (ctx->IDENTIFIER()) {    
        auto funcName = ctx->IDENTIFIER()->getText();
        if (funcName != "main") {
            auto paramList = ctx->parameterList();
            if (paramList) {
                auto paramCount = paramList->parameterDecl().size();
                if (paramCount == 2) {
                    std::snprintf(mainBuffer, bufferSize, ArgsMain, funcName.c_str());
                    MainFunc = std::string(mainBuffer);
                }
                else {
                    ErrorDescr = "Illegal parameter count";
                    HasValidMain = false;
                    return;
                }
            }
            else {
                std::snprintf(mainBuffer, bufferSize, NoArgsMain, funcName.c_str());
                MainFunc = std::string(mainBuffer);
            }
        }
    }

    if (HasMain) {
        ErrorDescr = "Multiple defined main functions";
        HasValidMain = false;
        return;
    }

    HasMain = true;
}


