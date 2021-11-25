#include "file_utils.hpp"

#include "console.hpp"

#define MAX_FILE_SIZE 0x40000000 // 1GB

LL_INTERNAL_FN u64 get_file_size(const std::filesystem::path &in_path) { return std::filesystem::file_size(in_path); }

LL_INTERNAL_FN std::filesystem::perms openfilemode_to_permissions(const FILE_MODE_FLAG in_mode) {
  std::filesystem::perms perms = std::filesystem::perms(0);

  if (true == bool(u64(in_mode) & u64(FILE_MODE_FLAG::READ))) {
    perms |=
      std::filesystem::perms::group_read | std::filesystem::perms::owner_read | std::filesystem::perms::others_read;
  }

  if (true == bool(u64(in_mode) & u64(FILE_MODE_FLAG::WRITE))) {
    perms |=
      std::filesystem::perms::group_write | std::filesystem::perms::owner_write | std::filesystem::perms::others_write;
  }

  return perms;
}

LL_INTERNAL_FN std::ios::openmode openfilemode_to_openmode(const FILE_MODE_FLAG in_mode) {
  std::ios::openmode open_mode;

  if (u64(in_mode) & u64(FILE_MODE_FLAG::READ)) {
    open_mode |= std::ios::in;
  }

  if (u64(in_mode) & u64(FILE_MODE_FLAG::WRITE)) {
    open_mode |= std::ios::out;
  }

  return open_mode;
}

std::filesystem::path resolve_path(const std::string &in_file_path, std::error_code &error_code) {
  return std::filesystem::absolute(std::filesystem::path(in_file_path), error_code);
}

OPEN_FILE_ERROR FileInput::verify_file_path(const std::filesystem::path &in_path,
                                            const FILE_MODE_FLAG in_mode_flags) const {
  std::error_code error_code;

  bool path_exists = std::filesystem::exists(in_path, error_code);
  if (!path_exists) {
    return OPEN_FILE_ERROR::NOT_FOUND;
  }

  bool is_file = std::filesystem::is_regular_file(in_path, error_code);
  if (!is_file) {
    return OPEN_FILE_ERROR::NOT_A_FILE;
  }

  std::filesystem::file_status path_status = std::filesystem::status(in_path);
  std::filesystem::perms wanted_permissions = openfilemode_to_permissions(in_mode_flags);
  std::filesystem::perms permissions = path_status.permissions();

  if (true == bool(permissions & wanted_permissions)) {
    return OPEN_FILE_ERROR::PERMISSION_DENIED;
  }

  return OPEN_FILE_ERROR::OK;
}

READ_FILE_ERROR read_bytes_at_offset(File &in_file, const u64 offset, const u64 count, u8 *out_content) const {
  LL_ASSERT(out_content != nullptr);

  auto &file = in_file.handle;

  file.seekg(std::streampos(offset));
  if (file.bad())
    return READ_FILE_ERROR::OFFSET_TO_LARGE;

  file.read((char *)out_content, count);
  if (file.bad())
    return READ_FILE_ERROR::COUNT_OUT_OF_RANGE;

  out_content[count] = 0;
  return READ_FILE_ERROR::OK;
}

READ_FILE_ERROR FileInput::read_rune_at_offset(File &in_file, const u64 offset, rune &out_content) const {
  u64 count = (u64)sizeof(rune);

  auto &file = in_file.handle;

  file.seekg(std::streampos(offset));
  if (file.bad())
    return READ_FILE_ERROR::OFFSET_TO_LARGE;

  file.read((char *)&out_content, count);
  if (file.bad())
    return READ_FILE_ERROR::COUNT_OUT_OF_RANGE;

  return READ_FILE_ERROR::OK;
}

OPEN_FILE_ERROR FileInput::read_full_file(File &in_file, std::string &out_content) const {
  auto &file = in_file.handle;
  defer(file.close(););

  out_content.reserve(in_file.size);
  out_content.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  if (file.bad()) {
    return OPEN_FILE_ERROR::INVALID;
  }

  return OPEN_FILE_ERROR::OK;
}

OPEN_FILE_ERROR FileInput::open_file(const std::string &in_file_path, const FILE_MODE_FLAG in_mode_flags,
                                     File &out_file, std::string *out_fullpath = nullptr) const {
  std::error_code err_code;

  auto path = resolve_path(in_file_path, err_code);
  if (nullptr != out_fullpath) {
    *out_fullpath = path.string();
  }

  OPEN_FILE_ERROR file_path_status = this->verify_file_path(path, in_mode_flags);
  if (file_path_status != OPEN_FILE_ERROR::OK) {
    return file_path_status;
  }

  std::ios::openmode mode = openfilemode_to_openmode(in_mode_flags);

  out_file.handle.open(in_file_path, mode);
  defer(out_file.handle.close());

  if (out_file.handle.bad()) {
    return OPEN_FILE_ERROR::INVALID;
  }

  out_file.size = get_file_size(path);
  if (MAX_FILE_SIZE < out_file.size) {
    return OPEN_FILE_ERROR::TOO_LARGE;
  }

  if (0 >= out_file.size) {
    return OPEN_FILE_ERROR::EMPTY;
  }

  return OPEN_FILE_ERROR::OK;
}

LL_LOCAL FileInput *file_input = nullptr;

const FileInput &get_std_file_input() {
  if (file_input == nullptr) {
    file_input = new FileInput();
  }
  return *file_input;
}
