#pragma once
#include "StatementNode.hpp"

namespace llang::ast
{
    /**
     * Represents a call to a function
    */
    struct FunctionCallNode : public StatementNode {

        FunctionCallNode() :
            StatementNode(STATEMENT_TYPE::CALL) {}

        AST_TYPE GetType() const override {
            return GET_AST_TYPE(FunctionCallNode);
        }

        void ToString(std::string &str, const int tabLevel) const override {

        }
    };
}