#pragma once
#include <string>

namespace llang::error_handling
{
    struct Error
    {
        const long Line;
        const long Column;
        const std::string FileName;
        const std::string Message;

        Error() : Line(0L), Column(0L), FileName(""), Message("") {}

        Error(long line, const std::string &file, const std::string &message, long column = 0)
        : Line(line), Column(column), FileName(file), Message(message)
        {}

        void ToString(std::string &str, const int tabLevel) const {
          str += FileName + "\t:: line: " + std::to_string(Line) +
                 "\t:: col: " + std::to_string(Column) + "\t:: " + Message;
        }
    }; // namespace llang::error_handling
}
