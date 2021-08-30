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
  auto node_expr = new AstNode(AstNodeType::AstVarDef, 0, 0, "");

  auto node_type_0 = get_type_node("bool");
  auto node_type_1 = get_type_node("i32");

  bool is_ok = check_types(errors, node_type_0, node_type_1, node_expr);

  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_ok);
}

TEST(SemanticTypes, PointerDistinctTypes) {
  std::vector<Error> errors;
  auto node_expr = new AstNode(AstNodeType::AstVarDef, 0, 0, "");

  auto node_child_type_0 = get_type_node("i32");

  auto node_type_0 = new AstNode(AstNodeType::AstType, 0, 0, "");
  node_type_0->ast_type.type_id = AstTypeId::Pointer;
  node_type_0->ast_type.child_type = node_child_type_0;

  auto node_child_type_1 = get_type_node("f32");

  auto node_type_1 = new AstNode(AstNodeType::AstType, 0, 0, "");
  node_type_1->ast_type.type_id = AstTypeId::Pointer;
  node_type_1->ast_type.child_type = node_child_type_1;

  bool is_ok = check_types(errors, node_type_0, node_type_1, node_expr);

  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_ok);
}

TEST(SemanticTypes, ArrayDistinctTypes) {
  std::vector<Error> errors;
  auto node_expr = new AstNode(AstNodeType::AstVarDef, 0, 0, "");

  auto node_child_type_0 = get_type_node("i32");

  auto node_type_0 = new AstNode(AstNodeType::AstType, 0, 0, "");
  node_type_0->ast_type.type_id = AstTypeId::Array;
  node_type_0->ast_type.child_type = node_child_type_0;

  auto node_child_type_1 = get_type_node("f32");

  auto node_type_1 = new AstNode(AstNodeType::AstType, 0, 0, "");
  node_type_1->ast_type.type_id = AstTypeId::Array;
  node_type_1->ast_type.child_type = node_child_type_1;

  bool is_ok = check_types(errors, node_type_0, node_type_1, node_expr);

  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_ok);
}

//==================================================================================
//          SEMANTIC VARIABLES DEFINITION
//==================================================================================

TEST(SemanticVariableDefinitions, GlobalVariableNoInit) {
  bool is_global = true;
  std::vector<Error> errors;

  auto i32_type_node = get_type_node("i32");

  auto var_def_node = new AstNode(AstNodeType::AstVarDef, 0, 0, "");
  var_def_node->var_def.type = i32_type_node;
  var_def_node->var_def.name = "global_var";
  var_def_node->var_def.initializer = nullptr;

  SemanticAnalyzer analizer(errors);
  bool is_valid = analizer.analizeVarDef(var_def_node, is_global);

  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);
}

TEST(SemanticVariableDefinitions, LocalVariableTypeMismatch) {
  bool is_global = false;
  std::vector<Error> errors;

  auto f32_type_node = get_type_node("f32");

  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;
  const_value_node->const_value.number = "0";
  const_value_node->const_value.is_negative = false;

  auto var_def_node = new AstNode(AstNodeType::AstVarDef, 0, 0, "");
  var_def_node->var_def.type = f32_type_node;
  var_def_node->var_def.name = "local_var";
  var_def_node->var_def.initializer = const_value_node;

  SemanticAnalyzer analizer(errors);
  bool is_valid = analizer.analizeVarDef(var_def_node, is_global);

  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);
}

//==================================================================================
//          SEMANTIC EXPRESSIONS
//==================================================================================

TEST(SemanticExpressions, UnknownConstantValue) {
  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType(-1);
  const_value_node->const_value.number = "0";
  const_value_node->const_value.is_negative = false;

  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);
  ASSERT_DEATH(analizer.analizeExpr(const_value_node), "");
}

TEST(SemanticExpressions, ResolveUnknownVariableSymbol) {
  // given: symbol node
  std::string var_name = "global_var";
  auto symbol_node = new AstNode(AstNodeType::AstSymbol, 0, 0, "");
  symbol_node->symbol.cached_name = std::string_view(var_name.data(), var_name.size());

  // given: configured analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  bool is_valid = analizer.analizeExpr(symbol_node);

  // then:
  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);
}

TEST(SemanticExpressions, UnaryExprBoolWrongOp) {
  // given: expr
  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::BOOL;
  const_value_node->const_value.boolean = false;

  // given: unary expr
  auto unary_epxr_node = new AstNode(AstNodeType::AstUnaryExpr, 0, 0, "");
  unary_epxr_node->unary_expr.op = UnaryExprType::NEG;
  unary_epxr_node->unary_expr.expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: a bool expresion
  // with: a unary operator valid for the expr type
  bool is_valid = analizer.analizeExpr(unary_epxr_node);

  // then:
  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);
}

TEST(SemanticExpressions, UnaryExprNumberWrongOp) {
  // given: expr
  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;
  const_value_node->const_value.number = "0";
  const_value_node->const_value.is_negative = false;

  // given: unary expr
  auto unary_epxr_node = new AstNode(AstNodeType::AstUnaryExpr, 0, 0, "");
  unary_epxr_node->unary_expr.op = UnaryExprType::NOT;
  unary_epxr_node->unary_expr.expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: a number expresion
  // with: a unary operator valid for the expr type
  bool is_valid = analizer.analizeExpr(unary_epxr_node);

  // then:
  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);
}

TEST(SemanticExpressions, BinaryExprBitShiftNoIntRExpr) {
  // given: expr
  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::FLOAT;

  // given: biary expr
  auto binary_epxr_node = new AstNode(AstNodeType::AstBinaryExpr, 0, 0, "");
  // bin_expr -> 1 << 1
  binary_epxr_node->binary_expr.bin_op = BinaryExprType::LSHIFT;
  binary_epxr_node->binary_expr.left_expr = const_value_node;
  binary_epxr_node->binary_expr.right_expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: a number expresion
  // with: a unary operator valid for the expr type
  bool is_valid = analizer.analizeExpr(binary_epxr_node);

  // then:
  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);
}

TEST(SemanticExpressions, BinaryExprBitShiftUnknownSymbol) {
  // given: l_expr -> unkown symbol node
  auto symbol_node = new AstNode(AstNodeType::AstSymbol, 0, 0, "");
  symbol_node->symbol.cached_name = "var_name";

  // given: r_expr
  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;

  // given: biary expr -> var_name << 1
  auto binary_epxr_node = new AstNode(AstNodeType::AstBinaryExpr, 0, 0, "");
  binary_epxr_node->binary_expr.bin_op = BinaryExprType::LSHIFT;
  binary_epxr_node->binary_expr.left_expr = symbol_node;
  binary_epxr_node->binary_expr.right_expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: a number expresion
  // with: a unary operator valid for the expr type
  bool is_valid = analizer.analizeExpr(binary_epxr_node);

  // then:
  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);
}

TEST(SemanticExpressions, BinaryExprBoolOperatorWrongExpr) {
  // given: l_expr -> unkown symbol node
  auto symbol_node = new AstNode(AstNodeType::AstSymbol, 0, 0, "");
  symbol_node->symbol.cached_name = "var_name";

  // given: r_expr -> constant integer
  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;

  // given: biary expr -> my_var == SOME_INT
  auto binary_epxr_node = new AstNode(AstNodeType::AstBinaryExpr, 0, 0, "");
  binary_epxr_node->binary_expr.bin_op = BinaryExprType::EQUALS;
  binary_epxr_node->binary_expr.left_expr = symbol_node;
  binary_epxr_node->binary_expr.right_expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: symbol as left expr
  // with: constant as right expr
  bool is_valid = analizer.analizeExpr(binary_epxr_node);

  // then:
  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);
}

TEST(SemanticExpressions, BinaryExprAssignOperatorTypesMismatch) {
  // given: variable definition
  auto i32_type_node = get_type_node("i32");

  auto var_def_node = new AstNode(AstNodeType::AstVarDef, 0, 0, "");
  var_def_node->var_def.type = i32_type_node;
  auto var_name = var_def_node->var_def.name = "my_var";

  // given: l_expr -> symbol node
  auto symbol_node = new AstNode(AstNodeType::AstSymbol, 0, 0, "");
  symbol_node->symbol.cached_name = std::string_view(var_name.data(), var_name.size());

  // given: r_expr -> constant integer
  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::FLOAT;

  // given: biary expr -> my_var = SOME_INT
  auto binary_epxr_node = new AstNode(AstNodeType::AstBinaryExpr, 0, 0, "");
  binary_epxr_node->binary_expr.bin_op = BinaryExprType::ASSIGN;
  binary_epxr_node->binary_expr.left_expr = symbol_node;
  binary_epxr_node->binary_expr.right_expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);
  bool is_valid_var_def = analizer.analizeVarDef(var_def_node, false);

  // when: call to analize_expr
  // with: symbol as left expr
  // with: constant as right expr
  bool is_valid = analizer.analizeExpr(binary_epxr_node);

  // then:
  ASSERT_EQ(is_valid_var_def, true);
  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);
}

TEST(SemanticExpressions, BinaryExprAssignOperatorWrongExpr) {
  // given: l_expr -> symbol node
  auto symbol_node = new AstNode(AstNodeType::AstSymbol, 0, 0, "");
  symbol_node->symbol.cached_name = "my_var";

  // given: r_expr -> constant integer
  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::FLOAT;

  // given: biary expr -> my_var = SOME_INT
  auto binary_epxr_node = new AstNode(AstNodeType::AstBinaryExpr, 0, 0, "");
  binary_epxr_node->binary_expr.bin_op = BinaryExprType::ASSIGN;
  binary_epxr_node->binary_expr.left_expr = symbol_node;
  binary_epxr_node->binary_expr.right_expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: symbol as left expr
  // with: constant as right expr
  bool is_valid = analizer.analizeExpr(binary_epxr_node);

  // then:
  ASSERT_EQ(errors.size(), 1L);
  ASSERT_FALSE(is_valid);
}

//==================================================================================
//          SEMANTIC FUNCTIONS
//==================================================================================

TEST(SemanticExpressions, FunctionInvalidStmnt) {
  // given: variable definition
  auto i32_type_node = get_type_node("i32");

  // given: l_expr -> symbol node
  auto symbol_node = new AstNode(AstNodeType::AstSymbol, 0, 0, "");
  symbol_node->symbol.cached_name = "my_var";

  // given: r_expr -> constant integer
  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;

  // given: biary expr -> my_var = SOME_INT
  auto binary_epxr_node = new AstNode(AstNodeType::AstBinaryExpr, 0, 0, "");
  binary_epxr_node->binary_expr.bin_op = BinaryExprType::ASSIGN;
  binary_epxr_node->binary_expr.left_expr = symbol_node;
  binary_epxr_node->binary_expr.right_expr = const_value_node;

  // given: function proto -> fn my_func() void
  auto function_proto_node = new AstNode(AstNodeType::AstFuncProto, 0, 0, "");
  function_proto_node->function_proto.name = "my_func";
  function_proto_node->function_proto.return_type = get_type_node("void");

  // given: function block
  auto function_block_node = new AstNode(AstNodeType::AstBlock, 0, 0, "");
  function_block_node->block.statements.push_back(binary_epxr_node);

  // given: function
  auto function_node = new AstNode(AstNodeType::AstBinaryExpr, 0, 0, "");
  function_node->function_def.proto = function_proto_node;
  function_node->function_def.block = function_block_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  bool is_valid_proto = analizer.analizeFuncProto(function_proto_node);
  bool is_valid = analizer.analizeFuncBlock(function_block_node->block, function_node->function_def);

  // then:
  ASSERT_TRUE(is_valid_proto);
  ASSERT_FALSE(is_valid);
  ASSERT_GT(errors.size(), 0L);
}

TEST(SemanticExpressions, FunctionNoReqRet) {
  // given: variable definition
  auto i32_type_node = get_type_node("i32");

  auto var_def_node = new AstNode(AstNodeType::AstVarDef, 0, 0, "");
  var_def_node->var_def.type = i32_type_node;
  auto var_name = var_def_node->var_def.name = "my_var";

  // given: l_expr -> symbol node
  auto symbol_node = new AstNode(AstNodeType::AstSymbol, 0, 0, "");
  symbol_node->symbol.cached_name = std::string_view(var_name.data(), var_name.size());

  // given: r_expr -> constant integer
  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;

  // given: binary expr -> my_var = SOME_INT
  auto binary_epxr_node = new AstNode(AstNodeType::AstBinaryExpr, 0, 0, "");
  binary_epxr_node->binary_expr.bin_op = BinaryExprType::ASSIGN;
  binary_epxr_node->binary_expr.left_expr = symbol_node;
  binary_epxr_node->binary_expr.right_expr = const_value_node;

  // given: function proto -> fn my_func() i32
  auto function_proto_node = new AstNode(AstNodeType::AstFuncProto, 0, 0, "");
  function_proto_node->function_proto.name = "my_func";
  function_proto_node->function_proto.return_type = i32_type_node;

  // given: function block
  auto function_block_node = new AstNode(AstNodeType::AstBlock, 0, 0, "");
  function_block_node->block.statements.push_back(var_def_node);
  function_block_node->block.statements.push_back(binary_epxr_node);

  // given: function
  auto function_node = new AstNode(AstNodeType::AstBinaryExpr, 0, 0, "");
  function_node->function_def.proto = function_proto_node;
  function_node->function_def.block = function_block_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  bool is_valid_proto = analizer.analizeFuncProto(function_proto_node);
  bool is_valid = analizer.analizeFuncBlock(function_block_node->block, function_node->function_def);

  // then:
  ASSERT_TRUE(is_valid_proto);
  ASSERT_FALSE(is_valid);
  ASSERT_GT(errors.size(), 0L);
}

//==================================================================================
//          SEMANTIC FUNCTION CALLS
//==================================================================================

TEST(SemanticExpressions, FunctionCallUndeclaredFn) {
  auto function_call_node = new AstNode(AstNodeType::AstFuncCallExpr, 0, 0, "");
  function_call_node->func_call.fn_name = "my_func";
  function_call_node->func_call.fn_ref = nullptr;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  bool is_valid_call = analizer.analizeExpr(function_call_node);

  // then:
  ASSERT_FALSE(is_valid_call);
  ASSERT_GT(errors.size(), 0L);
}

TEST(SemanticExpressions, FunctionCallSymbolNotAFn) {
  bool is_global = false;
  // given: variable definition
  auto i32_type_node = get_type_node("i32");

  auto var_def_node = new AstNode(AstNodeType::AstVarDef, 0, 0, "");
  var_def_node->var_def.type = i32_type_node;
  var_def_node->var_def.name = "my_var";
  var_def_node->var_def.initializer = nullptr;

  auto function_call_node = new AstNode(AstNodeType::AstFuncCallExpr, 0, 0, "");
  function_call_node->func_call.fn_name = "my_var";
  function_call_node->func_call.fn_ref = nullptr;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // given: valid variable
  bool is_valid_var = analizer.analizeVarDef(var_def_node, is_global);

  // when: call to analize_expr
  bool is_valid_call = analizer.analizeExpr(function_call_node);

  // then:
  ASSERT_TRUE(is_valid_var);
  ASSERT_FALSE(is_valid_call);
  ASSERT_GT(errors.size(), 0L);
}

TEST(SemanticExpressions, FunctionCallParamsCountMismatch) {
  // given: variable definition
  auto void_type_node = get_type_node("void");

  // given: arg -> constant integer
  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;

  // given: function proto -> fn my_func() void
  auto function_proto_node = new AstNode(AstNodeType::AstFuncProto, 0, 0, "");
  function_proto_node->function_proto.name = "my_func";
  function_proto_node->function_proto.return_type = void_type_node;

  // given: function block
  auto function_block_node = new AstNode(AstNodeType::AstBlock, 0, 0, "");

  // given: function
  auto function_node = new AstNode(AstNodeType::AstBinaryExpr, 0, 0, "");
  function_node->function_def.proto = function_proto_node;
  function_node->function_def.block = function_block_node;

  auto function_call_node = new AstNode(AstNodeType::AstFuncCallExpr, 0, 0, "");
  function_call_node->func_call.fn_name = "my_func";
  function_call_node->func_call.fn_ref = nullptr;
  function_call_node->func_call.args.push_back(const_value_node);

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  bool is_valid_proto = analizer.analizeFuncProto(function_proto_node);
  bool is_valid_block = analizer.analizeFuncBlock(function_block_node->block, function_node->function_def);
  bool is_valid_call = analizer.analizeExpr(function_call_node);

  // then:
  ASSERT_TRUE(is_valid_proto);
  ASSERT_TRUE(is_valid_block);
  ASSERT_FALSE(is_valid_call);
  ASSERT_GT(errors.size(), 0L);
}

TEST(SemanticExpressions, FunctionCallParamsTypeMismatch) {
  // given: types
  auto void_type_node = get_type_node("void");
  auto i32_type_node = get_type_node("i32");

  // given: arg -> constant integer
  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::FLOAT;

  // given: param -> param1 i32
  auto param_node = new AstNode(AstNodeType::AstParamDecl, 0, 0, "");
  param_node->param_decl.name = "param1";
  param_node->param_decl.type = i32_type_node;
  param_node->param_decl.initializer = nullptr;

  // given: function proto -> fn my_func(param1 i32) void
  auto function_proto_node = new AstNode(AstNodeType::AstFuncProto, 0, 0, "");
  function_proto_node->function_proto.name = "my_func";
  function_proto_node->function_proto.return_type = void_type_node;
  function_proto_node->function_proto.params.push_back(param_node);

  // given: function block
  auto function_block_node = new AstNode(AstNodeType::AstBlock, 0, 0, "");

  // given: function
  auto function_node = new AstNode(AstNodeType::AstBinaryExpr, 0, 0, "");
  function_node->function_def.proto = function_proto_node;
  function_node->function_def.block = function_block_node;

  auto function_call_node = new AstNode(AstNodeType::AstFuncCallExpr, 0, 0, "");
  function_call_node->func_call.fn_name = "my_func";
  function_call_node->func_call.fn_ref = nullptr;
  function_call_node->func_call.args.push_back(const_value_node);

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  bool is_valid_proto = analizer.analizeFuncProto(function_proto_node);
  bool is_valid_block = analizer.analizeFuncBlock(function_block_node->block, function_node->function_def);
  bool is_valid_call = analizer.analizeExpr(function_call_node);

  // then:
  ASSERT_TRUE(is_valid_proto);
  ASSERT_TRUE(is_valid_block);
  ASSERT_FALSE(is_valid_call);
  ASSERT_GT(errors.size(), 0L);
}
