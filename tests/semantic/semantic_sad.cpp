#include "../../src/Types.hpp"
#include "../../src/ast_nodes.hpp"
#include "../../src/error.hpp"
#include "../../src/semantic_analyzer.hpp"

#include <gtest/gtest.h>

//==================================================================================
//          SEMANTIC TYPES ANALYSIS
//==================================================================================

TEST(SemanticTypes, DistinctTypes) {
  std::vector<Error> errors;
  TypesRepository types_repository = TypesRepository::get();

  auto node_expr = new AstNode(AstNodeType::AST_VAR_DEF, 0, 0, "");

  auto node_type_0 = types_repository.get_type_node("bool");
  auto node_type_1 = types_repository.get_type_node("i32");

  bool is_ok = check_types(errors, node_type_0, node_type_1, node_expr);

  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_ok);
}

TEST(SemanticTypes, PointerDistinctTypes) {
  std::vector<Error> errors;
  TypesRepository types_repository = TypesRepository::get();

  AstType *node_child_type_0 = types_repository.get_type_node("i32");
  AstType *node_child_type_1 = types_repository.get_type_node("f32");

  AstVarDef *node_expr = new AstVarDef(0, 0, "");

  AstType *node_type_0 = types_repository.get_type_node("pointer");
  node_type_0->child_type = node_child_type_0;

  AstType *node_type_1 = types_repository.get_type_node("pointer");
  node_type_1->child_type = node_child_type_1;

  bool is_ok = check_types(errors, node_type_0, node_type_1, node_expr);

  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_ok);

  delete node_expr;
  delete node_type_0;
  delete node_type_1;
}

TEST(SemanticTypes, ArrayDistinctTypes) {
  std::vector<Error> errors;
  TypesRepository types_repository = TypesRepository::get();

  AstVarDef *node_expr = new AstVarDef(0, 0, "");

  AstType *node_child_type_0 = types_repository.get_type_node("i32");
  AstType *node_child_type_1 = types_repository.get_type_node("f32");

  AstType *node_type_0 = types_repository.get_type_node("array");
  node_type_0->child_type = node_child_type_0;

  AstType *node_type_1 = types_repository.get_type_node("array");
  node_type_1->child_type = node_child_type_1;

  bool is_ok = check_types(errors, node_type_0, node_type_1, node_expr);

  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_ok);

  delete node_type_0;
  delete node_type_1;
}

//==================================================================================
//          SEMANTIC VARIABLES DEFINITION
//==================================================================================

TEST(SemanticVariableDefinitions, GlobalVariableNoInit) {
  std::vector<Error> errors;
  TypesRepository types_repository = TypesRepository::get();

  AstType *i32_type_node = types_repository.get_type_node("i32");

  AstVarDef *var_def_node = new AstVarDef(0, 0, "");
  var_def_node->type = i32_type_node;
  var_def_node->name = "global_var";
  var_def_node->initializer = nullptr;

  SemanticAnalyzer analizer(errors);
  bool is_valid = analizer.analize_global_var_def(var_def_node);

  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);

  delete var_def_node;
}

TEST(SemanticVariableDefinitions, LocalVariableTypeMismatch) {
  std::vector<Error> errors;
  TypesRepository types_repository = TypesRepository::get();

  AstType *f32_type_node = types_repository.get_type_node("f32");

  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::INT;
  const_value_node->number = "0";
  const_value_node->is_negative = false;

  AstVarDef *var_def_node = new AstVarDef(0, 0, "");
  var_def_node->type = f32_type_node;
  var_def_node->name = "local_var";
  var_def_node->initializer = const_value_node;

  SemanticAnalyzer analizer(errors);
  bool is_valid = analizer.analize_var_def(var_def_node);

  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);

  delete var_def_node;
}

//==================================================================================
//          SEMANTIC EXPRESSIONS
//==================================================================================

TEST(SemanticExpressions, UnknownConstantValue) {
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType(-1);
  const_value_node->number = "0";
  const_value_node->is_negative = false;

  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);
  ASSERT_DEATH(analizer.analize_expr(const_value_node), "");

  delete const_value_node;
}

TEST(SemanticExpressions, ResolveUnknownVariableSymbol) {
  // given: symbol node
  std::string var_name = "global_var";
  AstSymbol *symbol_node = new AstSymbol(0, 0, "");
  symbol_node->cached_name = std::string_view(var_name.data(), var_name.size());

  // given: configured analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  bool is_valid = analizer.analize_expr(symbol_node);

  // then:
  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);

  delete symbol_node;
}

TEST(SemanticExpressions, UnaryExprBoolWrongOp) {
  // given: expr
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::BOOL;
  const_value_node->boolean = false;

  // given: unary expr
  AstUnaryExpr *unary_epxr_node = new AstUnaryExpr(0, 0, "");
  unary_epxr_node->op = UnaryExprType::NEG;
  unary_epxr_node->expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: a bool expresion
  // with: a unary operator valid for the expr type
  bool is_valid = analizer.analize_expr(unary_epxr_node);

  // then:
  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);

  delete unary_epxr_node;
}

TEST(SemanticExpressions, UnaryExprNumberWrongOp) {
  // given: expr
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::INT;
  const_value_node->number = "0";
  const_value_node->is_negative = false;

  // given: unary expr
  AstUnaryExpr *unary_epxr_node = new AstUnaryExpr(0, 0, "");
  unary_epxr_node->op = UnaryExprType::NOT;
  unary_epxr_node->expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: a number expresion
  // with: a unary operator valid for the expr type
  bool is_valid = analizer.analize_expr(unary_epxr_node);

  // then:
  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);

  delete unary_epxr_node;
}

TEST(SemanticExpressions, BinaryExprBitShiftNoIntRExpr) {
  // given: expr
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::FLOAT;

  // given: biary expr
  AstBinaryExpr *binary_epxr_node = new AstBinaryExpr(0, 0, "");
  // bin_expr -> 1 << 1
  binary_epxr_node->bin_op = BinaryExprType::LSHIFT;
  binary_epxr_node->left_expr = const_value_node;
  binary_epxr_node->right_expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: a number expresion
  // with: a unary operator valid for the expr type
  bool is_valid = analizer.analize_expr(binary_epxr_node);

  // then:
  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);

  delete binary_epxr_node;
}

TEST(SemanticExpressions, BinaryExprBitShiftUnknownSymbol) {
  // given: l_expr -> unkown symbol node
  AstSymbol *symbol_node = new AstSymbol(0, 0, "");
  symbol_node->cached_name = "var_name";

  // given: r_expr
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::INT;

  // given: biary expr -> var_name << 1
  AstBinaryExpr *binary_expr_node = new AstBinaryExpr(0, 0, "");
  binary_expr_node->bin_op = BinaryExprType::LSHIFT;
  binary_expr_node->left_expr = symbol_node;
  binary_expr_node->right_expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: a number expresion
  // with: a unary operator valid for the expr type
  bool is_valid = analizer.analize_expr(binary_expr_node);

  // then:
  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);

  delete binary_expr_node;
}

TEST(SemanticExpressions, BinaryExprBoolOperatorWrongExpr) {
  // given: l_expr -> unkown symbol node
  AstSymbol *symbol_node = new AstSymbol(0, 0, "");
  symbol_node->cached_name = "var_name";

  // given: r_expr -> constant integer
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::INT;

  // given: biary expr -> my_var == SOME_INT
  AstBinaryExpr *binary_expr_node = new AstBinaryExpr(0, 0, "");
  binary_expr_node->bin_op = BinaryExprType::EQUALS;
  binary_expr_node->left_expr = symbol_node;
  binary_expr_node->right_expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: symbol as left expr
  // with: constant as right expr
  bool is_valid = analizer.analize_expr(binary_expr_node);

  // then:
  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);

  delete binary_expr_node;
}

TEST(SemanticExpressions, BinaryExprAssignOperatorTypesMismatch) {
  TypesRepository types_repository = TypesRepository::get();

  // given: variable definition
  AstType *i32_type_node = types_repository.get_type_node("i32");

  AstVarDef *var_def_node = new AstVarDef(0, 0, "");
  var_def_node->type = i32_type_node;
  auto var_name = var_def_node->name = "my_var";

  // given: l_expr -> symbol node
  AstSymbol *symbol_node = new AstSymbol(0, 0, "");
  symbol_node->cached_name = std::string_view(var_name.data(), var_name.size());

  // given: r_expr -> constant integer
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::FLOAT;

  // given: biary expr -> my_var = SOME_INT
  AstBinaryExpr *binary_expr_node = new AstBinaryExpr(0, 0, "");
  binary_expr_node->bin_op = BinaryExprType::ASSIGN;
  binary_expr_node->left_expr = symbol_node;
  binary_expr_node->right_expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);
  bool is_valid_var_def = analizer.analize_var_def(var_def_node);

  // when: call to analize_expr
  // with: symbol as left expr
  // with: constant as right expr
  bool is_valid = analizer.analize_expr(binary_expr_node);

  // then:
  ASSERT_EQ(is_valid_var_def, true);
  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);

  delete var_def_node;
  delete binary_expr_node;
}

TEST(SemanticExpressions, BinaryExprAssignOperatorWrongExpr) {
  // given: l_expr -> symbol node
  AstSymbol *symbol_node = new AstSymbol(0, 0, "");
  symbol_node->cached_name = "my_var";

  // given: r_expr -> constant integer
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::FLOAT;

  // given: biary expr -> my_var = SOME_INT
  AstBinaryExpr *binary_expr_node = new AstBinaryExpr(0, 0, "");
  binary_expr_node->bin_op = BinaryExprType::ASSIGN;
  binary_expr_node->left_expr = symbol_node;
  binary_expr_node->right_expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: symbol as left expr
  // with: constant as right expr
  bool is_valid = analizer.analize_expr(binary_expr_node);

  // then:
  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);

  delete binary_expr_node;
}

//==================================================================================
//          SEMANTIC FUNCTIONS
//==================================================================================

TEST(SemanticFunctions, FunctionInvalidStmnt) {
  TypesRepository types_repository = TypesRepository::get();

  // given: l_expr -> symbol node
  AstSymbol *symbol_node = new AstSymbol(0, 0, "");
  symbol_node->cached_name = "my_var";

  // given: r_expr -> constant integer
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::INT;

  // given: biary expr -> my_var = SOME_INT
  AstBinaryExpr *binary_expr_node = new AstBinaryExpr(0, 0, "");
  binary_expr_node->bin_op = BinaryExprType::ASSIGN;
  binary_expr_node->left_expr = symbol_node;
  binary_expr_node->right_expr = const_value_node;

  // given: function proto -> fn my_func() void
  AstFnProto *function_proto_node = new AstFnProto(0, 0, "");
  function_proto_node->name = "my_func";
  function_proto_node->return_type = types_repository.get_type_node("void");

  // given: function block
  AstBlock *function_block_node = new AstBlock(0, 0, "");
  function_block_node->statements.push_back(binary_expr_node);

  // given: function
  AstFnDef *function_node = new AstFnDef(0, 0, "");
  function_node->proto = function_proto_node;
  function_node->block = function_block_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  bool is_valid_proto = analizer.analize_fn_proto(function_proto_node);
  analizer.enter_fn_scope(function_node);
  bool is_valid = analizer.analize_fn_block(function_block_node);
  analizer.exit_fn_scope();

  // then:
  ASSERT_TRUE(is_valid_proto);
  ASSERT_FALSE(is_valid);
  ASSERT_GT(errors.size(), 0L);

  delete function_node;
}

TEST(SemanticFunctions, FunctionNoReqRet) {
  TypesRepository types_repository = TypesRepository::get();

  // given: variable definition
  AstType *i32_type_node = types_repository.get_type_node("i32");

  AstVarDef *var_def_node = new AstVarDef(0, 0, "");
  var_def_node->type = i32_type_node;
  auto var_name = var_def_node->name = "my_var";

  // given: l_expr -> symbol node
  AstSymbol *symbol_node = new AstSymbol(0, 0, "");
  symbol_node->cached_name = std::string_view(var_name.data(), var_name.size());

  // given: r_expr -> constant integer
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::INT;

  // given: binary expr -> my_var = SOME_INT
  AstBinaryExpr *binary_epxr_node = new AstBinaryExpr(0, 0, "");
  binary_epxr_node->bin_op = BinaryExprType::ASSIGN;
  binary_epxr_node->left_expr = symbol_node;
  binary_epxr_node->right_expr = const_value_node;

  // given: function proto -> fn my_func() i32
  AstFnProto *function_proto_node = new AstFnProto(0, 0, "");
  function_proto_node->name = "my_func";
  function_proto_node->return_type = i32_type_node;

  // given: function block
  AstBlock *function_block_node = new AstBlock(0, 0, "");
  function_block_node->statements.push_back(var_def_node);
  function_block_node->statements.push_back(binary_epxr_node);

  // given: function
  AstFnDef *function_node = new AstFnDef(0, 0, "");
  function_node->proto = function_proto_node;
  function_node->block = function_block_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  bool is_valid_proto = analizer.analize_fn_proto(function_proto_node);
  analizer.enter_fn_scope(function_node);
  bool is_valid = analizer.analize_fn_block(function_block_node);
  analizer.exit_fn_scope();

  // then:
  ASSERT_TRUE(is_valid_proto);
  ASSERT_FALSE(is_valid);
  ASSERT_GT(errors.size(), 0L);

  delete function_node;
}

//==================================================================================
//          SEMANTIC FUNCTION CALLS
//==================================================================================

TEST(SemanticFunctionsCalls, FunctionCallUndeclaredFn) {
  AstFnCallExpr *function_call_node = new AstFnCallExpr(0, 0, "");
  function_call_node->fn_name = "my_func";
  function_call_node->fn_ref = nullptr;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  bool is_valid_call = analizer.analize_expr(function_call_node);

  // then:
  ASSERT_FALSE(is_valid_call);
  ASSERT_GT(errors.size(), 0L);

  delete function_call_node;
}

TEST(SemanticFunctionsCalls, FunctionCallSymbolNotAFn) {
  TypesRepository types_repository = TypesRepository::get();

  AstFnCallExpr *function_call_node = new AstFnCallExpr(0, 0, "");
  function_call_node->fn_name = "my_var";
  function_call_node->fn_ref = nullptr;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  bool is_valid_call = analizer.analize_expr(function_call_node);

  // then:
  ASSERT_FALSE(is_valid_call);
  ASSERT_GT(errors.size(), 0L);

  delete function_call_node;
}

TEST(SemanticFunctionsCalls, FunctionCallParamsCountMismatch) {
  TypesRepository types_repository = TypesRepository::get();

  // given: variable definition
  AstType *void_type_node = types_repository.get_type_node("void");

  // given: arg -> constant integer
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::INT;

  // given: function proto -> fn my_func() void
  AstFnProto *function_proto_node = new AstFnProto(0, 0, "");
  function_proto_node->name = "my_func";
  function_proto_node->return_type = void_type_node;

  // given: function block
  AstBlock *function_block_node = new AstBlock(0, 0, "");

  // given: function
  AstFnDef *function_node = new AstFnDef(0, 0, "");
  function_node->proto = function_proto_node;
  function_node->block = function_block_node;

  AstFnCallExpr *function_call_node = new AstFnCallExpr(0, 0, "");
  function_call_node->fn_name = "my_func";
  function_call_node->fn_ref = nullptr;
  function_call_node->args.push_back(const_value_node);

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  bool is_valid_proto = analizer.analize_fn_proto(function_proto_node);
  analizer.enter_fn_scope(function_node);
  bool is_valid_block = analizer.analize_fn_block(function_block_node);
  analizer.exit_fn_scope();
  bool is_valid_call = analizer.analize_expr(function_call_node);

  // then:
  ASSERT_TRUE(is_valid_proto);
  ASSERT_TRUE(is_valid_block);
  ASSERT_FALSE(is_valid_call);
  ASSERT_GT(errors.size(), 0L);

  delete function_call_node;
  delete function_node;
}

TEST(SemanticFunctionsCalls, FunctionCallParamsTypeMismatch) {
  TypesRepository types_repository = TypesRepository::get();

  // given: types
  AstType *void_type_node = types_repository.get_type_node("void");
  AstType *i32_type_node = types_repository.get_type_node("i32");

  // given: arg -> constant integer
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::FLOAT;

  // given: param -> param1 i32
  AstParamDef *param_node = new AstParamDef(0, 0, "");
  param_node->name = "param1";
  param_node->type = i32_type_node;
  param_node->initializer = nullptr;

  // given: function proto -> fn my_func(param1 i32) void
  AstFnProto *function_proto_node = new AstFnProto(0, 0, "");
  function_proto_node->name = "my_func";
  function_proto_node->return_type = void_type_node;
  function_proto_node->params.push_back(param_node);

  // given: empty function block
  AstBlock *function_block_node = new AstBlock(0, 0, "");

  // given: empty function
  AstFnDef *function_node = new AstFnDef(0, 0, "");
  function_node->proto = function_proto_node;
  function_node->block = function_block_node;

  // given: call with argument f32 constant
  AstFnCallExpr *function_call_node = new AstFnCallExpr(0, 0, "");
  function_call_node->fn_name = "my_func";
  function_call_node->fn_ref = nullptr;
  function_call_node->args.push_back(const_value_node);

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  bool is_valid_proto = analizer.analize_fn_proto(function_proto_node);
  analizer.enter_fn_scope(function_node);
  bool is_valid_block = analizer.analize_fn_block(function_block_node);
  analizer.exit_fn_scope();
  bool is_valid_call = analizer.analize_expr(function_call_node);

  // then:
  ASSERT_TRUE(is_valid_proto);
  ASSERT_TRUE(is_valid_block);
  ASSERT_FALSE(is_valid_call);
  ASSERT_GT(errors.size(), 0L);

  delete function_node;
  delete function_call_node;
}
