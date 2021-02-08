#pragma once
#include <string>
#include <vector>
#include "Node.hpp"
#include "AST_Types.hpp"

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
    struct StatementNode : public Node {
        STATEMENT_TYPE StmntType;
        
        
        void ToString(std::string& str, const int tabLevel) const override {}

    protected:
        StatementNode(STATEMENT_TYPE type) : StmntType(type) {}

    };
}