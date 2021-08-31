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

std::filesystem::path resolve_path(std::string &in_file_path, std::error_code &error_code);

FILE_PATH_STATUS verify_file_path(std::filesystem::path &in_path);

std::string read_file(std::ifstream &in_file);
