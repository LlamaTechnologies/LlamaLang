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

TEST(SemanticTypes, Integer) {
  std::vector<Error> errors;
  TypesRepository types_repository = TypesRepository::get();

  auto node_expr = new AstNode(AstNodeType::AST_VAR_DEF, 0, 0, "");

  const AstType *node_type_0 = types_repository.get_type_node("i32");
  const AstType *node_type_1 = types_repository.get_type_node("i32");

  bool is_ok = check_types(errors, node_type_0, node_type_1, node_expr);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_TRUE(is_ok);

  delete node_expr;
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
}

//==================================================================================
//          SEMANTIC VARIABLES DEFINITION
//==================================================================================

TEST(SemanticVariableDefinitions, GlobalVariable) {
  bool is_global = true;
  std::vector<Error> errors;
  TypesRepository types_repository = TypesRepository::get();

  auto i32_type_node = types_repository.get_type_node("i32");

  auto const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::INT;
  const_value_node->number = "0";

  auto var_def_node = new AstVarDef(0, 0, "");
  var_def_node->type = i32_type_node;
  var_def_node->name = "global_var";
  var_def_node->initializer = const_value_node;

  SemanticAnalyzer analizer(errors);
  bool is_valid = analizer.analize_global_var_def(var_def_node);

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

  AstVarDef *var_def_node = new AstVarDef(0, 0, "");
  var_def_node->type = i32_type_node;
  var_def_node->name = "local_var";
  var_def_node->initializer = nullptr;

  SemanticAnalyzer analizer(errors);
  bool is_valid = analizer.analize_var_def(var_def_node);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_TRUE(is_valid);

  // clean:
  delete var_def_node;
}

//==================================================================================
//          SEMANTIC EXPRESSIONS
//==================================================================================

TEST(SemanticExpressions, ConstantValue) {
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::INT;
  const_value_node->number = "0";

  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);
  bool is_valid = analizer.analize_expr(const_value_node);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_TRUE(is_valid);

  // clean:
  delete const_value_node;
}

TEST(SemanticExpressions, ResolveKnownVariableSymbol) {
  TypesRepository types_repository = TypesRepository::get();

  // given: variable definition
  auto i32_type_node = types_repository.get_type_node("i32");

  AstVarDef *var_def_node = new AstVarDef(0, 0, "");
  var_def_node->type = i32_type_node;
  auto var_name = var_def_node->name = "my_var";

  // given: symbol node
  AstSymbol *symbol_node = new AstSymbol(0, 0, "");
  symbol_node->cached_name = std::string_view(var_name.data(), var_name.size());

  // given: configured analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);
  analizer.analize_var_def(var_def_node);

  // when: call to analize_expr
  bool is_valid = analizer.analize_expr(symbol_node);

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_TRUE(is_valid);
  ASSERT_EQ(symbol_node->type, SymbolType::VAR);

  // clean:
  delete var_def_node;
  delete symbol_node;
}

TEST(SemanticExpressions, UnaryExprBool) {
  // given: expr
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::BOOL;
  const_value_node->boolean = false;

  // given: unary expr
  AstUnaryExpr *unary_epxr_node = new AstUnaryExpr(0, 0, "");
  unary_epxr_node->op = UnaryExprType::NOT;
  unary_epxr_node->expr = const_value_node;
  const_value_node->parent = unary_epxr_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: a bool constant
  // with: a unary operator valid for the expr type
  bool is_valid = analizer.analize_expr(unary_epxr_node);

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_TRUE(is_valid);

  // clean:
  delete unary_epxr_node;
}

TEST(SemanticExpressions, UnaryExprNumber) {
  // given: expr
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::INT;
  const_value_node->number = "0";

  // given: unary expr
  AstUnaryExpr *unary_epxr_node = new AstUnaryExpr(0, 0, "");
  unary_epxr_node->op = UnaryExprType::NEG;
  unary_epxr_node->expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: a number expresion
  // with: a unary operator valid for the expr type
  bool is_valid = analizer.analize_expr(unary_epxr_node);

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_TRUE(is_valid);

  // clean:
  delete unary_epxr_node;
}

TEST(SemanticExpressions, UnaryExprNumberBitwiseOp) {
  // given: expr
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::INT;
  const_value_node->number = "0";

  // given: unary expr
  AstUnaryExpr *unary_epxr_node = new AstUnaryExpr(0, 0, "");
  unary_epxr_node->op = UnaryExprType::BIT_INV;
  unary_epxr_node->expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: a number expresion
  // with: a unary operator valid for the expr type
  bool is_valid = analizer.analize_expr(unary_epxr_node);

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_TRUE(is_valid);

  delete unary_epxr_node;
}

TEST(SemanticExpressions, BinaryExprBitShift) {
  // given: expr
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::INT;
  const_value_node->number = "1";

  // given: biary expr
  AstBinaryExpr *binary_epxr_node = new AstBinaryExpr(0, 0, "");
  // bin_expr -> 1 << 1
  binary_epxr_node->bin_op = BinaryExprType::LSHIFT;
  binary_epxr_node->left_expr = const_value_node;
  binary_epxr_node->right_expr = const_value_node;
  const_value_node->parent = binary_epxr_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  // with: 2 constants expresions as child expr
  // with: right expr of type integer
  bool is_valid = analizer.analize_expr(binary_epxr_node);

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_TRUE(is_valid);

  delete binary_epxr_node;
}

TEST(SemanticExpressions, BinaryExprBoolOperator) {
  TypesRepository types_repository = TypesRepository::get();

  // given: variable definition
  auto i32_type_node = types_repository.get_type_node("i32");

  AstVarDef *var_def_node = new AstVarDef(0, 0, "");
  var_def_node->type = i32_type_node;
  auto var_name = var_def_node->name = "my_var";

  // given: l_expr -> symbol node
  AstSymbol *symbol_node = new AstSymbol(0, 0, "");
  symbol_node->cached_name = std::string_view(var_name.data(), var_name.size());

  // given: r_expr -> constant integer
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::INT;

  // given: biary expr -> my_var == SOME_INT
  AstBinaryExpr *binary_epxr_node = new AstBinaryExpr(0, 0, "");
  binary_epxr_node->bin_op = BinaryExprType::EQUALS;
  binary_epxr_node->left_expr = symbol_node;
  binary_epxr_node->right_expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);
  bool is_valid_var_def = analizer.analize_var_def(var_def_node);

  // when: call to analize_expr
  // with: symbol as left expr
  // with: constant as right expr
  bool is_valid = analizer.analize_expr(binary_epxr_node);

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

  AstVarDef *var_def_node = new AstVarDef(0, 0, "");
  var_def_node->type = i32_type_node;
  auto var_name = var_def_node->name = "my_var";

  // given: l_expr -> symbol node
  AstSymbol *symbol_node = new AstSymbol(0, 0, "");
  symbol_node->cached_name = std::string_view(var_name.data(), var_name.size());

  // given: r_expr -> constant integer
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::INT;

  // given: biary expr -> my_var = SOME_INT
  AstBinaryExpr *binary_epxr_node = new AstBinaryExpr(0, 0, "");
  binary_epxr_node->bin_op = BinaryExprType::ASSIGN;
  binary_epxr_node->left_expr = symbol_node;
  binary_epxr_node->right_expr = const_value_node;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);
  bool is_valid_var_def = analizer.analize_var_def(var_def_node);

  // when: call to analize_expr
  // with: symbol as left expr
  // with: constant as right expr
  bool is_valid = analizer.analize_expr(binary_epxr_node);

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

TEST(SemanticFunctions, FunctionNoRet) {
  std::vector<Error> errors;

  // given: source_file
  const char *source_file = "fn my_func() void {\n"
                            "my_var i32\n"
                            "my_var = 34\n"
                            "}";

  // given: tokens
  Lexer lexer = Lexer(source_file, "file/directory", "FunctionNoRet", errors);
  lexer.tokenize();

  // given: parsed source node
  Parser parser = Parser(errors);
  AstNode *function_node = parser.parse_function_def(lexer);

  // then:
  ASSERT_NE(function_node, nullptr);
  ASSERT_EQ(function_node->node_type, AstNodeType::AST_FN_DEF);

  AstFnDef *fn_def_node = function_node->fn_def();
  ASSERT_NE(fn_def_node->proto, nullptr);
  ASSERT_NE(fn_def_node->block, nullptr);
  ASSERT_EQ(errors.size(), 0L);

  // and given: analizer
  SemanticAnalyzer analizer(errors);

  AstFnProto *function_proto_node = fn_def_node->proto;
  AstBlock *function_block_node = fn_def_node->block;
  bool is_valid_proto = analizer.analize_fn_proto(function_proto_node);
  analizer.enter_fn_scope(fn_def_node);
  bool is_valid = analizer.analize_fn_block(function_block_node);
  analizer.exit_fn_scope();

  // then:
  ASSERT_TRUE(is_valid_proto);
  ASSERT_TRUE(is_valid);
  ASSERT_EQ(errors.size(), 0L);

  delete function_node;
}

TEST(SemanticFunctions, FunctionRetConstant) {
  // given: code
  const char *source_name = "function_ret.llama";
  const char *source_code = "fn function_ret() i32 {\n"
                            "\tret 5\n"
                            "}\n";

  // given: lexer
  std::vector<Error> errors;
  Lexer lexer(source_code, "file/directory", source_name, errors);
  lexer.tokenize();

  // given: parser
  Parser parser(errors);
  AstNode *function_def_node = parser.parse_function_def(lexer);

  ASSERT_NE(function_def_node, nullptr);
  ASSERT_EQ(function_def_node->node_type, AstNodeType::AST_FN_DEF);

  AstFnDef *fn_def_node = function_def_node->fn_def();
  // given: analizer
  SemanticAnalyzer analizer(errors);

  AstFnProto *fn_proto = fn_def_node->proto;
  AstBlock *block_node = fn_def_node->block;
  ASSERT_NE(block_node, nullptr);
  ASSERT_NE(block_node, nullptr);

  // when: call to analize_expr
  bool is_valid_proto = analizer.analize_fn_proto(fn_proto);
  analizer.enter_fn_scope(fn_def_node);
  bool is_valid = analizer.analize_fn_block(block_node);
  analizer.exit_fn_scope();

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(block_node->statements.size(), 1);

  ASSERT_TRUE(is_valid_proto);
  ASSERT_TRUE(is_valid);

  const AstNode *ret_node = block_node->statements[0];
  ASSERT_NE(ret_node, nullptr);
  ASSERT_EQ(ret_node->node_type, AstNodeType::AST_UNARY_EXPR);
  const AstUnaryExpr *ret_expr = ret_node->unary_expr();
  ASSERT_EQ(ret_expr->op, UnaryExprType::RET);
  ASSERT_NE(ret_expr->expr, nullptr);
  ASSERT_EQ(ret_expr->expr->node_type, AstNodeType::AST_CONST_VALUE);

  const AstConstValue *const_value_node = ret_expr->expr->const_value();
  ASSERT_EQ(const_value_node->is_negative, false);
  ASSERT_EQ(const_value_node->bit_size, 32);

  // clean:
  delete function_def_node;
}

//==================================================================================
//          SEMANTIC FUNCTION CALLS
//==================================================================================

TEST(SemanticFunctionsCalls, FunctionCallNoParam) {
  TypesRepository types_repository = TypesRepository::get();

  // given: variable definition
  AstType *void_type_node = types_repository.get_type_node("void");

  // given: function proto -> fn my_func() void
  AstFnProto *function_proto_node = new AstFnProto(0, 0, "");
  function_proto_node->name = "my_func";
  function_proto_node->return_type = void_type_node;

  AstFnCallExpr *function_call_node = new AstFnCallExpr(0, 0, "");
  function_call_node->fn_name = "my_func";
  function_call_node->fn_ref = nullptr;

  // given: analizer
  std::vector<Error> errors;
  SemanticAnalyzer analizer(errors);

  // when: call to analize_expr
  bool is_valid_proto = analizer.analize_fn_proto(function_proto_node);
  bool is_valid_call = analizer.analize_expr(function_call_node);

  // then:
  ASSERT_TRUE(is_valid_proto);
  ASSERT_TRUE(is_valid_call);
  ASSERT_EQ(errors.size(), 0L);

  // clean:
  delete function_call_node;
  delete function_proto_node;
}

TEST(SemanticFunctionsCalls, FunctionCallWithParams) {
  TypesRepository types_repository = TypesRepository::get();

  // given: types
  AstType *void_type_node = types_repository.get_type_node("void");
  AstType *i32_type_node = types_repository.get_type_node("i32");

  // given: arg -> constant integer
  AstConstValue *const_value_node = new AstConstValue(0, 0, "");
  const_value_node->type = ConstValueType::INT;

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
  ASSERT_TRUE(is_valid_call);
  ASSERT_EQ(errors.size(), 0L);

  // clean:
  delete function_node;
  delete function_call_node;
}

//==================================================================================
//          SEMANTIC BRANCHES
//==================================================================================

TEST(SemanticBranches, BoolVar) {
  std::vector<Error> errors;

  // given: source_file
  const char *source_file = "fn my_condition() void {\n"
                            "\tmy_condition bool = false\n"
                            "\n"
                            "\tif my_condition {\n"
                            "\t\tmy_var i32\n"
                            "\t\tmy_var = 34\n"
                            "\t}\n"
                            "}";

  // given: tokens
  Lexer lexer = Lexer(source_file, "file/directory", "FunctionNoRet", errors);
  lexer.tokenize();

  // given: parsed source node
  Parser parser = Parser(errors);
  AstFnDef *fn_def = parser.parse_function_def(lexer)->fn_def();

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(fn_def, nullptr);

  // and given: analizer
  SemanticAnalyzer analizer(errors);

  bool is_valid_proto = analizer.analize_fn_proto(fn_def->proto);

  analizer.enter_fn_scope(fn_def);

  const AstVarDef *var_def = fn_def->block->statements.at(0)->var_def();
  bool is_valid_var_def = analizer.analize_var_def(var_def);

  const AstIfStmnt *if_stmnt = fn_def->block->statements.at(1)->if_stmnt();
  bool is_valid_if_stmnt = analizer.analize_if_stmnt(if_stmnt);

  analizer.exit_fn_scope();

  // then:
  ASSERT_TRUE(is_valid_proto);
  ASSERT_TRUE(is_valid_var_def);
  ASSERT_TRUE(is_valid_if_stmnt);
  ASSERT_EQ(errors.size(), 0L);

  delete fn_def;
}
