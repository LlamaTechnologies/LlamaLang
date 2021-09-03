#pragma once
#include "common_defs.hpp"

#include <string>
#include <vector>

enum class TokenId;
class Lexer;
struct Token;
struct Error;
struct AstNode;
struct AstSourceCode;
struct AstDirective;
struct AstFnDef;
struct AstFnProto;
struct AstParamDef;
struct AstVarDef;
struct AstBlock;
struct AstType;
struct AstBinaryExpr;
struct AstUnaryExpr;
struct AstFnCallExpr;

bool match(const Token *token, ...) noexcept;
#define MATCH(token, ...) match(token, __VA_ARGS__, TokenId(size_t(TokenId::_EOF) + 1))

class Parser {
  std::vector<Error> &error_vec;

public:
  Parser(std::vector<Error> &in_error_vec);

  AstSourceCode *parse(const Lexer &lexer) noexcept;

  LL_NODISCARD AstSourceCode *parse_source_code(const Lexer &lexer) noexcept;

  LL_NODISCARD AstDirective *parse_directive(const Lexer &lexer) noexcept;

  // returns AstFnDef | AstFnProto
  LL_NODISCARD AstNode *parse_function_def(const Lexer &lexer) noexcept;

  LL_NODISCARD AstFnProto *parse_function_proto(const Lexer &lexer) noexcept;

  LL_NODISCARD AstParamDef *parse_param_def(const Lexer &lexer) noexcept;

  LL_NODISCARD AstBlock *parse_block(const Lexer &lexer) noexcept;

  // returns AstSymbol | AstBinaryExpr | AstUnaryExpr
  LL_NODISCARD AstNode *parse_statement(const Lexer &lexer) noexcept;

  LL_NODISCARD AstVarDef *parse_vardef_stmnt(const Lexer &lexer) noexcept;

  LL_NODISCARD AstType *parse_type(const Lexer &lexer) noexcept;

  LL_NODISCARD AstBinaryExpr *parse_assign_stmnt(const Lexer &lexer) noexcept;

  LL_NODISCARD AstUnaryExpr *parse_ret_stmnt(const Lexer &lexer) noexcept;

  LL_NODISCARD AstFnCallExpr *parse_function_call(const Lexer &lexer) noexcept;

  // returns AstSymbol | AstConstValue | AstFnCallExpr | AstBinaryExpr | AstUnaryExpr
  LL_NODISCARD AstNode *parse_expr(const Lexer &lexer) noexcept;

  // returns AstSymbol | AstConstValue | AstFnCallExpr | AstBinaryExpr | AstUnaryExpr
  LL_NODISCARD AstNode *parse_comp_expr(const Lexer &lexer) noexcept;

  // returns AstSymbol | AstConstValue | AstFnCallExpr | AstBinaryExpr | AstUnaryExpr
  LL_NODISCARD AstNode *parse_algebraic_expr(const Lexer &lexer) noexcept;

  // returns AstSymbol | AstConstValue | AstFnCallExpr | AstBinaryExpr | AstUnaryExpr
  LL_NODISCARD AstNode *parse_term_expr(const Lexer &lexer) noexcept;

  // returns AstSymbol | AstConstValue | AstFnCallExpr | AstUnaryExpr
  LL_NODISCARD AstNode *parse_unary_expr(const Lexer &lexer) noexcept;

  // returns AstSymbol | AstConstValue | AstFnCallExpr | AstBinaryExpr | AstUnaryExpr
  LL_NODISCARD AstNode *parse_primary_expr(const Lexer &lexer) noexcept;

  AstNode *parse_error(const Token &token, const char *format, ...) noexcept;

  bool is_new_line_between(const Lexer &lexer, const size_t start_pos, const size_t end_pos);

  // consumes the forbiden statement, report the error and return true else returns false
  bool is_forbiden_statement(const Token &token) noexcept;
};
