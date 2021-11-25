#pragma once
#include "../common_defs.hpp"

#include <string>

enum class TokenId
{
  HASH,       // #
  EXTERN,     // extern
  FN,         // fn
  RET,        // ret
  IF,         // if
  ELIF,       // elif
  ELSE,       // else
  LOOP,       // loop
  TRUE,       // true
  FALSE,      // false
  NIL,        // nil
  BREAK,      // break
  CONTINUE,   // continue
  L_PAREN,    // (
  R_PAREN,    // )
  L_CURLY,    // {
  R_CURLY,    // }
  L_BRACKET,  // [
  R_BRACKET,  // ]
  COMMA,      // ,
  SEMI_COLON, // ;
  COLON,      // :
  DOT,        // .
  EQUALS,     // =

  // UNARY OPERATORS
  NOT, // !

  // RELATION OPERATORS
  CMP_EQUALS,        // ==
  CMP_NOT_EQUALS,    // !=
  CMP_LESS_OR_EQ,    // <=
  CMP_GREATER_OR_EQ, // >=
  CMP_LESS,          // <
  CMP_GREATER,       // >
  CMP_AND,           // && and
  CMP_OR,            // || or

  // ARITHMETIVC OPERATORS
  PLUS,  // +
  DASH,  // -
  STAR,  // *
  SLASH, // /
  MOD,   // %
  // BITWISE OPERATORS
  AMPERSAND, // &
  CARET,     // ^
  BIT_OR,    // |
  BIT_NOT,   // ~
  LSHIFT,    // <<
  RSHIFT,    // >>

  // COMPLEX TOKENS
  IDENTIFIER, // [a-zA-Z_] [a-zA-Z_0-9]*
  INT_LIT,
  // [0-9]+               [u]? [bwl]?
  // [0] [o] [0-7]*       [u]? [bwl]?
  // [0] [x] [0-9A-F]*    [u]? [bwl]?
  // [0] [b] [0-1]*       [u]? [bwl]?
  FLOAT_LIT, // [0-9]* [.] [0-9]* [f]?

  ESCAPED_VALUE, // \[value]
  STRING,        // " (~["\\] | ESCAPED_VALUE)* "
  UNICODE_CHAR,  // " (~["\\] | ESCAPED_VALUE)* "

  DOC_COMMENT,  // '/*' . '*/'
  LINE_COMMENT, // // .
  ERROR,        // special token: describes an invalid token
  _EOF          // end of file
};

struct TokenPos {
  s32 file_id;
  s64 column; // starts at 1
  s64 line;   // starts at 1
  u64 offset; // starts at 0
};

struct Token {
  TokenPos pos;
  TokenId id;
  std::string string;
};
