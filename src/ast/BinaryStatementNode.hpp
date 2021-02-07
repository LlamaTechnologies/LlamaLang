#pragma once
#include <string>
#include <memory>
#include "StatementNode.hpp"
#include "RightValueNode.hpp"
#include "../CppReflection.hpp"

namespace llang::ast 
{
    /**
    * Represents a binary instruction for example:
    *   - additions
    *   - assignments
    *   - etc
    * Any statement that has two sides involved
    **/
    struct BinaryStatementNode : public RightValueNode
    {
        std::string left; // only identifiers
        std::shared_ptr<StatementNode> right; // any statement

        BinaryStatementNode(STATEMENT_TYPE type)
            : RightValueNode(type)
        {
            AssertEnumValueGraterThan<STATEMENT_TYPE, type>(STATEMENT_TYPE::BINARY_STMNT, "Statement type is not a binary statement");
        }

        void ToString(std::string& str, const int tabLevel) const override {
            /*
            var tabs = new string('\t', tabLevel);
            return tabs + GetType().Name;
            */
        }
    }
}