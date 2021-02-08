#pragma once
#include <vector>
#include <memory>
#include "../Primitives.hpp"
#include "../ast/ProgramNode.hpp"
#include "../ast/FunctionNode.hpp"
#include "../ast/ConstantNode.hpp"
#include "../ast/UnaryStatementNode.hpp"
#include "../symbol_table/Symbol.hpp"
#include "../built-in/Built-inTypes.hpp"
#include "../error_handling/ErrorDescription.hpp"

namespace llang::semantics {
struct SemanticAnalyzer {
  using ErrorList = std::vector<error_handling::Error>;
  static std::shared_ptr<ast::ProgramNode> ast;
  static ErrorList* errors;

  SemanticAnalyzer(std::shared_ptr<ast::ProgramNode> ast, ErrorList &errors);

  std::shared_ptr<ast::ProgramNode> check() {
    ast->ForEachDeep([](ast::Node::ChildType child) { checkNode(child); });

    return ast;
  }

private:
  static bool checkNode(ast::Node::ChildType node);

  static bool checkNode(std::shared_ptr<ast::FunctionNode> funcNode);
};
} // namespace llang::semantics

