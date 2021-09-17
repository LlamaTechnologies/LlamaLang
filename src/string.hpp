#pragma once
#include "common_defs.hpp"

#include <stddef.h>

class StringView;

class String {
  char *buffer;
  size_t buf_size;
  size_t data_size;

  void _calculate_sizes(const char *in_cstr) noexcept;

public:
  /**
   * Empty string "\0"
   **/
  String();

  /**
   * new buffer with size in_buf_size
   * zeroed buffer
   **/
  String(const size_t in_buf_size);

  /**
   * Copy cstr to a new buffer
   **/
  String(const char *in_cstr);

  /**
   * Set cstr as the buffer
   **/
  String(char *in_cstr);

  String(const StringView in_view);

  const char *c_str() const;
  size_t size() const;
  size_t buffer_size() const;

  s32 to_int32(const int in_base = 10) const;
  u32 to_uint32(const int in_base = 10) const;

  s64 to_int64(const int in_base = 10) const;
  u64 to_uint64(const int in_base = 10) const;

  f32 to_float32() const;
  f64 to_float64() const;

  ~String();
};

class StringView {
  friend class String;
  const char *buffer;
  size_t len;

public:
  StringView(const char *in_cstr, const size_t in_offset, const size_t in_len);

  size_t size() const;

  /**
   * copy the string view content to a new string
   */
  String to_string() const;
};
