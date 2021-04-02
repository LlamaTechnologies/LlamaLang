#pragma once
#include "../antlr/ModuleRetrieverBaseListener.h"
namespace llang {
    struct ModuleRetriever : public ModuleRetrieverBaseListener {
        bool IsValid = false;

        ModuleRetriever(const std::string& moduleName) : moduleName(moduleName) {}
        
        virtual ~ModuleRetriever() = default;

        void enterValidSource(ModuleRetrieverParser::ValidSourceContext* ctx) override;

        void enterModuleDirective(ModuleRetrieverParser::ModuleDirectiveContext* ctx) override;

    private:
        std::string moduleName;
    };
}