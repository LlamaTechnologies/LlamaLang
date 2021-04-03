
// Generated from ../../ModuleRetriever.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"


namespace llang {


class  ModuleRetrieverParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, HASH = 2, MODULE = 3, INCLUDE = 4, MAIN = 5, FUNC = 6, LPAREN = 7, 
    RPAREN = 8, LBRACKET = 9, RBRACKET = 10, COMMA = 11, IDENTIFIER = 12, 
    WS = 13, UNICODE_WS = 14, COMMENT = 15, LINE_COMMENT = 16, OTHER = 17
  };

  enum {
    RuleValidSource = 0, RuleModuleDirective = 1, RuleMainDirective = 2, 
    RuleParameterList = 3, RuleParameterDecl = 4, RuleAnyToken = 5
  };

  explicit ModuleRetrieverParser(antlr4::TokenStream *input);
  ~ModuleRetrieverParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class ValidSourceContext;
  class ModuleDirectiveContext;
  class MainDirectiveContext;
  class ParameterListContext;
  class ParameterDeclContext;
  class AnyTokenContext; 

  class  ValidSourceContext : public antlr4::ParserRuleContext {
  public:
    ValidSourceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ModuleDirectiveContext *moduleDirective();
    std::vector<AnyTokenContext *> anyToken();
    AnyTokenContext* anyToken(size_t i);
    antlr4::tree::TerminalNode *EOF();
    std::vector<MainDirectiveContext *> mainDirective();
    MainDirectiveContext* mainDirective(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ValidSourceContext* validSource();

  class  ModuleDirectiveContext : public antlr4::ParserRuleContext {
  public:
    ModuleDirectiveContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *HASH();
    antlr4::tree::TerminalNode *MODULE();
    antlr4::tree::TerminalNode *IDENTIFIER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ModuleDirectiveContext* moduleDirective();

  class  MainDirectiveContext : public antlr4::ParserRuleContext {
  public:
    MainDirectiveContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    ParameterListContext *parameterList();
    antlr4::tree::TerminalNode *HASH();
    antlr4::tree::TerminalNode *MAIN();
    antlr4::tree::TerminalNode *FUNC();
    antlr4::tree::TerminalNode *IDENTIFIER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  MainDirectiveContext* mainDirective();

  class  ParameterListContext : public antlr4::ParserRuleContext {
  public:
    ParameterListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ParameterDeclContext *> parameterDecl();
    ParameterDeclContext* parameterDecl(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ParameterListContext* parameterList();

  class  ParameterDeclContext : public antlr4::ParserRuleContext {
  public:
    ParameterDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    antlr4::tree::TerminalNode *LBRACKET();
    antlr4::tree::TerminalNode *RBRACKET();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  ParameterDeclContext* parameterDecl();

  class  AnyTokenContext : public antlr4::ParserRuleContext {
  public:
    AnyTokenContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  AnyTokenContext* anyToken();


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
