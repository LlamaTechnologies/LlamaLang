
// Generated from ../../LlamaLang.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"
#include "LlamaLangParser.h"


namespace llang {

/**
 * This class defines an abstract visitor for a parse tree
 * produced by LlamaLangParser.
 */
class  LlamaLangVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by LlamaLangParser.
   */
    virtual antlrcpp::Any visitSourceFile(LlamaLangParser::SourceFileContext *context) = 0;

    virtual antlrcpp::Any visitIdentifierList(LlamaLangParser::IdentifierListContext *context) = 0;

    virtual antlrcpp::Any visitExpressionList(LlamaLangParser::ExpressionListContext *context) = 0;

    virtual antlrcpp::Any visitFunctionDef(LlamaLangParser::FunctionDefContext *context) = 0;

    virtual antlrcpp::Any visitVarDef(LlamaLangParser::VarDefContext *context) = 0;

    virtual antlrcpp::Any visitBlock(LlamaLangParser::BlockContext *context) = 0;

    virtual antlrcpp::Any visitStatementList(LlamaLangParser::StatementListContext *context) = 0;

    virtual antlrcpp::Any visitStatement(LlamaLangParser::StatementContext *context) = 0;

    virtual antlrcpp::Any visitSimpleStmt(LlamaLangParser::SimpleStmtContext *context) = 0;

    virtual antlrcpp::Any visitExpressionStmt(LlamaLangParser::ExpressionStmtContext *context) = 0;

    virtual antlrcpp::Any visitAssignment(LlamaLangParser::AssignmentContext *context) = 0;

    virtual antlrcpp::Any visitAssign_op(LlamaLangParser::Assign_opContext *context) = 0;

    virtual antlrcpp::Any visitEmptyStmt(LlamaLangParser::EmptyStmtContext *context) = 0;

    virtual antlrcpp::Any visitReturnStmt(LlamaLangParser::ReturnStmtContext *context) = 0;

    virtual antlrcpp::Any visitType_(LlamaLangParser::Type_Context *context) = 0;

    virtual antlrcpp::Any visitPointer(LlamaLangParser::PointerContext *context) = 0;

    virtual antlrcpp::Any visitArray(LlamaLangParser::ArrayContext *context) = 0;

    virtual antlrcpp::Any visitTypeName(LlamaLangParser::TypeNameContext *context) = 0;

    virtual antlrcpp::Any visitSignature(LlamaLangParser::SignatureContext *context) = 0;

    virtual antlrcpp::Any visitResult(LlamaLangParser::ResultContext *context) = 0;

    virtual antlrcpp::Any visitParameters(LlamaLangParser::ParametersContext *context) = 0;

    virtual antlrcpp::Any visitParameterDecl(LlamaLangParser::ParameterDeclContext *context) = 0;

    virtual antlrcpp::Any visitExpression(LlamaLangParser::ExpressionContext *context) = 0;

    virtual antlrcpp::Any visitPrimaryExpr(LlamaLangParser::PrimaryExprContext *context) = 0;

    virtual antlrcpp::Any visitUnaryExpr(LlamaLangParser::UnaryExprContext *context) = 0;

    virtual antlrcpp::Any visitConversion(LlamaLangParser::ConversionContext *context) = 0;

    virtual antlrcpp::Any visitOperand(LlamaLangParser::OperandContext *context) = 0;

    virtual antlrcpp::Any visitUnaryOp(LlamaLangParser::UnaryOpContext *context) = 0;

    virtual antlrcpp::Any visitLiteral(LlamaLangParser::LiteralContext *context) = 0;

    virtual antlrcpp::Any visitBasicLit(LlamaLangParser::BasicLitContext *context) = 0;

    virtual antlrcpp::Any visitInteger(LlamaLangParser::IntegerContext *context) = 0;

    virtual antlrcpp::Any visitFloatingPoint(LlamaLangParser::FloatingPointContext *context) = 0;

    virtual antlrcpp::Any visitOperandName(LlamaLangParser::OperandNameContext *context) = 0;

    virtual antlrcpp::Any visitQualifiedIdent(LlamaLangParser::QualifiedIdentContext *context) = 0;

    virtual antlrcpp::Any visitLiteralType(LlamaLangParser::LiteralTypeContext *context) = 0;

    virtual antlrcpp::Any visitFieldDecl(LlamaLangParser::FieldDeclContext *context) = 0;

    virtual antlrcpp::Any visitString_(LlamaLangParser::String_Context *context) = 0;

    virtual antlrcpp::Any visitArguments(LlamaLangParser::ArgumentsContext *context) = 0;

    virtual antlrcpp::Any visitMethodExpr(LlamaLangParser::MethodExprContext *context) = 0;

    virtual antlrcpp::Any visitReceiverType(LlamaLangParser::ReceiverTypeContext *context) = 0;

    virtual antlrcpp::Any visitEos(LlamaLangParser::EosContext *context) = 0;


};

}  // namespace llang
