#pragma once
#include "StatementNode.hpp"
#include "VariableDefNode.hpp"

namespace llang::ast
{
    /**
     * Represents a variable reference
    */
    struct VariableRefNode : public StatementNode {
        std::shared_ptr<VariableDefNode> Var;
        bool WasFound;

        VariableRefNode() :
            StatementNode(STATEMENT_TYPE::VAR_REF) {}

        inline AST_TYPE GetType() const override {
            return GET_AST_TYPE(VariableRefNode);
        }

        inline void SetFound(const std::string &varName) {
            if( !Var ) {
                WasFound = false;
                Var = std::make_shared<ast::VariableDefNode>();
                Var->Name = varName;
            } else {
                WasFound = true;
            }
        }

        inline void ToString(std::string &str, const int tabLevel) const override {
            auto tabs = GetTabs(tabLevel);
            str += tabs + Var->Name;
        }
    };
}