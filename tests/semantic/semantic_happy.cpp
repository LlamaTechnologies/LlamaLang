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

TEST(SemanticTypes, BoolBool) {
  std::vector<Error> errors;
  TypesRepository types_repository = TypesRepository::get();

  auto node_expr = new AstNode(AstNodeType::AST_VAR_DEF, 0, 0, "");

  auto node_type_0 = types_repository.get_type_node("bool");
  auto node_type_1 = types_repository.get_type_node("bool");

  bool is_ok = check_types(errors, node_type_0, node_type_1, node_expr);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_TRUE(is_ok);
}

TEST(SemanticTypes, Pointer) {
  std::vector<Error> errors;
  TypesRepository types_repository = TypesRepository::get();

  auto node_expr = new AstNode(AstNodeType::AST_VAR_DEF, 0, 0, "");

  auto node_child_type_0 = types_repository.get_type_node("i32");

  auto node_type_0 = new AstNode(AstNodeType::AST_TYPE, 0, 0, "");
  node_type_0->ast_type.type_id = AstTypeId::POINTER;
  node_type_0->ast_type.child_type = node_child_type_0;

  auto node_child_type_1 = types_repository.get_type_node("i32");

  auto node_type_1 = new AstNode(AstNodeType::AST_TYPE, 0, 0, "");
  node_type_1->ast_type.type_id = AstTypeId::POINTER;
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
  TypesRepository types_repository = TypesRepository::get();

  auto node_expr = new AstNode(AstNodeType::AST_VAR_DEF, 0, 0, "");

  auto node_child_type_0 = types_repository.get_type_node("i32");

  auto node_type_0 = new AstNode(AstNodeType::AST_TYPE, 0, 0, "");
  node_type_0->ast_type.type_id = AstTypeId::ARRAY;
  node_type_0->ast_type.child_type = node_child_type_0;

  auto node_child_type_1 = types_repository.get_type_node("i32");

  auto node_type_1 = new AstNode(AstNodeType::AST_TYPE, 0, 0, "");
  node_type_1->ast_type.type_id = AstTypeId::ARRAY;
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
  TypesRepository types_repository = TypesRepository::get();

  auto node_expr = new AstNode(AstNodeType::AST_VAR_DEF, 0, 0, "");

  auto node_type_0 = types_repository.get_type_node("i32");
  auto node_type_1 = types_repository.get_type_node("i32");

  bool is_ok = check_types(errors, node_type_0, node_type_1, node_expr);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_TRUE(is_ok);

  delete node_expr;
  delete node_type_0;
  delete node_type_1;
}

TEST(SemanticTypes, Float) {
  std::vector<Error> errors;
  TypesRepository types_repository = TypesRepository::get();

  auto node_expr = new AstNode(AstNodeType::AST_VAR_DEF, 0, 0, "");

  auto node_type_0 = types_repository.get_type_node("f32");
  auto node_type_1 = types_repository.get_type_node("f32");

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
  TypesRepository types_repository = TypesRepository::get();

  auto i32_type_node = types_repository.get_type_node("i32");

  auto const_value_node = new AstNode(AstNodeType::AST_CONST_VALUE, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;
  const_value_node->const_value.number = "0";

  auto var_def_node = new AstNode(AstNodeType::AST_VAR_DEF, 0, 0, "");
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
  TypesRepository types_repository = TypesRepository::get();

  auto i32_type_node = types_repository.get_type_node("i32");

  auto var_def_node = new AstNode(AstNodeType::AST_VAR_DEF, 0, 0, "");
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
  auto const_value_node = new AstNode(AstNodeType::AST_CONST_VALUE, 0, 0, "");
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
  TypesRepository types_repository = TypesRepository::get();

  // given: variable definition
  auto i32_type_node = types_repository.get_type_node("i32");

  auto var_def_node = new AstNode(AstNodeType::AST_VAR_DEF, 0, 0, "");
  var_def_node->var_def.type = i32_type_node;
  auto var_name = var_def_node->var_def.name = "my_var";

  // given: symbol node
  auto symbol_node = new AstNode(AstNodeType::AST_SYMBOL, 0, 0, "");
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
  auto const_value_node = new AstNode(AstNodeType::AST_CONST_VALUE, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::BOOL;
  const_value_node->const_value.boolean = false;

  // given: unary expr
  auto unary_epxr_node = new AstNode(AstNodeType::AST_UNARY_EXPR, 0, 0, "");
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
  auto const_value_node = new AstNode(AstNodeType::AST_CONST_VALUE, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;
  const_value_node->const_value.number = "0";

  // given: unary expr
  auto unary_epxr_node = new AstNode(AstNodeType::AST_UNARY_EXPR, 0, 0, "");
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
  auto const_value_node = new AstNode(AstNodeType::AST_CONST_VALUE, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;
  const_value_node->const_value.number = "0";

  // given: unary expr
  auto unary_epxr_node = new AstNode(AstNodeType::AST_UNARY_EXPR, 0, 0, "");
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
  auto const_value_node = new AstNode(AstNodeType::AST_CONST_VALUE, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;
  const_value_node->const_value.number = "1";

  // given: biary expr
  auto binary_epxr_node = new AstNode(AstNodeType::AST_BINARY_EXPR, 0, 0, "");
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
  TypesRepository types_repository = TypesRepository::get();

  // given: variable definition
  auto i32_type_node = types_repository.get_type_node("i32");

  auto var_def_node = new AstNode(AstNodeType::AST_VAR_DEF, 0, 0, "");
  var_def_node->var_def.type = i32_type_node;
  auto var_name = var_def_node->var_def.name = "my_var";

  // given: l_expr -> symbol node
  auto symbol_node = new AstNode(AstNodeType::AST_SYMBOL, 0, 0, "");
  symbol_node->symbol.cached_name = std::string_view(var_name.data(), var_name.size());

  // given: r_expr -> constant integer
  auto const_value_node = new AstNode(AstNodeType::AST_CONST_VALUE, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;

  // given: biary expr -> my_var == SOME_INT
  auto binary_epxr_node = new AstNode(AstNodeType::AST_BINARY_EXPR, 0, 0, "");
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
  TypesRepository types_repository = TypesRepository::get();

  // given: variable definition
  auto i32_type_node = types_repository.get_type_node("i32");

  auto var_def_node = new AstNode(AstNodeType::AST_VAR_DEF, 0, 0, "");
  var_def_node->var_def.type = i32_type_node;
  auto var_name = var_def_node->var_def.name = "my_var";

  // given: l_expr -> symbol node
  auto symbol_node = new AstNode(AstNodeType::AST_SYMBOL, 0, 0, "");
  symbol_node->symbol.cached_name = std::string_view(var_name.data(), var_name.size());

  // given: r_expr -> constant integer
  auto const_value_node = new AstNode(AstNodeType::AST_CONST_VALUE, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;

  // given: biary expr -> my_var = SOME_INT
  auto binary_epxr_node = new AstNode(AstNodeType::AST_BINARY_EXPR, 0, 0, "");
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
  std::vector<Error> errors;

  // given: source_file
  const char *source_file = "fn my_func() void {\n"
                            "my_var i32\n"
                            "my_var = 34\n"
                            "}";

  // given: tokens
  Lexer lexer = Lexer(source_file, "FunctionNoRet", errors);
  lexer.tokenize();

  // given: parsed source node
  Parser parser = Parser(errors);
  AstNode *function_node = parser.parse_function_def(lexer);

  // then:
  ASSERT_NE(function_node, nullptr);
  ASSERT_EQ(function_node->node_type, AstNodeType::AST_FUNC_DEF);
  ASSERT_NE(function_node->function_def.proto, nullptr);
  ASSERT_NE(function_node->function_def.block, nullptr);
  ASSERT_EQ(errors.size(), 0L);

  // and given: analizer
  SemanticAnalyzer analizer(errors);

  AstNode *function_proto_node = function_node->function_def.proto;
  AstNode *function_block_node = function_node->function_def.block;
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

  AstNode *function_proto = function_def_node->function_def.proto;
  AstNode *function_block = function_def_node->function_def.block;

  // when: call to analize_expr
  bool is_valid_proto = analizer.analizeFuncProto(function_proto);
  bool is_valid = analizer.analizeFuncBlock(function_block->block, function_def_node->function_def);

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(function_def_node->node_type, AstNodeType::AST_FUNC_DEF);
  ASSERT_NE(function_def_node->function_def.proto, nullptr);
  ASSERT_NE(function_def_node->function_def.block, nullptr);

  const AstNode *block_node = function_def_node->function_def.block;
  ASSERT_EQ(block_node->block.statements.size(), 1);

  ASSERT_TRUE(is_valid_proto);
  ASSERT_TRUE(is_valid);

  const AstNode *ret_node = block_node->block.statements[0];
  ASSERT_NE(ret_node, nullptr);
  ASSERT_EQ(ret_node->node_type, AstNodeType::AST_UNARY_EXPR);
  ASSERT_EQ(ret_node->unary_expr.op, UnaryExprType::RET);

  const AstNode *const_value_node = ret_node->unary_expr.expr;
  ASSERT_NE(const_value_node, nullptr);
  ASSERT_EQ(const_value_node->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(const_value_node->const_value.is_negative, false);
  ASSERT_EQ(const_value_node->const_value.bit_size, 32);

  // clean:
  delete function_def_node;
}

//==================================================================================
//          SEMANTIC FUNCTION CALLS
//==================================================================================

TEST(SemanticExpressions, FunctionCallNoParam) {
  TypesRepository types_repository = TypesRepository::get();

  // given: variable definition
  auto void_type_node = types_repository.get_type_node("void");

  // given: function proto -> fn my_func() void
  auto function_proto_node = new AstNode(AstNodeType::AST_FUNC_PROTO, 0, 0, "");
  function_proto_node->function_proto.name = "my_func";
  function_proto_node->function_proto.return_type = void_type_node;

  // given: function block
  auto function_block_node = new AstNode(AstNodeType::AST_BLOCK, 0, 0, "");

  // given: function
  auto function_node = new AstNode(AstNodeType::AST_BINARY_EXPR, 0, 0, "");
  function_node->function_def.proto = function_proto_node;
  function_node->function_def.block = function_block_node;

  auto function_call_node = new AstNode(AstNodeType::AST_FN_CALL_EXPR, 0, 0, "");
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
  TypesRepository types_repository = TypesRepository::get();

  // given: types
  auto void_type_node = types_repository.get_type_node("void");
  auto i32_type_node = types_repository.get_type_node("i32");

  // given: arg -> constant integer
  auto const_value_node = new AstNode(AstNodeType::AST_CONST_VALUE, 0, 0, "");
  const_value_node->const_value.type = ConstValueType::INT;

  // given: param -> param1 i32
  auto param_node = new AstNode(AstNodeType::AST_PARAM_DECL, 0, 0, "");
  param_node->param_decl.name = "param1";
  param_node->param_decl.type = i32_type_node;
  param_node->param_decl.initializer = nullptr;

  // given: function proto -> fn my_func(param1 i32) void
  auto function_proto_node = new AstNode(AstNodeType::AST_FUNC_PROTO, 0, 0, "");
  function_proto_node->function_proto.name = "my_func";
  function_proto_node->function_proto.return_type = void_type_node;
  function_proto_node->function_proto.params.push_back(param_node);

  // given: function block
  auto function_block_node = new AstNode(AstNodeType::AST_BLOCK, 0, 0, "");

  // given: function
  auto function_node = new AstNode(AstNodeType::AST_BINARY_EXPR, 0, 0, "");
  function_node->function_def.proto = function_proto_node;
  function_node->function_def.block = function_block_node;

  auto function_call_node = new AstNode(AstNodeType::AST_FN_CALL_EXPR, 0, 0, "");
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
