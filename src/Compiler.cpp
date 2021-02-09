#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <antlr4-runtime.h>
#include "semantic_analyzer/SemanticAnalyzer.hpp"
#include "error_handling/SyntaxErrorListener.hpp"
#include "antlr/LlamaLangLexer.h"
#include "antlr/LlamaLangParser.h"

static std::string logFileName = "../../../../../Examples/log_antlr4.txt";
// std::string inputFileName =
// "../../../../Examples/test_function_syntax_errors.llang";
static std::string inputFileName = "../../../../../Examples/test_function.llang";

namespace Console {
    void WriteLine() { std::cout << std::endl; }

    void WriteLine(const std::string &msg) { 
        std::cout << msg << std::endl;
    }

    void WriteLine(const std::fstream &msg) { 
        std::cout << msg.rdbuf() << std::endl; 
    }

    void WriteLine(antlr4::ANTLRInputStream &msg) {
      std::cout << msg.toString() << std::endl;
    }

    char ReadKey() { 
        char key;
        std::cin >> key;
        return key;
    }
}

int main(int argc, const char *argv[]) {
    using namespace llang;

    // Log file
    auto logFile = std::fstream(logFileName, std::ios_base::out);

    // Read arguments

    // Prepare files
    auto sourceFile = antlr4::ANTLRInputStream(inputFileName);

    Console::WriteLine("======== Source File ========");
    Console::WriteLine();
    Console::WriteLine(sourceFile);

    auto lexer = LlamaLangLexer(&sourceFile);
    auto tokens = antlr4::CommonTokenStream(&lexer);
    
    auto parser = LlamaLangParser(&tokens);
    parser.setBuildParseTree(true);

    auto syntaxErrorListener = error_handling::SyntaxErrorListener();
    parser.addErrorListener(&syntaxErrorListener);

    auto tree = parser.sourceFile();
    auto ast = ASTBuilder(std::filesystem::path(inputFileName).filename().string()).VisitSourceFile(tree);

    auto errors = syntaxErrorListener.Errors;
    auto analisedAST = semantics::SemanticAnalyzer(ast, errors).check();

    // Print Errors
    Console::WriteLine("======== Errors ========");
    Console::WriteLine("count: " + std::to_string(parser.getNumberOfSyntaxErrors()));
    Console::WriteLine();

    for (auto error : errors) { 
        Console::WriteLine(error.ToString());
    }

    Console::WriteLine();
    Console::WriteLine("======== Abstract Syntax Tree ========");
    Console::WriteLine();
    Console::WriteLine(analisedAST->ToString());

    // Close logFile
    logFile.close();
    Console::ReadKey();

    // If errors do not create executable
    if (errors.size() > 0)
        return;

    // Create IR
    Console::WriteLine("======== Intermediate Representation ========");
    IR.Translate(analisedAST);

    Console::ReadKey();

    // Create executable

    // Done!
    return 0;
}
