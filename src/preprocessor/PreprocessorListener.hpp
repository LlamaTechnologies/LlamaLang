#pragma once
#include "../antlr/ModuleRetrieverBaseListener.h"

namespace llang {
    struct ModuleRetriever : public ModuleRetrieverBaseListener {
        bool BelongToModule = false;
        bool HasMain = false;
        bool HasValidMain = true;
        std::string ErrorDescr = "";
        std::string MainFunc = "";

        ModuleRetriever(const std::string& moduleName) : moduleName(moduleName) {}

        void enterValidSource(ModuleRetrieverParser::ValidSourceContext* ctx) override;

        void enterModuleDirective(ModuleRetrieverParser::ModuleDirectiveContext* ctx) override;

        void enterMainDirective(ModuleRetrieverParser::MainDirectiveContext* ctx) override;

    private:
        std::string moduleName;
        const char* ArgsMain = "func main(argc char, argv []string) int32 {\n    ret %s(argc, argv)\n}";
        const char* NoArgsMain = "func main() int32 {\n    ret %s()\n}";
    };
}