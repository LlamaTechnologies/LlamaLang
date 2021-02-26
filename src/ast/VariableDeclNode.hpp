#pragma once
#include <string>
#include "Node.hpp"
#include "AST_Types.hpp"

namespace llang::ast {
    /**
     * Represents a variable/param declaration
     **/
    struct VariableDeclNode : public Node {
        std::string Name;
        std::string VarType;
        
        AST_TYPE GetType() const override { 
            return GET_AST_TYPE(VariableDeclNode);
        }

        void ToString(std::string& str, const int tabLevel) const override {
            auto tabs = GetTabs(tabLevel);
            str += tabs + Name + " : " + VarType;
        }
    };
}