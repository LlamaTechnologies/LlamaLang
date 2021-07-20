#pragma once
#include <string>
#include <vector>
#include "error.hpp"

struct AstNode;

namespace compiler {
    bool compile(const std::string& in_output_directory, const std::string& in_executable_name, AstNode* in_source_code_node, std::vector<Error>& errors);
}

