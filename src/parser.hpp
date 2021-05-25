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

    /*
    // returns AstSourceCode
    _NODISCARD AstNode* parse_source_code() noexcept;

    // returns AstDirective
    _NODISCARD AstNode* parse_basic_directive() noexcept;

    // returns AstFuncDef
    _NODISCARD AstNode* parse_function_def() noexcept;

    // returns AstFuncProto
    _NODISCARD AstNode* parse_function_proto() noexcept;

    // returns AstBlock
    _NODISCARD AstNode* parse_block() noexcept;

    // returns AstSymbol | AstBinaryExpr | AstUnaryExpr
    _NODISCARD AstNode* parse_statement() noexcept;

    // returns AstBinaryExpr
    _NODISCARD AstNode* parse_assignment() noexcept;

    // returns AstUnaryExpr
    _NODISCARD AstNode* parse_ret() noexcept;
    */

    // returns AstSymbol | AstBinaryExpr
    _NODISCARD AstNode* parse_expr() noexcept;

    // returns AstSymbol | AstBinaryExpr
    _NODISCARD AstNode* parse_mulexpr() noexcept;

    // returns AstSymbol
    _NODISCARD AstNode* parse_value() noexcept;
    
    /*
    // returns AstVarDef
    _NODISCARD AstNode* parse_variable_def() noexcept;

    // returns AstType
    _NODISCARD AstNode* parse_type() noexcept;
    */
    AstNode* parse_error(const Token& token, const char* format, ...) noexcept;
};

