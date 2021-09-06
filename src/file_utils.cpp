#include "file_utils.hpp"

#include "console.hpp"

std::filesystem::path resolve_path(const std::string &in_file_path, std::error_code &error_code) {
  return std::filesystem::absolute(std::filesystem::path(in_file_path), error_code);
}

FILE_PATH_STATUS FileInput::verify_file_path(std::filesystem::path &in_path) const {
  std::error_code error_code;

  bool path_exists = std::filesystem::exists(in_path, error_code);
  if (!path_exists) {
    return FILE_PATH_STATUS::NOT_FOUND;
  }

  bool is_file = std::filesystem::is_regular_file(in_path, error_code);
  if (!is_file) {
    return FILE_PATH_STATUS::NOT_A_FILE;
  }

  if (in_path.extension() != LL_FILE_EXTENSION) {
    return FILE_PATH_STATUS::NOT_LLAMA_FILE;
  }

  return FILE_PATH_STATUS::OK;
}

std::string FileInput::read_file(std::ifstream &in_file) const {
  std::string file_content;

  in_file.seekg(0, std::ios::end);

  file_content.reserve(in_file.tellg());

  in_file.seekg(0, std::ios::beg);

  // store content in this.source
  file_content.assign((std::istreambuf_iterator<char>(in_file)), std::istreambuf_iterator<char>());

  in_file.close();

  return file_content;
}

std::string FileInput::open_and_read_file(const std::string &in_file_path) const {
  std::ifstream file = std::ifstream(in_file_path);
  return read_file(file);
}

FileInput *file_input = nullptr;

const FileInput &get_std_file_input() {
  if (file_input == nullptr) {
    file_input = new FileInput();
  }
  return *file_input;
}
