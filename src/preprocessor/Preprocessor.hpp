#pragma once
#include "../ast/ProgramNode.hpp"
#include "../antlr/ModuleRetrieverLexer.h"
#include "../antlr/ModuleRetrieverParser.h"
#include "../Console.hpp"
#include "PreprocessorListener.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <memory>

namespace llang {
    
    struct ModuleInfo {
        std::string ModuleName;
        std::string FullProgram;
    };

    struct Preprocessor {
        int Error = 0;
        std::string ModuleFolder;

        explicit Preprocessor(const std::string& moduleFolder);

        /*
        * Process de folder given search for the requested module 
        * Returns ModuleInfo
        */
        ModuleInfo process(const std::string& moduleName);

    private:
        std::string readWholeFile(std::ifstream& file, const std::string& moduleName, const bool hasMain, const std::string& mainFunc);

    };
}