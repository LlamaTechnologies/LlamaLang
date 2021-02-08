#pragma once

#define SET_VALUE(value) value

namespace llang::ast {
    struct ProgramNode;
    struct RightValueNode;
    struct BinaryStatementNode;
    struct ConstantNode;
    struct FunctionNode; 
    struct StatementNode;
    struct UnaryStatementNode;
    struct VariableDeclNode;

    enum class AST_TYPE {
        SET_VALUE(ProgramNode),
        SET_VALUE(RightValueNode),
        SET_VALUE(BinaryStatementNode),
        SET_VALUE(ConstantNode),
        SET_VALUE(FunctionNode),
        SET_VALUE(StatementNode),
        SET_VALUE(UnaryStatementNode),
        SET_VALUE(VariableDeclNode)
    };
}

#undef SET_VALUE
