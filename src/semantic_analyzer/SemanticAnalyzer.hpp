#pragma once
#include <vector>
#include <memory>
#include "../ast/ProgramNode.hpp"
#include "../Primitives.hpp"
#include "../built-in/Built-inTypes.hpp"
#include "../error_handling/ErrorDescription.hpp"

namespace llang::ast
{
    struct FunctionDefNode;
}

namespace llang::semantics
{
    struct SemanticAnalyzer {
        using ErrorList = std::vector<error_handling::Error>;
        static std::shared_ptr<ast::ProgramNode> ast;
        static ErrorList *errors;

        SemanticAnalyzer(std::shared_ptr<ast::ProgramNode> ast, ErrorList &errors);

        std::shared_ptr<ast::ProgramNode> check();

    private:
        static bool checkNode(ast::Node::ChildType node);

        static bool checkNode(std::shared_ptr<ast::FunctionDefNode> funcNode);
    };
} // namespace llang::semantics

