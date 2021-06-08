#pragma once
#include "common_defs.hpp"
#include <vector>
#include <string>

enum class TokenId;
class Lexer;
struct Token;
struct Error;
struct AstNode;

bool match(const Token* token, ...) noexcept;
#define MATCH(token, ...) match(token, __VA_ARGS__, TokenId(size_t(TokenId::_EOF) + 1))

class Parser {
    const Lexer& lexer;
    std::vector<Error>& error_vec;
public:
    Parser(const Lexer& in_lexer, std::vector<Error>& in_error_vec);

    AstNode* parse() noexcept;

    /*
    // returns AstSourceCode
    LL_NODISCARD AstNode* parse_source_code() noexcept;

    // returns AstDirective
    LL_NODISCARD AstNode* parse_basic_directive() noexcept;

    // returns AstFuncDef
    LL_NODISCARD AstNode* parse_function_def() noexcept;
    */

    // returns AstFuncProto
    LL_NODISCARD AstNode* parse_function_proto() noexcept;

    // returns AstParamDecl
    LL_NODISCARD AstNode* parse_param_decl() noexcept;

    // returns AstBlock
    LL_NODISCARD AstNode* parse_block() noexcept;

    // returns AstSymbol | AstBinaryExpr | AstUnaryExpr
    LL_NODISCARD AstNode* parse_statement() noexcept;

    // returns AstVarDef
    LL_NODISCARD AstNode* parse_vardef_stmnt() noexcept;

    // returns AstTypeNode
    LL_NODISCARD AstNode* parse_type() noexcept;

    // returns AstBinaryExpr
    LL_NODISCARD AstNode* parse_assign_stmnt() noexcept;

    // returns AstUnaryExpr
    LL_NODISCARD AstNode* parse_ret_stmnt() noexcept;

     // returns AstSymbol | AstFuncCallExpr | AstBinaryExpr | AstUnaryExpr
    LL_NODISCARD AstNode* parse_expr() noexcept;

    // returns AstSymbol | AstFuncCallExpr | AstBinaryExpr | AstUnaryExpr
    LL_NODISCARD AstNode* parse_comp_expr() noexcept;

    // returns AstSymbol | AstFuncCallExpr | AstBinaryExpr | AstUnaryExpr
    LL_NODISCARD AstNode* parse_algebraic_expr() noexcept;

    // returns AstSymbol | AstFuncCallExpr | AstBinaryExpr | AstUnaryExpr
    LL_NODISCARD AstNode* parse_term_expr() noexcept;

    // returns AstSymbol | AstFuncCallExpr | AstUnaryExpr
    LL_NODISCARD AstNode* parse_unary_expr() noexcept;

    // returns AstSymbol | AstFuncCallExpr
    LL_NODISCARD AstNode* parse_primary_expr() noexcept;
    
    AstNode* parse_error(const Token& token, const char* format, ...) noexcept;

    bool is_new_line_between(const size_t start_pos, const size_t end_pos);
};

