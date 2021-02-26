#pragma once
#include <memory>
#include "ast/ProgramNode.hpp"

namespace llang::IR {
void Translate(std::shared_ptr<ast::ProgramNode> program, const std::string &outputFileName);
}