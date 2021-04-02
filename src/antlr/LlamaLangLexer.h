
// Generated from ../../LlamaLang.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"


namespace llang {


class  LlamaLangLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, DIR_BEGIN = 4, FUNC = 5, RETURN = 6, IDENTIFIER = 7, 
    L_PAREN = 8, R_PAREN = 9, L_CURLY = 10, R_CURLY = 11, L_BRACKET = 12, 
    R_BRACKET = 13, ASSIGN = 14, COMMA = 15, SEMI = 16, COLON = 17, DOT = 18, 
    PLUS_PLUS = 19, MINUS_MINUS = 20, DECLARE_ASSIGN = 21, ELLIPSIS = 22, 
    LOGICAL_OR = 23, LOGICAL_AND = 24, EQUALS = 25, NOT_EQUALS = 26, LESS = 27, 
    LESS_OR_EQUALS = 28, GREATER = 29, GREATER_OR_EQUALS = 30, OR = 31, 
    DIV = 32, MOD = 33, LSHIFT = 34, RSHIFT = 35, BIT_CLEAR = 36, EXCLAMATION = 37, 
    PLUS = 38, MINUS = 39, CARET = 40, STAR = 41, AMPERSAND = 42, DECIMAL_LIT = 43, 
    OCTAL_LIT = 44, HEX_LIT = 45, FLOAT_LIT = 46, DOUBLE_LIT = 47, RUNE_LIT = 48, 
    INTERPRETED_STRING_LIT = 49, WS = 50, COMMENT = 51, TERMINATOR = 52, 
    LINE_COMMENT = 53
  };

  explicit LlamaLangLexer(antlr4::CharStream *input);
  ~LlamaLangLexer();

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
