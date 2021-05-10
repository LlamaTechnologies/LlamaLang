#include <string>
#include <fstream>
#include <filesystem>
#include "config.hpp"
#include "console.hpp"
#include "tokenize.hpp"

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#define MOD_KEY_SRC_FOLDER "src_folder"
#define MOD_KEY_MODULE_NAME "module_name"
#define MOD_KEY_EXPORT_MOD_INFO "export_mod_info"
#define MOD_KEY_TYPE "type"

static std::string get_current_dir();
static void parseModuleFile(std::ifstream &moduleFile, ModuleConfig &moduleConfig);
static std::string get_first_file_name(const ModuleConfig& module_config, int &error);

int main(int argc, const char *argv[])
{
  namespace fs = std::filesystem;

  fs::path currentDirPath(get_current_dir());
  std::fstream logFile(currentDirPath.string() + "logs/llama.log", std::ios::out);

  // read config file
  fs::directory_iterator currentDir(currentDirPath);

  bool compileModule = true;

  for (auto entry : currentDir) {
    if (entry.is_regular_file()) {
      auto fileName = entry.path().filename();

      if (fileName == "llang.config") {
        compileModule = false;
        break;
      }
    }
  }


  if (compileModule) {
    ModuleConfig moduleConfig = {
      currentDirPath.string(),
      currentDirPath.filename().string(),
      MODULE_TYPE::EXE,
      false
    };

    std::ifstream moduleFile(currentDirPath.string() + "/llang.module");

    if (moduleFile.is_open()) {
      parseModuleFile(moduleFile, moduleConfig);
    }

    std::vector<Error> errors;

    int error;
    std::string file_name = get_first_file_name(moduleConfig, error);
    
    if (error) {
        return error;
    }

    Lexer lexer(file_name, errors);
    lexer.tokenize();
    auto print_lines = print_tokens(lexer);
    for (auto line: print_lines)
        console::WriteLine(line); 

    return 0;
  }

  // solution not supported yet
  console::WriteLine("Solution not supported yet");
  return -1;

  //return Compiler::compile(solutionConfig);
}

void parseModuleFile(std::ifstream &moduleFile, ModuleConfig &moduleConfig)
{
  namespace fs = std::filesystem;

  while (moduleFile.good() && !moduleFile.eof()) {
    std::string key;
    std::string value;

    moduleFile >> key;
    moduleFile >> value;

    auto semiPos = key.find_first_of(":");
    key = key.substr(0, semiPos);

    if (key == MOD_KEY_SRC_FOLDER) {
      moduleConfig.srcFolder = fs::canonical(value).string();
    } else if (key == MOD_KEY_MODULE_NAME) {
      moduleConfig.moduleName = value;
    } else if (key == MOD_KEY_EXPORT_MOD_INFO) {
      moduleConfig.exportInfo = value == "true";
    } else if (key == MOD_KEY_TYPE) {
      if (value == "EXE")
        moduleConfig.moduleType = MODULE_TYPE::EXE;
      else if (value == "DLL")
        moduleConfig.moduleType = MODULE_TYPE::DLL;
      else if (value == "LIB")
        moduleConfig.moduleType = MODULE_TYPE::LIB;
      else
        console::WriteLine("Invalid module type: " + value);
    }
  }
}

std::string get_first_file_name(const ModuleConfig& module_config, int &error)
{
    namespace fs = std::filesystem;
    auto folder_name = module_config.srcFolder;

    auto path = fs::canonical(folder_name);
    error = 0;

    if (!fs::exists(path)) {
        console::WriteLine("Directory not found: " + folder_name);
        error = -1;
        return "";
    }

    if (!fs::is_directory(path)) {
        console::WriteLine("Path is not a directory: " + folder_name);
        error = -1;
        return "";
    }

    auto directory = fs::recursive_directory_iterator(folder_name);

    for (auto entry : directory) {
        auto filePath = entry.path();
        auto ext = filePath.extension().string();
        auto fileName = filePath.filename().string();

        if (entry.is_regular_file() && ext == ".llang") {
            return entry.path().string();
        }
    }

    error = -1;
    return "";
}

std::string get_current_dir()
{
  char buff[FILENAME_MAX]; //create string buffer to hold path
  GetCurrentDir(buff, FILENAME_MAX);
  std::string current_working_dir(buff);
  return current_working_dir;
}
