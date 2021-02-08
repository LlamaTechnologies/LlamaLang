#pragma once
#include <memory>
#include "StatementNode.hpp"
#include "RightValueNode.hpp"
#include "../CppReflection.hpp"
#include "AST_Types.hpp"

namespace llang::ast {
    /**
     * Represents any right side expresion
     * - constants
     * - binary expressions
     * - calls
     **/
    struct UnaryStatementNode : public StatementNode
    {
        std::shared_ptr<RightValueNode> Right;
        
        UnaryStatementNode(STATEMENT_TYPE type)
           : StatementNode(type) {
               AssertEnumValueLessThan<STATEMENT_TYPE>(type, STATEMENT_TYPE::BINARY_STMNT, "Statement type is not a unary statement");
           }
 
        AST_TYPE GetType() const override {
             return GET_AST_TYPE(UnaryStatementNode);
           }

        void ToString(std::string& str, const int tabLevel) const override {
/*
            var tabs = new string('\t', tabLevel);
            string name = Enum.GetName(typeof(STATEMENT_TYPE), StmntType);
            string str = tabs + name + " " + Right?.ToString();
            return ConcatenateChildren(str, tabLevel);
*/
        }
    };
}