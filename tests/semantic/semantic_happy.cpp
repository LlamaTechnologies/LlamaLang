#include "../../src/Types.hpp"
#include "../../src/ast_nodes.hpp"
#include "../../src/error.hpp"
#include "../../src/semantic_analyzer.hpp"

#include <gtest/gtest.h>

//==================================================================================
//          SEMANTIC TYPES ANALYSIS
//==================================================================================

TEST(SemanticTypes, BoolVoid) {
  std::vector<Error> errors;
  auto node_expr = new AstNode(AstNodeType::AstVarDef, 0, 0, "");

  auto node_type_0 = get_type_node("bool");
  auto node_type_1 = get_type_node("bool");

  SemanticAnalyzer analizer(errors);
  analizer.check_type_compat(node_type_0, node_type_1, node_expr);

  ASSERT_EQ(errors.size(), 0L);
}

TEST(SemanticTypes, StructsUnsuported) {
  std::vector<Error> errors;
  auto node_expr = new AstNode(AstNodeType::AstVarDef, 0, 0, "");

  auto node_type_0 = new AstNode(AstNodeType::AstType, 0, 0, "");
  node_type_0->ast_type.type_id = AstTypeId::Struct;
  node_type_0->ast_type.type_info = new TypeInfo();
  node_type_0->ast_type.type_info->bit_size = 1;
  auto node_type_1 = new AstNode(AstNodeType::AstType, 0, 0, "");
  node_type_1->ast_type.type_id = AstTypeId::Struct;
  node_type_1->ast_type.type_info = new TypeInfo();
  node_type_1->ast_type.type_info->bit_size = 1;

  SemanticAnalyzer analizer(errors);

  ASSERT_DEATH(analizer.check_type_compat(node_type_0, node_type_1, node_expr), "");
}

TEST(SemanticTypes, Pointer) {
  std::vector<Error> errors;
  auto node_expr = new AstNode(AstNodeType::AstVarDef, 0, 0, "");

  auto node_child_type_0 = get_type_node("i32");

  auto node_type_0 = new AstNode(AstNodeType::AstType, 0, 0, "");
  node_type_0->ast_type.type_id = AstTypeId::Pointer;
  node_type_0->ast_type.child_type = node_child_type_0;

  auto node_child_type_1 = get_type_node("i32");

  auto node_type_1 = new AstNode(AstNodeType::AstType, 0, 0, "");
  node_type_1->ast_type.type_id = AstTypeId::Pointer;
  node_type_1->ast_type.child_type = node_child_type_1;

  SemanticAnalyzer analizer(errors);
  analizer.check_type_compat(node_type_0, node_type_1, node_expr);

  ASSERT_EQ(errors.size(), 0L);
}

TEST(SemanticTypes, Array) {
  std::vector<Error> errors;
  auto node_expr = new AstNode(AstNodeType::AstVarDef, 0, 0, "");

  auto node_child_type_0 = get_type_node("i32");

  auto node_type_0 = new AstNode(AstNodeType::AstType, 0, 0, "");
  node_type_0->ast_type.type_id = AstTypeId::Array;
  node_type_0->ast_type.child_type = node_child_type_0;

  auto node_child_type_1 = get_type_node("i32");

  auto node_type_1 = new AstNode(AstNodeType::AstType, 0, 0, "");
  node_type_1->ast_type.type_id = AstTypeId::Array;
  node_type_1->ast_type.child_type = node_child_type_1;

  SemanticAnalyzer analizer(errors);
  analizer.check_type_compat(node_type_0, node_type_1, node_expr);

  ASSERT_EQ(errors.size(), 0L);
}

TEST(SemanticTypes, Integer) {
  std::vector<Error> errors;
  auto node_expr = new AstNode(AstNodeType::AstVarDef, 0, 0, "");

  auto node_type_0 = get_type_node("i32");
  auto node_type_1 = get_type_node("i32");

  SemanticAnalyzer analizer(errors);
  analizer.check_type_compat(node_type_0, node_type_1, node_expr);

  ASSERT_EQ(errors.size(), 0L);
}

TEST(SemanticTypes, Float) {
  std::vector<Error> errors;
  auto node_expr = new AstNode(AstNodeType::AstVarDef, 0, 0, "");

  auto node_type_0 = get_type_node("f32");
  auto node_type_1 = get_type_node("f32");

  SemanticAnalyzer analizer(errors);
  analizer.check_type_compat(node_type_0, node_type_1, node_expr);

  ASSERT_EQ(errors.size(), 0L);
}

//==================================================================================
//          SEMANTIC VARIABLES DEFINITION
//==================================================================================

TEST(SemanticVariableDefinitions, GlobalVariable) {
  bool is_global = true;
  std::vector<Error> errors;

  auto i32_type_node = get_type_node("i32");

  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;
  const_value_node->const_value.number = "0";

  auto var_def_node = new AstNode(AstNodeType::AstVarDef, 0, 0, "");
  var_def_node->var_def.type = i32_type_node;
  var_def_node->var_def.name = "global_var";
  var_def_node->var_def.initializer = const_value_node;

  SemanticAnalyzer analizer(errors);
  bool is_valid = analizer.analizeVarDef(var_def_node, is_global);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(is_valid, true);
}

TEST(SemanticVariableDefinitions, LocalVariableNoInit) {
  bool is_global = false;
  std::vector<Error> errors;

  auto i32_type_node = get_type_node("i32");

  auto var_def_node = new AstNode(AstNodeType::AstVarDef, 0, 0, "");
  var_def_node->var_def.type = i32_type_node;
  var_def_node->var_def.name = "local_var";
  var_def_node->var_def.initializer = nullptr;

  SemanticAnalyzer analizer(errors);
  bool is_valid = analizer.analizeVarDef(var_def_node, is_global);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(is_valid, true);
}

//==================================================================================
//          SEMANTIC EXPRESSIONS
//==================================================================================

TEST(SemanticExpressions, ConstantValue) {
  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;
  const_value_node->const_value.number = "0";

  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);
  bool is_valid = analizer.analizeExpr(const_value_node);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(is_valid, true);
}

TEST(SemanticExpressions, ResolveKnownVariableSymbol) {
  // given: variable definition
  auto i32_type_node = get_type_node("i32");

  auto var_def_node = new AstNode(AstNodeType::AstVarDef, 0, 0, "");
  var_def_node->var_def.type = i32_type_node;
  auto var_name = var_def_node->var_def.name = "my_var";

  // given: symbol node
  auto symbol_node = new AstNode(AstNodeType::AstSymbol, 0, 0, "");
  symbol_node->symbol.cached_name = std::string_view(var_name.data(), var_name.size());

  // given: configured analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);
  analizer.analizeVarDef(var_def_node, false);

  // when: call to analize_expr
  bool is_valid = analizer.analizeExpr(symbol_node);

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(is_valid, true);
  ASSERT_EQ(symbol_node->symbol.type, SymbolType::VAR);
}

TEST(SemanticExpressions, UnaryExprBool) {
  // given: expr
  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::BOOL;
  const_value_node->const_value.boolean = false;

  // given: unary expr
  auto unary_epxr_node = new AstNode(AstNodeType::AstUnaryExpr, 0, 0, "");
  unary_epxr_node->unary_expr.op = UnaryExprType::NOT;
  unary_epxr_node->unary_expr.expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: a bool expresion
  // with: a unary operator valid for the expr type
  bool is_valid = analizer.analizeExpr(unary_epxr_node);

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(is_valid, true);
}

TEST(SemanticExpressions, UnaryExprNumber) {
  // given: expr
  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;
  const_value_node->const_value.number = "0";

  // given: unary expr
  auto unary_epxr_node = new AstNode(AstNodeType::AstUnaryExpr, 0, 0, "");
  unary_epxr_node->unary_expr.op = UnaryExprType::NEG;
  unary_epxr_node->unary_expr.expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: a number expresion
  // with: a unary operator valid for the expr type
  bool is_valid = analizer.analizeExpr(unary_epxr_node);

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(is_valid, true);
}

TEST(SemanticExpressions, UnaryExprNumberBitwiseOp) {
  // given: expr
  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;
  const_value_node->const_value.number = "0";

  // given: unary expr
  auto unary_epxr_node = new AstNode(AstNodeType::AstUnaryExpr, 0, 0, "");
  unary_epxr_node->unary_expr.op = UnaryExprType::BIT_INV;
  unary_epxr_node->unary_expr.expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: a number expresion
  // with: a unary operator valid for the expr type
  bool is_valid = analizer.analizeExpr(unary_epxr_node);

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(is_valid, true);
}

TEST(SemanticExpressions, BinaryExprBitShift) {
  // given: expr
  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;
  const_value_node->const_value.number = "1";

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
  // with: 2 constants expresions as child expr
  // with: right expr of type integer
  bool is_valid = analizer.analizeExpr(binary_epxr_node);

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(is_valid, true);
}

TEST(SemanticExpressions, BinaryExprBoolOperator) {
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

  // given: biary expr -> my_var == SOME_INT
  auto binary_epxr_node = new AstNode(AstNodeType::AstBinaryExpr, 0, 0, "");
  binary_epxr_node->binary_expr.bin_op = BinaryExprType::EQUALS;
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
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(is_valid, true);
}

TEST(SemanticExpressions, BinaryExprAssignOperator) {
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
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(is_valid, true);
}

//==================================================================================
//          SEMANTIC FUNCTIONS
//==================================================================================

TEST(SemanticExpressions, FunctionNoRet) {
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
  ASSERT_EQ(is_valid_proto, true);
  ASSERT_EQ(is_valid, true);
  ASSERT_EQ(errors.size(), 0L);
}

TEST(SemanticExpressions, FunctionRet) {
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

  // given: unary expr -> ret my_var
  auto ret_stmnt_node = new AstNode(AstNodeType::AstUnaryExpr, 0, 0, "");
  ret_stmnt_node->unary_expr.op = UnaryExprType::RET;
  ret_stmnt_node->unary_expr.expr = symbol_node;

  // given: function proto -> fn my_func() i32
  auto function_proto_node = new AstNode(AstNodeType::AstFuncProto, 0, 0, "");
  function_proto_node->function_proto.name = "my_func";
  function_proto_node->function_proto.return_type = i32_type_node;

  // given: function block
  auto function_block_node = new AstNode(AstNodeType::AstBlock, 0, 0, "");
  function_block_node->block.statements.push_back(var_def_node);
  function_block_node->block.statements.push_back(binary_epxr_node);
  function_block_node->block.statements.push_back(ret_stmnt_node);

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
  ASSERT_EQ(is_valid_proto, true);
  ASSERT_EQ(is_valid, true);
  ASSERT_EQ(errors.size(), 0L);
}

//==================================================================================
//          SEMANTIC FUNCTION CALLS
//==================================================================================

TEST(SemanticExpressions, FunctionCallNoParam) {
  // given: variable definition
  auto void_type_node = get_type_node("void");

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

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  bool is_valid_proto = analizer.analizeFuncProto(function_proto_node);
  bool is_valid_block = analizer.analizeFuncBlock(function_block_node->block, function_node->function_def);
  bool is_valid_call = analizer.analizeExpr(function_call_node);

  // then:
  ASSERT_EQ(is_valid_proto, true);
  ASSERT_EQ(is_valid_block, true);
  ASSERT_EQ(is_valid_call, true);
  ASSERT_EQ(errors.size(), 0L);
}

TEST(SemanticExpressions, FunctionCallWithParams) {
  // given: types
  auto void_type_node = get_type_node("void");
  auto i32_type_node = get_type_node("i32");

  // given: arg -> constant integer
  auto const_value_node = new AstNode(AstNodeType::AstConstValue, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;

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
  function_call_node->func_call.params.push_back(const_value_node);

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  bool is_valid_proto = analizer.analizeFuncProto(function_proto_node);
  bool is_valid_block = analizer.analizeFuncBlock(function_block_node->block, function_node->function_def);
  bool is_valid_call = analizer.analizeExpr(function_call_node);

  // then:
  ASSERT_EQ(is_valid_proto, true);
  ASSERT_EQ(is_valid_block, true);
  ASSERT_EQ(is_valid_call, true);
  ASSERT_EQ(errors.size(), 0L);
}
