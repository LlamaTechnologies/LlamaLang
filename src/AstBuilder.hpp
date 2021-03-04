#pragma once
#include <string>
#include <vector>
#include "antlr/LlamaLangBaseVisitor.h"
#include "error_handling/ErrorDescription.hpp"
#include "symbol_table/SymbolTableScope.hpp"
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
        std::shared_ptr<symbol_table::SymbolTableScope> globalScope;
        std::shared_ptr<symbol_table::SymbolTableScope> currentScope;

        AstBuilder(const std::string &fileName)
            : FileName(fileName), ASTree(std::make_shared<ast::ProgramNode>(fileName)),
            globalScope(std::make_shared<symbol_table::SymbolTableScope>(symbol_table::SCOPE_TYPE::GLOBAL)),
            currentScope(globalScope) {}

        antlrcpp::Any visitSourceFile(LlamaLangParser::SourceFileContext *context) override;
        antlrcpp::Any visitFunctionDef(LlamaLangParser::FunctionDefContext *context);
        antlrcpp::Any visitSignature(LlamaLangParser::SignatureContext *context);
        antlrcpp::Any VisitParameters(LlamaLangParser::ParametersContext *context);
        antlrcpp::Any visitBlock(LlamaLangParser::BlockContext *context);
        antlrcpp::Any visitStatementList(LlamaLangParser::StatementListContext *context) override;
        antlrcpp::Any visitReturnStmt(LlamaLangParser::ReturnStmtContext *context) override;
        antlrcpp::Any visitUnaryExpr(LlamaLangParser::UnaryExprContext *context) override;
        antlrcpp::Any visitExpression(LlamaLangParser::ExpressionContext *context) override;
        antlrcpp::Any visitVarDef(LlamaLangParser::VarDefContext *context) override;
        antlrcpp::Any visitAssignment(LlamaLangParser::AssignmentContext *context) override;
        antlrcpp::Any visitOperandName(LlamaLangParser::OperandNameContext *context) override;
        antlrcpp::Any visitBasicLit(LlamaLangParser::BasicLitContext *context) override;

        antlrcpp::Any AstBuilder::visitChildren(antlr4::tree::ParseTree *node) override;
        bool shouldVisitNextChild(antlr4::tree::ParseTree *node, const antlrcpp::Any &currentResult);
        antlrcpp::Any &aggregateResult(antlrcpp::Any &result, antlrcpp::Any &nextResult);
    };
} // namespace LlamaLangCompiler
