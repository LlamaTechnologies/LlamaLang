#include "../../src/ast_nodes.hpp"
#include "../../src/lexer.hpp"
#include "../../src/parser.hpp"

#include <gtest/gtest.h>
#include <string.h>

//==================================================================================
//          PARSE VALUE FUNCTIONS
//==================================================================================

TEST(ParserHappyParseValueTests, IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar", "file/directory", "IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *node = parser.parse_primary_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(node, nullptr);
  ASSERT_EQ(node->node_type, AstNodeType::AST_SYMBOL);

  const AstSymbol *symbol_node = node->symbol();
  ASSERT_NE(symbol_node->token, nullptr);
  ASSERT_EQ(symbol_node->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(symbol_node->cached_name, "myVar");

  delete node;
}

TEST(ParserHappyParseValueTests, FloatTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547.12f", "file/directory", "FloatTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *node = parser.parse_primary_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(node, nullptr);
  ASSERT_EQ(node->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(node->const_value()->type, ConstValueType::FLOAT);

  delete node;
}

TEST(ParserHappyParseValueTests, IntTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547", "file/directory", "IntTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *node = parser.parse_primary_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(node, nullptr);
  ASSERT_EQ(node->node_type, AstNodeType::AST_CONST_VALUE);

  const AstConstValue *const_value_node = node->const_value();
  ASSERT_EQ(const_value_node->type, ConstValueType::INT);
  ASSERT_EQ(strcmp(const_value_node->number, "12547"), 0);

  delete node;
}

TEST(ParserHappyParseValueTests, UnicodeCharTest) {
  std::vector<Error> errors;
  Lexer lexer("\'g\'", "file/directory", "UnicodeCharTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *node = parser.parse_primary_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(node, nullptr);
  ASSERT_EQ(node->node_type, AstNodeType::AST_CONST_VALUE);

  const AstConstValue *const_value_node = node->const_value();
  ASSERT_EQ(const_value_node->type, ConstValueType::CHAR);
  ASSERT_EQ(const_value_node->unicode_char, 'g');

  delete node;
}

//==================================================================================
//          PARSE UNARY EXPRESSIONS FUNCTIONS
//==================================================================================

TEST(ParserHappyParseUnaryExprTests, PreIncIdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("++myVar", "file/directory", "PreIncIdentifierTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_unary_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_UNARY_EXPR);

  const AstUnaryExpr *unary_expr = value_node->unary_expr();
  ASSERT_EQ(unary_expr->expr->parent, value_node);
  ASSERT_EQ(unary_expr->op, UnaryExprType::INC);
  ASSERT_EQ(unary_expr->expr->symbol()->token->id, TokenId::IDENTIFIER);

  delete value_node;
}

TEST(ParserHappyParseUnaryExprTests, PostIncIdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar++", "file/directory", "PostIncIdentifierTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_unary_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_UNARY_EXPR);

  const AstUnaryExpr *unary_expr = value_node->unary_expr();
  ASSERT_EQ(unary_expr->expr->parent, value_node);
  ASSERT_EQ(unary_expr->op, UnaryExprType::INC);
  ASSERT_EQ(unary_expr->expr->symbol()->token->id, TokenId::IDENTIFIER);

  delete value_node;
}

TEST(ParserHappyParseUnaryExprTests, PreIncFuncCallTest) {
  std::vector<Error> errors;
  Lexer lexer("++myFunc()", "file/directory", "PreIncFuncCallTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_unary_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_UNARY_EXPR);

  const AstUnaryExpr *unary_expr = value_node->unary_expr();
  ASSERT_EQ(unary_expr->expr->parent, value_node);
  ASSERT_EQ(unary_expr->op, UnaryExprType::INC);
  ASSERT_NE(unary_expr->expr, nullptr);
  ASSERT_EQ(unary_expr->expr->node_type, AstNodeType::AST_FN_CALL_EXPR);

  const AstFnCallExpr *func_call = unary_expr->expr->fn_call();
  ASSERT_EQ(func_call->parent, value_node);
  ASSERT_EQ(func_call->fn_name, "myFunc");
  ASSERT_EQ(func_call->args.size(), 0L);

  delete value_node;
}

TEST(ParserHappyParseUnaryExprTests, IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar", "file/directory", "IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_unary_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_SYMBOL);

  const AstSymbol *symbol_node = value_node->symbol();
  ASSERT_NE(symbol_node->token, nullptr);
  ASSERT_EQ(symbol_node->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(symbol_node->cached_name, "myVar");

  delete value_node;
}

TEST(ParserHappyParseUnaryExprTests, FloatTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547.12f", "file/directory", "FloatTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_unary_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(value_node->const_value()->type, ConstValueType::FLOAT);

  delete value_node;
}

TEST(ParserHappyParseUnaryExprTests, IntTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547", "file/directory", "IntTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_unary_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_CONST_VALUE);

  const AstConstValue *const_value = value_node->const_value();
  ASSERT_EQ(const_value->type, ConstValueType::INT);
  ASSERT_EQ(strcmp(const_value->number, "12547"), 0);

  delete value_node;
}

TEST(ParserHappyParseUnaryExprTests, NegativeIntTest) {
  std::vector<Error> errors;
  Lexer lexer("-12_547", "file/directory", "NegativeIntTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_unary_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->parent, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_CONST_VALUE);

  const AstConstValue *const_value = value_node->const_value();
  ASSERT_EQ(const_value->type, ConstValueType::INT);
  ASSERT_NE(const_value->number, nullptr);
  ASSERT_EQ(strcmp(const_value->number, "-12547"), 0);
  ASSERT_EQ(const_value->is_negative, true);

  delete value_node;
}

TEST(ParserHappyParseUnaryExprTests, ExplicitPositiveIntTest) {
  std::vector<Error> errors;
  Lexer lexer("+12_547", "file/directory", "ExplicitPositiveIntTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_unary_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_CONST_VALUE);

  const AstConstValue *const_value = value_node->const_value();
  ASSERT_EQ(const_value->type, ConstValueType::INT);
  ASSERT_EQ(strcmp(const_value->number, "12547"), 0);
  ASSERT_EQ(const_value->is_negative, false);

  delete value_node;
}

TEST(ParserHappyParseUnaryExprTests, UnicodeCharTest) {
  std::vector<Error> errors;
  Lexer lexer("\'g\'", "file/directory", "UnicodeCharTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_unary_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_CONST_VALUE);

  const AstConstValue *const_value = value_node->const_value();
  ASSERT_EQ(const_value->type, ConstValueType::CHAR);
  ASSERT_EQ(const_value->unicode_char, 'g');

  delete value_node;
}

//==================================================================================
//          PARSE PRODUCT EXPRESSIONS FUNCTIONS
//==================================================================================

TEST(ParserHappyParseMulExprTests, Mul2IdentifierAndDecTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar-- * myVar2", "file/directory", "Mul2IdentifierAndDecTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_term_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->right_expr->symbol()->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::MUL);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_UNARY_EXPR);
  ASSERT_EQ(binary_expr->left_expr->parent, value_node);
  ASSERT_EQ(binary_expr->right_expr->parent, value_node);

  const AstUnaryExpr *unary_expr = binary_expr->left_expr->unary_expr();
  ASSERT_EQ(unary_expr->op, UnaryExprType::DEC);
  ASSERT_EQ(unary_expr->expr->symbol()->token->id, TokenId::IDENTIFIER);

  delete value_node;
}

TEST(ParserHappyParseMulExprTests, Mul2IdentifierAndFuncCallTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar-- * myFunc()", "file/directory", "Mul2IdentifierAndFuncCallTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_term_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::MUL);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_UNARY_EXPR);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_FN_CALL_EXPR);

  const AstUnaryExpr *unary_expr = binary_expr->left_expr->unary_expr();
  ASSERT_EQ(unary_expr->op, UnaryExprType::DEC);
  ASSERT_EQ(unary_expr->expr->symbol()->token->id, TokenId::IDENTIFIER);

  const AstFnCallExpr *func_call = binary_expr->right_expr->fn_call();
  ASSERT_NE(func_call, nullptr);
  ASSERT_EQ(func_call->parent, binary_expr);
  ASSERT_EQ(func_call->fn_name, "myFunc");
  ASSERT_EQ(func_call->args.size(), 0L);

  delete value_node;
}

TEST(ParserHappyParseMulExprTests, Mul2IdentifierAndIncTest) {
  std::vector<Error> errors;
  Lexer lexer("++myVar * myVar2", "file/directory", "Mul2IdentifierAndIncTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_term_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::MUL);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);

  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(binary_expr->right_expr->symbol()->token->id, TokenId::IDENTIFIER);

  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_UNARY_EXPR);

  const AstUnaryExpr *unary_expr = binary_expr->left_expr->unary_expr();
  ASSERT_EQ(unary_expr->op, UnaryExprType::INC);
  ASSERT_EQ(unary_expr->expr->symbol()->token->id, TokenId::IDENTIFIER);

  delete value_node;
}

TEST(ParserHappyParseMulExprTests, Mul2IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar * myVar2", "file/directory", "Mul2IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_term_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::MUL);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);

  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_SYMBOL);

  ASSERT_EQ(binary_expr->left_expr->symbol()->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(binary_expr->right_expr->symbol()->token->id, TokenId::IDENTIFIER);

  delete value_node;
}

TEST(ParserHappyParseMulExprTests, MulNumberIdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("25 * myVar2", "file/directory", "MulNumberIdentifierTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_term_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::MUL);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(binary_expr->left_expr->const_value()->type, ConstValueType::INT);
  ASSERT_EQ(binary_expr->right_expr->symbol()->token->id, TokenId::IDENTIFIER);

  delete value_node;
}

TEST(ParserHappyParseMulExprTests, Mul2NumbersTest) {
  std::vector<Error> errors;
  Lexer lexer("25 * 21.5f", "file/directory", "Mul2NumbersTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_term_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::MUL);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(binary_expr->left_expr->const_value()->type, ConstValueType::INT);
  ASSERT_EQ(binary_expr->right_expr->const_value()->type, ConstValueType::FLOAT);

  delete value_node;
}

TEST(ParserHappyParseMulExprTests, Mul2NumbersAndCharTest) {
  std::vector<Error> errors;
  Lexer lexer("25 * 21.5f / 'g'", "file/directory", "Mul2NumbersAndCharTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_term_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::DIV);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_BINARY_EXPR);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_CONST_VALUE);

  ASSERT_EQ(binary_expr->right_expr->const_value()->type, ConstValueType::CHAR);

  auto bin_exp_op1 = binary_expr->left_expr->binary_expr();
  ASSERT_EQ(bin_exp_op1->bin_op, BinaryExprType::MUL);
  ASSERT_NE(bin_exp_op1->left_expr, nullptr);
  ASSERT_NE(bin_exp_op1->right_expr, nullptr);
  ASSERT_EQ(bin_exp_op1->left_expr->parent, bin_exp_op1);
  ASSERT_EQ(bin_exp_op1->right_expr->parent, bin_exp_op1);
  ASSERT_EQ(bin_exp_op1->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(bin_exp_op1->right_expr->node_type, AstNodeType::AST_CONST_VALUE);

  ASSERT_EQ(bin_exp_op1->left_expr->const_value()->type, ConstValueType::INT);
  ASSERT_EQ(bin_exp_op1->right_expr->const_value()->type, ConstValueType::FLOAT);

  delete value_node;
}

TEST(ParserHappyParseMulExprTests, IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar", "file/directory", "IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_term_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_SYMBOL);

  const AstSymbol *symbol_node = value_node->symbol();
  ASSERT_NE(symbol_node->token, nullptr);
  ASSERT_EQ(symbol_node->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(symbol_node->cached_name, "myVar");

  delete value_node;
}

TEST(ParserHappyParseMulExprTests, FloatTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547.12f", "file/directory", "FloatTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_term_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(value_node->const_value()->type, ConstValueType::FLOAT);

  delete value_node;
}

TEST(ParserHappyParseMulExprTests, IntTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547", "file/directory", "IntTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_term_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_CONST_VALUE);

  const AstConstValue *const_value = value_node->const_value();
  ASSERT_EQ(const_value->type, ConstValueType::INT);
  ASSERT_EQ(strcmp(const_value->number, "12547"), 0);

  delete value_node;
}

TEST(ParserHappyParseMulExprTests, UnicodeCharTest) {
  std::vector<Error> errors;
  Lexer lexer("\'g\'", "file/directory", "UnicodeCharTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_term_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_CONST_VALUE);

  const AstConstValue *const_value = value_node->const_value();
  ASSERT_EQ(const_value->type, ConstValueType::CHAR);
  ASSERT_EQ(const_value->unicode_char, 'g');

  delete value_node;
}

//==================================================================================
//          PARSE ADDITION EXPRESSIONS FUNCTIONS
//==================================================================================
TEST(ParserHappyParseAddExprTests, Add2IdentifierAndIncTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar++ + myVar2", "file/directory", "Add2IdentifierAndIncTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_algebraic_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::ADD);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_UNARY_EXPR);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_SYMBOL);

  const AstUnaryExpr *bin_exp_left = binary_expr->left_expr->unary_expr();
  ASSERT_EQ(bin_exp_left->op, UnaryExprType::INC);
  ASSERT_NE(bin_exp_left->expr, nullptr);
  ASSERT_EQ(bin_exp_left->expr->node_type, AstNodeType::AST_SYMBOL);

  const AstSymbol *symbol_node = bin_exp_left->expr->symbol();
  ASSERT_EQ(symbol_node->parent, bin_exp_left);
  ASSERT_EQ(symbol_node->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(symbol_node->cached_name, "myVar");

  const AstSymbol *bin_exp_right = binary_expr->right_expr->symbol();
  ASSERT_EQ(bin_exp_right->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(bin_exp_right->cached_name, "myVar2");

  delete value_node;
}

TEST(ParserHappyParseAddExprTests, Add2IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar + myVar2", "file/directory", "Add2IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_algebraic_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::ADD);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(binary_expr->left_expr->symbol()->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(binary_expr->right_expr->symbol()->token->id, TokenId::IDENTIFIER);

  delete value_node;
}

TEST(ParserHappyParseAddExprTests, AddIdentifierNumberTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar + 25", "file/directory", "AddIdentifierNumberTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_algebraic_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::ADD);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(binary_expr->left_expr->symbol()->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(binary_expr->right_expr->const_value()->type, ConstValueType::INT);

  delete value_node;
}

TEST(ParserHappyParseAddExprTests, Add2NumbersTest) {
  std::vector<Error> errors;
  Lexer lexer(".25f + 25", "file/directory", "Add2NumbersTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_algebraic_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::ADD);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(binary_expr->left_expr->const_value()->type, ConstValueType::FLOAT);
  ASSERT_EQ(binary_expr->right_expr->const_value()->type, ConstValueType::INT);

  delete value_node;
}

TEST(ParserHappyParseAddExprTests, Add2NumberAndCharTest) {
  std::vector<Error> errors;
  Lexer lexer(".25f + 25 - 'g'", "file/directory", "Add2NumberAndCharTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_algebraic_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::SUB);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_BINARY_EXPR);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(binary_expr->right_expr->const_value()->type, ConstValueType::CHAR);

  const AstBinaryExpr *bin_op1 = binary_expr->left_expr->binary_expr();
  ASSERT_EQ(bin_op1->bin_op, BinaryExprType::ADD);
  ASSERT_NE(bin_op1->left_expr, nullptr);
  ASSERT_NE(bin_op1->right_expr, nullptr);
  ASSERT_EQ(bin_op1->left_expr->parent, bin_op1);
  ASSERT_EQ(bin_op1->right_expr->parent, bin_op1);
  ASSERT_EQ(bin_op1->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(bin_op1->right_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(bin_op1->left_expr->const_value()->type, ConstValueType::FLOAT);
  ASSERT_EQ(bin_op1->right_expr->const_value()->type, ConstValueType::INT);

  delete value_node;
}

TEST(ParserHappyParseAddExprTests, FullAlgebraicTest) {
  std::vector<Error> errors;
  Lexer lexer(".25f + 25 * 14 - 'g' % 15", "file/directory", "FullAlgebraicTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_algebraic_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::SUB);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_BINARY_EXPR);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *gMod15 = binary_expr->right_expr->binary_expr();
  ASSERT_EQ(gMod15->bin_op, BinaryExprType::MOD);
  ASSERT_EQ(gMod15->right_expr->parent, gMod15);
  ASSERT_EQ(gMod15->left_expr->parent, gMod15);
  ASSERT_EQ(gMod15->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(gMod15->right_expr->node_type, AstNodeType::AST_CONST_VALUE);

  const AstBinaryExpr *floatPlusMul = binary_expr->left_expr->binary_expr();
  ASSERT_EQ(floatPlusMul->bin_op, BinaryExprType::ADD);
  ASSERT_NE(floatPlusMul->left_expr, nullptr);
  ASSERT_EQ(floatPlusMul->left_expr->parent, floatPlusMul);
  ASSERT_EQ(floatPlusMul->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_NE(floatPlusMul->right_expr, nullptr);
  ASSERT_EQ(floatPlusMul->right_expr->parent, floatPlusMul);
  ASSERT_EQ(floatPlusMul->right_expr->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *intMulInt = floatPlusMul->right_expr->binary_expr();
  ASSERT_EQ(intMulInt->bin_op, BinaryExprType::MUL);
  ASSERT_NE(intMulInt->left_expr, nullptr);
  ASSERT_NE(intMulInt->right_expr, nullptr);
  ASSERT_EQ(intMulInt->left_expr->parent, intMulInt);
  ASSERT_EQ(intMulInt->right_expr->parent, intMulInt);
  ASSERT_EQ(intMulInt->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(intMulInt->right_expr->node_type, AstNodeType::AST_CONST_VALUE);

  delete value_node;
}

TEST(ParserHappyParseAddExprTests, Mul2IdentifierAndDecTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar-- * myVar2", "file/directory", "Mul2IdentifierAndDecTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_algebraic_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->right_expr->symbol()->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::MUL);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_UNARY_EXPR);
  ASSERT_EQ(binary_expr->left_expr->parent, value_node);
  ASSERT_EQ(binary_expr->right_expr->parent, value_node);

  const AstUnaryExpr *unary_expr = binary_expr->left_expr->unary_expr();
  ASSERT_EQ(unary_expr->op, UnaryExprType::DEC);
  ASSERT_EQ(unary_expr->expr->symbol()->token->id, TokenId::IDENTIFIER);

  delete value_node;
}

TEST(ParserHappyParseAddExprTests, Mul2IdentifierAndFuncCallTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar-- * myFunc()", "file/directory", "Mul2IdentifierAndFuncCallTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_algebraic_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::MUL);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_UNARY_EXPR);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_FN_CALL_EXPR);

  const AstUnaryExpr *unary_expr = binary_expr->left_expr->unary_expr();
  ASSERT_EQ(unary_expr->op, UnaryExprType::DEC);
  ASSERT_EQ(unary_expr->expr->symbol()->token->id, TokenId::IDENTIFIER);

  const AstFnCallExpr *func_call = binary_expr->right_expr->fn_call();
  ASSERT_NE(func_call, nullptr);
  ASSERT_EQ(func_call->parent, binary_expr);
  ASSERT_EQ(func_call->fn_name, "myFunc");
  ASSERT_EQ(func_call->args.size(), 0L);

  delete value_node;
}

TEST(ParserHappyParseAddExprTests, Mul2IdentifierAndIncTest) {
  std::vector<Error> errors;
  Lexer lexer("++myVar * myVar2", "file/directory", "Mul2IdentifierAndIncTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_algebraic_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::MUL);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);

  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(binary_expr->right_expr->symbol()->token->id, TokenId::IDENTIFIER);

  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_UNARY_EXPR);

  const AstUnaryExpr *unary_expr = binary_expr->left_expr->unary_expr();
  ASSERT_EQ(unary_expr->op, UnaryExprType::INC);
  ASSERT_EQ(unary_expr->expr->symbol()->token->id, TokenId::IDENTIFIER);

  delete value_node;
}

TEST(ParserHappyParseAddExprTests, Mul2IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar * myVar2", "file/directory", "Mul2IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_algebraic_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::MUL);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);

  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_SYMBOL);

  ASSERT_EQ(binary_expr->left_expr->symbol()->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(binary_expr->right_expr->symbol()->token->id, TokenId::IDENTIFIER);

  delete value_node;
}

TEST(ParserHappyParseAddExprTests, MulNumberIdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("25 * myVar2", "file/directory", "MulNumberIdentifierTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_algebraic_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::MUL);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(binary_expr->left_expr->const_value()->type, ConstValueType::INT);
  ASSERT_EQ(binary_expr->right_expr->symbol()->token->id, TokenId::IDENTIFIER);

  delete value_node;
}

TEST(ParserHappyParseAddExprTests, Mul2NumbersTest) {
  std::vector<Error> errors;
  Lexer lexer("25 * 21.5f", "file/directory", "Mul2NumbersTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_algebraic_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::MUL);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(binary_expr->left_expr->const_value()->type, ConstValueType::INT);
  ASSERT_EQ(binary_expr->right_expr->const_value()->type, ConstValueType::FLOAT);

  delete value_node;
}

TEST(ParserHappyParseAddExprTests, Mul2NumbersAndCharTest) {
  std::vector<Error> errors;
  Lexer lexer("25 * 21.5f / 'g'", "file/directory", "Mul2NumbersAndCharTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_algebraic_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::DIV);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_BINARY_EXPR);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_CONST_VALUE);

  ASSERT_EQ(binary_expr->right_expr->const_value()->type, ConstValueType::CHAR);

  auto bin_exp_op1 = binary_expr->left_expr->binary_expr();
  ASSERT_EQ(bin_exp_op1->bin_op, BinaryExprType::MUL);
  ASSERT_NE(bin_exp_op1->left_expr, nullptr);
  ASSERT_NE(bin_exp_op1->right_expr, nullptr);
  ASSERT_EQ(bin_exp_op1->left_expr->parent, bin_exp_op1);
  ASSERT_EQ(bin_exp_op1->right_expr->parent, bin_exp_op1);
  ASSERT_EQ(bin_exp_op1->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(bin_exp_op1->right_expr->node_type, AstNodeType::AST_CONST_VALUE);

  ASSERT_EQ(bin_exp_op1->left_expr->const_value()->type, ConstValueType::INT);
  ASSERT_EQ(bin_exp_op1->right_expr->const_value()->type, ConstValueType::FLOAT);

  delete value_node;
}

TEST(ParserHappyParseAddExprTests, IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar", "file/directory", "IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_algebraic_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_SYMBOL);

  const AstSymbol *symbol_node = value_node->symbol();
  ASSERT_NE(symbol_node->token, nullptr);
  ASSERT_EQ(symbol_node->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(symbol_node->cached_name, "myVar");

  delete value_node;
}

TEST(ParserHappyParseAddExprTests, FloatTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547.12f", "file/directory", "FloatTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_algebraic_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(value_node->const_value()->type, ConstValueType::FLOAT);

  delete value_node;
}

TEST(ParserHappyParseAddExprTests, IntTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547", "file/directory", "IntTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_algebraic_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_CONST_VALUE);

  const AstConstValue *const_value = value_node->const_value();
  ASSERT_EQ(const_value->type, ConstValueType::INT);
  ASSERT_EQ(strcmp(const_value->number, "12547"), 0);

  delete value_node;
}

TEST(ParserHappyParseAddExprTests, UnicodeCharTest) {
  std::vector<Error> errors;
  Lexer lexer("\'g\'", "file/directory", "UnicodeCharTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_algebraic_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_CONST_VALUE);

  const AstConstValue *const_value = value_node->const_value();
  ASSERT_EQ(const_value->type, ConstValueType::CHAR);
  ASSERT_EQ(const_value->unicode_char, 'g');

  delete value_node;
}

//==================================================================================
//          PARSE COMPARATIVE EXPRESSIONS FUNCTIONS
//==================================================================================

TEST(ParserHappyParseCompExprTests, Comp2IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar == myVar2", "file/directory", "Add2IdentifierAndIncTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_comp_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::EQUALS);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, value_node);
  ASSERT_EQ(binary_expr->right_expr->parent, value_node);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(binary_expr->left_expr->symbol()->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(binary_expr->right_expr->symbol()->token->id, TokenId::IDENTIFIER);

  delete value_node;
}

TEST(ParserHappyParseCompExprTests, Comp2IdentifierAndNumberTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar == myVar2 >= 45", "file/directory", "Add2IdentifierAndIncTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_comp_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::GREATER_OR_EQUALS);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_BINARY_EXPR);
  ASSERT_EQ(binary_expr->right_expr->const_value()->type, ConstValueType::INT);

  const AstBinaryExpr *identComp = binary_expr->left_expr->binary_expr();
  ASSERT_EQ(identComp->bin_op, BinaryExprType::EQUALS);
  ASSERT_NE(identComp->left_expr, nullptr);
  ASSERT_NE(identComp->right_expr, nullptr);
  ASSERT_EQ(identComp->left_expr->parent, identComp);
  ASSERT_EQ(identComp->right_expr->parent, identComp);
  ASSERT_EQ(identComp->left_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(identComp->right_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(identComp->left_expr->symbol()->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(identComp->right_expr->symbol()->token->id, TokenId::IDENTIFIER);

  delete value_node;
}

TEST(ParserHappyParseCompExprTests, Add2IdentifierAndIncTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar++ + myVar2", "file/directory", "Add2IdentifierAndIncTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_comp_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::ADD);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_UNARY_EXPR);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_SYMBOL);

  const AstUnaryExpr *bin_exp_left = binary_expr->left_expr->unary_expr();
  ASSERT_EQ(bin_exp_left->op, UnaryExprType::INC);
  ASSERT_NE(bin_exp_left->expr, nullptr);
  ASSERT_EQ(bin_exp_left->expr->node_type, AstNodeType::AST_SYMBOL);

  const AstSymbol *symbol_node = bin_exp_left->expr->symbol();
  ASSERT_EQ(symbol_node->parent, bin_exp_left);
  ASSERT_EQ(symbol_node->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(symbol_node->cached_name, "myVar");

  const AstSymbol *bin_exp_right = binary_expr->right_expr->symbol();
  ASSERT_EQ(bin_exp_right->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(bin_exp_right->cached_name, "myVar2");

  delete value_node;
}

TEST(ParserHappyParseCompExprTests, Add2IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar + myVar2", "file/directory", "Add2IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_comp_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::ADD);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(binary_expr->left_expr->symbol()->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(binary_expr->right_expr->symbol()->token->id, TokenId::IDENTIFIER);

  delete value_node;
}

TEST(ParserHappyParseCompExprTests, AddIdentifierNumberTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar + 25", "file/directory", "AddIdentifierNumberTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_comp_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::ADD);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(binary_expr->left_expr->symbol()->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(binary_expr->right_expr->const_value()->type, ConstValueType::INT);

  delete value_node;
}

TEST(ParserHappyParseCompExprTests, Add2NumbersTest) {
  std::vector<Error> errors;
  Lexer lexer(".25f + 25", "file/directory", "Add2NumbersTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_comp_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::ADD);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(binary_expr->left_expr->const_value()->type, ConstValueType::FLOAT);
  ASSERT_EQ(binary_expr->right_expr->const_value()->type, ConstValueType::INT);

  delete value_node;
}

TEST(ParserHappyParseCompExprTests, Add2NumberAndCharTest) {
  std::vector<Error> errors;
  Lexer lexer(".25f + 25 - 'g'", "file/directory", "Add2NumberAndCharTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_comp_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::SUB);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_BINARY_EXPR);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(binary_expr->right_expr->const_value()->type, ConstValueType::CHAR);

  const AstBinaryExpr *bin_op1 = binary_expr->left_expr->binary_expr();
  ASSERT_EQ(bin_op1->bin_op, BinaryExprType::ADD);
  ASSERT_NE(bin_op1->left_expr, nullptr);
  ASSERT_NE(bin_op1->right_expr, nullptr);
  ASSERT_EQ(bin_op1->left_expr->parent, bin_op1);
  ASSERT_EQ(bin_op1->right_expr->parent, bin_op1);
  ASSERT_EQ(bin_op1->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(bin_op1->right_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(bin_op1->left_expr->const_value()->type, ConstValueType::FLOAT);
  ASSERT_EQ(bin_op1->right_expr->const_value()->type, ConstValueType::INT);

  delete value_node;
}

TEST(ParserHappyParseCompExprTests, FullAlgebraicTest) {
  std::vector<Error> errors;
  Lexer lexer(".25f + 25 * 14 - 'g' % 15", "file/directory", "FullAlgebraicTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_comp_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::SUB);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_BINARY_EXPR);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *gMod15 = binary_expr->right_expr->binary_expr();
  ASSERT_EQ(gMod15->bin_op, BinaryExprType::MOD);
  ASSERT_EQ(gMod15->right_expr->parent, gMod15);
  ASSERT_EQ(gMod15->left_expr->parent, gMod15);
  ASSERT_EQ(gMod15->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(gMod15->right_expr->node_type, AstNodeType::AST_CONST_VALUE);

  const AstBinaryExpr *floatPlusMul = binary_expr->left_expr->binary_expr();
  ASSERT_EQ(floatPlusMul->bin_op, BinaryExprType::ADD);
  ASSERT_NE(floatPlusMul->left_expr, nullptr);
  ASSERT_EQ(floatPlusMul->left_expr->parent, floatPlusMul);
  ASSERT_EQ(floatPlusMul->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_NE(floatPlusMul->right_expr, nullptr);
  ASSERT_EQ(floatPlusMul->right_expr->parent, floatPlusMul);
  ASSERT_EQ(floatPlusMul->right_expr->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *intMulInt = floatPlusMul->right_expr->binary_expr();
  ASSERT_EQ(intMulInt->bin_op, BinaryExprType::MUL);
  ASSERT_NE(intMulInt->left_expr, nullptr);
  ASSERT_NE(intMulInt->right_expr, nullptr);
  ASSERT_EQ(intMulInt->left_expr->parent, intMulInt);
  ASSERT_EQ(intMulInt->right_expr->parent, intMulInt);
  ASSERT_EQ(intMulInt->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(intMulInt->right_expr->node_type, AstNodeType::AST_CONST_VALUE);

  delete value_node;
}

TEST(ParserHappyParseCompExprTests, Mul2IdentifierAndDecTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar-- * myVar2", "file/directory", "Mul2IdentifierAndDecTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_comp_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->right_expr->symbol()->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::MUL);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_UNARY_EXPR);
  ASSERT_EQ(binary_expr->left_expr->parent, value_node);
  ASSERT_EQ(binary_expr->right_expr->parent, value_node);

  const AstUnaryExpr *unary_expr = binary_expr->left_expr->unary_expr();
  ASSERT_EQ(unary_expr->op, UnaryExprType::DEC);
  ASSERT_EQ(unary_expr->expr->symbol()->token->id, TokenId::IDENTIFIER);

  delete value_node;
}

TEST(ParserHappyParseCompExprTests, Mul2IdentifierAndFuncCallTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar-- * myFunc()", "file/directory", "Mul2IdentifierAndFuncCallTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_comp_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::MUL);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_UNARY_EXPR);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_FN_CALL_EXPR);

  const AstUnaryExpr *unary_expr = binary_expr->left_expr->unary_expr();
  ASSERT_EQ(unary_expr->op, UnaryExprType::DEC);
  ASSERT_EQ(unary_expr->expr->symbol()->token->id, TokenId::IDENTIFIER);

  const AstFnCallExpr *func_call = binary_expr->right_expr->fn_call();
  ASSERT_NE(func_call, nullptr);
  ASSERT_EQ(func_call->parent, binary_expr);
  ASSERT_EQ(func_call->fn_name, "myFunc");
  ASSERT_EQ(func_call->args.size(), 0L);

  delete value_node;
}

TEST(ParserHappyParseCompExprTests, Mul2IdentifierAndIncTest) {
  std::vector<Error> errors;
  Lexer lexer("++myVar * myVar2", "file/directory", "Mul2IdentifierAndIncTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_comp_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::MUL);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);

  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(binary_expr->right_expr->symbol()->token->id, TokenId::IDENTIFIER);

  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_UNARY_EXPR);

  const AstUnaryExpr *unary_expr = binary_expr->left_expr->unary_expr();
  ASSERT_EQ(unary_expr->op, UnaryExprType::INC);
  ASSERT_EQ(unary_expr->expr->symbol()->token->id, TokenId::IDENTIFIER);

  delete value_node;
}

TEST(ParserHappyParseCompExprTests, Mul2IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar * myVar2", "file/directory", "Mul2IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_comp_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::MUL);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);

  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_SYMBOL);

  ASSERT_EQ(binary_expr->left_expr->symbol()->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(binary_expr->right_expr->symbol()->token->id, TokenId::IDENTIFIER);

  delete value_node;
}

TEST(ParserHappyParseCompExprTests, MulNumberIdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("25 * myVar2", "file/directory", "MulNumberIdentifierTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_comp_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::MUL);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(binary_expr->left_expr->const_value()->type, ConstValueType::INT);
  ASSERT_EQ(binary_expr->right_expr->symbol()->token->id, TokenId::IDENTIFIER);

  delete value_node;
}

TEST(ParserHappyParseCompExprTests, Mul2NumbersTest) {
  std::vector<Error> errors;
  Lexer lexer("25 * 21.5f", "file/directory", "Mul2NumbersTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_comp_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::MUL);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(binary_expr->left_expr->const_value()->type, ConstValueType::INT);
  ASSERT_EQ(binary_expr->right_expr->const_value()->type, ConstValueType::FLOAT);

  delete value_node;
}

TEST(ParserHappyParseCompExprTests, Mul2NumbersAndCharTest) {
  std::vector<Error> errors;
  Lexer lexer("25 * 21.5f / 'g'", "file/directory", "Mul2NumbersAndCharTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_comp_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::DIV);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_BINARY_EXPR);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_CONST_VALUE);

  ASSERT_EQ(binary_expr->right_expr->const_value()->type, ConstValueType::CHAR);

  auto bin_exp_op1 = binary_expr->left_expr->binary_expr();
  ASSERT_EQ(bin_exp_op1->bin_op, BinaryExprType::MUL);
  ASSERT_NE(bin_exp_op1->left_expr, nullptr);
  ASSERT_NE(bin_exp_op1->right_expr, nullptr);
  ASSERT_EQ(bin_exp_op1->left_expr->parent, bin_exp_op1);
  ASSERT_EQ(bin_exp_op1->right_expr->parent, bin_exp_op1);
  ASSERT_EQ(bin_exp_op1->left_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(bin_exp_op1->right_expr->node_type, AstNodeType::AST_CONST_VALUE);

  ASSERT_EQ(bin_exp_op1->left_expr->const_value()->type, ConstValueType::INT);
  ASSERT_EQ(bin_exp_op1->right_expr->const_value()->type, ConstValueType::FLOAT);

  delete value_node;
}

TEST(ParserHappyParseCompExprTests, IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar", "file/directory", "IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_comp_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_SYMBOL);

  const AstSymbol *symbol_node = value_node->symbol();
  ASSERT_NE(symbol_node->token, nullptr);
  ASSERT_EQ(symbol_node->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(symbol_node->cached_name, "myVar");

  delete value_node;
}

TEST(ParserHappyParseCompExprTests, FloatTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547.12f", "file/directory", "FloatTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_comp_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(value_node->const_value()->type, ConstValueType::FLOAT);

  delete value_node;
}

TEST(ParserHappyParseCompExprTests, IntTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547", "file/directory", "IntTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_comp_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_CONST_VALUE);

  const AstConstValue *const_value = value_node->const_value();
  ASSERT_EQ(const_value->type, ConstValueType::INT);
  ASSERT_EQ(strcmp(const_value->number, "12547"), 0);

  delete value_node;
}

TEST(ParserHappyParseCompExprTests, UnicodeCharTest) {
  std::vector<Error> errors;
  Lexer lexer("\'g\'", "file/directory", "UnicodeCharTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstNode *value_node = parser.parse_comp_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_CONST_VALUE);

  const AstConstValue *const_value = value_node->const_value();
  ASSERT_EQ(const_value->type, ConstValueType::CHAR);
  ASSERT_EQ(const_value->unicode_char, 'g');

  delete value_node;
}

//==================================================================================
//          PARSE EXPRESSIONS FUNCTIONS
//==================================================================================

TEST(ParserHappyParseExprTests, CompGroupedTest) {
  std::vector<Error> errors;
  Lexer lexer("(myVar == (myVar2 + 5))", "file/directory", "CompGroupedTest", errors);
  lexer.tokenize();

  Parser parser(errors);
  auto value_node = parser.parse_expr(lexer);

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *binary_expr = value_node->binary_expr();
  ASSERT_EQ(binary_expr->bin_op, BinaryExprType::EQUALS);
  ASSERT_NE(binary_expr->left_expr, nullptr);
  ASSERT_EQ(binary_expr->left_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->left_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(binary_expr->left_expr->symbol()->token->id, TokenId::IDENTIFIER);
  ASSERT_NE(binary_expr->right_expr, nullptr);
  ASSERT_EQ(binary_expr->right_expr->parent, binary_expr);
  ASSERT_EQ(binary_expr->right_expr->node_type, AstNodeType::AST_BINARY_EXPR);

  const AstBinaryExpr *sum_node = binary_expr->right_expr->binary_expr();
  ASSERT_NE(sum_node->left_expr, nullptr);
  ASSERT_NE(sum_node->right_expr, nullptr);
  ASSERT_EQ(sum_node->left_expr->parent, sum_node);
  ASSERT_EQ(sum_node->right_expr->parent, sum_node);
  ASSERT_EQ(sum_node->left_expr->node_type, AstNodeType::AST_SYMBOL);
  ASSERT_EQ(sum_node->right_expr->node_type, AstNodeType::AST_CONST_VALUE);
  ASSERT_EQ(sum_node->left_expr->symbol()->token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(sum_node->right_expr->const_value()->type, ConstValueType::INT);

  delete value_node;
}

//==================================================================================
//          UTILS
//==================================================================================

TEST(ParserHappyUtilTests, MatchFuncTest) {
  Token token("");
  token.id = TokenId::FN;
  ASSERT_EQ(match(&token, TokenId::AND, TokenId::_EOF, TokenId::FN, TokenId(size_t(TokenId::_EOF) + 1)), true);
}

TEST(ParserHappyUtilTests, MatchMacroTest) {
  Token token("");
  token.id = TokenId::FN;
  ASSERT_EQ(MATCH(&token, TokenId::AND, TokenId::_EOF, TokenId::FN), true);
}

TEST(ParserHappyUtilTests, MatchMacroNoMatchTest) {
  Token token("");
  token.id = TokenId::FN;
  ASSERT_EQ(MATCH(&token, TokenId::AND, TokenId::_EOF, TokenId::OR), false);
}
