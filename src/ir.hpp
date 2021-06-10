#pragma once
#include <string>
//#include llvm.hpp


/*
* Translates the AST to LLVM intermediate representation
*/
class LlvmIrGenerator {

public:
    LlvmIrGenerator(const std::string& _output_directory, const std::string& _executable_name);
    void generate();
};
