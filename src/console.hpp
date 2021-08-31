#pragma once
#include <fstream>
#include <iostream>
#include <string>

struct Token;

namespace console {
inline void write_line() { std::cout << std::endl; }

inline void write_line(const std::string &msg) { std::cout << msg << std::endl; }

inline bool write_line(std::istream &msg) {
  msg.clear();
  msg.seekg(0, std::ios::beg);

  std::string line;
  while (!msg.eof()) {
    std::getline(msg, line);
    if (msg.bad()) {
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

inline char read_key() { return std::cin.get(); }

} // namespace console