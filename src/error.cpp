#include "error.hpp"

#include "common_defs.hpp"

static const char *error_type_strings[] = { "WARNING", "ERROR" };

std::string get_error_type_string(const ERROR_TYPE error_type) { return error_type_strings[(size_t)error_type]; }

void to_string(const Error &error, std::string &str) {
  str += "[" + get_error_type_string(error.type) + "]" + error.file_name + "\t:: line: " + std::to_string(error.line) +
         "\t:: col: " + std::to_string(error.column) + "\t:: " + error.message;
}