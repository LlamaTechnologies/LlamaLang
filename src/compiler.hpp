#pragma once
#include "error.hpp"
#include "ir.hpp"

#include <string>
#include <vector>

struct AstNode;

namespace compiler {
bool compile(const std::string &in_output_directory, const std::string &in_executable_name,
             const std::string &in_source_code, const std::string &in_source_name);

bool compile_node(GeneratorInterface *in_generator, std::vector<Error> &errors, AstSourceCode *const in_source_code);
} // namespace compiler
