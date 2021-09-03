#include "../../src/ast_nodes.hpp"
#include "../../src/lexer.hpp"
#include "../../src/parser.hpp"

#include <gtest/gtest.h>

//==================================================================================
//          PARSE RETURN STATEMENT
//==================================================================================

TEST(ParserHappyStmntTests, RetStmnt) {
  std::vector<Error> errors;
  Lexer lexer("ret myVar * (5 + 8)", "Ret stmnt", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_ret_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_UNARY_EXPR);
  ASSERT_EQ(value_node->op, UnaryExprType::RET);
  ASSERT_NE(value_node->expr, nullptr);
  ASSERT_EQ(value_node->expr->parent, value_node);

  delete value_node;
}

TEST(ParserHappyStmntTests, RetEmptyStmnt) {
  std::vector<Error> errors;
  Lexer lexer("ret ", "RetEmptyStmnt", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_ret_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_UNARY_EXPR);
  ASSERT_EQ(value_node->op, UnaryExprType::RET);
  ASSERT_EQ(value_node->expr, nullptr);

  delete value_node;
}

//==================================================================================
//          PARSE ASSIGN STATEMENT
//==================================================================================

TEST(ParserHappyStmntTests, AssignStmntTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar = 5 * (6 + 9)", "AssignStmntTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_assign_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);
  ASSERT_EQ(value_node->bin_op, BinaryExprType::ASSIGN);
  ASSERT_EQ(value_node->left_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(value_node->right_expr->node_type, AstNodeType::AST_BINARY_EXPR);
  ASSERT_EQ(value_node->left_expr->parent, value_node);
  ASSERT_EQ(value_node->right_expr->parent, value_node);

  delete value_node;
}

//==================================================================================
//          PARSE TYPES
//==================================================================================

TEST(ParserHappyStmntTests, TypeNameIsPrimitiveParse) {
  std::vector<Error> errors;
  Lexer lexer("i32", "TypeNameIsPrimitiveParse", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);

  Parser parser(errors);
  auto value_node = parser.parse_type(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(value_node->type_info->type_id, AstTypeId::INTEGER);
  ASSERT_EQ(value_node->type_info->name, "i32");
  ASSERT_EQ(value_node->type_info->bit_size, 32);
  ASSERT_EQ(value_node->type_info->llvm_type, nullptr);
  ASSERT_TRUE(value_node->type_info->is_signed);
}

TEST(ParserHappyStmntTests, TypeArrayParse) {
  std::vector<Error> errors;
  Lexer lexer("[]i32", "TypeArrayParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_type(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(value_node->type_info->type_id, AstTypeId::ARRAY);
  ASSERT_NE(value_node->child_type, nullptr);
  ASSERT_EQ(value_node->child_type->parent, value_node);
  ASSERT_EQ(value_node->child_type->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(value_node->child_type->type_info->type_id, AstTypeId::INTEGER);
  ASSERT_EQ(value_node->child_type->type_info->name, "i32");
  ASSERT_EQ(value_node->child_type->type_info->bit_size, 32);
  ASSERT_EQ(value_node->child_type->type_info->llvm_type, nullptr);
  ASSERT_TRUE(value_node->child_type->type_info->is_signed);
}

TEST(ParserHappyStmntTests, TypePointerParse) {
  std::vector<Error> errors;
  Lexer lexer("*i32", "TypePointerParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_type(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(value_node->type_info->type_id, AstTypeId::POINTER);
  ASSERT_NE(value_node->child_type, nullptr);
  ASSERT_EQ(value_node->child_type->parent, value_node);
  ASSERT_EQ(value_node->child_type->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(value_node->child_type->type_info->type_id, AstTypeId::INTEGER);
  ASSERT_EQ(value_node->child_type->type_info->name, "i32");
  ASSERT_EQ(value_node->child_type->type_info->bit_size, 32);
  ASSERT_EQ(value_node->child_type->type_info->llvm_type, nullptr);
  ASSERT_TRUE(value_node->child_type->type_info->is_signed);
}

//==================================================================================
//          PARSE VARIABLE DEFINITION STATEMENT
//==================================================================================

TEST(ParserHappyStmntTests, VarDefSimpleTypeParse) {
  std::vector<Error> errors;
  Lexer lexer("myVar i32", "VarDefSimpleTypeParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_vardef_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_VAR_DEF);
  ASSERT_EQ(value_node->name, "myVar");
  ASSERT_EQ(value_node->initializer, nullptr);
  ASSERT_NE(value_node->type, nullptr);
  auto type_node = value_node->type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(type_node->parent, value_node);
  ASSERT_EQ(type_node->type_info->type_id, AstTypeId::INTEGER);
  ASSERT_EQ(type_node->type_info->name, "i32");
  ASSERT_EQ(type_node->type_info->bit_size, 32);
  ASSERT_EQ(type_node->type_info->llvm_type, nullptr);
  ASSERT_TRUE(type_node->type_info->is_signed);

  delete value_node;
}

TEST(ParserHappyStmntTests, VarDefSimpleTypeInitializerParse) {
  std::vector<Error> errors;
  Lexer lexer("myVar i32 = -65", "VarDefSimpleTypeInitializerParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_vardef_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_VAR_DEF);
  ASSERT_EQ(value_node->name, "myVar");

  auto type_node = value_node->type;
  ASSERT_NE(type_node, nullptr);
  ASSERT_EQ(type_node->parent, value_node);
  ASSERT_EQ(type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(type_node->type_info->type_id, AstTypeId::INTEGER);
  ASSERT_EQ(type_node->type_info->name, "i32");
  ASSERT_EQ(type_node->type_info->bit_size, 32);
  ASSERT_EQ(type_node->type_info->llvm_type, nullptr);
  ASSERT_EQ(type_node->type_info->is_signed, true);

  auto init_node_generic = value_node->initializer;
  ASSERT_NE(init_node_generic, nullptr);
  ASSERT_EQ(init_node_generic->parent, value_node);
  ASSERT_EQ(init_node_generic->node_type, AstNodeType::AST_CONST_VALUE);

  auto init_node = init_node_generic->const_value();
  ASSERT_EQ(init_node->type, ConstValueType::INT);
  ASSERT_EQ(init_node->is_negative, true);

  delete value_node;
}

TEST(ParserHappyStmntTests, VarDefSimpleTypeInitializerAddParse) {
  std::vector<Error> errors;
  Lexer lexer("myVar i32 = 65 + PI", "VarDefSimpleTypeInitializerAddParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_vardef_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_VAR_DEF);
  ASSERT_EQ(value_node->name, "myVar");

  auto type_node = value_node->type;
  ASSERT_NE(type_node, nullptr);
  ASSERT_EQ(type_node->parent, value_node);
  ASSERT_EQ(type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(type_node->type_info->type_id, AstTypeId::INTEGER);
  ASSERT_EQ(type_node->type_info->name, "i32");
  ASSERT_EQ(type_node->type_info->bit_size, 32);
  ASSERT_EQ(type_node->type_info->llvm_type, nullptr);
  ASSERT_EQ(type_node->type_info->is_signed, true);

  auto init_node_generic = value_node->initializer;
  ASSERT_NE(init_node_generic, nullptr);
  ASSERT_EQ(init_node_generic->parent, value_node);
  ASSERT_EQ(init_node_generic->node_type, AstNodeType::AST_BINARY_EXPR);

  auto init_node = init_node_generic->binary_expr();
  ASSERT_EQ(init_node->bin_op, BinaryExprType::ADD);
  ASSERT_NE(init_node->left_expr, nullptr);
  ASSERT_NE(init_node->right_expr, nullptr);
  ASSERT_EQ(init_node->left_expr->parent, init_node);
  ASSERT_EQ(init_node->right_expr->parent, init_node);
  ASSERT_EQ(init_node->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(init_node->right_expr->node_type, AstNodeType::AST_SYMBOL);

  delete value_node;
}

TEST(ParserHappyStmntTests, VarDefArrayTypeParse) {
  std::vector<Error> errors;
  Lexer lexer("myVar []i32", "VarDefArrayTypeParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_vardef_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_VAR_DEF);
  ASSERT_EQ(value_node->name, "myVar");
  ASSERT_EQ(value_node->initializer, nullptr);
  ASSERT_NE(value_node->type, nullptr);
  auto type_node = value_node->type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(type_node->parent, value_node);
  ASSERT_EQ(type_node->type_info->type_id, AstTypeId::ARRAY);
  ASSERT_NE(type_node->child_type, nullptr);
  auto data_type_node = type_node->child_type;
  ASSERT_EQ(data_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(data_type_node->parent, type_node);
  ASSERT_EQ(data_type_node->type_info->type_id, AstTypeId::INTEGER);
  ASSERT_EQ(data_type_node->type_info->name, "i32");
  ASSERT_EQ(data_type_node->type_info->bit_size, 32);
  ASSERT_EQ(data_type_node->type_info->llvm_type, nullptr);
  ASSERT_EQ(data_type_node->type_info->is_signed, true);

  delete value_node;
}

TEST(ParserHappyStmntTests, VarDefPointerTypeParse) {
  std::vector<Error> errors;
  Lexer lexer("myVar *i32", "VarDefPointerTypeParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_vardef_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_VAR_DEF);
  ASSERT_EQ(value_node->name, "myVar");
  ASSERT_EQ(value_node->initializer, nullptr);
  ASSERT_NE(value_node->type, nullptr);
  auto type_node = value_node->type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(type_node->parent, value_node);
  ASSERT_EQ(type_node->type_info->type_id, AstTypeId::POINTER);
  ASSERT_NE(type_node->child_type, nullptr);
  auto data_type_node = type_node->child_type;
  ASSERT_EQ(data_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(data_type_node->parent, type_node);
  ASSERT_EQ(data_type_node->type_info->type_id, AstTypeId::INTEGER);
  ASSERT_EQ(data_type_node->type_info->name, "i32");
  ASSERT_EQ(data_type_node->type_info->bit_size, 32);
  ASSERT_EQ(data_type_node->type_info->llvm_type, nullptr);
  ASSERT_EQ(data_type_node->type_info->is_signed, true);

  delete value_node;
}

//==================================================================================
//          PARSE ANY STATEMENT
//==================================================================================

TEST(ParserHappyStmntTests, StatementVarDefSimpleTypeParse) {
  std::vector<Error> errors;
  Lexer lexer("myVar i32", "VarDefSimpleTypeParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto node = parser.parse_statement(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(node->node_type, AstNodeType::AST_VAR_DEF);
  auto value_node = node->var_def();
  ASSERT_EQ(value_node->name, "myVar");
  ASSERT_EQ(value_node->initializer, nullptr);
  ASSERT_NE(value_node->type, nullptr);
  auto type_node = value_node->type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(type_node->parent, value_node);
  ASSERT_EQ(type_node->type_info->type_id, AstTypeId::INTEGER);
  ASSERT_EQ(type_node->type_info->name, "i32");
  ASSERT_EQ(type_node->type_info->bit_size, 32);
  ASSERT_EQ(type_node->type_info->llvm_type, nullptr);
  ASSERT_EQ(type_node->type_info->is_signed, true);

  delete value_node;
}

TEST(ParserHappyStmntTests, StatementVarDefArrayTypeParse) {
  std::vector<Error> errors;
  Lexer lexer("myVar []i32", "VarDefArrayTypeParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto node = parser.parse_statement(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(node->node_type, AstNodeType::AST_VAR_DEF);
  auto value_node = node->var_def();
  ASSERT_EQ(value_node->name, "myVar");
  ASSERT_EQ(value_node->initializer, nullptr);
  ASSERT_NE(value_node->type, nullptr);
  auto type_node = value_node->type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(type_node->parent, value_node);
  ASSERT_EQ(type_node->type_info->type_id, AstTypeId::ARRAY);
  ASSERT_NE(type_node->child_type, nullptr);
  auto data_type_node = type_node->child_type;
  ASSERT_EQ(data_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(data_type_node->parent, type_node);
  ASSERT_EQ(data_type_node->type_info->type_id, AstTypeId::INTEGER);
  ASSERT_EQ(data_type_node->type_info->name, "i32");
  ASSERT_EQ(data_type_node->type_info->bit_size, 32);
  ASSERT_EQ(data_type_node->type_info->llvm_type, nullptr);
  ASSERT_EQ(data_type_node->type_info->is_signed, true);

  delete value_node;
}

TEST(ParserHappyStmntTests, StatementVarDefPointerTypeParse) {
  std::vector<Error> errors;
  Lexer lexer("myVar *i32", "VarDefPointerTypeParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto node = parser.parse_statement(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(node, nullptr);
  ASSERT_EQ(node->node_type, AstNodeType::AST_VAR_DEF);

  auto value_node = node->var_def();
  ASSERT_EQ(value_node->name, "myVar");
  ASSERT_EQ(value_node->initializer, nullptr);
  ASSERT_NE(value_node->type, nullptr);
  auto type_node = value_node->type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(type_node->parent, value_node);
  ASSERT_EQ(type_node->type_info->type_id, AstTypeId::POINTER);
  ASSERT_NE(type_node->child_type, nullptr);
  auto data_type_node = type_node->child_type;
  ASSERT_EQ(data_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(data_type_node->parent, type_node);
  ASSERT_EQ(data_type_node->type_info->type_id, AstTypeId::INTEGER);
  ASSERT_EQ(data_type_node->type_info->name, "i32");
  ASSERT_EQ(data_type_node->type_info->bit_size, 32);
  ASSERT_EQ(data_type_node->type_info->llvm_type, nullptr);
  ASSERT_EQ(data_type_node->type_info->is_signed, true);

  delete value_node;
}

TEST(ParserHappyStmntTests, StatementAssignStmntTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar = 5 * (6 + 9)", "AssignStmntTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto node = parser.parse_statement(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(node, nullptr);
  ASSERT_EQ(node->node_type, AstNodeType::AST_BINARY_EXPR);

  auto value_node = node->binary_expr();
  ASSERT_EQ(value_node->bin_op, BinaryExprType::ASSIGN);
  ASSERT_NE(value_node->left_expr, nullptr);
  ASSERT_NE(value_node->right_expr, nullptr);
  ASSERT_EQ(value_node->left_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(value_node->right_expr->node_type, AstNodeType::AST_BINARY_EXPR);
  ASSERT_EQ(value_node->left_expr->parent, value_node);
  ASSERT_EQ(value_node->right_expr->parent, value_node);

  delete value_node;
}

TEST(ParserHappyStmntTests, StatementRetStmnt) {
  std::vector<Error> errors;
  Lexer lexer("ret myVar * (5 + 8)", "Ret stmnt", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto node = parser.parse_statement(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(node, nullptr);
  ASSERT_EQ(node->node_type, AstNodeType::AST_UNARY_EXPR);

  auto value_node = node->unary_expr();
  ASSERT_EQ(value_node->op, UnaryExprType::RET);
  ASSERT_NE(value_node->expr, nullptr);
  ASSERT_EQ(value_node->expr->parent, value_node);

  delete value_node;
}

TEST(ParserHappyStmntTests, StatementRetEmptyStmnt) {
  std::vector<Error> errors;
  Lexer lexer("ret ", "RetEmptyStmnt", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto node = parser.parse_statement(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(node, nullptr);
  ASSERT_EQ(node->node_type, AstNodeType::AST_UNARY_EXPR);

  auto value_node = node->unary_expr();
  ASSERT_EQ(value_node->op, UnaryExprType::RET);
  ASSERT_EQ(value_node->expr, nullptr);

  delete value_node;
}

//==================================================================================
//          PARSE BLOCK STATEMENT
//==================================================================================

TEST(ParserHappyStmntTests, BlockEmptyTest) {
  std::vector<Error> errors;
  Lexer lexer("{}", "BlockEmptyTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstBlock *value_node = parser.parse_block(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BLOCK);
  ASSERT_EQ(value_node->statements.size(), 0L);

  delete value_node;
}

TEST(ParserHappyStmntTests, BlockNoSpaceNearCurliesTest) {
  std::vector<Error> errors;
  Lexer lexer("{myVar i32\nret myvar}", "BlockNoSpaceNearCurliesTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstBlock *value_node = parser.parse_block(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BLOCK);
  ASSERT_EQ(value_node->statements.size(), 2);

  ASSERT_EQ(value_node->statements.at(0)->node_type, AstNodeType::AST_VAR_DEF);

  AstVarDef *var_def_node = value_node->statements.at(0)->var_def();
  ASSERT_EQ(var_def_node->parent, value_node);
  ASSERT_EQ(var_def_node->name, "myVar");
  ASSERT_NE(var_def_node->type, nullptr);

  AstType *type_node = var_def_node->type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(type_node->type_info->type_id, AstTypeId::INTEGER);
  ASSERT_EQ(type_node->type_info->name, "i32");
  ASSERT_EQ(type_node->type_info->bit_size, 32);
  ASSERT_EQ(type_node->type_info->llvm_type, nullptr);
  ASSERT_EQ(type_node->type_info->is_signed, true);

  ASSERT_EQ(value_node->statements.at(1)->node_type, AstNodeType::AST_UNARY_EXPR);
  AstUnaryExpr *ret_node = value_node->statements.at(1)->unary_expr();
  ASSERT_EQ(ret_node->parent, value_node);
  ASSERT_EQ(ret_node->op, UnaryExprType::RET);
  ASSERT_NE(ret_node->expr, nullptr);

  delete value_node;
}

TEST(ParserHappyStmntTests, BlockSpaceNearCurliesTest) {
  std::vector<Error> errors;
  Lexer lexer("{ myVar i32\nret myvar }", "BlockSpaceNearCurliesTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstBlock *value_node = parser.parse_block(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BLOCK);
  ASSERT_EQ(value_node->statements.size(), 2);

  ASSERT_EQ(value_node->statements.at(0)->node_type, AstNodeType::AST_VAR_DEF);

  AstVarDef *var_def_node = value_node->statements.at(0)->var_def();
  ASSERT_EQ(var_def_node->parent, value_node);
  ASSERT_EQ(var_def_node->name, "myVar");
  ASSERT_NE(var_def_node->type, nullptr);

  AstType *type_node = var_def_node->type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(type_node->type_info->type_id, AstTypeId::INTEGER);
  ASSERT_EQ(type_node->type_info->name, "i32");
  ASSERT_EQ(type_node->type_info->bit_size, 32);
  ASSERT_EQ(type_node->type_info->llvm_type, nullptr);
  ASSERT_EQ(type_node->type_info->is_signed, true);

  ASSERT_EQ(value_node->statements.at(1)->node_type, AstNodeType::AST_UNARY_EXPR);
  AstUnaryExpr *ret_node = value_node->statements.at(1)->unary_expr();
  ASSERT_EQ(ret_node->parent, value_node);
  ASSERT_EQ(ret_node->op, UnaryExprType::RET);
  ASSERT_NE(ret_node->expr, nullptr);

  delete value_node;
}

TEST(ParserHappyStmntTests, BlockNewlinesNearCurliesTest) {
  std::vector<Error> errors;
  Lexer lexer("{\n\tmyVar i32\n\tret myvar\n}", "BlockNewlinesNearCurliesTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstBlock *value_node = parser.parse_block(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BLOCK);
  ASSERT_EQ(value_node->statements.size(), 2);

  ASSERT_EQ(value_node->statements.at(0)->node_type, AstNodeType::AST_VAR_DEF);
  AstVarDef *var_def_node = value_node->statements.at(0)->var_def();
  ASSERT_EQ(var_def_node->parent, value_node);
  ASSERT_EQ(var_def_node->name, "myVar");
  ASSERT_NE(var_def_node->type, nullptr);

  AstType *type_node = var_def_node->type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(type_node->type_info->type_id, AstTypeId::INTEGER);
  ASSERT_EQ(type_node->type_info->name, "i32");
  ASSERT_EQ(type_node->type_info->bit_size, 32);
  ASSERT_EQ(type_node->type_info->llvm_type, nullptr);
  ASSERT_EQ(type_node->type_info->is_signed, true);

  ASSERT_EQ(value_node->statements.at(1)->node_type, AstNodeType::AST_UNARY_EXPR);
  AstUnaryExpr *ret_node = value_node->statements.at(1)->unary_expr();
  ASSERT_EQ(ret_node->parent, value_node);
  ASSERT_EQ(ret_node->op, UnaryExprType::RET);
  ASSERT_NE(ret_node->expr, nullptr);

  delete value_node;
}

TEST(ParserHappyStmntTests, BlockSpacesBetweenNewlinesTest) {
  std::vector<Error> errors;
  Lexer lexer("{ \n  \t myVar i32   \n  \t  ret myvar  \n   }", "BlockSpacesBetweenNewlinesTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstBlock *value_node = parser.parse_block(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BLOCK);
  ASSERT_EQ(value_node->statements.size(), 2);

  ASSERT_EQ(value_node->statements.at(0)->node_type, AstNodeType::AST_VAR_DEF);
  AstVarDef *var_def_node = value_node->statements.at(0)->var_def();
  ASSERT_EQ(var_def_node->parent, value_node);
  ASSERT_EQ(var_def_node->name, "myVar");
  ASSERT_NE(var_def_node->type, nullptr);

  AstType *type_node = var_def_node->type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(type_node->type_info->type_id, AstTypeId::INTEGER);
  ASSERT_EQ(type_node->type_info->name, "i32");
  ASSERT_EQ(type_node->type_info->bit_size, 32);
  ASSERT_EQ(type_node->type_info->llvm_type, nullptr);
  ASSERT_EQ(type_node->type_info->is_signed, true);

  ASSERT_EQ(value_node->statements.at(1)->node_type, AstNodeType::AST_UNARY_EXPR);
  AstUnaryExpr *ret_node = value_node->statements.at(1)->unary_expr();
  ASSERT_EQ(ret_node->parent, value_node);
  ASSERT_EQ(ret_node->op, UnaryExprType::RET);
  ASSERT_NE(ret_node->expr, nullptr);

  delete value_node;
}

//==================================================================================
//          PARSE FUNCTION DECLARATION STATEMENT
//==================================================================================

TEST(ParserHappyStmntTests, FuncProtoEmptyParamTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc() void", "FuncProtoEmptyParamTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstFnProto *value_node = parser.parse_function_proto(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_FN_PROTO);
  ASSERT_EQ(value_node->name, "myFunc");
  ASSERT_EQ(value_node->params.size(), 0);
  ASSERT_NE(value_node->return_type, nullptr);

  auto ret_type_node = value_node->return_type;
  ASSERT_EQ(ret_type_node->parent, value_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(ret_type_node->type_info->name, "void");
  ASSERT_EQ(ret_type_node->type_info->type_id, AstTypeId::VOID);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncProtoSingleParamTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc(param1 i32) void", "FuncProtoSingleParamTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstFnProto *value_node = parser.parse_function_proto(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_FN_PROTO);
  ASSERT_EQ(value_node->name, "myFunc");
  ASSERT_NE(value_node->return_type, nullptr);
  ASSERT_EQ(value_node->params.size(), 1L);

  AstParamDef *param_node = value_node->params.at(0L);
  ASSERT_EQ(param_node->node_type, AstNodeType::AST_PARAM_DEF);
  ASSERT_EQ(param_node->parent, value_node);
  ASSERT_EQ(param_node->name, "param1");
  ASSERT_NE(param_node->type, nullptr);
  ASSERT_EQ(param_node->type->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(param_node->type->parent, param_node);
  ASSERT_EQ(param_node->type->type_info->name, "i32");
  ASSERT_EQ(param_node->type->type_info->type_id, AstTypeId::INTEGER);

  AstType *ret_type_node = value_node->return_type;
  ASSERT_EQ(ret_type_node->parent, value_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(ret_type_node->type_info->name, "void");
  ASSERT_EQ(ret_type_node->type_info->type_id, AstTypeId::VOID);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncProtoMultiParamTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc(param1 i32, param1 i32, param1 i32) i32", "FuncProtoMultiParamTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstFnProto *value_node = parser.parse_function_proto(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_FN_PROTO);
  ASSERT_EQ(value_node->name, "myFunc");
  ASSERT_NE(value_node->return_type, nullptr);
  ASSERT_EQ(value_node->params.size(), 3L);

  for (auto param_node : value_node->params) {
    ASSERT_EQ(param_node->node_type, AstNodeType::AST_PARAM_DEF);
    ASSERT_EQ(param_node->parent, value_node);
    ASSERT_EQ(param_node->name, "param1");
    ASSERT_NE(param_node->type, nullptr);
    ASSERT_EQ(param_node->type->node_type, AstNodeType::AST_TYPE);
    ASSERT_EQ(param_node->type->parent, param_node);
    ASSERT_EQ(param_node->type->type_info->name, "i32");
    ASSERT_EQ(param_node->type->type_info->type_id, AstTypeId::INTEGER);
  }

  AstType *ret_type_node = value_node->return_type;
  ASSERT_EQ(ret_type_node->parent, value_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(ret_type_node->type_info->name, "i32");
  ASSERT_EQ(ret_type_node->type_info->type_id, AstTypeId::INTEGER);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncProtoMultiLineTest) {
  std::vector<Error> errors;
  Lexer lexer("fn\nmyFunc\n(param1 i32\n, param1 i32,\n param1\ni32)\ni32", "FuncProtoMultiLineTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstFnProto *value_node = parser.parse_function_proto(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_FN_PROTO);
  ASSERT_EQ(value_node->name, "myFunc");
  ASSERT_NE(value_node->return_type, nullptr);
  ASSERT_EQ(value_node->params.size(), 3L);

  for (auto param_node : value_node->params) {
    ASSERT_EQ(param_node->node_type, AstNodeType::AST_PARAM_DEF);
    ASSERT_EQ(param_node->parent, value_node);
    ASSERT_EQ(param_node->name, "param1");
    ASSERT_NE(param_node->type, nullptr);
    ASSERT_EQ(param_node->type->node_type, AstNodeType::AST_TYPE);
    ASSERT_EQ(param_node->type->parent, param_node);
    ASSERT_EQ(param_node->type->type_info->name, "i32");
    ASSERT_EQ(param_node->type->type_info->type_id, AstTypeId::INTEGER);
  }

  AstType *ret_type_node = value_node->return_type;
  ASSERT_EQ(ret_type_node->parent, value_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(ret_type_node->type_info->name, "i32");
  ASSERT_EQ(ret_type_node->type_info->type_id, AstTypeId::INTEGER);

  delete value_node;
}

//==================================================================================
//          PARSE FUNCTION DEFINITION STATEMENT
//==================================================================================

TEST(ParserHappyStmntTests, FuncDefNoParamsVoidBlockTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc() void {}", "FuncDefNoParamsVoidBlockTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *node = parser.parse_function_def(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(node, nullptr);
  ASSERT_EQ(node->node_type, AstNodeType::AST_FN_DEF);

  AstFnDef *value_node = node->function_def();
  ASSERT_NE(value_node->block, nullptr);
  ASSERT_NE(value_node->proto, nullptr);

  AstBlock *block_node = value_node->block;
  ASSERT_EQ(block_node->parent, value_node);
  ASSERT_EQ(block_node->node_type, AstNodeType::AST_BLOCK);
  ASSERT_EQ(block_node->statements.size(), 0L);

  AstFnProto *proto_node = value_node->proto;
  ASSERT_EQ(proto_node->parent, value_node);
  ASSERT_EQ(proto_node->node_type, AstNodeType::AST_FN_PROTO);
  ASSERT_EQ(proto_node->name, "myFunc");
  ASSERT_EQ(proto_node->params.size(), 0L);

  AstType *ret_type_node = proto_node->return_type;
  ASSERT_NE(ret_type_node, nullptr);
  ASSERT_EQ(ret_type_node->parent, proto_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(ret_type_node->type_info->name, "void");
  ASSERT_EQ(ret_type_node->type_info->type_id, AstTypeId::VOID);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncDefSingleParamsVoidBlockTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc(param1 i32) i32 {}", "FuncDefSingleParamsVoidBlockTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *node = parser.parse_function_def(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(node, nullptr);
  ASSERT_EQ(node->node_type, AstNodeType::AST_FN_DEF);

  AstFnDef *value_node = node->function_def();
  ASSERT_NE(value_node->block, nullptr);
  ASSERT_NE(value_node->proto, nullptr);

  AstBlock *block_node = value_node->block;
  ASSERT_EQ(block_node->parent, value_node);
  ASSERT_EQ(block_node->node_type, AstNodeType::AST_BLOCK);
  ASSERT_EQ(block_node->statements.size(), 0L);

  AstFnProto *proto_node = value_node->proto;
  ASSERT_EQ(proto_node->parent, value_node);
  ASSERT_EQ(proto_node->node_type, AstNodeType::AST_FN_PROTO);
  ASSERT_EQ(proto_node->name, "myFunc");
  ASSERT_NE(proto_node->return_type, nullptr);
  ASSERT_EQ(proto_node->params.size(), 1L);

  AstParamDef *param_node = proto_node->params.at(0L);
  ASSERT_EQ(param_node->node_type, AstNodeType::AST_PARAM_DEF);
  ASSERT_EQ(param_node->parent, proto_node);
  ASSERT_EQ(param_node->name, "param1");
  ASSERT_NE(param_node->type, nullptr);
  ASSERT_EQ(param_node->type->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(param_node->type->parent, param_node);
  ASSERT_EQ(param_node->type->type_info->name, "i32");
  ASSERT_EQ(param_node->type->type_info->type_id, AstTypeId::INTEGER);

  AstType *ret_type_node = proto_node->return_type;
  ASSERT_NE(ret_type_node, nullptr);
  ASSERT_EQ(ret_type_node->parent, proto_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(ret_type_node->type_info->name, "i32");
  ASSERT_EQ(ret_type_node->type_info->type_id, AstTypeId::INTEGER);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncDefMultiParamsVoidBlockTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc(param1 i32, param1 i32, param1 i32) i32 {}", "FuncDefMultiParamsVoidBlockTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *node = parser.parse_function_def(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(node, nullptr);
  ASSERT_EQ(node->node_type, AstNodeType::AST_FN_DEF);

  AstFnDef *value_node = node->function_def();
  ASSERT_NE(value_node->block, nullptr);
  ASSERT_NE(value_node->proto, nullptr);

  AstBlock *block_node = value_node->block;
  ASSERT_EQ(block_node->parent, value_node);
  ASSERT_EQ(block_node->node_type, AstNodeType::AST_BLOCK);
  ASSERT_EQ(block_node->statements.size(), 0L);

  AstFnProto *proto_node = value_node->proto;
  ASSERT_EQ(proto_node->parent, value_node);
  ASSERT_EQ(proto_node->node_type, AstNodeType::AST_FN_PROTO);
  ASSERT_EQ(proto_node->name, "myFunc");
  ASSERT_NE(proto_node->return_type, nullptr);
  ASSERT_EQ(proto_node->params.size(), 3L);

  for (AstParamDef *param_node : proto_node->params) {
    ASSERT_EQ(param_node->node_type, AstNodeType::AST_PARAM_DEF);
    ASSERT_EQ(param_node->parent, proto_node);
    ASSERT_EQ(param_node->name, "param1");
    ASSERT_NE(param_node->type, nullptr);
    ASSERT_EQ(param_node->type->node_type, AstNodeType::AST_TYPE);
    ASSERT_EQ(param_node->type->parent, param_node);
    ASSERT_EQ(param_node->type->type_info->name, "i32");
    ASSERT_EQ(param_node->type->type_info->type_id, AstTypeId::INTEGER);
  }

  AstType *ret_type_node = proto_node->return_type;
  ASSERT_NE(ret_type_node, nullptr);
  ASSERT_EQ(ret_type_node->parent, proto_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(ret_type_node->type_info->name, "i32");
  ASSERT_EQ(ret_type_node->type_info->type_id, AstTypeId::INTEGER);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncDefNoParamsTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc() void {\nmyVar i32\n}", "FuncDefNoParamsTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *node = parser.parse_function_def(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(node, nullptr);
  ASSERT_EQ(node->node_type, AstNodeType::AST_FN_DEF);

  AstFnDef *value_node = node->function_def();
  ASSERT_NE(value_node->block, nullptr);
  ASSERT_NE(value_node->proto, nullptr);

  AstBlock *block_node = value_node->block;
  ASSERT_EQ(block_node->parent, value_node);
  ASSERT_EQ(block_node->node_type, AstNodeType::AST_BLOCK);
  ASSERT_EQ(block_node->statements.size(), 1L);

  AstVarDef *var_def_node = block_node->statements.at(0)->var_def();
  ASSERT_NE(var_def_node, nullptr);
  ASSERT_EQ(var_def_node->parent, block_node);
  ASSERT_EQ(var_def_node->node_type, AstNodeType::AST_VAR_DEF);
  ASSERT_EQ(var_def_node->name, "myVar");
  ASSERT_NE(var_def_node->type, nullptr);
  ASSERT_EQ(var_def_node->initializer, nullptr);

  AstType *type_node = var_def_node->type;
  ASSERT_EQ(type_node->parent, var_def_node);
  ASSERT_EQ(type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(type_node->type_info->type_id, AstTypeId::INTEGER);
  ASSERT_EQ(type_node->type_info->name, "i32");

  AstFnProto *proto_node = value_node->proto;
  ASSERT_EQ(proto_node->parent, value_node);
  ASSERT_EQ(proto_node->node_type, AstNodeType::AST_FN_PROTO);
  ASSERT_EQ(proto_node->name, "myFunc");
  ASSERT_EQ(proto_node->params.size(), 0L);

  AstType *ret_type_node = proto_node->return_type;
  ASSERT_NE(ret_type_node, nullptr);
  ASSERT_EQ(ret_type_node->parent, proto_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(ret_type_node->type_info->name, "void");
  ASSERT_EQ(ret_type_node->type_info->type_id, AstTypeId::VOID);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncDefSingleParamsTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc(param1 i32) i32 {\nret param1 + 5\n}", "FuncDefSingleParamsTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *node = parser.parse_function_def(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(node, nullptr);
  ASSERT_EQ(node->node_type, AstNodeType::AST_FN_DEF);

  AstFnDef *value_node = node->function_def();
  ASSERT_NE(value_node->block, nullptr);
  ASSERT_NE(value_node->proto, nullptr);

  AstBlock *block_node = value_node->block;
  ASSERT_EQ(block_node->parent, value_node);
  ASSERT_EQ(block_node->node_type, AstNodeType::AST_BLOCK);
  ASSERT_EQ(block_node->statements.size(), 1L);

  AstUnaryExpr *ret_node = block_node->statements.at(0)->unary_expr();
  ASSERT_NE(ret_node, nullptr);
  ASSERT_EQ(ret_node->parent, block_node);
  ASSERT_EQ(ret_node->node_type, AstNodeType::AST_UNARY_EXPR);
  ASSERT_EQ(ret_node->op, UnaryExprType::RET);
  ASSERT_NE(ret_node->expr, nullptr);
  ASSERT_EQ(ret_node->expr->parent, ret_node);
  ASSERT_EQ(ret_node->expr->node_type, AstNodeType::AST_BINARY_EXPR);
  ASSERT_EQ(ret_node->expr->binary_expr()->bin_op, BinaryExprType::ADD);

  AstFnProto *proto_node = value_node->proto;
  ASSERT_EQ(proto_node->parent, value_node);
  ASSERT_EQ(proto_node->node_type, AstNodeType::AST_FN_PROTO);
  ASSERT_EQ(proto_node->name, "myFunc");
  ASSERT_NE(proto_node->return_type, nullptr);
  ASSERT_EQ(proto_node->params.size(), 1L);

  AstParamDef *param_node = proto_node->params.at(0L);
  ASSERT_EQ(param_node->node_type, AstNodeType::AST_PARAM_DEF);
  ASSERT_EQ(param_node->parent, proto_node);
  ASSERT_EQ(param_node->name, "param1");
  ASSERT_NE(param_node->type, nullptr);
  ASSERT_EQ(param_node->type->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(param_node->type->parent, param_node);
  ASSERT_EQ(param_node->type->type_info->name, "i32");
  ASSERT_EQ(param_node->type->type_info->type_id, AstTypeId::INTEGER);

  AstType *ret_type_node = proto_node->return_type;
  ASSERT_NE(ret_type_node, nullptr);
  ASSERT_EQ(ret_type_node->parent, proto_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(ret_type_node->type_info->name, "i32");
  ASSERT_EQ(ret_type_node->type_info->type_id, AstTypeId::INTEGER);

  delete value_node;
}

//==================================================================================
//          PARSE FUNCTION CALL STATEMENT
//==================================================================================

TEST(ParserHappyStmntTests, FuncCallNoParamsTest) {
  std::vector<Error> errors;
  Lexer lexer("myFunc()", "FuncCallNoParamsTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstFnCallExpr *value_node = parser.parse_function_call(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_FN_CALL_EXPR);
  ASSERT_EQ(value_node->fn_name, "myFunc");
  ASSERT_EQ(value_node->args.size(), 0L);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncCall1ParamTest) {
  std::vector<Error> errors;
  Lexer lexer("myFunc(5)", "FuncCall1ParamTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstFnCallExpr *value_node = parser.parse_function_call(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_FN_CALL_EXPR);
  ASSERT_EQ(value_node->fn_name, "myFunc");
  ASSERT_EQ(value_node->args.size(), 1L);

  AstNode *param_node = value_node->args.at(0);
  ASSERT_NE(param_node, nullptr);
  ASSERT_EQ(param_node->parent, value_node);
  ASSERT_EQ(param_node->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(param_node->const_value()->type, ConstValueType::INT);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncCall1ParamAddOpTest) {
  std::vector<Error> errors;
  Lexer lexer("85 + myFunc(5)", "FuncCall1ParamTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_algebraic_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *bin_expr = value_node->binary_expr();
  ASSERT_EQ(bin_expr->bin_op, BinaryExprType::ADD);
  ASSERT_NE(bin_expr->left_expr, nullptr);
  ASSERT_EQ(bin_expr->left_expr->node_type, AstNodeType::AST_CONST_VALUE);

  const AstConstValue *const_value_node = bin_expr->left_expr->const_value();
  ASSERT_EQ(const_value_node->parent, bin_expr);
  ASSERT_EQ(const_value_node->type, ConstValueType::INT);
  ASSERT_EQ(const_value_node->is_negative, false);
  ASSERT_EQ(strcmp(const_value_node->number, "85"), 0);

  ASSERT_NE(bin_expr->right_expr, nullptr);
  ASSERT_EQ(bin_expr->right_expr->node_type, AstNodeType::AST_FN_CALL_EXPR);

  const AstFnCallExpr *fn_call_node = bin_expr->right_expr->func_call();
  ASSERT_EQ(fn_call_node->parent, bin_expr);
  ASSERT_EQ(fn_call_node->fn_name, "myFunc");
  ASSERT_EQ(fn_call_node->args.size(), 1L);

  const AstNode *arg_node = fn_call_node->args.at(0);
  ASSERT_NE(arg_node, nullptr);
  ASSERT_EQ(arg_node->parent, fn_call_node);
  ASSERT_EQ(arg_node->node_type, AstNodeType::AST_CONST_VALUE);

  const AstConstValue *const_value_arg = arg_node->const_value();
  ASSERT_EQ(const_value_arg->type, ConstValueType::INT);
  ASSERT_EQ(strcmp(const_value_arg->number, "5"), 0);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncCallNestedTest) {
  std::vector<Error> errors;
  Lexer lexer("myFunc(myFunc2(5))", "FuncCallNestedTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstFnCallExpr *value_node = parser.parse_function_call(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_FN_CALL_EXPR);
  ASSERT_EQ(value_node->fn_name, "myFunc");
  ASSERT_EQ(value_node->args.size(), 1L);

  const AstNode *param_node_0 = value_node->args.at(0);
  ASSERT_NE(param_node_0, nullptr);
  ASSERT_EQ(param_node_0->parent, value_node);
  ASSERT_EQ(param_node_0->node_type, AstNodeType::AST_FN_CALL_EXPR);

  const AstFnCallExpr *param_node_func = param_node_0->func_call();
  ASSERT_EQ(param_node_func->fn_name, "myFunc2");
  ASSERT_EQ(param_node_func->args.size(), 1L);

  const AstNode *param_node_1 = param_node_func->args.at(0);
  ASSERT_NE(param_node_1, nullptr);
  ASSERT_EQ(param_node_1->parent, param_node_func);
  ASSERT_EQ(param_node_1->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(param_node_1->const_value()->type, ConstValueType::INT);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncCallMultiParamsTest) {
  std::vector<Error> errors;
  Lexer lexer("myFunc(myVar, myFunc2(5))", "FuncCallMultiParamsTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstFnCallExpr *value_node = parser.parse_function_call(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_FN_CALL_EXPR);
  ASSERT_EQ(value_node->fn_name, "myFunc");
  ASSERT_EQ(value_node->args.size(), 2L);

  const AstNode *param_node_0 = value_node->args.at(0);
  ASSERT_NE(param_node_0, nullptr);
  ASSERT_EQ(param_node_0->parent, value_node);
  ASSERT_EQ(param_node_0->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(param_node_0->symbol()->token->id, TokenId::IDENTIFIER);

  const AstNode *param_node_1 = value_node->args.at(1);
  ASSERT_NE(param_node_1, nullptr);
  ASSERT_EQ(param_node_1->node_type, AstNodeType::AST_FN_CALL_EXPR);

  const AstFnCallExpr *param_node_func = param_node_1->func_call();
  ASSERT_EQ(param_node_func->parent, value_node);
  ASSERT_EQ(param_node_func->fn_name, "myFunc2");
  ASSERT_EQ(param_node_func->args.size(), 1L);

  const AstNode *func_param_node = param_node_func->args.at(0);
  ASSERT_NE(func_param_node, nullptr);
  ASSERT_EQ(func_param_node->parent, param_node_func);
  ASSERT_EQ(func_param_node->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(func_param_node->const_value()->type, ConstValueType::INT);

  delete value_node;
}

//==================================================================================
//          PARSE FULL PROGRAM
//==================================================================================

TEST(ParserHappyStmntTests, FullProgramNoNewLineEnd) {
  const char *source_code = "/* Test program */\n" // multiline comment
                            "\n"
                            ";\n"         // empty statement : discarded
                            ";"           // empty statement : discarded
                            "myVar i32\n" // variable definition
                            "myVar\n"     // unused variable reference : discarded
                            "myVar;\n"    // unused variable reference : discarded
                            "\n"
                            "fn myFunc() void {\n"
                            "\tret\n"
                            "}";

  std::vector<Error> errors;
  Lexer lexer(source_code, "FullProgramNoNewLineEnd", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstSourceCode *source_code_node = parser.parse(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(source_code_node, nullptr);
  ASSERT_EQ(source_code_node->node_type, AstNodeType::AST_SOURCE_CODE);
  ASSERT_EQ(source_code_node->children.size(), 2L);
  auto main_stmnts = source_code_node->children;

  const AstNode *var_def_node = main_stmnts.at(0);
  ASSERT_NE(var_def_node, nullptr);
  ASSERT_EQ(var_def_node->node_type, AstNodeType::AST_VAR_DEF);

  const AstVarDef *var_def_expr = var_def_node->var_def();
  ASSERT_EQ(var_def_expr->parent, source_code_node);
  ASSERT_EQ(var_def_expr->name, "myVar");
  ASSERT_NE(var_def_expr->type, nullptr);

  const AstType *var_type = var_def_expr->type;
  ASSERT_EQ(var_type->parent, var_def_expr);
  ASSERT_EQ(var_type->type_info->type_id, AstTypeId::INTEGER);
  ASSERT_EQ(var_type->child_type, nullptr);
  ASSERT_NE(var_type->type_info->name, "i32");
  ASSERT_NE(var_type->type_info->bit_size, 32);
  ASSERT_NE(var_type->type_info->is_signed, true);

  const AstNode *func_def_node = main_stmnts.at(1);
  ASSERT_NE(func_def_node, nullptr);
  ASSERT_EQ(func_def_node->node_type, AstNodeType::AST_FN_DEF);

  const AstFnDef *fn_def = func_def_node->function_def();
  ASSERT_EQ(fn_def->parent, source_code_node);
  ASSERT_NE(fn_def->block, nullptr);
  ASSERT_NE(fn_def->proto, nullptr);

  const AstFnProto *proto_node = fn_def->proto;
  ASSERT_EQ(proto_node->parent, fn_def);
  ASSERT_EQ(proto_node->node_type, AstNodeType::AST_FN_PROTO);
  ASSERT_EQ(proto_node->name, "myFunc");
  ASSERT_EQ(proto_node->params.size(), 0L);

  const AstBlock *block_node = fn_def->block;
  ASSERT_EQ(block_node->parent, fn_def);
  ASSERT_EQ(block_node->node_type, AstNodeType::AST_BLOCK);
  ASSERT_EQ(block_node->statements.size(), 1L);

  const AstNode *ret_node = block_node->statements.at(0);
  ASSERT_NE(ret_node, nullptr);
  ASSERT_EQ(ret_node->node_type, AstNodeType::AST_UNARY_EXPR);

  const AstUnaryExpr *ret_expr = ret_node->unary_expr();
  ASSERT_EQ(ret_expr->parent, block_node);
  ASSERT_EQ(ret_expr->op, UnaryExprType::RET);
  ASSERT_EQ(ret_expr->expr, nullptr);

  const AstType *ret_type_node = proto_node->return_type;
  ASSERT_NE(ret_type_node, nullptr);
  ASSERT_EQ(ret_type_node->parent, proto_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(ret_type_node->type_info->name, "void");
  ASSERT_EQ(ret_type_node->type_info->type_id, AstTypeId::VOID);

  delete source_code_node;
}
