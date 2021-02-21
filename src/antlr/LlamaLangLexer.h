
// Generated from ../../LlamaLang.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"


namespace llang {


class  LlamaLangLexer : public antlr4::Lexer {
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
    RUNE_LIT = 44, INTERPRETED_STRING_LIT = 45, WS = 46, COMMENT = 47, TERMINATOR = 48, 
    LINE_COMMENT = 49
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
