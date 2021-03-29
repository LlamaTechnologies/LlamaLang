#pragma once
#include <antlr4-runtime.h>
#include "../ast/Node.hpp"

namespace llang {
    struct LlamaLangParseContext : antlr4::ParserRuleContext {
      ast::Node::ChildType AstNode;

      LlamaLangParseContext(antlr4::ParserRuleContext* parent, size_t invokingState)
          : antlr4::ParserRuleContext(parent, invokingState), AstNode(nullptr) {}
    };
}