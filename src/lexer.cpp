#include "lexer.hpp"

#include "common_defs.hpp"
#include "file_utils.hpp"

#include <cassert>
#include <cstdarg>
#include <fstream>
#include <unordered_map>

#define WHITESPACE ' ' : case '\r' : case '\t' : case '\n'

// [1-9]
#define DIGIT_NON_ZERO '1' : case '2' : case '3' : case '4' : case '5' : case '6' : case '7' : case '8' : case '9'

// [0-9]
#define DIGIT '0' : case DIGIT_NON_ZERO

// [a-zA-Z]
#define ALPHA                                                                                                       \
  'a' : case 'b' : case 'c' : case 'd' : case 'e' : case 'f' : case 'g' : case 'h' : case 'i' : case 'j' : case 'k' \
      : case 'l' : case 'm' : case 'n' : case 'o' : case 'p' : case 'q' : case 'r' : case 's' : case 't' : case 'u' \
      : case 'v' : case 'w' : case 'x' : case 'y' : case 'z' : case 'A' : case 'B' : case 'C' : case 'D' : case 'E' \
      : case 'F' : case 'G' : case 'H' : case 'I' : case 'J' : case 'K' : case 'L' : case 'M' : case 'N' : case 'O' \
      : case 'P' : case 'Q' : case 'R' : case 'S' : case 'T' : case 'U' : case 'V' : case 'W' : case 'X' : case 'Y' \
      : case 'Z'

// [a-zA-Z_]
#define SYMBOL_START \
ALPHA:               \
  case '_'

// [a-zA-Z_0-9]
#define SYMBOL_CHAR \
ALPHA:              \
  case DIGIT:       \
  case '_'

static void _set_token_id(Token &, const TokenId id) noexcept;
static void _is_keyword(Token &, std::string_view) noexcept;
static uint32_t _get_digit_value(uint8_t);
static const char *_get_escape_shorthand(uint8_t);
static bool _is_symbol_char(uint8_t);
static bool _is_reserved_char(uint8_t);
static bool _is_float_specifier(uint8_t);
static bool _is_sign_or_type_specifier(uint8_t);
static bool _is_exponent_signifier(uint8_t, int);

enum class TokenizerState
{
  START,
  SYMBOL,                              // [a-zA-Z_][a-zA-Z_0-9]*
  ZERO,                                // "0", which might lead to "0x"
  NUMBER,                              // "123", "0x123"
  NUMBER_NO_UNDERSCORE,                // "12_", "0x12_" next char must be digit
  NUMBER_DOT,                          // . inside a number
  FLOAT_FRACTION,                      // "123.456", "0x123.456"
  FLOAT_FRACTION_NO_UNDERSCORE,        // "123.45_", "0x123.45_"
  FLOAT_EXPONENT_UNSIGNED,             // "123.456e", "123e", "0x123p"
  FLOAT_EXPONENT_NUMBER,               // "123.456e7", "123.456e+7", "123.456e-7"
  FLOAT_EXPONENT_NUMBER_NO_UNDERSCORE, // "123.456e7_", "123.456e+7_", "123.456e-7_"
  STRING,                              //
  STRING_ESCAPE,                       // saw \ inside a string
  STRING_ESCAPE_UNICODE_START,         // saw u inside string_escape
  CHAR_CODE,                           // saw x in string_escape or began the unicode escape
  CHAR_LITERAL,                        // saw '
  CHAR_LITERAL_UNICODE,                // is 'uXXXX'
  CHAR_LITERAL_END,                    // saw ' after '
  SAW_EQUALS,                          // saw =
  SAW_STAR,                            // saw *
  SAW_SLASH,                           // saw /
  SAW_PERCENT,                         // saw %
  SAW_PLUS,                            // saw +
  SAW_DASH,                            // saw -
  SAW_NOT,                             // saw !
  SAW_VERTICAL_BAR,                    // saw |
  SAW_AMPERSAND,                       // saw &
  SAW_LESS,                            // saw <
  SAW_GREATER,                         // saw >
  SAW_SIGN_TYPE_SPEC,                  // saw [ubwl]
  DOC_COMMENT,                         // inside /* */
  SAW_STAR_DOC_COMMENT,                // * in multiline comment may lead to end comment
  LINE_COMMENT,                        // inside // \n
  ERROR,                               //
};

Lexer::Lexer(const std::string &_src_file, const std::string_view &_file_directory, const std::string &_file_name,
             std::vector<Error> &_errors)
    : file_directory(_file_directory), file_name(_file_name), source(_src_file), errors(_errors), cursor_pos(0L),
      curr_index(SIZE_MAX), current_line(0L), current_column(0L), state(TokenizerState::START), radix(10),
      is_trailing_underscore(false), is_invalid_token(false), current_token(file_name), tokens(), comments() {}

// IMPORTANT!: should not be called more than once after the constructor.
void Lexer::tokenize() noexcept {
  // reading file while no errors in it
  for (/*cursor_pos = 0*/; cursor_pos < source.size(); cursor_pos++) {
    unsigned char c = source[cursor_pos];

    switch (state) {
      // begining of the source file
    case TokenizerState::START:
      switch (c) {
      case WHITESPACE:
        break;
        // we faund a symbol
      case SYMBOL_START:
        state = TokenizerState::SYMBOL;
        _begin_token(*this, TokenId::IDENTIFIER);
        break;
        // we found a 0 so we have to tokenize a
        // octal: 0o[0-7]...
        // binary:  0b[0-1]...
        // hex: 0x[0-9A-F]...
        // trailing 0 decimal: 0[1-9]...
      case '0':
        state = TokenizerState::ZERO;
        _begin_token(*this, TokenId::INT_LIT);
        is_trailing_underscore = false;
        radix = 10;
        current_token.int_lit.number[current_token.int_lit.digits_count] = c;
        current_token.int_lit.digits_count++;
        break;
      case DIGIT_NON_ZERO:
        state = TokenizerState::NUMBER;
        _begin_token(*this, TokenId::INT_LIT);
        is_trailing_underscore = false;
        radix = 10;
        current_token.int_lit.number[current_token.int_lit.digits_count] = c;
        current_token.int_lit.digits_count++;
        break;
        // we found the beginning of a string literal
      case '"':
        _begin_token(*this, TokenId::STRING);
        state = TokenizerState::STRING;
        break;
        // we found the beginning of a char literal
      case '\'':
        _begin_token(*this, TokenId::UNICODE_CHAR);
        state = TokenizerState::CHAR_LITERAL;
        break;
        // we found
        // ASSIGN =
        // EQUALS ==
      case '=':
        _begin_token(*this, TokenId::ASSIGN);
        state = TokenizerState::SAW_EQUALS;
        break;
        // we found
        // a DIV
        // /=
        // the beginning of a DOC comment
        // the beginning of a line comment
      case '/':
        _begin_token(*this, TokenId::DIV);
        state = TokenizerState::SAW_SLASH;
        break;
        // we found
        // MUL
        // *=
      case '*':
        _begin_token(*this, TokenId::MUL);
        state = TokenizerState::SAW_STAR;
        break;
        // we found
        // a PLUS
        // +=
        // ++
      case '+':
        _begin_token(*this, TokenId::PLUS);
        state = TokenizerState::SAW_PLUS;
        break;
        // we found
        // a MINUS
        // -=
        // --
      case '-':
        _begin_token(*this, TokenId::MINUS);
        state = TokenizerState::SAW_DASH;
        break;
        // we found
        // MOD
        // %=
      case '%':
        _begin_token(*this, TokenId::MOD);
        state = TokenizerState::SAW_PERCENT;
        break;
        // we found a hash
      case '#':
        _begin_token(*this, TokenId::HASH);
        _end_token(*this);
        break;
        // we found a semicolon
      case ';':
        _begin_token(*this, TokenId::SEMI);

        _end_token(*this);
        break;
      case '(':
        _begin_token(*this, TokenId::L_PAREN);
        _end_token(*this);
        break;
      case ')':
        _begin_token(*this, TokenId::R_PAREN);
        _end_token(*this);
        break;
      case '{':
        _begin_token(*this, TokenId::L_CURLY);
        _end_token(*this);
        break;
      case '}':
        _begin_token(*this, TokenId::R_CURLY);
        _end_token(*this);
        break;
      case '[':
        _begin_token(*this, TokenId::L_BRACKET);
        _end_token(*this);
        break;
      case ']':
        _begin_token(*this, TokenId::R_BRACKET);
        _end_token(*this);
        break;
      case '.':
        _begin_token(*this, TokenId::FLOAT_LIT);
        state = TokenizerState::NUMBER_DOT;
        break;
      case '!':
        _begin_token(*this, TokenId::NOT);
        state = TokenizerState::SAW_NOT;
        break;
      case '|':
        _begin_token(*this, TokenId::BIT_OR);
        state = TokenizerState::SAW_VERTICAL_BAR;
        break;
      case '&':
        _begin_token(*this, TokenId::BIT_AND);
        state = TokenizerState::SAW_AMPERSAND;
        break;
      case '~':
        _begin_token(*this, TokenId::BIT_NOT);
        _end_token(*this);
        break;
      case ',':
        _begin_token(*this, TokenId::COMMA);
        _end_token(*this);
        break;
      case '^':
        _begin_token(*this, TokenId::BIT_XOR);
        _end_token(*this);
        break;
      case '<':
        _begin_token(*this, TokenId::LESS);
        state = TokenizerState::SAW_LESS;
        break;
      case '>':
        _begin_token(*this, TokenId::GREATER);
        state = TokenizerState::SAW_GREATER;
        break;
      default:
        _tokenize_error(*this, "Unidentified character in symbol %c", c);
        is_invalid_token = true;
        state = TokenizerState::SYMBOL;
        break;
      }
      break;
      // TOKENIZING A IDENTIFIER
    case TokenizerState::SYMBOL:
      switch (c) {
      case SYMBOL_CHAR:
        break;
      default:
        cursor_pos--;

        if (is_invalid_token) {
          is_invalid_token = false;
          _set_token_id(current_token, TokenId::ERROR);
          _end_token(*this);
        } else {
          _end_token_check_is_keyword(*this);
        }

        state = TokenizerState::START;
        continue;
      }
      break;
      // TRYING TO TOKENIZE /...
    case TokenizerState::SAW_SLASH:
      switch (c) {
        // line comment //
      case '/':
        state = TokenizerState::LINE_COMMENT;
        break;
        // doc comment /* */
      case '*':
        _set_token_id(current_token, TokenId::DOC_COMMENT);
        state = TokenizerState::DOC_COMMENT;
        break;
        // DIV_ASSIGN /=
      case '=':
        _set_token_id(current_token, TokenId::DIV_ASSIGN);
        _end_token(*this);
        state = TokenizerState::START;
        break;
        // DIV
      default:
        cursor_pos -= 1;
        _end_token(*this);
        state = TokenizerState::START;
        continue;
      }
      break;
      // TOKENIZING A DOC COMMENT
    case TokenizerState::DOC_COMMENT:
      switch (c) {
      case '*':
        state = TokenizerState::SAW_STAR_DOC_COMMENT;
        break;
      default:
        break;
      }
      break;
    case TokenizerState::SAW_STAR_DOC_COMMENT:
      switch (c) {
      case '/':
        _end_token(*this);
        state = TokenizerState::START;
        break;
      default:
        state = TokenizerState::DOC_COMMENT;
        break;
      }
      break;
      // TOKENIZING A LINE COMMENT
    case TokenizerState::LINE_COMMENT:
      switch (c) {
      case '\n':
        state = TokenizerState::START;
        break;
      default:
        break;
      }
      break;
      // Saw a 0
    case TokenizerState::ZERO:
      switch (c) {
      case 'b':
        radix = 2;
        state = TokenizerState::NUMBER_NO_UNDERSCORE;
        current_token.int_lit.base = INT_BASE::BINARY;
        break;
      case 'o':
        radix = 8;
        state = TokenizerState::NUMBER_NO_UNDERSCORE;
        current_token.int_lit.base = INT_BASE::OCTAL;
        break;
      case 'x':
        radix = 16;
        state = TokenizerState::NUMBER_NO_UNDERSCORE;
        current_token.int_lit.base = INT_BASE::HEXADECIMAL;
        break;
      default:
        // reinterpret as normal number
        cursor_pos--;
        state = TokenizerState::NUMBER;
        current_token.int_lit.base = INT_BASE::DECIMAL;
        continue;
      }
      break;
      // saw a number that is not 0
    case TokenizerState::NUMBER_NO_UNDERSCORE:
      // cant have two undersocores in a number
      if (c == '_') {
        _invalid_char_error(*this, c);
        state = TokenizerState::NUMBER_NO_UNDERSCORE;
        is_invalid_token = true;
        break;
      }
      // get the digit
      else if (_get_digit_value(c) < radix) {
        is_trailing_underscore = false;
        state = TokenizerState::NUMBER;
      }
      LL_FALLTHROUGH
      // we saw a number that my have '.' '_' or [eEpP]
    case TokenizerState::NUMBER: {
      // underscore in number
      if (c == '_') {
        is_trailing_underscore = true;
        state = TokenizerState::NUMBER_NO_UNDERSCORE;
        break;
      }
      // dot in number
      if (c == '.') {
        // _. not allowed
        if (is_trailing_underscore) {
          _invalid_char_error(*this, c);
          break;
        }
        state = TokenizerState::NUMBER_DOT;
        break;
      }
      // is eE or pP
      if (_is_exponent_signifier(c, radix)) {
        // _[eEpP] not allowed
        if (is_trailing_underscore) {
          _invalid_char_error(*this, c);
          break;
        }
        // only accept hex and dec numbers to have exp
        if (radix != 16 && radix != 10) {
          _invalid_char_error(*this, c);
        }
        state = TokenizerState::FLOAT_EXPONENT_UNSIGNED;
        radix = 10; // exponent is always base 10
        assert(current_token.id == TokenId::INT_LIT);
        _set_token_id(current_token, TokenId::FLOAT_LIT);

        break;
      }
      uint32_t digit_value = _get_digit_value(c);
      if (digit_value >= radix) {
        if (_is_sign_or_type_specifier(c)) {
          state = TokenizerState::SAW_SIGN_TYPE_SPEC;
          break;
        } else if (is_trailing_underscore) {
          _invalid_char_error(*this, c);
          is_invalid_token = true;
          state = TokenizerState::SYMBOL;
          break;
        } else if (_is_symbol_char(c) || (!isxdigit(c) && !_is_reserved_char(c))) {
          _invalid_char_error(*this, c);
          is_invalid_token = true;
          state = TokenizerState::SYMBOL;
          break;
        } else {
          state = TokenizerState::START;
          // not my char
          cursor_pos--;

          if (is_invalid_token) {
            is_invalid_token = false;
            _set_token_id(current_token, TokenId::ERROR);
          }

          _end_token(*this);
          continue;
        }
      }
      current_token.int_lit.number[current_token.int_lit.digits_count] = c;
      current_token.int_lit.digits_count++;
      break;
    }
    case TokenizerState::SAW_SIGN_TYPE_SPEC:
      if (_is_sign_or_type_specifier(c)) {
        _end_token(*this);
        state = TokenizerState::START;
        break;
      } else {
        // not my char
        cursor_pos--;
        _end_token(*this);
        state = TokenizerState::START;
        continue;
      }
    case TokenizerState::NUMBER_DOT: {
      // two points '..' means something else?
      if (c == '.') {
        cursor_pos -= 2;
        _end_token(*this);
        state = TokenizerState::START;
        continue;
      }
      if (radix != 16 && radix != 10) {
        _invalid_char_error(*this, c);
      } else if (_is_float_specifier(c)) {
        _set_token_id(current_token, TokenId::FLOAT_LIT);
        _end_token(*this);
        state = TokenizerState::START;
        break;
      }
      cursor_pos--;
      state = TokenizerState::FLOAT_FRACTION_NO_UNDERSCORE;
      _set_token_id(current_token, TokenId::FLOAT_LIT);
      continue;
    }
    case TokenizerState::FLOAT_FRACTION_NO_UNDERSCORE:
      if (c == '_') {
        _invalid_char_error(*this, c);
      } else if (_get_digit_value(c) < radix) {
        is_trailing_underscore = false;
        state = TokenizerState::FLOAT_FRACTION;
      }
      LL_FALLTHROUGH
    case TokenizerState::FLOAT_FRACTION: {
      if (c == '_') {
        is_trailing_underscore = true;
        state = TokenizerState::FLOAT_FRACTION_NO_UNDERSCORE;
        break;
      }
      if (_is_exponent_signifier(c, radix)) {
        if (is_trailing_underscore) {
          _invalid_char_error(*this, c);
          break;
        }
        state = TokenizerState::FLOAT_EXPONENT_UNSIGNED;
        radix = 10; // exponent is always base 10
        break;
      }
      uint32_t digit_value = _get_digit_value(c);
      if (digit_value >= radix) {
        if (is_trailing_underscore) {
          _invalid_char_error(*this, c);
          break;
        }

        if (_is_float_specifier(c)) {
          _end_token(*this);
          state = TokenizerState::START;
          break;
        }

        if (_is_symbol_char(c)) {
          _invalid_char_error(*this, c);
        }

        // not my char
        cursor_pos--;
        _end_token(*this);
        state = TokenizerState::START;
        continue;
      }
      // we use parse_f128 to generate the float literal, so just
      // need to get to the end of the token

    } break;
    case TokenizerState::FLOAT_EXPONENT_UNSIGNED:
      switch (c) {
      case '+':
        state = TokenizerState::FLOAT_EXPONENT_NUMBER_NO_UNDERSCORE;
        break;
      case '-':
        state = TokenizerState::FLOAT_EXPONENT_NUMBER_NO_UNDERSCORE;
        break;
      default:
        // reinterpret as normal exponent number
        cursor_pos--;
        state = TokenizerState::FLOAT_EXPONENT_NUMBER_NO_UNDERSCORE;
        continue;
      }
      break;
    case TokenizerState::FLOAT_EXPONENT_NUMBER_NO_UNDERSCORE:
      if (c == '_') {
        _invalid_char_error(*this, c);
      } else if (_get_digit_value(c) < radix) {
        is_trailing_underscore = false;
        state = TokenizerState::FLOAT_EXPONENT_NUMBER;
      }
      LL_FALLTHROUGH
    case TokenizerState::FLOAT_EXPONENT_NUMBER: {
      if (c == '_') {
        is_trailing_underscore = true;
        state = TokenizerState::FLOAT_EXPONENT_NUMBER_NO_UNDERSCORE;
        break;
      }
      uint32_t digit_value = _get_digit_value(c);
      if (digit_value >= radix) {
        if (is_trailing_underscore) {
          _invalid_char_error(*this, c);
          break;
        }

        if (_is_float_specifier(c)) {
          _end_token(*this);
          state = TokenizerState::START;
          break;
        }

        if (_is_symbol_char(c)) {
          _invalid_char_error(*this, c);
        }
        // not my char
        cursor_pos--;
        _end_token(*this);
        state = TokenizerState::START;
        continue;
      }

      // we use parse_f128 to generate the float literal, so just
      // need to get to the end of the token

    } break;
    case TokenizerState::SAW_EQUALS:
      switch (c) {
      case '=':
        _set_token_id(current_token, TokenId::EQUALS);
        _end_token(*this);
        state = TokenizerState::START;
        break;
      default:
        // assign
        cursor_pos--;
        _end_token(*this);
        state = TokenizerState::START;
        continue;
      }
      break;
    case TokenizerState::SAW_PLUS:
      switch (c) {
      case '=':
        _set_token_id(current_token, TokenId::PLUS_ASSIGN);
        _end_token(*this);
        state = TokenizerState::START;
        break;
      case '+':
        _set_token_id(current_token, TokenId::PLUS_PLUS);
        _end_token(*this);
        state = TokenizerState::START;
        break;
      default:
        // just a +
        cursor_pos--;
        _end_token(*this);
        state = TokenizerState::START;
        continue;
      }
      break;
    case TokenizerState::SAW_DASH:
      switch (c) {
      case '=':
        _set_token_id(current_token, TokenId::MINUS_ASSIGN);
        _end_token(*this);
        state = TokenizerState::START;
        break;
      case '-':
        _set_token_id(current_token, TokenId::MINUS_MINUS);
        _end_token(*this);
        state = TokenizerState::START;
        break;
      case '0':
        // negative number
        state = TokenizerState::ZERO;
        _set_token_id(current_token, TokenId::INT_LIT);
        is_trailing_underscore = false;
        radix = 10;
        current_token.int_lit.is_negative = true;
        current_token.int_lit.number[current_token.int_lit.digits_count] = '-';
        current_token.int_lit.digits_count++;
        current_token.int_lit.number[current_token.int_lit.digits_count] = c;
        current_token.int_lit.digits_count++;
        break;
      case DIGIT_NON_ZERO:
        // negative number
        state = TokenizerState::NUMBER;
        _set_token_id(current_token, TokenId::INT_LIT);
        is_trailing_underscore = false;
        radix = 10;
        current_token.int_lit.is_negative = true;
        current_token.int_lit.number[current_token.int_lit.digits_count] = '-';
        current_token.int_lit.digits_count++;
        current_token.int_lit.number[current_token.int_lit.digits_count] = c;
        current_token.int_lit.digits_count++;
        break;
      default:
        // just a -
        cursor_pos--;
        _end_token(*this);
        state = TokenizerState::START;
        continue;
      }
      break;
    case TokenizerState::SAW_STAR:
      switch (c) {
      case '=':
        _set_token_id(current_token, TokenId::MUL_ASSIGN);
        _end_token(*this);
        state = TokenizerState::START;
        break;
      default:
        // just a *
        cursor_pos--;
        _end_token(*this);
        state = TokenizerState::START;
        continue;
      }
      break;
    case TokenizerState::SAW_PERCENT:
      switch (c) {
      case '=':
        _set_token_id(current_token, TokenId::MOD_ASSIGN);
        _end_token(*this);
        state = TokenizerState::START;
        break;
      default:
        // just a %
        cursor_pos--;
        _end_token(*this);
        state = TokenizerState::START;
        continue;
      }
      break;
    case TokenizerState::STRING:
      switch (c) {
      case '"':
        if (is_invalid_token) {
          is_invalid_token = false;
          _set_token_id(current_token, TokenId::ERROR);
        }
        _end_token(*this);
        state = TokenizerState::START;
        break;
      case '\n':
        _tokenize_error(*this, "newline not allowed in string literal");
        is_invalid_token = true;
        state = TokenizerState::STRING;
        break;
      case '\\':
        state = TokenizerState::STRING_ESCAPE;
        break;
      default:
        break;
      }
      break;
    case TokenizerState::CHAR_LITERAL:
      if (c == '\'') {
        _tokenize_error(*this, "expected character");
        _set_token_id(current_token, TokenId::ERROR);
        _end_token(*this);
        state = TokenizerState::START;
      } else if (c == '\\') {
        state = TokenizerState::STRING_ESCAPE;
      } else if ((c >= 0x80 && c <= 0xbf) || c >= 0xf8) {
        // 10xxxxxx
        // 11111xxx
        _invalid_char_error(*this, c);
      } else if (c >= 0xc0 && c <= 0xdf) {
        // 110xxxxx
        current_token.char_lit = c & 0x1f;
        remaining_code_units = 1;
        state = TokenizerState::CHAR_LITERAL_UNICODE;
      } else if (c >= 0xe0 && c <= 0xef) {
        // 1110xxxx
        current_token.char_lit = c & 0x0f;
        remaining_code_units = 2;
        state = TokenizerState::CHAR_LITERAL_UNICODE;
      } else if (c >= 0xf0 && c <= 0xf7) {
        // 11110xxx
        current_token.char_lit = c & 0x07;
        remaining_code_units = 3;
        state = TokenizerState::CHAR_LITERAL_UNICODE;
      } else {
        current_token.char_lit = c;
        state = TokenizerState::CHAR_LITERAL_END;
      }
      break;
    case TokenizerState::CHAR_LITERAL_UNICODE:
      if (c <= 0x7f || c >= 0xc0) {
        _invalid_char_error(*this, c);
      }
      current_token.char_lit <<= 6;
      current_token.char_lit += c & 0x3f;
      remaining_code_units--;
      if (remaining_code_units == 0) {
        state = TokenizerState::CHAR_LITERAL_END;
      }
      break;
    case TokenizerState::CHAR_LITERAL_END:
      switch (c) {
      case '\'':
        _end_token(*this);
        state = TokenizerState::START;
        break;
      default:
        _invalid_char_error(*this, c);
      }
      break;
    case TokenizerState::STRING_ESCAPE:
      switch (c) {
      case 'x':
        state = TokenizerState::CHAR_CODE;
        radix = 16;
        char_code = 0;
        char_code_index = 0;
        unicode = false;
        break;
      case 'u':
        state = TokenizerState::STRING_ESCAPE_UNICODE_START;
        break;
      case 'n':
        _handle_string_escape(*this, '\n');
        break;
      case 'r':
        _handle_string_escape(*this, '\r');
        break;
      case '\\':
        _handle_string_escape(*this, '\\');
        break;
      case 't':
        _handle_string_escape(*this, '\t');
        break;
      case '\'':
        _handle_string_escape(*this, '\'');
        break;
      case '"':
        _handle_string_escape(*this, '\"');
        break;
      default:
        _invalid_char_error(*this, c);
      }
      break;
    case TokenizerState::STRING_ESCAPE_UNICODE_START:
      switch (c) {
      case '{':
        state = TokenizerState::CHAR_CODE;
        radix = 16;
        char_code = 0;
        char_code_index = 0;
        unicode = true;
        break;
      default:
        _invalid_char_error(*this, c);
      }
      break;
    case TokenizerState::CHAR_CODE: {
      if (unicode && c == '}') {
        if (char_code_index == 0) {
          _tokenize_error(*this, "empty unicode escape sequence");
          break;
        }
        if (char_code > 0x10ffff) {
          _tokenize_error(*this, "unicode value out of range: %x", char_code);
          break;
        }
        if (current_token.id == TokenId::UNICODE_CHAR) {
          current_token.char_lit = char_code;
          state = TokenizerState::CHAR_LITERAL_END;
        } else if (char_code <= 0x7f) {
          // 00000000 00000000 00000000 0xxxxxxx
          _handle_string_escape(*this, (uint8_t)char_code);
        } else if (char_code <= 0x7ff) {
          // 00000000 00000000 00000xxx xx000000
          _handle_string_escape(*this, (uint8_t)(0xc0 | (char_code >> 6)));
          // 00000000 00000000 00000000 00xxxxxx
          _handle_string_escape(*this, (uint8_t)(0x80 | (char_code & 0x3f)));
        } else if (char_code <= 0xffff) {
          // 00000000 00000000 xxxx0000 00000000
          _handle_string_escape(*this, (uint8_t)(0xe0 | (char_code >> 12)));
          // 00000000 00000000 0000xxxx xx000000
          _handle_string_escape(*this, (uint8_t)(0x80 | ((char_code >> 6) & 0x3f)));
          // 00000000 00000000 00000000 00xxxxxx
          _handle_string_escape(*this, (uint8_t)(0x80 | (char_code & 0x3f)));
        } else if (char_code <= 0x10ffff) {
          // 00000000 000xxx00 00000000 00000000
          _handle_string_escape(*this, (uint8_t)(0xf0 | (char_code >> 18)));
          // 00000000 000000xx xxxx0000 00000000
          _handle_string_escape(*this, (uint8_t)(0x80 | ((char_code >> 12) & 0x3f)));
          // 00000000 00000000 0000xxxx xx000000
          _handle_string_escape(*this, (uint8_t)(0x80 | ((char_code >> 6) & 0x3f)));
          // 00000000 00000000 00000000 00xxxxxx
          _handle_string_escape(*this, (uint8_t)(0x80 | (char_code & 0x3f)));
        } else {
          LL_UNREACHEABLE;
        }
        break;
      }

      uint32_t digit_value = _get_digit_value(c);
      if (digit_value >= radix) {
        _tokenize_error(*this, "invalid digit: '%c'", c);
        break;
      }
      char_code *= radix;
      char_code += digit_value;
      char_code_index += 1;

      if (!unicode && char_code_index >= 2) {
        assert(char_code <= 255);
        _handle_string_escape(*this, (uint8_t)char_code);
      }
    } break;
    case TokenizerState::SAW_NOT:
      if (c == '=') {
        _set_token_id(current_token, TokenId::NOT_EQUALS);
        _end_token(*this);
        state = TokenizerState::START;
        break;
      }
      // just a !
      cursor_pos--;
      _end_token(*this);
      state = TokenizerState::START;
      continue;
    case TokenizerState::SAW_VERTICAL_BAR:
      if (c == '|') {
        _set_token_id(current_token, TokenId::OR);
        _end_token(*this);
        state = TokenizerState::START;
        break;
      }
      // just a |
      cursor_pos--;
      _end_token(*this);
      state = TokenizerState::START;
      continue;
    case TokenizerState::SAW_AMPERSAND:
      if (c == '&') {
        _set_token_id(current_token, TokenId::AND);

        _end_token(*this);
        state = TokenizerState::START;
        break;
      }
      // just a &
      cursor_pos--;
      _end_token(*this);
      state = TokenizerState::START;
      continue;
    case TokenizerState::SAW_LESS:
      if (c == '<') {
        _set_token_id(current_token, TokenId::LSHIFT);
        _end_token(*this);
        state = TokenizerState::START;
        break;
      }
      if (c == '=') {
        _set_token_id(current_token, TokenId::LESS_OR_EQUALS);
        _end_token(*this);
        state = TokenizerState::START;
        break;
      }
      // just a <
      cursor_pos--;
      _end_token(*this);
      state = TokenizerState::START;
      continue;
    case TokenizerState::SAW_GREATER:
      if (c == '>') {
        _set_token_id(current_token, TokenId::RSHIFT);
        _end_token(*this);
        state = TokenizerState::START;
        break;
      }
      if (c == '=') {
        _set_token_id(current_token, TokenId::GREATER_OR_EQUALS);
        _end_token(*this);
        state = TokenizerState::START;
        break;
      }
      // just a >
      cursor_pos--;
      _end_token(*this);
      state = TokenizerState::START;
      continue;
    // If error just get to the next token
    case TokenizerState::ERROR:
      break;

    default:
      LL_UNREACHEABLE;
      break;
    }

    // Handle end of line
    if (c == '\n')
      _reset_line(*this);
    // else we just move a column
    else
      current_column++;
  }
  cursor_pos--;

  // EOF
  switch (state) {
  case TokenizerState::START:
  case TokenizerState::ERROR:
  case TokenizerState::LINE_COMMENT:
    break;
  case TokenizerState::SYMBOL:
    if (is_invalid_token) {
      is_invalid_token = false;
      _set_token_id(current_token, TokenId::ERROR);
      _end_token(*this);
    } else {
      _end_token_check_is_keyword(*this);
    }
    break;
  case TokenizerState::ZERO:
  case TokenizerState::NUMBER:
    if (is_invalid_token) {
      is_invalid_token = false;
      _set_token_id(current_token, TokenId::ERROR);
    }
    _end_token(*this);
    break;
  case TokenizerState::NUMBER_DOT:
    _set_token_id(current_token, TokenId::FLOAT_LIT);
    LL_FALLTHROUGH
  case TokenizerState::SAW_SIGN_TYPE_SPEC:
  case TokenizerState::FLOAT_FRACTION:
  case TokenizerState::FLOAT_EXPONENT_UNSIGNED:
  case TokenizerState::FLOAT_EXPONENT_NUMBER:
  case TokenizerState::NUMBER_NO_UNDERSCORE:
  case TokenizerState::FLOAT_FRACTION_NO_UNDERSCORE:
  case TokenizerState::FLOAT_EXPONENT_NUMBER_NO_UNDERSCORE:
  case TokenizerState::SAW_STAR:
  case TokenizerState::SAW_SLASH:
  case TokenizerState::SAW_PERCENT:
  case TokenizerState::SAW_PLUS:
  case TokenizerState::SAW_DASH:
  case TokenizerState::SAW_EQUALS:
  case TokenizerState::DOC_COMMENT:
    _end_token(*this);
    break;
  case TokenizerState::STRING:
    _tokenize_error(*this, "unterminated string literal");
    break;
  case TokenizerState::CHAR_LITERAL:
    _tokenize_error(*this, "unterminated Unicode point literal");
    break;
  case TokenizerState::SAW_STAR_DOC_COMMENT:
    _tokenize_error(*this, "unexpected EOF");
    break;
  default:
    LL_UNREACHEABLE;
  }

  _begin_token(*this, TokenId::_EOF);
  _end_token(*this);
}

const bool Lexer::has_tokens() const noexcept { return tokens.size() - (curr_index + 1) != 0; }

const Token &Lexer::get_previous_token() const noexcept { return tokens.at(curr_index - 1); }

const Token &Lexer::get_next_token() const noexcept { return tokens.at(++curr_index); }

void Lexer::get_back() const noexcept { curr_index--; }

std::string_view Lexer::get_token_value(const Token &token) const noexcept {
  auto len = token.end_pos - token.start_pos + 1;
  return std::string_view(source.data() + token.start_pos, len);
}

void _begin_token(Lexer &in_lexer, const TokenId id) noexcept {
  in_lexer.current_token = Token(in_lexer.file_name);
  in_lexer.current_token.id = id;
  in_lexer.current_token.start_line = in_lexer.current_line;
  in_lexer.current_token.start_column = in_lexer.current_column;
  in_lexer.current_token.start_pos = in_lexer.cursor_pos;
  in_lexer.current_token.file_name = in_lexer.file_name;
}

void _set_token_id(Token &in_token, const TokenId id) noexcept { in_token.id = id; }

void _end_token(Lexer &in_lexer) noexcept {
  in_lexer.current_token.end_pos = in_lexer.cursor_pos;

  switch (in_lexer.current_token.id) {
  case TokenId::DOC_COMMENT:
    in_lexer.comments.push_back(in_lexer.current_token);
    break;
  default:
    in_lexer.tokens.push_back(in_lexer.current_token);
    break;
  }
}

void _end_token_check_is_keyword(Lexer &in_lexer) noexcept {
  in_lexer.current_token.end_pos = in_lexer.cursor_pos;

  _is_keyword(in_lexer.current_token, in_lexer.get_token_value(in_lexer.current_token));

  switch (in_lexer.current_token.id) {
  case TokenId::EXTERN:
  case TokenId::FN:
  case TokenId::RET:
  case TokenId::IF:
  case TokenId::ELSE:
  case TokenId::ELIF:
  case TokenId::AND:
  case TokenId::OR:
  case TokenId::TRUE:
  case TokenId::FALSE:
  case TokenId::IDENTIFIER:
    in_lexer.tokens.push_back(in_lexer.current_token);
    break;
  default:
    LL_UNREACHEABLE;
  }
}

void _reset_line(Lexer &in_lexer) noexcept {
  in_lexer.current_line++;
  in_lexer.current_column = 0;
}

void _invalid_char_error(Lexer &in_lexer, uint8_t c) noexcept {
  if (c == '\r') {
    _tokenize_error(in_lexer, "invalid carriage return, only '\\n' line endings are supported");
    return;
  }

  const char *sh = _get_escape_shorthand(c);
  if (sh) {
    _tokenize_error(in_lexer, "invalid character: '%s'", sh);
    return;
  }

  if (isprint(c)) {
    _tokenize_error(in_lexer, "invalid character: '%c'", c);
    return;
  }

  _tokenize_error(in_lexer, "invalid character: '\\x%02x'", c);
}

void _tokenize_error(Lexer &in_lexer, const char *format, ...) noexcept {
  in_lexer.state = TokenizerState::ERROR;

  va_list ap, ap2;
  va_start(ap, format);
  va_copy(ap2, ap);

  int len1 = snprintf(nullptr, 0, format, ap);
  assert(len1 >= 0);

  std::string msg;
  msg.reserve(len1 + 1);

  int len2 = snprintf(msg.data(), msg.capacity(), format, ap2);
  assert(len2 >= 0);
  assert(len2 == len1);

  Error error(ERROR_TYPE::ERROR, in_lexer.current_line, in_lexer.current_column, in_lexer.file_name, msg);

  in_lexer.errors.push_back(error);

  va_end(ap2);
  va_end(ap);
}

void _handle_string_escape(Lexer &in_lexer, uint8_t c) noexcept {
  if (in_lexer.current_token.id == TokenId::UNICODE_CHAR) {
    in_lexer.current_token.char_lit = c;

    in_lexer.state = TokenizerState::CHAR_LITERAL_END;
  } else if (in_lexer.current_token.id == TokenId::STRING) {
    in_lexer.state = TokenizerState::STRING;
  } else {
    LL_UNREACHEABLE;
  }
}

static std::unordered_map<std::string_view, TokenId> keywords = {
  { "extern", TokenId::EXTERN }, { "fn", TokenId::FN },      { "ret", TokenId::RET },   { "and", TokenId::AND },
  { "if", TokenId::IF },         { "elif", TokenId::ELIF },  { "else", TokenId::ELSE }, { "or", TokenId::OR },
  { "true", TokenId::TRUE },     { "false", TokenId::FALSE }
};

void _is_keyword(Token &in_token, std::string_view value) noexcept {
  auto len = in_token.end_pos - in_token.start_pos + 1;
  if (keywords.find(value) != keywords.end()) {
    _set_token_id(in_token, keywords.at(value));
  }
}

uint32_t _get_digit_value(uint8_t c) {
  if ('0' <= c && c <= '9') {
    return c - '0';
  }
  if ('A' <= c && c <= 'Z') {
    return c - 'A' + 10;
  }
  if ('a' <= c && c <= 'z') {
    return c - 'a' + 10;
  }
  return UINT32_MAX;
}

bool _is_symbol_char(uint8_t c) {
  switch (c) {
  case SYMBOL_CHAR:
    return true;
  default:
    return false;
  }
}

bool _is_reserved_char(uint8_t c) {
  switch (c) {
  case WHITESPACE:
  case '(':
  case ')':
  case ',':
    return true;
  default:
    return false;
  }
}

bool _is_float_specifier(uint8_t c) { return c == 'f'; }

bool _is_sign_or_type_specifier(uint8_t c) {
  switch (c) {
  case 'u':
  case 'b':
  case 'w':
  case 'l':
    return true;
  default:
    return false;
  }
}

bool _is_exponent_signifier(uint8_t c, int radix) {
  if (radix == 16) {
    return c == 'p' || c == 'P';
  } else {
    return c == 'e' || c == 'E';
  }
}

static const char *_get_escape_shorthand(uint8_t c) {
  switch (c) {
  case '\0':
    return "\\0";
  case '\a':
    return "\\a";
  case '\b':
    return "\\b";
  case '\t':
    return "\\t";
  case '\n':
    return "\\n";
  case '\v':
    return "\\v";
  case '\f':
    return "\\f";
  case '\r':
    return "\\r";
  default:
    return nullptr;
  }
}

static const char *token_id_names[] = { "HASH",
                                        "FUNC",
                                        "RET",
                                        "L_PAREN",
                                        "R_PAREN",
                                        "L_CURLY",
                                        "R_CURLY",
                                        "L_BRACKET",
                                        "R_BRACKET",
                                        "COMMA",
                                        "SEMI",
                                        "COLON",
                                        "DOT",
                                        "ASSIGN",
                                        "PLUS_PLUS",
                                        "MINUS_MINUS",
                                        "NOT",
                                        "OR",
                                        "AND",
                                        "EQUALS",
                                        "NOT_EQUALS",
                                        "LESS",
                                        "LESS_OR_EQUALS",
                                        "GREATER",
                                        "GREATER_OR_EQUALS",
                                        "PLUS",
                                        "MINUS",
                                        "MUL",
                                        "DIV",
                                        "MOD",
                                        "PLUS_ASSIGN",
                                        "MINUS_ASSIGN",
                                        "MUL_ASSIGN",
                                        "DIV_ASSIGN",
                                        "MOD_ASSIGN",
                                        "BIT_AND",
                                        "BIT_XOR",
                                        "BIT_OR",
                                        "BIT_NOT",
                                        "LSHIFT",
                                        "RSHIFT",
                                        "IDENTIFIER",
                                        "INT_LIT",
                                        "FLOAT_LIT",
                                        "ESCAPED_VALUE",
                                        "STRING",
                                        "UNICODE_CHAR",
                                        "WS",
                                        "DOC_COMMENT",
                                        "LINE_COMMENT",
                                        "ERROR",
                                        "EOF" };

inline const char *token_id_name(TokenId id) { return token_id_names[(size_t)id]; }

size_t token_spaces(const size_t value_size, const size_t id_name_size) {
  return value_size >= id_name_size ? value_size - id_name_size : id_name_size - value_size;
}

struct TokenPrintInfo {
  std::string id_name;
  size_t value_size;
  size_t token_name_size;
  bool is_value;
  TokenPrintInfo(const size_t in_value_size, const size_t in_token_name_size, const std::string &in_id_name,
                 const bool in_is_value)
      : value_size(in_value_size), token_name_size(in_token_name_size), id_name(in_id_name), is_value(in_is_value) {}
};

/* Used by print_tokens */
std::string create_values_line(const std::string &source, const size_t start, const size_t end,
                               const std::vector<Token> &tokens, const std::vector<TokenPrintInfo> &token_infos) {
  std::string line;
  for (size_t j = start; j < end; j++) {
    auto &token_info = token_infos.at(j);
    size_t is_value = (size_t)!token_info.is_value;

    auto spaces_count = is_value * token_spaces(token_info.value_size, token_info.token_name_size);
    auto spaces = std::string(spaces_count, ' ');

    std::string value;
    auto &token = tokens.at(j);
    auto len = token.end_pos - token.start_pos;
    auto token_value = std::string_view(source.data() + token.start_pos, len);

    if (token.id == TokenId::DOC_COMMENT) {
      for (char c : token_value) {
        switch (c) {
        case '\a':
          value.append("\\a");
          break;
        case '\b':
          value.append("\\b");
          break;
        case '\f':
          value.append("\\f");
          break;
        case '\n':
          value.append("\\n");
          break;
        case '\r':
          value.append("\\r");
          break;
        case '\t':
          value.append("\\t");
          break;
        case '\v':
          value.append("\\v");
          break;
        default:
          value += c;
          break;
        }
      }
    } else {
      value = token_value;
    }

    line.append(value + spaces);

    // if not final token add space
    if (j < end - 1)
      line.append(" ");
  }
  return line;
}

/* Used by print_tokens */
std::string create_id_names_line(const size_t start, const size_t end, const std::vector<TokenPrintInfo> &token_infos) {
  std::string line;
  for (size_t j = start; j < end; j++) {
    auto &token_info = token_infos.at(j);
    size_t is_value = (size_t)token_info.is_value;

    auto spaces_count = is_value * token_spaces(token_info.value_size, token_info.token_name_size);
    auto spaces = std::string(spaces_count, ' ');

    auto token_name = "|" + token_info.id_name + "|";
    line.append(token_name + spaces);

    // if not final token add space
    if (j < end - 1)
      line.append(" ");
  }
  return line;
}

Console print_tokens(Lexer &lexer) {
  auto &tokens = lexer.tokens;
  Console console;

  size_t total_size = 0;
  std::vector<TokenPrintInfo> token_infos;
  token_infos.reserve(tokens.size());

  size_t last_it = 0;
  for (size_t i = 0; i < tokens.size(); i++) {
    auto &token = tokens.at(i);

    auto token_name = std::string(token_id_name(token.id));
    auto token_value_size = token.get_value_size();
    auto token_name_size = token_name.size() + 2;

    auto &token_info =
      token_infos.emplace_back(token_value_size, token_name_size, token_name, token_value_size > token_name_size);

    auto size = token_info.is_value ? token_value_size : token_name_size;

    if (total_size + size > 80) {
      console.push_back(create_values_line(lexer.source, last_it, i, tokens, token_infos));
      console.push_back(create_id_names_line(last_it, i, token_infos));
      last_it = i;
      total_size = 0;
    } else {
      total_size += size;
      if (i == tokens.size() - 1) {
        console.push_back(create_values_line(lexer.source, last_it, tokens.size(), tokens, token_infos));
        console.push_back(create_id_names_line(last_it, tokens.size(), token_infos));
      }
    }
  }

  return console;
}
