
// Generated from ../../ModuleRetriever.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"


namespace llang {


class  ModuleRetrieverParser : public antlr4::Parser {
public:
  enum {
    HASH = 1, MODULE = 2, INCLUDE = 3, IDENTIFIER = 4, WS = 5, UNICODE_WS = 6, 
    COMMENT = 7, LINE_COMMENT = 8
  };

  enum {
    RuleValidSource = 0, RuleModuleDirective = 1, RuleIncludeDirective = 2, 
    RuleAnyToken = 3
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
  class IncludeDirectiveContext;
  class AnyTokenContext; 

  class  ValidSourceContext : public antlr4::ParserRuleContext {
  public:
    ValidSourceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ModuleDirectiveContext *moduleDirective();
    AnyTokenContext *anyToken();
    antlr4::tree::TerminalNode *EOF();
    std::vector<IncludeDirectiveContext *> includeDirective();
    IncludeDirectiveContext* includeDirective(size_t i);

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

  class  IncludeDirectiveContext : public antlr4::ParserRuleContext {
  public:
    IncludeDirectiveContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *HASH();
    antlr4::tree::TerminalNode *INCLUDE();
    antlr4::tree::TerminalNode *IDENTIFIER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;
   
  };

  IncludeDirectiveContext* includeDirective();

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
