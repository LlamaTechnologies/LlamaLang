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


#include <Windows.h>


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

  IR::Translate(analisedAST, moduleConfig.moduleName);

  Console::WriteLine("\n\n======== Linker Call =========\n");

  // Create executable
  auto canExecute = [](const std::string & dir) {
    namespace fs = std::filesystem;
    fs::path path(dir);
    return status(path).permissions() == fs::perms::owner_exec;
  };

  auto canExecutePath = [](const std::filesystem::path & dir) {
    namespace fs = std::filesystem;
    return status(dir).permissions() == fs::perms::owner_exec;
  };

  auto ScanDirForExecutable = [&] (const std::filesystem::path & dir, const std::string & fileName) {
    namespace fs = std::filesystem;
    auto directory = fs::directory_iterator(dir);

    for (auto file : directory) {
      if (canExecutePath(file.path())) {
        return true;
      }
    }

    return false;
  };

  auto pathStr = std::string(getenv("PATH"));

  auto GetProgramPath = [&](const std::string & Name) {
    namespace fs = std::filesystem;

    std::string pathVariable = getenv("PATH");
    std::vector<std::string> pathVariableDirs;
    std::string delimiter = ";";

    size_t pos = 0;
    std::string token;

    while ((pos = pathVariable.find(delimiter)) != std::string::npos) {
      token = pathVariable.substr(0, pos);
      pathVariableDirs.push_back(token);
      pathVariable.erase(0, pos + delimiter.length());
    }

    std::string llvmBinariesDir;
    bool found = false;

    for (auto &dir : pathVariableDirs) {
      auto start = dir.find("LLVM");

      if (start != std::string::npos) {
        llvmBinariesDir = dir;
        found = true;
        break;
      } else {
        start = dir.find("llvm");

        if (start != std::string::npos) {
          llvmBinariesDir = dir;
          found = true;
          break;
        }
      }
    }

    if (!found)
      return std::string("");

    fs::path PrefixDir(llvmBinariesDir);

    if (fs::is_directory(PrefixDir)) {
      if (ScanDirForExecutable(PrefixDir, Name))
        return PrefixDir.string() + Name;
    } else {
      std::string P(PrefixDir.string() + Name);

      if (canExecute(P))
        return P;
    }

    return std::string(Name);
  };

  std::string linkerInputFile = moduleConfig.moduleName + ".bc ";
  std::string linkerOutputFile = moduleConfig.moduleName + ".exe ";
  std::string args =
    linkerInputFile +
    "/out:" + linkerOutputFile +
    "/machine:x64 "
    "/defaultlib:libcmt "
    "/nologo "
    "/subsystem:console "
    "/libpath:\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.28.29910\\lib\\x64\" "
    "/libpath:\"C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.19041.0\\ucrt\\x64\" "
    "/libpath:\"C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.19041.0\\um\\x64\"";

  std::string linkerPath = GetProgramPath("lld-link");

  if (linkerPath.empty()) {
    Console::WriteLine("Could not Find install directory for lld");
    return -1;
  }

  std::string command = linkerPath + " " + args;

#ifdef _DEBUG
  Console::WriteLine(command);
#endif

  int errorCode = std::system(command.c_str());

  if (errorCode == 0)
    std::remove(linkerInputFile.c_str());

  return 0;
}

void PrintAST(std::shared_ptr<llang::ast::Node> programNode)
{
  std::string programNodesString = programNode->ToString();

  Console::WriteLine();
  Console::WriteLine("======== Abstract Syntax Tree ========");
  Console::WriteLine();
  Console::WriteLine(programNodesString);
  Console::WriteLine();
}

void PrintErrors(std::vector<llang::error_handling::Error> errors)
{
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

