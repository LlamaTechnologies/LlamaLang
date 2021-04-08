
// Generated from ../../LlamaLang.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"
#include "LlamaLangParserBase.hpp"
#include "LlamaLangParseContext.hpp"

namespace llang {


class  LlamaLangParser : public LlamaLangParserBase {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, DIR_BEGIN = 10, FUNC = 11, RETURN = 12, IDENTIFIER = 13, 
    L_PAREN = 14, R_PAREN = 15, L_CURLY = 16, R_CURLY = 17, L_BRACKET = 18, 
    R_BRACKET = 19, ASSIGN = 20, COMMA = 21, SEMI = 22, COLON = 23, DOT = 24, 
    PLUS_PLUS = 25, MINUS_MINUS = 26, DECLARE_ASSIGN = 27, ELLIPSIS = 28, 
    LOGICAL_OR = 29, LOGICAL_AND = 30, EQUALS = 31, NOT_EQUALS = 32, LESS = 33, 
    LESS_OR_EQUALS = 34, GREATER = 35, GREATER_OR_EQUALS = 36, OR = 37, 
    DIV = 38, MOD = 39, LSHIFT = 40, RSHIFT = 41, BIT_CLEAR = 42, EXCLAMATION = 43, 
    PLUS = 44, MINUS = 45, CARET = 46, STAR = 47, AMPERSAND = 48, TYPE_SPEC = 49, 
    DECIMAL_LIT = 50, OCTAL_LIT = 51, HEX_LIT = 52, BIN_LIT = 53, FLOAT_LIT = 54, 
    DOUBLE_LIT = 55, RUNE_LIT = 56, INTERPRETED_STRING_LIT = 57, WS = 58, 
    COMMENT = 59, TERMINATOR = 60, LINE_COMMENT = 61
  };

  enum {
    RuleSourceFile = 0, RuleIdentifierList = 1, RuleExpressionList = 2, 
    RuleIncludeDirective = 3, RuleExternDirective = 4, RuleRunDirective = 5, 
    RuleBasicDirective = 6, RuleTypeSpecifier = 7, RuleFunctionDef = 8, 
    RuleVarDef = 9, RuleBlock = 10, RuleStatementList = 11, RuleStatement = 12, 
    RuleSimpleStmt = 13, RuleExpressionStmt = 14, RuleAssignment = 15, RuleAssign_op = 16, 
    RuleEmptyStmt = 17, RuleReturnStmt = 18, RuleType_ = 19, RulePointer = 20, 
    RuleArray = 21, RuleTypeName = 22, RuleSignature = 23, RuleResult = 24, 
    RuleParameters = 25, RuleParameterDecl = 26, RuleExpression = 27, RulePrimaryExpr = 28, 
    RuleUnaryExpr = 29, RuleOperand = 30, RuleUnaryOp = 31, RuleLiteral = 32, 
    RuleBasicLit = 33, RuleInteger = 34, RuleFloatingPoint = 35, RuleOperandName = 36, 
    RuleQualifiedIdent = 37, RuleLiteralType = 38, RuleFieldDecl = 39, RuleString_ = 40, 
    RuleArguments = 41, RuleMethodExpr = 42, RuleReceiverType = 43, RuleEos = 44
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
  class IncludeDirectiveContext;
  class ExternDirectiveContext;
  class RunDirectiveContext;
  class BasicDirectiveContext;
  class TypeSpecifierContext;
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
    std::vector<IncludeDirectiveContext *> includeDirective();
    IncludeDirectiveContext* includeDirective(size_t i);
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

  class  IncludeDirectiveContext : public LlamaLangParseContext {
  public:
    IncludeDirectiveContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DIR_BEGIN();
    String_Context *string_();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IncludeDirectiveContext* includeDirective();

  class  ExternDirectiveContext : public LlamaLangParseContext {
  public:
    ExternDirectiveContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DIR_BEGIN();
    antlr4::tree::TerminalNode *IDENTIFIER();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExternDirectiveContext* externDirective();

  class  RunDirectiveContext : public LlamaLangParseContext {
  public:
    RunDirectiveContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DIR_BEGIN();
    PrimaryExprContext *primaryExpr();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RunDirectiveContext* runDirective();

  class  BasicDirectiveContext : public LlamaLangParseContext {
  public:
    BasicDirectiveContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DIR_BEGIN();
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    String_Context *string_();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BasicDirectiveContext* basicDirective();

  class  TypeSpecifierContext : public LlamaLangParseContext {
  public:
    TypeSpecifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeSpecifierContext* typeSpecifier();

  class  FunctionDefContext : public LlamaLangParseContext {
  public:
    FunctionDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FUNC();
    antlr4::tree::TerminalNode *IDENTIFIER();
    SignatureContext *signature();
    Type_Context *type_();
    BlockContext *block();
    BasicDirectiveContext *basicDirective();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionDefContext* functionDef();

  class  VarDefContext : public LlamaLangParseContext {
  public:
    VarDefContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
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
    std::vector<antlr4::tree::TerminalNode *> BIN_LIT();
    antlr4::tree::TerminalNode* BIN_LIT(size_t i);


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
  bool eosSempred(EosContext *_localctx, size_t predicateIndex);

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
