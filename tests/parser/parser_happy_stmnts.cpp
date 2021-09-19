#include "../../src/ast_nodes.hpp"
#include "../../src/lexer.hpp"
#include "../../src/parser.hpp"

#include <gtest/gtest.h>

//==================================================================================
//          PARSE RETURN STATEMENT
//==================================================================================

TEST(ParserHappyRetStmntTests, ComplexExpr) {
  std::vector<Error> errors;
  Lexer lexer("ret myVar * (5 + 8)", "file/directory", "Ret stmnt", errors);
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

TEST(ParserHappyRetStmntTests, EmptyExpr) {
  std::vector<Error> errors;
  Lexer lexer("ret ", "file/directory", "RetEmptyStmnt", errors);
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

TEST(ParserHappyAssignStmntTests, ComplexExpr) {
  std::vector<Error> errors;
  Lexer lexer("myVar = 5 * (6 + 9)", "file/directory", "AssignStmntTest", errors);
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

TEST(ParserHappyTypeStmntTests, NameIsPrimitiveParse) {
  std::vector<Error> errors;
  Lexer lexer("s32", "file/directory", "TypeNameIsPrimitiveParse", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);

  Parser parser(errors);
  auto value_node = parser.parse_type(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(value_node->type_info->type_id, AstTypeId::INTEGER);
  ASSERT_EQ(value_node->type_info->name, "s32");
  ASSERT_EQ(value_node->type_info->bit_size, 32);
  ASSERT_EQ(value_node->type_info->llvm_type, nullptr);
  ASSERT_TRUE(value_node->type_info->is_signed);
}

TEST(ParserHappyTypeStmntTests, ArrayParse) {
  std::vector<Error> errors;
  Lexer lexer("[]s32", "file/directory", "TypeArrayParse", errors);
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
  ASSERT_EQ(value_node->child_type->type_info->name, "s32");
  ASSERT_EQ(value_node->child_type->type_info->bit_size, 32);
  ASSERT_EQ(value_node->child_type->type_info->llvm_type, nullptr);
  ASSERT_TRUE(value_node->child_type->type_info->is_signed);
}

TEST(ParserHappyTypeStmntTests, PointerParse) {
  std::vector<Error> errors;
  Lexer lexer("*s32", "file/directory", "TypePointerParse", errors);
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
  ASSERT_EQ(value_node->child_type->type_info->name, "s32");
  ASSERT_EQ(value_node->child_type->type_info->bit_size, 32);
  ASSERT_EQ(value_node->child_type->type_info->llvm_type, nullptr);
  ASSERT_TRUE(value_node->child_type->type_info->is_signed);
}

//==================================================================================
//          PARSE VARIABLE DEFINITION STATEMENT
//==================================================================================

TEST(ParserHappyVarDefStmntTests, SimpleTypeParse) {
  std::vector<Error> errors;
  Lexer lexer("myVar s32", "file/directory", "VarDefSimpleTypeParse", errors);
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
  ASSERT_EQ(type_node->type_info->name, "s32");
  ASSERT_EQ(type_node->type_info->bit_size, 32);
  ASSERT_EQ(type_node->type_info->llvm_type, nullptr);
  ASSERT_TRUE(type_node->type_info->is_signed);

  delete value_node;
}

TEST(ParserHappyVarDefStmntTests, SimpleTypeInitializerParse) {
  std::vector<Error> errors;
  Lexer lexer("myVar s32 = -65", "file/directory", "VarDefSimpleTypeInitializerParse", errors);
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
  ASSERT_EQ(type_node->type_info->name, "s32");
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

TEST(ParserHappyVarDefStmntTests, SimpleTypeInitializerAddParse) {
  std::vector<Error> errors;
  Lexer lexer("myVar s32 = 65 + PI", "file/directory", "VarDefSimpleTypeInitializerAddParse", errors);
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
  ASSERT_EQ(type_node->type_info->name, "s32");
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

TEST(ParserHappyVarDefStmntTests, ArrayTypeParse) {
  std::vector<Error> errors;
  Lexer lexer("myVar []s32", "file/directory", "VarDefArrayTypeParse", errors);
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
  ASSERT_EQ(data_type_node->type_info->name, "s32");
  ASSERT_EQ(data_type_node->type_info->bit_size, 32);
  ASSERT_EQ(data_type_node->type_info->llvm_type, nullptr);
  ASSERT_EQ(data_type_node->type_info->is_signed, true);

  delete value_node;
}

TEST(ParserHappyVarDefStmntTests, PointerTypeParse) {
  std::vector<Error> errors;
  Lexer lexer("myVar *s32", "file/directory", "VarDefPointerTypeParse", errors);
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
  ASSERT_EQ(data_type_node->type_info->name, "s32");
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
  Lexer lexer("myVar s32", "file/directory", "VarDefSimpleTypeParse", errors);
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
  ASSERT_EQ(type_node->type_info->name, "s32");
  ASSERT_EQ(type_node->type_info->bit_size, 32);
  ASSERT_EQ(type_node->type_info->llvm_type, nullptr);
  ASSERT_EQ(type_node->type_info->is_signed, true);

  delete value_node;
}

TEST(ParserHappyStmntTests, StatementVarDefArrayTypeParse) {
  std::vector<Error> errors;
  Lexer lexer("myVar []s32", "file/directory", "VarDefArrayTypeParse", errors);
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
  ASSERT_EQ(data_type_node->type_info->name, "s32");
  ASSERT_EQ(data_type_node->type_info->bit_size, 32);
  ASSERT_EQ(data_type_node->type_info->llvm_type, nullptr);
  ASSERT_EQ(data_type_node->type_info->is_signed, true);

  delete value_node;
}

TEST(ParserHappyStmntTests, StatementVarDefPointerTypeParse) {
  std::vector<Error> errors;
  Lexer lexer("myVar *s32", "file/directory", "VarDefPointerTypeParse", errors);
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
  ASSERT_EQ(data_type_node->type_info->name, "s32");
  ASSERT_EQ(data_type_node->type_info->bit_size, 32);
  ASSERT_EQ(data_type_node->type_info->llvm_type, nullptr);
  ASSERT_EQ(data_type_node->type_info->is_signed, true);

  delete value_node;
}

TEST(ParserHappyStmntTests, StatementAssignStmntTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar = 5 * (6 + 9)", "file/directory", "AssignStmntTest", errors);
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
  Lexer lexer("ret myVar * (5 + 8)", "file/directory", "Ret stmnt", errors);
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
  Lexer lexer("ret ", "file/directory", "RetEmptyStmnt", errors);
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

TEST(ParserHappyBlockStmntTests, EmptyTest) {
  std::vector<Error> errors;
  Lexer lexer("{}", "file/directory", "BlockEmptyTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstBlock *value_node = parser.parse_block(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BLOCK);
  ASSERT_EQ(value_node->statements.size(), 0L);

  delete value_node;
}

TEST(ParserHappyBlockStmntTests, NoSpaceNearCurliesTest) {
  std::vector<Error> errors;
  Lexer lexer("{myVar s32\nret myvar}", "file/directory", "BlockNoSpaceNearCurliesTest", errors);
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
  ASSERT_EQ(type_node->type_info->name, "s32");
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

TEST(ParserHappyBlockStmntTests, SpaceNearCurliesTest) {
  std::vector<Error> errors;
  Lexer lexer("{ myVar s32\nret myvar }", "file/directory", "BlockSpaceNearCurliesTest", errors);
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
  ASSERT_EQ(type_node->type_info->name, "s32");
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

TEST(ParserHappyBlockStmntTests, NewlinesNearCurliesTest) {
  std::vector<Error> errors;
  Lexer lexer("{\n\tmyVar s32\n\tret myvar\n}", "file/directory", "BlockNewlinesNearCurliesTest", errors);
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
  ASSERT_EQ(type_node->type_info->name, "s32");
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

TEST(ParserHappyBlockStmntTests, SpacesBetweenNewlinesTest) {
  std::vector<Error> errors;
  Lexer lexer("{ \n  \t myVar s32   \n  \t  ret myvar  \n   }", "file/directory", "BlockSpacesBetweenNewlinesTest",
              errors);
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
  ASSERT_EQ(type_node->type_info->name, "s32");
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

TEST(ParserHappyFnDeclStmntTests, EmptyParamTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc() void", "file/directory", "FuncProtoEmptyParamTest", errors);
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

TEST(ParserHappyFnDeclStmntTests, SingleParamTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc(param1 s32) void", "file/directory", "FuncProtoSingleParamTest", errors);
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
  ASSERT_EQ(param_node->type->type_info->name, "s32");
  ASSERT_EQ(param_node->type->type_info->type_id, AstTypeId::INTEGER);

  AstType *ret_type_node = value_node->return_type;
  ASSERT_EQ(ret_type_node->parent, value_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(ret_type_node->type_info->name, "void");
  ASSERT_EQ(ret_type_node->type_info->type_id, AstTypeId::VOID);

  delete value_node;
}

TEST(ParserHappyFnDeclStmntTests, MultiParamTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc(param1 s32, param1 s32, param1 s32) s32", "file/directory", "FuncProtoMultiParamTest", errors);
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
    ASSERT_EQ(param_node->type->type_info->name, "s32");
    ASSERT_EQ(param_node->type->type_info->type_id, AstTypeId::INTEGER);
  }

  AstType *ret_type_node = value_node->return_type;
  ASSERT_EQ(ret_type_node->parent, value_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(ret_type_node->type_info->name, "s32");
  ASSERT_EQ(ret_type_node->type_info->type_id, AstTypeId::INTEGER);

  delete value_node;
}

TEST(ParserHappyFnDeclStmntTests, MultiLineTest) {
  std::vector<Error> errors;
  Lexer lexer("fn\nmyFunc\n(param1 s32\n, param1 s32,\n param1\ns32)\ns32", " file / directory ",
              "FuncProtoMultiLineTest", errors);
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
    ASSERT_EQ(param_node->type->type_info->name, "s32");
    ASSERT_EQ(param_node->type->type_info->type_id, AstTypeId::INTEGER);
  }

  AstType *ret_type_node = value_node->return_type;
  ASSERT_EQ(ret_type_node->parent, value_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(ret_type_node->type_info->name, "s32");
  ASSERT_EQ(ret_type_node->type_info->type_id, AstTypeId::INTEGER);

  delete value_node;
}

//==================================================================================
//          PARSE FUNCTION DEFINITION STATEMENT
//==================================================================================

TEST(ParserHappyFnDefStmntTests, NoParamsVoidBlockTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc() void {}", "file/directory", "FuncDefNoParamsVoidBlockTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *node = parser.parse_function_def(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(node, nullptr);
  ASSERT_EQ(node->node_type, AstNodeType::AST_FN_DEF);

  AstFnDef *value_node = node->fn_def();
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

TEST(ParserHappyFnDefStmntTests, OneParamVoidBlockTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc(param1 s32) s32 {}", "file/directory", "FuncDefSingleParamsVoidBlockTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *node = parser.parse_function_def(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(node, nullptr);
  ASSERT_EQ(node->node_type, AstNodeType::AST_FN_DEF);

  AstFnDef *value_node = node->fn_def();
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
  ASSERT_EQ(param_node->type->type_info->name, "s32");
  ASSERT_EQ(param_node->type->type_info->type_id, AstTypeId::INTEGER);

  AstType *ret_type_node = proto_node->return_type;
  ASSERT_NE(ret_type_node, nullptr);
  ASSERT_EQ(ret_type_node->parent, proto_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(ret_type_node->type_info->name, "s32");
  ASSERT_EQ(ret_type_node->type_info->type_id, AstTypeId::INTEGER);

  delete value_node;
}

TEST(ParserHappyFnDefStmntTests, MultiParamsVoidBlockTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc(param1 s32, param1 s32, param1 s32) s32 {}", "file/directory",
              "FuncDefMultiParamsVoidBlockTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *node = parser.parse_function_def(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(node, nullptr);
  ASSERT_EQ(node->node_type, AstNodeType::AST_FN_DEF);

  AstFnDef *value_node = node->fn_def();
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
    ASSERT_EQ(param_node->type->type_info->name, "s32");
    ASSERT_EQ(param_node->type->type_info->type_id, AstTypeId::INTEGER);
  }

  AstType *ret_type_node = proto_node->return_type;
  ASSERT_NE(ret_type_node, nullptr);
  ASSERT_EQ(ret_type_node->parent, proto_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(ret_type_node->type_info->name, "s32");
  ASSERT_EQ(ret_type_node->type_info->type_id, AstTypeId::INTEGER);

  delete value_node;
}

TEST(ParserHappyFnDefStmntTests, NoParamsTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc() void {\nmyVar s32\n}", "file/directory", "FuncDefNoParamsTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *node = parser.parse_function_def(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(node, nullptr);
  ASSERT_EQ(node->node_type, AstNodeType::AST_FN_DEF);

  AstFnDef *value_node = node->fn_def();
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
  ASSERT_EQ(type_node->type_info->name, "s32");

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

TEST(ParserHappyFnDefStmntTests, OneParamTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc(param1 s32) s32 {\nret param1 + 5\n}", "file/directory", "FuncDefSingleParamsTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *node = parser.parse_function_def(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(node, nullptr);
  ASSERT_EQ(node->node_type, AstNodeType::AST_FN_DEF);

  AstFnDef *value_node = node->fn_def();
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
  ASSERT_EQ(param_node->type->type_info->name, "s32");
  ASSERT_EQ(param_node->type->type_info->type_id, AstTypeId::INTEGER);

  AstType *ret_type_node = proto_node->return_type;
  ASSERT_NE(ret_type_node, nullptr);
  ASSERT_EQ(ret_type_node->parent, proto_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AST_TYPE);
  ASSERT_EQ(ret_type_node->type_info->name, "s32");
  ASSERT_EQ(ret_type_node->type_info->type_id, AstTypeId::INTEGER);

  delete value_node;
}

//==================================================================================
//          PARSE FUNCTION CALL STATEMENT
//==================================================================================

TEST(ParserHappyCallStmntTests, NoParamsTest) {
  std::vector<Error> errors;
  Lexer lexer("myFunc()", "file/directory", "FuncCallNoParamsTest", errors);
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

TEST(ParserHappyCallStmntTests, OneParamTest) {
  std::vector<Error> errors;
  Lexer lexer("myFunc(5)", "file/directory", "FuncCall1ParamTest", errors);
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

TEST(ParserHappyCallStmntTests, OneParamAddOpTest) {
  std::vector<Error> errors;
  Lexer lexer("85 + myFunc(5)", "file/directory", "FuncCall1ParamTest", errors);
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

  const AstFnCallExpr *fn_call_node = bin_expr->right_expr->fn_call();
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

TEST(ParserHappyCallStmntTests, NestedCallsTest) {
  std::vector<Error> errors;
  Lexer lexer("myFunc(myFunc2(5))", "file/directory", "FuncCallNestedTest", errors);
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

  const AstFnCallExpr *param_node_func = param_node_0->fn_call();
  ASSERT_EQ(param_node_func->fn_name, "myFunc2");
  ASSERT_EQ(param_node_func->args.size(), 1L);

  const AstNode *param_node_1 = param_node_func->args.at(0);
  ASSERT_NE(param_node_1, nullptr);
  ASSERT_EQ(param_node_1->parent, param_node_func);
  ASSERT_EQ(param_node_1->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(param_node_1->const_value()->type, ConstValueType::INT);

  delete value_node;
}

TEST(ParserHappyCallStmntTests, MultiParamsTest) {
  std::vector<Error> errors;
  Lexer lexer("myFunc(myVar, myFunc2(5))", "file/directory", "FuncCallMultiParamsTest", errors);
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

  const AstFnCallExpr *param_node_func = param_node_1->fn_call();
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
//          PARSE BRANCHES STATEMENT
//==================================================================================

TEST(ParserHappyBranchStmntTests, IfConstantExprTest) {
  std::vector<Error> errors;
  const char *source_code = "if true {}\n";

  Lexer lexer(source_code, "file/directory", "BranchIfRetTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstIfStmnt *if_stmnt = parser.parse_if_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(if_stmnt, nullptr);
  ASSERT_EQ(if_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_TRUE(if_stmnt->is_condition_checked);

  const AstConstValue *cond_expr = if_stmnt->condition_expr->const_value();
  ASSERT_NE(cond_expr, nullptr);
  ASSERT_EQ(cond_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_TRUE(cond_expr->boolean);

  delete if_stmnt;
}

TEST(ParserHappyBranchStmntTests, IfBinaryExprTest) {
  std::vector<Error> errors;
  const char *source_code = "if false == true {}\n";

  Lexer lexer(source_code, "file/directory", "BranchIfRetTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstIfStmnt *if_stmnt = parser.parse_if_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(if_stmnt, nullptr);
  ASSERT_EQ(if_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_TRUE(if_stmnt->is_condition_checked);

  const AstBinaryExpr *cond_expr = if_stmnt->condition_expr->binary_expr();
  ASSERT_NE(cond_expr, nullptr);
  ASSERT_EQ(cond_expr->node_type, AstNodeType::AST_BINARY_EXPR);
  ASSERT_EQ(cond_expr->bin_op, BinaryExprType::EQUALS);

  delete if_stmnt;
}

TEST(ParserHappyBranchStmntTests, IfUnaryExprTest) {
  std::vector<Error> errors;
  const char *source_code = "if !false {}\n";

  Lexer lexer(source_code, "file/directory", "BranchIfRetTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstIfStmnt *if_stmnt = parser.parse_if_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(if_stmnt, nullptr);
  ASSERT_EQ(if_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_TRUE(if_stmnt->is_condition_checked);

  const AstUnaryExpr *cond_expr = if_stmnt->condition_expr->unary_expr();
  ASSERT_NE(cond_expr, nullptr);
  ASSERT_EQ(cond_expr->node_type, AstNodeType::AST_UNARY_EXPR);
  ASSERT_EQ(cond_expr->op, UnaryExprType::NOT);

  delete if_stmnt;
}

TEST(ParserHappyBranchStmntTests, IfComplexExprTest) {
  std::vector<Error> errors;
  const char *source_code = "if true == false < 1 + 2 {}\n";

  Lexer lexer(source_code, "file/directory", "BranchIfRetTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstIfStmnt *if_stmnt = parser.parse_if_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(if_stmnt, nullptr);
  ASSERT_EQ(if_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_TRUE(if_stmnt->is_condition_checked);

  const AstBinaryExpr *cond_expr = if_stmnt->condition_expr->binary_expr();
  ASSERT_NE(cond_expr, nullptr);
  ASSERT_EQ(cond_expr->node_type, AstNodeType::AST_BINARY_EXPR);
  ASSERT_EQ(cond_expr->bin_op, BinaryExprType::LESS);

  delete if_stmnt;
}

TEST(ParserHappyBranchStmntTests, IfArithmeticEqualityExprTest) {
  std::vector<Error> errors;
  const char *source_code = "if 5 * 9 == 40 + 5 {}\n";

  Lexer lexer(source_code, "file/directory", "BranchIfRetTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstIfStmnt *if_stmnt = parser.parse_if_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(if_stmnt, nullptr);
  ASSERT_EQ(if_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_TRUE(if_stmnt->is_condition_checked);

  const AstBinaryExpr *cond_expr = if_stmnt->condition_expr->binary_expr();
  ASSERT_NE(cond_expr, nullptr);
  ASSERT_EQ(cond_expr->node_type, AstNodeType::AST_BINARY_EXPR);
  ASSERT_EQ(cond_expr->bin_op, BinaryExprType::EQUALS);

  delete if_stmnt;
}

TEST(ParserHappyBranchStmntTests, IfSymbolExprTest) {
  std::vector<Error> errors;
  const char *source_code = "if my_var {}\n";

  Lexer lexer(source_code, "file/directory", "BranchIfRetTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstIfStmnt *if_stmnt = parser.parse_if_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(if_stmnt, nullptr);
  ASSERT_EQ(if_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_FALSE(if_stmnt->is_condition_checked);

  const AstSymbol *cond_expr = if_stmnt->condition_expr->symbol();
  ASSERT_NE(cond_expr, nullptr);
  ASSERT_EQ(cond_expr->node_type, AstNodeType::AST_SYMBOL);

  delete if_stmnt;
}

TEST(ParserHappyBranchStmntTests, IfConstantWithParentsExprTest) {
  std::vector<Error> errors;
  const char *source_code = "if (true) {}\n";

  Lexer lexer(source_code, "file/directory", "BranchIfRetTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstIfStmnt *if_stmnt = parser.parse_if_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(if_stmnt, nullptr);
  ASSERT_EQ(if_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_TRUE(if_stmnt->is_condition_checked);

  const AstConstValue *cond_expr = if_stmnt->condition_expr->const_value();
  ASSERT_NE(cond_expr, nullptr);
  ASSERT_EQ(cond_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_TRUE(cond_expr->boolean);

  delete if_stmnt;
}

TEST(ParserHappyBranchStmntTests, IfBinaryWithParentsExprTest) {
  std::vector<Error> errors;
  const char *source_code = "if (false == true) {}\n";

  Lexer lexer(source_code, "file/directory", "BranchIfRetTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstIfStmnt *if_stmnt = parser.parse_if_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(if_stmnt, nullptr);
  ASSERT_EQ(if_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_TRUE(if_stmnt->is_condition_checked);

  const AstBinaryExpr *cond_expr = if_stmnt->condition_expr->binary_expr();
  ASSERT_NE(cond_expr, nullptr);
  ASSERT_EQ(cond_expr->node_type, AstNodeType::AST_BINARY_EXPR);
  ASSERT_EQ(cond_expr->bin_op, BinaryExprType::EQUALS);

  delete if_stmnt;
}

TEST(ParserHappyBranchStmntTests, IfUnaryWithParentsExprTest) {
  std::vector<Error> errors;
  const char *source_code = "if (!false) {}\n";

  Lexer lexer(source_code, "file/directory", "BranchIfRetTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstIfStmnt *if_stmnt = parser.parse_if_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(if_stmnt, nullptr);
  ASSERT_EQ(if_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_TRUE(if_stmnt->is_condition_checked);

  const AstUnaryExpr *cond_expr = if_stmnt->condition_expr->unary_expr();
  ASSERT_NE(cond_expr, nullptr);
  ASSERT_EQ(cond_expr->node_type, AstNodeType::AST_UNARY_EXPR);
  ASSERT_EQ(cond_expr->op, UnaryExprType::NOT);

  delete if_stmnt;
}

TEST(ParserHappyBranchStmntTests, IfComplexWithParentsExprTest) {
  std::vector<Error> errors;
  const char *source_code = "if (true == false < 1 + 2) {}\n";

  Lexer lexer(source_code, "file/directory", "BranchIfRetTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstIfStmnt *if_stmnt = parser.parse_if_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(if_stmnt, nullptr);
  ASSERT_EQ(if_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_TRUE(if_stmnt->is_condition_checked);

  const AstBinaryExpr *cond_expr = if_stmnt->condition_expr->binary_expr();
  ASSERT_NE(cond_expr, nullptr);
  ASSERT_EQ(cond_expr->node_type, AstNodeType::AST_BINARY_EXPR);
  ASSERT_EQ(cond_expr->bin_op, BinaryExprType::LESS);

  delete if_stmnt;
}

TEST(ParserHappyBranchStmntTests, IfArithmeticEqualityWithParentsExprTest) {
  std::vector<Error> errors;
  const char *source_code = "if (5 * 9 == 40 + 5) {}\n";

  Lexer lexer(source_code, "file/directory", "BranchIfRetTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstIfStmnt *if_stmnt = parser.parse_if_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(if_stmnt, nullptr);
  ASSERT_EQ(if_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_TRUE(if_stmnt->is_condition_checked);

  const AstBinaryExpr *cond_expr = if_stmnt->condition_expr->binary_expr();
  ASSERT_NE(cond_expr, nullptr);
  ASSERT_EQ(cond_expr->node_type, AstNodeType::AST_BINARY_EXPR);
  ASSERT_EQ(cond_expr->bin_op, BinaryExprType::EQUALS);

  delete if_stmnt;
}

TEST(ParserHappyBranchStmntTests, IfSymbolWithParentsExprTest) {
  std::vector<Error> errors;
  const char *source_code = "if ( my_var ) {}\n";

  Lexer lexer(source_code, "file/directory", "BranchIfRetTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstIfStmnt *if_stmnt = parser.parse_if_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(if_stmnt, nullptr);
  ASSERT_EQ(if_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_FALSE(if_stmnt->is_condition_checked);

  const AstSymbol *cond_expr = if_stmnt->condition_expr->symbol();
  ASSERT_NE(cond_expr, nullptr);
  ASSERT_EQ(cond_expr->node_type, AstNodeType::AST_SYMBOL);

  delete if_stmnt;
}

TEST(ParserHappyBranchStmntTests, IfStmntFilledBlock) {
  std::vector<Error> errors;

  // given: source_file
  const char *source_file = "if my_condition {\n"
                            "\tmy_var s32\n"
                            "\tmy_var = 34\n"
                            "}";

  // given: tokens
  Lexer lexer = Lexer(source_file, "file/directory", "IfStmntFilledBlock", errors);
  lexer.tokenize();

  // given: parsed source node
  Parser parser = Parser(errors);
  AstIfStmnt *if_stmnt = parser.parse_if_stmnt(lexer);

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(if_stmnt, nullptr);
  ASSERT_EQ(if_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_NE(if_stmnt->true_block, nullptr);
  ASSERT_EQ(if_stmnt->true_block->statements.size(), 2);
  ASSERT_EQ(if_stmnt->false_block, nullptr);

  delete if_stmnt;
}

TEST(ParserHappyBranchStmntTests, IfElseStmntFilledBlock) {
  std::vector<Error> errors;

  // given: source_file
  const char *source_file = "if my_condition {\n"
                            "\tmy_var s32\n"
                            "\tmy_var = 34\n"
                            "} else {\n"
                            "\tmy_var s32\n"
                            "\tmy_var = 43\n"
                            "}\n";

  // given: tokens
  Lexer lexer = Lexer(source_file, "file/directory", "IfElseStmntFilledBlock", errors);
  lexer.tokenize();

  // given: parsed source node
  Parser parser = Parser(errors);
  AstIfStmnt *if_stmnt = parser.parse_if_stmnt(lexer);

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(if_stmnt, nullptr);
  ASSERT_EQ(if_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_NE(if_stmnt->true_block, nullptr);
  ASSERT_EQ(if_stmnt->true_block->statements.size(), 2);
  ASSERT_NE(if_stmnt->false_block, nullptr);
  ASSERT_EQ(if_stmnt->false_block->statements.size(), 2);

  delete if_stmnt;
}

TEST(ParserHappyBranchStmntTests, IfElifStmntFilledBlock) {
  std::vector<Error> errors;

  // given: source_file
  const char *source_file = "if my_condition {\n"
                            "\tmy_var s32\n"
                            "\tmy_var = 34\n"
                            "} elif !my_condition {\n"
                            "\tmy_var s32\n"
                            "\tmy_var = 43\n"
                            "}\n";

  // given: tokens
  Lexer lexer = Lexer(source_file, "file/directory", "IfElifStmntFilledBlock", errors);
  lexer.tokenize();

  // given: parsed source node
  Parser parser = Parser(errors);
  AstIfStmnt *if_stmnt = parser.parse_if_stmnt(lexer);

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(if_stmnt, nullptr);
  ASSERT_EQ(if_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_NE(if_stmnt->true_block, nullptr);
  ASSERT_EQ(if_stmnt->true_block->statements.size(), 2);

  ASSERT_NE(if_stmnt->false_block, nullptr);
  ASSERT_EQ(if_stmnt->false_block->statements.size(), 1);

  const AstIfStmnt *elif_stmnt = if_stmnt->false_block->statements.at(0)->if_stmnt();
  ASSERT_NE(elif_stmnt, nullptr);
  ASSERT_EQ(elif_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_NE(elif_stmnt->true_block, nullptr);
  ASSERT_EQ(elif_stmnt->true_block->statements.size(), 2);
  ASSERT_EQ(elif_stmnt->false_block, nullptr);

  delete if_stmnt;
}

TEST(ParserHappyBranchStmntTests, IfElifStmntWithParenFilledBlock) {
  std::vector<Error> errors;

  // given: source_file
  const char *source_file = "if my_condition {\n"
                            "\tmy_var s32\n"
                            "\tmy_var = 34\n"
                            "} elif (!my_condition) {\n"
                            "\tmy_var s32\n"
                            "\tmy_var = 43\n"
                            "}\n";

  // given: tokens
  Lexer lexer = Lexer(source_file, "file/directory", "IfElifStmntFilledBlock", errors);
  lexer.tokenize();

  // given: parsed source node
  Parser parser = Parser(errors);
  AstIfStmnt *if_stmnt = parser.parse_if_stmnt(lexer);

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(if_stmnt, nullptr);
  ASSERT_EQ(if_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_NE(if_stmnt->true_block, nullptr);
  ASSERT_EQ(if_stmnt->true_block->statements.size(), 2);

  ASSERT_NE(if_stmnt->false_block, nullptr);
  ASSERT_EQ(if_stmnt->false_block->statements.size(), 1);

  const AstIfStmnt *elif_stmnt = if_stmnt->false_block->statements.at(0)->if_stmnt();
  ASSERT_NE(elif_stmnt, nullptr);
  ASSERT_EQ(elif_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_NE(elif_stmnt->true_block, nullptr);
  ASSERT_EQ(elif_stmnt->true_block->statements.size(), 2);
  ASSERT_EQ(elif_stmnt->false_block, nullptr);

  delete if_stmnt;
}

TEST(ParserHappyBranchStmntTests, IfElifElseStmntFilledBlock) {
  std::vector<Error> errors;

  // given: source_file
  const char *source_file = "if my_condition {\n"
                            "\tmy_var s32\n"
                            "\tmy_var = 34\n"
                            "} elif !my_condition {\n"
                            "\tmy_var s32\n"
                            "\tmy_var = 43\n"
                            "} else {\n"
                            "\tmy_var s32\n"
                            "\tmy_var = 43\n"
                            "}\n";

  // given: tokens
  Lexer lexer = Lexer(source_file, "file/directory", "IfElifElseStmntFilledBlock", errors);
  lexer.tokenize();

  // given: parsed source node
  Parser parser = Parser(errors);
  AstIfStmnt *if_stmnt = parser.parse_if_stmnt(lexer);

  // then:
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(if_stmnt, nullptr);
  ASSERT_EQ(if_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_NE(if_stmnt->true_block, nullptr);
  ASSERT_EQ(if_stmnt->true_block->statements.size(), 2);

  ASSERT_NE(if_stmnt->false_block, nullptr);
  ASSERT_EQ(if_stmnt->false_block->statements.size(), 1);

  const AstIfStmnt *elif_stmnt = if_stmnt->false_block->statements.at(0)->if_stmnt();
  ASSERT_NE(elif_stmnt, nullptr);
  ASSERT_EQ(elif_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_NE(elif_stmnt->true_block, nullptr);
  ASSERT_EQ(elif_stmnt->true_block->statements.size(), 2);
  ASSERT_NE(elif_stmnt->false_block, nullptr);
  ASSERT_EQ(elif_stmnt->false_block->statements.size(), 2);

  delete if_stmnt;
}

//==================================================================================
//          PARSE LOOP STATEMENT
//==================================================================================

TEST(ParserHappyLoopStmntTests, ConstantExprTest) {
  std::vector<Error> errors;
  const char *source_code = "loop true {}\n";

  Lexer lexer(source_code, "file/directory", "BranchIfRetTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstLoopStmnt *loop_stmnt = parser.parse_loop_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(loop_stmnt, nullptr);
  ASSERT_EQ(loop_stmnt->node_type, AstNodeType::AST_LOOP_STMNT);
  ASSERT_TRUE(loop_stmnt->is_condition_checked);

  const AstConstValue *cond_expr = loop_stmnt->condition_expr->const_value();
  ASSERT_NE(cond_expr, nullptr);
  ASSERT_EQ(cond_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_TRUE(cond_expr->boolean);

  delete loop_stmnt;
}

TEST(ParserHappyLoopStmntTests, ConstantExprBreakEmptyTest) {
  std::vector<Error> errors;
  const char *source_code = "loop true {\n"
                            "\tif true {\n"
                            "\t\tbreak\n"
                            "\t}\n"
                            "}\n";

  Lexer lexer(source_code, "file/directory", "BranchIfRetTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstLoopStmnt *loop_stmnt = parser.parse_loop_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(loop_stmnt, nullptr);
  ASSERT_EQ(loop_stmnt->node_type, AstNodeType::AST_LOOP_STMNT);
  ASSERT_TRUE(loop_stmnt->is_condition_checked);
  ASSERT_EQ(loop_stmnt->content_block->statements.size(), 1L);

  const AstConstValue *cond_expr = loop_stmnt->condition_expr->const_value();
  ASSERT_NE(cond_expr, nullptr);
  ASSERT_EQ(cond_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_TRUE(cond_expr->boolean);

  const AstIfStmnt *if_stmnt = loop_stmnt->content_block->statements.at(0)->if_stmnt();
  ASSERT_NE(if_stmnt, nullptr);
  ASSERT_EQ(if_stmnt->node_type, AstNodeType::AST_IF_STMNT);
  ASSERT_EQ(if_stmnt->true_block->statements.size(), 1L);

  const AstCtrlStmnt *ctrl_stmnt = if_stmnt->true_block->statements.at(0)->ctrl_stmnt();
  ASSERT_NE(ctrl_stmnt, nullptr);
  ASSERT_EQ(ctrl_stmnt->node_type, AstNodeType::AST_CTRL_STMNT);
  ASSERT_EQ(ctrl_stmnt->ctrl_type, CtrlStmntType::BREAK);
  ASSERT_EQ(ctrl_stmnt->index, 0L);
  ASSERT_EQ(ctrl_stmnt->label, nullptr);

  delete loop_stmnt;
}

//==================================================================================
//          PARSE FULL PROGRAM
//==================================================================================

TEST(ParserHappyFullProgramStmntTests, NoNewLineEnd) {
  const char *source_code = "/* Test program */\n" // multiline comment
                            "\n"
                            ";\n"         // empty statement : discarded
                            ";"           // empty statement : discarded
                            "myVar s32\n" // variable definition
                            "myVar\n"     // unused variable reference : discarded
                            "myVar;\n"    // unused variable reference : discarded
                            "\n"
                            "fn myFunc() void {\n"
                            "\tret\n"
                            "}";

  std::vector<Error> errors;
  Lexer lexer(source_code, "file/directory", "FullProgramNoNewLineEnd", errors);
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
  ASSERT_EQ(var_type->type_info->name, "s32");
  ASSERT_EQ(var_type->type_info->bit_size, 32);
  ASSERT_EQ(var_type->type_info->is_signed, true);

  const AstNode *func_def_node = main_stmnts.at(1);
  ASSERT_NE(func_def_node, nullptr);
  ASSERT_EQ(func_def_node->node_type, AstNodeType::AST_FN_DEF);

  const AstFnDef *fn_def = func_def_node->fn_def();
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

  const AstUnaryExpr *ret_expr = block_node->statements.at(0)->unary_expr();
  ASSERT_NE(ret_expr, nullptr);
  ASSERT_EQ(ret_expr->parent, block_node);
  ASSERT_EQ(ret_expr->node_type, AstNodeType::AST_UNARY_EXPR);
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
