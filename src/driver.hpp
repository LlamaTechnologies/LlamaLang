#pragma once
#include "common_defs.hpp"

#include <filesystem>
#include <string>
struct FileInput;

struct Driver {
  Driver();

  bool setup(const char **argv, const s32 argc);

  bool run();

private:
  std::string current_dir;
  std::string lld_path;

  std::string output_dir;
  std::string output_name;

  std::filesystem::path file_path;

  bool _parse_args(const char **argv, const s32 argc);

  bool _verify_file_path();

  bool _get_tool_chain();
};
