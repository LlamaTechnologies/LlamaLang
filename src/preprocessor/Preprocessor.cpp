#include "Preprocessor.hpp"

using namespace llang;

Preprocessor::Preprocessor(const std::string& moduleFolder)
    : ModuleFolder(moduleFolder) {
    namespace fs = std::filesystem;

    auto path = fs::canonical(moduleFolder);
    if (!fs::exists(path)) {
        Console::WriteLine("Directory not found: " + ModuleFolder);
        Error = -1;
        return;
    }

    if (!fs::is_directory(path)) {
        Console::WriteLine("Path is not a directory: " + ModuleFolder);
        Error = -1;
        return;
    }
}

ModuleInfo Preprocessor::process(const std::string& moduleName) {
    namespace fs = std::filesystem;

    ModuleInfo moduleInfo;
    moduleInfo.ModuleName = moduleName;

    if (Error)
        return moduleInfo;

    bool hadMain = false;
    std::string fileMainName = "";
    auto directory = fs::recursive_directory_iterator(ModuleFolder);

    for (auto entry : directory) {
        auto filePath = entry.path();
        auto ext = filePath.extension().string();
        auto fileName = filePath.filename().string();

        if (entry.is_regular_file() && ext == ".llang") {
            auto file = std::ifstream(entry);
            auto inputStream = antlr4::ANTLRInputStream(file);
            auto lexer = ModuleRetrieverLexer(&inputStream);
            auto tokens = antlr4::CommonTokenStream(&lexer);
            auto parser = ModuleRetrieverParser(&tokens);
            auto listener = ModuleRetriever(moduleName);
            auto tree = parser.validSource();
            antlr4::tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);
            
            if (listener.BelongToModule) {
                if (listener.HasValidMain == false) {
                    Console::WriteLine(listener.ErrorDescr + " in file: " + fileName);
                    file.close();
                    Error = -2;
                    break;
                }

                if (hadMain && listener.HasMain) {
                    Console::WriteLine("Module redefined main in files: " + fileName + ", " + fileMainName);
                    file.close();
                    Error = -2;
                    break;
                }

                if (listener.HasMain)
                    fileMainName = fileName;

                moduleInfo.FullProgram += readWholeFile(file, moduleName, listener.HasMain, listener.MainFunc);
            }
            file.close();
        }
    }

    Error = Error || moduleInfo.FullProgram.empty();

    return moduleInfo;
}

std::string Preprocessor::readWholeFile(std::ifstream& file, const std::string &moduleName, const bool hasMain, const std::string &mainFunc) {
    std::string str;

    file.seekg(0, std::ios::end);
    str.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    // erase module directive
    auto start = str.find_first_of("#");
    auto erase = "#module " + moduleName;
    str = str.erase(start, erase.size());

    
    if (hasMain) {
        // search main directive
        erase = "#main";
        auto found = false;

        
        start = 0;
        while (!found) {
            start = str.find_first_of("#", start);
            auto subStr = str.substr(start, erase.size());

            if (subStr == "#main") {
                // erase main directive
                str = str.erase(start, erase.size());
                found = true;
            }
            else {
                start += subStr.size();
            }
        }
        // add main function
        str += "\n" + mainFunc;
    }

    return str;
}
