#pragma once
#include <iostream>
#include <antlr4-runtime.h>
#include <string>
#include <fstream>

namespace Console
{
    inline void WriteLine() {
        std::cout << std::endl;
    }

    inline void WriteLine(const std::string &msg) {
        std::cout << msg << std::endl;
    }

    inline bool WriteLine(std::istream &msg) {
        msg.clear();
        msg.seekg(0, std::ios::beg);

        std::string line;
        while( !msg.eof() ) {
            std::getline(msg, line);
            if( msg.bad() ) {
                std::cout << "reading failure!" << std::endl;
                return false;
            }
            std::cout << line << "\n";
        }

        std::cout.flush();
        msg.clear();
        msg.seekg(0, std::ios::beg);
        return true;
    }

    inline char ReadKey() {
        return std::cin.get();
    }
} // namespace Console