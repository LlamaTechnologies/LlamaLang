#include "semantic_analyzer.hpp"

#include "Types.hpp"
#include "ast_nodes.hpp"
#include "common_defs.hpp"
#include "semantic_error_msgs.hpp"

#include <stdarg.h>
#include <stdlib.h>

//----- PRE DECLARATIONS - UTIL FUNCTIONS -----

inline static bool _is_ret_statement(const AstNode *in_node) {
  return in_node->node_type == AstNodeType::AST_UNARY_EXPR && in_node->unary_expr()->op == UnaryExprType::RET;
}

inline static bool _is_type_array_or_pointer(const AstType *in_type) {
  return in_type->type_info->type_id == AstTypeId::ARRAY || in_type->type_info->type_id == AstTypeId::POINTER;
}

inline bool _check_integer_types(std::vector<Error> &errors, const AstNode *in_expr_node, const AstType *in_type_0,
                                 const AstType *in_type_1);
inline bool _check_floating_point_types(std::vector<Error> &errors, const AstNode *in_expr_node,
                                        const AstType *in_type_0, const AstType *in_type_1);
inline static const AstType *_get_binary_expr_type(std::vector<Error> &errors, const Table *symbol_table,
                                                   const AstBinaryExpr *in_bin_expr);
inline static const AstType *_get_unary_expr_type(std::vector<Error> &errors, const Table *symbol_table,
                                                  const AstUnaryExpr *in_unary_expr);
inline static const AstType *_get_const_value_type(std::vector<Error> &errors, const Table *symbol_table,
                                                   const AstConstValue *in_const_value);
inline static const AstType *_get_fn_call_expr_type(std::vector<Error> &errors, const Table *symbol_table,
                                                    const AstFnCallExpr *in_fn_call_expr);
inline static const AstType *_get_symbol_type(std::vector<Error> &errors, const Table *symbol_table,
                                              const AstSymbol *in_symbol);
inline static bool _analize_param(std::vector<Error> &errors, Table *symbol_table, const AstParamDef *in_node);
inline static void _set_type_info(const AstNode *expr_node, const AstType *type_node);

//----- START PRIMARY CHECK FUNCTIONS -----

bool SemanticAnalyzer::analize_var_def(const AstVarDef *in_var_def) {
  LL_ASSERT(in_var_def != nullptr);
  LL_ASSERT((in_var_def->node_type == AstNodeType::AST_VAR_DEF));

  auto var_name = std::string(in_var_def->name);

  // local variable declaration
  symbol_table->add_symbol(var_name, SymbolType::VAR, in_var_def);

  if (in_var_def->initializer == nullptr) {
    return true;
  }

  // variable declaration and definition
  bool is_valid_init = analize_expr(in_var_def->initializer);
  if (!is_valid_init)
    return false;

  const AstType *expr_type = get_expr_type(errors, symbol_table, in_var_def->initializer);
  if (in_var_def->type->type_info->type_id != expr_type->type_info->type_id) {
    add_semantic_error(errors, in_var_def, ERROR_TYPES_MISMATCH, var_name.c_str());
    symbol_table->remove_last_symbol();
    return false;
  }

  _set_type_info(in_var_def->initializer, in_var_def->type);
  return true;
}

bool SemanticAnalyzer::analize_global_var_def(const AstVarDef *in_var_def) {
  LL_ASSERT(in_var_def != nullptr);
  LL_ASSERT((in_var_def->node_type == AstNodeType::AST_VAR_DEF));

  auto var_name = std::string(in_var_def->name);

  if (!in_var_def->initializer) {
    add_semantic_error(errors, in_var_def, ERROR_GLOBAL_NEED_INITIALIZER, var_name.c_str());
    return false;
  }

  if (in_var_def->initializer->node_type != AstNodeType::AST_CONST_VALUE) {
    add_semantic_error(errors, in_var_def, ERROR_GLOBAL_INITIALIZER_NO_CONST, var_name.c_str());
    return false;
  }

  global_symbol_table->add_symbol(var_name, SymbolType::VAR, in_var_def);

  const AstType *expr_type = get_expr_type(errors, symbol_table, in_var_def->initializer);
  if (in_var_def->type->type_info->type_id != expr_type->type_info->type_id) {
    add_semantic_error(errors, in_var_def, ERROR_TYPES_MISMATCH, var_name.c_str());
    global_symbol_table->remove_last_symbol();
    return false;
  }

  _set_type_info(in_var_def->initializer, in_var_def->type);
  return true;
}

bool SemanticAnalyzer::analize_fn_proto(const AstFnProto *in_proto) {
  LL_ASSERT(in_proto != nullptr);
  LL_ASSERT(in_proto->node_type == AstNodeType::AST_FN_PROTO);

  auto ret_type = in_proto->return_type;

  // we dont support structs yet
  if (ret_type->type_info->type_id == AstTypeId::STRUCT) {
    add_semantic_error(errors, ret_type, ERROR_STRUCTS_UNSUPORTED);
    return false;
  }

  auto str_name = std::string(in_proto->name);

  // add function to the parent's symbol table
  symbol_table->add_symbol(str_name, SymbolType::FUNC, in_proto);

  // create the function's symbol table
  symbol_table = symbol_table->create_child(std::string(in_proto->name), in_proto);

  bool has_no_error = true;
  for (AstParamDef *param : in_proto->params) {
    bool param_ok = _analize_param(errors, symbol_table, param);
    if (param_ok) {
      auto param_name = std::string(param->name);
      symbol_table->add_symbol(param_name, SymbolType::VAR, param);
    }
    has_no_error = has_no_error && param_ok;
  }

  LL_ASSERT(symbol_table->parent != nullptr);
  // set symbol table to previous table
  symbol_table = symbol_table->parent;
  return has_no_error;
}

bool SemanticAnalyzer::analize_fn_block(const AstBlock *in_fn_block) {
  LL_ASSERT(in_fn_block != nullptr);
  return analize_block(in_fn_block, true);
}

void SemanticAnalyzer::enter_fn_scope(AstFnDef *in_function) {
  LL_ASSERT(in_function != nullptr);

  // set function's symbol table as current table
  symbol_table = symbol_table->get_child(std::string(in_function->proto->name));
}

void SemanticAnalyzer::exit_fn_scope() {
  // if the function has no parent is a bug in the compiler
  // at least global scope should be the parent of the function
  LL_ASSERT(symbol_table->parent != nullptr);
  symbol_table = symbol_table->parent;
}

//----- END PRIMARY CHECK FUNCTIONS -------

bool SemanticAnalyzer::analize_block(const AstBlock *in_block, bool is_first_level_block) {
  size_t errors_before = errors.size();

  const AstFnProto *fn_proto = symbol_table->fn_proto;
  // We dont allow blocks outside a function for example:
  // we dont allow namespaces yet
  LL_ASSERT(fn_proto != nullptr);

  const AstType *ret_type = fn_proto->return_type;
  bool requires_ret_stmnt = is_first_level_block && ret_type->type_info->type_id != AstTypeId::VOID;
  bool has_ret_stmnt = false;

  // check every statement in the function
  for (const AstNode *stmnt : in_block->statements) {
    if (_is_ret_statement(stmnt)) {
      has_ret_stmnt = true;

      if (!analize_expr(stmnt))
        continue;

      // if ret stmnt is ok
      // then check if return type is the same as the one in the prototype
      check_and_set_type(stmnt, ret_type, stmnt->unary_expr()->expr);
    } else if (stmnt->node_type == AstNodeType::AST_VAR_DEF) {
      analize_var_def(stmnt->var_def());
    } else if (stmnt->node_type == AstNodeType::AST_IF_STMNT) {
      analize_if_stmnt(stmnt);
    } else {
      analize_expr(stmnt);
    }
  }

  // if the function should return a type and it doesnt then report it;
  if (requires_ret_stmnt && !has_ret_stmnt) {
    add_semantic_error(errors, fn_proto, ERROR_REQUIRE_RET_STMNT);
  }

  size_t errors_after = errors.size();
  return errors_before == errors_after;
}

bool SemanticAnalyzer::analize_if_stmnt(const AstNode *in_stmnt) {
  LL_ASSERT(in_stmnt->node_type == AstNodeType::AST_IF_STMNT);
  const AstIfStmnt *if_stmnt = in_stmnt->if_stmnt();

  if (!if_stmnt->is_condition_checked) {
    const AstNode *conditional_expr = if_stmnt->condition_expr;
    const AstType *expr_type = get_expr_type(this->errors, this->symbol_table, conditional_expr);
    if (expr_type->type_info->type_id != AstTypeId::BOOL) {
      add_semantic_error(errors, conditional_expr, ERROR_EXPECTED_BOOL_EXPR,
                         get_type_id_name(expr_type->type_info->type_id));
      return false;
    }
  }

  const AstBlock *if_block = if_stmnt->if_block;
  if (!analize_block(if_block)) {
    // errors have been set inside analize_block so we just return false
    return false;
  }

  // check else block
  const AstBlock *else_block = if_stmnt->else_block;
  if (else_block != nullptr) {
    if (!analize_block(else_block)) {
      // errors have been set inside analize_block so we just return false
      return false;
    }
  }

  return true;
}

bool SemanticAnalyzer::analize_expr(const AstNode *in_expr) {
  switch (in_expr->node_type) {
  case AstNodeType::AST_BINARY_EXPR: {
    const AstBinaryExpr &bin_expr = *in_expr->binary_expr();
    switch (bin_expr.bin_op) {
    // BITWISE OPERATORS
    case BinaryExprType::LSHIFT:
    case BinaryExprType::RSHIFT: {
      if (!analize_expr(bin_expr.left_expr) || !analize_expr(bin_expr.right_expr))
        return false;

      // right value should be an integer
      auto r_expr_type_node = get_expr_type(errors, symbol_table, bin_expr.right_expr);
      if (r_expr_type_node->type_info->type_id != AstTypeId::INTEGER) {
        add_semantic_error(errors, in_expr, ERROR_BIT_SHIFT_LEFT_EXPR_NO_INT);
        return false;
      }

      // now we depend on left expr beeing ok
      return get_expr_type(errors, symbol_table, bin_expr.left_expr) != nullptr;
    }
    // BOOLEAN OPERATORS
    case BinaryExprType::EQUALS:
    case BinaryExprType::NOT_EQUALS:
    case BinaryExprType::GREATER_OR_EQUALS:
    case BinaryExprType::LESS_OR_EQUALS:
    case BinaryExprType::GREATER:
    case BinaryExprType::LESS:
      // We just check their expressions are good
      return analize_expr(bin_expr.left_expr) && analize_expr(bin_expr.right_expr);
    // BINARY OPERATORS
    case BinaryExprType::ADD:
    case BinaryExprType::SUB:
    case BinaryExprType::MUL:
    case BinaryExprType::DIV:
    case BinaryExprType::MOD:
    case BinaryExprType::ASSIGN: {
      if (!analize_expr(bin_expr.left_expr) || !analize_expr(bin_expr.right_expr))
        return false;

      auto l_expr_type_node = get_expr_type(errors, symbol_table, bin_expr.left_expr);
      auto r_expr_type_node = get_expr_type(errors, symbol_table, bin_expr.right_expr);

      // are types compatible?
      if (l_expr_type_node->type_info->type_id != r_expr_type_node->type_info->type_id) {
        add_semantic_error(errors, in_expr, ERROR_TYPES_MISMATCH);
        return false;
      }

      bool l_is_const = bin_expr.left_expr->node_type == AstNodeType::AST_CONST_VALUE;
      bool r_is_const = bin_expr.right_expr->node_type == AstNodeType::AST_CONST_VALUE;

      // if one of them is a constant then
      // then the constant is the same type as the symbol
      if (l_is_const != r_is_const) {
        if (bin_expr.left_expr->node_type == AstNodeType::AST_CONST_VALUE) {
          const AstNode *l_const_value = bin_expr.left_expr;
          _set_type_info(l_const_value, r_expr_type_node);
        } else if (bin_expr.right_expr->node_type == AstNodeType::AST_CONST_VALUE) {
          const AstNode *r_const_value = bin_expr.right_expr;
          _set_type_info(r_const_value, l_expr_type_node);
        }
      } else // if both are NOT a constant check their types
        if (!check_types(errors, l_expr_type_node, r_expr_type_node, in_expr)) {
        // not the same type
        return false;
      }

      // they are the same type or they are both constants and type compatible
      return true;
    }
    default:
      LL_UNREACHEABLE;
    }
  }
  case AstNodeType::AST_UNARY_EXPR: {
    const AstUnaryExpr &unary_expr = *in_expr->unary_expr();
    auto type_node = get_expr_type(errors, symbol_table, unary_expr.expr);

    if (!type_node) {
      return false;
    }

    // Overloading operators is not supported
    if (type_node->type_info->type_id == AstTypeId::STRUCT) {
      auto operator_symbol = get_unary_op_symbol(unary_expr.op);
      add_semantic_error(errors, in_expr, ERROR_UNSUPORTED_UNARY_OP_STRUCT_EXPR, operator_symbol.c_str());
      return false;
    }
    // If it is a boolean expr only the NOT operator is valid
    if (type_node->type_info->type_id == AstTypeId::BOOL && unary_expr.op != UnaryExprType::NOT) {
      auto operator_symbol = get_unary_op_symbol(unary_expr.op);
      add_semantic_error(errors, in_expr, ERROR_UNSUPORTED_OP_BOOL_EXPR, operator_symbol.c_str());
      return false;
    }
    // If it is not boolean expr the NOT operator is invalid
    if (type_node->type_info->type_id != AstTypeId::BOOL && unary_expr.op == UnaryExprType::NOT) {
      add_semantic_error(errors, in_expr, ERROR_UNSUPORTED_OP_NOT_BOOL_EXPR);
      return false;
    }

    return analize_expr(unary_expr.expr);
  }
  case AstNodeType::AST_FN_CALL_EXPR: {
    const AstFnCallExpr &fn_call = *in_expr->fn_call();

    SymbolType symbol_type;
    fn_call.fn_ref =
      resolve_function_variable(errors, symbol_table, std::string(fn_call.fn_name), in_expr, &symbol_type)->fn_proto();

    if (symbol_type != SymbolType::FUNC) {
      add_semantic_error(errors, in_expr, ERROR_SYMBOL_NOT_A_FN, fn_call.fn_name);
      return false;
    }

    LL_ASSERT(fn_call.fn_ref->node_type == AstNodeType::AST_FN_PROTO);

    if (!fn_call.fn_ref) {
      add_semantic_error(errors, in_expr, ERROR_UNDECLARED_FN, fn_call.fn_name);
      return false;
    }

    // TODO(pablo96): temporal solution to printf!
    if (fn_call.fn_name == "printf") {
      for (auto arg : fn_call.args) {
        if (!analize_expr(arg))
          continue;

        _set_type_info(arg, TypesRepository::get().get_type_node("i32"));
      }
      return true;
    }

    const AstFnProto &fn_proto = *fn_call.fn_ref;
    if (fn_call.args.size() != fn_proto.params.size()) {
      auto params_size = fn_proto.params.size();
      auto args_size = fn_call.args.size();

      // BUG(pablo96): len1 != len2
      add_semantic_error(errors, in_expr, ERROR_ARGUMENT_COUNT_MISMATCH, params_size, args_size);
      return false;
    }

    auto err_size_before = errors.size();

    size_t i = 0;
    for (auto arg : fn_call.args) {
      if (!analize_expr(arg)) {
        i++;
        continue;
      }

      const AstParamDef *param = fn_proto.params.at(i);
      const AstType *param_type = param->type;

      check_and_set_type(in_expr, param_type, arg);

      i++;
    }

    auto err_size_after = errors.size();
    return err_size_after == err_size_before;
  }
  case AstNodeType::AST_SYMBOL: {
    auto name = std::string(in_expr->symbol()->cached_name);
    const AstNode *data = in_expr->symbol()->data =
      resolve_function_variable(errors, symbol_table, name, in_expr, &in_expr->symbol()->type);
    return data != nullptr;
  }
  case AstNodeType::AST_CONST_VALUE: {
    switch (in_expr->const_value()->type) {
    case ConstValueType::BOOL:
    case ConstValueType::INT:
    case ConstValueType::FLOAT:
    case ConstValueType::CHAR:
      return true;
    default:
      LL_UNREACHEABLE;
    }
  }
  default:
    LL_UNREACHEABLE;
  }
  return false;
}

bool SemanticAnalyzer::check_and_set_type(const AstNode *in_node, const AstType *l_type_node,
                                          const AstNode *expr_node) {
  const AstType *expr_type = get_expr_type(errors, symbol_table, expr_node);

  // return false if:
  // 1. is not a constant and the types are not the same
  // 2. or it is a constant but the types dont match
  if (expr_node->node_type != AstNodeType::AST_CONST_VALUE && !check_types(errors, l_type_node, expr_type, in_node)) {
    return false;
  } else if (expr_type->type_info->type_id != l_type_node->type_info->type_id) {
    add_semantic_error(errors, expr_node, ERROR_TYPES_MISMATCH);
    return false;
  }

  _set_type_info(expr_node, l_type_node);
  return true;
}

const AstNode *resolve_function_variable(std::vector<Error> &errors, const Table *symbol_table,
                                         const std::string &in_name, const AstNode *in_parent_node,
                                         SymbolType *out_symbol_type) {
  auto curr_table = symbol_table;
  do {
    // is it a scope? (functions)
    if (curr_table->has_child(in_name)) {
      const Symbol &symbol = curr_table->get_symbol(in_name);
      if (out_symbol_type)
        *out_symbol_type = SymbolType::FUNC;
      return symbol.data_node;
    }
    // is it a variable?
    if (curr_table->has_symbol(in_name)) {
      const Symbol &symbol = curr_table->get_symbol(in_name);
      if (out_symbol_type)
        *out_symbol_type = SymbolType::VAR;
      return symbol.data_node;
    }
    curr_table = curr_table->parent;
  } while (curr_table);

  // may be a scope in another file
  // TODO(pablo96): search in loaded files!

  add_semantic_error(errors, in_parent_node, ERROR_UNKNOWN_SYMBOL, in_name.c_str());

  return nullptr;
}

bool check_types(std::vector<Error> &errors, const AstType *type_node0, const AstType *type_node1,
                 const AstNode *expr_node) {
  LL_ASSERT(type_node0 != nullptr);
  LL_ASSERT(type_node1 != nullptr);
  LL_ASSERT(type_node0->node_type == AstNodeType::AST_TYPE);
  LL_ASSERT(type_node1->node_type == AstNodeType::AST_TYPE);
  LL_ASSERT(expr_node->node_type != AstNodeType::AST_TYPE);

  if (type_node0->type_info->type_id == type_node1->type_info->type_id) {
    if (_is_type_array_or_pointer(type_node0))
      return check_types(errors, type_node0->child_type, type_node1->child_type, expr_node);

    if (type_node0->type_info->type_id == AstTypeId::STRUCT)
      // NOTE: Structs No Supported
      // TODO: Add support for structs
      LL_UNREACHEABLE;

    if (type_node0->type_info->type_id == AstTypeId::INTEGER)
      return _check_integer_types(errors, expr_node, type_node0, type_node1);

    if (type_node0->type_info->type_id == AstTypeId::FLOATING_POINT)
      return _check_floating_point_types(errors, expr_node, type_node0, type_node1);

    if (type_node0->type_info->type_id == AstTypeId::BOOL || type_node0->type_info->type_id == AstTypeId::VOID)
      return true;

    if (type_node0->type_info->type_id == AstTypeId::UNKNOWN) {
      // TODO(pablo96): should unknown types comparison ever happen?
      LL_UNREACHEABLE;
    }
  }

  // incompatible types
  add_semantic_error(errors, expr_node, ERROR_TYPES_MISMATCH);

  return false;
}

const AstType *get_expr_type(std::vector<Error> &errors, const Table *symbol_table, const AstNode *expr) {
  LL_ASSERT(symbol_table != nullptr);
  LL_ASSERT(expr != nullptr);

  switch (expr->node_type) {
  case AstNodeType::AST_BINARY_EXPR:
    return _get_binary_expr_type(errors, symbol_table, expr->binary_expr());
  case AstNodeType::AST_UNARY_EXPR:
    return _get_unary_expr_type(errors, symbol_table, expr->unary_expr());
  case AstNodeType::AST_FN_CALL_EXPR:
    return _get_fn_call_expr_type(errors, symbol_table, expr->fn_call());
  case AstNodeType::AST_SYMBOL:
    return _get_symbol_type(errors, symbol_table, expr->symbol());
  case AstNodeType::AST_CONST_VALUE:
    return _get_const_value_type(errors, symbol_table, expr->const_value());
  case AstNodeType::AST_FN_DEF:
    return expr->fn_def()->proto->return_type;
  case AstNodeType::AST_FN_PROTO:
    return expr->fn_proto()->return_type;
  default:
    LL_UNREACHEABLE;
  }
}

void add_semantic_error(std::vector<Error> &errors, const AstNode *in_node, const char *in_msg, ...) {
  va_list ap, ap2;
  va_start(ap, in_msg);
  va_copy(ap2, ap);

  int len1 = snprintf(nullptr, 0, in_msg, ap);
  LL_ASSERT(len1 >= 0);

  const int CAPACITY = len1 + 1;
  char *msg = new char[CAPACITY];

  int len2 = snprintf(msg, CAPACITY, in_msg, ap2);
  LL_ASSERT(len2 >= 0);
  // assert(len2 == len1);

  Error error(ERROR_TYPE::ERROR, in_node->line, in_node->column, in_node->file_name, msg);
  errors.push_back(error);

  delete[] msg;

  va_end(ap);
  va_end(ap2);
}

void _set_type_info(const AstNode *in_expr_node, const AstType *in_type_node) {
  LL_ASSERT(in_expr_node != nullptr);
  LL_ASSERT(in_expr_node->node_type != AstNodeType::AST_TYPE);
  LL_ASSERT(in_type_node != nullptr);
  LL_ASSERT(in_type_node->node_type == AstNodeType::AST_TYPE);

  switch (in_expr_node->node_type) {
  case AstNodeType::AST_CONST_VALUE:
    if (_is_type_array_or_pointer(in_type_node) || in_type_node->type_info->type_id == AstTypeId::STRUCT ||
        in_type_node->type_info->type_id == AstTypeId::VOID) {
      LL_UNREACHEABLE;
    }
    in_expr_node->const_value()->bit_size = in_type_node->type_info->bit_size;

    LL_FALLTHROUGH
  default:
    break;
  }
}

bool _analize_param(std::vector<Error> &errors, Table *symbol_table, const AstParamDef *in_node) {
  LL_ASSERT(symbol_table != nullptr);
  LL_ASSERT(in_node != nullptr);
  LL_ASSERT(in_node->node_type == AstNodeType::AST_PARAM_DEF);

  // variable declaration and definition
  if (in_node->initializer) {
    add_semantic_error(errors, in_node, ERROR_UNSUPPORTED_PARAM_INITIALIZER);
    return false;
  }

  // just an unitialized variable
  return true;
}

/**
 * NOTE: here we start defining the helper functions
 * for the get_expr_type fn
 **/

inline bool _is_boolean_bin_op(const AstBinaryExpr *in_bin_expr) {
  switch (in_bin_expr->bin_op) {
  case BinaryExprType::EQUALS:
  case BinaryExprType::NOT_EQUALS:
  case BinaryExprType::GREATER:
  case BinaryExprType::LESS:
  case BinaryExprType::GREATER_OR_EQUALS:
  case BinaryExprType::LESS_OR_EQUALS:
    return true;
  default:
    return false;
  }
}

inline bool _is_shift_bin_op(const AstBinaryExpr *in_bin_expr) {
  return in_bin_expr->bin_op == BinaryExprType::LSHIFT || in_bin_expr->bin_op == BinaryExprType::RSHIFT;
}

inline bool _is_only_one_node_const_value(const AstNode *in_left_expr, const AstNode *in_right_expr) {
  return (in_left_expr->node_type != AstNodeType::AST_CONST_VALUE) !=
         (in_right_expr->node_type != AstNodeType::AST_CONST_VALUE);
}

inline bool _is_number_type(const AstType *in_type) {
  return in_type->type_info->type_id == AstTypeId::INTEGER || in_type->type_info->type_id == AstTypeId::FLOATING_POINT;
}

const AstType *_get_binary_expr_type(std::vector<Error> &errors, const Table *symbol_table,
                                     const AstBinaryExpr *in_bin_expr) {
  if (_is_boolean_bin_op(in_bin_expr))
    return TypesRepository::get().get_type_node("bool");

  if (_is_shift_bin_op(in_bin_expr))
    return get_expr_type(errors, symbol_table, in_bin_expr->left_expr);

  const AstType *type_left = get_expr_type(errors, symbol_table, in_bin_expr->left_expr);
  const AstType *type_right = get_expr_type(errors, symbol_table, in_bin_expr->right_expr);

  if (type_left == nullptr || type_right == nullptr)
    return nullptr;

  if (_is_only_one_node_const_value(in_bin_expr->left_expr, in_bin_expr->right_expr)) {
    if (type_left->type_info->type_id != type_right->type_info->type_id) {
      return nullptr;
    }

    LL_ASSERT(_is_number_type(type_left) != _is_number_type(type_left));

    return in_bin_expr->left_expr->node_type == AstNodeType::AST_CONST_VALUE ? type_right : type_left;
  }

  if (type_left->type_info != type_right->type_info) {
    // TODO(pablo96): error types mismatch
    return nullptr;
  }

  // return any of them because they are equals
  return type_left;
}

const AstType *_get_unary_expr_type(std::vector<Error> &errors, const Table *symbol_table,
                                    const AstUnaryExpr *in_unary_expr) {
  if (in_unary_expr->op == UnaryExprType::NOT)
    return TypesRepository::get().get_type_node("bool");

  return get_expr_type(errors, symbol_table, in_unary_expr->expr);
}

const AstType *_get_const_value_type(std::vector<Error> &errors, const Table *symbol_table,
                                     const AstConstValue *in_const_value) {
  TypesRepository &types_repository = TypesRepository::get();
  switch (in_const_value->type) {
  case ConstValueType::BOOL:
    return types_repository.get_type_node("bool");
  case ConstValueType::FLOAT:
    return types_repository.get_type_node("f128");
  case ConstValueType::INT:
    if (in_const_value->is_negative)
      return types_repository.get_type_node("i128");
    return types_repository.get_type_node("u128");
  case ConstValueType::CHAR:
    return types_repository.get_type_node("u32");
  default:
    LL_UNREACHEABLE;
  }
}

const AstType *_get_fn_call_expr_type(std::vector<Error> &errors, const Table *symbol_table,
                                      const AstFnCallExpr *in_fn_call_expr) {
  const AstFnProto **fn_node = &in_fn_call_expr->fn_ref;

  if (!*fn_node) {
    auto fn_name = std::string(in_fn_call_expr->fn_name);
    *fn_node = resolve_function_variable(errors, symbol_table, fn_name, in_fn_call_expr)->fn_proto();
  }

  LL_ASSERT((*fn_node)->node_type == AstNodeType::AST_FN_PROTO);

  return get_expr_type(errors, symbol_table, *fn_node);
}

const AstType *_get_symbol_type(std::vector<Error> &errors, const Table *symbol_table, const AstSymbol *in_symbol) {
  // TODO(pablo96): should make it posible to resolve function names as func pointers
  std::string name = std::string(in_symbol->cached_name);

  const AstNode *var_node = resolve_function_variable(errors, symbol_table, name, in_symbol);
  if (!var_node) {
    return nullptr;
  }

  switch (var_node->node_type) {
  case AstNodeType::AST_VAR_DEF:
    return var_node->var_def()->type;
  case AstNodeType::AST_PARAM_DEF:
    return var_node->param_decl()->type;
  case AstNodeType::AST_FN_PROTO:
    return var_node->fn_proto()->return_type;
  default:
    LL_UNREACHEABLE;
  }
}

bool _check_integer_types(std::vector<Error> &errors, const AstNode *in_expr_node, const AstType *in_type_0,
                          const AstType *in_type_1) {
  bool same_size = in_type_0->type_info->bit_size == in_type_1->type_info->bit_size;

  if (!same_size) {
    add_semantic_error(errors, in_expr_node, ERROR_TYPES_SIZE_MISMATCH);
    return false;
  }

  bool same_sign = in_type_0->type_info->is_signed & in_type_1->type_info->is_signed;

  if (!same_sign) {
    add_semantic_error(errors, in_expr_node, ERROR_INTEGERS_SIGN_MISMATCH);
    return false;
  }

  return true;
}

bool _check_floating_point_types(std::vector<Error> &errors, const AstNode *in_expr_node, const AstType *in_type_0,
                                 const AstType *in_type_1) {
  bool are_same = in_type_0->type_info->bit_size == in_type_1->type_info->bit_size;

  if (!are_same) {
    add_semantic_error(errors, in_expr_node, ERROR_TYPES_SIZE_MISMATCH);
    return false;
  }

  return true;
}
