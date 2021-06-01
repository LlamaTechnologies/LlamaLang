#include "Tokenize.hpp"
#include <fstream>
#include <cassert>
#include <cstdarg>
#include <unordered_map>

#define WHITESPACE\
        ' ':\
    case '\r':\
    case '\t':\
    case '\n'


// [1-9]
#define DIGIT_NON_ZERO \
         '1': \
    case '2': \
    case '3': \
    case '4': \
    case '5': \
    case '6': \
    case '7': \
    case '8': \
    case '9'

// [0-9]
#define DIGIT \
         '0': \
    case DIGIT_NON_ZERO

// [a-zA-Z]
#define ALPHA \
         'a': \
    case 'b': \
    case 'c': \
    case 'd': \
    case 'e': \
    case 'f': \
    case 'g': \
    case 'h': \
    case 'i': \
    case 'j': \
    case 'k': \
    case 'l': \
    case 'm': \
    case 'n': \
    case 'o': \
    case 'p': \
    case 'q': \
    case 'r': \
    case 's': \
    case 't': \
    case 'u': \
    case 'v': \
    case 'w': \
    case 'x': \
    case 'y': \
    case 'z': \
    case 'A': \
    case 'B': \
    case 'C': \
    case 'D': \
    case 'E': \
    case 'F': \
    case 'G': \
    case 'H': \
    case 'I': \
    case 'J': \
    case 'K': \
    case 'L': \
    case 'M': \
    case 'N': \
    case 'O': \
    case 'P': \
    case 'Q': \
    case 'R': \
    case 'S': \
    case 'T': \
    case 'U': \
    case 'V': \
    case 'W': \
    case 'X': \
    case 'Y': \
    case 'Z'

// [a-zA-Z_]
#define SYMBOL_START \
    ALPHA: \
    case '_'

// [a-zA-Z_0-9]
#define SYMBOL_CHAR \
    ALPHA: \
    case DIGIT: \
    case '_'


static uint32_t get_digit_value(uint8_t c);
static const char* get_escape_shorthand(uint8_t c);
static bool is_symbol_char(uint8_t c);
static bool is_whitespace_char(uint8_t c);
static bool is_float_specifier(uint8_t c);
static bool is_sign_or_type_specifier(uint8_t c);
static bool is_exponent_signifier(uint8_t c, int radix);

Lexer::Lexer(const std::string& _file_name, std::vector<Error>& _errors)
    : file_name(_file_name), errors(_errors), cursor_pos(0L), curr_index(SIZE_MAX),
    curr_line(0L), curr_column(0L), state(TokenizerState::Start),
    radix(10), is_trailing_underscore(false), is_invalid_token(false),
    curr_token(), tokens_vec(), comments_vec()
{
    // open file
    auto file = std::ifstream(file_name);
    if (file.is_open() && file.good()) {
        // go to end of file
        file.seekg(0, std::ios::end);
        
        // Reserve memory
        source.reserve(file.tellg());
        
        // reset file cursor
        file.seekg(0, std::ios::beg);
        
        // store content in this.source
        source.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }
    file.close();
}

Lexer::Lexer(const std::string& _src_file, const std::string& _file_name, std::vector<Error>& _errors)
    : file_name(_file_name), source(_src_file), errors(_errors), cursor_pos(0L), curr_index(SIZE_MAX),
    curr_line(0L), curr_column(0L), state(TokenizerState::Start),
    radix(10), is_trailing_underscore(false), is_invalid_token(false),
    curr_token(), tokens_vec(), comments_vec()
{}

// IMPORTANT!: should not be called more than once after the constructor.
void Lexer::tokenize() noexcept
{
    // reading file while no errors in it
    for (/*cursor_pos = 0*/; cursor_pos < source.size(); cursor_pos++) {
        char c = source[cursor_pos];

        switch (state)
        {
            // begining of the source file
        case TokenizerState::Start:
            switch (c)
            {
            case WHITESPACE:
                break;
                // we faund a symbol
            case SYMBOL_START:
                state = TokenizerState::Symbol;
                begin_token(TokenId::IDENTIFIER);
                break;
                // we found a 0 so we have to tokenize a 
                // octal: 0o[0-7]...
                // binary:  0b[0-1]...
                // hex: 0x[0-9A-F]...
                // trailing 0 decimal: 0[1-9]...
            case '0':
                state = TokenizerState::Zero;
                begin_token(TokenId::INT_LIT);
                is_trailing_underscore = false;
                radix = 10;
                bigint_init_unsigned(&curr_token.int_lit, 0);
                break;
            case DIGIT_NON_ZERO:
                state = TokenizerState::Number;
                begin_token(TokenId::INT_LIT);
                is_trailing_underscore = false;
                radix = 10;
                bigint_init_unsigned(&curr_token.int_lit, get_digit_value(c));
                break;
                // we found the beginning of a string literal
            case '"':
                begin_token(TokenId::STRING);
                state = TokenizerState::String;
                break;
                // we found the beginning of a char literal
            case '\'':
                begin_token(TokenId::UNICODE_CHAR);
                state = TokenizerState::CharLiteral;
                break;
                // we found
                // ASSIGN =
                // EQUALS ==
            case '=':
                begin_token(TokenId::ASSIGN);
                state = TokenizerState::SawEq;
                break;
                // we found 
                // a DIV
                // /=
                // the beginning of a DOC comment
                // the beginning of a line comment
            case '/':
                begin_token(TokenId::DIV);
                state = TokenizerState::SawSlash;
                break;
                // we found
                // MUL
                // *=
            case '*':
                begin_token(TokenId::MUL);
                state = TokenizerState::SawStar;
                break;
                // we found
                // a PLUS
                // +=
                // ++
            case '+':
                begin_token(TokenId::PLUS);
                state = TokenizerState::SawPlus;
                break;
                // we found
                // a MINUS
                // -=
                // --
            case '-':
                begin_token(TokenId::MINUS);
                state = TokenizerState::SawDash;
                break;
                // we found
                // MOD
                // %=
            case '%':
                begin_token(TokenId::MOD);
                state = TokenizerState::SawPercent;
                break;
                // we found a hash
            case '#':
                begin_token(TokenId::HASH);
                end_token();
                break;
                // we found a semicolon
            case ';':
                begin_token(TokenId::SEMI);
                
                end_token();
                break;
            case '(':
                begin_token(TokenId::L_PAREN);
                end_token();
                break;
            case ')':
                begin_token(TokenId::R_PAREN);
                end_token();
                break;
            case '{':
                begin_token(TokenId::L_CURLY);
                end_token();
                break;
            case '}':
                begin_token(TokenId::R_CURLY);
                end_token();
                break;
            case '[':
                begin_token(TokenId::L_BRACKET);
                end_token();
                break;
            case ']':
                begin_token(TokenId::R_BRACKET);
                end_token();
                break;
            case '.':
                begin_token(TokenId::FLOAT_LIT);
                state = TokenizerState::NumberDot;
                break;
            case '!':
                begin_token(TokenId::NOT);
                state = TokenizerState::SawNot;
                break;
            case '|':
                begin_token(TokenId::BIT_OR);
                state = TokenizerState::SawVerticalBar;
                break;
            case '&':
                begin_token(TokenId::BIT_AND);
                state = TokenizerState::SawAmpersand;
                break;
            case '~':
                begin_token(TokenId::BIT_NOT);
                end_token();
                break;
            case '^':
                begin_token(TokenId::BIT_XOR);
                end_token();
                break;
            case '<':
                begin_token(TokenId::LESS);
                state = TokenizerState::SawLess;
                break;
            case '>':
                begin_token(TokenId::GREATER);
                state = TokenizerState::SawGreater;
                break;
            default:
                tokenize_error("Unidentified character in symbol %c", c);
                is_invalid_token = true;
                state = TokenizerState::Symbol;
                break;
            }
            break;
            // TOKENIZING A IDENTIFIER
        case TokenizerState::Symbol:
            switch (c) {
            case SYMBOL_CHAR:
                break;
            default:
                cursor_pos--;
                
                if (is_invalid_token) {
                    is_invalid_token = false;
                    set_token_id(TokenId::ERROR);
                    end_token();
                }
                else {
                    end_token_check_is_keyword();
                }
                
                state = TokenizerState::Start;
                continue;
            }
            break;
            // TRYING TO TOKENIZE /...
        case TokenizerState::SawSlash:
            switch (c) {
                // line comment //
            case '/':
                state = TokenizerState::LineComment;
                break;
                // doc comment /* */
            case '*':
                set_token_id(TokenId::DOC_COMMENT);
                state = TokenizerState::DocComment;
                break;
                // DIV_ASSIGN /=
            case '=':
                set_token_id(TokenId::DIV_ASSIGN);
                end_token();
                state = TokenizerState::Start;
                break;
                // DIV
            default:
                cursor_pos -= 1;
                end_token();
                state = TokenizerState::Start;
                continue;
            }
            break;
            // TOKENIZING A DOC COMMENT
        case TokenizerState::DocComment:
            switch (c) {
            case '*':
                state = TokenizerState::SawStarDocComment;
                break;
            default:
                break;
            }
            break;
        case TokenizerState::SawStarDocComment:
            switch (c)
            {
            case '/':
                end_token();
                state = TokenizerState::Start;
                break;
            default:
                state = TokenizerState::DocComment;
                break;
            }
            break;
            // TOKENIZING A LINE COMMENT
        case TokenizerState::LineComment:
            switch (c) {
            case '\n':
                state = TokenizerState::Start;
                break;
            default:
                break;
            }
            break;
            // Saw a 0
        case TokenizerState::Zero:
            switch (c) {
            case 'b':
                radix = 2;
                state = TokenizerState::NumberNoUnderscore;
                break;
            case 'o':
                radix = 8;
                state = TokenizerState::NumberNoUnderscore;
                break;
            case 'x':
                radix = 16;
                state = TokenizerState::NumberNoUnderscore;
                break;
            default:
                // reinterpret as normal number
                cursor_pos--;
                state = TokenizerState::Number;
                continue;
            }
            break;
            // saw a number that is not 0
        case TokenizerState::NumberNoUnderscore:
            // cant have two undersocores in a number
            if (c == '_') {
                invalid_char_error(c);
                state = TokenizerState::NumberNoUnderscore;
                is_invalid_token = true;
                break;
            }
            // get the digit
            else if (get_digit_value(c) < radix) {
                is_trailing_underscore = false;
                state = TokenizerState::Number;
            }
            __fallthrough;
            // we saw a number that my have '.' '_' or [eEpP]
        case TokenizerState::Number:
        {
            // underscore in number
            if (c == '_') {
                is_trailing_underscore = true;
                state = TokenizerState::NumberNoUnderscore;
                break;
            }
            // dot in number
            if (c == '.') {
                // _. not allowed
                if (is_trailing_underscore) {
                    invalid_char_error(c);
                    break;
                }
                state = TokenizerState::NumberDot;
                break;
            }
            // is eE or pP
            if (is_exponent_signifier(c, radix)) {
                // _[eEpP] not allowed
                if (is_trailing_underscore) {
                    invalid_char_error(c);
                    break;
                }
                // only accept hex and dec numbers to have exp
                if (radix != 16 && radix != 10) {
                    invalid_char_error(c);
                }
                state = TokenizerState::FloatExponentUnsigned;
                radix = 10; // exponent is always base 10
                assert(curr_token.id == TokenId::INT_LIT);
                set_token_id(TokenId::FLOAT_LIT);
                
                break;
            }
            uint32_t digit_value = get_digit_value(c);
            if (digit_value >= radix) {
                if (is_sign_or_type_specifier(c)) {
                    state = TokenizerState::SawSignOrTypeSpec;
                    break;
                }
                else if (is_trailing_underscore) {
                    invalid_char_error(c);
                    is_invalid_token = true;
                    state = TokenizerState::Symbol;
                    break;
                } else if ( is_symbol_char(c) || (!isxdigit(c) && !is_whitespace_char(c)) ) {
                    invalid_char_error(c);
                    is_invalid_token = true;
                    state = TokenizerState::Symbol;
                    break;
                } else {
                    state = TokenizerState::Start;
                    // not my char
                    cursor_pos--;
                    
                    if (is_invalid_token) {
                        is_invalid_token = false;
                        set_token_id(TokenId::ERROR);
                    }

                    end_token();
                    continue;
                }
            }
            BigInt digit_value_bi;
            bigint_init_unsigned(&digit_value_bi, digit_value);

            BigInt radix_bi;
            bigint_init_unsigned(&radix_bi, radix);

            BigInt multiplied;
            bigint_mul(&multiplied, &curr_token.int_lit, &radix_bi);

            bigint_add(&curr_token.int_lit, &multiplied, &digit_value_bi);
            
            break;
        }
        case TokenizerState::SawSignOrTypeSpec:
            if (is_sign_or_type_specifier(c)) {
                end_token();
                state = TokenizerState::Start;
                break;
            }
            else {
                // not my char
                cursor_pos--;
                end_token();
                state = TokenizerState::Start;
                continue;
            }
        case TokenizerState::NumberDot:
        {
            // two points '..' means something else?
            if (c == '.') {
                cursor_pos -= 2;
                end_token();
                state = TokenizerState::Start;
                continue;
            }
            if (radix != 16 && radix != 10) {
                invalid_char_error(c);
            }
            else if (is_float_specifier(c)) {
                set_token_id(TokenId::FLOAT_LIT);
                end_token();
                state = TokenizerState::Start;
                break;
            }
            cursor_pos--;
            state = TokenizerState::FloatFractionNoUnderscore;
            set_token_id(TokenId::FLOAT_LIT);
            continue;
        }
        case TokenizerState::FloatFractionNoUnderscore:
            if (c == '_') {
                invalid_char_error(c);
            }
            else if (get_digit_value(c) < radix) {
                is_trailing_underscore = false;
                state = TokenizerState::FloatFraction;
            }
            __fallthrough;
        case TokenizerState::FloatFraction:
        {
            if (c == '_') {
                is_trailing_underscore = true;
                state = TokenizerState::FloatFractionNoUnderscore;
                break;
            }
            if (is_exponent_signifier(c, radix)) {
                if (is_trailing_underscore) {
                    invalid_char_error(c);
                    break;
                }
                state = TokenizerState::FloatExponentUnsigned;
                radix = 10; // exponent is always base 10
                break;
            }
            uint32_t digit_value = get_digit_value(c);
            if (digit_value >= radix) {
                if (is_trailing_underscore) {
                    invalid_char_error(c);
                    break;
                } 
                
                if (is_float_specifier(c)) {
                    end_token();
                    state = TokenizerState::Start;
                    break;
                }
                
                if (is_symbol_char(c)) {
                    invalid_char_error(c);
                }

                // not my char
                cursor_pos--;
                end_token();
                state = TokenizerState::Start;
                continue;
            }
            // we use parse_f128 to generate the float literal, so just
            // need to get to the end of the token
            
        }
        break;
        case TokenizerState::FloatExponentUnsigned:
            switch (c) {
            case '+':
                state = TokenizerState::FloatExponentNumberNoUnderscore;
                break;
            case '-':
                state = TokenizerState::FloatExponentNumberNoUnderscore;
                break;
            default:
                // reinterpret as normal exponent number
                cursor_pos--;
                state = TokenizerState::FloatExponentNumberNoUnderscore;
                continue;
            }
            break;
        case TokenizerState::FloatExponentNumberNoUnderscore:
            if (c == '_') {
                invalid_char_error(c);
            }
            else if (get_digit_value(c) < radix) {
                is_trailing_underscore = false;
                state = TokenizerState::FloatExponentNumber;
            }
            __fallthrough;
        case TokenizerState::FloatExponentNumber:
        {
            if (c == '_') {
                is_trailing_underscore = true;
                state = TokenizerState::FloatExponentNumberNoUnderscore;
                break;
            }
            uint32_t digit_value = get_digit_value(c);
            if (digit_value >= radix) {
                if (is_trailing_underscore) {
                    invalid_char_error(c);
                    break;
                }

                if (is_float_specifier(c)) {
                    end_token();
                    state = TokenizerState::Start;
                    break;
                }

                if (is_symbol_char(c)) {
                    invalid_char_error(c);
                }
                // not my char
                cursor_pos--;
                end_token();
                state = TokenizerState::Start;
                continue;
            }

            // we use parse_f128 to generate the float literal, so just
            // need to get to the end of the token
            
        }
        break;
        case TokenizerState::SawEq:
            switch (c) {
            case '=':
                set_token_id(TokenId::EQUALS);
                end_token();
                state = TokenizerState::Start;
                break;
            default:
                // assign
                cursor_pos--;
                end_token();
                state = TokenizerState::Start;
                continue;
            }
            break;
        case TokenizerState::SawPlus:
            switch (c) {
            case '=':
                set_token_id(TokenId::PLUS_ASSIGN);
                end_token();
                state = TokenizerState::Start;
                break;
            case '+':
                set_token_id(TokenId::PLUS_PLUS);
                end_token();
                state = TokenizerState::Start;
                break;
            default:
                // just a +
                cursor_pos--;
                end_token();
                state = TokenizerState::Start;
                continue;
            }
            break;
        case TokenizerState::SawDash:
            switch (c) {
            case '=':
                set_token_id(TokenId::MINUS_ASSIGN);
                end_token();
                state = TokenizerState::Start;
                break;
            case '-':
                set_token_id(TokenId::MINUS_MINUS);
                end_token();
                state = TokenizerState::Start;
                break;
            default:
                // just a -
                cursor_pos--;
                end_token();
                state = TokenizerState::Start;
                continue;
            }
            break;
        case TokenizerState::SawStar:
            switch (c) {
            case '=':
                set_token_id(TokenId::MUL_ASSIGN);
                end_token();
                state = TokenizerState::Start;
                break;
            default:
                // just a *
                cursor_pos--;
                end_token();
                state = TokenizerState::Start;
                continue;
            }
            break;
        case TokenizerState::SawPercent:
            switch (c) {
            case '=':
                set_token_id(TokenId::MOD_ASSIGN);
                end_token();
                state = TokenizerState::Start;
                break;
            default:
                // just a %
                cursor_pos--;
                end_token();
                state = TokenizerState::Start;
                continue;
            }
            break;
        case TokenizerState::String:
            switch (c) {
            case '"':
                if (is_invalid_token) {
                    is_invalid_token = false;
                    set_token_id(TokenId::ERROR);
                }
                end_token();
                state = TokenizerState::Start;
                break;
            case '\n':
                tokenize_error("newline not allowed in string literal");
                is_invalid_token = true;
                state = TokenizerState::String;
                break;
            case '\\':
                state = TokenizerState::StringEscape;
                break;
            default:
                break;
            }
            break;
        case TokenizerState::CharLiteral:
            if (c == '\'') {
                tokenize_error("expected character");
                set_token_id(TokenId::ERROR);
                end_token();
                state = TokenizerState::Start;
            }
            else if (c == '\\') {
                state = TokenizerState::StringEscape;
            }
            else if ((c >= 0x80 && c <= 0xbf) || c >= 0xf8) {
                // 10xxxxxx
                // 11111xxx
                invalid_char_error(c);
            }
            else if (c >= 0xc0 && c <= 0xdf) {
                // 110xxxxx
                curr_token.char_lit = c & 0x1f;
                remaining_code_units = 1;
                state = TokenizerState::CharLiteralUnicode;
            }
            else if (c >= 0xe0 && c <= 0xef) {
                // 1110xxxx
                curr_token.char_lit = c & 0x0f;
                remaining_code_units = 2;
                state = TokenizerState::CharLiteralUnicode;
            }
            else if (c >= 0xf0 && c <= 0xf7) {
                // 11110xxx
                curr_token.char_lit = c & 0x07;
                remaining_code_units = 3;
                state = TokenizerState::CharLiteralUnicode;
            }
            else {
                curr_token.char_lit = c;
                state = TokenizerState::CharLiteralEnd;
            }
            break;
        case TokenizerState::CharLiteralUnicode:
            if (c <= 0x7f || c >= 0xc0) {
                invalid_char_error(c);
            }
            curr_token.char_lit <<= 6;
            curr_token.char_lit += c & 0x3f;
            remaining_code_units--;
            if (remaining_code_units == 0) {
                state = TokenizerState::CharLiteralEnd;
            }
            break;
        case TokenizerState::CharLiteralEnd:
            switch (c) {
            case '\'':
                end_token();
                state = TokenizerState::Start;
                break;
            default:
                invalid_char_error(c);
            }
            break;
        case TokenizerState::StringEscape:
            switch (c) {
            case 'x':
                state = TokenizerState::CharCode;
                radix = 16;
                char_code = 0;
                char_code_index = 0;
                unicode = false;
                break;
            case 'u':
                state = TokenizerState::StringEscapeUnicodeStart;
                break;
            case 'n':
                handle_string_escape('\n');
                break;
            case 'r':
                handle_string_escape('\r');
                break;
            case '\\':
                handle_string_escape('\\');
                break;
            case 't':
                handle_string_escape('\t');
                break;
            case '\'':
                handle_string_escape('\'');
                break;
            case '"':
                handle_string_escape('\"');
                break;
            default:
                invalid_char_error(c);
            }
            break;
        case TokenizerState::StringEscapeUnicodeStart:
            switch (c) {
            case '{':
                state = TokenizerState::CharCode;
                radix = 16;
                char_code = 0;
                char_code_index = 0;
                unicode = true;
                break;
            default:
                invalid_char_error(c);
            }
            break;
        case TokenizerState::CharCode:
        {
            if (unicode && c == '}') {
                if (char_code_index == 0) {
                    tokenize_error("empty unicode escape sequence");
                    break;
                }
                if (char_code > 0x10ffff) {
                    tokenize_error("unicode value out of range: %x", char_code);
                    break;
                }
                if (curr_token.id == TokenId::UNICODE_CHAR) {
                    curr_token.char_lit = char_code;
                    state = TokenizerState::CharLiteralEnd;
                }
                else if (char_code <= 0x7f) {
                    // 00000000 00000000 00000000 0xxxxxxx
                    handle_string_escape((uint8_t)char_code);
                }
                else if (char_code <= 0x7ff) {
                    // 00000000 00000000 00000xxx xx000000
                    handle_string_escape((uint8_t)(0xc0 | (char_code >> 6)));
                    // 00000000 00000000 00000000 00xxxxxx
                    handle_string_escape((uint8_t)(0x80 | (char_code & 0x3f)));
                }
                else if (char_code <= 0xffff) {
                    // 00000000 00000000 xxxx0000 00000000
                    handle_string_escape((uint8_t)(0xe0 | (char_code >> 12)));
                    // 00000000 00000000 0000xxxx xx000000
                    handle_string_escape((uint8_t)(0x80 | ((char_code >> 6) & 0x3f)));
                    // 00000000 00000000 00000000 00xxxxxx
                    handle_string_escape((uint8_t)(0x80 | (char_code & 0x3f)));
                }
                else if (char_code <= 0x10ffff) {
                    // 00000000 000xxx00 00000000 00000000
                    handle_string_escape((uint8_t)(0xf0 | (char_code >> 18)));
                    // 00000000 000000xx xxxx0000 00000000
                    handle_string_escape((uint8_t)(0x80 | ((char_code >> 12) & 0x3f)));
                    // 00000000 00000000 0000xxxx xx000000
                    handle_string_escape((uint8_t)(0x80 | ((char_code >> 6) & 0x3f)));
                    // 00000000 00000000 00000000 00xxxxxx
                    handle_string_escape((uint8_t)(0x80 | (char_code & 0x3f)));
                }
                else {
                    UNREACHEABLE;
                }
                break;
            }

            uint32_t digit_value = get_digit_value(c);
            if (digit_value >= radix) {
                tokenize_error("invalid digit: '%c'", c);
                break;
            }
            char_code *= radix;
            char_code += digit_value;
            char_code_index += 1;

            if (!unicode && char_code_index >= 2) {
                assert(char_code <= 255);
                handle_string_escape((uint8_t)char_code);
            }
        }
            break;
        case TokenizerState::SawNot:
            if (c == '=') {
                set_token_id(TokenId::NOT_EQUALS);
                end_token();
                state = TokenizerState::Start;
                break;
            }
            // just a !
            cursor_pos--;
            end_token();
            state = TokenizerState::Start;
            continue;
        case TokenizerState::SawVerticalBar:
            if (c == '|') {
                set_token_id(TokenId::OR);
                end_token();
                state = TokenizerState::Start;
                break;
            }
            // just a |
            cursor_pos--;
            end_token();
            state = TokenizerState::Start;
            continue;
        case TokenizerState::SawAmpersand:
            if (c == '&') {
                set_token_id(TokenId::AND);
                
                end_token();
                state = TokenizerState::Start;
                break;
            }
            // just a &
            cursor_pos--;
            end_token();
            state = TokenizerState::Start;
            continue;
        case TokenizerState::SawLess:
            if (c == '<') {
                set_token_id(TokenId::LSHIFT);
                end_token();
                state = TokenizerState::Start;
                break;
            }
            if (c == '=') {
                set_token_id(TokenId::LESS_OR_EQUALS);
                end_token();
                state = TokenizerState::Start;
                break;
            }
            // just a <
            cursor_pos--;
            end_token();
            state = TokenizerState::Start;
            continue;
        case TokenizerState::SawGreater:
            if (c == '>') {
                set_token_id(TokenId::RSHIFT);
                end_token();
                state = TokenizerState::Start;
                break;
            }
            if (c == '=') {
                set_token_id(TokenId::GREATER_OR_EQUALS);
                end_token();
                state = TokenizerState::Start;
                break;
            }
            // just a >
            cursor_pos--;
            end_token();
            state = TokenizerState::Start;
            continue;
        // If error just get to the next token
        case TokenizerState::Error:
            break;

        default:
            UNREACHEABLE;
            break;
        }

        // Handle end of line
        if (c == '\n')
            reset_line();
        // else we just move a column
        else
            curr_column++;
    }
    cursor_pos--;

    // EOF
    switch (state)
    {
    case TokenizerState::Start:
    case TokenizerState::Error:
    case TokenizerState::LineComment:
        break;
    case TokenizerState::Symbol:
        if (is_invalid_token) {
            is_invalid_token = false;
            set_token_id(TokenId::ERROR);
            end_token();
        }
        else {
            end_token_check_is_keyword();
        }
        break;
    case TokenizerState::Zero:
    case TokenizerState::Number:
        if (is_invalid_token) {
            is_invalid_token = false;
            set_token_id(TokenId::ERROR);
        }
        end_token();
        break;
    case TokenizerState::NumberDot:
        set_token_id(TokenId::FLOAT_LIT);
        __fallthrough;
    case TokenizerState::SawSignOrTypeSpec:
    case TokenizerState::FloatFraction:
    case TokenizerState::FloatExponentUnsigned:
    case TokenizerState::FloatExponentNumber:
    case TokenizerState::NumberNoUnderscore:
    case TokenizerState::FloatFractionNoUnderscore:
    case TokenizerState::FloatExponentNumberNoUnderscore:
    case TokenizerState::SawStar:
    case TokenizerState::SawSlash:
    case TokenizerState::SawPercent:
    case TokenizerState::SawPlus:
    case TokenizerState::SawDash:
    case TokenizerState::SawEq:
    case TokenizerState::DocComment:
        end_token();
        break;
    case TokenizerState::String:
        tokenize_error("unterminated string literal");
        break;
    case TokenizerState::CharLiteral:
        tokenize_error("unterminated Unicode point literal");
        break;
    case TokenizerState::SawStarDocComment:
        tokenize_error("unexpected EOF");
        break;
    default:
        UNREACHEABLE;
    }

    begin_token(TokenId::_EOF);
    end_token();
}

const bool Lexer::has_tokens() const noexcept
{
    return tokens_vec.size() - (curr_index + 1)  != 0;
}

const Token& Lexer::get_previous_token() const noexcept
{
    // TODO: insert return statement here
    return tokens_vec.at(curr_index - 1);
}

const Token& Lexer::get_next_token() const noexcept
{
    return tokens_vec.at(++curr_index);
}

void Lexer::get_back() const noexcept
{
    curr_index--;
}

std::string_view Lexer::get_token_value(const Token& token) const noexcept
{
    return std::string_view(source.begin() + token.start_pos, source.begin() + token.end_pos + 1);
}

void Lexer::begin_token(const TokenId id) noexcept
{
    curr_token = Token();
    curr_token.id = id;
    curr_token.start_line = curr_line;
    curr_token.start_column = curr_column;
    curr_token.start_pos = cursor_pos;
    curr_token.file_name = file_name;
}

void Lexer::set_token_id(const TokenId id) noexcept
{
    curr_token.id = id;
}

void Lexer::end_token() noexcept {
    curr_token.end_pos = cursor_pos;

    switch (curr_token.id) {
    case TokenId::DOC_COMMENT:
        comments_vec.push_back(curr_token);
        break;
    default:
        tokens_vec.push_back(curr_token);
        break;
    }
}

void Lexer::end_token_check_is_keyword() noexcept
{
    curr_token.end_pos = cursor_pos;
    
    is_keyword();

    switch (curr_token.id) {
    case TokenId::FN:
    case TokenId::RET:
    case TokenId::AND:
    case TokenId::OR:
    case TokenId::IDENTIFIER:
        tokens_vec.push_back(curr_token);
        break;
    default:
        UNREACHEABLE;
    }
}

void Lexer::reset_line() noexcept {
    curr_line++;
    curr_column = 0;
}

void Lexer::invalid_char_error(uint8_t c) noexcept {
    if (c == '\r') {
        tokenize_error("invalid carriage return, only '\\n' line endings are supported");
        return;
    }

    const char* sh = get_escape_shorthand(c);
    if (sh) {
        tokenize_error("invalid character: '%s'", sh);
        return;
    }

    if (isprint(c)) {
        tokenize_error("invalid character: '%c'", c);
        return;
    }

    tokenize_error("invalid character: '\\x%02x'", c);
}

void Lexer::tokenize_error(const char* format, ...) noexcept {
    state = TokenizerState::Error;

    
    va_list ap, ap2;
    va_start(ap, format);
    va_copy(ap2, ap);
    
    int len1 = snprintf(nullptr, 0, format, ap);
    assert(len1 >= 0);

    std::string msg;
    msg.reserve(len1);

    int len2 = snprintf(msg.data(), len1, format, ap2);
    assert(len2 == len1);

    va_end(ap);

    Error error(ERROR_TYPE::ERROR,
        curr_line,
        curr_column,
        file_name, msg);

    errors.push_back(error);
}

void Lexer::handle_string_escape(uint8_t c) noexcept {
    if (curr_token.id == TokenId::UNICODE_CHAR) {
        curr_token.char_lit = c;
        
        state = TokenizerState::CharLiteralEnd;
    }
    else if (curr_token.id == TokenId::STRING) {
        
        state = TokenizerState::String;
    }
    else {
        UNREACHEABLE;
    }
}

static std::unordered_map<std::string_view, TokenId> keywords = {
    {"fn", TokenId::FN},
    {"ret", TokenId::RET},
    {"and", TokenId::AND},
    {"or", TokenId::OR}
};

void Lexer::is_keyword() noexcept
{
    auto value = std::string_view(source.begin() + curr_token.start_pos, source.begin() + curr_token.end_pos + 1);
    if (keywords.find(value) != keywords.end()) {
        set_token_id(keywords.at(value));
    }
}

uint32_t get_digit_value(uint8_t c) {
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

bool is_symbol_char(uint8_t c) {
    switch (c) {
    case SYMBOL_CHAR:
        return true;
    default:
        return false;
    }
}

bool is_whitespace_char(uint8_t c)
{
    switch (c) {
    case WHITESPACE:
            return true;
    default:
        return false;
    }
}

bool is_float_specifier(uint8_t c)
{
    return c == 'f';
}

bool is_sign_or_type_specifier(uint8_t c) {
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

bool is_exponent_signifier(uint8_t c, int radix) {
    if (radix == 16) {
        return c == 'p' || c == 'P';
    }
    else {
        return c == 'e' || c == 'E';
    }
}

static const char* get_escape_shorthand(uint8_t c) {
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

static std::string token_id_names[] = {
    "HASH",
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
    "EOF"
};

inline std::string& token_id_name(TokenId id) {
    return token_id_names[(size_t)id];
}

size_t token_spaces(const size_t value_size, const size_t id_name_size) {
    return value_size >= id_name_size
        ? value_size - id_name_size
        : id_name_size - value_size;
}

struct TokenPrintInfo {
    std::string id_name;
    size_t value_size;
    size_t token_name_size;
    bool is_value;
    TokenPrintInfo(const size_t in_value_size, const size_t in_token_name_size, const std::string& in_id_name, const bool in_is_value)
        : value_size(in_value_size), token_name_size(in_token_name_size), id_name(in_id_name), is_value(in_is_value){}
};

std::string create_values_line(const std::string& source, const size_t start, const size_t end, const std::vector<Token>& tokens, const std::vector<TokenPrintInfo>& token_infos) {
    std::string line;
    for (size_t j = start; j < end; j++) {
        auto& token_info = token_infos.at(j);
        size_t is_value = (size_t)!token_info.is_value;

        auto spaces_count = is_value * token_spaces(token_info.value_size, token_info.token_name_size);
        auto spaces = std::string(spaces_count, ' ');

        std::string value;
        auto& token = tokens.at(j);
        auto token_value = std::string_view(source.begin() + token.start_pos, source.begin() + token.end_pos + 1);

        if (token.id == TokenId::DOC_COMMENT) {
            
            for (char c : token_value) {
                switch (c)
                {
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
        }
        else {
            value = token_value;
        }

        line.append(value + spaces);

        // if not final token add space
        if (j < end - 1)
            line.append(" ");
    }
    return line;
}

std::string create_id_names_line(const size_t start, const size_t end, const std::vector<TokenPrintInfo>& token_infos) {
    std::string line;
    for (size_t j = start; j < end; j++) {
        auto& token_info = token_infos.at(j);
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

Console print_tokens(Lexer& lexer)
{
    auto& tokens = lexer.tokens_vec;
    Console console;
    
    size_t total_size = 0;
    std::vector<TokenPrintInfo> token_infos;
    token_infos.reserve(tokens.size());

    size_t last_it = 0;
    for (size_t i = 0; i < tokens.size(); i++)
    {
        auto& token = tokens.at(i);
        
        auto& token_name = token_id_name(token.id);
        auto token_value_size = token.get_value_size();
        auto token_name_size = token_name.size() + 2;

        auto& token_info = token_infos.emplace_back(token_value_size, token_name_size, token_name, token_value_size > token_name_size);
                
        auto size =  token_info.is_value ? token_value_size : token_name_size;
        
        
        if (total_size + size > 80) {
            console.push_back(create_values_line(lexer.source, last_it, i, tokens, token_infos));
            console.push_back(create_id_names_line(last_it, i, token_infos));
            last_it = i;
            total_size = 0;
        }
        else {
            total_size += size;
            if (i == tokens.size() -1) {
                console.push_back(create_values_line(lexer.source, last_it, tokens.size(), tokens, token_infos));
                console.push_back(create_id_names_line(last_it, tokens.size(), token_infos));
            }
        }
    }

    return console;
}
