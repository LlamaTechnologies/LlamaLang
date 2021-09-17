#include "driver.hpp"

#include "common_defs.hpp"
#include "compiler.hpp"
#include "console.hpp"
#include "error.hpp"
#include "file_utils.hpp"
#include "lexer.hpp"
#include "parser.hpp"

#include <system_error>

static std::string get_current_dir();
static std::vector<char *> *split_string(const std::string &in_str, const char in_separator);
static std::string get_path_to_program_by_name(const std::string &in_name);
static s32 run_process(const std::string &in_program_path, const std::string &in_program_args);

constexpr const char *ARG_SRC_FILE = "-s";
constexpr const char *ARG_OUT_NAME = "-o";
constexpr const char *ARG_OUT_DIR = "-O";

Driver::Driver() : current_dir(get_current_dir()) {}

bool Driver::setup(const char **argv, const s32 argc) {
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
  auto source_file = std::ifstream(this->file_path);
  if (source_file.bad()) {
    console::write_line("corrupted file \"" + this->file_path.string() + "\"");
    return false;
  }

  const std::string source_name = this->file_path.filename().string();
  std::string source_code = get_std_file_input().read_file(source_file);

  bool program_ok = compiler::compile(this->file_path.parent_path().string(), this->output_dir, this->output_name,
                                      source_code, source_name);
  if (program_ok == false) {
    return false;
  }

  std::string file_full_path = this->output_dir + "/" + this->output_name;
  auto file_out = file_full_path + ".a";
  auto file_in = file_full_path + BITCODE_FILE_EXTENSION;
  std::string lld_args = this->lld_path + " " + file_in + " -o " + file_out;

  console::write_line(lld_args);

  s32 lld_exit_code = run_process(this->lld_path, lld_args);
  if (lld_exit_code < 0) {
    console::write_line("error with lld");
    return false;
  }

  console::write_line("Compiled Successfuly: " + file_out);
  return true;
}

#ifdef LL_WIN32
  #define LLD_NAME "link"
#elif defined(LL_LINUX)
  #define LLD_NAME "clang"
#endif

bool Driver::_get_tool_chain() {
  this->lld_path = LLD_NAME; // get_path_to_program_by_name(LLD_NAME);
  {
    if (this->lld_path.size() == 0) {
      console::write_line("lld not found. please install llvm toolchain.");
      return false;
    }
  }
  return true;
}

bool Driver::_verify_file_path() {
  switch (get_std_file_input().verify_file_path(this->file_path)) {
  case FILE_PATH_STATUS::NOT_FOUND:
    console::write_line("file '" + this->file_path.string() + "' does not exists!");
    return false;
  case FILE_PATH_STATUS::NOT_A_FILE:
    console::write_line("file '" + this->file_path.string() + "' is not a file!");
    return false;
  case FILE_PATH_STATUS::NOT_LLAMA_FILE:
    console::write_line("file '" + this->file_path.string() + "' is not a llama lang file!");
    return false;
  default:
    return true;
  }
}

bool Driver::_parse_args(const char **argv, const s32 argc) {
  std::string source_name;

  for (size_t i = 1; i < argc; i += 2) {
    const char *option = argv[i];
    size_t option_len = strlen(option);
    if (strncmp(option, ARG_SRC_FILE, option_len > 2 ? 2 : option_len) == 0) {
      source_name = argv[i + 1];
    } else if (strncmp(option, ARG_OUT_NAME, option_len > 2 ? 2 : option_len) == 0) {
      output_name = argv[i + 1];
    } else if (strncmp(option, ARG_OUT_DIR, option_len > 2 ? 2 : option_len) == 0) {
      this->output_dir = argv[i + 1];
    } else {
      console::write_line(std::string("bad argument: ") + option);
      return false;
    }
  }

  if (this->output_dir.empty()) {
    this->output_dir = current_dir;
  }

  if (this->output_name.empty()) {
    this->output_name = "llama_program";
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
  this->file_path = resolve_path(source_name, error_code);
  // TODO(pablo96): handle error_code

  return true;
}

#ifdef LL_WIN32
  #include <Windows.h>
  #include <direct.h>
  #define GetCurrentDir _getcwd
#else
  #include <sys/wait.h>
  #include <unistd.h>
  #define GetCurrentDir getcwd
#endif

std::string get_current_dir() {
  char buff[FILENAME_MAX]; // create string buffer to hold path
  GetCurrentDir(buff, FILENAME_MAX);
  std::string current_working_dir(buff);
  return current_working_dir;
}

#ifdef LL_LINUX
std::string get_path_to_program_by_name(const std::string &in_name) {
  FILE *fp;
  char path[1035];

  /* Open the command for reading. */
  fp = popen(std::string("/bin/which " + in_name).data(), "r");
  if (fp == NULL) {
    printf("Failed to run command\n");
    return "";
  }

  /* Read the output a line at a time - output it. */
  if (fgets(path, sizeof(path), fp) != NULL) {
    printf("%s", path);
    pclose(fp);
    auto str = std::string(path);
    auto index = str.find_last_of('\n');
    if (index != str.npos)
      str.erase(index);
    return str;
  }

  pclose(fp);
  return "";
}

s32 run_process(const std::string &in_program_path, const std::string &in_program_args) {
  return system(in_program_args.c_str());
}
#endif

#ifdef LL_WIN32
std::string get_path_to_program_by_name(const std::string &in_name) {
  const u32 n_buffer_len = MAX_PATH;

  char *buffer = new char[n_buffer_len];
  DWORD writen_chars = SearchPathA(NULL, // null to make it search on the registry
                                 in_name.c_str(), ".exe", n_buffer_len, buffer, nullptr);

  if (writen_chars <= 0) {
    return "";
  }
  auto ret_val = std::string(buffer);
  delete[] buffer;
  return ret_val;
}

void handle_error() {
  LPVOID lp_msg_buf = nullptr;
  DWORD error_code = GetLastError();

  DWORD no_error =
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
                   error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char *)&lp_msg_buf, 0, NULL);

  if (!no_error) {
    console::write_line("error formatting msg");
  }

  if (lp_msg_buf) {
    console::write_line((char *)lp_msg_buf);
    LocalFree(lp_msg_buf);
  }
}

i32 run_process(const std::string &in_program_path, std::string &in_program_args) {
  PROCESS_INFORMATION process_info = {};
  STARTUPINFOA start_info = {};
  start_info.cb = sizeof(STARTUPINFOA);

  bool is_ok = CreateProcessA(in_program_path.c_str(), in_program_args.data(),
                              nullptr, // default security attributes for process
                              nullptr, // default security attributes for thread
                              false, 0, nullptr, nullptr, &start_info, &process_info);

  auto handle = process_info.hProcess;
  if (is_ok == false) {
    handle_error();
    CloseHandle(handle);
    return -1;
  }

  // lock until process finish
  DWORD wait_status = WaitForSingleObject(handle, INFINITE);

  if (wait_status == WAIT_FAILED) {
    handle_error();
    CloseHandle(handle);
    return -1;
  }

  // get process exit status
  DWORD process_exit_code;
  is_ok = GetExitCodeProcess(handle, &process_exit_code);

  if (is_ok == false) {
    handle_error();
    CloseHandle(handle);
    return -1;
  }

  CloseHandle(handle);

  return process_exit_code;
}
#endif

std::vector<char *> *split_string(const std::string &in_str, const char in_separator) {
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
