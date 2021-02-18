#pragma once
#include "StatementNode.hpp"
#include "VariableRefNode.hpp"

namespace llang::ast
{
    /**
     * Represents a assignment to a variable
    */
    struct AssignNode : public StatementNode {
        std::shared_ptr<VariableRefNode> Left;
        std::shared_ptr<StatementNode> Right;

        AssignNode() :
            StatementNode(STATEMENT_TYPE::ASSIGN) {}

        AST_TYPE GetType() const override {
            return GET_AST_TYPE(AssignNode);
        }

        void ToString(std::string &str, const int tabLevel) const override {

        }
    };
}