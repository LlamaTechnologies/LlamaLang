
// Generated from ../../LlamaLang.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"
#include "LlamaLangVisitor.h"


namespace llang {

/**
 * This class provides an empty implementation of LlamaLangVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  LlamaLangBaseVisitor : public LlamaLangVisitor {
public:

  virtual antlrcpp::Any visitSourceFile(LlamaLangParser::SourceFileContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIdentifierList(LlamaLangParser::IdentifierListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExpressionList(LlamaLangParser::ExpressionListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitModuleDirective(LlamaLangParser::ModuleDirectiveContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIncludeDirective(LlamaLangParser::IncludeDirectiveContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExternDirective(LlamaLangParser::ExternDirectiveContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRunDirective(LlamaLangParser::RunDirectiveContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBasicDirective(LlamaLangParser::BasicDirectiveContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunctionDef(LlamaLangParser::FunctionDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVarDef(LlamaLangParser::VarDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBlock(LlamaLangParser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStatementList(LlamaLangParser::StatementListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStatement(LlamaLangParser::StatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSimpleStmt(LlamaLangParser::SimpleStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExpressionStmt(LlamaLangParser::ExpressionStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssignment(LlamaLangParser::AssignmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssign_op(LlamaLangParser::Assign_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEmptyStmt(LlamaLangParser::EmptyStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReturnStmt(LlamaLangParser::ReturnStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitType_(LlamaLangParser::Type_Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPointer(LlamaLangParser::PointerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitArray(LlamaLangParser::ArrayContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTypeName(LlamaLangParser::TypeNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSignature(LlamaLangParser::SignatureContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitResult(LlamaLangParser::ResultContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParameters(LlamaLangParser::ParametersContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParameterDecl(LlamaLangParser::ParameterDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExpression(LlamaLangParser::ExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPrimaryExpr(LlamaLangParser::PrimaryExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnaryExpr(LlamaLangParser::UnaryExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOperand(LlamaLangParser::OperandContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnaryOp(LlamaLangParser::UnaryOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLiteral(LlamaLangParser::LiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBasicLit(LlamaLangParser::BasicLitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInteger(LlamaLangParser::IntegerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFloatingPoint(LlamaLangParser::FloatingPointContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOperandName(LlamaLangParser::OperandNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitQualifiedIdent(LlamaLangParser::QualifiedIdentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLiteralType(LlamaLangParser::LiteralTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFieldDecl(LlamaLangParser::FieldDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitString_(LlamaLangParser::String_Context *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitArguments(LlamaLangParser::ArgumentsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMethodExpr(LlamaLangParser::MethodExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitReceiverType(LlamaLangParser::ReceiverTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEos(LlamaLangParser::EosContext *ctx) override {
    return visitChildren(ctx);
  }


};

}  // namespace llang
