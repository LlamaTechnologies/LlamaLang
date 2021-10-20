#include "parser.hpp"

#include "Types.hpp"
#include "ast_nodes.hpp"
#include "file_utils.hpp"
#include "lexer.hpp"
#include "parse_error_msgs.hpp"
#include "src_code_repository.hpp"

#include <cassert>
#include <stdarg.h>
#include <stdlib.h>
#include <unordered_map>

static bool _is_boolean_expression(const AstNode *in_expr, AstNode *if_stmnt_node);
static bool _is_after_undef(const Lexer &lexer, std::vector<Error> &in_errors, AstNode *in_node,
                            bool (*predicate)(const Token &)) noexcept;
static bool _is_before_undef(const Lexer &lexer, std::vector<Error> &in_errors, AstNode *in_node) noexcept;
static void _parse_error(std::vector<Error> &in_errors, const Token &token, const char *format, ...) noexcept;
static void _parse_warning(std::vector<Error> &in_errors, const Token &token, const char *format, ...) noexcept;
static bool _get_file_name_and_path(const FileInput &file_input, std::vector<Error> &in_errors,
                                    const Token &in_string_token, const std::string_view in_relative_file_name,
                                    const std::string &in_file_directory, std::string &out_file_path,
                                    std::string &out_file_name);
static const char *_get_const_char(const std::string_view str);
static BinaryExprType _get_binary_op(const Token &token) noexcept;
static UnaryExprType _get_unary_op(const Token &token) noexcept;
static bool _is_type_start_token(const Token &token) noexcept;
static bool _is_expr_token(const Token &token) noexcept;
static bool _is_symbol_start_char(const s8 _char) noexcept;
static bool _is_whitespace_char(const s8 _char) noexcept;
/* Returns true if it is a valid directive type */
static bool _get_directive_type(DirectiveType &, std::string_view in_directive_name);
static bool _end_of_statement(const Lexer &lexer, std::vector<Error> errors, const TokenId terminator);
static bool _is_new_line_between(const Lexer &lexer, const size_t start_pos, const size_t end_pos);
static bool _is_forbiden_statement(const Token &token, const Token &next_token) noexcept;
static bool _is_var_def(const Lexer &lexer, const Token &token);
static AstBlock *_create_block(const Token &token, AstNode *parent);
static AstBinaryExpr *_create_bin_expr(const Token &token, const BinaryExprType bin_op, AstNode *parent, AstNode *left,
                                       AstNode *right);

//  ('=='|'!=' | '!' | '>=' | '<=' | '<' | '>')
#define COMPARATIVE_OPERATOR                                                                       \
  TokenId::EQUALS : case TokenId::NOT_EQUALS : case TokenId::NOT : case TokenId::GREATER_OR_EQUALS \
      : case TokenId::LESS_OR_EQUALS : case TokenId::LESS : case TokenId::GREATER

//('+' '++' | '-' '--' | '*' | '/' | '%')
#define ARITHMETIC_OPERATOR \
  TokenId::PLUS : case TokenId::MINUS : case TokenId::MUL : case TokenId::DIV : case TokenId::MOD

// ('<<' | '>>' | '&' | '^' | '~' | '|')
#define BITWISE_OPERATOR                                                                                           \
  TokenId::LSHIFT : case TokenId::RSHIFT : case TokenId::AMPERSAND : case TokenId::BIT_XOR : case TokenId::BIT_NOT \
      : case TokenId::BIT_OR

Parser::Parser(std::vector<Error> &in_error_vec, const FileInput &_file_input)
    : errors(in_error_vec), file_input(_file_input) {}

Parser::Parser(std::vector<Error> &in_error_vec) : errors(in_error_vec), file_input(get_std_file_input()) {}

AstSourceCode *Parser::parse(const Lexer &lexer) noexcept { return parse_source_code(lexer); }
/*
 * Parses any posible statement in llamacode
 * sourceFile
 *   : (functionProto | functionDef | varDef eos)* _EOF
 *   ;
 */
AstSourceCode *Parser::parse_source_code(const Lexer &lexer) noexcept {
  if (!lexer.has_tokens()) {
    // TODO: handle empty source_code
    return nullptr;
  }

  const Token &first_token = lexer.get_next_token();
  lexer.get_back();

  AstSourceCode *source_code_node =
    new AstSourceCode(first_token.start_line, first_token.start_column, first_token.file_name);

  for (;;) {
    AstNode *node = nullptr;

    const Token &token = lexer.get_next_token();
    if (token.id == TokenId::_EOF) {
      break;
    }

    switch (token.id) {
    case TokenId::HASH: {
      lexer.get_back(); // token
      node = parse_directive(lexer);
      if (!node) {
        // TODO: handle error
        continue;
      }
    } break;
    case TokenId::FN:
    case TokenId::EXTERN: {
      lexer.get_back(); // token
      node = parse_function_def(lexer);
      if (!node) {
        // TODO: handle error
        continue;
      }
    } break;
    case TokenId::IDENTIFIER: {
      if (!_is_var_def(lexer, token)) {
        continue;
      }

      node = parse_vardef_stmnt(lexer);
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
    if (_end_of_statement(lexer, errors, TokenId::_EOF) == false) {
      delete node;
      continue;
    }

    node->parent = source_code_node;
    source_code_node->children.push_back(node);
  }

  return source_code_node;
}

bool _end_of_statement(const Lexer &lexer, std::vector<Error> errors, const TokenId terminator) {
  const Token &semicolon_token = lexer.get_next_token();
  const Token &prev_semi_token = lexer.get_previous_token();
  if (semicolon_token.id != TokenId::SEMI) {
    if (semicolon_token.id != terminator &&
        !_is_new_line_between(lexer, prev_semi_token.end_pos, semicolon_token.start_pos)) {
      // statement wrong ending
      _parse_error(errors, prev_semi_token, ERROR_EXPECTED_NEWLINE_OR_SEMICOLON_AFTER,
                   lexer.get_token_value(prev_semi_token));
      return false;
    }

    // was not a semicolon.
    lexer.get_back();
  }
  return true;
}

bool _is_var_def(const Lexer &lexer, const Token &token) {
  const Token &next_token = lexer.get_next_token();
  lexer.get_back(); // next_token

  if (next_token.id == TokenId::SEMI) {
    // ignore statement
    return false;
  }

  const Token &next_next_token = lexer.get_next_token();
  if (_is_forbiden_statement(token, next_next_token)) {
    return false;
  }
  lexer.get_back();

  if (_is_new_line_between(lexer, token.end_pos, next_token.start_pos)) {
    // ignore statement of type
    // IDENTIFIER\n
    return false;
  }

  lexer.get_back(); // token
  return true;
}

/*
 * Parses a directive
 * directive
 *   : # 'load' string
 *   | # 'main'
 *   | # 'run' call
 *   | # 'compile'
 *   | # 'fn_type' identifier
 *   ;
 */
AstDirective *Parser::parse_directive(const Lexer &lexer) noexcept {
  const Token &hash_token = lexer.get_next_token();
  if (hash_token.id != TokenId::HASH) {
    LL_UNREACHEABLE;
  }

  const Token &identifier_token = lexer.get_next_token();
  std::string_view identifier_value = lexer.get_token_value(identifier_token);

  if (identifier_token.id != TokenId::IDENTIFIER) {
    _parse_error(errors, hash_token, ERROR_UNEXPECTED_HASH, std::string(identifier_value).c_str());
    return nullptr;
  }

  AstDirective *directive_node = new AstDirective(hash_token.start_line, hash_token.start_column, hash_token.file_name);

  DirectiveType dir_type;
  if (_get_directive_type(dir_type, identifier_value) == false) {
    delete directive_node;
    _parse_error(errors, identifier_token, ERROR_UNKNOWN_DIRECTIVE, std::string(identifier_value).c_str());
    return nullptr;
  }

  directive_node->directive_type = dir_type;

  switch (dir_type) {
  case DirectiveType::LOAD: {
    if (_parse_load_directive(lexer, directive_node) == false) {
      delete directive_node;
      return nullptr;
    }
    return directive_node;
  } break;
  default:
    LL_UNREACHEABLE; // unimplemented directive
  }
}

bool Parser::_parse_load_directive(const Lexer &lexer, AstDirective *directive_node) noexcept {
  const Token &file_name_token = lexer.get_next_token();
  std::string_view relative_file_path = lexer.get_token_value(file_name_token);

  if (file_name_token.id != TokenId::STRING) {
    _parse_error(this->errors, file_name_token, ERROR_DIRECTIVE_EXPECTED_STRING,
                 std::string(relative_file_path).c_str());
    return false;
  }

  std::string file_name;
  std::string file_directory;
  if (_get_file_name_and_path(this->file_input, this->errors, file_name_token, relative_file_path, lexer.file_directory,
                              file_directory, file_name) == false) {
    // path was not a file, file did not exist or file was no ".llama"
    return false;
  }

  std::string file_path = file_directory + std::filesystem::path::preferred_separator + file_name;
  // copy file_path to directive argument
  {
    char *str = (char *)malloc(file_path.size() + 1);
    ::memcpy(str, file_path.c_str(), file_path.size() + 1);
    directive_node->argument.str = str;
  }

  RepositorySrcCode &code_repository = RepositorySrcCode::get();
  // only parse the file if not in the code repository
  if (!code_repository.has_file(file_path)) {
    size_t errors_before = this->errors.size();

    std::string source_code_str = this->file_input.open_and_read_file(file_path);

    // create new lexer and tokenize.
    Lexer *loaded_file_lexer = new Lexer(source_code_str, file_directory, file_name, this->errors);
    loaded_file_lexer->tokenize();

    // there were errors in lexing.
    if (errors_before > this->errors.size()) {
      // TODO(pablo96): error handling
      return false;
    };

    // parse source code.
    AstSourceCode *src_code_node = this->parse(*loaded_file_lexer);

    /* NOTE(pablo96): Since AstSourceCode owns the lexer pointer
     * only loaded SourceCodes have the lexer pointer.
     */
    src_code_node->lexer = loaded_file_lexer;

    // add parsed source code to the repository.
    code_repository._add_source_code(file_path, src_code_node);
  }

  return true;
}

/*
 * Parses a function definition
 * functionDef
 *   : functionProto block
 *   ;
 */
AstNode *Parser::parse_function_def(const Lexer &lexer) noexcept {
  const Token &fn_token = lexer.get_next_token();
  if (fn_token.id != TokenId::FN && fn_token.id != TokenId::EXTERN) {
    // Bad prediction
    LL_UNREACHEABLE;
  }

  lexer.get_back();
  auto func_prot_node = parse_function_proto(lexer);
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
  if (func_prot_node->is_extern) {
    _parse_error(errors, l_curly_token, ERROR_EXTERN_FN_HAS_BODY);
  }

  lexer.get_back();
  auto block_node = parse_block(lexer);
  if (!block_node) {
    // TODO(pablo96): handle error
    delete func_prot_node;
    return nullptr;
  }

  auto func_node = new AstFnDef(fn_token.start_line, fn_token.start_column, fn_token.file_name);
  func_prot_node->parent = func_node;
  block_node->parent = func_node;
  func_node->proto = func_prot_node;
  func_node->block = block_node;

  return func_node;
}

/*
 * Parses a function prototype
 * functionProto
 *   : 'extern? 'fn' IDENTIFIER '(' (parameterDecl (',' parameterDecl)*)? ')' type_name
 *   ;
 */
AstFnProto *Parser::parse_function_proto(const Lexer &lexer) noexcept {
  const Token &first_token = lexer.get_next_token();

  if (first_token.id != TokenId::EXTERN && first_token.id != TokenId::FN) {
    // Bad prediction
    LL_UNREACHEABLE;
  }

  const Token &fn_token = first_token.id == TokenId::EXTERN ? lexer.get_next_token() : first_token;
  if (fn_token.id != TokenId::FN && first_token.id == TokenId::EXTERN) {
    _parse_error(errors, fn_token, ERROR_UNEXPECTED_EXTERN, lexer.get_token_value(fn_token));
    return nullptr;
  }

  auto func_prot_node = new AstFnProto(fn_token.start_line, fn_token.start_column, fn_token.file_name);

  // set function as extern
  {
    if (first_token.id == TokenId::EXTERN) {
      func_prot_node->is_extern = true;
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
    func_prot_node->name = lexer.get_token_value(func_name_token);
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
        _parse_error(errors, prev_token, ERROR_UNEXPECTED_EOF_AFTER, lexer.get_token_value(prev_token));
        delete func_prot_node;
        return nullptr;
      }

      lexer.get_back();
      auto param_node = parse_param_def(lexer);
      if (!param_node) {
        // TODO(pablo96): handle error
        delete func_prot_node;
        return nullptr;
      }

      param_node->parent = func_prot_node;
      func_prot_node->params.push_back(param_node);
    }
  }

  // return type
  {
    const Token &ret_type_token = lexer.get_next_token();

    if (!_is_type_start_token(ret_type_token)) {
      // TODO(pablo96): handle error | UNEXPECTED TOKEN
      delete func_prot_node;
      return nullptr;
    }

    lexer.get_back();
    auto ret_type_node = parse_type(lexer);
    if (!ret_type_node) {
      // TODO(pablo96): handle error
      delete func_prot_node;
      return nullptr;
    }

    ret_type_node->parent = func_prot_node;
    func_prot_node->return_type = ret_type_node;
  }

  return func_prot_node;
}

/*
 * Parses a paramter declaration
 * parameterDecl
 *   : IDENTIFIER type_name
 *   ;
 */
AstParamDef *Parser::parse_param_def(const Lexer &lexer) noexcept {
  const Token &name_token = lexer.get_next_token();

  if (name_token.id != TokenId::IDENTIFIER) {
    // Bad prediction
    LL_UNREACHEABLE;
  }

  const Token &type_token = lexer.get_next_token();
  switch (type_token.id) {
  case TokenId::R_PAREN:
  case TokenId::COMMA: {
    _parse_error(errors, name_token, ERROR_UNNAMED_PARAM, lexer.get_token_value(name_token));
    return nullptr;
  }
  default:
    break;
  }
  lexer.get_back();

  AstType *type_node = parse_type(lexer);

  if (!type_node) {
    // TODO(pablo96): handle error | wrong expression expected type name
    return nullptr;
  }

  AstParamDef *param_decl_node = new AstParamDef(name_token.start_line, name_token.start_column, name_token.file_name);
  type_node->parent = param_decl_node;
  param_decl_node->name = lexer.get_token_value(name_token);
  param_decl_node->type = type_node;
  param_decl_node->llvm_value = nullptr;
  param_decl_node->initializer = nullptr;
  return param_decl_node;
}

/*
 * Parses an if/elif/else statement
 * if_stmnt
 *   : 'if' expr block ('elif' expr block)* ('else' block)?
 *   ;
 */
AstIfStmnt *Parser::parse_branch_stmnt(const Lexer &lexer) noexcept {
  AstIfStmnt *if_stmnt_node = parse_if_stmnt(lexer);
  if (if_stmnt_node == nullptr) {
    return nullptr;
  }

  // PARSE ELIF
  AstIfStmnt *last_elif_stmnt = parse_elif_stmnt(lexer, if_stmnt_node);
  if (last_elif_stmnt == nullptr) {
    delete if_stmnt_node;
    return nullptr;
  }

  // PARSE ELSE
  if (parse_else_stmnt(lexer, last_elif_stmnt) == false) {
    delete if_stmnt_node;
    return nullptr;
  }

  return if_stmnt_node;
}

AstIfStmnt *Parser::parse_if_stmnt(const Lexer &lexer) noexcept {
  const Token &if_token = lexer.get_next_token();

  if (if_token.id != TokenId::IF) {
    // compiler bug! we did not predict correctly
    LL_UNREACHEABLE;
  }

  AstNode *conditional_expr = parse_expr(lexer);

  AstIfStmnt *if_stmnt_node = new AstIfStmnt(if_token.start_line, if_token.start_column, if_token.file_name);

  if (!_is_boolean_expression(conditional_expr, if_stmnt_node)) {
    _parse_error(errors, if_token, ERROR_BRANCH_EXPR_NOT_BOOL);
    // NOTE(pablo96): we dont break here since we want to parse as much as we can
  }

  if_stmnt_node->condition_expr = conditional_expr;

  // PARSE BLOCK
  const Token &true_lcurly_token = lexer.get_next_token();
  if (true_lcurly_token.id != TokenId::L_CURLY) {
    // Warning: empty branch
    _parse_warning(errors, lexer.get_previous_token(), WARN_EMPTY_BRANCH);
    return if_stmnt_node;
  }

  lexer.get_back();
  AstBlock *true_block_node = parse_block(lexer);
  if (true_block_node == nullptr) {
    // TODO(pablo96): handle error in block parsing
    delete if_stmnt_node;
    return nullptr;
  }

  true_block_node->parent = if_stmnt_node;
  if_stmnt_node->true_block = true_block_node;

  return if_stmnt_node;
}

AstIfStmnt *Parser::parse_elif_stmnt(const Lexer &lexer, AstIfStmnt *if_stmnt) noexcept {
  // PARSE ELIFs
  AstIfStmnt *prev_if_stmnt = if_stmnt;
  while (true) {
    const Token &elif_token = lexer.get_next_token();
    if (elif_token.id != TokenId::ELIF) {
      lexer.get_back();
      break;
    }

    AstNode *elif_conditional_expr = parse_expr(lexer);
    if (elif_conditional_expr == nullptr) {
      // TODO(pablo96): handle error
      return nullptr;
    }

    AstIfStmnt *elif_stmnt = new AstIfStmnt(elif_token.start_line, elif_token.start_column, elif_token.file_name);

    if (!_is_boolean_expression(elif_conditional_expr, elif_stmnt)) {
      _parse_error(errors, elif_token, ERROR_BRANCH_EXPR_NOT_BOOL);
      // NOTE(pablo96): we dont break here since we want to parse as much as we can
    }

    elif_stmnt->condition_expr = elif_conditional_expr;

    // PARSE ELIF BLOCK
    const Token &elif_lcurly_token = lexer.get_next_token();
    if (elif_lcurly_token.id != TokenId::L_CURLY) {
      // Warning: empty branch
      _parse_warning(errors, lexer.get_previous_token(), WARN_EMPTY_BRANCH);
      continue;
    }

    lexer.get_back();
    AstBlock *elif_block_node = parse_block(lexer);
    if (elif_block_node == nullptr) {
      // TODO(pablo96): handle error in block parsing
      return nullptr;
    }

    elif_block_node->parent = elif_stmnt;
    elif_stmnt->true_block = elif_block_node;

    // ELIF is an IF stmnt in a ELSE block
    AstBlock *else_block = _create_block(elif_token, prev_if_stmnt);
    else_block->statements.push_back(elif_stmnt);

    prev_if_stmnt->false_block = else_block;
    prev_if_stmnt = elif_stmnt;
  }

  LL_ASSERT(prev_if_stmnt->false_block == nullptr);

  return prev_if_stmnt;
}

bool Parser::parse_else_stmnt(const Lexer &lexer, AstIfStmnt *last_elif_stmnt) noexcept {
  const Token &else_token = lexer.get_next_token();
  if (else_token.id != TokenId::ELSE) {
    lexer.get_back();
    return true;
  }

  // PARSE ELSE BLOCK
  const Token &false_lcurly_token = lexer.get_next_token();
  if (false_lcurly_token.id != TokenId::L_CURLY) {
    // Warning: empty branch
    _parse_warning(errors, lexer.get_previous_token(), WARN_EMPTY_BRANCH);
    return true;
  }

  lexer.get_back();
  AstBlock *false_block_node = parse_block(lexer);
  if (false_block_node == nullptr) {
    // TODO(pablo96): handle error in block parsing
    return false;
  }

  false_block_node->parent = last_elif_stmnt;
  last_elif_stmnt->false_block = false_block_node;

  return true;
}

/*
 * Parses a loop statement
 * loop_stmnt
 *   : 'loop' expr  block #while
 *   | 'loop' IDENTIFIER ('=' INTEGER)? ':' (IDENTIFIER | INTEGER) (';' INTEGER)? #range
 *   | 'loop' IDENTIFIER ':' IDENTIFIER (';' ( (IDENTIFIER (, INTEGER)?) | INTEGER) )? #each
 *   ;
 */
AstLoopStmnt *Parser::parse_loop_stmnt(const Lexer &lexer) noexcept {
  const Token &loop_token = lexer.get_next_token();
  if (loop_token.id != TokenId::LOOP) {
    // compiler bug! we did not predict correctly
    LL_UNREACHEABLE;
  }

  // try to predict loop type
  const Token &token = lexer.get_next_token();
  if (token.id == TokenId::IDENTIFIER) {
    const Token &eq_colon_token = lexer.get_next_token();
    if (eq_colon_token.id == TokenId::ASSIGN) {
      lexer.get_back(); // '=' token
      lexer.get_back(); // IDENTIFIER token
      return parse_rangeloop_stmnt(lexer, loop_token);
    }

    if (eq_colon_token.id == TokenId::COLON) {
      const Token &id_num_token = lexer.get_next_token();
      if (id_num_token.id == TokenId::INT_LIT) {
        lexer.get_back(); // IDENTIFIER | NUMBER token
        lexer.get_back(); // ':' token
        lexer.get_back(); // IDENTIFIER token
        return parse_rangeloop_stmnt(lexer, loop_token);
      }

      // else we parse it as a each loop and let the semantic phase check the syntax
      lexer.get_back(); // IDENTIFIER | NUMBER token
      lexer.get_back(); // ':' token
      lexer.get_back(); // IDENTIFIER token
      return parse_eachloop_stmnt(lexer, loop_token);
    }

    // else is a while loop
    lexer.get_back(); // '=' or ':' token
  }

  lexer.get_back(); // IDENTIFIER token

  return parse_whileloop_stmnt(lexer, loop_token);
}

/*
 * Parses a loop statement
 * loop_stmnt
 *   : 'loop' boolExpr  block
 */
AstLoopStmnt *Parser::parse_whileloop_stmnt(const Lexer &lexer, const Token &loop_token) noexcept {
  AstNode *conditional_expr = parse_expr(lexer);

  AstLoopStmnt *loop_stmnt = new AstLoopStmnt(loop_token.start_line, loop_token.start_column, loop_token.file_name);
  loop_stmnt->condition_expr = conditional_expr;
  conditional_expr->parent = loop_stmnt;

  if (!_is_boolean_expression(conditional_expr, loop_stmnt)) {
    _parse_error(errors, loop_token, ERROR_BRANCH_EXPR_NOT_BOOL);
    // NOTE(pablo96): we dont break here since we want to parse as much as we can
  }

  const Token &true_lcurly_token = lexer.get_next_token();
  if (true_lcurly_token.id != TokenId::L_CURLY) {
    // Warning: empty loop
    _parse_warning(errors, lexer.get_previous_token(), WARN_EMPTY_BRANCH);
    return loop_stmnt;
  }

  lexer.get_back();
  AstBlock *content_block_node = parse_block(lexer);
  if (content_block_node == nullptr) {
    // TODO(pablo96): handle error in block parsing
    delete loop_stmnt;
    return nullptr;
  }

  content_block_node->parent = loop_stmnt;
  loop_stmnt->content_block = content_block_node;

  loop_stmnt->header_block = _create_block(loop_token, loop_stmnt);

  return loop_stmnt;
}

AstLoopStmnt *Parser::parse_rangeloop_stmnt(const Lexer &lexer, const Token &loop_token) noexcept {
  AstLoopStmnt *loop_stmnt = new AstLoopStmnt(loop_token.start_line, loop_token.start_column, loop_token.file_name);

  AstBinaryExpr *it_initializer = parse_assign_stmnt(lexer);
  if (it_initializer == nullptr) {
    delete loop_stmnt;
    return nullptr;
  }

  { // init block
    AstBlock *init_block = _create_block(loop_token, loop_stmnt);
    loop_stmnt->initializer_block = init_block;

    init_block->statements.push_back(it_initializer);
  }

  const Token &colon = lexer.get_next_token();
  if (colon.id != TokenId::COLON) {
    std::string value = std::string(lexer.get_token_value(colon));
    _parse_error(this->errors, colon, ERROR_EXPECTED_COLON_LOOP, value.c_str());
    delete loop_stmnt;
    return nullptr;
  }

  AstNode *end_value = parse_primary_expr(lexer);
  if (end_value == nullptr) {
    delete loop_stmnt;
    return nullptr;
  }

  AstNode *incr_expr = nullptr;
  const Token &semi_colon = lexer.get_next_token();
  if (semi_colon.id == TokenId::SEMI) {
    AstNode *_increment_amount = parse_primary_expr(lexer);
    AstSymbol *_left = new AstSymbol(*it_initializer->left_expr->symbol());
    incr_expr = _create_bin_expr(semi_colon, BinaryExprType::ADD, nullptr, _left, _increment_amount);
  } else {
    lexer.get_back();
  }

  const Token &true_lcurly_token = lexer.get_next_token();
  if (true_lcurly_token.id != TokenId::L_CURLY) {
    // Warning: empty loop
    _parse_warning(errors, lexer.get_previous_token(), WARN_EMPTY_BRANCH);
    return loop_stmnt;
  }

  lexer.get_back();
  AstBlock *content_block_node = parse_block(lexer);
  if (content_block_node == nullptr) {
    // TODO(pablo96): handle error in block parsing
    delete loop_stmnt;
    return nullptr;
  }
  loop_stmnt->content_block = content_block_node;
  content_block_node->parent = loop_stmnt;

  // conditional expr
  {
    AstSymbol *_left = new AstSymbol(*it_initializer->left_expr->symbol());
    loop_stmnt->condition_expr = _create_bin_expr(semi_colon, BinaryExprType::LESS, loop_stmnt, _left, end_value);
    loop_stmnt->is_condition_checked = true;
  }

  { // header
    loop_stmnt->header_block = _create_block(loop_token, loop_stmnt);
  }

  { // footer
    // var += 1
    AstBlock *_footer_block = loop_stmnt->footer_block = _create_block(semi_colon, loop_stmnt);

    if (incr_expr == nullptr) {
      AstSymbol *_symbol = new AstSymbol(*it_initializer->left_expr->symbol());
      AstUnaryExpr *_incr = new AstUnaryExpr(semi_colon.start_line, semi_colon.start_column, semi_colon.file_name);
      _incr->op = UnaryExprType::INC;
      _incr->expr = _symbol;
      _incr->expr->parent = _incr;
      incr_expr = _incr;
    }

    AstSymbol *_left = new AstSymbol(*it_initializer->left_expr->symbol());
    AstBinaryExpr *_assign = _create_bin_expr(semi_colon, BinaryExprType::ASSIGN, _footer_block, _left, incr_expr);
    _footer_block->statements.push_back(_assign);
  }

  return loop_stmnt;
}

AstLoopStmnt *Parser::parse_eachloop_stmnt(const Lexer &lexer, const Token &loop_token) noexcept { return nullptr; }

/*
 * Parses a (function | if-else stmnt) block
 * block
 *   : '{' (statement eos)* '}'
 *   ;
 */
AstBlock *Parser::parse_block(const Lexer &lexer) noexcept {
  const Token &l_curly_token = lexer.get_next_token();
  if (l_curly_token.id != TokenId::L_CURLY) {
    // bad_prediction
    LL_UNREACHEABLE;
  }

  AstBlock *block_node = new AstBlock(l_curly_token.start_line, l_curly_token.start_column, l_curly_token.file_name);

  for (;;) {
    const Token &token = lexer.get_next_token();

    if (token.id == TokenId::R_CURLY) {
      break;
    }

    if (token.id == TokenId::_EOF) {
      const Token &prev_token = lexer.get_previous_token();
      _parse_error(errors, prev_token, ERROR_UNEXPECTED_EOF_AFTER, lexer.get_token_value(prev_token));
      delete block_node;
      return nullptr;
    }

    lexer.get_back();
    AstNode *stmnt = parse_statement(lexer);
    if (!stmnt) {
      // TODO(pablo96): handle error in statement parsing
      delete block_node;
      return nullptr;
    }

    if (_end_of_statement(lexer, errors, TokenId::R_CURLY) == false) {
      delete stmnt;
      delete block_node;
      return nullptr;
    }

    stmnt->parent = block_node;
    block_node->statements.push_back(stmnt);
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
AstNode *Parser::parse_statement(const Lexer &lexer) noexcept {
  const Token &token = lexer.get_next_token();
  switch (token.id) {
  case TokenId::IDENTIFIER: {
    const Token &second_token = lexer.get_next_token();
    if (second_token.id == TokenId::ASSIGN) {
      if (lexer.get_next_token().id != TokenId::ASSIGN) {
        lexer.get_back();
        lexer.get_back(); // second_token
        lexer.get_back(); // token
        return parse_assign_stmnt(lexer);
      }
      // is '==' expression
      lexer.get_back();
      goto stmnt_expr;
    } else if (_is_type_start_token(second_token)) {
      lexer.get_back(); // second_token
      lexer.get_back(); // token
      return parse_vardef_stmnt(lexer);
    }
stmnt_expr:
    lexer.get_back(); // second_token
    lexer.get_back(); // token
    return parse_expr(lexer);
  }
  case TokenId::RET: {
    lexer.get_back();
    return parse_ret_stmnt(lexer);
  }
  case TokenId::IF: {
    lexer.get_back();
    return parse_branch_stmnt(lexer);
  }
  case TokenId::MUL: {
    // ptr dereference can only be an assignment of type
    // *ptr = expr
    lexer.get_back();
    return parse_assign_stmnt(lexer);
  }
  case TokenId::LOOP: {
    lexer.get_back();
    return parse_loop_stmnt(lexer);
  }
  case TokenId::CONTINUE:
  case TokenId::BREAK: {
    lexer.get_back();
    return parse_ctrl_stmnt(lexer);
  }
  case TokenId::L_CURLY: {
    // TODO(pablo96): parse local block
    return nullptr;
  }
  case TokenId::SEMI: {
    // empty_statement
    // consume the token and predict again
    return parse_statement(lexer);
  }
  case TokenId::_EOF:
    return nullptr;
  default:
    LL_UNREACHEABLE;
  }
}

/*
 * parses a variable definition/initialization
 * varDef
 *   : IDENTIFIER type_name '=' expression
 *   ;
 */
AstVarDef *Parser::parse_vardef_stmnt(const Lexer &lexer) noexcept {
  const Token &token_symbol_name = lexer.get_next_token();

  if (token_symbol_name.id != TokenId::IDENTIFIER) {
    // Bad prediction
    LL_UNREACHEABLE;
  }

  AstType *type_node = parse_type(lexer);

  if (!type_node) {
    // TODO(pablo96): handle error | wrong expression expected type name
    return nullptr;
  }

  AstVarDef *var_def_node =
    new AstVarDef(token_symbol_name.start_line, token_symbol_name.start_column, token_symbol_name.file_name);
  type_node->parent = var_def_node;
  var_def_node->name = lexer.get_token_value(token_symbol_name);
  var_def_node->type = type_node;

  const Token &assign_token = lexer.get_next_token();
  if (assign_token.id != TokenId::ASSIGN) {
    _parse_error(errors, token_symbol_name, ERROR_IMPLICIT_UNDEFINED_VAR);
    return nullptr;
  }

  auto expr = parse_expr(lexer);
  if (!expr) {
    // TODO(pablo96): handle error in unary_expr => sync parsing
    return nullptr;
  }

  expr->parent = var_def_node;
  var_def_node->initializer = expr;

  return var_def_node;
}

/*
 * type_name
 *   : '*' type_name
 *   | IDENTIFIER
 *   ;
 */
AstType *Parser::parse_type(const Lexer &lexer) noexcept {
  const Token &token = lexer.get_next_token();
  switch (token.id) {
  case TokenId::MUL: {
    // POINTER TYPE
    const TypeInfo *type_info = TypesRepository::get().get_type("pointer");
    auto type_node = new AstType(type_info, token.start_line, token.start_column, token.file_name);
    const Token &next_token = lexer.get_next_token();
    if (!_is_type_start_token(next_token)) {
      _parse_error(errors, next_token, ERROR_EXPECTED_TYPE_EXPR_INSTEAD_OF, lexer.get_token_value(next_token));
      lexer.get_back();
      delete type_node;
      return nullptr;
    }
    lexer.get_back();
    auto data_type_node = parse_type(lexer);
    data_type_node->parent = type_node;
    type_node->child_type = data_type_node;

    return type_node;
  }
  case TokenId::IDENTIFIER: {
    // TODO(pablo96): register new types
    return TypesRepository::get().get_type_node(lexer.get_token_value(token));
  }
  case TokenId::_EOF: {
    const Token &prev_token = lexer.get_previous_token();
    _parse_error(errors, prev_token, ERROR_UNEXPECTED_EOF_AFTER, lexer.get_token_value(prev_token));
    return nullptr;
  }
  default:
    // Bad prediction
    LL_UNREACHEABLE;
  }
}

/*
 * Parses an assignment
 *
 * assignmentStmnt
 *   : IDENTIFIER assign_op expression
 *   ;
 */
AstBinaryExpr *Parser::parse_assign_stmnt(const Lexer &lexer) noexcept {
  auto identifier_node = parse_unary_expr(lexer);
  if (!identifier_node) {
    // TODO(pablo96): error in unary_expr => sync parsing
    return nullptr;
  }

  const Token &token = lexer.get_next_token();
  if (token.id == TokenId::ASSIGN) {
    auto expr = parse_expr(lexer);
    if (!expr) {
      // TODO(pablo96): error in unary_expr => sync parsing
      return nullptr;
    }
    return _create_bin_expr(token, BinaryExprType::ASSIGN, nullptr, identifier_node, expr);
  }

  // Getting here means the prediction failed.
  LL_UNREACHEABLE;
}

/*
 * Parses a return statement
 * returnStmt
 *   : 'ret' expression?
 *   ;
 */
AstUnaryExpr *Parser::parse_ret_stmnt(const Lexer &lexer) noexcept {
  const Token &token = lexer.get_next_token();
  if (token.id != TokenId::RET) {
    // Prediction error
    LL_UNREACHEABLE;
  }

  AstUnaryExpr *node = new AstUnaryExpr(token.start_line, token.start_column, token.file_name);
  node->op = _get_unary_op(token);

  if (_is_expr_token(lexer.get_next_token())) {
    lexer.get_back();

    auto expr = parse_expr(lexer);
    if (!expr) {
      // TODO(pablo96): error in unary_expr => sync parsing
      return nullptr;
    }

    expr->parent = node;
    node->expr = expr;
  }
  // void return
  else {
    node->expr = nullptr;
    lexer.get_back();
  }

  return node;
}

/*
 * Parses a break or continue statement
 * ctrlStmnt
 *   : 'break' (IDENTIFIER | INTEGER)?
 *   | 'continue' (IDENTIFIER | INTEGER)?
 */
AstCtrlStmnt *Parser::parse_ctrl_stmnt(const Lexer &lexer) noexcept {
  const Token &keyword = lexer.get_next_token();
  if (!match(&keyword, TokenId::BREAK, TokenId::CONTINUE)) {
    LL_UNREACHEABLE;
  }

  AstCtrlStmnt *ctrl_stmnt = new AstCtrlStmnt(keyword.start_line, keyword.start_column, keyword.file_name);
  ctrl_stmnt->ctrl_type = keyword.id == TokenId::BREAK ? CtrlStmntType::BREAK : CtrlStmntType::CONTINUE;

  const Token &label_index = lexer.get_next_token();
  if (match(&label_index, TokenId::IDENTIFIER, TokenId::INT_LIT)) {
    if (label_index.id == TokenId::IDENTIFIER) {
      ctrl_stmnt->label = std::string(lexer.get_token_value(label_index)).c_str();
    } else {
      ctrl_stmnt->index = strtoul(label_index.int_lit.number, nullptr, (int)label_index.int_lit.base);
    }
  } else {
    lexer.get_back();
  }

  return ctrl_stmnt;
}

/*
 * Parses any supported expresion
 * expression
 *   : '(' expression ')'
 *   | compExpr
 *   ;
 */
AstNode *Parser::parse_expr(const Lexer &lexer) noexcept {
  if (lexer.get_next_token().id == TokenId::L_PAREN) {
    auto expression = parse_expr(lexer);
    if (lexer.get_next_token().id != TokenId::R_PAREN) {
      const Token &prev_token = lexer.get_previous_token();
      _parse_error(errors, prev_token, ERROR_EXPECTED_R_PAREN_AFTER, lexer.get_token_value(prev_token));
      return nullptr;
    }
    return expression;
  }
  // Was not a ')'
  lexer.get_back();
  return parse_comp_expr(lexer);
}

/*
 * Parses comparative expresions
 * compExpr
 *   : algebraicExpr ( '&&', '||', '==' | '!=' | '>='| '<=' | '<'| '>') algebraicExpr
 *   | algebraicExpr
 *   ;
 */
AstNode *Parser::parse_comp_expr(const Lexer &lexer) noexcept {
  AstNode *root_node = parse_algebraic_expr(lexer);
  if (!root_node) {
    // TODO(pablo96): error in primary expr => sync parsing
    return nullptr;
  }

  // check if operator is after a UNDEF(---) value
  if (_is_after_undef(lexer, this->errors, root_node, [](const Token &token) {
        return !MATCH(&token, TokenId::MUL, TokenId::DIV, TokenId::MOD, TokenId::LSHIFT, TokenId::RSHIFT,
                      TokenId::AMPERSAND, TokenId::BIT_XOR);
      })) {
    delete root_node;
    return nullptr;
  }

  while (true) {
    const Token &token = lexer.get_next_token();

    if (!MATCH(&token, TokenId::AND, TokenId::OR, TokenId::EQUALS, TokenId::NOT_EQUALS, TokenId::GREATER,
               TokenId::GREATER_OR_EQUALS, TokenId::LESS, TokenId::LESS_OR_EQUALS)) {
      // Not my token
      lexer.get_back();
      break;
    }

    AstNode *unary_expr = parse_algebraic_expr(lexer);
    if (!unary_expr) {
      // TODO(pablo96): error in unary_expr => sync parsing
      break;
    }

    if (_is_before_undef(lexer, this->errors, unary_expr)) {
      delete root_node;
      delete unary_expr;

      return nullptr;
    }

    // create binary node
    BinaryExprType bin_op = _get_binary_op(token);
    AstBinaryExpr *binary_expr = _create_bin_expr(token, bin_op, nullptr, root_node, unary_expr);

    // set the new node as root.
    root_node = binary_expr;
  }

  return root_node;
}

/*
 * Parses addition like expressions
 * algebraicExpr
 *   : termExpr ('+' | '-' | '|') termExpr
 *   | termExpr
 */
AstNode *Parser::parse_algebraic_expr(const Lexer &lexer) noexcept {
  AstNode *root_node = parse_term_expr(lexer);
  if (!root_node) {
    // TODO(pablo96): error in primary expr => sync parsing
    return nullptr;
  }

  // check if operator is after a UNDEF(---) value
  if (_is_after_undef(lexer, this->errors, root_node, [](const Token &token) {
        return !MATCH(&token, TokenId::PLUS, TokenId::MINUS, TokenId::BIT_OR);
      })) {
    delete root_node;
    return nullptr;
  }

  do {
    const Token &token = lexer.get_next_token();
    if (!MATCH(&token, TokenId::PLUS, TokenId::MINUS, TokenId::BIT_OR)) {
      // Not my token
      lexer.get_back();
      break;
    }

    AstNode *term_expr = parse_term_expr(lexer);
    if (!term_expr) {
      // TODO(pablo96): error in term_expr => sync parsing
      break;
    }

    if (_is_before_undef(lexer, this->errors, term_expr)) {
      delete root_node;
      delete term_expr;

      return nullptr;
    }

    // create binary node
    BinaryExprType bin_op = _get_binary_op(token);
    AstBinaryExpr *binary_expr = _create_bin_expr(token, bin_op, nullptr, root_node, term_expr);

    // set the new node as root.
    root_node = binary_expr;
  } while (true);

  return root_node;
}

/*
 * Parses multiplication like expressions
 * termExpr
 *   : unaryExpr ('*' | '/' | '%' | '<<' | '>>' | '&' | '^') unaryExpr
 *   | unaryExpr
 */
AstNode *Parser::parse_term_expr(const Lexer &lexer) noexcept {
  AstNode *root_node = parse_unary_expr(lexer);

  if (!root_node) {
    // TODO(pablo96): error in primary expr => sync parsing
    return nullptr;
  }

  // check if operator is after a UNDEF(---) value
  if (_is_after_undef(lexer, this->errors, root_node, [](const Token &token) {
        return !MATCH(&token, TokenId::MUL, TokenId::DIV, TokenId::MOD, TokenId::LSHIFT, TokenId::RSHIFT,
                      TokenId::AMPERSAND, TokenId::BIT_XOR);
      })) {
    delete root_node;
    return nullptr;
  }

  do {
    const Token *prev_token = lexer.get_current_pos() > 0 ? &lexer.get_previous_token() : nullptr;
    const Token &token = lexer.get_next_token();
    if (!MATCH(&token, TokenId::MUL, TokenId::DIV, TokenId::MOD, TokenId::LSHIFT, TokenId::RSHIFT, TokenId::AMPERSAND,
               TokenId::BIT_XOR)) {
      // Not my token
      lexer.get_back();
      break;
    }

    if (prev_token) {
      const Token &identifier = lexer.get_next_token();
      if (identifier.id == TokenId::IDENTIFIER) {
        const Token &assign_token = lexer.get_next_token();
        if (assign_token.id == TokenId::ASSIGN) {
          if (_is_new_line_between(lexer, prev_token->end_pos, token.start_pos)) {
            // is a new line with ptr dereference
            // example:
            // id = &id1
            // *id2 = ...
            lexer.get_back(); // = token
            lexer.get_back(); // identifier
            lexer.get_back(); // token
            break;
          }
        }
        lexer.get_back(); // = token
      }
      lexer.get_back(); // identifier
    }

    AstNode *symbol_node = parse_unary_expr(lexer);
    if (!symbol_node) {
      // TODO(pablo96): error in primary expr => sync parsing
      break;
    }

    if (_is_before_undef(lexer, this->errors, symbol_node)) {
      delete root_node;
      delete symbol_node;

      return nullptr;
    }

    // create binary node
    BinaryExprType bin_op = _get_binary_op(token);
    AstBinaryExpr *binary_expr = _create_bin_expr(token, bin_op, nullptr, root_node, symbol_node);

    // set the new node as root.
    root_node = binary_expr;
  } while (true);

  return root_node;
}

/*
 * Parses unary expresions
 * unaryExpr
 *   : ('!' | '~' | '--' | '++' | '-' | '&') primaryExpr
 *   | primaryExpr ('--' | '++')
 *   | primaryExpr
 *   ;
 */
AstNode *Parser::parse_unary_expr(const Lexer &lexer) noexcept {
consume_plus:
  const Token &unary_op_token = lexer.get_next_token();
  if (unary_op_token.id == TokenId::PLUS) {
    // ignore plus token
    goto consume_plus;
  }

  if (unary_op_token.id == TokenId::_EOF) {
    const Token &prev_token = lexer.get_previous_token();
    _parse_error(errors, prev_token, ERROR_UNEXPECTED_EOF_AFTER, lexer.get_token_value(prev_token));
    return nullptr;
  }

  // TODO(pablo96): Add an assign in the inc/dec operations

  // op primary_expr
  if (MATCH(&unary_op_token, TokenId::NOT, TokenId::BIT_NOT, TokenId::PLUS_PLUS, TokenId::MINUS_MINUS, TokenId::MINUS,
            TokenId::AMPERSAND, TokenId::MUL)) {
    {
      const Token &next_token = lexer.get_next_token();
      if (next_token.id == TokenId::UNDEF) {
        lexer.get_back(); // next_token
        lexer.get_back(); // unary_op_token

        _parse_error(errors, unary_op_token, ERROR_UNEXPECTED_UNDEF_AFTER, lexer.get_token_value(unary_op_token));
        return nullptr;
      }

      if (unary_op_token.id == TokenId::MINUS && next_token.id != TokenId::IDENTIFIER) {
        lexer.get_back(); // next_token
        lexer.get_back(); // unary_op_token
        return parse_primary_expr(lexer);
      }
      lexer.get_back(); // next_token
    }

    AstNode *primary_expr = parse_primary_expr(lexer);
    if (!primary_expr) {
      // TODO(pablo96): error in algebraic_expr => sync parsing
      // if this happens to be
      // ++ERROR_TOKEN
      // then we need to see if we can start over from the next token
      // for that we need to go back a step up into algebraic
      return nullptr;
    }

    AstUnaryExpr *node =
      new AstUnaryExpr(unary_op_token.start_line, unary_op_token.start_column, unary_op_token.file_name);
    primary_expr->parent = node;
    node->op = _get_unary_op(unary_op_token);
    node->expr = primary_expr;
    return node;
  }

  lexer.get_back();
  AstNode *primary_expr = parse_primary_expr(lexer);
  if (primary_expr == nullptr) {
    // TODO(pablo96): error in algebraic_expr => sync parsing
    return nullptr;
  }

  const Token &token = lexer.get_next_token();
  // primary_expr op
  if (MATCH(&token, TokenId::PLUS_PLUS, TokenId::MINUS_MINUS)) {
    if (_is_before_undef(lexer, this->errors, primary_expr)) {
      delete primary_expr;
      return nullptr;
    }

    AstUnaryExpr *node = new AstUnaryExpr(token.start_line, token.start_column, token.file_name);
    primary_expr->parent = node;
    node->expr = primary_expr;
    node->op = _get_unary_op(token);
    return node;
  }
  lexer.get_back();

  return primary_expr;
}

bool _is_after_undef(const Lexer &lexer, std::vector<Error> &in_errors, AstNode *in_node,
                     bool (*in_predicate)(const Token &)) noexcept {
  const Token &token = lexer.get_next_token();
  if (in_predicate(token)) {
    // Not my token
    lexer.get_back();
    return false;
  }

  if (in_node->node_type == AstNodeType::AST_CONST_VALUE && in_node->const_value()->type == ConstValueType::UNDEF) {
    _parse_error(in_errors, token, ERROR_FORBIDEN_OP_AFTER_UNDEF, lexer.get_token_value(token));

    lexer.get_back();
    return true;
  }

  lexer.get_back();
  return false;
}

bool _is_before_undef(const Lexer &lexer, std::vector<Error> &in_errors, AstNode *in_node) noexcept {
  const Token &token = lexer.get_previous_token();
  if (in_node->node_type == AstNodeType::AST_CONST_VALUE && in_node->const_value()->type == ConstValueType::UNDEF) {
    _parse_error(in_errors, token, ERROR_UNEXPECTED_AFTER_UNDEF, lexer.get_token_value(token));
    return true;
  }
  return false;
}

/*

 * Parses primary expressions
 * primary_expr
 *   : expression
 *   | call_expr
 *   | arrayConstExpr
 *   | IDENTIFIER
 *   | FLOAT_LIT
 *   | INT_LIT
 *   | UNICODE_CHAR
 */
AstNode *Parser::parse_primary_expr(const Lexer &lexer) noexcept {
  const Token &token = lexer.get_next_token();

  if (token.id == TokenId::_EOF) {
    const Token &prev_token = lexer.get_previous_token();
    _parse_error(errors, prev_token, ERROR_UNEXPECTED_EOF_AFTER, lexer.get_token_value(prev_token));
    return nullptr;
  }

  if (token.id == TokenId::L_PAREN) {
    // group expression
    lexer.get_back();
    return parse_expr(lexer);
  }

  if (token.id == TokenId::L_BRACKET) {
    lexer.get_back();
    return parse_const_array(lexer);
  }

  if (token.id == TokenId::IDENTIFIER) {
    // CALL EXPR?
    const Token &next_token = lexer.get_next_token();
    if (next_token.id == TokenId::L_PAREN) {
      lexer.get_back(); // LPAREN
      lexer.get_back(); // IDENTIFIER
      return parse_function_call(lexer);
    }
    // else
    lexer.get_back();

    AstSymbol *symbol_node = new AstSymbol(token.start_line, token.start_column, token.file_name);
    symbol_node->token = &token;
    symbol_node->cached_name = lexer.get_token_value(token);
    return symbol_node;
  }

  bool is_negative = token.id == TokenId::MINUS;
  const Token &number_token = is_negative ? lexer.get_next_token() : token;

  if (MATCH(&number_token, TokenId::FLOAT_LIT, TokenId::INT_LIT, TokenId::UNICODE_CHAR, TokenId::TRUE, TokenId::FALSE,
            TokenId::NIL, TokenId::UNDEF, TokenId::L_BRACKET)) {
    return parse_const_expr(lexer, token, number_token);
  }

  auto token_value = lexer.get_token_value(number_token);
  _parse_error(errors, number_token, ERROR_EXPECTED_NUMBER_IDENTIFIER_CHAR_TOKEN, token_value,
               token_id_name(number_token.id));
  return nullptr;
}

AstNode *Parser::parse_const_expr(const Lexer &lexer, const Token &token, const Token &number_token) noexcept {
  AstConstValue *const_value_node = new AstConstValue(token.start_line, token.start_column, token.file_name);
  switch (number_token.id) {
  case TokenId::INT_LIT: {
    const_value_node->type = ConstValueType::INT;
    auto number = const_value_node->number = number_token.int_lit.number;
    const_value_node->is_negative = number_token.int_lit.is_negative;
  } break;
  case TokenId::FLOAT_LIT: {
    const_value_node->type = ConstValueType::FLOAT;
    const_value_node->number = number_token.float_lit.number;
  } break;
  case TokenId::UNICODE_CHAR: {
    const_value_node->type = ConstValueType::CHAR;
    const_value_node->unicode_char = number_token.char_lit;
  } break;
  case TokenId::FALSE:
  case TokenId::TRUE: {
    const_value_node->type = ConstValueType::BOOL;
    const_value_node->boolean = number_token.id == TokenId::TRUE;
  } break;
  case TokenId::UNDEF: {
    const_value_node->type = ConstValueType::UNDEF;
  } break;
  case TokenId::NIL: {
    /* NOTE:
     * If it is another type of value for a pointer,
     * the semantic analizer will resolve it
     */
    const_value_node->type = ConstValueType::PTR;
    const_value_node->number = "0";
    const_value_node->bit_size = 64;
  } break;
  default:
    LL_UNREACHEABLE;
  }

  return const_value_node;
}

/*
 * functionCallExpr
 *   : IDENTIFIER '(' (expression (, expression)*)? ')'
 *   ;
 */
AstFnCallExpr *Parser::parse_function_call(const Lexer &lexer) noexcept {
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

  AstFnCallExpr *func_call_node =
    new AstFnCallExpr(name_token.start_line, name_token.start_column, name_token.file_name);
  func_call_node->fn_name = lexer.get_token_value(name_token);

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
      _parse_error(errors, prev_token, ERROR_UNEXPECTED_EOF_AFTER, lexer.get_token_value(prev_token));
      delete func_call_node;
      return nullptr;
    }

    lexer.get_back();
    auto expr = parse_expr(lexer);
    if (!expr) {
      // TODO(pablo96): handle error | find a comma or rparen
      continue;
    }
    expr->parent = func_call_node;
    func_call_node->args.push_back(expr);
  }

  return func_call_node;
}

/*
 * arrayConstExpr
 *   : '[' INT_LIT? ']' '{' (expression (',' expression)*) '}'
 *   | '[' INT_LIT ']' '{' '---'? '}'
 *   ;
 */
AstConstArray *Parser::parse_const_array(const Lexer &lexer) noexcept {
  const Token &l_bracket = lexer.get_next_token();
  LL_ASSERT(l_bracket.id == TokenId::L_BRACKET);

  const Token &int_lit = lexer.get_next_token();
  const bool require_elements = int_lit.id != TokenId::INT_LIT;

  u64 *expected_size = nullptr;
  if (int_lit.id == TokenId::INT_LIT) {
    if (int_lit.int_lit.is_negative) {
      _parse_error(this->errors, int_lit, ERROR_EXPECTED_POSITIVE_ARRAY_SIZE);
      return nullptr;
    }
    std::string number_str = int_lit.int_lit.number;
    u64 number = strtoull(number_str.c_str(), nullptr, (s32)int_lit.int_lit.base);
    expected_size = new u64(number);
  } else {
    lexer.get_back();
  }

  // ']'
  {
    const Token &r_bracket = lexer.get_next_token();
    if (r_bracket.id != TokenId::R_BRACKET) {
      _parse_error(this->errors, r_bracket, ERROR_EXPECTED_CLOSING_BRAKET_BEFORE, lexer.get_token_value(r_bracket));
      return nullptr;
    }
  }

  // '{'
  {
    const Token &l_curly = lexer.get_next_token();
    if (l_curly.id != TokenId::L_CURLY) {
      _parse_error(this->errors, l_curly, ERROR_EXPECTED_OPENING_CURLY_AFTER);
      return nullptr;
    }
  }

  std::vector<AstNode *> array_elements;
  if (expected_size != nullptr) {
    array_elements.reserve(*expected_size);
  }

  for (u64 element_index = 0; true; ++element_index) {
    const Token &element_token = lexer.get_next_token();

    if (element_token.id == TokenId::R_CURLY) {
      if (require_elements && element_index == 0) {
        _parse_error(this->errors, element_token, ERROR_EXPECTED_ELEMENTS_IN_CONST_ARRAY);
        return nullptr;
      }
      lexer.get_back();
      break;
    } else {
      if (element_index != 0) {
        const AstNode *elem = array_elements[element_index - 1];
        if (elem->node_type == AstNodeType::AST_CONST_VALUE) {
          const AstConstValue *const_value = elem->const_value();
          if (const_value->type == ConstValueType::UNDEF) {
            _parse_error(this->errors, element_token, ERROR_EXPECTED_NO_ELEMENTS_AFTER_UNDEF);
            return nullptr;
          }
        }
      }
      lexer.get_back();
    }

    AstNode *element_node = parse_expr(lexer);
    if (element_node == nullptr) {
      continue;
    }

    array_elements.push_back(element_node);

    const Token &comma = lexer.get_next_token();
    if (comma.id != TokenId::COMMA) {
      lexer.get_back();
      break;
    }
  }

  // '}'
  {
    const Token &r_curly = lexer.get_next_token();
    if (r_curly.id != TokenId::R_CURLY) {
      const Token &type_name = lexer.get_previous_token();
      _parse_error(this->errors, r_curly, ERROR_EXPECTED_OPENING_CURLY_AFTER, lexer.get_token_value(type_name));
      return nullptr;
    }
  }

  if (expected_size != nullptr && *expected_size != array_elements.size()) {
    bool array_has_elements = array_elements.size() != 0;
    if (array_has_elements) {
      const AstConstValue *first_elem = array_elements.at(0)->const_value();
      bool is_const_val = first_elem->node_type == AstNodeType::AST_CONST_VALUE;
      bool is_uninit_array = is_const_val && first_elem->type == ConstValueType::UNDEF;
      if (!is_uninit_array) {
        _parse_error(this->errors, int_lit, ERROR_EXPECTED_ARRAY_SIZE_BUT_GOT, *expected_size, array_elements.size());
        return nullptr;
      }
    }
  }

  AstConstArray *const_array = new AstConstArray(l_bracket.start_line, l_bracket.start_column, l_bracket.file_name);
  const_array->elements = array_elements;
  const_array->elem_count = nullptr == expected_size ? array_elements.size() : *expected_size;

  return const_array;
}

static void _parse_msg(std::vector<Error> &in_errors, ERROR_TYPE in_msg_type, const Token &in_token,
                       const char *in_format, va_list ap) noexcept {
  va_list ap2;
  va_copy(ap2, ap);

  s32 len1 = snprintf(nullptr, 0, in_format, ap);
  assert(len1 >= 0);

  std::string msg;
  msg.reserve(len1 + 1);
  msg.resize(len1);

  s32 len2 = snprintf(msg.data(), msg.capacity(), in_format, ap2);
  assert(len2 >= 0);
  assert(len2 == len1);

  Error error(in_msg_type, in_token.start_line, in_token.start_column, in_token.file_name, msg);
  in_errors.push_back(error);

  va_end(ap2);
}

void _parse_error(std::vector<Error> &in_errors, const Token &in_token, const char *in_format, ...) noexcept {
  va_list args;
  va_start(args, in_format);

  _parse_msg(in_errors, ERROR_TYPE::ERROR, in_token, in_format, args);

  va_end(args);
}

void _parse_warning(std::vector<Error> &in_errors, const Token &in_token, const char *in_format, ...) noexcept {
  va_list args;
  va_start(args, in_format);

  _parse_msg(in_errors, ERROR_TYPE::WARNING, in_token, in_format, args);

  va_end(args);
}

bool _is_new_line_between(const Lexer &lexer, const size_t start_pos, const size_t end_pos) {
  auto start_it = lexer.source.data();
  auto len = end_pos - start_pos;
  auto str_view = std::string_view(start_it + start_pos, len);

  return str_view.find_first_of('\n') != str_view.npos;
}

bool _is_forbiden_statement(const Token &token, const Token &next_token) noexcept {
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
    if (token.id == TokenId::MUL) {
      if (match(&next_token, TokenId::IDENTIFIER, TokenId::MUL, TokenId::L_BRACKET)) {
        return false;
      }
    }
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

BinaryExprType _get_binary_op(const Token &token) noexcept {
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
  case TokenId::AMPERSAND:
    return BinaryExprType::BIT_AND;
  case TokenId::ASSIGN:
    return BinaryExprType::ASSIGN;
  case TokenId::AND:
    return BinaryExprType::AND;
  case TokenId::OR:
    return BinaryExprType::OR;
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

UnaryExprType _get_unary_op(const Token &token) noexcept {
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
  case TokenId::NOT:
    return UnaryExprType::NOT;
  case TokenId::AMPERSAND:
    return UnaryExprType::ADDRESS_OF;
  case TokenId::MUL:
    return UnaryExprType::DEREFERENCE;
  default:
    LL_UNREACHEABLE;
  }
}

bool _is_type_start_token(const Token &token) noexcept {
  switch (token.id) {
  case TokenId::IDENTIFIER:
  case TokenId::L_BRACKET:
  case TokenId::MUL:
    return true;
  default:
    return false;
  }
}

bool _is_expr_token(const Token &token) noexcept {
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
  case TokenId::AMPERSAND:
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

bool _is_symbol_start_char(const s8 next_char) noexcept {
  return (next_char >= 'a' && next_char <= 'z') || (next_char >= 'A' && next_char <= 'Z') || next_char == '_';
}

bool _is_whitespace_char(const s8 _char) noexcept {
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

bool _get_directive_type(DirectiveType &dir_type, std::string_view in_directive_name) {
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

const char *_get_const_char(const std::string_view str) {
  const auto src_size = str.size();
  const auto size = src_size + 1;
  char *str_nt = new char[size];
  strncpy(str_nt, str.data(), src_size);
  str_nt[src_size] = 0;
  return str_nt;
}

bool _get_file_name_and_path(const FileInput &file_input, std::vector<Error> &in_errors, const Token &in_string_token,
                             const std::string_view in_relative_file_name, const std::string &in_file_directory,
                             std::string &out_file_path, std::string &out_file_name) {
  // file_name without double quotes
  std::string_view relative_file_name = in_relative_file_name.substr(1, in_relative_file_name.size() - 2);

  std::error_code error_code;

  // complete relative file name path
  std::string relative_file_name_path =
    in_file_directory + std::filesystem::path::preferred_separator + std::string(relative_file_name);
  std::filesystem::path file_path = resolve_path(relative_file_name_path, error_code);

  switch (file_input.verify_file_path(file_path)) {
  case FILE_PATH_STATUS::NOT_A_FILE: {
    _parse_error(in_errors, in_string_token, ERROR_NOT_A_FILE, file_path.string().c_str());
    return false;
  }
  case FILE_PATH_STATUS::NOT_FOUND: {
    _parse_error(in_errors, in_string_token, ERROR_FILE_NOT_FOUND, file_path.string().c_str());
    return false;
  }
  case FILE_PATH_STATUS::NOT_LLAMA_FILE: {
    _parse_error(in_errors, in_string_token, ERROR_NOT_LLAMA_FILE, file_path.string().c_str());
    return false;
  }
  default:
    break;
  }

  out_file_name = file_path.filename();
  out_file_path = file_path.parent_path();

  return true;
}

bool _is_boolean_expression(const AstNode *in_expr, AstNode *in_stmnt_node) {
  LL_ASSERT(in_stmnt_node->node_type == AstNodeType::AST_IF_STMNT ||
            in_stmnt_node->node_type == AstNodeType::AST_LOOP_STMNT);
  bool &is_condition_checked = in_stmnt_node->node_type == AstNodeType::AST_IF_STMNT
                                 ? in_stmnt_node->if_stmnt()->is_condition_checked
                                 : in_stmnt_node->loop_stmnt()->is_condition_checked;
  is_condition_checked = true;

  if (in_expr->node_type == AstNodeType::AST_UNARY_EXPR) {
    switch (in_expr->unary_expr()->op) {
    case UnaryExprType::NOT:
      return true;
    default:
      return false;
    }
  } else if (in_expr->node_type == AstNodeType::AST_BINARY_EXPR) {
    switch (in_expr->binary_expr()->bin_op) {
    case BinaryExprType::GREATER:
    case BinaryExprType::GREATER_OR_EQUALS:
    case BinaryExprType::LESS:
    case BinaryExprType::LESS_OR_EQUALS:
    case BinaryExprType::EQUALS:
    case BinaryExprType::NOT_EQUALS:
      return true;
    default:
      return false;
    }
  } else if (in_expr->node_type == AstNodeType::AST_CONST_VALUE) {
    return in_expr->const_value()->type == ConstValueType::BOOL;
  } else if (in_expr->node_type == AstNodeType::AST_SYMBOL) {
    // if it is a symbol is up to the semantic analyzer to know its type.
    is_condition_checked = false;
    return true;
  }
  return false;
}

AstBlock *_create_block(const Token &token, AstNode *parent) {
  AstBlock *_block = new AstBlock(token.start_line, token.start_column, token.file_name);
  _block->parent = parent;
  return _block;
}

AstBinaryExpr *_create_bin_expr(const Token &token, const BinaryExprType bin_op, AstNode *parent, AstNode *left,
                                AstNode *right) {
  AstBinaryExpr *_bin_expr = new AstBinaryExpr(token.start_line, token.start_column, token.file_name);
  _bin_expr->bin_op = bin_op;
  _bin_expr->left_expr = left;
  _bin_expr->right_expr = right;
  _bin_expr->left_expr->parent = _bin_expr;
  _bin_expr->right_expr->parent = _bin_expr;
  _bin_expr->parent = parent;

  return _bin_expr;
}
