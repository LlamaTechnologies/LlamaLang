#include "Tokenize.hpp"
#include <fstream>
#include <cassert>
#include <cstdarg>

#define WHITESPACE \
         ' ': \
    case '\r': \
    case '\t': \
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
static bool is_exponent_signifier(uint8_t c, int radix);

Lexer::Lexer(const std::string& _file_name, std::vector<Error>& _errors)
    : file_name(_file_name), errors(_errors),
    curr_line(0L), curr_column(0L), state(TokenizerState::Start),
    radix(10), is_trailing_underscore(false),
    curr_token(), tokens_vec()
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

std::vector<Token> Lexer::tokenize() noexcept
{
    // reading file while no errors in it
    for (cursor_pos = 0; cursor_pos < source.size(); cursor_pos++) {
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
                append_char(c);
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
                append_char(c);
                break;
                // we found the beginning of a char literal
            case '\'':
                begin_token(TokenId::UNICODE_CHAR);
                state = TokenizerState::CharLiteral;
                append_char(c);
                break;
                // we found
                // ASSIGN =
                // EQUALS ==
            case '=':
                begin_token(TokenId::ASSIGN);
                state = TokenizerState::SawEq;
                append_char(c);
                break;
                // we found 
                // a DIV
                // /=
                // the beginning of a DOC comment
                // the beginning of a line comment
            case '/':
                begin_token(TokenId::DIV);
                state = TokenizerState::SawSlash;
                append_char(c);
                break;
                // we found
                // MUL
                // *=
            case '*':
                begin_token(TokenId::MUL);
                state = TokenizerState::SawStar;
                append_char(c);
                break;
                // we found
                // a PLUS
                // +=
                // ++
            case '+':
                begin_token(TokenId::PLUS);
                state = TokenizerState::SawPlus;
                append_char(c);
                break;
                // we found
                // a MINUS
                // -=
                // --
            case '-':
                begin_token(TokenId::MINUS);
                state = TokenizerState::SawDash;
                append_char(c);
                break;
                // we found
                // MOD
                // %=
            case '%':
                begin_token(TokenId::MOD);
                state = TokenizerState::SawPercent;
                append_char(c);
                break;
                // we found a hash
            case '#':
                begin_token(TokenId::HASH);
                append_char(c);
                end_token();
                break;
                // we found a semicolon
            case ';':
                begin_token(TokenId::SEMI);
                append_char(c);
                end_token();
                break;
            case '(':
                begin_token(TokenId::L_PAREN);
                append_char(c);
                end_token();
                break;
            case ')':
                begin_token(TokenId::R_PAREN);
                append_char(c);
                end_token();
                break;
            case '{':
                begin_token(TokenId::L_CURLY);
                append_char(c);
                end_token();
                break;
            case '}':
                begin_token(TokenId::R_CURLY);
                append_char(c);
                end_token();
                break;
            case '[':
                begin_token(TokenId::L_BRACKET);
                append_char(c);
                end_token();
                break;
            case ']':
                begin_token(TokenId::R_BRACKET);
                append_char(c);
                end_token();
                break;
            default:
                break;
            }
            break;
            // TOKENIZING A IDENTIFIER
        case TokenizerState::Symbol:
            switch (c) {
            case SYMBOL_CHAR:
                append_char(c);
                break;
            default:
                cursor_pos--;
                end_token();
                state = TokenizerState::Start;
                continue;
            }
            break;
            // TRYING TO TOKENIZE /...
        case TokenizerState::SawSlash:
            switch (c) {
                // line comment //
            case '/':
                set_token_id(TokenId::LINE_COMMENT);
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
                // do nothing
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
                end_token();
                state = TokenizerState::Start;
                break;
            default:
                // do nothing
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
                if (is_trailing_underscore) {
                    invalid_char_error(c);
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
            BigInt digit_value_bi;
            bigint_init_unsigned(&digit_value_bi, digit_value);

            BigInt radix_bi;
            bigint_init_unsigned(&radix_bi, radix);

            BigInt multiplied;
            bigint_mul(&multiplied, &curr_token.int_lit, &radix_bi);

            bigint_add(&curr_token.int_lit, &multiplied, &digit_value_bi);
            break;
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
            cursor_pos--;
            state = TokenizerState::FloatFractionNoUnderscore;
            assert(curr_token.id == TokenId::INT_LIT);
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

    // EOF
    switch (state)
    {
    case Lexer::TokenizerState::Start:
    case Lexer::TokenizerState::Error:
    case Lexer::TokenizerState::LineComment:
        break;
    case Lexer::TokenizerState::Symbol:
    case Lexer::TokenizerState::Zero:
    case Lexer::TokenizerState::Number:
    case Lexer::TokenizerState::FloatFraction:
    case Lexer::TokenizerState::FloatExponentUnsigned:
    case Lexer::TokenizerState::FloatExponentNumber:
    case Lexer::TokenizerState::SawStar:
    case Lexer::TokenizerState::SawSlash:
    case Lexer::TokenizerState::SawPercent:
    case Lexer::TokenizerState::SawPlus:
    case Lexer::TokenizerState::SawDash:
    case Lexer::TokenizerState::SawEq:
    case Lexer::TokenizerState::DocComment:
        end_token();
        break;
    case Lexer::TokenizerState::NumberDot:
    case Lexer::TokenizerState::NumberNoUnderscore:
    case Lexer::TokenizerState::FloatFractionNoUnderscore:
    case Lexer::TokenizerState::FloatExponentNumberNoUnderscore:
        tokenize_error("unterminated number literal");
        break;
    case Lexer::TokenizerState::String:
        tokenize_error("unterminated string literal");
        break;
    case Lexer::TokenizerState::CharLiteral:
        tokenize_error("unterminated Unicode point literal");
        break;
    case Lexer::TokenizerState::SawStarDocComment:
        tokenize_error("unexpected EOF");
        break;
    default:
        UNREACHEABLE;
    }

    if (state != TokenizerState::Error) {
        begin_token(TokenId::_EOF);
        end_token();
    }
    return tokens_vec;
}

void Lexer::begin_token(const TokenId id) noexcept
{
    curr_token = Token();
    curr_token.id = id;
    curr_token.line = curr_line;
    curr_token.column_start = curr_column;
    curr_token.file_name = file_name;
}

void Lexer::set_token_id(const TokenId id) noexcept
{
}

void Lexer::end_token() noexcept {
    curr_token.column_end = curr_column;
    tokens_vec.push_back(curr_token);
}

void Lexer::append_char(const char c) noexcept {
    curr_token.value.push_back(c);
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
    "EOL",
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

std::string create_values_line(const size_t start, const size_t end, const std::vector<Token>& tokens, const std::vector<TokenPrintInfo>& token_infos) {
    std::string line;
    for (size_t j = start; j < end; j++) {
        auto& token_info = token_infos.at(j);
        size_t is_value = (size_t)!token_info.is_value;

        auto spaces_count = is_value * token_spaces(token_info.value_size, token_info.token_name_size);
        auto spaces = std::string(spaces_count, ' ');

        line.append(tokens.at(j).value + spaces);

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
        auto token_value_size = token.value.size();
        auto token_name_size = token_name.size() + 2;

        auto& token_info = token_infos.emplace_back(token_value_size, token_name_size, token_name, token_value_size > token_name_size);
                
        auto size =  token_info.is_value ? token_value_size : token_name_size;
        
        
        if (total_size + size > 80) {
            console.push_back(create_values_line(last_it, i, tokens, token_infos));
            console.push_back(create_id_names_line(last_it, i, token_infos));
            last_it = i;
            total_size = 0;
        }
        else {
            total_size += size;
            if (i == tokens.size() -1) {
                console.push_back(create_values_line(last_it, tokens.size(), tokens, token_infos));
                console.push_back(create_id_names_line(last_it, tokens.size(), token_infos));
            }
        }
    }

    return console;
}