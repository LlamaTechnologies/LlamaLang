#pragma once
#include <string>
#include <vector>
#include "Node.hpp"
#include "AST_Types.hpp"

namespace llang::ast {
    enum class STATEMENT_TYPE
    {
        CONSTANT,   // any constant statement
        BINARY_OP,
        UNARY_OP,
        VAR_REF,
        ASSIGN,
        CALL,
    };

    /**
    * Represents an instruction
    **/
    struct StatementNode : public Node {
        STATEMENT_TYPE StmntType;
        
        AST_TYPE GetType() const override {
            return GET_AST_TYPE(StatementNode);
        }
        
        void ToString(std::string& str, const int tabLevel) const override {}

    protected:
        StatementNode(STATEMENT_TYPE type) : StmntType(type) {}

    };
}