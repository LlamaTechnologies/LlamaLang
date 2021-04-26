#pragma once
#include "StatementNode.hpp"
#include "../Primitives.hpp"

namespace llang::ast {
    struct CastOperationNode : public StatementNode {
        PRIMITIVE_TYPE TypeTo;
        PRIMITIVE_TYPE TypeFrom;
        std::shared_ptr<StatementNode> Value;

        CastOperationNode() : StatementNode(STATEMENT_TYPE::CAST) {}

        AST_TYPE GetType() const override {
            return GET_AST_TYPE(CastOperationNode);
        }

        void ToString(std::string& str, const int tabLevel) const override {
            auto tabs = GetTabs(tabLevel);
            str += tabs + "cast(" + Primitives::GetName(TypeTo) + ", " + Primitives::GetName(TypeFrom) + ", ";
            Value->ToString(str, 0);
            str += ")";
        }
    };
}


