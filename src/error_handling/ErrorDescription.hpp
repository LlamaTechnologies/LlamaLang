#pragma once
#include <string>

namespace llang::error_handling
{
    struct Error
    {
        int Line;
        int Column;
        std::string FileName;
        std::string Message;

        Error(int line, std::string &file, std::string &message, int column = 0)
        : Line(line), Column(column), FileName(file), Message(message)
        {}

        void ToString(std::string& str, const int tabLevel) const {
        {
            str += FileName + "\t:: line: " + Line + "\t:: col: " + Column + "\t:: " + Message;
        }
    }
}