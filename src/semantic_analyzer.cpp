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

inline static bool _is_type(const AstType *in_type, AstTypeId in_type_id) {
  return in_type->type_info->type_id == in_type_id;
}

inline static bool _are_type_compatible(const AstType *in_type0, const AstType *in_type1) {
  return in_type0->type_info->type_id == in_type1->type_info->type_id;
}

inline bool _is_boolean_bin_op(const AstBinaryExpr *in_bin_expr);

inline bool _is_shift_bin_op(const AstBinaryExpr *in_bin_expr);

inline bool _is_only_one_node_const_value(const AstNode *in_left_expr, const AstNode *in_right_expr);

inline bool _both_are_not_const_value(const AstNode *in_left_expr, const AstNode *in_right_expr);

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
  if (!_are_type_compatible(in_var_def->type, expr_type)) {
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

bool SemanticAnalyzer::analize_if_stmnt(const AstIfStmnt *in_if_stmnt) {
  LL_ASSERT(in_if_stmnt->node_type == AstNodeType::AST_IF_STMNT);

  const AstNode *conditional_expr = in_if_stmnt->condition_expr;
  if (!analize_expr(conditional_expr))
    return false;

  if (!in_if_stmnt->is_condition_checked) {
    const AstType *expr_type = get_expr_type(this->errors, this->symbol_table, conditional_expr);
    if (!_is_type(expr_type, AstTypeId::BOOL)) {
      add_semantic_error(errors, conditional_expr, ERROR_EXPECTED_BOOL_EXPR,
                         get_type_id_name(expr_type->type_info->type_id));
      return false;
    }
  }

  const AstBlock *true_block = in_if_stmnt->true_block;
  if (!analize_block(true_block)) {
    // errors have been set inside analize_block so we just return false
    return false;
  }

  // check else block
  const AstBlock *false_block = in_if_stmnt->false_block;
  if (false_block != nullptr) {
    if (!analize_block(false_block)) {
      // errors have been set inside analize_block so we just return false
      return false;
    }
  }

  return true;
}

bool SemanticAnalyzer::analize_loop_stmnt(const AstLoopStmnt *in_loop_stmnt) {
  LL_ASSERT(in_loop_stmnt->node_type == AstNodeType::AST_LOOP_STMNT);

  const AstNode *conditional_expr = in_loop_stmnt->condition_expr;
  if (!analize_expr(conditional_expr))
    return false;

  if (!in_loop_stmnt->is_condition_checked) {
    const AstType *expr_type = get_expr_type(this->errors, this->symbol_table, conditional_expr);
    if (!_is_type(expr_type, AstTypeId::BOOL)) {
      add_semantic_error(errors, conditional_expr, ERROR_EXPECTED_BOOL_EXPR,
                         get_type_id_name(expr_type->type_info->type_id));
      return false;
    }
  }

  const AstBlock *init_block = in_loop_stmnt->initializer_block;
  if (init_block && !analize_block(init_block)) {
    // errors have been set inside analize_block so we just return false
    return false;
  }

  const AstBlock *header_block = in_loop_stmnt->header_block;
  if (!analize_block(header_block)) {
    LL_UNREACHEABLE;
  }

  const AstBlock *footer_block = in_loop_stmnt->footer_block;
  if (footer_block) {
    if (!analize_block(footer_block)) {
      LL_UNREACHEABLE;
    }
  }

  const AstBlock *content_block = in_loop_stmnt->content_block;
  if (!analize_block(content_block)) {
    // errors have been set inside analize_block so we just return false
    return false;
  }

  return true;
}

inline static bool _analize_const_value(const AstConstValue *in_const_value) {
  switch (in_const_value->type) {
  case ConstValueType::BOOL:
  case ConstValueType::INT:
  case ConstValueType::FLOAT:
  case ConstValueType::CHAR:
  case ConstValueType::PTR:
    return true;
  default:
    LL_UNREACHEABLE;
  }
}

inline static bool _analize_symbol(std::vector<Error> &in_errors, const Table *in_symbol_table,
                                   const AstSymbol *in_symbol) {
  auto name = std::string(in_symbol->cached_name);
  const AstNode *data = in_symbol->data =
    resolve_function_variable(in_errors, in_symbol_table, name, in_symbol, &in_symbol->type);
  return data != nullptr;
}

inline bool SemanticAnalyzer::_analize_call_expr(const AstFnCallExpr *in_fn_call) {
  auto fn_name = std::string(in_fn_call->fn_name);

  SymbolType symbol_type;
  in_fn_call->fn_ref =
    resolve_function_variable(this->errors, this->symbol_table, fn_name, in_fn_call, &symbol_type)->fn_proto();

  if (symbol_type != SymbolType::FUNC) {
    add_semantic_error(this->errors, in_fn_call, ERROR_SYMBOL_NOT_A_FN, fn_name.c_str());
    return false;
  }

  LL_ASSERT(in_fn_call->fn_ref->node_type == AstNodeType::AST_FN_PROTO);

  if (!in_fn_call->fn_ref) {
    add_semantic_error(this->errors, in_fn_call, ERROR_UNDECLARED_FN, fn_name.c_str());
    return false;
  }

  // TODO(pablo96): temporal solution to printf!
  if (fn_name == "printf") {
    for (auto arg : in_fn_call->args) {
      if (!analize_expr(arg))
        continue;

      _set_type_info(arg, TypesRepository::get().get_type_node("s32"));
    }
    return true;
  }

  const AstFnProto &fn_proto = *in_fn_call->fn_ref;
  if (in_fn_call->args.size() != fn_proto.params.size()) {
    auto params_size = fn_proto.params.size();
    auto args_size = in_fn_call->args.size();

    add_semantic_error(this->errors, in_fn_call, ERROR_ARGUMENT_COUNT_MISMATCH, params_size, args_size);
    return false;
  }

  auto err_size_before = this->errors.size();

  size_t i = 0;
  for (AstNode *arg : in_fn_call->args) {
    if (!analize_expr(arg)) {
      i++;
      continue;
    }

    const AstParamDef *param = fn_proto.params.at(i);
    const AstType *param_type = param->type;

    check_and_set_type(in_fn_call, param_type, arg);

    i++;
  }

  size_t err_size_after = this->errors.size();
  return err_size_after == err_size_before;
}

inline bool SemanticAnalyzer::_analize_unary_expr(const AstUnaryExpr *in_unary_expr) {
  if (in_unary_expr->op == UnaryExprType::RET && in_unary_expr->node_type == AstNodeType::AST_LOOP_STMNT) {
    add_semantic_error(this->errors, in_unary_expr, ERROR_RET_STMNT_INVALID_LOCATION, "direct loop");
    return false;
  }

  const AstType *expr_type = get_expr_type(this->errors, this->symbol_table, in_unary_expr->expr);

  if (!expr_type) {
    return false;
  }

  // Overloading operators is not supported
  if (_is_type(expr_type, AstTypeId::STRUCT)) {
    auto operator_symbol = get_unary_op_symbol(in_unary_expr->op);
    add_semantic_error(this->errors, in_unary_expr, ERROR_UNSUPORTED_UNARY_OP_STRUCT_EXPR, operator_symbol.c_str());
    return false;
  }

  // If it is a boolean expr only the NOT operator is valid
  if (_is_type(expr_type, AstTypeId::BOOL) && in_unary_expr->op != UnaryExprType::NOT) {
    auto operator_symbol = get_unary_op_symbol(in_unary_expr->op);
    add_semantic_error(this->errors, in_unary_expr, ERROR_UNSUPORTED_OP_BOOL_EXPR, operator_symbol.c_str());
    return false;
  }

  // If it is not boolean expr the NOT operator is invalid
  if (!_is_type(expr_type, AstTypeId::BOOL) && in_unary_expr->op == UnaryExprType::NOT) {
    add_semantic_error(this->errors, in_unary_expr, ERROR_UNSUPORTED_OP_NOT_BOOL_EXPR);
    return false;
  }

  // if it is the address_of operator then only symbols are supported
  if (in_unary_expr->op == UnaryExprType::ADDRESS_OF && in_unary_expr->expr->node_type != AstNodeType::AST_SYMBOL) {
    add_semantic_error(this->errors, in_unary_expr, ERROR_UNSUPORTED_ADDRESSOF_NOT_SYMBOL_EXPR);
    return false;
  }

  return analize_expr(in_unary_expr->expr);
}

inline bool SemanticAnalyzer::_analize_binary_expr(const AstBinaryExpr *in_binary_expr) {
  const AstNode *l_expr = in_binary_expr->left_expr;
  const AstNode *r_expr = in_binary_expr->right_expr;

  bool is_left_ok = analize_expr(l_expr);
  bool is_right_ok = analize_expr(r_expr);

  if (!is_left_ok || !is_right_ok)
    return false;

  if (_is_shift_bin_op(in_binary_expr)) {
    const AstType *r_expr_type = get_expr_type(this->errors, this->symbol_table, r_expr);
    LL_ASSERT(r_expr_type != nullptr);

    // right value should be an integer
    if (!_is_type(r_expr_type, AstTypeId::INTEGER)) {
      add_semantic_error(this->errors, in_binary_expr, ERROR_BIT_SHIFT_LEFT_EXPR_NO_INT);
      return false;
    }

    return true;
  }

  const AstType *l_expr_type = get_expr_type(this->errors, this->symbol_table, l_expr);
  const AstType *r_expr_type = get_expr_type(this->errors, this->symbol_table, r_expr);

  if (!_are_type_compatible(l_expr_type, r_expr_type)) {
    add_semantic_error(this->errors, in_binary_expr, ERROR_TYPES_MISMATCH);
    return false;
  }

  if (_both_are_not_const_value(l_expr, r_expr))
    return check_types(this->errors, l_expr_type, r_expr_type, in_binary_expr);
  else if (_is_only_one_node_const_value(l_expr, r_expr)) {
    if (l_expr->node_type == AstNodeType::AST_CONST_VALUE)
      _set_type_info(l_expr, r_expr_type);
    else
      _set_type_info(r_expr, l_expr_type);
  }

  return true;
}

inline bool SemanticAnalyzer::_analize_ctrl_stmnt(const AstCtrlStmnt *in_ctrl_stmnt) {
  LL_ASSERT(in_ctrl_stmnt->parent != nullptr);
  LL_ASSERT(in_ctrl_stmnt->node_type == AstNodeType::AST_CTRL_STMNT);

  // check for parent is not loop
  if (in_ctrl_stmnt->parent->node_type == AstNodeType::AST_LOOP_STMNT) {
    add_semantic_error(this->errors, in_ctrl_stmnt, ERROR_CTRL_STMNT_INVALID_LOCATION, "direct loop");
    return false;
  }

  // check for any grandparent to be a loop
  AstNode *node = in_ctrl_stmnt->parent;
  while (node->node_type != AstNodeType::AST_LOOP_STMNT) {
    // check for parent is not fn
    if (in_ctrl_stmnt->parent->node_type == AstNodeType::AST_FN_DEF) {
      add_semantic_error(this->errors, in_ctrl_stmnt, ERROR_CTRL_STMNT_INVALID_LOCATION, "no outer loop");
      return false;
    }

    node = node->parent;

    LL_ASSERT(node != nullptr);
  }

  in_ctrl_stmnt->loop_ref = node->loop_stmnt();

  return true;
}

bool SemanticAnalyzer::analize_expr(const AstNode *in_expr) {
  switch (in_expr->node_type) {
  case AstNodeType::AST_BINARY_EXPR:
    return _analize_binary_expr(in_expr->binary_expr());
  case AstNodeType::AST_UNARY_EXPR:
    return _analize_unary_expr(in_expr->unary_expr());
  case AstNodeType::AST_FN_CALL_EXPR:
    return _analize_call_expr(in_expr->fn_call());
  case AstNodeType::AST_SYMBOL:
    return _analize_symbol(this->errors, this->symbol_table, in_expr->symbol());
  case AstNodeType::AST_CONST_VALUE:
    return _analize_const_value(in_expr->const_value());
  case AstNodeType::AST_IF_STMNT:
    return analize_if_stmnt(in_expr->if_stmnt());
  case AstNodeType::AST_LOOP_STMNT:
    return analize_loop_stmnt(in_expr->loop_stmnt());
  case AstNodeType::AST_CTRL_STMNT:
    return _analize_ctrl_stmnt(in_expr->ctrl_stmnt());
  default:
    LL_UNREACHEABLE;
  }
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

  if (_are_type_compatible(type_node0, type_node1)) {
    if (_is_type_array_or_pointer(type_node0))
      return check_types(errors, type_node0->child_type, type_node1->child_type, expr_node);

    if (_is_type(type_node0, AstTypeId::STRUCT))
      // NOTE: Structs No Supported
      // TODO: Add support for structs
      LL_UNREACHEABLE;

    if (_is_type(type_node0, AstTypeId::INTEGER))
      return _check_integer_types(errors, expr_node, type_node0, type_node1);

    if (_is_type(type_node0, AstTypeId::FLOATING_POINT))
      return _check_floating_point_types(errors, expr_node, type_node0, type_node1);

    if (_is_type(type_node0, AstTypeId::BOOL) || _is_type(type_node0, AstTypeId::VOID))
      return true;

    if (_is_type(type_node0, AstTypeId::UNKNOWN)) {
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

  s32 len1 = snprintf(nullptr, 0, in_msg, ap);
  LL_ASSERT(len1 >= 0);

  const s32 CAPACITY = len1 + 1;
  char *msg = new char[CAPACITY];

  s32 len2 = snprintf(msg, CAPACITY, in_msg, ap2);
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
    if (in_type_node->type_info->type_id == AstTypeId::ARRAY || in_type_node->type_info->type_id == AstTypeId::STRUCT ||
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

bool _is_boolean_bin_op(const AstBinaryExpr *in_bin_expr) {
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

bool _is_shift_bin_op(const AstBinaryExpr *in_bin_expr) {
  return in_bin_expr->bin_op == BinaryExprType::LSHIFT || in_bin_expr->bin_op == BinaryExprType::RSHIFT;
}

bool _is_only_one_node_const_value(const AstNode *in_left_expr, const AstNode *in_right_expr) {
  return (in_left_expr->node_type != AstNodeType::AST_CONST_VALUE) !=
         (in_right_expr->node_type != AstNodeType::AST_CONST_VALUE);
}

bool _both_are_not_const_value(const AstNode *in_left_expr, const AstNode *in_right_expr) {
  return (in_left_expr->node_type != AstNodeType::AST_CONST_VALUE) &&
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

    // TODO(pablo96): revisar por que esta el assert y comentarlo
    LL_ASSERT(_is_number_type(type_left) == _is_number_type(type_left));

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
      return types_repository.get_type_node("s128");
    return types_repository.get_type_node("u128");
  case ConstValueType::CHAR:
    return types_repository.get_type_node("u32");
  case ConstValueType::PTR:
    return types_repository.get_type_node("*u8");
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

  bool same_sign = in_type_0->type_info->is_signed == in_type_1->type_info->is_signed;

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
