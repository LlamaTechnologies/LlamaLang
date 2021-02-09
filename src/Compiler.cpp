#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <antlr4-runtime.h>
#include "Console.hpp"
#include "antlr/LlamaLangLexer.h"
#include "antlr/LlamaLangParser.h"
#include "error_handling/SyntaxErrorListener.hpp"
#include "semantic_analyzer/SemanticAnalyzer.hpp"
#include "AstBuilder.hpp"

static std::string logFileName = "../../../../../Examples/log_antlr4.txt";
// std::string inputFileName =
// "../../../../Examples/test_function_syntax_errors.llang";
static std::string inputFileName = "../../../../../Examples/test_function.llang";



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
    auto ast = AstBuilder(std::filesystem::path(inputFileName).filename().string()).visitSourceFile(tree);

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
