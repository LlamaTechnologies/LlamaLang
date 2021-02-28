#pragma once
#include <string>
#include "Node.hpp"
#include "AST_Types.hpp"
#include "StatementNode.hpp"

namespace llang::ast {
    struct AssignNode;

    /**
     * Represents a variable/param definition
     **/
    struct VariableDefNode : public StatementNode {
        std::string Name;
        std::string VarType;
        std::shared_ptr<AssignNode> assignmentStmnt;
        
        VariableDefNode() : StatementNode(STATEMENT_TYPE::VARIABLE) {}

        inline AST_TYPE GetType() const override { 
            return GET_AST_TYPE(VariableDefNode);
        }

        void ToString(std::string &str, const int tabLevel) const override;
    };
}