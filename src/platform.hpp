#include "common_defs.hpp"

#include <string>

extern const char *LINKER_NAME;
extern char *(*GetCurrentDir)(char *, size_t);

std::string get_path_to_program_by_name(const std::string &in_name);

s32 run_process(const std::string &in_program_path, const std::string &in_program_args);
