#pragma once
#include <string>
#include <memory>
#include "StatementNode.hpp"
#include "../CppReflection.hpp"
#include "AST_Types.hpp"

namespace llang::ast 
{
    enum class BINARY_OPERATION {
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        BIT_AND,
        BIT_OR,




        _COUNT
    };

    std::string GetBinaryOperationName(BINARY_OPERATION op);

    enum BINARY_OPERANDS_TYPES {
        INT_INT,
        INT_UINT,
        INT_FLOAT,
        UINT_INT,
        UINT_UINT,
        UINT_FLOAT,
        FLOAT_INT,
        FLOAT_UINT,
        FLOAT_FLOAT
    };

    /**
    * Represents a binary instruction for example:
    *   - additions
    *   - multiplications
    *   - etc
    * Any statement that has two sides involved
    **/
    struct BinaryOperationNode : public StatementNode {
        BINARY_OPERATION Op;
        std::shared_ptr<StatementNode> Left;
        std::shared_ptr<StatementNode> Right;
        std::string                    ResultType;

        BinaryOperationNode(BINARY_OPERATION binOp)
            : StatementNode(STATEMENT_TYPE::BINARY_OP), Op(binOp) {}

        AST_TYPE GetType() const override {
            return GET_AST_TYPE(BinaryOperationNode);
        }

        void ToString(std::string& str, const int tabLevel) const override;
    };
}