#pragma once
#include "common_defs.hpp"

#include <string>
#include <vector>

enum class TokenId;
class Lexer;
struct Token;
struct Error;
struct AstNode;

bool match(const Token *token, ...) noexcept;
#define MATCH(token, ...) match(token, __VA_ARGS__, TokenId(size_t(TokenId::_EOF) + 1))

class Parser {
  std::vector<Error> &error_vec;

public:
  Parser(std::vector<Error> &in_error_vec);

  AstNode *parse(const Lexer &lexer) noexcept;

  // returns AstSourceCode
  LL_NODISCARD AstNode *parse_source_code(const Lexer &lexer) noexcept;

  // returns AstDirective
  LL_NODISCARD AstNode *parse_directive(const Lexer &lexer) noexcept;

  // returns AstFnDef
  LL_NODISCARD AstNode *parse_function_def(const Lexer &lexer) noexcept;

  // returns AstFnProto
  LL_NODISCARD AstNode *parse_function_proto(const Lexer &lexer) noexcept;

  // returns AstParamDecl
  LL_NODISCARD AstNode *parse_param_decl(const Lexer &lexer) noexcept;

  // returns AstBlock
  LL_NODISCARD AstNode *parse_block(const Lexer &lexer) noexcept;

  // returns AstSymbol | AstBinaryExpr | AstUnaryExpr
  LL_NODISCARD AstNode *parse_statement(const Lexer &lexer) noexcept;

  // returns AstVarDef
  LL_NODISCARD AstNode *parse_vardef_stmnt(const Lexer &lexer) noexcept;

  // returns AstTypeNode
  LL_NODISCARD AstNode *parse_type(const Lexer &lexer) noexcept;

  // returns AstBinaryExpr
  LL_NODISCARD AstNode *parse_assign_stmnt(const Lexer &lexer) noexcept;

  // returns AstUnaryExpr
  LL_NODISCARD AstNode *parse_ret_stmnt(const Lexer &lexer) noexcept;

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

  // returns AstFnCallExpr
  LL_NODISCARD AstNode *parse_function_call(const Lexer &lexer) noexcept;

  AstNode *parse_error(const Token &token, const char *format, ...) noexcept;

  bool is_new_line_between(const Lexer &lexer, const size_t start_pos, const size_t end_pos);

  // consumes the forbiden statement, report the error and return true else returns false
  bool is_forbiden_statement(const Token &token) noexcept;
};
