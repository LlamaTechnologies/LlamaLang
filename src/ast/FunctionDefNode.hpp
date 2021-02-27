#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Node.hpp"
#include "VariableDeclNode.hpp"
#include "StatementNode.hpp"
#include "AST_Types.hpp"

namespace llang::ast {
    /**
    * Represents a function definition
    **/
    struct FunctionDefNode : public Node {
        using StatementType = std::shared_ptr<ast::StatementNode>;
        using BlockType = std::vector<StatementType>;

        std::string Name;
        std::string ReturnType;
        std::vector<std::shared_ptr<VariableDeclNode>> Parameters;
        BlockType Block;
        
        AST_TYPE GetType() const override { 
            return GET_AST_TYPE(FunctionDefNode);
        }

        void ToString(std::string& str, const int tabLevel) const override {
            auto tabs = GetTabs(tabLevel);
            str += tabs + GetNodeBeginChar(tabLevel) + GET_TYPE_NAME(FunctionDefNode) + ": " + ReturnType + " :: " + Name + "(";

            const int childTabLevel = tabLevel + 1;
            if( Parameters.size() ) {
                for( auto param : Parameters ) {
                    str += "\n";
                    param->ToString(str, childTabLevel);
                }
                str += "\n" + tabs;
            }
            
            str += ") {";

            for( auto stmnt: Block)
            {
                str += "\n";
                stmnt->ToString(str, childTabLevel);
            }

            str += "\n" + tabs + "}";
        }
    };
}