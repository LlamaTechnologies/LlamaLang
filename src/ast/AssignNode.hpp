#pragma once
#include "StatementNode.hpp"

namespace llang::ast
{
    struct VariableRefNode;

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

        void ToString(std::string &str, const int tabLevel) const override;
    };
}