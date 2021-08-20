#include "semantic_analyzer.hpp"

#include "Types.hpp"
#include "ast_nodes.hpp"
#include "common_defs.hpp"
#include "semantic_error_msgs.hpp"

#include <stdarg.h>
#include <stdlib.h>

static bool is_ret_stmnt(const AstNode *stmnt);
static const AstNode *get_best_type(const AstNode *type_node0, const AstNode *type_node1);
static void set_type_info(const AstNode *expr_node, const AstNode *type_node);

Table *Table::create_child(const std::string &in_name) {
  auto child = new Table(in_name, this);
  last_child_key = in_name;
  children_scopes.emplace(in_name, child);
  return child;
}

bool Table::has_child(const std::string &in_name) { return children_scopes.find(in_name) != children_scopes.end(); }

bool Table::has_symbol(const std::string &in_name) { return symbols.find(in_name) != symbols.end(); }

Table *Table::get_child(const std::string &in_name) {
#ifdef LL_DEBUG
  if (has_child(in_name))
#endif
    return children_scopes.at(in_name);
#ifdef LL_DEBUG
  LL_UNREACHEABLE;
#endif
}

const Symbol &Table::get_symbol(const std::string &in_name) {
#ifdef LL_DEBUG
  if (has_symbol(in_name))
#endif
    return symbols.at(in_name);
#ifdef LL_DEBUG
  LL_UNREACHEABLE;
#endif
}

void Table::remove_last_child() { children_scopes.erase(last_child_key); }

void Table::add_symbol(const std::string &in_name, const SymbolType in_type, const AstNode *in_data) {
  Symbol symbol = Symbol(in_name, in_type, in_data);
  last_symbol_key = in_name;
  symbols.emplace(in_name, symbol);
}

void Table::remove_last_symbol() { symbols.erase(last_symbol_key); }

bool SemanticAnalyzer::analizeFuncProto(const AstNode *in_proto_node) {
  assert(in_proto_node != nullptr);
  assert(in_proto_node->node_type == AstNodeType::AstFuncProto);

  const AstFuncProto &func_proto = in_proto_node->function_proto;
  auto ret_type = func_proto.return_type;

  if (ret_type->ast_type.type_id == AstTypeId::Struct) {
    add_semantic_error(ret_type, ERROR_STRUCTS_UNSUPORTED);
    return false;
  }

  auto str_name = std::string(func_proto.name);

  symbol_table->add_symbol(str_name, SymbolType::FUNC, in_proto_node);
  symbol_table = symbol_table->create_child(std::string(func_proto.name));

  bool has_no_error = true;
  for (AstNode *param : func_proto.params) {
    bool param_ok = analizeVarDef(param, false);
    has_no_error = has_no_error && param_ok;
  }

  symbol_table = symbol_table->parent;
  return has_no_error;
}

bool SemanticAnalyzer::analizeFuncBlock(const AstBlock &in_func_block, AstFuncDef &in_function) {
  auto ret_type = in_function.proto->function_proto.return_type;

  symbol_table = symbol_table->get_child(std::string(in_function.proto->function_proto.name));

  size_t errors_before = errors.size();
  bool has_ret_type = ret_type->ast_type.type_id != AstTypeId::Void;
  bool has_ret_stmnt = false;

  for (auto stmnt : in_func_block.statements) {
    if (is_ret_stmnt(stmnt)) {
      has_ret_stmnt = true;
      analizeExpr(stmnt);
      auto expr_type = get_expr_type(stmnt->unary_expr.expr);
      check_type_compat(expr_type, ret_type, stmnt);
    } else if (stmnt->node_type == AstNodeType::AstVarDef) {
      analizeVarDef(stmnt, false);
    } else {
      analizeExpr(stmnt);
    }
  }

  // if the function should return a type and it doesnt then report it;
  if (has_ret_type && !has_ret_stmnt) {
    add_semantic_error(in_function.proto, ERROR_REQUIRE_RET_STMNT);
  }

  size_t errors_after = errors.size();

  if (symbol_table->parent)
    symbol_table = symbol_table->parent;

  return errors_before == errors_after;
}

bool SemanticAnalyzer::analizeVarDef(const AstNode *in_node, const bool is_global) {
  assert(in_node != nullptr);
  assert((in_node->node_type == AstNodeType::AstVarDef) || (in_node->node_type == AstNodeType::AstParamDecl));

  const AstVarDef &var_def = in_node->node_type == AstNodeType::AstVarDef ? in_node->var_def : in_node->param_decl;
  auto var_name = std::string(var_def.name);

  if (is_global) {
    if (!var_def.initializer) {
      add_semantic_error(in_node, ERROR_GLOBAL_NEED_INITIALIZER, var_def.name);
      return false;
    }
    global_symbol_table->add_symbol(var_name, SymbolType::VAR, in_node);


    const AstNode *expr_type = get_expr_type(var_def.initializer);
    if (!check_type_compat(var_def.type, expr_type, in_node)) {
      global_symbol_table->remove_last_symbol();
      return false;
    }
    
    set_type_info(var_def.initializer, var_def.type);
    return true;
  }

  symbol_table->add_symbol(var_name, SymbolType::VAR, in_node);

  if (var_def.initializer) {
    bool is_valid_init = analizeExpr(var_def.initializer);
    if (!is_valid_init)
      return false;

    auto expr_type = get_expr_type(var_def.initializer);
    if (!check_type_compat(var_def.type, expr_type, in_node)) {
      symbol_table->remove_last_symbol();
      return false;
    }
    set_type_info(var_def.initializer, expr_type);
  }

  return true;
}

bool SemanticAnalyzer::analizeExpr(const AstNode *in_expr) {
  switch (in_expr->node_type) {
  case AstNodeType::AstBinaryExpr: {
    const AstBinaryExpr &bin_expr = in_expr->binary_expr;
    switch (bin_expr.bin_op) {
    case BinaryExprType::LSHIFT:
    case BinaryExprType::RSHIFT: {
      if (!analizeExpr(bin_expr.left_expr) || !analizeExpr(bin_expr.right_expr))
        return false;

      // right value should be an integer
      auto r_expr_type_node = get_expr_type(bin_expr.right_expr);
      if (r_expr_type_node->ast_type.type_id != AstTypeId::Integer) {
        add_semantic_error(in_expr, ERROR_BIT_SHIFT_LEFT_EXPR_NO_INT);
        return false;
      }

      // now we depend on left expr beeing ok
      return get_expr_type(bin_expr.left_expr);
    }
    case BinaryExprType::EQUALS:
    case BinaryExprType::NOT_EQUALS:
    case BinaryExprType::GREATER_OR_EQUALS:
    case BinaryExprType::LESS_OR_EQUALS:
    case BinaryExprType::GREATER:
    case BinaryExprType::LESS:
      // We just check their expressions are good
      return analizeExpr(bin_expr.left_expr) && analizeExpr(bin_expr.right_expr);
    case BinaryExprType::ADD:
    case BinaryExprType::SUB:
    case BinaryExprType::MUL:
    case BinaryExprType::DIV:
    case BinaryExprType::MOD:
    case BinaryExprType::ASSIGN: {
      if (!analizeExpr(bin_expr.left_expr) || !analizeExpr(bin_expr.right_expr))
        return false;

      auto l_expr_type_node = get_expr_type(bin_expr.left_expr);
      auto r_expr_type_node = get_expr_type(bin_expr.right_expr);

      // We check they have compatible types
      return check_type_compat(l_expr_type_node, r_expr_type_node, in_expr);
    }
    default:
      LL_UNREACHEABLE;
    }
  }
  case AstNodeType::AstUnaryExpr: {
    const AstUnaryExpr &unary_expr = in_expr->unary_expr;
    auto type_node = get_expr_type(unary_expr.expr);

    if (!type_node) {
      return false;
    }

    // Overloading operators is not supported
    if (type_node->ast_type.type_id == AstTypeId::Struct) {
      auto operator_symbol = get_unary_op_symbol(unary_expr.op);
      add_semantic_error(in_expr, ERROR_UNSUPORTED_UNARY_OP_STRUCT_EXPR, operator_symbol.c_str());
      return false;
    }
    // If it is a boolean expr only the NOT operator is valid
    if (type_node->ast_type.type_id == AstTypeId::Bool && unary_expr.op != UnaryExprType::NOT) {
      auto operator_symbol = get_unary_op_symbol(unary_expr.op);
      add_semantic_error(in_expr, ERROR_UNSUPORTED_OP_BOOL_EXPR, operator_symbol.c_str());
      return false;
    }
    // If it is not boolean expr the NOT operator is invalid
    if (type_node->ast_type.type_id != AstTypeId::Bool && unary_expr.op == UnaryExprType::NOT) {
      add_semantic_error(in_expr, ERROR_UNSUPORTED_OP_NOT_BOOL_EXPR);
      return false;
    }

    return analizeExpr(unary_expr.expr);
  }
  case AstNodeType::AstFuncCallExpr: {
    const AstFuncCallExpr &fn_call = in_expr->func_call;
    SymbolType symbol_type;
    fn_call.fn_ref = resolve_function_variable(std::string(fn_call.fn_name), in_expr, &symbol_type);

    if (symbol_type != SymbolType::FUNC) {
      add_semantic_error(in_expr, ERROR_SYMBOL_NOT_A_FN, fn_call.fn_name);
      return false;
    }

    if (!fn_call.fn_ref) {
      add_semantic_error(in_expr, ERROR_UNDECLARED_FN, fn_call.fn_name);
      return false;
    }

    auto &fn_proto = fn_call.fn_ref->function_proto;
    if (fn_call.params.size() != fn_proto.params.size()) {
      auto params_size = fn_proto.params.size();
      auto args_size = fn_call.params.size();

      // BUG(pablo96): len1 != len2
      add_semantic_error(in_expr, ERROR_ARGUMENT_COUNT_MISMATCH, params_size, args_size);
      return false;
    }

    auto err_size_before = errors.size();

    size_t i = 0;
    for (auto arg : fn_call.params) {
      if (!analizeExpr(arg))
        continue;

      auto arg_type = get_expr_type(arg);
      auto param = fn_proto.params.at(i);
      check_type_compat(param->param_decl.type, arg_type, in_expr);
    }

    auto err_size_after = errors.size();
    return err_size_after == err_size_before;
  }
  case AstNodeType::AstSymbol: {
    auto name = std::string(in_expr->symbol.cached_name);
    auto data = in_expr->symbol.data = resolve_function_variable(name, in_expr, &in_expr->symbol.type);
    return data != nullptr;
  }
  case AstNodeType::AstConstValue: {
    switch (in_expr->const_value.type) {
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

const AstNode *SemanticAnalyzer::resolve_function_variable(const std::string &in_name, const AstNode *in_parent_node,
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

  add_semantic_error(in_parent_node, ERROR_UNKNOWN_SYMBOL, in_name.c_str());

  return nullptr;
}

void SemanticAnalyzer::add_semantic_error(const AstNode *in_node, const char *in_msg, ...) {
  va_list ap, ap2;
  va_start(ap, in_msg);
  va_copy(ap2, ap);

  int len1 = snprintf(nullptr, 0, in_msg, ap);
  assert(len1 >= 0);

  std::string msg;
  msg.reserve(len1 + 1);

  int len2 = snprintf(msg.data(), msg.capacity(), in_msg, ap2);
  assert(len2 >= 0);
  // assert(len2 == len1);

  Error error(ERROR_TYPE::ERROR, in_node->line, in_node->column, in_node->file_name, msg);

  errors.push_back(error);

  va_end(ap);
  va_end(ap2);
}

bool SemanticAnalyzer::check_type_compat(const AstNode *type_node0, const AstNode *type_node1,
                                         const AstNode *expr_node) {
  assert(type_node0 != nullptr);
  assert(type_node1 != nullptr);
  assert(type_node0->node_type == AstNodeType::AstType);
  assert(type_node1->node_type == AstNodeType::AstType);
  assert(expr_node->node_type != AstNodeType::AstType);

  const AstType &type0 = type_node0->ast_type;
  const AstType &type1 = type_node1->ast_type;

  if (type1.type_id == type0.type_id) {
    if (type1.type_id == AstTypeId::Pointer || type1.type_id == AstTypeId::Array)
      return check_type_compat(type0.child_type, type1.child_type, expr_node);
    if (type0.type_id == AstTypeId::Struct)
      // NOTE: Structs No Supported
      // TODO: Add support for structs
      LL_UNREACHEABLE;
    return true;
  }

  // incompatible types
  // void | bool
  // int  | bool
  // int  | void
  // int  | float
  // int  | pointer
  // int  | array
  // float| bool
  // float| void
  // float| array
  // float| pointer
  // array| pointer
  add_semantic_error(expr_node, ERROR_TYPES_MISMATCH);

  return false;
}

const AstNode *SemanticAnalyzer::get_expr_type(const AstNode *expr) {
  assert(expr != nullptr);

  switch (expr->node_type) {
  case AstNodeType::AstBinaryExpr: {
    auto bin_expr = expr->binary_expr;
    switch (bin_expr.bin_op) {
    case BinaryExprType::EQUALS:
    case BinaryExprType::NOT_EQUALS:
    case BinaryExprType::GREATER:
    case BinaryExprType::LESS:
    case BinaryExprType::GREATER_OR_EQUALS:
    case BinaryExprType::LESS_OR_EQUALS:
      return get_type_node("bool");
    default:
      auto type0 = get_expr_type(bin_expr.left_expr);
      auto type1 = get_expr_type(bin_expr.right_expr);
      if (check_type_compat(type0, type1, expr))
        return get_best_type(type0, type1);
      return nullptr;
    }
    LL_UNREACHEABLE;
  }
  case AstNodeType::AstUnaryExpr: {
    auto unary_expr = expr->unary_expr;
    switch (unary_expr.op) {
    case UnaryExprType::NOT:
      return get_type_node("bool");
    default:
      return get_expr_type(unary_expr.expr);
    }
  }
  case AstNodeType::AstFuncCallExpr: {
    auto func_node = resolve_function_variable(std::string(expr->func_call.fn_name), expr);
    return get_expr_type(func_node);
  }
  case AstNodeType::AstFuncDef: {
    auto proto_node = expr->function_def.proto->function_proto;
    return proto_node.return_type;
  }
  case AstNodeType::AstFuncProto: {
    auto proto_node = expr->function_proto;
    return proto_node.return_type;
  }
  case AstNodeType::AstSymbol: {
    // TODO(pablo96): should make it posible to resolve function names as func pointers
    auto name = std::string(expr->symbol.cached_name);
    auto var_node = resolve_function_variable(name, expr);
    if (!var_node) {
      return nullptr;
    }
    if (var_node->node_type == AstNodeType::AstVarDef) {
      return var_node->var_def.type;
    }
    if (var_node->node_type == AstNodeType::AstParamDecl) {
      return var_node->param_decl.type;
    }
    if (var_node->node_type == AstNodeType::AstFuncDef) {
      return var_node->function_def.proto->function_proto.return_type;
    }
    if (var_node->node_type == AstNodeType::AstFuncProto) {
      return var_node->function_proto.return_type;
    }
    LL_UNREACHEABLE;
  }
  case AstNodeType::AstConstValue: {
    auto const_value = expr->const_value;
    switch (const_value.type) {
    case ConstValueType::BOOL:
      return get_type_node("bool");
    case ConstValueType::FLOAT:
      return get_type_node("f128");
    case ConstValueType::INT:
      if (const_value.is_negative)
        return get_type_node("i128");
      return get_type_node("u128");
    case ConstValueType::CHAR:
      return get_type_node("u32");
    default:
      LL_UNREACHEABLE;
    }
  }
  default:
    LL_UNREACHEABLE;
  }
}

bool is_ret_stmnt(const AstNode *stmnt) {
  return stmnt->node_type == AstNodeType::AstUnaryExpr && stmnt->unary_expr.op == UnaryExprType::RET;
}

const AstNode *get_best_type(const AstNode *type_node0, const AstNode *type_node1) {
  assert(type_node0 != nullptr);
  assert(type_node1 != nullptr);
  assert(type_node0->node_type == AstNodeType::AstType);
  assert(type_node1->node_type == AstNodeType::AstType);

  const AstType &type0 = type_node0->ast_type;
  const AstType &type1 = type_node1->ast_type;

  if (type1.type_id == AstTypeId::Pointer || type1.type_id == AstTypeId::Array) {
    return get_best_type(type0.child_type, type1.child_type);
  }

  return type0.type_info->bit_size > type1.type_info->bit_size ? type_node0 : type_node1;
}

void set_type_info(const AstNode *expr_node, const AstNode *type_node) {
  assert(expr_node != nullptr);
  assert(expr_node->node_type != AstNodeType::AstType);
  assert(type_node != nullptr);
  assert(type_node->node_type == AstNodeType::AstType);

  switch (expr_node->node_type) {
  case AstNodeType::AstConstValue:
    if (type_node->ast_type.type_id == AstTypeId::Array || type_node->ast_type.type_id == AstTypeId::Pointer ||
        type_node->ast_type.type_id == AstTypeId::Struct || type_node->ast_type.type_id == AstTypeId::Void)
      LL_UNREACHEABLE;
    expr_node->const_value.bit_size = type_node->ast_type.type_info->bit_size;
    return;
  default:
    return;
  }
}