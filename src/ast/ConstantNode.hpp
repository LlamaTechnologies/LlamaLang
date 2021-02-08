#pragma once
#include <string>
#include <vector>
#include "RightValueNode.hpp"
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
    struct ConstantNode : public RightValueNode
    {
        std::string Value;
        CONSTANT_TYPE ConstType;

        ConstantNode(CONSTANT_TYPE constType)
            : RightValueNode(STATEMENT_TYPE::CONSTANT), ConstType(constType)
            {}

        AST_TYPE GetType() const override { return GET_AST_TYPE(ConstantNode); }


        void ToString(std::string& str, const int tabLevel) const override {
            /*
            var tabs = new string('\t', tabLevel);
            string typeName = Enum.GetName(typeof(CONSTANT_TYPE), ConstType);
            return tabs + "%" + typeName + " " + Value;
            */
        }
    };
}