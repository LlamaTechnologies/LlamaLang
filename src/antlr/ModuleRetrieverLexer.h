
// Generated from ../../ModuleRetriever.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"


namespace llang {


class  ModuleRetrieverLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, HASH = 2, MODULE = 3, INCLUDE = 4, MAIN = 5, FUNC = 6, LPAREN = 7, 
    RPAREN = 8, LBRACKET = 9, RBRACKET = 10, COMMA = 11, IDENTIFIER = 12, 
    WS = 13, UNICODE_WS = 14, COMMENT = 15, LINE_COMMENT = 16, OTHER = 17
  };

  explicit ModuleRetrieverLexer(antlr4::CharStream *input);
  ~ModuleRetrieverLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

}  // namespace llang
