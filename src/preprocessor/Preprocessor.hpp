#pragma once
#include "../ast/ProgramNode.hpp"
#include "../Console.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <memory>

namespace llang {
    
    struct ModuleInfo {
        const std::string ModuleName;
        std::vector<std::ifstream> Files;
        std::shared_ptr<ast::ProgramNode> ImportedLibrariesTree;
    };

    struct ModuleRetriever {
        int Error = 0;
        std::string ModuleFolder;

        ModuleRetriever(const std::string& moduleFolder)
            : ModuleFolder(moduleFolder) {
            namespace fs = std::filesystem;


            auto path = fs::path(moduleFolder);
            if (!fs::exists(path)) {
                Console::WriteLine("Directory not found: " + ModuleFolder);
                Error = -1;
                return;
            }

            auto directory = fs::directory_entry(path);
            for (auto entry : directory)

        }

        /*
        * Process de folder given search for the requested module 
        * Returns ModuleInfo
        */
        ModuleInfo process(const std::string &moduleName);

    private:


    };
}