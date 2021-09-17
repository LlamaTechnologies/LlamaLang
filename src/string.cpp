#include "string.hpp"

#include <immintrin.h>
#include <stdlib.h>
#include <string.h>

String::String() : buf_size(1), data_size(0) { buffer = (char *)calloc(0, 1L); }

String::String(const size_t in_buf_size) : buf_size(1), data_size(0) { buffer = (char *)calloc(0, in_buf_size); }

String::String(const char *in_cstr) {
  _calculate_sizes(in_cstr);
  buffer = new char[buf_size];
  memcpy(buffer, in_cstr, data_size);
}

String::String(char *in_cstr) {
  _calculate_sizes(in_cstr);
  buffer = in_cstr;
}

String::String(const StringView in_view) {
  data_size = in_view.len;
  buf_size = data_size + 1;
  memcpy(buffer, in_view.buffer, data_size);
  buffer[data_size] = 0;
}

void String::_calculate_sizes(const char *in_cstr) noexcept {
  data_size = strlen(in_cstr);
  buf_size = data_size + 1;
}

const char *String::c_str() const { return buffer; }
size_t String::size() const { return data_size; }
size_t String::buffer_size() const { return buf_size; }

s32 String::to_int32(const int in_base) const { return strtol(buffer, nullptr, in_base); }
u32 String::to_uint32(const int in_base) const { return strtoul(buffer, nullptr, in_base); }

s64 String::to_int64(const int in_base) const { return strtoll(buffer, nullptr, in_base); }
u64 String::to_uint64(const int in_base) const { return strtoll(buffer, nullptr, in_base); }

f32 String::to_float32() const { return strtof32(buffer, nullptr); }
f64 String::to_float64() const { return strtof64(buffer, nullptr); }

String::~String() { delete[] buffer; }

StringView::StringView(const char *in_cstr, const size_t in_offset, const size_t in_len)
    : buffer(in_cstr + in_offset), len(in_len) {}

String StringView::to_string() const { return String(*this); }

size_t StringView::size() const { return len; }
