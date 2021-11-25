#include "driver.hpp"

#include "common_defs.hpp"
#include "compiler.hpp"
#include "error.hpp"
#include "platform.hpp"
#include "utils/console.hpp"
#include "utils/file_utils.hpp"
#include "tokenizer/tokenizer.hpp"
#include "parser/parser.hpp"

#include <filesystem>
#include <system_error>

namespace ArgFlags {
  LL_LOCAL constexpr const char *SRC_FILE = "-s";
  LL_LOCAL constexpr const char *OUT_NAME = "-o";
  LL_LOCAL constexpr const char *OUT_DIR = "-O";
};

LL_LOCAL struct {
  std::string current_dir;
  std::string lld_path;

  std::string output_dir;
  std::string output_name;

  std::filesystem::path file_path;
} build_context;


LL_INTERNAL_FN bool _link_stage();
LL_INTERNAL_FN bool _get_tool_chain();
LL_INTERNAL_FN bool _verify_file_path();
LL_INTERNAL_FN bool _parse_args(const char **argv, const s32 argc);
LL_INTERNAL_FN std::string _get_current_dir();
LL_INTERNAL_FN std::vector<char *> *_split_string(const std::string &in_str, const char in_separator);

bool Driver::setup(const char **argv, const s32 argc) {
    build_context.current_dir = _get_current_dir();
    
    if (!_parse_args(argv, argc)) {
      return false;
    }

    if (!_verify_file_path()) {
      return false;
    }

    if (!_get_tool_chain()) {
      return false;
    }

    return true;
  }

bool Driver::run() {
    auto source_file = std::ifstream(build_context.file_path);
    if (source_file.bad()) {
      console::write_line("corrupted file \"" + build_context.file_path.string() + "\"");
      return false;
    }

    const std::string source_name = build_context.file_path.filename().string();
    std::string source_code = get_std_file_input().read_file(source_file);

    bool program_ok = compiler::compile(build_context.file_path.parent_path().string(),
                                        build_context.output_dir,
                                        build_context.output_name,
                                        source_code, source_name);
    if (!program_ok) {
      return false;
    }

    if (!_link_stage()) {
      return false;
    }

    console::write_line("Compiled Successfuly!");
    return true;
  }

bool _get_tool_chain() {
  build_context.lld_path = LINKER_NAME; // get_path_to_program_by_name(LLD_NAME);
  {
    if (build_context.lld_path.size() == 0) {
      console::write_line("lld not found. please install llvm toolchain.");
      return false;
    }
  }
  return true;
}

bool _verify_file_path() {
  switch (get_std_file_input().verify_file_path(build_context.file_path)) {
  case FILE_PATH_STATUS::NOT_FOUND:
    console::write_line("file '" + build_context.file_path.string() + "' does not exists!");
    return false;
  case FILE_PATH_STATUS::NOT_A_FILE:
    console::write_line("file '" + build_context.file_path.string() + "' is not a file!");
    return false;
  case FILE_PATH_STATUS::NOT_LLAMA_FILE:
    console::write_line("file '" + build_context.file_path.string() + "' is not a llama lang file!");
    return false;
  default:
    return true;
  }
}

bool _parse_args(const char **argv, const s32 argc) {
  std::string source_name;

  for (size_t i = 1; i < argc; i += 2) {
    const char *option = argv[i];
    size_t option_len = strlen(option);
    if (strncmp(option, ArgFlags::SRC_FILE, option_len > 2 ? 2 : option_len) == 0) {
      source_name = argv[i + 1];
    } else if (strncmp(option, ArgFlags::OUT_NAME, option_len > 2 ? 2 : option_len) == 0) {
      build_context.output_name = argv[i + 1];
    } else if (strncmp(option, ArgFlags::OUT_DIR, option_len > 2 ? 2 : option_len) == 0) {
      build_context.output_dir = argv[i + 1];
    } else {
      console::write_line(std::string("bad argument: ") + option);
      return false;
    }
  }

  if (build_context.output_dir.empty()) {
    build_context.output_dir = build_context.current_dir;
  }

  if (build_context.output_name.empty()) {
    build_context.output_name = "llama_program";
  }

  // if not source file provided then search for build file
  if (source_name.empty()) {
    // TODO(pablo96): search for build file
    /*
    for (auto entry : currentDir) {
      if (entry.is_regular_file()) {
        auto fileName = entry.path().filename();

        if (fileName == "build.llang") {}
      }
    }
    */
  }

  std::error_code error_code;
  build_context.file_path = resolve_path(source_name, error_code);
  // TODO(pablo96): handle error_code

  return true;
}

bool _link_stage() {
  std::string file_full_path = build_context.output_dir + "/" + build_context.output_name;
  auto file_out = file_full_path + ".a";
  auto file_in = file_full_path + FileExtensions::BITCODE_FILE_EXTENSION;
  std::string lld_args = build_context.lld_path + " " + file_in + " -o " + file_out;

  console::write_line(lld_args);

  s32 lld_exit_code = run_process(build_context.lld_path, lld_args);
  if (lld_exit_code < 0) {
    console::write_line("error with lld");
    return false;
  }

  return true;
}

std::string _get_current_dir() {
  char buff[FILENAME_MAX]; // create string buffer to hold path
  GetCurrentDir(buff, FILENAME_MAX);
  std::string current_working_dir(buff);
  return current_working_dir;
}

std::vector<char *> *_split_string(const std::string &in_str, const char in_separator) {
  auto strings = new std::vector<char *>();

  size_t last_null = 0;
  for (size_t i = 0; i < in_str.size(); ++i) {
    char c = in_str[i];
    if (c == in_separator) {
      char *str_c = new char[i + 1];
      std::string str = in_str.substr(last_null + 1, i);
      last_null = i;
      memcpy(str_c, str.data(), str.size());
      strings->push_back(str_c);
    }
  }
  strings->push_back(NULL);
  return strings;
}
