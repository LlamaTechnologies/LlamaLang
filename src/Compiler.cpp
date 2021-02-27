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

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

static std::string get_current_dir();
static void PrintAST(std::shared_ptr<llang::ast::Node> programNode);
static void PrintErrors(std::vector<llang::error_handling::Error> errors);
static void AwaitUserInput();

static std::string logFileName = "../tests/log_antlr4.txt";
static std::string ouputFileName = "../tests/output_bitcode.ll";
static std::string inputFileName = "../tests/test.llang";
// static std::string inputFileName = "../tests/test_semantic_errors.llang";

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

#ifdef _DEBUG
    Console::WriteLine("======== Source File ========");
    Console::WriteLine();

    if( !Console::WriteLine(sourceFile) ) {
        return 1;
    }

    Console::WriteLine();
#endif

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

    PrintErrors(errors);
    PrintAST(analisedAST);

    // Close logFile
    logFile.close();

    AwaitUserInput();
    
    // If errors do not create executable
    if( !errors.empty() )
        return 1;

    // Create IR
    Console::WriteLine("======== Intermediate Representation ========");
    IR::Translate(analisedAST, ouputFileName);

    AwaitUserInput();

    // Create executable

    // Done!
    return 0;
}

void AwaitUserInput() {
    Console::WriteLine();
    Console::WriteLine(">> Press any key to continue << ");
    Console::ReadKey();
}

void PrintAST(std::shared_ptr<llang::ast::Node> programNode) {
    std::string programNodesString = programNode->ToString();

    Console::WriteLine();
    Console::WriteLine("======== Abstract Syntax Tree ========");
    Console::WriteLine();
    Console::WriteLine(programNodesString);
    Console::WriteLine();
}

void PrintErrors(std::vector<llang::error_handling::Error> errors) {
    // Print Errors
    if( !errors.empty() ) {
        Console::WriteLine();
        Console::WriteLine("======== Errors ========");
        Console::WriteLine("count: " + std::to_string(errors.size()));
        Console::WriteLine();

        for( auto error : errors ) {
            Console::WriteLine(error.ToString());
        }
    }
}

std::string get_current_dir() {
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    return current_working_dir;
}
