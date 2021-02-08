#pragma once
#include <string>
#include "Node.hpp"
#include "../CppReflection.hpp"
#include "AST_Types.hpp"

namespace llang::ast {
    /**
     * Represents a whole program.
     * It is the root node of the tree.
     **/
    struct ProgramNode : public Node {
        
        AST_TYPE GetType() const override { 
            return GET_AST_TYPE(ProgramNode);
        }
        
        void ToString(std::string& str, const int tabLevel) const override {
            auto tabs = std::string('\t', tabLevel);
            str += tabs + GET_TYPE_NAME(ProgramNode);
        }
    };
}