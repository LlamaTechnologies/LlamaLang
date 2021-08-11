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
  Lexer lexer("myVar", "IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_primary_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
  ASSERT_NE(value_node->symbol.token, nullptr);
  ASSERT_EQ(value_node->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->symbol.cached_name, "myVar");
}

TEST(ParserHappyParseValueTests, FloatTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547.12f", "FloatTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_primary_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->const_value.type, ConstValueType::FLOAT);
}

TEST(ParserHappyParseValueTests, IntTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547", "IntTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_primary_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->const_value.type, ConstValueType::INT);
  ASSERT_EQ(strcmp(value_node->const_value.number, "12547"), 0);
}

TEST(ParserHappyParseValueTests, UnicodeCharTest) {
  std::vector<Error> errors;
  Lexer lexer("\'g\'", "UnicodeCharTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_primary_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->const_value.type, ConstValueType::CHAR);
  ASSERT_EQ(value_node->const_value.unicode_char, 'g');
}

//==================================================================================
//          PARSE UNARY EXPRESSIONS FUNCTIONS
//==================================================================================

TEST(ParserHappyParseUnaryExprTests, PreIncIdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("++myVar", "PreIncIdentifierTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_unary_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstUnaryExpr);
  ASSERT_EQ(value_node->unary_expr.op, UnaryExprType::INC);
  ASSERT_EQ(value_node->unary_expr.expr->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->unary_expr.expr->parent, value_node);
}

TEST(ParserHappyParseUnaryExprTests, PostIncIdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar++", "PostIncIdentifierTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_unary_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstUnaryExpr);
  ASSERT_EQ(value_node->unary_expr.op, UnaryExprType::INC);
  ASSERT_EQ(value_node->unary_expr.expr->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->unary_expr.expr->parent, value_node);
}

TEST(ParserHappyParseUnaryExprTests, PreIncFuncCallTest) {
  std::vector<Error> errors;
  Lexer lexer("++myFunc()", "PreIncFuncCallTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_unary_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstUnaryExpr);
  ASSERT_EQ(value_node->unary_expr.op, UnaryExprType::INC);
  ASSERT_EQ(value_node->unary_expr.expr->parent, value_node);
  auto func_call = value_node->unary_expr.expr;
  ASSERT_NE(func_call, nullptr);
  ASSERT_EQ(func_call->parent, value_node);
  ASSERT_EQ(func_call->node_type, AstNodeType::AstFuncCallExpr);
  ASSERT_EQ(func_call->func_call.fn_name, "myFunc");
  ASSERT_EQ(func_call->func_call.params.size(), 0L);
}

TEST(ParserHappyParseUnaryExprTests, IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar", "IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_unary_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
  ASSERT_NE(value_node->symbol.token, nullptr);
  ASSERT_EQ(value_node->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->symbol.cached_name, "myVar");
}

TEST(ParserHappyParseUnaryExprTests, FloatTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547.12f", "FloatTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_unary_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->const_value.type, ConstValueType::FLOAT);
}

TEST(ParserHappyParseUnaryExprTests, IntTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547", "IntTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_unary_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->const_value.type, ConstValueType::INT);
  ASSERT_EQ(strcmp(value_node->const_value.number, "12547"), 0);
}

TEST(ParserHappyParseUnaryExprTests, NegativeIntTest) {
  std::vector<Error> errors;
  Lexer lexer("-12_547", "NegativeIntTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_unary_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstUnaryExpr);
  ASSERT_EQ(value_node->unary_expr.op, UnaryExprType::NEG);

  auto expr_node = value_node->unary_expr.expr;
  ASSERT_NE(expr_node, nullptr);
  ASSERT_EQ(expr_node->parent, value_node);
  ASSERT_EQ(expr_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(expr_node->const_value.type, ConstValueType::INT);
  ASSERT_NE(value_node->const_value.number, nullptr);
  ASSERT_EQ(strcmp(value_node->const_value.number, "-12547"), 0);
  ASSERT_EQ(value_node->const_value.is_negative, true);
}

TEST(ParserHappyParseUnaryExprTests, ExplicitPositiveIntTest) {
  std::vector<Error> errors;
  Lexer lexer("+12_547", "ExplicitPositiveIntTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_unary_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->const_value.type, ConstValueType::INT);
  ASSERT_EQ(strcmp(value_node->const_value.number, "12547"), 0);
}

TEST(ParserHappyParseUnaryExprTests, UnicodeCharTest) {
  std::vector<Error> errors;
  Lexer lexer("\'g\'", "UnicodeCharTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_unary_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->const_value.type, ConstValueType::CHAR);
  ASSERT_EQ(value_node->const_value.unicode_char, 'g');
}

//==================================================================================
//          PARSE PRODUCT EXPRESSIONS FUNCTIONS
//==================================================================================

TEST(ParserHappyParseMulExprTests, Mul2IdentifierAndDecTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar-- * myVar2", "Mul2IdentifierAndDecTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_term_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.right_expr->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstUnaryExpr);
  ASSERT_EQ(value_node->binary_expr.left_expr->unary_expr.op, UnaryExprType::DEC);
  ASSERT_EQ(value_node->binary_expr.left_expr->unary_expr.expr->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
}

TEST(ParserHappyParseMulExprTests, Mul2IdentifierAndFuncCallTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar-- * myFunc()", "Mul2IdentifierAndFuncCallTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_term_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstUnaryExpr);
  ASSERT_EQ(value_node->binary_expr.left_expr->unary_expr.op, UnaryExprType::DEC);
  ASSERT_EQ(value_node->binary_expr.left_expr->unary_expr.expr->symbol.token->id, TokenId::IDENTIFIER);

  auto func_call = value_node->binary_expr.right_expr;
  ASSERT_NE(func_call, nullptr);
  ASSERT_EQ(func_call->parent, value_node);
  ASSERT_EQ(func_call->node_type, AstNodeType::AstFuncCallExpr);
  ASSERT_EQ(func_call->func_call.fn_name, "myFunc");
  ASSERT_EQ(func_call->func_call.params.size(), 0L);
}

TEST(ParserHappyParseMulExprTests, Mul2IdentifierAndIncTest) {
  std::vector<Error> errors;
  Lexer lexer("++myVar * myVar2", "Mul2IdentifierAndIncTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_term_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.right_expr->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstUnaryExpr);
  ASSERT_EQ(value_node->binary_expr.left_expr->unary_expr.op, UnaryExprType::INC);
  ASSERT_EQ(value_node->binary_expr.left_expr->unary_expr.expr->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
}

TEST(ParserHappyParseMulExprTests, Mul2IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar * myVar2", "Mul2IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_term_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
  ASSERT_EQ(value_node->binary_expr.left_expr->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->binary_expr.right_expr->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
}

TEST(ParserHappyParseMulExprTests, MulNumberIdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("25 * myVar2", "MulNumberIdentifierTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_term_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
  ASSERT_NE(value_node->binary_expr.left_expr, nullptr);
  ASSERT_NE(value_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(value_node->binary_expr.left_expr->const_value.type, ConstValueType::INT);
  ASSERT_EQ(value_node->binary_expr.right_expr->symbol.token->id, TokenId::IDENTIFIER);
}

TEST(ParserHappyParseMulExprTests, Mul2NumbersTest) {
  std::vector<Error> errors;
  Lexer lexer("25 * 21.5f", "Mul2NumbersTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_term_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
  ASSERT_NE(value_node->binary_expr.left_expr, nullptr);
  ASSERT_NE(value_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.left_expr->const_value.type, ConstValueType::INT);
  ASSERT_EQ(value_node->binary_expr.right_expr->const_value.type, ConstValueType::FLOAT);
}

TEST(ParserHappyParseMulExprTests, Mul2NumbersAndCharTest) {
  std::vector<Error> errors;
  Lexer lexer("25 * 21.5f / 'g'", "Mul2NumbersAndCharTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_term_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::DIV);
  ASSERT_NE(value_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.right_expr->const_value.type, ConstValueType::CHAR);

  auto bin_exp_op1 = value_node->binary_expr.left_expr;
  ASSERT_NE(bin_exp_op1, nullptr);
  ASSERT_EQ(bin_exp_op1->parent, value_node);
  ASSERT_EQ(bin_exp_op1->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(bin_exp_op1->binary_expr.bin_op, BinaryExprType::MUL);

  auto bin_exp2_op1 = bin_exp_op1->binary_expr.left_expr;
  ASSERT_NE(bin_exp2_op1, nullptr);
  ASSERT_EQ(bin_exp2_op1->parent, bin_exp_op1);
  ASSERT_EQ(bin_exp2_op1->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(bin_exp2_op1->const_value.type, ConstValueType::INT);

  auto bin_exp2_op2 = bin_exp_op1->binary_expr.right_expr;
  ASSERT_NE(bin_exp2_op2, nullptr);
  ASSERT_EQ(bin_exp2_op2->parent, bin_exp_op1);
  ASSERT_EQ(bin_exp2_op2->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(bin_exp2_op2->const_value.type, ConstValueType::FLOAT);
}

TEST(ParserHappyParseMulExprTests, IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar", "IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_term_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
  ASSERT_NE(value_node->symbol.token, nullptr);
  ASSERT_EQ(value_node->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->symbol.cached_name, "myVar");
}

TEST(ParserHappyParseMulExprTests, FloatTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547.12f", "FloatTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_term_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->const_value.type, ConstValueType::FLOAT);
}

TEST(ParserHappyParseMulExprTests, IntTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547", "IntTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_term_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->const_value.type, ConstValueType::INT);
  ASSERT_EQ(strcmp(value_node->const_value.number, "12547"), 0);
}

TEST(ParserHappyParseMulExprTests, UnicodeCharTest) {
  std::vector<Error> errors;
  Lexer lexer("\'g\'", "UnicodeCharTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_term_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->const_value.type, ConstValueType::CHAR);
  ASSERT_EQ(value_node->const_value.unicode_char, 'g');
}

//==================================================================================
//          PARSE ADDITION EXPRESSIONS FUNCTIONS
//==================================================================================
TEST(ParserHappyParseAddExprTests, Add2IdentifierAndIncTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar++ + myVar2", "Add2IdentifierAndIncTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_algebraic_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ADD);

  auto bin_exp_left = value_node->binary_expr.left_expr;
  ASSERT_NE(bin_exp_left, nullptr);
  ASSERT_EQ(bin_exp_left->parent, value_node);
  ASSERT_EQ(bin_exp_left->node_type, AstNodeType::AstUnaryExpr);
  ASSERT_EQ(bin_exp_left->unary_expr.op, UnaryExprType::INC);

  auto symbol_node = bin_exp_left->unary_expr.expr;
  ASSERT_NE(symbol_node, nullptr);
  ASSERT_EQ(symbol_node->parent, bin_exp_left);
  ASSERT_EQ(symbol_node->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(symbol_node->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(symbol_node->symbol.cached_name, "myVar");

  auto bin_exp_right = value_node->binary_expr.right_expr;
  ASSERT_NE(bin_exp_right, nullptr);
  ASSERT_EQ(bin_exp_right->parent, value_node);
  ASSERT_EQ(bin_exp_right->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(bin_exp_right->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(bin_exp_right->symbol.cached_name, "myVar2");
}

TEST(ParserHappyParseAddExprTests, Add2IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar + myVar2", "Add2IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_algebraic_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ADD);
  ASSERT_NE(value_node->binary_expr.left_expr, nullptr);
  ASSERT_NE(value_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(value_node->binary_expr.left_expr->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->binary_expr.right_expr->symbol.token->id, TokenId::IDENTIFIER);
}

TEST(ParserHappyParseAddExprTests, AddIdentifierNumberTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar + 25", "AddIdentifierNumberTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_algebraic_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ADD);

  ASSERT_NE(value_node->binary_expr.left_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(value_node->binary_expr.left_expr->symbol.token->id, TokenId::IDENTIFIER);

  ASSERT_NE(value_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.right_expr->const_value.type, ConstValueType::INT);
}

TEST(ParserHappyParseAddExprTests, Add2NumbersTest) {
  std::vector<Error> errors;
  Lexer lexer(".25f + 25", "Add2NumbersTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_algebraic_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ADD);
  ASSERT_NE(value_node->binary_expr.left_expr, nullptr);
  ASSERT_NE(value_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.left_expr->const_value.type, ConstValueType::FLOAT);
  ASSERT_EQ(value_node->binary_expr.right_expr->const_value.type, ConstValueType::INT);
}

TEST(ParserHappyParseAddExprTests, Add2NumberAndCharTest) {
  std::vector<Error> errors;
  Lexer lexer(".25f + 25 - 'g'", "Add2NumberAndCharTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_algebraic_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::SUB);

  auto bin_op1 = value_node->binary_expr.left_expr;
  ASSERT_NE(bin_op1, nullptr);
  ASSERT_EQ(bin_op1->parent, value_node);
  ASSERT_EQ(bin_op1->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(bin_op1->binary_expr.bin_op, BinaryExprType::ADD);
  ASSERT_NE(bin_op1->binary_expr.left_expr, nullptr);
  ASSERT_NE(bin_op1->binary_expr.right_expr, nullptr);
  ASSERT_EQ(bin_op1->binary_expr.left_expr->parent, bin_op1);
  ASSERT_EQ(bin_op1->binary_expr.right_expr->parent, bin_op1);
  ASSERT_EQ(bin_op1->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(bin_op1->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(bin_op1->binary_expr.left_expr->const_value.type, ConstValueType::FLOAT);
  ASSERT_EQ(bin_op1->binary_expr.right_expr->const_value.type, ConstValueType::INT);

  auto bin_op2 = value_node->binary_expr.right_expr;
  ASSERT_NE(bin_op2, nullptr);
  ASSERT_EQ(bin_op2->parent, value_node);
  ASSERT_EQ(bin_op2->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(bin_op2->const_value.type, ConstValueType::CHAR);
}

TEST(ParserHappyParseAddExprTests, FullAlgebraicTest) {
  std::vector<Error> errors;
  Lexer lexer(".25f + 25 * 14 - 'g' % 15", "FullAlgebraicTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_algebraic_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::SUB);

  const auto gMod15 = value_node->binary_expr.right_expr;
  ASSERT_NE(gMod15, nullptr);
  ASSERT_EQ(gMod15->parent, value_node);
  ASSERT_EQ(gMod15->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(gMod15->binary_expr.bin_op, BinaryExprType::MOD);
  ASSERT_EQ(gMod15->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(gMod15->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(gMod15->binary_expr.right_expr->parent, gMod15);
  ASSERT_EQ(gMod15->binary_expr.left_expr->parent, gMod15);

  const auto floatPlusMul = value_node->binary_expr.left_expr;
  ASSERT_NE(floatPlusMul, nullptr);
  ASSERT_EQ(floatPlusMul->parent, value_node);
  ASSERT_EQ(floatPlusMul->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(floatPlusMul->binary_expr.bin_op, BinaryExprType::ADD);
  ASSERT_NE(floatPlusMul->binary_expr.left_expr, nullptr);
  ASSERT_EQ(floatPlusMul->binary_expr.left_expr->parent, floatPlusMul);
  ASSERT_EQ(floatPlusMul->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);

  const auto intMulInt = floatPlusMul->binary_expr.right_expr;
  ASSERT_NE(intMulInt, nullptr);
  ASSERT_EQ(intMulInt->parent, floatPlusMul);
  ASSERT_EQ(intMulInt->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(intMulInt->binary_expr.bin_op, BinaryExprType::MUL);
  ASSERT_NE(intMulInt->binary_expr.left_expr, nullptr);
  ASSERT_NE(intMulInt->binary_expr.right_expr, nullptr);
  ASSERT_EQ(intMulInt->binary_expr.left_expr->parent, intMulInt);
  ASSERT_EQ(intMulInt->binary_expr.right_expr->parent, intMulInt);
  ASSERT_EQ(intMulInt->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(intMulInt->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
}

TEST(ParserHappyParseAddExprTests, Mul2IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar * myVar2", "Mul2IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_algebraic_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
  ASSERT_NE(value_node->binary_expr.left_expr, nullptr);
  ASSERT_NE(value_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(value_node->binary_expr.left_expr->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->binary_expr.right_expr->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
}

TEST(ParserHappyParseAddExprTests, MulNumberIdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("25 * myVar2", "MulNumberIdentifierTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_algebraic_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);

  ASSERT_NE(value_node->binary_expr.left_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.left_expr->const_value.type, ConstValueType::INT);

  ASSERT_NE(value_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(value_node->binary_expr.right_expr->symbol.token->id, TokenId::IDENTIFIER);
}

TEST(ParserHappyParseAddExprTests, Mul2NumbersTest) {
  std::vector<Error> errors;
  Lexer lexer("25 * 21.5f", "Mul2NumbersTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_algebraic_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
  ASSERT_NE(value_node->binary_expr.left_expr, nullptr);
  ASSERT_NE(value_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.left_expr->const_value.type, ConstValueType::INT);
  ASSERT_EQ(value_node->binary_expr.right_expr->const_value.type, ConstValueType::FLOAT);
}

TEST(ParserHappyParseAddExprTests, Mul2NumbersAndCharTest) {
  std::vector<Error> errors;
  Lexer lexer("25 * 21.5f / 'g'", "Mul2NumbersAndCharTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_algebraic_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::DIV);
  ASSERT_NE(value_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.right_expr->const_value.type, ConstValueType::CHAR);

  auto bin_exp_op1 = value_node->binary_expr.left_expr;
  ASSERT_NE(bin_exp_op1, nullptr);
  ASSERT_EQ(bin_exp_op1->parent, value_node);
  ASSERT_EQ(bin_exp_op1->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(bin_exp_op1->binary_expr.bin_op, BinaryExprType::MUL);

  auto bin_exp2_op1 = bin_exp_op1->binary_expr.left_expr;
  ASSERT_NE(bin_exp2_op1, nullptr);
  ASSERT_EQ(bin_exp2_op1->parent, bin_exp_op1);
  ASSERT_EQ(bin_exp2_op1->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(bin_exp2_op1->const_value.type, ConstValueType::INT);

  auto bin_exp2_op2 = bin_exp_op1->binary_expr.right_expr;
  ASSERT_NE(bin_exp2_op2, nullptr);
  ASSERT_EQ(bin_exp2_op2->parent, bin_exp_op1);
  ASSERT_EQ(bin_exp2_op2->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(bin_exp2_op2->const_value.type, ConstValueType::FLOAT);
}

TEST(ParserHappyParseAddExprTests, IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar", "IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_algebraic_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
  ASSERT_NE(value_node->symbol.token, nullptr);
  ASSERT_EQ(value_node->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->symbol.cached_name, "myVar");
}

TEST(ParserHappyParseAddExprTests, FloatTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547.12f", "FloatTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_algebraic_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->const_value.type, ConstValueType::FLOAT);
}

TEST(ParserHappyParseAddExprTests, IntTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547", "IntTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_algebraic_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->const_value.type, ConstValueType::INT);
  ASSERT_EQ(strcmp(value_node->const_value.number, "12547"), 0);
}

TEST(ParserHappyParseAddExprTests, UnicodeCharTest) {
  std::vector<Error> errors;
  Lexer lexer("\'g\'", "UnicodeCharTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_algebraic_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->const_value.type, ConstValueType::CHAR);
  ASSERT_EQ(value_node->const_value.unicode_char, 'g');
}

//==================================================================================
//          PARSE COMPARATIVE EXPRESSIONS FUNCTIONS
//==================================================================================

TEST(ParserHappyParseCompExprTests, Comp2IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar == myVar2", "Add2IdentifierAndIncTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_comp_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::EQUALS);
  ASSERT_NE(value_node->binary_expr.left_expr, nullptr);
  ASSERT_NE(value_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(value_node->binary_expr.left_expr->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->binary_expr.right_expr->symbol.token->id, TokenId::IDENTIFIER);
}

TEST(ParserHappyParseCompExprTests, Comp2IdentifierAndNumberTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar == myVar2 >= 45", "Add2IdentifierAndIncTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_comp_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::GREATER_OR_EQUALS);
  ASSERT_NE(value_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.right_expr->const_value.type, ConstValueType::INT);

  const auto identComp = value_node->binary_expr.left_expr;
  ASSERT_NE(identComp, nullptr);
  ASSERT_EQ(identComp->parent, value_node);
  ASSERT_EQ(identComp->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(identComp->binary_expr.bin_op, BinaryExprType::EQUALS);
  ASSERT_NE(identComp->binary_expr.left_expr, nullptr);
  ASSERT_NE(identComp->binary_expr.right_expr, nullptr);
  ASSERT_EQ(identComp->binary_expr.left_expr->parent, identComp);
  ASSERT_EQ(identComp->binary_expr.right_expr->parent, identComp);
  ASSERT_EQ(identComp->binary_expr.left_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(identComp->binary_expr.right_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(identComp->binary_expr.left_expr->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(identComp->binary_expr.right_expr->symbol.token->id, TokenId::IDENTIFIER);
}

TEST(ParserHappyParseCompExprTests, FullAlgebraicCompareTest) {
  std::vector<Error> errors;
  Lexer lexer(".25f + 25 * 14 - 'g' % 15 == 3", "FullAlgebraicCompareTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_comp_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::EQUALS);
  ASSERT_NE(value_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.right_expr->const_value.type, ConstValueType::INT);

  const auto algebraicExpr = value_node->binary_expr.left_expr;
  ASSERT_NE(algebraicExpr, nullptr);
  ASSERT_EQ(algebraicExpr->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(algebraicExpr->binary_expr.bin_op, BinaryExprType::SUB);

  const auto gMod15 = algebraicExpr->binary_expr.right_expr;
  ASSERT_NE(gMod15, nullptr);
  ASSERT_EQ(gMod15->parent, algebraicExpr);
  ASSERT_EQ(gMod15->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(gMod15->binary_expr.bin_op, BinaryExprType::MOD);
  ASSERT_EQ(gMod15->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(gMod15->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(gMod15->binary_expr.right_expr->parent, gMod15);
  ASSERT_EQ(gMod15->binary_expr.left_expr->parent, gMod15);

  const auto floatPlusMul = algebraicExpr->binary_expr.left_expr;
  ASSERT_NE(floatPlusMul, nullptr);
  ASSERT_EQ(floatPlusMul->parent, algebraicExpr);
  ASSERT_EQ(floatPlusMul->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(floatPlusMul->binary_expr.bin_op, BinaryExprType::ADD);
  ASSERT_NE(floatPlusMul->binary_expr.left_expr, nullptr);
  ASSERT_EQ(floatPlusMul->binary_expr.left_expr->parent, floatPlusMul);
  ASSERT_EQ(floatPlusMul->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);

  const auto intMulInt = floatPlusMul->binary_expr.right_expr;
  ASSERT_NE(intMulInt, nullptr);
  ASSERT_EQ(intMulInt->parent, floatPlusMul);
  ASSERT_EQ(intMulInt->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(intMulInt->binary_expr.bin_op, BinaryExprType::MUL);
  ASSERT_NE(intMulInt->binary_expr.left_expr, nullptr);
  ASSERT_NE(intMulInt->binary_expr.right_expr, nullptr);
  ASSERT_EQ(intMulInt->binary_expr.left_expr->parent, intMulInt);
  ASSERT_EQ(intMulInt->binary_expr.right_expr->parent, intMulInt);
  ASSERT_EQ(intMulInt->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(intMulInt->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
}

TEST(ParserHappyParseCompExprTests, Add2IdentifierAndIncTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar++ + myVar2", "Add2IdentifierAndIncTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_comp_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ADD);
  ASSERT_NE(value_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(value_node->binary_expr.right_expr->symbol.token->id, TokenId::IDENTIFIER);
  auto bin_exp_op1 = value_node->binary_expr.left_expr;
  ASSERT_EQ(bin_exp_op1->parent, value_node);
  ASSERT_EQ(bin_exp_op1->node_type, AstNodeType::AstUnaryExpr);
  ASSERT_EQ(bin_exp_op1->unary_expr.op, UnaryExprType::INC);
  ASSERT_NE(bin_exp_op1->unary_expr.expr, nullptr);
  ASSERT_EQ(bin_exp_op1->unary_expr.expr->parent, bin_exp_op1);
  ASSERT_EQ(bin_exp_op1->unary_expr.expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(bin_exp_op1->unary_expr.expr->symbol.token->id, TokenId::IDENTIFIER);
}

TEST(ParserHappyParseCompExprTests, Add2IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar + myVar2", "Add2IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_comp_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ADD);
  ASSERT_NE(value_node->binary_expr.left_expr, nullptr);
  ASSERT_NE(value_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(value_node->binary_expr.left_expr->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->binary_expr.right_expr->symbol.token->id, TokenId::IDENTIFIER);
}

TEST(ParserHappyParseCompExprTests, AddIdentifierNumberTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar + 25", "AddIdentifierNumberTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_comp_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ADD);

  ASSERT_NE(value_node->binary_expr.left_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(value_node->binary_expr.left_expr->symbol.token->id, TokenId::IDENTIFIER);

  ASSERT_NE(value_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.right_expr->const_value.type, ConstValueType::INT);
}

TEST(ParserHappyParseCompExprTests, Add2NumbersTest) {
  std::vector<Error> errors;
  Lexer lexer(".25f + 25", "Add2NumbersTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_comp_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ADD);

  ASSERT_NE(value_node->binary_expr.left_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.left_expr->const_value.type, ConstValueType::FLOAT);

  ASSERT_NE(value_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.right_expr->const_value.type, ConstValueType::INT);
}

TEST(ParserHappyParseCompExprTests, Add2NumberAndCharTest) {
  std::vector<Error> errors;
  Lexer lexer(".25f + 25 - 'g'", "Add2NumberAndCharTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_comp_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::SUB);

  auto bin_op1 = value_node->binary_expr.left_expr;
  ASSERT_NE(bin_op1, nullptr);
  ASSERT_EQ(bin_op1->parent, value_node);
  ASSERT_EQ(bin_op1->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(bin_op1->binary_expr.bin_op, BinaryExprType::ADD);
  ASSERT_NE(bin_op1->binary_expr.left_expr, nullptr);
  ASSERT_NE(bin_op1->binary_expr.right_expr, nullptr);
  ASSERT_EQ(bin_op1->binary_expr.left_expr->parent, bin_op1);
  ASSERT_EQ(bin_op1->binary_expr.right_expr->parent, bin_op1);
  ASSERT_EQ(bin_op1->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(bin_op1->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(bin_op1->binary_expr.left_expr->const_value.type, ConstValueType::FLOAT);
  ASSERT_EQ(bin_op1->binary_expr.right_expr->const_value.type, ConstValueType::INT);

  auto bin_op2 = value_node->binary_expr.right_expr;
  ASSERT_NE(bin_op2, nullptr);
  ASSERT_EQ(bin_op2->parent, value_node);
  ASSERT_EQ(bin_op2->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(bin_op2->const_value.type, ConstValueType::CHAR);
}

TEST(ParserHappyParseCompExprTests, FullAlgebraicTest) {
  std::vector<Error> errors;
  Lexer lexer(".25f + 25 * 14 - 'g' % 15", "FullAlgebraicTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_comp_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::SUB);

  const auto gMod15 = value_node->binary_expr.right_expr;
  ASSERT_NE(gMod15, nullptr);
  ASSERT_EQ(gMod15->parent, value_node);
  ASSERT_EQ(gMod15->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(gMod15->binary_expr.bin_op, BinaryExprType::MOD);
  ASSERT_EQ(gMod15->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(gMod15->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(gMod15->binary_expr.right_expr->parent, gMod15);
  ASSERT_EQ(gMod15->binary_expr.left_expr->parent, gMod15);

  const auto floatPlusMul = value_node->binary_expr.left_expr;
  ASSERT_NE(floatPlusMul, nullptr);
  ASSERT_EQ(floatPlusMul->parent, value_node);
  ASSERT_EQ(floatPlusMul->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(floatPlusMul->binary_expr.bin_op, BinaryExprType::ADD);
  ASSERT_NE(floatPlusMul->binary_expr.left_expr, nullptr);
  ASSERT_EQ(floatPlusMul->binary_expr.left_expr->parent, floatPlusMul);
  ASSERT_EQ(floatPlusMul->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);

  const auto intMulInt = floatPlusMul->binary_expr.right_expr;
  ASSERT_NE(intMulInt, nullptr);
  ASSERT_EQ(intMulInt->parent, floatPlusMul);
  ASSERT_EQ(intMulInt->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(intMulInt->binary_expr.bin_op, BinaryExprType::MUL);
  ASSERT_NE(intMulInt->binary_expr.left_expr, nullptr);
  ASSERT_NE(intMulInt->binary_expr.right_expr, nullptr);
  ASSERT_EQ(intMulInt->binary_expr.left_expr->parent, intMulInt);
  ASSERT_EQ(intMulInt->binary_expr.right_expr->parent, intMulInt);
  ASSERT_EQ(intMulInt->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(intMulInt->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
}

TEST(ParserHappyParseCompExprTests, Mul2IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar * myVar2", "Mul2IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_comp_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
  ASSERT_EQ(value_node->binary_expr.left_expr->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->binary_expr.right_expr->symbol.token->id, TokenId::IDENTIFIER);
}

TEST(ParserHappyParseCompExprTests, MulNumberIdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("25 * myVar2", "MulNumberIdentifierTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_comp_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(value_node->binary_expr.left_expr->const_value.type, ConstValueType::INT);
  ASSERT_EQ(value_node->binary_expr.right_expr->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
}

TEST(ParserHappyParseCompExprTests, Mul2NumbersTest) {
  std::vector<Error> errors;
  Lexer lexer("25 * 21.5f", "Mul2NumbersTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_comp_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
  ASSERT_NE(value_node->binary_expr.left_expr, nullptr);
  ASSERT_NE(value_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.left_expr->const_value.type, ConstValueType::INT);
  ASSERT_EQ(value_node->binary_expr.right_expr->const_value.type, ConstValueType::FLOAT);
}

TEST(ParserHappyParseCompExprTests, Mul2NumbersAndCharTest) {
  std::vector<Error> errors;
  Lexer lexer("25 * 21.5f / 'g'", "Mul2NumbersAndCharTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_comp_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::DIV);
  ASSERT_NE(value_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.right_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->binary_expr.right_expr->const_value.type, ConstValueType::CHAR);

  auto bin_exp_op1 = value_node->binary_expr.left_expr;
  ASSERT_NE(bin_exp_op1, nullptr);
  ASSERT_EQ(bin_exp_op1->parent, value_node);
  ASSERT_EQ(bin_exp_op1->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(bin_exp_op1->binary_expr.bin_op, BinaryExprType::MUL);

  auto bin_exp2_op1 = bin_exp_op1->binary_expr.left_expr;
  ASSERT_NE(bin_exp2_op1, nullptr);
  ASSERT_EQ(bin_exp2_op1->parent, bin_exp_op1);
  ASSERT_EQ(bin_exp2_op1->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(bin_exp2_op1->const_value.type, ConstValueType::INT);

  auto bin_exp2_op2 = bin_exp_op1->binary_expr.right_expr;
  ASSERT_NE(bin_exp2_op2, nullptr);
  ASSERT_EQ(bin_exp2_op2->parent, bin_exp_op1);
  ASSERT_EQ(bin_exp2_op2->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(bin_exp2_op2->const_value.type, ConstValueType::FLOAT);
}

TEST(ParserHappyParseCompExprTests, IdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("myVar", "IdentifierTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_comp_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
  ASSERT_NE(value_node->symbol.token, nullptr);
  ASSERT_EQ(value_node->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(value_node->symbol.cached_name, "myVar");
}

TEST(ParserHappyParseCompExprTests, FloatTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547.12f", "FloatTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_comp_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->const_value.type, ConstValueType::FLOAT);
}

TEST(ParserHappyParseCompExprTests, IntTest) {
  std::vector<Error> errors;
  Lexer lexer("12_547", "IntTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_comp_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->const_value.type, ConstValueType::INT);
  ASSERT_EQ(strcmp(value_node->const_value.number, "12547"), 0);
}

TEST(ParserHappyParseCompExprTests, UnicodeCharTest) {
  std::vector<Error> errors;
  Lexer lexer("\'g\'", "UnicodeCharTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_comp_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(value_node->const_value.type, ConstValueType::CHAR);
  ASSERT_EQ(value_node->const_value.unicode_char, 'g');
}

//==================================================================================
//          PARSE EXPRESSIONS FUNCTIONS
//==================================================================================

TEST(ParserHappyParseExprTests, CompGroupedTest) {
  std::vector<Error> errors;
  Lexer lexer("(myVar == (myVar2 + 5))", "CompGroupedTest", errors);
  lexer.tokenize();

  Parser parser(lexer, errors);
  auto value_node = parser.parse_expr();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_NE(value_node, nullptr);
  ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::EQUALS);
  ASSERT_NE(value_node->binary_expr.left_expr, nullptr);
  ASSERT_EQ(value_node->binary_expr.left_expr->parent, value_node);
  ASSERT_EQ(value_node->binary_expr.left_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(value_node->binary_expr.left_expr->symbol.token->id, TokenId::IDENTIFIER);

  const auto sum_node = value_node->binary_expr.right_expr;
  ASSERT_NE(sum_node, nullptr);
  ASSERT_EQ(sum_node->parent, value_node);
  ASSERT_EQ(sum_node->node_type, AstNodeType::AstBinaryExpr);
  ASSERT_NE(sum_node->binary_expr.left_expr, nullptr);
  ASSERT_NE(sum_node->binary_expr.right_expr, nullptr);
  ASSERT_EQ(sum_node->binary_expr.left_expr->parent, sum_node);
  ASSERT_EQ(sum_node->binary_expr.right_expr->parent, sum_node);
  ASSERT_EQ(sum_node->binary_expr.left_expr->node_type, AstNodeType::AstSymbol);
  ASSERT_EQ(sum_node->binary_expr.right_expr->node_type, AstNodeType::AstConstValue);
  ASSERT_EQ(sum_node->binary_expr.left_expr->symbol.token->id, TokenId::IDENTIFIER);
  ASSERT_EQ(sum_node->binary_expr.right_expr->const_value.type, ConstValueType::INT);
}

//==================================================================================
//          UTILS
//==================================================================================

TEST(ParserHappyUtilTests, MatchFuncTest) {
  Token token;
  token.id = TokenId::FN;
  ASSERT_EQ(match(&token, TokenId::AND, TokenId::_EOF, TokenId::FN, TokenId(size_t(TokenId::_EOF) + 1)), true);
}

TEST(ParserHappyUtilTests, MatchMacroTest) {
  Token token;
  token.id = TokenId::FN;
  ASSERT_EQ(MATCH(&token, TokenId::AND, TokenId::_EOF, TokenId::FN), true);
}

TEST(ParserHappyUtilTests, MatchMacroNoMatchTest) {
  Token token;
  token.id = TokenId::FN;
  ASSERT_EQ(MATCH(&token, TokenId::AND, TokenId::_EOF, TokenId::OR), false);
}
