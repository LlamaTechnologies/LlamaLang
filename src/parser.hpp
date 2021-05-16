#pragma once
#include <vector>
#include <string>

class Lexer;
struct Token;
struct Error;
struct AstNode;

class Parser {
    const Lexer& lexer;
    std::vector<Error>& error_vec;
public:
    Parser(const Lexer& in_lexer, std::vector<Error>& in_error_vec);

    AstNode* parse() noexcept;

private:

    _NODISCARD AstNode* parse_source_code() noexcept;

    _NODISCARD AstNode* parse_basic_directive() noexcept;

    _NODISCARD AstNode* parse_function_def() noexcept;

    _NODISCARD AstNode* parse_function_proto() noexcept;

    _NODISCARD AstNode* parse_block() noexcept;

    _NODISCARD AstNode* parse_statement() noexcept;

    _NODISCARD AstNode* parse_variable_def() noexcept;

    _NODISCARD AstNode* parse_type() noexcept;

    AstNode* parse_error(const Token& token, const char* format, ...) noexcept;
};

