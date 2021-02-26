#pragma once
#include <memory>
#include "StatementNode.hpp"
#include "VariableRefNode.hpp"
#include "../CppReflection.hpp"
#include "AST_Types.hpp"

namespace llang::ast {
    enum class UNARY_STATEMENT_TYPE {
        INCREMENT,
        DECREMENT,
        RETURN
    };

    /**
     * Represents
     * - inc/dec
     * - return
     * - etc
     **/
    struct UnaryOperationNode : public StatementNode
    {
        UNARY_STATEMENT_TYPE Op;
        std::shared_ptr<StatementNode> Right;
        
        UnaryOperationNode(UNARY_STATEMENT_TYPE unaryOp)
           : StatementNode(STATEMENT_TYPE::UNARY_OP), Op(unaryOp)
        {}
 
        AST_TYPE GetType() const override {
            return GET_AST_TYPE(UnaryOperationNode);
        }

        void ToString(std::string& str, const int tabLevel) const override {
            auto tabs = GetTabs(tabLevel);
            auto name = "RETURN";
            str += tabs + name + " ";

            if ( Right )
                Right->ToString(str, 0);
        }
    };
}