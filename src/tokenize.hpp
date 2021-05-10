#pragma once
#include <vector>
#include "error.hpp"
#include "bigint.hpp"

enum class TokenId {
    HASH,               // #
    FUNC,               // fn
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
    // [0]* [1-9] [0-9]*       [u]? [bwl]?
    // [0]  [o]   [0-7]*       [u]? [bwl]?
    // [0]  [x]   [0-9A-F]*    [u]? [bwl]?
    // [0]  [b]   [0-1]*       [u]? [bwl]?
    FLOAT_LIT,          // [0-9]* [.] [0-9]* [f]?

    ESCAPED_VALUE,      // \[value]
    STRING,             // " (~["\\] | ESCAPED_VALUE)* "
    UNICODE_CHAR,       // " (~["\\] | ESCAPED_VALUE)* "

    WS,                 // [\t]
    DOC_COMMENT,        // '/*' . '*/'
    LINE_COMMENT,       // // .
    EOL,                // [\r ? \n]
    _EOF                 // enf of file
};

std::string& token_id_name(TokenId id);

typedef struct { uint64_t v[2]; } float128_t;
typedef uint32_t Char;

struct BigFloat {
    float128_t value;
    bool overflow;
};

struct Token {
    TokenId       id;
    size_t        line;
    size_t        column_start;
    size_t        column_end;
    std::string   file_name;

    union {
        std::string   value;
        std::string   string_lit;
    };
    
    union {
        BigInt    int_lit;
        BigFloat  float_lit;
        Char      char_lit;
    };

    Token()
        : id(TokenId::_EOF),
        line(0), column_start(0), column_end(0),
        file_name(""),
        value(""), int_lit({ 0 }) {}

    Token(const TokenId _id, const long _line, const long _column_start, const long _column_end, const std::string &_file_name, const std::string &_value)
        : id(_id),
        line(_line), column_start(_column_start), column_end(_column_end),
        file_name(_file_name),
        value(_value), int_lit({ 0 }) {}

    ~Token() {}

    Token(const Token& other)
        : id(other.id), 
        line(other.line),
        column_start(other.column_start),
        column_end(other.column_end),
        file_name(other.file_name),
        value(other.value),
        int_lit(other.int_lit) {}

    Token& operator=(const Token& other) {
        // self-assignment guard
        if (this == &other)
            return *this;

        id = other.id;
        line = other.line;
        column_start = other.column_start;
        column_end = other.column_end;
        file_name = other.file_name;
        value = other.value; 
        int_lit = other.int_lit;

        return *this;
    }
};


typedef std::vector<std::string> Console;

class Lexer {
    enum class TokenizerState;

    size_t cursor_pos;
    size_t curr_line;
    size_t curr_column;
    
    int radix;  // used for getting number value.
    bool is_trailing_underscore;    // used to interpret number_number

    TokenizerState state;
    Token curr_token;
    
    std::string file_name;
    std::string source;

    std::vector<Token>  tokens_vec;
    std::vector<Error>& errors;
public:
    Lexer(const std::string& _file_name, std::vector<Error>& errors);

    std::vector<Token> tokenize() noexcept;

    friend Console print_tokens(Lexer& lexer);
private:
    void begin_token(const TokenId id) noexcept;
    void set_token_id(const TokenId id) noexcept;
    void end_token() noexcept;
    void append_char(const char c) noexcept;
    void reset_line() noexcept;

    void invalid_char_error(uint8_t c) noexcept;
    void tokenize_error(const char* format, ...) noexcept;

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
        CharLiteral,               //
        SawStar,                   //
        SawSlash,                  //
        SawPercent,                //
        SawPlus,                   //
        SawDash,                   //
        DocComment,                //
        SawStarDocComment,         // * in multiline comment may lead to end comment
        LineComment,               //
        SawEq,                     //
        Error,                     //
    };
};


Console print_tokens(Lexer& lexer);

