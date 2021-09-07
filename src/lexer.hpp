#pragma once
#include "error.hpp"

#include <string.h>
#include <string>
#include <string_view>
#include <vector>

enum class TokenId
{
  HASH,      // #
  EXTERN,    // extern
  FN,        // fn
  RET,       // ret
  IF,        // if
  ELIF,      // elif
  ELSE,      // else
  L_PAREN,   // (
  R_PAREN,   // )
  L_CURLY,   // {
  R_CURLY,   // }
  L_BRACKET, // [
  R_BRACKET, // ]
  COMMA,     // ,
  SEMI,      // ;
  COLON,     // :
  DOT,       // .
  ASSIGN,    // =

  // UNARY OPERATORS
  PLUS_PLUS,   // ++
  MINUS_MINUS, // --
  NOT,         // !

  // RELATION OPERATORS
  OR,                // || or
  AND,               // && and
  EQUALS,            // ==
  NOT_EQUALS,        // !=
  LESS,              // <
  LESS_OR_EQUALS,    // <=
  GREATER,           // >
  GREATER_OR_EQUALS, // >=

  // ARITHMETIVC OPERATORS
  PLUS,         // +
  MINUS,        // -
  MUL,          // *
  DIV,          // /
  MOD,          // %
  PLUS_ASSIGN,  // +=
  MINUS_ASSIGN, // -=
  MUL_ASSIGN,   // *=
  DIV_ASSIGN,   // /=
  MOD_ASSIGN,   // %=
  // BITWISE OPERATORS
  BIT_AND, // &
  BIT_XOR, // ^
  BIT_OR,  // |
  BIT_NOT, // ~
  LSHIFT,  // <<
  RSHIFT,  // >>

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

  WS,           // [\t \r \n ' ']
  DOC_COMMENT,  // '/*' . '*/'
  LINE_COMMENT, // // .
  ERROR,        // special token: describes an invalid token
  _EOF          // end of file
};

const char *token_id_name(TokenId id);
typedef uint32_t Char;

enum class INT_BASE
{
  BINARY,
  OCTAL,
  DECIMAL,
  HEXADECIMAL
};

constexpr const size_t MAX_NUMBER_DIGITS = 40;

struct IntToken {
  char number[MAX_NUMBER_DIGITS];
  size_t digits_count;
  INT_BASE base;
  bool is_negative;

  IntToken() : digits_count(0), base(INT_BASE::DECIMAL), is_negative(false) { memset(number, 0, MAX_NUMBER_DIGITS); }
};

struct FloatToken {
  char number[MAX_NUMBER_DIGITS];
  size_t digits_count;

  FloatToken() : digits_count(0) { memset(number, 0, MAX_NUMBER_DIGITS); }
};

struct Token {
  TokenId id;
  size_t start_pos;
  size_t end_pos;
  size_t start_line;
  size_t start_column;
  std::string_view file_name;

  union {
    Char char_lit;
    FloatToken float_lit;
    IntToken int_lit;
  };

  Token(const std::string &in_file_name)
      : id(TokenId::_EOF), start_pos(0L), end_pos(0L), start_line(0), start_column(0), file_name(in_file_name),
        int_lit() {}

  virtual ~Token() {}

  Token(const Token &other)
      : id(other.id), start_pos(other.start_pos), end_pos(other.end_pos), start_line(other.start_line),
        start_column(other.start_column), file_name(other.file_name) {
    switch (id) {
    case TokenId::FLOAT_LIT:
      float_lit = other.float_lit;
      break;
    case TokenId::UNICODE_CHAR:
      char_lit = other.char_lit;
      break;
    default:
      int_lit = other.int_lit;
      break;
    }
  }

  Token &operator=(const Token &other) {
    // self-assignment guard
    if (this == &other)
      return *this;

    id = other.id;
    start_line = other.start_line;
    start_column = other.start_column;
    start_pos = other.start_pos;
    end_pos = other.end_pos;
    file_name = other.file_name;

    switch (id) {
    case TokenId::FLOAT_LIT:
      float_lit = other.float_lit;
      break;
    case TokenId::UNICODE_CHAR:
      char_lit = other.char_lit;
      break;
    default:
      int_lit = other.int_lit;
      break;
    }

    return *this;
  }

  size_t get_value_size() { return (end_pos + 1) - start_pos; }
};

typedef std::vector<std::string> Console;

enum class TokenizerState;

class Lexer {
  Token current_token;
  TokenizerState state;

public:
  std::string file_directory;
  std::string file_name;
  std::string source;

private:
  std::vector<Token> tokens;
  std::vector<Token> comments;
  std::vector<Error> &errors;

  size_t cursor_pos;
  size_t current_line;
  size_t current_column;
  mutable size_t curr_index; // used to consume tokens

  size_t char_code_index;      // char_code char counter
  size_t remaining_code_units; // used to count bytes in unicode char
  uint32_t radix;              // used for getting number value.
  uint32_t char_code;          // char_code used accros the char_code state
  bool unicode;                // is unicode char code
  bool is_trailing_underscore; // used to interpret number_number
  bool is_invalid_token;       // used to finish tokenizing a error tokens

  bool _padding; // not used, it is to have a clear visual of the struct size

public:
  /**
   * Tokenize the '_src_file' string
   **/
  Lexer(const std::string &_src_file, const std::string_view &_file_directory, const std::string &_file_name,
        std::vector<Error> &_errors);
  void tokenize() noexcept;

  const bool has_tokens() const noexcept;

  const Token &get_previous_token() const noexcept;

  // should not be called after EOF token
  const Token &get_next_token() const noexcept;

  // --curr_index
  void get_back() const noexcept;

  std::string_view get_token_value(const Token &token) const noexcept;

  friend Console print_tokens(Lexer &lexer);

private:
  friend void _begin_token(Lexer &, const TokenId id) noexcept;
  friend void _end_token(Lexer &) noexcept;
  friend void _end_token_check_is_keyword(Lexer &) noexcept;
  friend void _reset_line(Lexer &) noexcept;
  friend void _invalid_char_error(Lexer &, uint8_t c) noexcept;
  friend void _tokenize_error(Lexer &, const char *format, ...) noexcept;
  friend void _handle_string_escape(Lexer &, uint8_t c) noexcept;
};

Console print_tokens(Lexer &lexer);
