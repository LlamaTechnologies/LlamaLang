#pragma once
#include "Node.hpp"
#include "AST_Types.hpp"

namespace llang::ast {

    enum class DIRECTIVE_TYPE {
        MODULE,
        INCLUDE,
        EXTERNAL,
        MAIN
    };

    std::string GetDirectiveTypeName(DIRECTIVE_TYPE type);

    struct DirectiveNode : public Node {
        DIRECTIVE_TYPE Type;

        DirectiveNode(DIRECTIVE_TYPE directiveType)
            : Node(), Type(directiveType) {}

        AST_TYPE GetType() const override {
            return GET_AST_TYPE(DirectiveNode);
        }

        void ToString(std::string& str, const int tabLevel) const override {
            auto tabs = GetTabs(tabLevel);
            str += tabs + '#' + GetDirectiveTypeName(Type);
        }
    };

    struct ExternalDirectiveNode : public DirectiveNode {
        std::string Language;

        ExternalDirectiveNode() : DirectiveNode(DIRECTIVE_TYPE::EXTERNAL) {}

        void ToString(std::string& str, const int tabLevel) const override {
            DirectiveNode::ToString(str, tabLevel);
            str +=  " " + Language;
        }
    };

    struct ModuleDirectiveNode : public DirectiveNode {
        std::string Name;

        ModuleDirectiveNode() : DirectiveNode(DIRECTIVE_TYPE::MODULE) {}

        void ToString(std::string& str, const int tabLevel) const override {
            DirectiveNode::ToString(str, tabLevel);
            str += " " + Name;
        }
    };

    struct IncludeDirectiveNode : public DirectiveNode {
        std::string ModuleDirectory;

        IncludeDirectiveNode() : DirectiveNode(DIRECTIVE_TYPE::INCLUDE) {}

        void ToString(std::string& str, const int tabLevel) const override {
            DirectiveNode::ToString(str, tabLevel);
            str += " " + ModuleDirectory;
        }
    };

}
