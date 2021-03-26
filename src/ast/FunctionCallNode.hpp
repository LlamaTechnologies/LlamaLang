#pragma once
#include "StatementNode.hpp"

namespace llang::ast
{
    /**
     * Represents a call to a function
    */
    struct FunctionCallNode : public StatementNode {
        std::string Name;
        std::vector<std::shared_ptr<StatementNode>> Parameters;
        std::shared_ptr<FunctionDefNode> functionFound;

        FunctionCallNode() :
            StatementNode(STATEMENT_TYPE::CALL) {}

        AST_TYPE GetType() const override {
            return GET_AST_TYPE(FunctionCallNode);
        }

        void ToString(std::string &str, const int tabLevel) const override {
            auto tabs = GetTabs(tabLevel);
            str += tabs + Name + "(";

            // print parameters
            size_t i = 0;
            for (auto param : Parameters) {
                param->ToString(str, 0);
                if (++i != Parameters.size())
                    str += ", ";
            }

            str += ")";
        }
    };
}