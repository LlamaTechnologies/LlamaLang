#pragma once
#include <iostream>
#include <antlr4-runtime.h>

namespace Console
{
    inline void WriteLine() {
        std::cout << std::endl;
    }

    inline void WriteLine(const std::string &msg) {
        std::cout << msg << std::endl;
    }

    inline void WriteLine(const std::fstream &msg) {
        std::cout << msg.rdbuf() << std::endl;
    }

    inline void WriteLine(antlr4::ANTLRInputStream &msg) {
        std::cout << msg.toString() << std::endl;
    }

    inline char ReadKey() {
        char key;
        std::cin >> key;
        return key;
    }
} // namespace Console