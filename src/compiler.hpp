#pragma once
#include <string>

struct AstNode;

namespace compiler {
    void compile(const std::string& in_output_directory, const std::string& in_executable_name, AstNode* in_source_code_node);
}

