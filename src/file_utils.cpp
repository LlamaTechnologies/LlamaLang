#include "file_utils.hpp"

#include "console.hpp"

std::filesystem::path resolve_path(std::string &in_file_path, std::error_code &error_code) {
  return std::filesystem::absolute(std::filesystem::path(in_file_path), error_code);
}

FILE_PATH_STATUS verify_file_path(std::filesystem::path &in_path) {
  std::error_code error_code;

  bool path_exists = std::filesystem::exists(in_path, error_code);
  if (!path_exists) {
    console::write_line("file '" + in_path.string() + "' does not exists!");
    return FILE_PATH_STATUS::NOT_FOUND;
  }

  bool is_file = std::filesystem::is_regular_file(in_path, error_code);
  if (!is_file) {
    console::write_line("file '" + in_path.string() + "' is not a file!");
    return FILE_PATH_STATUS::NOT_A_FILE;
  }

  if (in_path.extension() != LL_FILE_EXTENSION) {
    console::write_line("file '" + in_path.string() + "' is not a llama lang file!");
    return FILE_PATH_STATUS::NOT_LLAMA_FILE;
  }

  return FILE_PATH_STATUS::OK;
}

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
