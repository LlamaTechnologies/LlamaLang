#include "parser.hpp"

#include "Types.hpp"
#include "ast_nodes.hpp"
#include "lexer.hpp"
#include "parse_error_msgs.hpp"

#include <cassert>
#include <stdarg.h>
#include <unordered_map>

static const char *get_const_char(const std::string_view str);
static BinaryExprType get_binary_op(const Token &token) noexcept;
static UnaryExprType get_unary_op(const Token &token) noexcept;
static bool is_type_start_token(const Token &token) noexcept;
static bool is_expr_token(const Token &token) noexcept;
static bool is_symbol_start_char(const char _char) noexcept;
static bool is_whitespace_char(const char _char) noexcept;
static bool get_directive_type(DirectiveType &, std::string_view in_directive_name);

//  ('=='|'!=' | '!' | '>=' | '<=' | '<' | '>')
#define COMPARATIVE_OPERATOR                                                                       \
  TokenId::EQUALS : case TokenId::NOT_EQUALS : case TokenId::NOT : case TokenId::GREATER_OR_EQUALS \
      : case TokenId::LESS_OR_EQUALS : case TokenId::LESS : case TokenId::GREATER

//('+' '++' | '-' '--' | '*' | '/' | '%')
#define ARITHMETIC_OPERATOR \
  TokenId::PLUS : case TokenId::MINUS : case TokenId::MUL : case TokenId::DIV : case TokenId::MOD

// ('<<' | '>>' | '&' | '^' | '~' | '|')
#define BITWISE_OPERATOR                                                                                         \
  TokenId::LSHIFT : case TokenId::RSHIFT : case TokenId::BIT_AND : case TokenId::BIT_XOR : case TokenId::BIT_NOT \
      : case TokenId::BIT_OR

Parser::Parser(const Lexer &in_lexer, std::vector<Error> &in_error_vec) : lexer(in_lexer), error_vec(in_error_vec) {}

AstNode *Parser::parse() noexcept { return parse_source_code(); }
/*
 * Parses any posible statement in llamacode
 * sourceFile
 *   : (functionProto | functionDef | varDef eos)* _EOF
 *   ;
 */
AstNode *Parser::parse_source_code() noexcept {
  if (!lexer.has_tokens()) {
    // TODO: handle empty source_code
    return nullptr;
  }

  const Token &first_token = lexer.get_next_token();
  lexer.get_back();

  AstNode *source_code_node =
    new AstNode(AstNodeType::AstSourceCode, first_token.start_line, first_token.start_column, lexer.file_name);

  for (;;) {
    AstNode *node = nullptr;

    const Token &token = lexer.get_next_token();
    if (token.id == TokenId::_EOF) {
      break;
    }

    switch (token.id) {
    case TokenId::FN:
    case TokenId::EXTERN: {
      lexer.get_back(); // token
      node = parse_function_def();
      if (!node) {
        // TODO: handle error
        continue;
      }
    } break;
    case TokenId::IDENTIFIER: {
      const Token &next_token = lexer.get_next_token();
      lexer.get_back(); // next_token

      if (next_token.id == TokenId::SEMI) {
        // ignore statement
        continue;
      }

      if (is_forbiden_statement(next_token)) {
        continue;
      }

      if (is_new_line_between(token.end_pos, next_token.start_pos)) {
        // ignore statement of type
        // IDENTIFIER\n
        continue;
      }

      lexer.get_back(); // token
      node = parse_vardef_stmnt();
      if (!node) {
        // TODO: handle error
        continue;
      }
    } break;
    case TokenId::SEMI:
      // Empty statement
    default:
      // TODO: handle Unexpected token
      continue;
    }

    // handle EOS (end of statement)
    const Token &semicolon_token = lexer.get_next_token();
    const Token &prev_semi_token = lexer.get_previous_token();
    if (semicolon_token.id != TokenId::SEMI) {
      if (semicolon_token.id != TokenId::_EOF &&
          !is_new_line_between(prev_semi_token.end_pos, semicolon_token.start_pos)) {
        // statement wrong ending
        parse_error(prev_semi_token, ERROR_EXPECTED_NEWLINE_OR_SEMICOLON_AFTER, lexer.get_token_value(prev_semi_token));
        delete node;
        continue;
      }

      // was not a semicolon.
      lexer.get_back();
    }

    node->parent = source_code_node;
    source_code_node->source_code.children.push_back(node);
  }

  return source_code_node;
}

/*
 * Parses a directive
 * directive
 *   : # 'load' string
 *   | # 'main'
 *   | # 'run' call
 *   | # 'compile'
 *   | # 'fn_type' type
 *   ;
 */
AstNode *Parser::parse_directive() noexcept {
  const Token &hash_token = lexer.get_next_token();
  if (hash_token.id != TokenId::HASH) {
    LL_UNREACHEABLE;
  }

  const Token &identifier_token = lexer.get_next_token();
  std::string_view dir_name = lexer.get_token_value(identifier_token);

  if (hash_token.id != TokenId::IDENTIFIER) {
    parse_error(hash_token, ERROR_UNEXPECTED_HASH, std::string(dir_name).c_str());
    return nullptr;
  }

  AstNode *directive_node =
    new AstNode(AstNodeType::AstDirective, hash_token.start_line, hash_token.start_column, lexer.file_name);

  DirectiveType dir_type;
  if (get_directive_type(dir_type, dir_name)) {
    delete directive_node;
    parse_error(identifier_token, ERROR_UNKNOWN_DIRECTIVE, std::string(dir_name).c_str());
    return nullptr;
  }

  directive_node->directive.directive_type = dir_type;

  switch (dir_type) {
  case DirectiveType::LOAD:
    // TODO(pablo96): implement load directive
    break;
  default:
    LL_UNREACHEABLE; // unimplemented directive
  }
  return nullptr;
}

/*
 * Parses a function definition
 * functionDef
 *   : functionProto block
 *   ;
 */
AstNode *Parser::parse_function_def() noexcept {
  const Token &fn_token = lexer.get_next_token();
  if (fn_token.id != TokenId::FN && fn_token.id != TokenId::EXTERN) {
    // Bad prediction
    LL_UNREACHEABLE;
  }

  lexer.get_back();
  auto func_prot_node = parse_function_proto();
  if (!func_prot_node) {
    // TODO(pablo96): handle error
    return nullptr;
  }

  const Token &l_curly_token = lexer.get_next_token();
  if (l_curly_token.id != TokenId::L_CURLY) {
    // just a function declaration (prototype)
    lexer.get_back();
    return func_prot_node;
  }
  if (func_prot_node->function_proto.is_extern) {
    parse_error(l_curly_token, ERROR_EXTERN_FN_HAS_BODY);
  }

  lexer.get_back();
  auto block_node = parse_block();
  if (!block_node) {
    // TODO(pablo96): handle error
    delete func_prot_node;
    return nullptr;
  }

  auto func_node = new AstNode(AstNodeType::AstFuncDef, fn_token.start_line, fn_token.start_column, lexer.file_name);
  func_prot_node->parent = func_node;
  block_node->parent = func_node;
  func_node->function_def.proto = func_prot_node;
  func_node->function_def.block = block_node;

  return func_node;
}

/*
 * Parses a function prototype
 * functionProto
 *   : 'extern? 'fn' IDENTIFIER '(' (parameterDecl (',' parameterDecl)*)? ')' type_name
 *   ;
 */
AstNode *Parser::parse_function_proto() noexcept {
  const Token &first_token = lexer.get_next_token();

  if (first_token.id != TokenId::EXTERN && first_token.id != TokenId::FN) {
    // Bad prediction
    LL_UNREACHEABLE;
  }

  const Token &fn_token = first_token.id == TokenId::EXTERN ? lexer.get_next_token() : first_token;
  if (fn_token.id != TokenId::FN && first_token.id == TokenId::EXTERN) {
    parse_error(fn_token, ERROR_UNEXPECTED_EXTERN, lexer.get_token_value(fn_token));
    return nullptr;
  }

  auto func_prot_node =
    new AstNode(AstNodeType::AstFuncProto, fn_token.start_line, fn_token.start_column, lexer.file_name);

  // set function as extern
  {
    if (first_token.id == TokenId::EXTERN) {
      func_prot_node->function_proto.is_extern = true;
    }
  }

  // function name
  {
    const Token &func_name_token = lexer.get_next_token();
    if (func_name_token.id != TokenId::IDENTIFIER) {
      // TODO(pablo96): handle error | UNEXPECTED TOKEN
      delete func_prot_node;
      return nullptr;
    }
    func_prot_node->function_proto.name = lexer.get_token_value(func_name_token);
  }

  // parameter list
  {
    const Token &l_paren_token = lexer.get_next_token();
    if (l_paren_token.id != TokenId::L_PAREN) {
      // TODO(pablo96): handle error | UNEXPECTED TOKEN
      return nullptr;
      delete func_prot_node;
    }

    for (;;) {
      const Token &token = lexer.get_next_token();

      if (token.id == TokenId::R_PAREN) {
        break;
      }

      if (token.id == TokenId::COMMA) {
        continue;
      }

      if (token.id == TokenId::_EOF) {
        const Token &prev_token = lexer.get_previous_token();
        parse_error(prev_token, ERROR_UNEXPECTED_EOF_AFTER, lexer.get_token_value(prev_token));
        delete func_prot_node;
        return nullptr;
      }

      lexer.get_back();
      auto param_node = parse_param_decl();
      if (!param_node) {
        // TODO(pablo96): handle error
        delete func_prot_node;
        return nullptr;
      }

      param_node->parent = func_prot_node;
      func_prot_node->function_proto.params.push_back(param_node);
    }
  }

  // return type
  {
    AstNode *ret_type_node = nullptr;
    const Token &ret_type_token = lexer.get_next_token();

    if (!is_type_start_token(ret_type_token)) {
      // TODO(pablo96): handle error | UNEXPECTED TOKEN
      delete func_prot_node;
      return nullptr;
    }

    lexer.get_back();
    ret_type_node = parse_type();
    if (!ret_type_node) {
      // TODO(pablo96): handle error
      delete func_prot_node;
      return nullptr;
    }

    ret_type_node->parent = func_prot_node;
    func_prot_node->function_proto.return_type = ret_type_node;
  }

  return func_prot_node;
}

/*
 * Parses a paramter declaration
 * parameterDecl
 *   : IDENTIFIER type_name
 *   ;
 */
AstNode *Parser::parse_param_decl() noexcept {
  const Token &name_token = lexer.get_next_token();

  if (name_token.id != TokenId::IDENTIFIER) {
    // Bad prediction
    LL_UNREACHEABLE;
  }

  AstNode *type_node = parse_type();

  if (!type_node) {
    // TODO(pablo96): handle error | wrong expression expected type name
    return nullptr;
  }

  AstNode *param_decl_node =
    new AstNode(AstNodeType::AstParamDecl, name_token.start_line, name_token.start_column, lexer.file_name);
  type_node->parent = param_decl_node;
  param_decl_node->param_decl.name = lexer.get_token_value(name_token);
  param_decl_node->param_decl.type = type_node;
  param_decl_node->param_decl.llvm_value = nullptr;
  param_decl_node->param_decl.initializer = nullptr;
  return param_decl_node;
}

/*
 * Parses a (function | if-else stmnt) block
 * block
 *   : '{' (statement eos)* '}'
 *   ;
 */
AstNode *Parser::parse_block() noexcept {
  const Token &l_curly_token = lexer.get_next_token();
  if (l_curly_token.id != TokenId::L_CURLY) {
    // bad_prediction
    LL_UNREACHEABLE;
  }

  AstNode *block_node =
    new AstNode(AstNodeType::AstBlock, l_curly_token.start_line, l_curly_token.start_column, lexer.file_name);

  for (;;) {
    const Token &token = lexer.get_next_token();

    if (token.id == TokenId::R_CURLY) {
      break;
    }

    if (token.id == TokenId::_EOF) {
      const Token &prev_token = lexer.get_previous_token();
      parse_error(prev_token, ERROR_UNEXPECTED_EOF_AFTER, lexer.get_token_value(prev_token));
      delete block_node;
      return nullptr;
    }

    lexer.get_back();
    AstNode *stmnt = parse_statement();
    if (!stmnt) {
      // TODO(pablo96): handle error in statement parsing
      delete block_node;
      return nullptr;
    }

    const Token &semicolon_token = lexer.get_next_token();
    if (semicolon_token.id != TokenId::SEMI) {
      bool has_new_line = is_new_line_between(token.end_pos, semicolon_token.start_pos);
      // checking for r_curly allows for '{stmnt}' as block
      if (semicolon_token.id != TokenId::R_CURLY && !has_new_line) {
        // statement wrong ending
        parse_error(token, ERROR_EXPECTED_NEWLINE_OR_SEMICOLON_AFTER, lexer.get_token_value(token));
        delete stmnt;
        delete block_node;
        return nullptr;
      }

      // was not a semicolon.
      lexer.get_back();
    }

    stmnt->parent = block_node;
    block_node->block.statements.push_back(stmnt);
  }

  return block_node;
}

/*
 * This predicts what statement to parse
 * statement
 *   : varDef
 *   | expression
 *   | assignmentStmnt
 *   | returnStmt
 *   | block
 *   | emptyStmt
 *   ;
 */
AstNode *Parser::parse_statement() noexcept {
  const Token &token = lexer.get_next_token();
  switch (token.id) {
  case TokenId::IDENTIFIER: {
    const Token &second_token = lexer.get_next_token();
    if (second_token.id == TokenId::ASSIGN) {
      if (lexer.get_next_token().id != TokenId::ASSIGN) {
        lexer.get_back();
        lexer.get_back(); // second_token
        lexer.get_back(); // token
        return parse_assign_stmnt();
      }
      // is '==' expression
      lexer.get_back();
      goto stmnt_expr;
    } else if (is_type_start_token(second_token)) {
      lexer.get_back(); // second_token
      lexer.get_back(); // token
      return parse_vardef_stmnt();
    }
stmnt_expr:
    lexer.get_back(); // second_token
    lexer.get_back(); // token
    return parse_expr();
  }
  case TokenId::RET:
    lexer.get_back();
    return parse_ret_stmnt();
  case TokenId::L_CURLY:
    // TODO(pablo96): parse block
    return nullptr;
  case TokenId::SEMI:
    // empty_statement
    // consume the token and predict again
    return parse_statement();
  case TokenId::_EOF:
    return nullptr;
  default:
    LL_UNREACHEABLE;
  }
}

/*
 * parses a variable definition/initialization
 * varDef
 *   : IDENTIFIER type_name ('=' expression)?
 *   ;
 */
AstNode *Parser::parse_vardef_stmnt() noexcept {
  const Token &token_symbol_name = lexer.get_next_token();

  if (token_symbol_name.id != TokenId::IDENTIFIER) {
    // Bad prediction
    LL_UNREACHEABLE;
  }

  AstNode *type_node = parse_type();

  if (!type_node) {
    // TODO(pablo96): handle error | wrong expression expected type name
    return nullptr;
  }

  AstNode *var_def_node =
    new AstNode(AstNodeType::AstVarDef, token_symbol_name.start_line, token_symbol_name.start_column, lexer.file_name);
  type_node->parent = var_def_node;
  var_def_node->var_def.name = lexer.get_token_value(token_symbol_name);
  var_def_node->var_def.type = type_node;

  const Token &assign_token = lexer.get_next_token();
  if (assign_token.id == TokenId::ASSIGN) {
    auto expr = parse_expr();
    if (!expr) {
      // TODO(pablo96): handle error in unary_expr => sync parsing
      return nullptr;
    }

    expr->parent = var_def_node;
    var_def_node->var_def.initializer = expr;
  } else {
    lexer.get_back();
    var_def_node->var_def.initializer = nullptr;
  }

  return var_def_node;
}

/*
 * type_name
 *   : '*' type_name
 *   | '[' ']' type_name
 *   | IDENTIFIER
 *   ;
 */
AstNode *Parser::parse_type() noexcept {
  const Token &token = lexer.get_next_token();
  if (token.id == TokenId::MUL) {
    // POINTER TYPE
    AstNode *type_node = new AstNode(AstNodeType::AstType, token.start_line, token.start_column, lexer.file_name);
    type_node->ast_type.type_id = AstTypeId::Pointer;
    const Token &next_token = lexer.get_next_token();
    if (!is_type_start_token(next_token)) {
      parse_error(next_token, ERROR_EXPECTED_TYPE_EXPR_INSTEAD_OF, lexer.get_token_value(next_token));
      lexer.get_back();
      delete type_node;
      return nullptr;
    }
    lexer.get_back();
    auto data_tye_node = parse_type();
    data_tye_node->parent = type_node;
    type_node->ast_type.child_type = data_tye_node;

    return type_node;
  } else if (token.id == TokenId::L_BRACKET) {
    // ARRAY TYPE
    const Token &r_braket_token = lexer.get_next_token();
    if (r_braket_token.id != TokenId::R_BRACKET) {
      parse_error(r_braket_token, ERROR_EXPECTED_CLOSING_BRAKET_BEFORE, lexer.get_token_value(r_braket_token));
      lexer.get_back();
      return nullptr;
    }
    AstNode *type_node = new AstNode(AstNodeType::AstType, token.start_line, token.start_column, lexer.file_name);
    type_node->ast_type.type_id = AstTypeId::Array;

    const Token &next_token = lexer.get_next_token();
    if (!is_type_start_token(next_token)) {
      parse_error(next_token, ERROR_EXPECTED_TYPE_EXPR_INSTEAD_OF, lexer.get_token_value(next_token));
      lexer.get_back();
      delete type_node;
      return nullptr;
    }
    lexer.get_back();
    auto data_tye_node = parse_type();
    data_tye_node->parent = type_node;
    type_node->ast_type.child_type = data_tye_node;

    return type_node;
  } else if (token.id == TokenId::IDENTIFIER) {
    AstNode *type_node = new AstNode(AstNodeType::AstType, token.start_line, token.start_column, lexer.file_name);
    type_node->ast_type.type_id = get_type_id(lexer.get_token_value(token), &type_node->ast_type.type_info);

    return type_node;
  } else if (token.id == TokenId::_EOF) {
    const Token &prev_token = lexer.get_previous_token();
    parse_error(prev_token, ERROR_UNEXPECTED_EOF_AFTER, lexer.get_token_value(prev_token));
    return nullptr;
  }
  // Bad prediction
  LL_UNREACHEABLE;
}

/*
 * Parses an assignment
 *
 * assignmentStmnt
 *   : IDENTIFIER assign_op expression
 *   ;
 */
AstNode *Parser::parse_assign_stmnt() noexcept {
  auto identifier_node = parse_primary_expr();
  if (!identifier_node) {
    // TODO(pablo96): error in unary_expr => sync parsing
    return nullptr;
  }

  const Token &token = lexer.get_next_token();
  if (token.id == TokenId::ASSIGN) {
    auto expr = parse_expr();
    if (!expr) {
      // TODO(pablo96): error in unary_expr => sync parsing
      return nullptr;
    }
    AstNode *node = new AstNode(AstNodeType::AstBinaryExpr, token.start_line, token.start_column, lexer.file_name);
    expr->parent = node;
    identifier_node->parent = node;
    node->binary_expr.bin_op = get_binary_op(token);
    node->binary_expr.left_expr = identifier_node;
    node->binary_expr.right_expr = expr;
    return node;
  }

  // Getting here means the prediction failed.
  LL_UNREACHEABLE;
}

/*
 * Parses a return statement
 * returnStmt
 *   | 'ret' expression?
 *   ;
 */
AstNode *Parser::parse_ret_stmnt() noexcept {
  const Token &token = lexer.get_next_token();
  if (token.id != TokenId::RET) {
    // Prediction error
    LL_UNREACHEABLE;
  }

  AstNode *node = new AstNode(AstNodeType::AstUnaryExpr, token.start_line, token.start_column, lexer.file_name);
  node->unary_expr.op = get_unary_op(token);

  if (is_expr_token(lexer.get_next_token())) {
    lexer.get_back();

    auto expr = parse_expr();
    if (!expr) {
      // TODO(pablo96): error in unary_expr => sync parsing
      return nullptr;
    }

    expr->parent = node;
    node->unary_expr.expr = expr;
  }
  // void return
  else {
    node->unary_expr.expr = nullptr;
    lexer.get_back();
  }

  return node;
}

/*
 * Parses any supported expresion
 * expression
 *   : '(' expression ')'
 *   | compExpr
 *   ;
 */
AstNode *Parser::parse_expr() noexcept {
  if (lexer.get_next_token().id == TokenId::L_PAREN) {
    auto expression = parse_expr();
    if (lexer.get_next_token().id != TokenId::R_PAREN) {
      const Token &prev_token = lexer.get_previous_token();
      parse_error(prev_token, ERROR_EXPECTED_R_PAREN_AFTER, lexer.get_token_value(prev_token));
      return nullptr;
    }
    return expression;
  }
  // Was not a ')'
  lexer.get_back();
  return parse_comp_expr();
}

/*
 * Parses comparative expresions
 * compExpr
 *   : algebraicExpr ('=='|'!=' | '>='| '<=' | '<'| '>') algebraicExpr
 *   | algebraicExpr
 *   ;
 */
AstNode *Parser::parse_comp_expr() noexcept {
  auto root_node = parse_algebraic_expr();
  if (!root_node) {
    // TODO(pablo96): error in primary expr => sync parsing
    return nullptr;
  }

  do {
    const Token &token = lexer.get_next_token();

    if (!MATCH(&token, TokenId::EQUALS, TokenId::NOT_EQUALS, TokenId::GREATER, TokenId::GREATER_OR_EQUALS,
               TokenId::LESS, TokenId::LESS_OR_EQUALS)) {
      // Not my token
      lexer.get_back();
      break;
    }

    AstNode *unary_expr = parse_algebraic_expr();
    if (!unary_expr) {
      // TODO(pablo96): error in unary_expr => sync parsing
      break;
    }

    // create binary node
    auto binary_expr = new AstNode(AstNodeType::AstBinaryExpr, token.start_line, token.start_column, lexer.file_name);
    unary_expr->parent = binary_expr;
    root_node->parent = binary_expr;
    binary_expr->binary_expr.left_expr = root_node;
    binary_expr->binary_expr.bin_op = get_binary_op(token);
    binary_expr->binary_expr.right_expr = unary_expr;

    // set the new node as root.
    root_node = binary_expr;
  } while (true);

  return root_node;
}

/*
 * Parses addition like expressions
 * algebraicExpr
 *   : termExpr ('+' | '-' | '|') termExpr
 *   | termExpr
 */
AstNode *Parser::parse_algebraic_expr() noexcept {
  auto root_node = parse_term_expr();
  if (!root_node) {
    // TODO(pablo96): error in primary expr => sync parsing
    return nullptr;
  }

  do {
    const Token &token = lexer.get_next_token();
    if (!MATCH(&token, TokenId::PLUS, TokenId::MINUS, TokenId::BIT_OR)) {
      // Not my token
      lexer.get_back();
      break;
    }

    auto term_expr = parse_term_expr();
    if (!term_expr) {
      // TODO(pablo96): error in term_expr => sync parsing
      break;
    }

    // create binary node
    auto binary_expr = new AstNode(AstNodeType::AstBinaryExpr, token.start_line, token.start_column, lexer.file_name);
    term_expr->parent = binary_expr;
    root_node->parent = binary_expr;
    binary_expr->binary_expr.left_expr = root_node;
    binary_expr->binary_expr.bin_op = get_binary_op(token);
    binary_expr->binary_expr.right_expr = term_expr;

    // set the new node as root.
    root_node = binary_expr;
  } while (true);

  return root_node;
}

/*
 * Parses multiplication like expressions
 * termExpr
 *   : primaryExpr ('*' | '/' | '%' | '<<' | '>>' | '&' | '^') primaryExpr
 *   | primaryExpr
 */
AstNode *Parser::parse_term_expr() noexcept {
  auto root_node = parse_unary_expr();

  if (!root_node) {
    // TODO(pablo96): error in primary expr => sync parsing
    return nullptr;
  }

  do {
    const Token &token = lexer.get_next_token();
    if (!MATCH(&token, TokenId::MUL, TokenId::DIV, TokenId::MOD, TokenId::LSHIFT, TokenId::RSHIFT, TokenId::BIT_AND,
               TokenId::BIT_XOR)) {
      // Not my token
      lexer.get_back();
      break;
    }

    auto symbol_token = parse_unary_expr();
    if (!symbol_token) {
      // TODO(pablo96): error in primary expr => sync parsing
      break;
    }

    // create binary node
    auto binary_expr = new AstNode(AstNodeType::AstBinaryExpr, token.start_line, token.start_column, lexer.file_name);
    symbol_token->parent = binary_expr;
    root_node->parent = binary_expr;
    binary_expr->binary_expr.left_expr = root_node;
    binary_expr->binary_expr.bin_op = get_binary_op(token);
    binary_expr->binary_expr.right_expr = symbol_token;

    // set the new node as root.
    root_node = binary_expr;
  } while (true);

  return root_node;
}

/*
 * Parses unary expresions
 * unaryExpr
 *   : ('!' | '~' | '--' | '++' | '-') primaryExpr
 *   | primaryExpr ('!' | '~' | '--' | '++' | '-')
 *   | primaryExpr
 *   ;
 */
AstNode *Parser::parse_unary_expr() noexcept {
consume_plus:
  const Token &unary_op_token = lexer.get_next_token();
  if (unary_op_token.id == TokenId::PLUS) {
    // ignore plus token
    goto consume_plus;
  }

  if (unary_op_token.id == TokenId::_EOF) {
    const Token &prev_token = lexer.get_previous_token();
    parse_error(prev_token, ERROR_UNEXPECTED_EOF_AFTER, lexer.get_token_value(prev_token));
    return nullptr;
  }

  // op primary_expr
  if (MATCH(&unary_op_token, TokenId::NOT, TokenId::BIT_NOT, TokenId::PLUS_PLUS, TokenId::MINUS_MINUS,
            TokenId::MINUS)) {
    if (unary_op_token.id == TokenId::MINUS)
      lexer.get_back();

    AstNode *node =
      new AstNode(AstNodeType::AstUnaryExpr, unary_op_token.start_line, unary_op_token.start_column, lexer.file_name);
    AstNode *primary_expr = parse_primary_expr();
    if (!primary_expr) {
      // TODO(pablo96): error in algebraic_expr => sync parsing
      // if this happens to be
      // ++ERROR_TOKEN
      // then we need to see if we can start over from the next token
      // for that we need to go back a step up into algebraic
      return nullptr;
    }
    primary_expr->parent = node;
    node->unary_expr.op = get_unary_op(unary_op_token);
    node->unary_expr.expr = primary_expr;
    return node;
  }

  lexer.get_back();
  AstNode *primary_expr = parse_primary_expr();
  if (!primary_expr) {
    // TODO(pablo96): error in algebraic_expr => sync parsing
    return nullptr;
  }

  const Token &token = lexer.get_next_token();
  // primary_expr op
  if (MATCH(&token, TokenId::NOT, TokenId::BIT_NOT, TokenId::PLUS_PLUS, TokenId::MINUS_MINUS)) {
    AstNode *node = new AstNode(AstNodeType::AstUnaryExpr, token.start_line, token.start_column, lexer.file_name);
    primary_expr->parent = node;
    node->unary_expr.expr = primary_expr;
    node->unary_expr.op = get_unary_op(token);
    return node;
  }
  lexer.get_back();

  return primary_expr;
}

/*
 * Parses primary expressions
 * primary_expr
 *   : expression
 *   | call_expr
 *   | IDENTIFIER
 *   | FLOAT_LIT
 *   | INT_LIT
 *   | UNICODE_CHAR
 */
AstNode *Parser::parse_primary_expr() noexcept {
  const Token &token = lexer.get_next_token();

  if (token.id == TokenId::_EOF) {
    const Token &prev_token = lexer.get_previous_token();
    parse_error(prev_token, ERROR_UNEXPECTED_EOF_AFTER, lexer.get_token_value(prev_token));
    return nullptr;
  }

  if (token.id == TokenId::L_PAREN) {
    // group expression
    lexer.get_back();
    return parse_expr();
  }

  if (token.id == TokenId::IDENTIFIER) {
    // CALL EXPR?
    const Token &next_token = lexer.get_next_token();
    if (next_token.id == TokenId::L_PAREN) {
      lexer.get_back(); // LPAREN
      lexer.get_back(); // IDENTIFIER
      return parse_function_call();
    }
    // else
    lexer.get_back();

    AstNode *symbol_node = new AstNode(AstNodeType::AstSymbol, token.start_line, token.start_column, lexer.file_name);
    symbol_node->symbol.token = &token;
    symbol_node->symbol.cached_name = lexer.get_token_value(token);
    return symbol_node;
  }

  bool is_negative = token.id == TokenId::MINUS;

  const Token &number_token = is_negative ? lexer.get_next_token() : token;

  if (MATCH(&number_token, TokenId::FLOAT_LIT, TokenId::INT_LIT, TokenId::UNICODE_CHAR)) {
    AstNode *const_value_node =
      new AstNode(AstNodeType::AstConstValue, token.start_line, token.start_column, lexer.file_name);
    switch (number_token.id) {
    case TokenId::INT_LIT: {
      const_value_node->const_value.type = ConstValueType::INT;
      auto number = const_value_node->const_value.number = number_token.int_lit.number;
      const_value_node->const_value.is_negative = number_token.int_lit.is_negative;
    } break;
    case TokenId::FLOAT_LIT:
      const_value_node->const_value.type = ConstValueType::FLOAT;
      const_value_node->const_value.number = number_token.float_lit.number;
      break;
    case TokenId::UNICODE_CHAR:
      const_value_node->const_value.type = ConstValueType::CHAR;
      const_value_node->const_value.unicode_char = number_token.char_lit;
      break;
    default:
      LL_UNREACHEABLE;
    }

    return const_value_node;
  }

  auto token_value = lexer.get_token_value(number_token);
  parse_error(number_token, ERROR_EXPECTED_NUMBER_IDENTIFIER_CHAR_TOKEN, token_value, token_id_name(number_token.id));
  return nullptr;
}

/*
 * functionCallExpr
 *   : IDENTIFIER '(' (expression (, expression))? ')'
 *   ;
 */
AstNode *Parser::parse_function_call() noexcept {
  const Token &name_token = lexer.get_next_token();
  if (name_token.id != TokenId::IDENTIFIER) {
    // bad prediction
    LL_UNREACHEABLE;
  }

  const Token &lparen_token = lexer.get_next_token();
  if (lparen_token.id != TokenId::L_PAREN) {
    // bad prediction
    LL_UNREACHEABLE;
  }

  AstNode *func_call_node =
    new AstNode(AstNodeType::AstFuncCallExpr, name_token.start_line, name_token.start_column, lexer.file_name);
  func_call_node->func_call.fn_name = lexer.get_token_value(name_token);

  // arguments
  for (;;) {
    const Token &token = lexer.get_next_token();
    if (token.id == TokenId::R_PAREN) {
      break;
    }

    if (token.id == TokenId::COMMA) {
      continue;
    }

    if (token.id == TokenId::_EOF) {
      const Token &prev_token = lexer.get_previous_token();
      parse_error(prev_token, ERROR_UNEXPECTED_EOF_AFTER, lexer.get_token_value(prev_token));
      delete func_call_node;
      return nullptr;
    }

    lexer.get_back();
    auto expr = parse_expr();
    if (!expr) {
      // TODO(pablo96): handle error | find a comma or rparen
      continue;
    }
    expr->parent = func_call_node;
    func_call_node->func_call.args.push_back(expr);
  }

  return func_call_node;
}

AstNode *Parser::parse_error(const Token &token, const char *format, ...) noexcept {
  va_list ap, ap2;
  va_start(ap, format);
  va_copy(ap2, ap);

  int len1 = snprintf(nullptr, 0, format, ap);
  assert(len1 >= 0);

  std::string msg;
  msg.reserve(len1 + 1);

  int len2 = snprintf(msg.data(), msg.capacity(), format, ap2);
  assert(len2 >= 0);
  assert(len2 == len1);

  Error error(ERROR_TYPE::ERROR, token.start_line, token.start_column, lexer.file_name, msg);

  va_end(ap);
  va_end(ap2);
  return nullptr;
}

bool Parser::is_new_line_between(const size_t start_pos, const size_t end_pos) {
  auto start_it = lexer.source.data();
  auto len = end_pos - start_pos;
  auto str_view = std::string_view(start_it + start_pos, len);

  return str_view.find_first_of('\n') != str_view.npos;
}

bool Parser::is_forbiden_statement(const Token &token) noexcept {
  switch (token.id) {
  case TokenId::ASSIGN: {
    // IDENTIFIER = ...
    // TODO(pablo96): Wrong place for assingment
    return true;
  }
  case TokenId::L_PAREN: {
    // IDENTIFIER(...
    // TODO(pablo96): Wrong place for function call
    return true;
  }
  case COMPARATIVE_OPERATOR: {
    // IDENTIFIER ==...
    // TODO(pablo96): Wrong place for comparative expr
    return true;
  }
  case ARITHMETIC_OPERATOR: {
    // IDENTIFIER +...
    // TODO(pablo96): Wrong place for arithmetic expr
    return true;
  }
  case BITWISE_OPERATOR: {
    // IDENTIFIER &...
    // TODO(pablo96): Wrong place for bitwise expr
    return true;
  }
  default:
    return false;
  }
}

BinaryExprType get_binary_op(const Token &token) noexcept {
  switch (token.id) {
  case TokenId::PLUS:
    return BinaryExprType::ADD;
  case TokenId::MINUS:
    return BinaryExprType::SUB;
  case TokenId::MUL:
    return BinaryExprType::MUL;
  case TokenId::DIV:
    return BinaryExprType::DIV;
  case TokenId::MOD:
    return BinaryExprType::MOD;
  case TokenId::LSHIFT:
    return BinaryExprType::LSHIFT;
  case TokenId::RSHIFT:
    return BinaryExprType::RSHIFT;
  case TokenId::BIT_XOR:
    return BinaryExprType::BIT_XOR;
  case TokenId::BIT_AND:
    return BinaryExprType::BIT_AND;
  case TokenId::ASSIGN:
    return BinaryExprType::ASSIGN;
  case TokenId::EQUALS:
    return BinaryExprType::EQUALS;
  case TokenId::NOT_EQUALS:
    return BinaryExprType::NOT_EQUALS;
  case TokenId::GREATER_OR_EQUALS:
    return BinaryExprType::GREATER_OR_EQUALS;
  case TokenId::GREATER:
    return BinaryExprType::GREATER;
  case TokenId::LESS:
    return BinaryExprType::LESS;
  case TokenId::LESS_OR_EQUALS:
    return BinaryExprType::LESS_OR_EQUALS;
  default:
    LL_UNREACHEABLE;
  }
}

UnaryExprType get_unary_op(const Token &token) noexcept {
  switch (token.id) {
  case TokenId::PLUS_PLUS:
    return UnaryExprType::INC;
  case TokenId::MINUS_MINUS:
    return UnaryExprType::DEC;
  case TokenId::MINUS:
    return UnaryExprType::NEG;
  case TokenId::BIT_NOT:
    return UnaryExprType::BIT_INV;
  case TokenId::RET:
    return UnaryExprType::RET;
  default:
    LL_UNREACHEABLE;
  }
}

bool is_type_start_token(const Token &token) noexcept {
  switch (token.id) {
  case TokenId::IDENTIFIER:
  case TokenId::L_BRACKET:
  case TokenId::MUL:
    return true;
  default:
    return false;
  }
}

bool is_expr_token(const Token &token) noexcept {
  switch (token.id) {
  case TokenId::IDENTIFIER:
  case TokenId::FLOAT_LIT:
  case TokenId::INT_LIT:
  case TokenId::UNICODE_CHAR:
  case TokenId::NOT:
  case TokenId::BIT_NOT:
  case TokenId::PLUS_PLUS:
  case TokenId::MINUS_MINUS:
  case TokenId::MUL:
  case TokenId::DIV:
  case TokenId::MOD:
  case TokenId::LSHIFT:
  case TokenId::RSHIFT:
  case TokenId::BIT_AND:
  case TokenId::BIT_XOR:
  case TokenId::PLUS:
  case TokenId::MINUS:
  case TokenId::BIT_OR:
  case TokenId::EQUALS:
  case TokenId::NOT_EQUALS:
  case TokenId::GREATER:
  case TokenId::GREATER_OR_EQUALS:
  case TokenId::LESS:
  case TokenId::LESS_OR_EQUALS:
  case TokenId::L_PAREN:
    return true;
  default:
    return false;
  }
}

bool is_symbol_start_char(const char next_char) noexcept {
  return (next_char >= 'a' && next_char <= 'z') || (next_char >= 'A' && next_char <= 'Z') || next_char == '_';
}

bool is_whitespace_char(const char _char) noexcept {
  switch (_char) {
  case '\t':
  case '\r':
  case '\n':
  case ' ':
    return true;
  default:
    return false;
  }
}

const std::unordered_map<std::string_view, DirectiveType> directives = { { "load", DirectiveType::LOAD },
                                                                         { "main", DirectiveType::MAIN },
                                                                         { "run", DirectiveType::RUN },
                                                                         { "compile", DirectiveType::COMPILE },
                                                                         { "fn_type", DirectiveType::FN_TYPE } };
bool get_directive_type(DirectiveType &dir_type, std::string_view in_directive_name) {
  bool exists = directives.find(in_directive_name) != directives.end();
  if (exists) {
    dir_type = directives.at(in_directive_name);
  }
  return exists;
}

bool match(const Token *token, ...) noexcept {
  va_list vargv;
  va_start(vargv, token);

  do {
    const TokenId id = va_arg(vargv, TokenId);

    // run out of token_ids
    if (size_t(id) > size_t(TokenId::_EOF)) {
      va_end(vargv);
      return false;
    }

    // found match
    if (token->id == id) {
      va_end(vargv);
      return true;
    }
  } while (true);
}

const char *get_const_char(const std::string_view str) {
  const auto src_size = str.size();
  const auto size = src_size + 1;
  char *str_nt = new char[size];
  strncpy(str_nt, str.data(), src_size);
  str_nt[src_size] = 0;
  return str_nt;
}
