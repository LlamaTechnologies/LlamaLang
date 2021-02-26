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
#include "IR.hpp"

static std::string logFileName = "../tests/log_antlr4.txt";


// std::string inputFileName = "../tests/test_function_syntax_errors.llang";


static std::string inputFileName = "../tests/test_function.llang";

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

std::string get_current_dir() {
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    return current_working_dir;
}


int main(int argc, const char *argv[]) {
    using namespace llang;

    std::string execution_path = get_current_dir();
    Console::WriteLine("Running in: " + execution_path);


    // Log file
    std::fstream logFile(logFileName, std::ios::out);

    // Read arguments

    // Prepare files
    
    std::fstream sourceFile(inputFileName, std::ios::in);
    if( sourceFile.fail() ) {
        Console::WriteLine("File does not exists!");
        return 1;
    }

    auto sourceStream = antlr4::ANTLRInputStream(sourceFile);

    Console::WriteLine("======== Source File ========");
    Console::WriteLine();
    
    if( !Console::WriteLine(sourceFile) ) {
        return 1;
    }

    Console::WriteLine();

    auto lexer = LlamaLangLexer(&sourceStream);
    auto tokens = antlr4::CommonTokenStream(&lexer);

    auto parser = LlamaLangParser(&tokens);
    parser.setBuildParseTree(true);

    auto fileName = std::filesystem::path(inputFileName).filename().string();
    auto syntaxErrorListener = error_handling::SyntaxErrorListener(fileName);
    parser.addErrorListener(&syntaxErrorListener);

    auto tree = parser.sourceFile();
    auto errors = syntaxErrorListener.Errors;
    sourceFile.close();


    auto ast = AstBuilder(fileName).visitSourceFile(tree).as<std::shared_ptr<ast::ProgramNode>>();
    auto analisedAST = semantics::SemanticAnalyzer(ast, errors).check();

    // Print Errors
    if( !errors.empty() ) {
        Console::WriteLine();
        Console::WriteLine("======== Errors ========");
        Console::WriteLine("count: " + std::to_string(parser.getNumberOfSyntaxErrors()));
        Console::WriteLine();

        for( auto error : errors ) {
            Console::WriteLine(error.ToString());
        }
    }

    auto programNode = std::static_pointer_cast<ast::Node, ast::ProgramNode>( analisedAST );
    auto programNodesString = programNode->ToString();
    Console::WriteLine();
    Console::WriteLine("======== Abstract Syntax Tree ========");
    Console::WriteLine();
    Console::WriteLine(programNodesString);
    Console::WriteLine();

    // Close logFile
    logFile.close();
    
    Console::WriteLine();
    Console::WriteLine(">> Press any key to continue << ");
    Console::ReadKey();

    // If errors do not create executable
    if( !errors.empty() )
        return 1;

    // Create IR
    Console::WriteLine("======== Intermediate Representation ========");
    IR::Translate(analisedAST);

    Console::ReadKey();

    // Create executable

    // Done!
    return 0;
}
