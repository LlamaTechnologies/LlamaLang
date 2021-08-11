#pragma once
#include <string>

enum class ERROR_TYPE
{
  WARNING_0,
  WARNING_1,
  WARNING_2,
  ERROR,
};

std::string get_error_type_string(const ERROR_TYPE error_type);

struct Error {
  const ERROR_TYPE type;
  const size_t line;
  const size_t column;
  const std::string file_name;
  const std::string message;

  Error() : type(ERROR_TYPE::ERROR), line(0L), column(0L), file_name(""), message("") {}

  Error(ERROR_TYPE type, const size_t line, const size_t column, const std::string &file, const std::string &message)
      : type(type), line(line), column(column), file_name(file), message(message) {}
};

void to_string(const Error &error, std::string &str);
