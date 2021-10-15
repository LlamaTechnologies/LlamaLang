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
struct AstIfStmnt;
struct AstLoopStmnt;
struct AstBlock;
struct AstParamDef;
struct AstVarDef;
struct AstType;
struct AstBinaryExpr;
struct AstUnaryExpr;
struct AstFnCallExpr;
struct AstCtrlStmnt;
struct FileInput;
struct AstConstArray;

bool match(const Token *token, ...) noexcept;
#define MATCH(token, ...) match(token, __VA_ARGS__, TokenId(size_t(TokenId::_EOF) + 1))

class Parser {
  std::vector<Error> &errors;
  const FileInput &file_input;

public:
  Parser(std::vector<Error> &in_error_vec);
  Parser(std::vector<Error> &in_error_vec, const FileInput &in_file_input);

  AstSourceCode *parse(const Lexer &lexer) noexcept;

  LL_NODISCARD AstSourceCode *parse_source_code(const Lexer &lexer) noexcept;

  LL_NODISCARD AstDirective *parse_directive(const Lexer &lexer) noexcept;

  // returns AstFnDef | AstFnProto
  LL_NODISCARD AstNode *parse_function_def(const Lexer &lexer) noexcept;

  LL_NODISCARD AstFnProto *parse_function_proto(const Lexer &lexer) noexcept;

  LL_NODISCARD AstParamDef *parse_param_def(const Lexer &lexer) noexcept;

  LL_NODISCARD AstIfStmnt *parse_branch_stmnt(const Lexer &lexer) noexcept;

  LL_NODISCARD AstLoopStmnt *parse_loop_stmnt(const Lexer &lexer) noexcept;

  LL_NODISCARD AstBlock *parse_block(const Lexer &lexer) noexcept;

  // returns AstSymbol | AstBinaryExpr | AstUnaryExpr
  LL_NODISCARD AstNode *parse_statement(const Lexer &lexer) noexcept;

  LL_NODISCARD AstVarDef *parse_vardef_stmnt(const Lexer &lexer) noexcept;

  LL_NODISCARD AstType *parse_type(const Lexer &lexer) noexcept;

  LL_NODISCARD AstBinaryExpr *parse_assign_stmnt(const Lexer &lexer) noexcept;

  LL_NODISCARD AstUnaryExpr *parse_ret_stmnt(const Lexer &lexer) noexcept;

  LL_NODISCARD AstCtrlStmnt *parse_ctrl_stmnt(const Lexer &lexer) noexcept;

  LL_NODISCARD AstFnCallExpr *parse_function_call(const Lexer &lexer) noexcept;

  LL_NODISCARD AstConstArray *parse_const_array(const Lexer &lexer) noexcept;

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

private:
  LL_NODISCARD bool _parse_load_directive(const Lexer &lexer, AstDirective *directive_node) noexcept;
  LL_NODISCARD AstIfStmnt *parse_if_stmnt(const Lexer &lexer) noexcept;
  LL_NODISCARD AstIfStmnt *parse_elif_stmnt(const Lexer &lexer, AstIfStmnt *if_stmnt) noexcept;
  LL_NODISCARD bool parse_else_stmnt(const Lexer &lexer, AstIfStmnt *elif_stmnt) noexcept;
  LL_NODISCARD AstNode *parse_const_expr(const Lexer &lexer, const Token &token, const Token &number_token) noexcept;
  LL_NODISCARD AstLoopStmnt *parse_whileloop_stmnt(const Lexer &lexer, const Token &loop_token) noexcept;
  LL_NODISCARD AstLoopStmnt *parse_eachloop_stmnt(const Lexer &lexer, const Token &loop_token) noexcept;
  LL_NODISCARD AstLoopStmnt *parse_rangeloop_stmnt(const Lexer &lexer, const Token &loop_token) noexcept;
};
