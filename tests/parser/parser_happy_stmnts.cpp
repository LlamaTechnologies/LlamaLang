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
  ASSERT_EQ(value_node->node_type, AstNodeType::AstUnaryExpr);
  ASSERT_EQ(value_node->unary_expr.op, UnaryExprType::RET);
  ASSERT_NE(value_node->unary_expr.expr, nullptr);
  ASSERT_EQ(value_node->unary_expr.expr->parent, value_node);

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
  ASSERT_EQ(value_node->node_type, AstNodeType::AstUnaryExpr);
  ASSERT_EQ(value_node->unary_expr.op, UnaryExprType::RET);
  ASSERT_EQ(value_node->unary_expr.expr, nullptr);

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
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ASSIGN);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);

  delete value_node;
}

//==================================================================================
//          PARSE TYPES
//==================================================================================

TEST(ParserHappyStmntTests, TypeNameIsPrimitiveParse) {
  std::vector<Error> errors;
  Lexer lexer("i32", "TypeNameIsPrimitiveParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_type(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(value_node->ast_type.type_id, AstTypeId::Integer);
  ASSERT_EQ(value_node->ast_type.type_info->name, "i32");
  ASSERT_EQ(value_node->ast_type.type_info->bit_size, 32);
  ASSERT_EQ(value_node->ast_type.type_info->llvm_type, nullptr);
  ASSERT_EQ(value_node->ast_type.type_info->is_signed, true);

  delete value_node;
}

TEST(ParserHappyStmntTests, TypeNameParse) {
  std::vector<Error> errors;
  Lexer lexer("MyType", "TypeNameParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_type(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(value_node->ast_type.type_id, AstTypeId::Struct);
  ASSERT_EQ(value_node->ast_type.type_info->name, "MyType");
  ASSERT_EQ(value_node->ast_type.type_info->bit_size, 8);
  ASSERT_EQ(value_node->ast_type.type_info->llvm_type, nullptr);
  ASSERT_EQ(value_node->ast_type.type_info->is_signed, false);

  delete value_node;
}

TEST(ParserHappyStmntTests, TypeArrayParse) {
  std::vector<Error> errors;
  Lexer lexer("[]MyType", "TypeArrayParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_type(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(value_node->ast_type.type_id, AstTypeId::Array);
  ASSERT_NE(value_node->ast_type.child_type, nullptr);
  ASSERT_EQ(value_node->ast_type.child_type->parent, value_node);
  ASSERT_EQ(value_node->ast_type.child_type->node_type, AstNodeType::AstType);
  ASSERT_EQ(value_node->ast_type.child_type->ast_type.type_id, AstTypeId::Struct);
  ASSERT_EQ(value_node->ast_type.child_type->ast_type.type_info->name, "MyType");
  ASSERT_EQ(value_node->ast_type.child_type->ast_type.type_info->bit_size, 8);
  ASSERT_EQ(value_node->ast_type.child_type->ast_type.type_info->llvm_type, nullptr);
  ASSERT_EQ(value_node->ast_type.child_type->ast_type.type_info->is_signed, false);

  delete value_node;
}

TEST(ParserHappyStmntTests, TypePointerParse) {
  std::vector<Error> errors;
  Lexer lexer("*MyType", "TypePointerParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_type(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(value_node->ast_type.type_id, AstTypeId::Pointer);
  ASSERT_NE(value_node->ast_type.child_type, nullptr);
  ASSERT_EQ(value_node->ast_type.child_type->parent, value_node);
  ASSERT_EQ(value_node->ast_type.child_type->node_type, AstNodeType::AstType);
  ASSERT_EQ(value_node->ast_type.child_type->ast_type.type_id, AstTypeId::Struct);
  ASSERT_EQ(value_node->ast_type.child_type->ast_type.type_info->name, "MyType");
  ASSERT_EQ(value_node->ast_type.child_type->ast_type.type_info->bit_size, 8);
  ASSERT_EQ(value_node->ast_type.child_type->ast_type.type_info->llvm_type, nullptr);
  ASSERT_EQ(value_node->ast_type.child_type->ast_type.type_info->is_signed, false);

  delete value_node;
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
  ASSERT_EQ(value_node->node_type, AstNodeType::AstVarDef);
  ASSERT_EQ(value_node->var_def.name, "myVar");
  ASSERT_EQ(value_node->var_def.initializer, nullptr);
  ASSERT_NE(value_node->var_def.type, nullptr);
  auto type_node = value_node->var_def.type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(type_node->parent, value_node);
  ASSERT_EQ(type_node->ast_type.type_id, AstTypeId::Integer);
  ASSERT_EQ(type_node->ast_type.type_info->name, "i32");
  ASSERT_EQ(type_node->ast_type.type_info->bit_size, 32);
  ASSERT_EQ(type_node->ast_type.type_info->llvm_type, nullptr);
  ASSERT_EQ(type_node->ast_type.type_info->is_signed, true);

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
  ASSERT_EQ(value_node->node_type, AstNodeType::AstVarDef);
  ASSERT_EQ(value_node->var_def.name, "myVar");

  auto type_node = value_node->var_def.type;
  ASSERT_NE(type_node, nullptr);
  ASSERT_EQ(type_node->parent, value_node);
  ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(type_node->ast_type.type_id, AstTypeId::Integer);
  ASSERT_EQ(type_node->ast_type.type_info->name, "i32");
  ASSERT_EQ(type_node->ast_type.type_info->bit_size, 32);
  ASSERT_EQ(type_node->ast_type.type_info->llvm_type, nullptr);
  ASSERT_EQ(type_node->ast_type.type_info->is_signed, true);

  auto init_node = value_node->var_def.initializer;
  ASSERT_NE(init_node, nullptr);
  ASSERT_EQ(init_node->parent, value_node);
  ASSERT_EQ(init_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(init_node->const_value.type, ConstValueType::INT);
  ASSERT_EQ(init_node->const_value.is_negative, true);

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
  ASSERT_EQ(value_node->node_type, AstNodeType::AstVarDef);
  ASSERT_EQ(value_node->var_def.name, "myVar");

  auto type_node = value_node->var_def.type;
  ASSERT_NE(type_node, nullptr);
  ASSERT_EQ(type_node->parent, value_node);
  ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(type_node->ast_type.type_id, AstTypeId::Integer);
  ASSERT_EQ(type_node->ast_type.type_info->name, "i32");
  ASSERT_EQ(type_node->ast_type.type_info->bit_size, 32);
  ASSERT_EQ(type_node->ast_type.type_info->llvm_type, nullptr);
  ASSERT_EQ(type_node->ast_type.type_info->is_signed, true);

  auto init_node = value_node->var_def.initializer;
  ASSERT_NE(init_node, nullptr);
  ASSERT_EQ(init_node->parent, value_node);
  ASSERT_EQ(init_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(init_node->binary_expr.bin_op, BinaryExprType::ADD);
  ASSERT_NE(init_node->binary_expr.left_expr, nullptr);
  ASSERT_NE(init_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(init_node->binary_expr.left_expr->parent, init_node);
  ASSERT_EQ(init_node->binary_expr.right_expr->parent, init_node);
  ASSERT_EQ(init_node->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(init_node->binary_expr.right_expr->node_type, AstNodeType::AstSymbol);

  delete value_node;
}

TEST(ParserHappyStmntTests, VarDefArrayTypeParse) {
  std::vector<Error> errors;
  Lexer lexer("myVar []i32", "VarDefArrayTypeParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_vardef_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstVarDef);
  ASSERT_EQ(value_node->var_def.name, "myVar");
  ASSERT_EQ(value_node->var_def.initializer, nullptr);
  ASSERT_NE(value_node->var_def.type, nullptr);
  auto type_node = value_node->var_def.type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(type_node->parent, value_node);
  ASSERT_EQ(type_node->ast_type.type_id, AstTypeId::Array);
  ASSERT_NE(type_node->ast_type.child_type, nullptr);
  auto data_type_node = type_node->ast_type.child_type;
  ASSERT_EQ(data_type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(data_type_node->parent, type_node);
  ASSERT_EQ(data_type_node->ast_type.type_id, AstTypeId::Integer);
  ASSERT_EQ(data_type_node->ast_type.type_info->name, "i32");
  ASSERT_EQ(data_type_node->ast_type.type_info->bit_size, 32);
  ASSERT_EQ(data_type_node->ast_type.type_info->llvm_type, nullptr);
  ASSERT_EQ(data_type_node->ast_type.type_info->is_signed, true);

  delete value_node;
}

TEST(ParserHappyStmntTests, VarDefPointerTypeParse) {
  std::vector<Error> errors;
  Lexer lexer("myVar *i32", "VarDefPointerTypeParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_vardef_stmnt(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstVarDef);
  ASSERT_EQ(value_node->var_def.name, "myVar");
  ASSERT_EQ(value_node->var_def.initializer, nullptr);
  ASSERT_NE(value_node->var_def.type, nullptr);
  auto type_node = value_node->var_def.type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(type_node->parent, value_node);
  ASSERT_EQ(type_node->ast_type.type_id, AstTypeId::Pointer);
  ASSERT_NE(type_node->ast_type.child_type, nullptr);
  auto data_type_node = type_node->ast_type.child_type;
  ASSERT_EQ(data_type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(data_type_node->parent, type_node);
  ASSERT_EQ(data_type_node->ast_type.type_id, AstTypeId::Integer);
  ASSERT_EQ(data_type_node->ast_type.type_info->name, "i32");
  ASSERT_EQ(data_type_node->ast_type.type_info->bit_size, 32);
  ASSERT_EQ(data_type_node->ast_type.type_info->llvm_type, nullptr);
  ASSERT_EQ(data_type_node->ast_type.type_info->is_signed, true);

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
  auto value_node = parser.parse_statement(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstVarDef);
  ASSERT_EQ(value_node->var_def.name, "myVar");
  ASSERT_EQ(value_node->var_def.initializer, nullptr);
  ASSERT_NE(value_node->var_def.type, nullptr);
  auto type_node = value_node->var_def.type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(type_node->parent, value_node);
  ASSERT_EQ(type_node->ast_type.type_id, AstTypeId::Integer);
  ASSERT_EQ(type_node->ast_type.type_info->name, "i32");
  ASSERT_EQ(type_node->ast_type.type_info->bit_size, 32);
  ASSERT_EQ(type_node->ast_type.type_info->llvm_type, nullptr);
  ASSERT_EQ(type_node->ast_type.type_info->is_signed, true);

  delete value_node;
}

TEST(ParserHappyStmntTests, StatementVarDefArrayTypeParse) {
  std::vector<Error> errors;
  Lexer lexer("myVar []i32", "VarDefArrayTypeParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_statement(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstVarDef);
  ASSERT_EQ(value_node->var_def.name, "myVar");
  ASSERT_EQ(value_node->var_def.initializer, nullptr);
  ASSERT_NE(value_node->var_def.type, nullptr);
  auto type_node = value_node->var_def.type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(type_node->parent, value_node);
  ASSERT_EQ(type_node->ast_type.type_id, AstTypeId::Array);
  ASSERT_NE(type_node->ast_type.child_type, nullptr);
  auto data_type_node = type_node->ast_type.child_type;
  ASSERT_EQ(data_type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(data_type_node->parent, type_node);
  ASSERT_EQ(data_type_node->ast_type.type_id, AstTypeId::Integer);
  ASSERT_EQ(data_type_node->ast_type.type_info->name, "i32");
  ASSERT_EQ(data_type_node->ast_type.type_info->bit_size, 32);
  ASSERT_EQ(data_type_node->ast_type.type_info->llvm_type, nullptr);
  ASSERT_EQ(data_type_node->ast_type.type_info->is_signed, true);

  delete value_node;
}

TEST(ParserHappyStmntTests, StatementVarDefPointerTypeParse) {
  std::vector<Error> errors;
  Lexer lexer("myVar *i32", "VarDefPointerTypeParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_statement(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstVarDef);
  ASSERT_EQ(value_node->var_def.name, "myVar");
  ASSERT_EQ(value_node->var_def.initializer, nullptr);
  ASSERT_NE(value_node->var_def.type, nullptr);
  auto type_node = value_node->var_def.type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(type_node->parent, value_node);
  ASSERT_EQ(type_node->ast_type.type_id, AstTypeId::Pointer);
  ASSERT_NE(type_node->ast_type.child_type, nullptr);
  auto data_type_node = type_node->ast_type.child_type;
  ASSERT_EQ(data_type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(data_type_node->parent, type_node);
  ASSERT_EQ(data_type_node->ast_type.type_id, AstTypeId::Integer);
  ASSERT_EQ(data_type_node->ast_type.type_info->name, "i32");
  ASSERT_EQ(data_type_node->ast_type.type_info->bit_size, 32);
  ASSERT_EQ(data_type_node->ast_type.type_info->llvm_type, nullptr);
  ASSERT_EQ(data_type_node->ast_type.type_info->is_signed, true);

  delete value_node;
}

TEST(ParserHappyStmntTests, StatementAssignStmntTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar = 5 * (6 + 9)", "AssignStmntTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_statement(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ASSIGN);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);

  delete value_node;
}

TEST(ParserHappyStmntTests, StatementRetStmnt) {
  std::vector<Error> errors;
  Lexer lexer("ret myVar * (5 + 8)", "Ret stmnt", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_statement(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstUnaryExpr);
  ASSERT_EQ(value_node->unary_expr.op, UnaryExprType::RET);
  ASSERT_NE(value_node->unary_expr.expr, nullptr);
  ASSERT_EQ(value_node->unary_expr.expr->parent, value_node);

  delete value_node;
}

TEST(ParserHappyStmntTests, StatementRetEmptyStmnt) {
  std::vector<Error> errors;
  Lexer lexer("ret ", "RetEmptyStmnt", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_statement(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstUnaryExpr);
  ASSERT_EQ(value_node->unary_expr.op, UnaryExprType::RET);
  ASSERT_EQ(value_node->unary_expr.expr, nullptr);

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
  AstNode *value_node = parser.parse_block(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBlock);
  ASSERT_EQ(value_node->block.statements.size(), 0L);

  delete value_node;
}

TEST(ParserHappyStmntTests, BlockNoSpaceNearCurliesTest) {
  std::vector<Error> errors;
  Lexer lexer("{myVar i32\nret myvar}", "BlockNoSpaceNearCurliesTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *value_node = parser.parse_block(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBlock);
  ASSERT_EQ(value_node->block.statements.size(), 2);

  ASSERT_EQ(value_node->block.statements.at(0)->node_type, AstNodeType::AstVarDef);
  AstNode *var_def_node = value_node->block.statements.at(0);
  ASSERT_EQ(var_def_node->parent, value_node);
  ASSERT_EQ(var_def_node->var_def.name, "myVar");
  ASSERT_NE(var_def_node->var_def.type, nullptr);
  AstNode *type_node = var_def_node->var_def.type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(type_node->ast_type.type_id, AstTypeId::Integer);
  ASSERT_EQ(type_node->ast_type.type_info->name, "i32");
  ASSERT_EQ(type_node->ast_type.type_info->bit_size, 32);
  ASSERT_EQ(type_node->ast_type.type_info->llvm_type, nullptr);
  ASSERT_EQ(type_node->ast_type.type_info->is_signed, true);

  ASSERT_EQ(value_node->block.statements.at(1)->node_type, AstNodeType::AstUnaryExpr);
  AstNode *ret_node = value_node->block.statements.at(1);
  ASSERT_EQ(ret_node->parent, value_node);
  ASSERT_EQ(ret_node->unary_expr.op, UnaryExprType::RET);
  ASSERT_NE(ret_node->unary_expr.expr, nullptr);

  delete value_node;
}

TEST(ParserHappyStmntTests, BlockSpaceNearCurliesTest) {
  std::vector<Error> errors;
  Lexer lexer("{ myVar i32\nret myvar }", "BlockSpaceNearCurliesTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *value_node = parser.parse_block(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBlock);
  ASSERT_EQ(value_node->block.statements.size(), 2);

  ASSERT_EQ(value_node->block.statements.at(0)->node_type, AstNodeType::AstVarDef);
  AstNode *var_def_node = value_node->block.statements.at(0);
  ASSERT_EQ(var_def_node->parent, value_node);
  ASSERT_EQ(var_def_node->var_def.name, "myVar");
  ASSERT_NE(var_def_node->var_def.type, nullptr);
  AstNode *type_node = var_def_node->var_def.type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(type_node->ast_type.type_id, AstTypeId::Integer);
  ASSERT_EQ(type_node->ast_type.type_info->name, "i32");
  ASSERT_EQ(type_node->ast_type.type_info->bit_size, 32);
  ASSERT_EQ(type_node->ast_type.type_info->llvm_type, nullptr);
  ASSERT_EQ(type_node->ast_type.type_info->is_signed, true);

  ASSERT_EQ(value_node->block.statements.at(1)->node_type, AstNodeType::AstUnaryExpr);
  AstNode *ret_node = value_node->block.statements.at(1);
  ASSERT_EQ(ret_node->parent, value_node);
  ASSERT_EQ(ret_node->unary_expr.op, UnaryExprType::RET);
  ASSERT_NE(ret_node->unary_expr.expr, nullptr);

  delete value_node;
}

TEST(ParserHappyStmntTests, BlockNewlinesNearCurliesTest) {
  std::vector<Error> errors;
  Lexer lexer("{\n\tmyVar i32\n\tret myvar\n}", "BlockNewlinesNearCurliesTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *value_node = parser.parse_block(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBlock);
  ASSERT_EQ(value_node->block.statements.size(), 2);

  ASSERT_EQ(value_node->block.statements.at(0)->node_type, AstNodeType::AstVarDef);
  AstNode *var_def_node = value_node->block.statements.at(0);
  ASSERT_EQ(var_def_node->parent, value_node);
  ASSERT_EQ(var_def_node->var_def.name, "myVar");
  ASSERT_NE(var_def_node->var_def.type, nullptr);
  AstNode *type_node = var_def_node->var_def.type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(type_node->ast_type.type_id, AstTypeId::Integer);
  ASSERT_EQ(type_node->ast_type.type_info->name, "i32");
  ASSERT_EQ(type_node->ast_type.type_info->bit_size, 32);
  ASSERT_EQ(type_node->ast_type.type_info->llvm_type, nullptr);
  ASSERT_EQ(type_node->ast_type.type_info->is_signed, true);

  ASSERT_EQ(value_node->block.statements.at(1)->node_type, AstNodeType::AstUnaryExpr);
  AstNode *ret_node = value_node->block.statements.at(1);
  ASSERT_EQ(ret_node->parent, value_node);
  ASSERT_EQ(ret_node->unary_expr.op, UnaryExprType::RET);
  ASSERT_NE(ret_node->unary_expr.expr, nullptr);

  delete value_node;
}

TEST(ParserHappyStmntTests, BlockSpacesBetweenNewlinesTest) {
  std::vector<Error> errors;
  Lexer lexer("{ \n  \t myVar i32   \n  \t  ret myvar  \n   }", "BlockSpacesBetweenNewlinesTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *value_node = parser.parse_block(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBlock);
  ASSERT_EQ(value_node->block.statements.size(), 2);

  ASSERT_EQ(value_node->block.statements.at(0)->node_type, AstNodeType::AstVarDef);
  AstNode *var_def_node = value_node->block.statements.at(0);
  ASSERT_EQ(var_def_node->parent, value_node);
  ASSERT_EQ(var_def_node->var_def.name, "myVar");
  ASSERT_NE(var_def_node->var_def.type, nullptr);
  AstNode *type_node = var_def_node->var_def.type;
  ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(type_node->ast_type.type_id, AstTypeId::Integer);
  ASSERT_EQ(type_node->ast_type.type_info->name, "i32");
  ASSERT_EQ(type_node->ast_type.type_info->bit_size, 32);
  ASSERT_EQ(type_node->ast_type.type_info->llvm_type, nullptr);
  ASSERT_EQ(type_node->ast_type.type_info->is_signed, true);

  ASSERT_EQ(value_node->block.statements.at(1)->node_type, AstNodeType::AstUnaryExpr);
  AstNode *ret_node = value_node->block.statements.at(1);
  ASSERT_EQ(ret_node->parent, value_node);
  ASSERT_EQ(ret_node->unary_expr.op, UnaryExprType::RET);
  ASSERT_NE(ret_node->unary_expr.expr, nullptr);

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
  AstNode *value_node = parser.parse_function_proto(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncProto);
  ASSERT_EQ(value_node->function_proto.name, "myFunc");
  ASSERT_EQ(value_node->function_proto.params.size(), 0);
  ASSERT_NE(value_node->function_proto.return_type, nullptr);
  auto ret_type_node = value_node->function_proto.return_type;
  ASSERT_EQ(ret_type_node->parent, value_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(ret_type_node->ast_type.type_info->name, "void");
  ASSERT_EQ(ret_type_node->ast_type.type_id, AstTypeId::Void);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncProtoSingleParamTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc(param1 i32) void", "FuncProtoSingleParamTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *value_node = parser.parse_function_proto(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncProto);
  ASSERT_EQ(value_node->function_proto.name, "myFunc");
  ASSERT_NE(value_node->function_proto.return_type, nullptr);
  ASSERT_EQ(value_node->function_proto.params.size(), 1L);

  auto param_node = value_node->function_proto.params.at(0L);
  ASSERT_EQ(param_node->node_type, AstNodeType::AstParamDecl);
  ASSERT_EQ(param_node->parent, value_node);
  ASSERT_EQ(param_node->param_decl.name, "param1");
  ASSERT_NE(param_node->param_decl.type, nullptr);
  ASSERT_EQ(param_node->param_decl.type->node_type, AstNodeType::AstType);
  ASSERT_EQ(param_node->param_decl.type->parent, param_node);
  ASSERT_EQ(param_node->param_decl.type->ast_type.type_info->name, "i32");
  ASSERT_EQ(param_node->param_decl.type->ast_type.type_id, AstTypeId::Integer);

  auto ret_type_node = value_node->function_proto.return_type;
  ASSERT_EQ(ret_type_node->parent, value_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(ret_type_node->ast_type.type_info->name, "void");
  ASSERT_EQ(ret_type_node->ast_type.type_id, AstTypeId::Void);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncProtoMultiParamTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc(param1 i32, param1 i32, param1 i32) i32", "FuncProtoMultiParamTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *value_node = parser.parse_function_proto(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncProto);
  ASSERT_EQ(value_node->function_proto.name, "myFunc");
  ASSERT_NE(value_node->function_proto.return_type, nullptr);
  ASSERT_EQ(value_node->function_proto.params.size(), 3L);

  for (auto param_node : value_node->function_proto.params) {
    ASSERT_EQ(param_node->node_type, AstNodeType::AstParamDecl);
    ASSERT_EQ(param_node->parent, value_node);
    ASSERT_EQ(param_node->param_decl.name, "param1");
    ASSERT_NE(param_node->param_decl.type, nullptr);
    ASSERT_EQ(param_node->param_decl.type->node_type, AstNodeType::AstType);
    ASSERT_EQ(param_node->param_decl.type->parent, param_node);
    ASSERT_EQ(param_node->param_decl.type->ast_type.type_info->name, "i32");
    ASSERT_EQ(param_node->param_decl.type->ast_type.type_id, AstTypeId::Integer);
  }

  auto ret_type_node = value_node->function_proto.return_type;
  ASSERT_EQ(ret_type_node->parent, value_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(ret_type_node->ast_type.type_info->name, "i32");
  ASSERT_EQ(ret_type_node->ast_type.type_id, AstTypeId::Integer);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncProtoMultiLineTest) {
  std::vector<Error> errors;
  Lexer lexer("fn\nmyFunc\n(param1 i32\n, param1 i32,\n param1\ni32)\ni32", "FuncProtoMultiLineTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *value_node = parser.parse_function_proto(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncProto);
  ASSERT_EQ(value_node->function_proto.name, "myFunc");
  ASSERT_NE(value_node->function_proto.return_type, nullptr);
  ASSERT_EQ(value_node->function_proto.params.size(), 3L);

  for (auto param_node : value_node->function_proto.params) {
    ASSERT_EQ(param_node->node_type, AstNodeType::AstParamDecl);
    ASSERT_EQ(param_node->parent, value_node);
    ASSERT_EQ(param_node->param_decl.name, "param1");
    ASSERT_NE(param_node->param_decl.type, nullptr);
    ASSERT_EQ(param_node->param_decl.type->node_type, AstNodeType::AstType);
    ASSERT_EQ(param_node->param_decl.type->parent, param_node);
    ASSERT_EQ(param_node->param_decl.type->ast_type.type_info->name, "i32");
    ASSERT_EQ(param_node->param_decl.type->ast_type.type_id, AstTypeId::Integer);
  }

  auto ret_type_node = value_node->function_proto.return_type;
  ASSERT_EQ(ret_type_node->parent, value_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(ret_type_node->ast_type.type_info->name, "i32");
  ASSERT_EQ(ret_type_node->ast_type.type_id, AstTypeId::Integer);

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
  AstNode *value_node = parser.parse_function_def(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncDef);
  ASSERT_NE(value_node->function_def.block, nullptr);
  ASSERT_NE(value_node->function_def.proto, nullptr);
  ASSERT_EQ(value_node->function_def.block->parent, value_node);
  ASSERT_EQ(value_node->function_def.block->node_type, AstNodeType::AstBlock);
  ASSERT_EQ(value_node->function_def.block->block.statements.size(), 0L);

  auto proto_node = value_node->function_def.proto;
  ASSERT_EQ(proto_node->parent, value_node);
  ASSERT_EQ(proto_node->node_type, AstNodeType::AstFuncProto);
  ASSERT_EQ(proto_node->function_proto.name, "myFunc");
  ASSERT_EQ(proto_node->function_proto.params.size(), 0L);

  auto ret_type_node = proto_node->function_proto.return_type;
  ASSERT_NE(ret_type_node, nullptr);
  ASSERT_EQ(ret_type_node->parent, proto_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(ret_type_node->ast_type.type_info->name, "void");
  ASSERT_EQ(ret_type_node->ast_type.type_id, AstTypeId::Void);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncDefSingleParamsVoidBlockTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc(param1 i32) i32 {}", "FuncDefSingleParamsVoidBlockTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *value_node = parser.parse_function_def(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncDef);
  ASSERT_NE(value_node->function_def.block, nullptr);
  ASSERT_NE(value_node->function_def.proto, nullptr);
  ASSERT_EQ(value_node->function_def.block->parent, value_node);
  ASSERT_EQ(value_node->function_def.block->node_type, AstNodeType::AstBlock);
  ASSERT_EQ(value_node->function_def.block->block.statements.size(), 0L);

  auto proto_node = value_node->function_def.proto;
  ASSERT_EQ(proto_node->parent, value_node);
  ASSERT_EQ(proto_node->node_type, AstNodeType::AstFuncProto);
  ASSERT_EQ(proto_node->function_proto.name, "myFunc");
  ASSERT_NE(proto_node->function_proto.return_type, nullptr);
  ASSERT_EQ(proto_node->function_proto.params.size(), 1L);

  auto param_node = proto_node->function_proto.params.at(0L);
  ASSERT_EQ(param_node->node_type, AstNodeType::AstParamDecl);
  ASSERT_EQ(param_node->parent, proto_node);
  ASSERT_EQ(param_node->param_decl.name, "param1");
  ASSERT_NE(param_node->param_decl.type, nullptr);
  ASSERT_EQ(param_node->param_decl.type->node_type, AstNodeType::AstType);
  ASSERT_EQ(param_node->param_decl.type->parent, param_node);
  ASSERT_EQ(param_node->param_decl.type->ast_type.type_info->name, "i32");
  ASSERT_EQ(param_node->param_decl.type->ast_type.type_id, AstTypeId::Integer);

  auto ret_type_node = proto_node->function_proto.return_type;
  ASSERT_NE(ret_type_node, nullptr);
  ASSERT_EQ(ret_type_node->parent, proto_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(ret_type_node->ast_type.type_info->name, "i32");
  ASSERT_EQ(ret_type_node->ast_type.type_id, AstTypeId::Integer);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncDefMultiParamsVoidBlockTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc(param1 i32, param1 i32, param1 i32) i32 {}", "FuncDefMultiParamsVoidBlockTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *value_node = parser.parse_function_def(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncDef);
  ASSERT_NE(value_node->function_def.block, nullptr);
  ASSERT_NE(value_node->function_def.proto, nullptr);
  ASSERT_EQ(value_node->function_def.block->parent, value_node);
  ASSERT_EQ(value_node->function_def.block->node_type, AstNodeType::AstBlock);
  ASSERT_EQ(value_node->function_def.block->block.statements.size(), 0L);

  auto proto_node = value_node->function_def.proto;
  ASSERT_EQ(proto_node->parent, value_node);
  ASSERT_EQ(proto_node->node_type, AstNodeType::AstFuncProto);
  ASSERT_EQ(proto_node->function_proto.name, "myFunc");
  ASSERT_NE(proto_node->function_proto.return_type, nullptr);
  ASSERT_EQ(proto_node->function_proto.params.size(), 3L);

  for (auto param_node : proto_node->function_proto.params) {
    ASSERT_EQ(param_node->node_type, AstNodeType::AstParamDecl);
    ASSERT_EQ(param_node->parent, proto_node);
    ASSERT_EQ(param_node->param_decl.name, "param1");
    ASSERT_NE(param_node->param_decl.type, nullptr);
    ASSERT_EQ(param_node->param_decl.type->node_type, AstNodeType::AstType);
    ASSERT_EQ(param_node->param_decl.type->parent, param_node);
    ASSERT_EQ(param_node->param_decl.type->ast_type.type_info->name, "i32");
    ASSERT_EQ(param_node->param_decl.type->ast_type.type_id, AstTypeId::Integer);
  }

  auto ret_type_node = proto_node->function_proto.return_type;
  ASSERT_NE(ret_type_node, nullptr);
  ASSERT_EQ(ret_type_node->parent, proto_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(ret_type_node->ast_type.type_info->name, "i32");
  ASSERT_EQ(ret_type_node->ast_type.type_id, AstTypeId::Integer);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncDefNoParamsTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc() void {\nmyVar i32\n}", "FuncDefNoParamsTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *value_node = parser.parse_function_def(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncDef);
  ASSERT_NE(value_node->function_def.block, nullptr);
  ASSERT_NE(value_node->function_def.proto, nullptr);
  auto block_node = value_node->function_def.block;
  ASSERT_EQ(block_node->parent, value_node);
  ASSERT_EQ(block_node->node_type, AstNodeType::AstBlock);
  ASSERT_EQ(block_node->block.statements.size(), 1L);

  auto var_def_node = value_node->function_def.block->block.statements.at(0);
  ASSERT_NE(var_def_node, nullptr);
  ASSERT_EQ(var_def_node->parent, block_node);
  ASSERT_EQ(var_def_node->node_type, AstNodeType::AstVarDef);
  ASSERT_EQ(var_def_node->var_def.name, "myVar");
  ASSERT_NE(var_def_node->var_def.type, nullptr);
  ASSERT_EQ(var_def_node->var_def.initializer, nullptr);

  auto type_node = var_def_node->var_def.type;
  ASSERT_EQ(type_node->parent, var_def_node);
  ASSERT_EQ(type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(type_node->ast_type.type_id, AstTypeId::Integer);
  ASSERT_EQ(type_node->ast_type.type_info->name, "i32");

  auto proto_node = value_node->function_def.proto;
  ASSERT_EQ(proto_node->parent, value_node);
  ASSERT_EQ(proto_node->node_type, AstNodeType::AstFuncProto);
  ASSERT_EQ(proto_node->function_proto.name, "myFunc");
  ASSERT_EQ(proto_node->function_proto.params.size(), 0L);

  auto ret_type_node = proto_node->function_proto.return_type;
  ASSERT_NE(ret_type_node, nullptr);
  ASSERT_EQ(ret_type_node->parent, proto_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(ret_type_node->ast_type.type_info->name, "void");
  ASSERT_EQ(ret_type_node->ast_type.type_id, AstTypeId::Void);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncDefSingleParamsTest) {
  std::vector<Error> errors;
  Lexer lexer("fn myFunc(param1 i32) i32 {\nret param1 + 5\n}", "FuncDefSingleParamsTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *value_node = parser.parse_function_def(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncDef);
  ASSERT_NE(value_node->function_def.block, nullptr);
  ASSERT_NE(value_node->function_def.proto, nullptr);

  auto block_node = value_node->function_def.block;
  ASSERT_EQ(block_node->parent, value_node);
  ASSERT_EQ(block_node->node_type, AstNodeType::AstBlock);
  ASSERT_EQ(block_node->block.statements.size(), 1L);

  auto ret_node = block_node->block.statements.at(0);
  ASSERT_NE(ret_node, nullptr);
  ASSERT_EQ(ret_node->parent, block_node);
  ASSERT_EQ(ret_node->node_type, AstNodeType::AstUnaryExpr);
  ASSERT_EQ(ret_node->unary_expr.op, UnaryExprType::RET);
  ASSERT_NE(ret_node->unary_expr.expr, nullptr);
  ASSERT_EQ(ret_node->unary_expr.expr->parent, ret_node);
  ASSERT_EQ(ret_node->unary_expr.expr->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(ret_node->unary_expr.expr->binary_expr.bin_op, BinaryExprType::ADD);

  auto proto_node = value_node->function_def.proto;
  ASSERT_EQ(proto_node->parent, value_node);
  ASSERT_EQ(proto_node->node_type, AstNodeType::AstFuncProto);
  ASSERT_EQ(proto_node->function_proto.name, "myFunc");
  ASSERT_NE(proto_node->function_proto.return_type, nullptr);
  ASSERT_EQ(proto_node->function_proto.params.size(), 1L);

  auto param_node = proto_node->function_proto.params.at(0L);
  ASSERT_EQ(param_node->node_type, AstNodeType::AstParamDecl);
  ASSERT_EQ(param_node->parent, proto_node);
  ASSERT_EQ(param_node->param_decl.name, "param1");
  ASSERT_NE(param_node->param_decl.type, nullptr);
  ASSERT_EQ(param_node->param_decl.type->node_type, AstNodeType::AstType);
  ASSERT_EQ(param_node->param_decl.type->parent, param_node);
  ASSERT_EQ(param_node->param_decl.type->ast_type.type_info->name, "i32");
  ASSERT_EQ(param_node->param_decl.type->ast_type.type_id, AstTypeId::Integer);

  auto ret_type_node = proto_node->function_proto.return_type;
  ASSERT_NE(ret_type_node, nullptr);
  ASSERT_EQ(ret_type_node->parent, proto_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(ret_type_node->ast_type.type_info->name, "i32");
  ASSERT_EQ(ret_type_node->ast_type.type_id, AstTypeId::Integer);

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
  AstNode *value_node = parser.parse_function_call(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncCallExpr);
  ASSERT_EQ(value_node->func_call.fn_name, "myFunc");
  ASSERT_EQ(value_node->func_call.args.size(), 0L);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncCall1ParamTest) {
  std::vector<Error> errors;
  Lexer lexer("myFunc(5)", "FuncCall1ParamTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *value_node = parser.parse_function_call(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncCallExpr);
  ASSERT_EQ(value_node->func_call.fn_name, "myFunc");
  ASSERT_EQ(value_node->func_call.args.size(), 1L);

  auto param_node = value_node->func_call.args.at(0);
  ASSERT_NE(param_node, nullptr);
  ASSERT_EQ(param_node->parent, value_node);
  ASSERT_EQ(param_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(param_node->const_value.type, ConstValueType::INT);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncCall1ParamAddOpTest) {
  std::vector<Error> errors;
  Lexer lexer("85 + myFunc(5)", "FuncCall1ParamTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *value_node = parser.parse_algebraic_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ADD);

  const AstNode *const_value_node = value_node->binary_expr.left_expr;
  ASSERT_NE(const_value_node, nullptr);
  ASSERT_EQ(const_value_node->parent, value_node);
  ASSERT_EQ(const_value_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(const_value_node->const_value.type, ConstValueType::INT);
  ASSERT_EQ(const_value_node->const_value.is_negative, false);
  ASSERT_EQ(strcmp(const_value_node->const_value.number, "85"), 0);

  const AstNode *fn_call_node = value_node->binary_expr.right_expr;
  ASSERT_NE(fn_call_node, nullptr);
  ASSERT_EQ(fn_call_node->parent, value_node);
  ASSERT_EQ(fn_call_node->node_type, AstNodeType::AstFuncCallExpr);
  ASSERT_EQ(fn_call_node->func_call.fn_name, "myFunc");
  ASSERT_EQ(fn_call_node->func_call.args.size(), 1L);

  const AstNode *arg_node = fn_call_node->func_call.args.at(0);
  ASSERT_NE(arg_node, nullptr);
  ASSERT_EQ(arg_node->parent, fn_call_node);
  ASSERT_EQ(arg_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(arg_node->const_value.type, ConstValueType::INT);
  ASSERT_EQ(strcmp(arg_node->const_value.number, "5"), 0);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncCallNestedTest) {
  std::vector<Error> errors;
  Lexer lexer("myFunc(myFunc2(5))", "FuncCallNestedTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *value_node = parser.parse_function_call(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncCallExpr);
  ASSERT_EQ(value_node->func_call.fn_name, "myFunc");
  ASSERT_EQ(value_node->func_call.args.size(), 1L);

  auto param_node_func = value_node->func_call.args.at(0);
  ASSERT_NE(param_node_func, nullptr);
  ASSERT_EQ(param_node_func->parent, value_node);
  ASSERT_EQ(param_node_func->node_type, AstNodeType::AstFuncCallExpr);
  ASSERT_EQ(param_node_func->func_call.fn_name, "myFunc2");
  ASSERT_EQ(param_node_func->func_call.args.size(), 1L);

  auto param_node = param_node_func->func_call.args.at(0);
  ASSERT_NE(param_node, nullptr);
  ASSERT_EQ(param_node->parent, param_node_func);
  ASSERT_EQ(param_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(param_node->const_value.type, ConstValueType::INT);

  delete value_node;
}

TEST(ParserHappyStmntTests, FuncCallMultiParamsTest) {
  std::vector<Error> errors;
  Lexer lexer("myFunc(myVar, myFunc2(5))", "FuncCallMultiParamsTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  AstNode *value_node = parser.parse_function_call(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstFuncCallExpr);
  ASSERT_EQ(value_node->func_call.fn_name, "myFunc");
  ASSERT_EQ(value_node->func_call.args.size(), 2L);

  auto param_node = value_node->func_call.args.at(0);
  ASSERT_NE(param_node, nullptr);
  ASSERT_EQ(param_node->parent, value_node);
  ASSERT_EQ(param_node->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(param_node->symbol.token->id, TokenId::IDENTIFIER);

  auto param_node_func = value_node->func_call.args.at(1);
  ASSERT_NE(param_node_func, nullptr);
  ASSERT_EQ(param_node_func->parent, value_node);
  ASSERT_EQ(param_node_func->node_type, AstNodeType::AstFuncCallExpr);
  ASSERT_EQ(param_node_func->func_call.fn_name, "myFunc2");
  ASSERT_EQ(param_node_func->func_call.args.size(), 1L);

  auto func_param_node = param_node_func->func_call.args.at(0);
  ASSERT_NE(func_param_node, nullptr);
  ASSERT_EQ(func_param_node->parent, param_node_func);
  ASSERT_EQ(func_param_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(func_param_node->const_value.type, ConstValueType::INT);

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
  AstNode *source_code_node = parser.parse(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(source_code_node, nullptr);
  ASSERT_EQ(source_code_node->node_type, AstNodeType::AstSourceCode);
  ASSERT_EQ(source_code_node->source_code.children.size(), 2L);
  auto main_stmnts = source_code_node->source_code.children;

  AstNode *var_def_node = main_stmnts.at(0);
  AstNode *func_def_node = main_stmnts.at(1);
  ASSERT_NE(func_def_node, nullptr);
  ASSERT_EQ(func_def_node->node_type, AstNodeType::AstFuncDef);
  ASSERT_NE(func_def_node->function_def.block, nullptr);
  ASSERT_NE(func_def_node->function_def.proto, nullptr);

  auto block_node = func_def_node->function_def.block;
  ASSERT_EQ(block_node->parent, func_def_node);
  ASSERT_EQ(block_node->node_type, AstNodeType::AstBlock);
  ASSERT_EQ(block_node->block.statements.size(), 1L);

  auto ret_node = block_node->block.statements.at(0);
  ASSERT_NE(ret_node, nullptr);
  ASSERT_EQ(ret_node->parent, block_node);
  ASSERT_EQ(ret_node->node_type, AstNodeType::AstUnaryExpr);
  ASSERT_EQ(ret_node->unary_expr.op, UnaryExprType::RET);
  ASSERT_EQ(ret_node->unary_expr.expr, nullptr);

  auto proto_node = func_def_node->function_def.proto;
  ASSERT_EQ(proto_node->parent, func_def_node);
  ASSERT_EQ(proto_node->node_type, AstNodeType::AstFuncProto);
  ASSERT_EQ(proto_node->function_proto.name, "myFunc");
  ASSERT_EQ(proto_node->function_proto.params.size(), 0L);

  auto ret_type_node = proto_node->function_proto.return_type;
  ASSERT_NE(ret_type_node, nullptr);
  ASSERT_EQ(ret_type_node->parent, proto_node);
  ASSERT_EQ(ret_type_node->node_type, AstNodeType::AstType);
  ASSERT_EQ(ret_type_node->ast_type.type_info->name, "void");
  ASSERT_EQ(ret_type_node->ast_type.type_id, AstTypeId::Void);

  delete source_code_node;
}
