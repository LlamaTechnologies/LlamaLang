#pragma once
#include <vector>
#include <string>

enum class TokenId;
class Lexer;
struct Token;
struct Error;
struct AstNode;

bool match(const Token token, const TokenId ...) noexcept;
#define MATCH(token, ...) match(token, __VA_ARGS__, TokenId(size_t(TokenId::_EOF) + 1))

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
    */

    // returns AstSymbol | AstBinaryExpr | AstUnaryExpr
    _NODISCARD AstNode* parse_statement() noexcept;

    // returns AstVarDef
    _NODISCARD AstNode* parse_vardef_stmnt() noexcept;

    // returns AstTypeNode
    _NODISCARD AstNode* parse_type() noexcept;

    // returns AstBinaryExpr
    _NODISCARD AstNode* parse_assign_stmnt() noexcept;

    // returns AstUnaryExpr
    _NODISCARD AstNode* parse_ret_stmnt() noexcept;

     // returns AstSymbol | AstFuncCallExpr | AstBinaryExpr | AstUnaryExpr
    _NODISCARD AstNode* parse_expr() noexcept;

    // returns AstSymbol | AstFuncCallExpr | AstBinaryExpr | AstUnaryExpr
    _NODISCARD AstNode* parse_comp_expr() noexcept;

    // returns AstSymbol | AstFuncCallExpr | AstBinaryExpr | AstUnaryExpr
    _NODISCARD AstNode* parse_algebraic_expr() noexcept;

    // returns AstSymbol | AstFuncCallExpr | AstBinaryExpr | AstUnaryExpr
    _NODISCARD AstNode* parse_term_expr() noexcept;

    // returns AstSymbol | AstFuncCallExpr | AstUnaryExpr
    _NODISCARD AstNode* parse_unary_expr() noexcept;

    // returns AstSymbol | AstFuncCallExpr
    _NODISCARD AstNode* parse_primary_expr() noexcept;
    
    /*
    // returns AstVarDef
    _NODISCARD AstNode* parse_variable_def() noexcept;

    // returns AstType
    _NODISCARD AstNode* parse_type() noexcept;
    */
    AstNode* parse_error(const Token& token, const char* format, ...) noexcept;
};

