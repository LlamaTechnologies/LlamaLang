#pragma once
#include "../common_defs.hpp"

#include <filesystem>
#include <fstream>

struct File {
  std::fstream handle;
  s32 size;
};

enum class FILE_MODE_FLAG
{
  READ = SET_BIT(0),
  WRITE = SET_BIT(1),
};

enum class OPEN_FILE_ERROR
{
  OK,
  NOT_FOUND,
  NOT_A_FILE,
  PERMISSION_DENIED,
  EMPTY,
  TOO_LARGE,
  INVALID,
};

enum class READ_FILE_ERROR
{
  OK,
  OFFSET_TO_LARGE,
  COUNT_OUT_OF_RANGE,
  INVALID,
};

namespace FileExtensions {
constexpr const char *BITCODE_FILE_EXTENSION = ".bc";
constexpr const char *LL_FILE_EXTENSION = ".llama";
extern const char *EXE_EXTENSION;
}; // namespace FileExtensions

std::filesystem::path resolve_path(const std::string &in_file_path, std::error_code &error_code);

struct FileInput {
  virtual OPEN_FILE_ERROR verify_file_path(const std::filesystem::path &in_path,
                                           const FILE_MODE_FLAG in_mode_falgs) const;

  /**
   * Offset the file cursor to the value given and reads count amount of bytes from it.
   * NOTE: if an error ocurs the file is closed.
   * NOTE: out_content should be valid memory of a size count+1 or more.
   *
   * The file's content is stored as string in out_content.
   * Returns true if an error occured at reading the file.
   */
  virtual READ_FILE_ERROR read_bytes_at_offset(File &in_file, const u64 offset, const u64 count, u8 *out_content) const;

  /**
   * Offset the file cursor to the value given and reads a rune (4 bytes) value from it.
   * NOTE: if an error ocurs the file is closed.
   *
   * The file's content is stored as string in out_content.
   * Returns true if an error occured at reading the file.
   */
  virtual READ_FILE_ERROR read_rune_at_offset(File &in_file, const u64 offset, rune &out_content) const;

  /**
   * Reads the entire file and close it.
   * NOTE: if an error ocurs the file is closed.
   *
   * The file's content is stored as string in out_content.
   * Returns OPEN_FILE_ERROR::INVALID or OPEN_FILE_ERROR::OK
   */
  virtual OPEN_FILE_ERROR read_full_file(File &in_file, std::string &out_content) const;

  /**
   * Opens a file in in_mode_flags mode
   */
  virtual OPEN_FILE_ERROR open_file(const std::string &in_file_path, const FILE_MODE_FLAG in_mode_falgs, File &out_file,
                                    std::string *out_fullpath = nullptr) const;
};

const FileInput &get_std_file_input();
