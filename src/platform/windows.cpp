#include "../common_defs.hpp"

#ifdef LL_WINDOWS
  #include <Windows.h>
  #include <direct.h>
  #include "../file_utils.hpp"
  
  constexpr const char *LINKER_NAME = "link";
  constexpr const char *EXE_EXTENSION = ".exe";

  char *(*GetCurrentDir)(char *, size_t) = _getcwd;
  
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

  s32 run_process(const std::string &in_program_path, std::string &in_program_args) {
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
