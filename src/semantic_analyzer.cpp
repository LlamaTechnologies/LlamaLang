#include "semantic_analyzer.hpp"

#include "Types.hpp"
#include "ast_nodes.hpp"
#include "common_defs.hpp"
#include "semantic_error_msgs.hpp"

#include <stdarg.h>
#include <stdlib.h>

//----- PRE DECLARATIONS - UTIL FUNCTIONS -----

#define IS_RET_STATEMENT(node) \
  node->node_type == AstNodeType::AST_UNARY_EXPR && node->unary_expr()->op == UnaryExprType::RET

static bool _analize_param(std::vector<Error> &errors, Table *symbol_table, const AstParamDef *in_node);
static void _set_type_info(const AstNode *expr_node, const AstType *type_node);

//----- START PRIMARY CHECK FUNCTIONS -----

bool SemanticAnalyzer::analize_var_def(const AstVarDef *in_node, const bool is_global) {
  LL_ASSERT(in_node != nullptr);
  LL_ASSERT((in_node->node_type == AstNodeType::AST_VAR_DEF));

  if (is_global) {
    const AstVarDef &var_def = *in_node->var_def();
    auto var_name = std::string(var_def.name);

    // global variable needs initializer!
    if (!var_def.initializer) {
      add_semantic_error(errors, in_node, ERROR_GLOBAL_NEED_INITIALIZER, var_def.name);
      return false;
    }

    // you can not assign expressions to globals!
    if (var_def.initializer->node_type != AstNodeType::AST_CONST_VALUE) {
      add_semantic_error(errors, in_node, ERROR_GLOBAL_INITIALIZER_NO_CONST, var_def.name);
      return false;
    }

    global_symbol_table->add_symbol(var_name, SymbolType::VAR, in_node);

    const AstType *expr_type = get_expr_type(errors, symbol_table, var_def.initializer);
    if (var_def.type->type_info->type_id != expr_type->type_info->type_id) {
      add_semantic_error(errors, in_node, ERROR_TYPES_MISMATCH, var_def.name);
      global_symbol_table->remove_last_symbol();
      return false;
    }

    _set_type_info(var_def.initializer, var_def.type);
    return true;
  }

  const AstVarDef &var_def = *in_node->var_def();
  auto var_name = std::string(var_def.name);

  // local variable declaration
  symbol_table->add_symbol(var_name, SymbolType::VAR, in_node);

  // variable declaration and definition
  if (var_def.initializer) {
    bool is_valid_init = analize_expr(var_def.initializer);
    if (!is_valid_init)
      return false;

    const AstType *expr_type = get_expr_type(errors, symbol_table, var_def.initializer);
    if (var_def.type->type_info->type_id != expr_type->type_info->type_id) {
      add_semantic_error(errors, in_node, ERROR_TYPES_MISMATCH, var_def.name);
      symbol_table->remove_last_symbol();
      return false;
    }

    _set_type_info(var_def.initializer, var_def.type);
    return true;
  }

  // just a unitialized variable
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

  // add function to the symbol table
  symbol_table->add_symbol(str_name, SymbolType::FUNC, in_proto);
  // create the function's symbol table
  symbol_table = symbol_table->create_child(std::string(in_proto->name));

  // check every param to not have any default value
  // and add them to the function's symbol table
  bool has_no_error = true;
  for (AstParamDef *param : in_proto->params) {
    bool param_ok = _analize_param(errors, symbol_table, param);
    has_no_error = has_no_error && param_ok;
  }

  LL_ASSERT(symbol_table->parent != nullptr);
  // set symbol table to previous table
  symbol_table = symbol_table->parent;
  return has_no_error;
}

bool SemanticAnalyzer::analize_fn_block(const AstBlock *in_func_block, AstFnDef *in_function) {
  auto ret_type = in_function->proto->return_type;

  // set function's symbol table as current table
  symbol_table = symbol_table->get_child(std::string(in_function->proto->name));

  size_t errors_before = errors.size();

  // gather:
  // - ret stmnt required
  // - ret stmnt type

  bool has_ret_type = ret_type->type_info->type_id != AstTypeId::VOID;
  bool has_ret_stmnt = false;

  // check every statement in the function
  for (auto stmnt : in_func_block->statements) {
    if (IS_RET_STATEMENT(stmnt)) {
      has_ret_stmnt = true;

      if (!analize_expr(stmnt))
        continue;

      // if ret stmnt is ok
      // then check if return type is the same as the one in the prototype
      check_and_set_type(stmnt, ret_type, stmnt->unary_expr()->expr);
    } else if (stmnt->node_type == AstNodeType::AST_VAR_DEF) {
      analize_var_def(stmnt->var_def(), false);
    } else {
      analize_expr(stmnt);
    }
  }

  // if the function should return a type and it doesnt then report it;
  if (has_ret_type && !has_ret_stmnt) {
    add_semantic_error(errors, in_function->proto, ERROR_REQUIRE_RET_STMNT);
  }

  // if the function has no parent is a bug in the compiler
  // at least global scope should be the parent of the function
  LL_ASSERT(symbol_table->parent != nullptr);

  symbol_table = symbol_table->parent;

  size_t errors_after = errors.size();
  return errors_before == errors_after;
}

//----- END PRIMARY CHECK FUNCTIONS -------

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
    const AstFnCallExpr &fn_call = *in_expr->func_call();

    SymbolType symbol_type;
    fn_call.fn_ref =
      resolve_function_variable(errors, symbol_table, std::string(fn_call.fn_name), in_expr, &symbol_type);

    if (symbol_type != SymbolType::FUNC) {
      add_semantic_error(errors, in_expr, ERROR_SYMBOL_NOT_A_FN, fn_call.fn_name);
      return false;
    }

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

    const AstFnProto &fn_proto = *fn_call.fn_ref->function_proto();
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
    if (curr_table->has_child(in_name)) {
      const Symbol &symbol = curr_table->get_symbol(in_name);
      if (out_symbol_type)
        *out_symbol_type = SymbolType::FUNC;
      return symbol.data_node;
    }
    if (curr_table->has_symbol(in_name)) {
      const Symbol &symbol = curr_table->get_symbol(in_name);
      if (out_symbol_type)
        *out_symbol_type = SymbolType::VAR;
      return symbol.data_node;
    }
    curr_table = curr_table->parent;
  } while (curr_table);

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

  const AstType &type0 = *type_node0;
  const AstType &type1 = *type_node1;

  if (type0.type_info->type_id == type1.type_info->type_id) {
    // check arrays or pointers
    if (type0.type_info->type_id == AstTypeId::POINTER || type0.type_info->type_id == AstTypeId::ARRAY)
      return check_types(errors, type0.child_type, type1.child_type, expr_node);

    // check complex types
    if (type0.type_info->type_id == AstTypeId::STRUCT)
      // NOTE: Structs No Supported
      // TODO: Add support for structs
      LL_UNREACHEABLE;

    if (type0.type_info->type_id == AstTypeId::INTEGER) {
      bool same_size = type0.type_info->bit_size == type1.type_info->bit_size;

      if (!same_size) {
        add_semantic_error(errors, expr_node, ERROR_TYPES_SIZE_MISMATCH);
        return false;
      }

      bool same_sign = type0.type_info->is_signed & type1.type_info->is_signed;

      if (!same_sign) {
        add_semantic_error(errors, expr_node, ERROR_INTEGERS_SIGN_MISMATCH);
        return false;
      }
    }

    if (type0.type_info->type_id == AstTypeId::FLOATING_POINT) {
      bool are_same = type0.type_info->bit_size == type1.type_info->bit_size;

      if (!are_same) {
        add_semantic_error(errors, expr_node, ERROR_TYPES_SIZE_MISMATCH);
        return false;
      }
    }

    return true;
  }

  // incompatible types
  add_semantic_error(errors, expr_node, ERROR_TYPES_MISMATCH);

  return false;
}

bool check_compatible_types(std::vector<Error> &errors, const AstType *type_node0, const AstType *type_node1,
                            const AstNode *expr_node) {
  LL_ASSERT(type_node0 != nullptr);
  LL_ASSERT(type_node1 != nullptr);
  LL_ASSERT(type_node0->node_type == AstNodeType::AST_TYPE);
  LL_ASSERT(type_node1->node_type == AstNodeType::AST_TYPE);
  LL_ASSERT(expr_node->node_type != AstNodeType::AST_TYPE);

  const AstType &type0 = *type_node0;
  const AstType &type1 = *type_node1;

  // check arrays or pointers
  if (type0.type_info->type_id == AstTypeId::POINTER || type0.type_info->type_id == AstTypeId::ARRAY)
    return check_types(errors, type0.child_type, type1.child_type, expr_node);

  // check complex types
  if (type0.type_info->type_id == AstTypeId::STRUCT)
    // NOTE: Structs No Supported
    // TODO: Add support for structs
    LL_UNREACHEABLE;

  if (type0.type_info->type_id == AstTypeId::INTEGER) {
    bool same_size = type0.type_info->bit_size == type1.type_info->bit_size;

    if (!same_size) {
      add_semantic_error(errors, expr_node, ERROR_TYPES_SIZE_MISMATCH);
      return false;
    }

    bool same_sign = type0.type_info->is_signed & type1.type_info->is_signed;

    if (!same_sign) {
      add_semantic_error(errors, expr_node, ERROR_INTEGERS_SIGN_MISMATCH);
      return false;
    }
  }

  if (type0.type_info->type_id == AstTypeId::FLOATING_POINT) {
    bool are_same = type0.type_info->bit_size == type1.type_info->bit_size;

    if (!are_same) {
      add_semantic_error(errors, expr_node, ERROR_TYPES_SIZE_MISMATCH);
      return false;
    }
  }

  return true;
}

const AstType *get_expr_type(std::vector<Error> &errors, const Table *symbol_table, const AstNode *expr) {
  LL_ASSERT(symbol_table != nullptr);
  LL_ASSERT(expr != nullptr);

  TypesRepository types_repository = TypesRepository::get();

  switch (expr->node_type) {
  case AstNodeType::AST_BINARY_EXPR: {
    const AstBinaryExpr &bin_expr = *expr->binary_expr();
    switch (bin_expr.bin_op) {
    // BOOLEAN OPERATORS
    case BinaryExprType::EQUALS:
    case BinaryExprType::NOT_EQUALS:
    case BinaryExprType::GREATER:
    case BinaryExprType::LESS:
    case BinaryExprType::GREATER_OR_EQUALS:
    case BinaryExprType::LESS_OR_EQUALS:
      return types_repository.get_type_node("bool");
    case BinaryExprType::LSHIFT:
    case BinaryExprType::RSHIFT:
      return get_expr_type(errors, symbol_table, bin_expr.left_expr);
    default: {
      const AstType *type_left = get_expr_type(errors, symbol_table, bin_expr.left_expr);
      const AstType *type_right = get_expr_type(errors, symbol_table, bin_expr.right_expr);

      // if are null then an error happened
      if (type_left == nullptr || type_right == nullptr)
        return nullptr;

      // if only one of them is a constant
      if ((bin_expr.left_expr->node_type != AstNodeType::AST_CONST_VALUE) !=
          (bin_expr.right_expr->node_type != AstNodeType::AST_CONST_VALUE)) {
        // if they are not compatibles
        if (type_left->type_info->type_id != type_right->type_info->type_id) {
          return nullptr;
        }

        // if they are numbers
        LL_ASSERT(type_left->type_info->type_id == AstTypeId::INTEGER ||
                  type_left->type_info->type_id == AstTypeId::FLOATING_POINT);

        return bin_expr.left_expr->node_type == AstNodeType::AST_CONST_VALUE ? type_right : type_left;
      }

      // if they are not the same return nullptr
      if (type_left != type_right)
        return nullptr;

      // else return any of them
      return type_left;
    } // default
    } // switch
  }   // BinaryExpr
  case AstNodeType::AST_UNARY_EXPR: {
    const AstUnaryExpr &unary_expr = *expr->unary_expr();

    if (unary_expr.op == UnaryExprType::NOT)
      return types_repository.get_type_node("bool");

    return get_expr_type(errors, symbol_table, unary_expr.expr);
  }
  case AstNodeType::AST_FN_CALL_EXPR: {
    const AstNode **func_node = &expr->func_call()->fn_ref;
    if (!*func_node)
      *func_node = resolve_function_variable(errors, symbol_table, std::string(expr->func_call()->fn_name), expr);

    return get_expr_type(errors, symbol_table, *func_node);
  }
  case AstNodeType::AST_FN_DEF: {
    return expr->function_def()->proto->return_type;
  }
  case AstNodeType::AST_FN_PROTO: {
    return expr->function_proto()->return_type;
  }
  case AstNodeType::AST_SYMBOL: {
    // TODO(pablo96): should make it posible to resolve function names as func pointers
    std::string name = std::string(expr->symbol()->cached_name);

    const AstNode *var_node = resolve_function_variable(errors, symbol_table, name, expr);
    if (!var_node) {
      return nullptr;
    }

    switch (var_node->node_type) {
    case AstNodeType::AST_VAR_DEF:
      return var_node->var_def()->type;
    case AstNodeType::AST_PARAM_DEF:
      return var_node->param_decl()->type;
    case AstNodeType::AST_FN_DEF:
      return var_node->function_def()->proto->return_type;
    case AstNodeType::AST_FN_PROTO:
      return var_node->function_proto()->return_type;
    default:
      LL_UNREACHEABLE;
    } // var_node type switch
  }
  case AstNodeType::AST_CONST_VALUE: {
    const AstConstValue &const_value = *expr->const_value();
    switch (const_value.type) {
    case ConstValueType::BOOL:
      return types_repository.get_type_node("bool");
    case ConstValueType::FLOAT:
      return types_repository.get_type_node("f128");
    case ConstValueType::INT:
      if (const_value.is_negative)
        return types_repository.get_type_node("i128");
      return types_repository.get_type_node("u128");
    case ConstValueType::CHAR:
      return types_repository.get_type_node("u32");
    default:
      LL_UNREACHEABLE;
    } // constant type switch
  }
  default:
    LL_UNREACHEABLE;
  } // node type switch
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

void _set_type_info(const AstNode *expr_node, const AstType *type_node) {
  LL_ASSERT(expr_node != nullptr);
  LL_ASSERT(expr_node->node_type != AstNodeType::AST_TYPE);
  LL_ASSERT(type_node != nullptr);
  LL_ASSERT(type_node->node_type == AstNodeType::AST_TYPE);

  switch (expr_node->node_type) {
  case AstNodeType::AST_CONST_VALUE:
    if (type_node->type_info->type_id == AstTypeId::ARRAY || type_node->type_info->type_id == AstTypeId::POINTER ||
        type_node->type_info->type_id == AstTypeId::STRUCT || type_node->type_info->type_id == AstTypeId::VOID)
      LL_UNREACHEABLE;
    expr_node->const_value()->bit_size = type_node->type_info->bit_size;
    return;
  default:
    return;
  }
}

bool _analize_param(std::vector<Error> &errors, Table *symbol_table, const AstParamDef *in_node) {
  LL_ASSERT(symbol_table != nullptr);
  LL_ASSERT(in_node != nullptr);
  LL_ASSERT(in_node->node_type == AstNodeType::AST_PARAM_DEF);

  // add param as local variable
  symbol_table->add_symbol(std::string(in_node->name), SymbolType::VAR, in_node);

  // variable declaration and definition
  if (in_node->initializer) {
    add_semantic_error(errors, in_node, ERROR_UNSUPPORTED_PARAM_INITIALIZER);
    return false;
  }

  // just a unitialized variable
  return true;
}
