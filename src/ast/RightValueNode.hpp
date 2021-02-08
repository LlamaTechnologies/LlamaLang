#pragma once
#include "StatementNode.hpp"
#include "AST_Types.hpp"

namespace llang::ast {
    /**
     * Represents any right side expresion
     * - constants
     * - binary expressions
     * - calls
     **/
    struct RightValueNode : public StatementNode
    {
        RightValueNode(STATEMENT_TYPE type)
           : StatementNode(type) {}

         AST_TYPE GetType() const override {
          return GET_AST_TYPE(RightValueNode);
        }
 
        void ToString(std::string& str, const int tabLevel) const override {
/*
            var tabs = new string('\t', tabLevel);
            return tabs + GetType().Name;
*/
        }
    };
}