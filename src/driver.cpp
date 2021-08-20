#include "driver.hpp"

#include "common_defs.hpp"
#include "compiler.hpp"
#include "console.hpp"
#include "error.hpp"
#include "lexer.hpp"
#include "parser.hpp"

#include <system_error>

static std::string read_file(std::ifstream &in_file);
static std::string get_current_dir();
static char **split_string(const std::string &in_str, const char in_separator);
static std::string get_path_to_program_by_name(const std::string &in_name);
static int run_process(const std::string &in_program_path, std::string &in_program_args);

#define LL_FILE_EXTENTION ".llang"
#define ARG_SRC_FILE "-s"
#define ARG_OUT_NAME "-o"
#define ARG_OUT_DIR "-O"

Driver::Driver() : current_dir(get_current_dir()) {}

bool Driver::setup(const char **argv, const int argc) {
  if (!parse_args(argv, argc)) {
    return false;
  }

  if (!verify_file_path()) {
    return false;
  }

  if (!get_tool_chain()) {
    return false;
  }

  return true;
}

bool Driver::run() {
  auto source_file = std::ifstream(this->file_path);
  if (source_file.bad()) {
    console::WriteLine("corrupted file \"" + this->file_path.string() + "\"");
    return false;
  }

  const std::string source_name = this->file_path.filename().string();
  std::string source_code = read_file(source_file);

  bool program_ok = compiler::compile(this->output_dir, this->output_name, source_code, source_name);
  if (program_ok == false) {
    return false;
  }

  // TODO(pablo96): generate exe|lib|dll output

  std::string llc_args = "";
  int llc_exit_code = run_process(this->llc_path, llc_args);
  if (llc_exit_code < 0) {
    return false;
  }

  std::string lld_args = "";
  int lld_exit_code = run_process(this->lld_path, lld_args);
  if (lld_exit_code < 0) {
    return false;
  }

  return true;
}

bool Driver::get_tool_chain() {
  this->llc_path = get_path_to_program_by_name("llc");
  this->lld_path = get_path_to_program_by_name("lld");
  {
    if (this->llc_path.size() == 0) {
      console::WriteLine("llc not found. please install llvm toolchain.");
      return false;
    }
    if (this->lld_path.size() == 0) {
      console::WriteLine("lld not found. please install llvm toolchain.");
      return false;
    }
  }
  return true;
}

bool Driver::verify_file_path() {
  std::error_code error_code;
  // TODO(pablo96): handle error_code
  bool path_exists = std::filesystem::exists(this->file_path, error_code);
  if (!path_exists) {
    console::WriteLine("file '" + this->file_path.string() + "' does not exists!");
    return false;
  }

  bool is_file = std::filesystem::is_regular_file(this->file_path, error_code);
  if (!is_file) {
    console::WriteLine("file '" + this->file_path.string() + "' is not a file!");
    return false;
  }

  if (this->file_path.extension() != LL_FILE_EXTENTION) {
    console::WriteLine("file '" + this->file_path.string() + "' is not a llama lang file!");
    return false;
  }

  return true;
}

bool Driver::parse_args(const char **argv, const int argc) {
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
      console::WriteLine(std::string("bad argument: ") + option);
      return false;
    }
  }

  if (this->output_dir.empty()) {
    this->output_dir = current_dir;
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
  this->file_path = std::filesystem::absolute(std::filesystem::path(source_name), error_code);
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

std::string read_file(std::ifstream &in_file) {
  std::string file_content;

  in_file.seekg(0, std::ios::end);

  file_content.reserve(in_file.tellg());

  in_file.seekg(0, std::ios::beg);

  // store content in this.source
  file_content.assign((std::istreambuf_iterator<char>(in_file)), std::istreambuf_iterator<char>());

  in_file.close();

  return file_content;
}

std::string get_current_dir() {
  char buff[FILENAME_MAX]; // create string buffer to hold path
  GetCurrentDir(buff, FILENAME_MAX);
  std::string current_working_dir(buff);
  return current_working_dir;
}

#ifdef LL_LINUX
std::string get_path_to_program_by_name(const std::string &in_name) {
  std::string path_var = std::string(getenv("PATH"));

  return path_var;
}

int run_process(const std::string &in_program_path, std::string &in_program_args) {
  pid_t pid = fork();
  int wstatus;

  if (pid == 0) {
    char **argv = split_string(in_program_args, ' ');
    // replace current process(child) to the desired program.
    // it is no return unless an error ocurred.
    auto error_code = execve(in_program_path.c_str(), argv, NULL);
    if (error_code == -1) {
      // Handle error
      return -1;
    }
  }

  int error = waitpid(pid, &wstatus, 0);
  if (error < 0) {
    // handle error
    return -1;
  }

  if (!WIFEXITED(wstatus)) {
    return -1;
  }

  return WEXITSTATUS(wstatus);
}
#endif

#ifdef LL_WIN32
std::string get_path_to_program_by_name(const std::string &in_name) {
  const unsigned int nBufferLength = MAX_PATH;

  std::string buffer;
  buffer.reserve(nBufferLength);

  DWORD writenChars = SearchPathA(NULL, // null to make it search on the registry
                                  in_name.c_str(), ".exe", nBufferLength, buffer.data(), nullptr);

  if (writenChars <= 0) {
    return "";
  }

  buffer.resize(writenChars);
  buffer.shrink_to_fit();
  return buffer;
}

int run_process(const std::string &in_program_path, std::string &in_program_args) {
  PROCESS_INFORMATION process_info = {};
  STARTUPINFOA start_info = {};
  start_info.cb = sizeof(STARTUPINFOA);

  bool is_ok = CreateProcessA(in_program_path.c_str(), in_program_args.data(),
                              nullptr, // default security attributes for process
                              nullptr, // default security attributes for thread
                              false, 0, nullptr, nullptr, &start_info, &process_info);

  if (is_ok == false) {
    // handle error
    return -1;
  }

  auto handle = process_info.hProcess;

  // lock until process finish
  DWORD wait_status = WaitForSingleObject(handle, INFINITE);

  if (wait_status == WAIT_FAILED) {
    // handle error
    return -1;
  }

  // get process exit status
  DWORD process_exit_code;
  is_ok = GetExitCodeProcess(handle, &process_exit_code);

  if (is_ok == false) {
    // handle error
    return -1;
  }

  CloseHandle(handle);

  return process_exit_code;
}
#endif

char **split_string(const std::string &in_str, const char in_separator) {
  char *strings = (char*) malloc(in_str.size());

  for (size_t i = 0; i < in_str.size(); ++i) {
    char *c = &strings[i];
    if (*c == in_separator) {
      *c = 0;
    }
  }
  return (char **)strings;
}