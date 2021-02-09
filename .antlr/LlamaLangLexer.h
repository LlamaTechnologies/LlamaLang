 #include "../ast/Node.hpp" 

// Generated from c:\Users\narbl\Documents\GitHub\LlamaLang-Cpp\LlamaLang.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"




class  LlamaLangLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, FUNC = 2, RETURN = 3, IDENTIFIER = 4, L_PAREN = 5, R_PAREN = 6, 
    L_CURLY = 7, R_CURLY = 8, L_BRACKET = 9, R_BRACKET = 10, ASSIGN = 11, 
    COMMA = 12, SEMI = 13, COLON = 14, DOT = 15, PLUS_PLUS = 16, MINUS_MINUS = 17, 
    DECLARE_ASSIGN = 18, ELLIPSIS = 19, LOGICAL_OR = 20, LOGICAL_AND = 21, 
    EQUALS = 22, NOT_EQUALS = 23, LESS = 24, LESS_OR_EQUALS = 25, GREATER = 26, 
    GREATER_OR_EQUALS = 27, OR = 28, DIV = 29, MOD = 30, LSHIFT = 31, RSHIFT = 32, 
    BIT_CLEAR = 33, EXCLAMATION = 34, PLUS = 35, MINUS = 36, CARET = 37, 
    STAR = 38, AMPERSAND = 39, ARROW = 40, DECIMAL_LIT = 41, OCTAL_LIT = 42, 
    HEX_LIT = 43, FLOAT_LIT = 44, RUNE_LIT = 45, INTERPRETED_STRING_LIT = 46, 
    WS = 47, COMMENT = 48, TERMINATOR = 49, LINE_COMMENT = 50
  };

  LlamaLangLexer(antlr4::CharStream *input);
  ~LlamaLangLexer();


  std::shared_ptr<ast::Node> AstNode;

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

