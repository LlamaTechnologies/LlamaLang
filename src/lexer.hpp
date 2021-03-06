#pragma once
#include <vector>
#include "error.hpp"
#include "bigint.hpp"
#include "bigfloat.hpp"

enum class TokenId {
    HASH,               // #
    FN,                 // fn
    RET,                // ret
    L_PAREN,            // (
    R_PAREN,            // )
    L_CURLY,            // {
    R_CURLY,            // }
    L_BRACKET,          // [
    R_BRACKET,          // ]
    COMMA,              // ,
    SEMI,               // ;
    COLON,              // :
    DOT,                // .
    ASSIGN,             // =

    // UNARY OPERATORS
    PLUS_PLUS,          // ++
    MINUS_MINUS,        // --
    NOT,                // !

    // RELATION OPERATORS
    OR,                 // || or
    AND,                // && and
    EQUALS,             // ==
    NOT_EQUALS,         // !=
    LESS,               // <
    LESS_OR_EQUALS,     // <=
    GREATER,            // >
    GREATER_OR_EQUALS,  // >=

    // ARITHMETIVC OPERATORS
    PLUS,               // +
    MINUS,              // -
    MUL,                // *
    DIV,                // /
    MOD,                // %
    PLUS_ASSIGN,        // +=
    MINUS_ASSIGN,       // -=
    MUL_ASSIGN,         // *=
    DIV_ASSIGN,         // /=
    MOD_ASSIGN,         // %=
    // BITWISE OPERATORS
    BIT_AND,            // &
    BIT_XOR,            // ^
    BIT_OR,             // |
    BIT_NOT,            // ~
    LSHIFT,             // <<
    RSHIFT,             // >>

    // COMPLEX TOKENS
    IDENTIFIER,         // [a-zA-Z_] [a-zA-Z_0-9]*
    INT_LIT,
    // [0-9]+               [u]? [bwl]?
    // [0] [o] [0-7]*       [u]? [bwl]?
    // [0] [x] [0-9A-F]*    [u]? [bwl]?
    // [0] [b] [0-1]*       [u]? [bwl]?
    FLOAT_LIT,          // [0-9]* [.] [0-9]* [f]?

    ESCAPED_VALUE,      // \[value]
    STRING,             // " (~["\\] | ESCAPED_VALUE)* "
    UNICODE_CHAR,       // " (~["\\] | ESCAPED_VALUE)* "

    WS,                 // [\t \r \n ' ']
    DOC_COMMENT,        // '/*' . '*/'
    LINE_COMMENT,       // // .
    ERROR,              // special token: describes an invalid token
    _EOF                // end of file
};

const char * token_id_name(TokenId id);
typedef uint32_t Char;


struct Token {
    TokenId       id;
    size_t        start_pos;
    size_t        end_pos;
    size_t        start_line;
    size_t        start_column;
    std::string   file_name;

    union {
        BigInt    int_lit;
        BigFloat  float_lit;
        Char      char_lit;
    };

    Token()
        : id(TokenId::_EOF),
        start_pos(0L), end_pos(0L),
        start_line(0), start_column(0),
        file_name(""),
        int_lit({ 0 }) {}

    ~Token() {}

    Token(const Token& other)
        : id(other.id), 
        start_pos(other.start_pos),
        end_pos(other.end_pos),
        start_line(other.start_line),
        start_column(other.start_column),
        file_name(other.file_name),
        int_lit(other.int_lit) {}

    Token& operator=(const Token& other) {
        // self-assignment guard
        if (this == &other)
            return *this;

        id = other.id;
        start_line = other.start_line;
        start_column = other.start_column;
        start_pos = other.start_pos;
        end_pos = other.end_pos;
        file_name = other.file_name;
        int_lit = other.int_lit;

        return *this;
    }

    size_t get_value_size() {
        return (end_pos + 1) - start_pos;
    }
};


typedef std::vector<std::string> Console;

class Lexer {
    enum class TokenizerState;

    size_t cursor_pos;
    size_t curr_line;
    size_t curr_column;
    mutable size_t curr_index;              // used to consume tokens

    size_t char_code_index;         // char_code char counter
    size_t remaining_code_units;    // used to count bytes in unicode char
    uint32_t radix;                 // used for getting number value.
    uint32_t char_code;             // char_code used accros the char_code state
    bool unicode;                   // is unicode char code
    bool is_trailing_underscore;    // used to interpret number_number
    bool is_invalid_token;          // used to finish tokenizing a error tokens

    TokenizerState state;
    Token curr_token;
public:
    std::string file_name;
    std::string source;

private:
    std::vector<Token>  tokens_vec;
    std::vector<Token>  comments_vec;
    std::vector<Error>& errors;
public:
    Lexer(const std::string& _file_name, std::vector<Error>& _errors);
    Lexer(const std::string& _src_file, const std::string& _file_name, std::vector<Error>& _errors);
    void tokenize() noexcept;

    const bool has_tokens() const noexcept;

    const Token& get_previous_token() const noexcept;

    // should not be called after EOF token
    const Token& get_next_token() const noexcept;

    // --curr_index
    void get_back() const noexcept;

    std::string_view get_token_value(const Token& token) const noexcept;

    friend Console print_tokens(Lexer& lexer);
private:
    void begin_token(const TokenId id) noexcept;
    void set_token_id(const TokenId id) noexcept;
    void end_token() noexcept;
    void end_token_check_is_keyword()  noexcept;
    void reset_line() noexcept; 
    void is_keyword() noexcept;
    void invalid_char_error(uint8_t c) noexcept;
    void tokenize_error(const char* format, ...) noexcept;
    void handle_string_escape(uint8_t c) noexcept;

    enum class TokenizerState {
        Start,
        Symbol,
        Zero,                      // "0", which might lead to "0x"
        Number,                    // "123", "0x123"
        NumberNoUnderscore,        // "12_", "0x12_" next char must be digit
        NumberDot,                 //
        FloatFraction,             // "123.456", "0x123.456"
        FloatFractionNoUnderscore, // "123.45_", "0x123.45_"
        FloatExponentUnsigned,     // "123.456e", "123e", "0x123p"
        FloatExponentNumber,       // "123.456e7", "123.456e+7", "123.456e-7"
        FloatExponentNumberNoUnderscore, // "123.456e7_", "123.456e+7_", "123.456e-7_"
        String,                    //
        StringEscape,              // saw \ inside a string
        StringEscapeUnicodeStart,  // saw u inside string_escape
        CharCode,                  // saw x in string_escape or began the unicode escape
        CharLiteral,               //
        CharLiteralUnicode,        // unicode char
        CharLiteralEnd,            //
        SawStar,                   //
        SawSlash,                  //
        SawPercent,                //
        SawPlus,                   //
        SawDash,                   //
        SawNot,
        SawVerticalBar,
        SawAmpersand,
        SawLess,
        SawGreater,
        SawSignOrTypeSpec,
        DocComment,                //
        SawStarDocComment,         // * in multiline comment may lead to end comment
        LineComment,               //
        SawEq,                     //
        Error,                     //
    };
};


Console print_tokens(Lexer& lexer);

