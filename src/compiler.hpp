#pragma once
#include "error.hpp"

#include <string>
#include <vector>

struct AstNode;

namespace compiler {
bool compile(const std::string &in_output_directory, const std::string &in_executable_name,
             AstNode *in_source_code_node, std::vector<Error> &errors);
}
