#pragma once
#include <string>
#include "Node.hpp"
#include "AST_Types.hpp"

namespace llang::ast {
    /**
     * Represents a variable declaration
     **/
    struct VariableDeclNode : public Node {
        std::string Name;
        std::string VarType;
        
        AST_TYPE GetType() const override { 
            return GET_AST_TYPE(VariableDeclNode);
        }

        void ToString(std::string& str, const int tabLevel) const override {
/*
            var tabs = new string('\t', tabLevel);
            string str = tabs + Name + " : " + VarType;
            return ConcatenateChildren(str, tabLevel);
*/
        }
    };
}