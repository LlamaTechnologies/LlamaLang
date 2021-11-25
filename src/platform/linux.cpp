#include "../common_defs.hpp"

#ifdef LL_LINUX
  #include "../utils/file_utils.hpp"

  #include <sys/wait.h>
  #include <unistd.h>

char *(*GetCurrentDir)(char *, size_t) = getcwd;

constexpr const char *LINKER_NAME = "clang";
constexpr const char *EXE_EXTENSION = "";

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
