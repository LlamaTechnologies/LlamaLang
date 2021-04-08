
// Generated from ../../LlamaLang.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"


namespace llang {


class  LlamaLangLexer : public antlr4::Lexer {
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
