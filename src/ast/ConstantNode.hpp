#pragma once
#include <string>
#include <vector>
#include "StatementNode.hpp"
#include "../CppReflection.hpp"
#include "AST_Types.hpp"

namespace llang::ast 
{
    enum class CONSTANT_TYPE
    {
        INTEGER,
        FLOAT,
        CHAR,
        STRING
    };

    /**
     * Represents any compile time constant like the one above
     **/
    struct ConstantNode : public StatementNode
    {
        std::string Value;
        CONSTANT_TYPE ConstType;

        ConstantNode(CONSTANT_TYPE constType)
            : StatementNode(STATEMENT_TYPE::CONSTANT), ConstType(constType)
            {}

        AST_TYPE GetType() const override { return GET_AST_TYPE(ConstantNode); }


        void ToString(std::string& str, const int tabLevel) const override {
            auto tabs = GetTabs(tabLevel);
            auto constType = "INTEGER";
            str += tabs + "%" + constType + " " + Value;
        }
    };
}