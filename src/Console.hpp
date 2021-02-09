#pragma once
#include <iostream>
#include <antlr4-runtime.h>

namespace Console {
void WriteLine() { std::cout << std::endl; }

void WriteLine(const std::string &msg) { std::cout << msg << std::endl; }

void WriteLine(const std::fstream &msg) {
  std::cout << msg.rdbuf() << std::endl;
}

void WriteLine(antlr4::ANTLRInputStream &msg) {
  std::cout << msg.toString() << std::endl;
}

char ReadKey() {
  char key;
  std::cin >> key;
  return key;
}
} // namespace Console