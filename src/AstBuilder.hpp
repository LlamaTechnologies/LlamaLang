#pragma once
#include <string>
#include <vector>
#include "antlr/LlamaLangBaseVisitor.h"
#include "ast/Node.hpp"
#include "ast/ProgramNode.hpp"
#include "error_handling/ErrorDescription.hpp"

namespace llang {
    /**
     * Builds an Abstract Syntax Tree (AST) to represent the program and report
     *syntax errors. The AST contains only the information needed to check semantics
     *and generate code.
     **/
    struct AstBuilder : public LlamaLangBaseVisitor {
      std::shared_ptr<ast::ProgramNode> ASTree;
      std::vector<error_handling::Error> Errors;
      std::string FileName;

      AstBuilder(std::string &fileName) : FileName(fileName) {}

      antlrcpp::Any
      visitSourceFile(LlamaLangParser::SourceFileContext *context) override;
      antlrcpp::Any
      visitFunctionDecl(LlamaLangParser::FunctionDeclContext *context);
      antlrcpp::Any visitSignature(LlamaLangParser::SignatureContext *context);
      antlrcpp::Any VisitParameters(LlamaLangParser::ParametersContext *context);
      antlrcpp::Any visitBlock(LlamaLangParser::BlockContext *context);
      antlrcpp::Any
      visitStatementList(LlamaLangParser::StatementListContext *context) override;
      antlrcpp::Any
      visitReturnStmt(LlamaLangParser::ReturnStmtContext *context) override;
      antlrcpp::Any
      visitUnaryExpr(LlamaLangParser::UnaryExprContext *context) override;
      antlrcpp::Any
      visitExpression(LlamaLangParser::ExpressionContext *context) override;
      antlrcpp::Any
      visitBasicLit(LlamaLangParser::BasicLitContext *context) override;
    };
} // namespace LlamaLangCompiler
