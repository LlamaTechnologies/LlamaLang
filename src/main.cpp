#include <string>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <stddef.h> // size_t
#include "console.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "compiler.hpp"

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#define ARG_SRC_FILE "-s"
#define ARG_OUT_NAME "-o"
#define ARG_OUT_DIR  "-O"

static std::string get_current_dir();

int main(int argc, const char *argv[])
{
  namespace fs = std::filesystem;
  // get current directory
  auto current_dir_str = get_current_dir();
  fs::path current_dir_path(current_dir_str);
  fs::directory_iterator current_dir(current_dir_path);

  std::string source_name;
  std::string output_name;
  std::string output_dir_str;

  // arg parsing
  {
      for (size_t i = 1; i < argc; i += 2) {
          const char* option = argv[i];
          size_t option_len = strlen(option);
          if (strncmp(option, ARG_SRC_FILE, option_len > 2 ? 2 : option_len) == 0) {
              source_name = argv[i + 1];
          }
          else if (strncmp(option, ARG_OUT_NAME, option_len > 2 ? 2 : option_len) == 0) {
              output_name = argv[i + 1];
          }
          else if (strncmp(option, ARG_OUT_DIR, option_len > 2 ? 2 : option_len) == 0) {
              output_dir_str = argv[i + 1];
          }
          else {
              std::cout << "bad argument: " << option << std::endl;
              return -1;
          }
      }

      if (output_dir_str.empty()) {
          output_dir_str = current_dir_str;
      }
  }

  // open source file
  auto file_path = fs::absolute(fs::path(source_name));
  std::ifstream source_file(file_path);
  {
      if (!source_file.is_open()) {
          // error could not find file
          std::cout << "could not find file \"" << source_name << "\" in directory \"" << current_dir_str << "\"" << std::endl;
          return -1;
      }
      if (source_file.bad()) {
          // error corrupted file
          std::cout << "corrupted file \"" << source_name << std::endl;
          return -1;
      }
  }

  /*
  for (auto entry : currentDir) {
    if (entry.is_regular_file()) {
      auto fileName = entry.path().filename();

      if (fileName == "build.llang") {

      }
    }
  }
  */

  std::string source_code;
  // get source_code
  {
      // go to end of file
      source_file.seekg(0, std::ios::end);

      // Reserve memory
      source_code.reserve(source_file.tellg());

      // reset file cursor
      source_file.seekg(0, std::ios::beg);

      // store content in this.source
      source_code.assign((std::istreambuf_iterator<char>(source_file)), std::istreambuf_iterator<char>());

      // close file
      source_file.close();
  }

  std::vector<Error> errors;
  Lexer lexer(source_code, source_name, errors);
  lexer.tokenize();
  
  /*
  auto print_lines = print_tokens(lexer);
  for (auto line : print_lines)
      console::WriteLine(line);
  */

  Parser parser(lexer, errors);
  auto source_code_node = parser.parse();
  
  compiler::compile(output_dir_str, output_name, source_code_node, errors);

  return 0;
  //return Compiler::compile(build_options);
}

std::string get_current_dir()
{
  char buff[FILENAME_MAX]; //create string buffer to hold path
  GetCurrentDir(buff, FILENAME_MAX);
  std::string current_working_dir(buff);
  return current_working_dir;
}
