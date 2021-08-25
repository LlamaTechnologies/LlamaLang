#pragma once
#include <string>
#include <filesystem>

class Driver {
  std::string lld_path;

  std::string current_dir;
  std::string output_dir;

  std::string output_name;

  std::filesystem::path file_path;

public:
  Driver();

  bool setup(const char **argv, const int argc);

  bool run();

private:
  bool parse_args(const char **argv, const int argc);

  bool verify_file_path();
  
  bool get_tool_chain();
};
