#pragma once
#include <filesystem>

enum class FILE_PATH_STATUS
{
  OK,
  NOT_FOUND,
  NOT_A_FILE,
  NOT_LLAMA_FILE
};

constexpr const char *BITCODE_FILE_EXTENSION = ".bc";
constexpr const char *EXE_EXTENSION = ".exe";
constexpr const char *LL_FILE_EXTENSION = ".llama";

std::filesystem::path resolve_path(const std::string &in_file_path, std::error_code &error_code);

struct FileInput {
  virtual FILE_PATH_STATUS verify_file_path(std::filesystem::path &in_path) const;

  /* Reads the entire file and returns it as string */
  virtual std::string read_file(std::ifstream &in_file) const;

  /* Opens and reads a file.
   * NOTE: assumes file exists
   */
  virtual std::string open_and_read_file(const std::string &in_file_path) const;
};

const FileInput &get_std_file_input();
