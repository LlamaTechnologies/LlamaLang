#pragma once
#include <string>
#include <vector>
#include "antlr/LlamaLangBaseVisitor.h"
#include "error_handling/ErrorDescription.hpp"
#include <memory>

namespace llang
{
    namespace ast
    {
        struct ProgramNode;
    }

    /**
     * Builds an Abstract Syntax Tree (AST) to represent the program and report
     *syntax errors. The AST contains only the information needed to check semantics
     *and generate code.
     **/
    struct AstBuilder : public LlamaLangBaseVisitor {
        std::shared_ptr<ast::ProgramNode> ASTree;
        std::vector<error_handling::Error> Errors;
        const std::string FileName;

        AstBuilder(const std::string &fileName)
            : FileName(fileName), ASTree(std::make_shared<ast::ProgramNode>(fileName)) {
        }

        antlrcpp::Any visitSourceFile(LlamaLangParser::SourceFileContext *context) override;
        antlrcpp::Any visitFunctionDecl(LlamaLangParser::FunctionDeclContext *context);
        antlrcpp::Any visitSignature(LlamaLangParser::SignatureContext *context);
        antlrcpp::Any VisitParameters(LlamaLangParser::ParametersContext *context);
        antlrcpp::Any visitBlock(LlamaLangParser::BlockContext *context);
        antlrcpp::Any visitStatementList(LlamaLangParser::StatementListContext *context) override;
        antlrcpp::Any visitReturnStmt(LlamaLangParser::ReturnStmtContext *context) override;
        antlrcpp::Any visitUnaryExpr(LlamaLangParser::UnaryExprContext *context) override;
        antlrcpp::Any visitExpression(LlamaLangParser::ExpressionContext *context) override;
        antlrcpp::Any visitBasicLit(LlamaLangParser::BasicLitContext *context) override;

        antlrcpp::Any AstBuilder::visitChildren(antlr4::tree::ParseTree *node) override;
        bool shouldVisitNextChild(antlr4::tree::ParseTree *node, const antlrcpp::Any &currentResult);
        antlrcpp::Any& aggregateResult(antlrcpp::Any &result, antlrcpp::Any &nextResult);
    private:
        antlrcpp::Any AstBuilder::visitBlockChildren(LlamaLangParser::BlockContext *node);
    };
} // namespace LlamaLangCompiler
