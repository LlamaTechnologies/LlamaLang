#include "Compiler.hpp"
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
#include "ast/AstBuilder.hpp"
#include "preprocessor/Preprocessor.hpp"
#include "IR.hpp"

using namespace llang;

static std::string fileName = ">UNKNOWN FILE>";
static void PrintErrors(std::vector<llang::error_handling::Error> errors);
static void PrintAST(std::shared_ptr<llang::ast::Node> programNode);
static int compileModule(ModuleConfig &moduleConfig);

int Compiler::compile(SolutionConfig &solutionConfig)
{
    //TODO. implement this function
    return 0;
}

int Compiler::compile(ModuleConfig &moduleConfig)
{
    return compileModule(moduleConfig);
}

int compileModule(ModuleConfig &moduleConfig)
{
    // Prepare files
    Preprocessor preprocessor(moduleConfig.srcFolder);
    auto moduleInfo = preprocessor.process(moduleConfig.moduleName);
    if (preprocessor.Error) {
        Console::WriteLine("Preprocessor error");
        return 1;
    }

    auto sourceStream = antlr4::ANTLRInputStream(moduleInfo.FullProgram);

#ifdef _DEBUG
    Console::WriteLine("======== Source File ========");
    Console::WriteLine();

    Console::WriteLine(moduleInfo.FullProgram);
    Console::WriteLine();
#endif

    auto lexer = LlamaLangLexer(&sourceStream);
    auto tokens = antlr4::CommonTokenStream(&lexer);

    auto parser = LlamaLangParser(&tokens);
    parser.setBuildParseTree(true);

    auto syntaxErrorListener = error_handling::SyntaxErrorListener(fileName);
    parser.addErrorListener(&syntaxErrorListener);

    auto tree = parser.sourceFile();
    auto errors = syntaxErrorListener.Errors;

    auto astBuilder = ast::AstBuilder(fileName, moduleInfo.ModuleName);
    auto ast = astBuilder.visitSourceFile(tree).as<std::shared_ptr<ast::ProgramNode>>();
    auto analisedAST = semantics::SemanticAnalyzer(ast, astBuilder.globalScope, errors).check();

    PrintErrors(errors);
    PrintAST(analisedAST);

    // If errors do not create executable
    if (!errors.empty())
        return 1;

    // Create IR
    Console::WriteLine("======== Intermediate Representation ========");
    IR::Translate(analisedAST, moduleConfig.moduleName + ".ll");

    // Create executable

    // Done!
    return 0;
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
    if (!errors.empty()) {
        Console::WriteLine();
        Console::WriteLine("======== Errors ========");
        Console::WriteLine("count: " + std::to_string(errors.size()));
        Console::WriteLine();

        for (auto error : errors) {
            Console::WriteLine(error.ToString());
        }
    }
}

