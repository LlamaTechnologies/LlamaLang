#include "../../src/Types.hpp"
#include "../../src/ast_nodes.hpp"
#include "../../src/error.hpp"
#include "../../src/lexer.hpp"
#include "../../src/parser.hpp"
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

  bool is_ok = check_types(errors, node_type_0, node_type_1, node_expr);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_TRUE(is_ok);
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

  bool is_ok = check_types(errors, node_type_0, node_type_1, node_expr);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_TRUE(is_ok);

  delete node_expr;
  delete node_type_0;
  delete node_type_1;
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

  bool is_ok = check_types(errors, node_type_0, node_type_1, node_expr);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_TRUE(is_ok);

  delete node_expr;
  delete node_type_0;
  delete node_type_1;
}

TEST(SemanticTypes, Integer) {
  std::vector<Error> errors;
  auto node_expr = new AstNode(AstNodeType::AstVarDef, 0, 0, "");

  auto node_type_0 = get_type_node("i32");
  auto node_type_1 = get_type_node("i32");

  bool is_ok = check_types(errors, node_type_0, node_type_1, node_expr);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_TRUE(is_ok);

  delete node_expr;
  delete node_type_0;
  delete node_type_1;
}

TEST(SemanticTypes, Float) {
  std::vector<Error> errors;
  auto node_expr = new AstNode(AstNodeType::AstVarDef, 0, 0, "");

  auto node_type_0 = get_type_node("f32");
  auto node_type_1 = get_type_node("f32");

  bool is_ok = check_types(errors, node_type_0, node_type_1, node_expr);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_TRUE(is_ok);

  delete node_expr;
  delete node_type_0;
  delete node_type_1;
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
  ASSERT_TRUE(is_valid);

  // clean:
  delete var_def_node;
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
  ASSERT_TRUE(is_valid);

  // clean:
  delete var_def_node;
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
  ASSERT_TRUE(is_valid);

  // clean:
  delete const_value_node;
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
  ASSERT_TRUE(is_valid);
  ASSERT_EQ(symbol_node->symbol.type, SymbolType::VAR);

  // clean:
  delete var_def_node;
  delete symbol_node;
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
  const_value_node->parent = unary_epxr_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: a bool constant
  // with: a unary operator valid for the expr type
  bool is_valid = analizer.analizeExpr(unary_epxr_node);

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_TRUE(is_valid);

  // clean:
  delete unary_epxr_node;
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
  ASSERT_TRUE(is_valid);

  // clean:
  delete unary_epxr_node;
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
  ASSERT_TRUE(is_valid);

  delete unary_epxr_node;
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
  const_value_node->parent = binary_epxr_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: 2 constants expresions as child expr
  // with: right expr of type integer
  bool is_valid = analizer.analizeExpr(binary_epxr_node);

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_TRUE(is_valid);

  delete binary_epxr_node;
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
  ASSERT_TRUE(is_valid_var_def);
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_TRUE(is_valid);

  delete binary_epxr_node;
  delete var_def_node;
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
  ASSERT_TRUE(is_valid_var_def);
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_TRUE(is_valid);

  delete binary_epxr_node;
  delete var_def_node;
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
  ASSERT_TRUE(is_valid_proto);
  ASSERT_TRUE(is_valid);
  ASSERT_EQ(errors.size(), 0L);

  delete function_node;
}

TEST(SemanticExpressions, FunctionRetConstant) {
  // given: code
  const char *source_name = "function_ret.llama";
  const char *source_code = "fn function_ret() i32 {\n"
                            "\tret 5\n"
                            "}\n";

  // given: lexer
  std::vector<Error> errors;
  Lexer lexer(source_code, source_name, errors);
  lexer.tokenize();

  // given: parser
  Parser parser(errors);
  auto function_def_node = parser.parse_function_def(lexer);
  ASSERT_NE(function_def_node, nullptr);

  // given: analizer
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  bool is_valid_proto = analizer.analizeFuncProto(function_def_node->function_def.proto);
  bool is_valid =
    analizer.analizeFuncBlock(function_def_node->function_def.block->block, function_def_node->function_def);

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(function_def_node->node_type, AstNodeType::AstFuncDef);
  ASSERT_NE(function_def_node->function_def.proto, nullptr);
  ASSERT_NE(function_def_node->function_def.block, nullptr);

  const AstNode *block_node = function_def_node->function_def.block;
  ASSERT_EQ(block_node->block.statements.size(), 1);

  ASSERT_TRUE(is_valid_proto);
  ASSERT_TRUE(is_valid);

  const AstNode *ret_node = block_node->block.statements[0];
  ASSERT_NE(ret_node, nullptr);
  ASSERT_EQ(ret_node->node_type, AstNodeType::AstUnaryExpr);
  ASSERT_EQ(ret_node->unary_expr.op, UnaryExprType::RET);

  const AstNode *const_value_node = ret_node->unary_expr.expr;
  ASSERT_NE(const_value_node, nullptr);
  ASSERT_EQ(const_value_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(const_value_node->const_value.is_negative, false);
  ASSERT_EQ(const_value_node->const_value.bit_size, 32);

  // clean:
  delete function_def_node;
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
  ASSERT_TRUE(is_valid_proto);
  ASSERT_TRUE(is_valid_block);
  ASSERT_TRUE(is_valid_call);
  ASSERT_EQ(errors.size(), 0L);

  // clean:
  delete function_node;
  delete function_call_node;
  delete void_type_node;
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
  ASSERT_TRUE(is_valid_call);
  ASSERT_EQ(errors.size(), 0L);

  // clean:
  delete function_node;
  delete function_call_node;
  delete void_type_node;
  delete i32_type_node;
}
