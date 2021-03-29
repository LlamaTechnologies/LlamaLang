
// Generated from ../../LlamaLang.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"


namespace llang {


class  LlamaLangLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, DIR_BEGIN = 5, FUNC = 6, RETURN = 7, 
    IDENTIFIER = 8, L_PAREN = 9, R_PAREN = 10, L_CURLY = 11, R_CURLY = 12, 
    L_BRACKET = 13, R_BRACKET = 14, ASSIGN = 15, COMMA = 16, SEMI = 17, 
    COLON = 18, DOT = 19, PLUS_PLUS = 20, MINUS_MINUS = 21, DECLARE_ASSIGN = 22, 
    ELLIPSIS = 23, LOGICAL_OR = 24, LOGICAL_AND = 25, EQUALS = 26, NOT_EQUALS = 27, 
    LESS = 28, LESS_OR_EQUALS = 29, GREATER = 30, GREATER_OR_EQUALS = 31, 
    OR = 32, DIV = 33, MOD = 34, LSHIFT = 35, RSHIFT = 36, BIT_CLEAR = 37, 
    EXCLAMATION = 38, PLUS = 39, MINUS = 40, CARET = 41, STAR = 42, AMPERSAND = 43, 
    DECIMAL_LIT = 44, OCTAL_LIT = 45, HEX_LIT = 46, FLOAT_LIT = 47, DOUBLE_LIT = 48, 
    RUNE_LIT = 49, INTERPRETED_STRING_LIT = 50, WS = 51, COMMENT = 52, TERMINATOR = 53, 
    LINE_COMMENT = 54
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
