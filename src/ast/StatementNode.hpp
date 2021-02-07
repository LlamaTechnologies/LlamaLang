#pragma once
#include <string>
#include <vector>
#include "Node.hpp"

namespace llang::ast {
    enum class STATEMENT_TYPE
    {
        EMPTY,
        RETURN,
        CONSTANT,   // any constant statement
        BINARY_STMNT,
        ASSIGN,
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        CALL,
    };

    /**
    * Represents an instruction
    **/
    struct StatementNode : public Node, REGISTER_AST_TYPE(StatementNode) {
        STATEMENT_TYPE StmntType;
        
        StatementNode(STATEMENT_TYPE type) : StmntType(type) {}

        void ToString(std::string& str, const int tabLevel) const override {

        }
    };
}