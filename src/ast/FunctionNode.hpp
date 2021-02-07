#pragma once
#include <string>
#include <vector>
#include "Node.hpp"
#include "VariableDeclNode.hpp"
#include "StatementNode.hpp"

namespace llang::ast {
    /**
    * Represents a function definition
    **/
    struct FunctionNode : public Node {
        std::string Name;
        std::string ReturnType;
        std::vector<VariableDeclNode> Parameters;
        std::vector<StatementNode> Block;
        
        void ToString(std::string& str, const int tabLevel) const override {
/*
            var tabs = new string('\t', tabLevel);
            string str = tabs + "function: " +  Name + " (";
            
            foreach (ASTVariableDeclNode param in Parameters)
            {
                str += "\n\t" + tabs + param.ToString();
            }
            
            str += "\n" + tabs + ") {";

            foreach (ASTStatementNode param in Block)
            {
                str += "\n\t  " + tabs + param.ToString();
            }

            str += "\n" + tabs + "}";
*/
        }
    };
}