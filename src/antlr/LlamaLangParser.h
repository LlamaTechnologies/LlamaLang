
// Generated from ../../LlamaLang.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"
#include "LlamaLangParseContext.hpp"

namespace llang {


class  LlamaLangParser : public antlr4::Parser {
public:
  enum {
    FUNC = 1, RETURN = 2, IDENTIFIER = 3, L_PAREN = 4, R_PAREN = 5, L_CURLY = 6, 
    R_CURLY = 7, L_BRACKET = 8, R_BRACKET = 9, ASSIGN = 10, COMMA = 11, 
    SEMI = 12, COLON = 13, DOT = 14, PLUS_PLUS = 15, MINUS_MINUS = 16, DECLARE_ASSIGN = 17, 
    ELLIPSIS = 18, LOGICAL_OR = 19, LOGICAL_AND = 20, EQUALS = 21, NOT_EQUALS = 22, 
    LESS = 23, LESS_OR_EQUALS = 24, GREATER = 25, GREATER_OR_EQUALS = 26, 
    OR = 27, DIV = 28, MOD = 29, LSHIFT = 30, RSHIFT = 31, BIT_CLEAR = 32, 
    EXCLAMATION = 33, PLUS = 34, MINUS = 35, CARET = 36, STAR = 37, AMPERSAND = 38, 
    ARROW = 39, DECIMAL_LIT = 40, OCTAL_LIT = 41, HEX_LIT = 42, FLOAT_LIT = 43, 
    DOUBLE_LIT = 44, RUNE_LIT = 45, INTERPRETED_STRING_LIT = 46, WS = 47, 
    COMMENT = 48, TERMINATOR = 49, LINE_COMMENT = 50
  };

  enum {
    RuleSourceFile = 0, RuleIdentifierList = 1, RuleExpressionList = 2, 
    RuleFunctionDef = 3, RuleVarDef = 4, RuleBlock = 5, RuleStatementList = 6, 
    RuleStatement = 7, RuleSimpleStmt = 8, RuleExpressionStmt = 9, RuleAssignment = 10, 
    RuleAssign_op = 11, RuleEmptyStmt = 12, RuleReturnStmt = 13, RuleType_ = 14, 
    RulePointer = 15, RuleArray = 16, RuleTypeName = 17, RuleSignature = 18, 
    RuleResult = 19, RuleParameters = 20, RuleParameterDecl = 21, RuleExpression = 22, 
    RulePrimaryExpr = 23, RuleUnaryExpr = 24, RuleConversion = 25, RuleOperand = 26, 
    RuleUnaryOp = 27, RuleLiteral = 28, RuleBasicLit = 29, RuleInteger = 30, 
    RuleFloatingPoint = 31, RuleOperandName = 32, RuleQualifiedIdent = 33, 
    RuleLiteralType = 34, RuleFieldDecl = 35, RuleString_ = 36, RuleArguments = 37, 
    RuleMethodExpr = 38, RuleReceiverType = 39, RuleEos = 40
  };

  explicit LlamaLangParser(antlr4::TokenStream *input);
  ~LlamaLangParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class SourceFileContext;
  class IdentifierListContext;
  class ExpressionListContext;
  class FunctionDefContext;
  class VarDefContext;
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
  class FloatingPointContext;
  class OperandNameContext;
  class QualifiedIdentContext;
  class LiteralTypeContext;
  class FieldDeclContext;
  class String_Context;
  class ArgumentsContext;
  class MethodExprContext;
  class ReceiverTypeContext;
  class EosContext; 

  class  SourceFileContext : public LlamaLangParseContext {
  public:
    SourceFileContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<EosContext *> eos();
    EosContext* eos(size_t i);
    std::vector<FunctionDefContext *> functionDef();
    FunctionDefContext* functionDef(size_t i);
    std::vector<VarDefContext *> varDef();
    VarDefContext* varDef(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SourceFileContext* sourceFile();

  class  IdentifierListContext : public LlamaLangParseContext {
  public:
    IdentifierListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IdentifierListContext* identifierList();

  class  ExpressionListContext : public LlamaLangParseContext {
  public:
    ExpressionListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExpressionListContext* expressionList();

  class  FunctionDefContext : public LlamaLangParseContext {
  public:
    FunctionDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FUNC();
    antlr4::tree::TerminalNode *IDENTIFIER();
    SignatureContext *signature();
    antlr4::tree::TerminalNode *ARROW();
    Type_Context *type_();
    BlockContext *block();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionDefContext* functionDef();

  class  VarDefContext : public LlamaLangParseContext {
  public:
    VarDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *COLON();
    Type_Context *type_();
    antlr4::tree::TerminalNode *ASSIGN();
    ExpressionListContext *expressionList();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VarDefContext* varDef();

  class  BlockContext : public LlamaLangParseContext {
  public:
    BlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_CURLY();
    antlr4::tree::TerminalNode *R_CURLY();
    StatementListContext *statementList();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BlockContext* block();

  class  StatementListContext : public LlamaLangParseContext {
  public:
    StatementListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<StatementContext *> statement();
    StatementContext* statement(size_t i);
    std::vector<EosContext *> eos();
    EosContext* eos(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StatementListContext* statementList();

  class  StatementContext : public LlamaLangParseContext {
  public:
    StatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VarDefContext *varDef();
    SimpleStmtContext *simpleStmt();
    ReturnStmtContext *returnStmt();
    BlockContext *block();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StatementContext* statement();

  class  SimpleStmtContext : public LlamaLangParseContext {
  public:
    SimpleStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionStmtContext *expressionStmt();
    AssignmentContext *assignment();
    EmptyStmtContext *emptyStmt();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleStmtContext* simpleStmt();

  class  ExpressionStmtContext : public LlamaLangParseContext {
  public:
    ExpressionStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpressionContext *expression();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExpressionStmtContext* expressionStmt();

  class  AssignmentContext : public LlamaLangParseContext {
  public:
    AssignmentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    Assign_opContext *assign_op();
    ExpressionListContext *expressionList();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AssignmentContext* assignment();

  class  Assign_opContext : public LlamaLangParseContext {
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

  Assign_opContext* assign_op();

  class  EmptyStmtContext : public LlamaLangParseContext {
  public:
    EmptyStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SEMI();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EmptyStmtContext* emptyStmt();

  class  ReturnStmtContext : public LlamaLangParseContext {
  public:
    ReturnStmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *RETURN();
    ExpressionContext *expression();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ReturnStmtContext* returnStmt();

  class  Type_Context : public LlamaLangParseContext {
  public:
    Type_Context(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeNameContext *typeName();
    PointerContext *pointer();
    ArrayContext *array();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Type_Context* type_();

  class  PointerContext : public LlamaLangParseContext {
  public:
    PointerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STAR();
    Type_Context *type_();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PointerContext* pointer();

  class  ArrayContext : public LlamaLangParseContext {
  public:
    ArrayContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_BRACKET();
    antlr4::tree::TerminalNode *R_BRACKET();
    TypeNameContext *typeName();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArrayContext* array();

  class  TypeNameContext : public LlamaLangParseContext {
  public:
    TypeNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    QualifiedIdentContext *qualifiedIdent();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeNameContext* typeName();

  class  SignatureContext : public LlamaLangParseContext {
  public:
    SignatureContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ParametersContext *parameters();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SignatureContext* signature();

  class  ResultContext : public LlamaLangParseContext {
  public:
    ResultContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ParametersContext *parameters();
    Type_Context *type_();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ResultContext* result();

  class  ParametersContext : public LlamaLangParseContext {
  public:
    ParametersContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_PAREN();
    antlr4::tree::TerminalNode *R_PAREN();
    std::vector<ParameterDeclContext *> parameterDecl();
    ParameterDeclContext* parameterDecl(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParametersContext* parameters();

  class  ParameterDeclContext : public LlamaLangParseContext {
  public:
    ParameterDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *COLON();
    Type_Context *type_();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParameterDeclContext* parameterDecl();

  class  ExpressionContext : public LlamaLangParseContext {
  public:
    LlamaLangParser::ExpressionContext *left = nullptr;
    LlamaLangParser::ExpressionContext *right = nullptr;
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PrimaryExprContext *primaryExpr();
    UnaryExprContext *unaryExpr();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
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

  ExpressionContext* expression();
  ExpressionContext* expression(int precedence);
  class  PrimaryExprContext : public LlamaLangParseContext {
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

  PrimaryExprContext* primaryExpr();
  PrimaryExprContext* primaryExpr(int precedence);
  class  UnaryExprContext : public LlamaLangParseContext {
  public:
    UnaryExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PrimaryExprContext *primaryExpr();
    UnaryOpContext *unaryOp();
    ExpressionContext *expression();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnaryExprContext* unaryExpr();

  class  ConversionContext : public LlamaLangParseContext {
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

  ConversionContext* conversion();

  class  OperandContext : public LlamaLangParseContext {
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

  OperandContext* operand();

  class  UnaryOpContext : public LlamaLangParseContext {
  public:
    UnaryOpContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *MINUS();
    antlr4::tree::TerminalNode *EXCLAMATION();
    antlr4::tree::TerminalNode *STAR();
    antlr4::tree::TerminalNode *AMPERSAND();
    antlr4::tree::TerminalNode *MINUS_MINUS();
    antlr4::tree::TerminalNode *PLUS_PLUS();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnaryOpContext* unaryOp();

  class  LiteralContext : public LlamaLangParseContext {
  public:
    LiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BasicLitContext *basicLit();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LiteralContext* literal();

  class  BasicLitContext : public LlamaLangParseContext {
  public:
    BasicLitContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IntegerContext *integer();
    String_Context *string_();
    FloatingPointContext *floatingPoint();
    antlr4::tree::TerminalNode *RUNE_LIT();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BasicLitContext* basicLit();

  class  IntegerContext : public LlamaLangParseContext {
  public:
    IntegerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DECIMAL_LIT();
    antlr4::tree::TerminalNode *OCTAL_LIT();
    antlr4::tree::TerminalNode *HEX_LIT();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntegerContext* integer();

  class  FloatingPointContext : public LlamaLangParseContext {
  public:
    FloatingPointContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FLOAT_LIT();
    antlr4::tree::TerminalNode *DOUBLE_LIT();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FloatingPointContext* floatingPoint();

  class  OperandNameContext : public LlamaLangParseContext {
  public:
    OperandNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    QualifiedIdentContext *qualifiedIdent();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OperandNameContext* operandName();

  class  QualifiedIdentContext : public LlamaLangParseContext {
  public:
    QualifiedIdentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    antlr4::tree::TerminalNode *DOT();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QualifiedIdentContext* qualifiedIdent();

  class  LiteralTypeContext : public LlamaLangParseContext {
  public:
    LiteralTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeNameContext *typeName();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LiteralTypeContext* literalType();

  class  FieldDeclContext : public LlamaLangParseContext {
  public:
    FieldDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdentifierListContext *identifierList();
    Type_Context *type_();
    String_Context *string_();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FieldDeclContext* fieldDecl();

  class  String_Context : public LlamaLangParseContext {
  public:
    String_Context(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTERPRETED_STRING_LIT();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  String_Context* string_();

  class  ArgumentsContext : public LlamaLangParseContext {
  public:
    ArgumentsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_PAREN();
    antlr4::tree::TerminalNode *R_PAREN();
    ExpressionListContext *expressionList();
    Type_Context *type_();
    antlr4::tree::TerminalNode *ELLIPSIS();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArgumentsContext* arguments();

  class  MethodExprContext : public LlamaLangParseContext {
  public:
    MethodExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ReceiverTypeContext *receiverType();
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *IDENTIFIER();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MethodExprContext* methodExpr();

  class  ReceiverTypeContext : public LlamaLangParseContext {
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

  ReceiverTypeContext* receiverType();

  class  EosContext : public LlamaLangParseContext {
  public:
    EosContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SEMI();
    antlr4::tree::TerminalNode *EOF();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EosContext* eos();


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


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

}  // namespace llang
