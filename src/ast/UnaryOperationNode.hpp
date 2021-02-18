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
        std::shared_ptr<VariableRefNode> Right;
        
        UnaryOperationNode(UNARY_STATEMENT_TYPE unaryOp)
           : StatementNode(STATEMENT_TYPE::UNARY_OP), Op(unaryOp)
        {}
 
        AST_TYPE GetType() const override {
            return GET_AST_TYPE(UnaryOperationNode);
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