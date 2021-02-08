
// Generated from c:\Users\narbl\Documents\GitHub\LlamaLang-Cpp\LlamaLang.g4 by ANTLR 4.8

#pragma once

#include "antlr4-runtime.h"

namespace llang
{

  class LlamaLangParser : public antlr4::Parser
  {
  public:
    enum
    {
      T__0 = 1,
      FUNC = 2,
      RETURN = 3,
      IDENTIFIER = 4,
      L_PAREN = 5,
      R_PAREN = 6,
      L_CURLY = 7,
      R_CURLY = 8,
      L_BRACKET = 9,
      R_BRACKET = 10,
      ASSIGN = 11,
      COMMA = 12,
      SEMI = 13,
      COLON = 14,
      DOT = 15,
      PLUS_PLUS = 16,
      MINUS_MINUS = 17,
      DECLARE_ASSIGN = 18,
      ELLIPSIS = 19,
      LOGICAL_OR = 20,
      LOGICAL_AND = 21,
      EQUALS = 22,
      NOT_EQUALS = 23,
      LESS = 24,
      LESS_OR_EQUALS = 25,
      GREATER = 26,
      GREATER_OR_EQUALS = 27,
      OR = 28,
      DIV = 29,
      MOD = 30,
      LSHIFT = 31,
      RSHIFT = 32,
      BIT_CLEAR = 33,
      EXCLAMATION = 34,
      PLUS = 35,
      MINUS = 36,
      CARET = 37,
      STAR = 38,
      AMPERSAND = 39,
      ARROW = 40,
      DECIMAL_LIT = 41,
      OCTAL_LIT = 42,
      HEX_LIT = 43,
      FLOAT_LIT = 44,
      RUNE_LIT = 45,
      INTERPRETED_STRING_LIT = 46,
      WS = 47,
      COMMENT = 48,
      TERMINATOR = 49,
      LINE_COMMENT = 50
    };

    enum
    {
      RuleSourceFile = 0,
      RuleIdentifierList = 1,
      RuleExpressionList = 2,
      RuleFunctionDecl = 3,
      RuleVarDecl = 4,
      RuleVarSpec = 5,
      RuleBlock = 6,
      RuleStatementList = 7,
      RuleStatement = 8,
      RuleSimpleStmt = 9,
      RuleExpressionStmt = 10,
      RuleAssignment = 11,
      RuleAssign_op = 12,
      RuleEmptyStmt = 13,
      RuleReturnStmt = 14,
      RuleType_ = 15,
      RulePointer = 16,
      RuleArray = 17,
      RuleTypeName = 18,
      RuleSignature = 19,
      RuleResult = 20,
      RuleParameters = 21,
      RuleParameterDecl = 22,
      RuleExpression = 23,
      RulePrimaryExpr = 24,
      RuleUnaryExpr = 25,
      RuleConversion = 26,
      RuleOperand = 27,
      RuleUnaryOp = 28,
      RuleLiteral = 29,
      RuleBasicLit = 30,
      RuleInteger = 31,
      RuleOperandName = 32,
      RuleQualifiedIdent = 33,
      RuleLiteralType = 34,
      RuleFieldDecl = 35,
      RuleString_ = 36,
      RuleArguments = 37,
      RuleMethodExpr = 38,
      RuleReceiverType = 39,
      RuleEos = 40
    };

    LlamaLangParser(antlr4::TokenStream *input);
    ~LlamaLangParser();

    virtual std::string getGrammarFileName() const override;
    virtual const antlr4::atn::ATN &getATN() const override { return _atn; };
    virtual const std::vector<std::string> &getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
    virtual const std::vector<std::string> &getRuleNames() const override;
    virtual antlr4::dfa::Vocabulary &getVocabulary() const override;

    class SourceFileContext;
    class IdentifierListContext;
    class ExpressionListContext;
    class FunctionDeclContext;
    class VarDeclContext;
    class VarSpecContext;
    class BlockContext;
    class StatementListContext;
    class StatementContext;
    class SimpleStmtContext;
    class ExpressionStmtContext;
    class AssignmentContext;
    class Assign_opContext;
    class EmptyStmtContext;
    class ReturnStmtContext;
    class Type_Context;
    class PointerContext;
    class ArrayContext;
    class TypeNameContext;
    class SignatureContext;
    class ResultContext;
    class ParametersContext;
    class ParameterDeclContext;
    class ExpressionContext;
    class PrimaryExprContext;
    class UnaryExprContext;
    class ConversionContext;
    class OperandContext;
    class UnaryOpContext;
    class LiteralContext;
    class BasicLitContext;
    class IntegerContext;
    class OperandNameContext;
    class QualifiedIdentContext;
    class LiteralTypeContext;
    class FieldDeclContext;
    class String_Context;
    class ArgumentsContext;
    class MethodExprContext;
    class ReceiverTypeContext;
    class EosContext;

    class SourceFileContext : public antlr4::ParserRuleContext
    {
    public:
      SourceFileContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      EosContext *eos();
      std::vector<FunctionDeclContext *> functionDecl();
      FunctionDeclContext *functionDecl(size_t i);
      std::vector<VarDeclContext *> varDecl();
      VarDeclContext *varDecl(size_t i);

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    SourceFileContext *sourceFile();

    class IdentifierListContext : public antlr4::ParserRuleContext
    {
    public:
      IdentifierListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
      antlr4::tree::TerminalNode *IDENTIFIER(size_t i);
      std::vector<antlr4::tree::TerminalNode *> COMMA();
      antlr4::tree::TerminalNode *COMMA(size_t i);

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    IdentifierListContext *identifierList();

    class ExpressionListContext : public antlr4::ParserRuleContext
    {
    public:
      ExpressionListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      std::vector<ExpressionContext *> expression();
      ExpressionContext *expression(size_t i);
      std::vector<antlr4::tree::TerminalNode *> COMMA();
      antlr4::tree::TerminalNode *COMMA(size_t i);

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    ExpressionListContext *expressionList();

    class FunctionDeclContext : public antlr4::ParserRuleContext
    {
    public:
      FunctionDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      antlr4::tree::TerminalNode *FUNC();
      antlr4::tree::TerminalNode *IDENTIFIER();
      SignatureContext *signature();
      antlr4::tree::TerminalNode *ARROW();
      Type_Context *type_();
      BlockContext *block();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    FunctionDeclContext *functionDecl();

    class VarDeclContext : public antlr4::ParserRuleContext
    {
    public:
      VarDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      VarSpecContext *varSpec();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    VarDeclContext *varDecl();

    class VarSpecContext : public antlr4::ParserRuleContext
    {
    public:
      VarSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      IdentifierListContext *identifierList();
      antlr4::tree::TerminalNode *COLON();
      Type_Context *type_();
      EosContext *eos();
      antlr4::tree::TerminalNode *ASSIGN();
      ExpressionListContext *expressionList();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    VarSpecContext *varSpec();

    class BlockContext : public antlr4::ParserRuleContext
    {
    public:
      BlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      antlr4::tree::TerminalNode *L_CURLY();
      antlr4::tree::TerminalNode *R_CURLY();
      StatementListContext *statementList();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    BlockContext *block();

    class StatementListContext : public antlr4::ParserRuleContext
    {
    public:
      StatementListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      std::vector<StatementContext *> statement();
      StatementContext *statement(size_t i);
      std::vector<EosContext *> eos();
      EosContext *eos(size_t i);

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    StatementListContext *statementList();

    class StatementContext : public antlr4::ParserRuleContext
    {
    public:
      StatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      VarDeclContext *varDecl();
      SimpleStmtContext *simpleStmt();
      ReturnStmtContext *returnStmt();
      BlockContext *block();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    StatementContext *statement();

    class SimpleStmtContext : public antlr4::ParserRuleContext
    {
    public:
      SimpleStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      ExpressionStmtContext *expressionStmt();
      AssignmentContext *assignment();
      EmptyStmtContext *emptyStmt();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    SimpleStmtContext *simpleStmt();

    class ExpressionStmtContext : public antlr4::ParserRuleContext
    {
    public:
      ExpressionStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      ExpressionContext *expression();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    ExpressionStmtContext *expressionStmt();

    class AssignmentContext : public antlr4::ParserRuleContext
    {
    public:
      AssignmentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      std::vector<ExpressionListContext *> expressionList();
      ExpressionListContext *expressionList(size_t i);
      Assign_opContext *assign_op();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    AssignmentContext *assignment();

    class Assign_opContext : public antlr4::ParserRuleContext
    {
    public:
      Assign_opContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      antlr4::tree::TerminalNode *ASSIGN();
      antlr4::tree::TerminalNode *PLUS();
      antlr4::tree::TerminalNode *MINUS();
      antlr4::tree::TerminalNode *OR();
      antlr4::tree::TerminalNode *CARET();
      antlr4::tree::TerminalNode *STAR();
      antlr4::tree::TerminalNode *DIV();
      antlr4::tree::TerminalNode *MOD();
      antlr4::tree::TerminalNode *LSHIFT();
      antlr4::tree::TerminalNode *RSHIFT();
      antlr4::tree::TerminalNode *AMPERSAND();
      antlr4::tree::TerminalNode *BIT_CLEAR();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    Assign_opContext *assign_op();

    class EmptyStmtContext : public antlr4::ParserRuleContext
    {
    public:
      EmptyStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      antlr4::tree::TerminalNode *SEMI();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    EmptyStmtContext *emptyStmt();

    class ReturnStmtContext : public antlr4::ParserRuleContext
    {
    public:
      ReturnStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      antlr4::tree::TerminalNode *RETURN();
      ExpressionContext *expression();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    ReturnStmtContext *returnStmt();

    class Type_Context : public antlr4::ParserRuleContext
    {
    public:
      Type_Context(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      TypeNameContext *typeName();
      PointerContext *pointer();
      ArrayContext *array();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    Type_Context *type_();

    class PointerContext : public antlr4::ParserRuleContext
    {
    public:
      PointerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      antlr4::tree::TerminalNode *STAR();
      Type_Context *type_();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    PointerContext *pointer();

    class ArrayContext : public antlr4::ParserRuleContext
    {
    public:
      ArrayContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      antlr4::tree::TerminalNode *L_BRACKET();
      antlr4::tree::TerminalNode *R_BRACKET();
      TypeNameContext *typeName();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    ArrayContext *array();

    class TypeNameContext : public antlr4::ParserRuleContext
    {
    public:
      TypeNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      antlr4::tree::TerminalNode *IDENTIFIER();
      QualifiedIdentContext *qualifiedIdent();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    TypeNameContext *typeName();

    class SignatureContext : public antlr4::ParserRuleContext
    {
    public:
      SignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      ParametersContext *parameters();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    SignatureContext *signature();

    class ResultContext : public antlr4::ParserRuleContext
    {
    public:
      ResultContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      ParametersContext *parameters();
      Type_Context *type_();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    ResultContext *result();

    class ParametersContext : public antlr4::ParserRuleContext
    {
    public:
      ParametersContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      antlr4::tree::TerminalNode *L_PAREN();
      antlr4::tree::TerminalNode *R_PAREN();
      std::vector<ParameterDeclContext *> parameterDecl();
      ParameterDeclContext *parameterDecl(size_t i);
      std::vector<antlr4::tree::TerminalNode *> COMMA();
      antlr4::tree::TerminalNode *COMMA(size_t i);

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    ParametersContext *parameters();

    class ParameterDeclContext : public antlr4::ParserRuleContext
    {
    public:
      ParameterDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      antlr4::tree::TerminalNode *IDENTIFIER();
      antlr4::tree::TerminalNode *COLON();
      Type_Context *type_();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    ParameterDeclContext *parameterDecl();

    class ExpressionContext : public antlr4::ParserRuleContext
    {
    public:
      ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      PrimaryExprContext *primaryExpr();
      UnaryExprContext *unaryExpr();
      std::vector<ExpressionContext *> expression();
      ExpressionContext *expression(size_t i);
      antlr4::tree::TerminalNode *STAR();
      antlr4::tree::TerminalNode *DIV();
      antlr4::tree::TerminalNode *MOD();
      antlr4::tree::TerminalNode *LSHIFT();
      antlr4::tree::TerminalNode *RSHIFT();
      antlr4::tree::TerminalNode *AMPERSAND();
      antlr4::tree::TerminalNode *BIT_CLEAR();
      antlr4::tree::TerminalNode *PLUS();
      antlr4::tree::TerminalNode *MINUS();
      antlr4::tree::TerminalNode *OR();
      antlr4::tree::TerminalNode *CARET();
      antlr4::tree::TerminalNode *EQUALS();
      antlr4::tree::TerminalNode *NOT_EQUALS();
      antlr4::tree::TerminalNode *LESS();
      antlr4::tree::TerminalNode *LESS_OR_EQUALS();
      antlr4::tree::TerminalNode *GREATER();
      antlr4::tree::TerminalNode *GREATER_OR_EQUALS();
      antlr4::tree::TerminalNode *LOGICAL_AND();
      antlr4::tree::TerminalNode *LOGICAL_OR();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    ExpressionContext *expression();
    ExpressionContext *expression(int precedence);
    class PrimaryExprContext : public antlr4::ParserRuleContext
    {
    public:
      PrimaryExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      OperandContext *operand();
      ConversionContext *conversion();
      PrimaryExprContext *primaryExpr();
      antlr4::tree::TerminalNode *DOT();
      antlr4::tree::TerminalNode *IDENTIFIER();
      ArgumentsContext *arguments();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    PrimaryExprContext *primaryExpr();
    PrimaryExprContext *primaryExpr(int precedence);
    class UnaryExprContext : public antlr4::ParserRuleContext
    {
    public:
      UnaryExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      PrimaryExprContext *primaryExpr();
      UnaryOpContext *unaryOp();
      ExpressionContext *expression();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    UnaryExprContext *unaryExpr();

    class ConversionContext : public antlr4::ParserRuleContext
    {
    public:
      ConversionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      Type_Context *type_();
      antlr4::tree::TerminalNode *L_PAREN();
      ExpressionContext *expression();
      antlr4::tree::TerminalNode *R_PAREN();
      antlr4::tree::TerminalNode *COMMA();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    ConversionContext *conversion();

    class OperandContext : public antlr4::ParserRuleContext
    {
    public:
      OperandContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      LiteralContext *literal();
      OperandNameContext *operandName();
      MethodExprContext *methodExpr();
      antlr4::tree::TerminalNode *L_PAREN();
      ExpressionContext *expression();
      antlr4::tree::TerminalNode *R_PAREN();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    OperandContext *operand();

    class UnaryOpContext : public antlr4::ParserRuleContext
    {
    public:
      UnaryOpContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      antlr4::tree::TerminalNode *PLUS();
      antlr4::tree::TerminalNode *MINUS();
      antlr4::tree::TerminalNode *EXCLAMATION();
      antlr4::tree::TerminalNode *CARET();
      antlr4::tree::TerminalNode *STAR();
      antlr4::tree::TerminalNode *AMPERSAND();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    UnaryOpContext *unaryOp();

    class LiteralContext : public antlr4::ParserRuleContext
    {
    public:
      LiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      BasicLitContext *basicLit();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    LiteralContext *literal();

    class BasicLitContext : public antlr4::ParserRuleContext
    {
    public:
      BasicLitContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      IntegerContext *integer();
      String_Context *string_();
      antlr4::tree::TerminalNode *FLOAT_LIT();
      antlr4::tree::TerminalNode *RUNE_LIT();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    BasicLitContext *basicLit();

    class IntegerContext : public antlr4::ParserRuleContext
    {
    public:
      IntegerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      antlr4::tree::TerminalNode *DECIMAL_LIT();
      antlr4::tree::TerminalNode *OCTAL_LIT();
      antlr4::tree::TerminalNode *HEX_LIT();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    IntegerContext *integer();

    class OperandNameContext : public antlr4::ParserRuleContext
    {
    public:
      OperandNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      antlr4::tree::TerminalNode *IDENTIFIER();
      QualifiedIdentContext *qualifiedIdent();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    OperandNameContext *operandName();

    class QualifiedIdentContext : public antlr4::ParserRuleContext
    {
    public:
      QualifiedIdentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
      antlr4::tree::TerminalNode *IDENTIFIER(size_t i);
      antlr4::tree::TerminalNode *DOT();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    QualifiedIdentContext *qualifiedIdent();

    class LiteralTypeContext : public antlr4::ParserRuleContext
    {
    public:
      LiteralTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      TypeNameContext *typeName();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    LiteralTypeContext *literalType();

    class FieldDeclContext : public antlr4::ParserRuleContext
    {
    public:
      FieldDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      IdentifierListContext *identifierList();
      Type_Context *type_();
      String_Context *string_();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    FieldDeclContext *fieldDecl();

    class String_Context : public antlr4::ParserRuleContext
    {
    public:
      String_Context(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      antlr4::tree::TerminalNode *INTERPRETED_STRING_LIT();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    String_Context *string_();

    class ArgumentsContext : public antlr4::ParserRuleContext
    {
    public:
      ArgumentsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      antlr4::tree::TerminalNode *L_PAREN();
      antlr4::tree::TerminalNode *R_PAREN();
      ExpressionListContext *expressionList();
      Type_Context *type_();
      antlr4::tree::TerminalNode *ELLIPSIS();
      std::vector<antlr4::tree::TerminalNode *> COMMA();
      antlr4::tree::TerminalNode *COMMA(size_t i);

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    ArgumentsContext *arguments();

    class MethodExprContext : public antlr4::ParserRuleContext
    {
    public:
      MethodExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      ReceiverTypeContext *receiverType();
      antlr4::tree::TerminalNode *DOT();
      antlr4::tree::TerminalNode *IDENTIFIER();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    MethodExprContext *methodExpr();

    class ReceiverTypeContext : public antlr4::ParserRuleContext
    {
    public:
      ReceiverTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      TypeNameContext *typeName();
      antlr4::tree::TerminalNode *L_PAREN();
      antlr4::tree::TerminalNode *R_PAREN();
      antlr4::tree::TerminalNode *STAR();
      ReceiverTypeContext *receiverType();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    ReceiverTypeContext *receiverType();

    class EosContext : public antlr4::ParserRuleContext
    {
    public:
      EosContext(antlr4::ParserRuleContext *parent, size_t invokingState);
      virtual size_t getRuleIndex() const override;
      antlr4::tree::TerminalNode *SEMI();
      antlr4::tree::TerminalNode *EOF();

      virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
    };

    EosContext *eos();

    virtual bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;
    bool expressionSempred(ExpressionContext *_localctx, size_t predicateIndex);
    bool primaryExprSempred(PrimaryExprContext *_localctx, size_t predicateIndex);

  private:
    static std::vector<antlr4::dfa::DFA> _decisionToDFA;
    static antlr4::atn::PredictionContextCache _sharedContextCache;
    static std::vector<std::string> _ruleNames;
    static std::vector<std::string> _tokenNames;

    static std::vector<std::string> _literalNames;
    static std::vector<std::string> _symbolicNames;
    static antlr4::dfa::Vocabulary _vocabulary;
    static antlr4::atn::ATN _atn;
    static std::vector<uint16_t> _serializedATN;

    struct Initializer
    {
      Initializer();
    };
    static Initializer _init;
  };
} // namespace llang
