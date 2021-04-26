#pragma once
#include <string>
#include <vector>
#include <memory>
#include "StatementNode.hpp"
#include "../CppReflection.hpp"
#include "../Primitives.hpp"
#include "AST_Types.hpp"
#include "BinaryOperationNode.hpp"

namespace llang::ast
{
    enum class CONSTANT_TYPE {
        I8,
        CHAR = I8,
        I16,
        I32,
        I64,
        U8 = I8,
        U16,
        U32,
        U64,
        FLOAT,
        DOUBLE,
        STRING,

        _COUNT
    };

    std::string GetConstantTypeName(CONSTANT_TYPE type);
    CONSTANT_TYPE GetConstantType(std::string name);
    CONSTANT_TYPE GetConstantType(PRIMITIVE_TYPE primitive);


    /**
     * Represents any compile time constant like the one above
     **/
    struct ConstantNode : public StatementNode {
        std::string Value;
        CONSTANT_TYPE ConstType;

        ConstantNode(CONSTANT_TYPE constType)
            : StatementNode(STATEMENT_TYPE::CONSTANT), ConstType(constType) {}

        AST_TYPE GetType() const override {
            return GET_AST_TYPE(ConstantNode);
        }


        void ToString(std::string &str, const int tabLevel) const override {
            auto tabs = GetTabs(tabLevel);
            auto constTypeName = GetConstantTypeName(ConstType);
            str += tabs + "%" + constTypeName + " " + Value;
        }
    };

    CONSTANT_TYPE GetResultType(BINARY_OPERATION op, std::shared_ptr<ConstantNode> rightNode, std::shared_ptr<ConstantNode> leftNode, bool &checkOverflow);
}
