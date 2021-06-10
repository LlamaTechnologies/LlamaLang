#include <gtest/gtest.h>
#include "../../src/ast_nodes.hpp"
#include "../../src/lexer.hpp"
#include "../../src/parser.hpp"

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
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->symbol.token, nullptr);
    ASSERT_EQ(value_node->symbol.token->id, TokenId::IDENTIFIER);
}

TEST(ParserHappyParseValueTests, FloatTest) {
    std::vector<Error> errors;
    Lexer lexer("12_547.12f", "FloatTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_primary_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_NE(value_node, nullptr);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->symbol.token, nullptr);
    ASSERT_EQ(value_node->symbol.token->id, TokenId::FLOAT_LIT);
}

TEST(ParserHappyParseValueTests, IntTest) {
    std::vector<Error> errors;
    Lexer lexer("12_547", "IntTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_primary_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->symbol.token, nullptr);
    ASSERT_EQ(value_node->symbol.token->id, TokenId::INT_LIT);
}

TEST(ParserHappyParseValueTests, UnicodeCharTest) {
    std::vector<Error> errors;
    Lexer lexer("\'g\'", "UnicodeCharTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_primary_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->symbol.token, nullptr);
    ASSERT_EQ(value_node->symbol.token->id, TokenId::UNICODE_CHAR);
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
}

TEST(ParserHappyParseUnaryExprTests, FloatTest) {
    std::vector<Error> errors;
    Lexer lexer("12_547.12f", "FloatTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_unary_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->symbol.token, nullptr);
    ASSERT_EQ(value_node->symbol.token->id, TokenId::FLOAT_LIT);
}

TEST(ParserHappyParseUnaryExprTests, IntTest) {
    std::vector<Error> errors;
    Lexer lexer("12_547", "IntTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_unary_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->symbol.token, nullptr);
    ASSERT_EQ(value_node->symbol.token->id, TokenId::INT_LIT);
}

TEST(ParserHappyParseUnaryExprTests, UnicodeCharTest) {
    std::vector<Error> errors;
    Lexer lexer("\'g\'", "UnicodeCharTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_unary_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->symbol.token, nullptr);
    ASSERT_EQ(value_node->symbol.token->id, TokenId::UNICODE_CHAR);
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
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
    ASSERT_EQ(value_node->binary_expr.op1->node_type, AstNodeType::AstUnaryExpr);
    ASSERT_EQ(value_node->binary_expr.op1->unary_expr.op, UnaryExprType::DEC);
    ASSERT_EQ(value_node->binary_expr.op1->unary_expr.expr->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);
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
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op1->node_type, AstNodeType::AstUnaryExpr);
    ASSERT_EQ(value_node->binary_expr.op1->unary_expr.op, UnaryExprType::DEC);
    ASSERT_EQ(value_node->binary_expr.op1->unary_expr.expr->symbol.token->id, TokenId::IDENTIFIER);

    auto func_call = value_node->binary_expr.op2;
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
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
    ASSERT_EQ(value_node->binary_expr.op1->node_type, AstNodeType::AstUnaryExpr);
    ASSERT_EQ(value_node->binary_expr.op1->unary_expr.op, UnaryExprType::INC);
    ASSERT_EQ(value_node->binary_expr.op1->unary_expr.expr->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);

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
    ASSERT_EQ(value_node->binary_expr.op1->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);
}

TEST(ParserHappyParseMulExprTests, MulNumberIdentifierTest) {
    std::vector<Error> errors;
    Lexer lexer("25 * myVar2", "MulNumberIdentifierTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_term_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
    ASSERT_EQ(value_node->binary_expr.op1->symbol.token->id, TokenId::INT_LIT);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);
}

TEST(ParserHappyParseMulExprTests, Mul2NumbersTest) {
    std::vector<Error> errors;
    Lexer lexer("25 * 21.5f", "Mul2NumbersTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_term_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
    ASSERT_EQ(value_node->binary_expr.op1->symbol.token->id, TokenId::INT_LIT);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::FLOAT_LIT);
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);
}

TEST(ParserHappyParseMulExprTests, Mul2NumbersAndCharTest) {
    std::vector<Error> errors;
    Lexer lexer("25 * 21.5f / 'g'", "Mul2NumbersAndCharTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_term_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::DIV);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::UNICODE_CHAR);
    ASSERT_EQ(value_node->binary_expr.op1->node_type, AstNodeType::AstBinaryExpr);
    auto bin_exp_op1 = value_node->binary_expr.op1;
    ASSERT_EQ(bin_exp_op1->parent, value_node);
    ASSERT_EQ(bin_exp_op1->binary_expr.bin_op, BinaryExprType::MUL);
    ASSERT_EQ(bin_exp_op1->binary_expr.op1->symbol.token->id, TokenId::INT_LIT);
    ASSERT_EQ(bin_exp_op1->binary_expr.op2->symbol.token->id, TokenId::FLOAT_LIT);
    auto bin_exp2_op1 = bin_exp_op1->binary_expr.op1;
    auto bin_exp2_op2 = bin_exp_op1->binary_expr.op2;
    ASSERT_EQ(bin_exp2_op1->parent, bin_exp_op1);
    ASSERT_EQ(bin_exp2_op2->parent, bin_exp_op1);
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
}

TEST(ParserHappyParseMulExprTests, FloatTest) {
    std::vector<Error> errors;
    Lexer lexer("12_547.12f", "FloatTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_term_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->symbol.token, nullptr);
    ASSERT_EQ(value_node->symbol.token->id, TokenId::FLOAT_LIT);
}

TEST(ParserHappyParseMulExprTests, IntTest) {
    std::vector<Error> errors;
    Lexer lexer("12_547", "IntTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_term_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->symbol.token, nullptr);
    ASSERT_EQ(value_node->symbol.token->id, TokenId::INT_LIT);
}

TEST(ParserHappyParseMulExprTests, UnicodeCharTest) {
    std::vector<Error> errors;
    Lexer lexer("\'g\'", "UnicodeCharTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_term_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->symbol.token, nullptr);
    ASSERT_EQ(value_node->symbol.token->id, TokenId::UNICODE_CHAR);
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
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ADD);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);
    auto bin_exp_op1 = value_node->binary_expr.op1;
    ASSERT_EQ(bin_exp_op1->parent, value_node);
    ASSERT_EQ(bin_exp_op1->node_type, AstNodeType::AstUnaryExpr);
    ASSERT_EQ(bin_exp_op1->unary_expr.op, UnaryExprType::INC);
    ASSERT_EQ(bin_exp_op1->unary_expr.expr->parent, bin_exp_op1);
    ASSERT_EQ(bin_exp_op1->unary_expr.expr->symbol.token->id, TokenId::IDENTIFIER);
}

TEST(ParserHappyParseAddExprTests, Add2IdentifierTest) {
    std::vector<Error> errors;
    Lexer lexer("myVar + myVar2", "Add2IdentifierTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_algebraic_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ADD);
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op1->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::IDENTIFIER);
}

TEST(ParserHappyParseAddExprTests, AddIdentifierNumberTest) {
    std::vector<Error> errors;
    Lexer lexer("myVar + 25", "AddIdentifierNumberTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_algebraic_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ADD);
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op1->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::INT_LIT);
}

TEST(ParserHappyParseAddExprTests, Add2NumbersTest) {
    std::vector<Error> errors;
    Lexer lexer(".25f + 25", "Add2NumbersTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_algebraic_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ADD);
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op1->symbol.token->id, TokenId::FLOAT_LIT);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::INT_LIT);
}

TEST(ParserHappyParseAddExprTests, Add2NumberAndCharTest) {
    std::vector<Error> errors;
    Lexer lexer(".25f + 25 - 'g'", "Add2NumberAndCharTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_algebraic_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::SUB);
    auto bin_op1 = value_node->binary_expr.op1;
    auto bin_op2 = value_node->binary_expr.op2;
    ASSERT_EQ(bin_op2->symbol.token->id, TokenId::UNICODE_CHAR);
    ASSERT_EQ(bin_op2->parent, value_node);
    ASSERT_EQ(bin_op1->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(bin_op1->parent, value_node);
    ASSERT_EQ(bin_op1->binary_expr.bin_op, BinaryExprType::ADD);
    ASSERT_EQ(bin_op1->binary_expr.op1->symbol.token->id, TokenId::FLOAT_LIT);
    ASSERT_EQ(bin_op1->binary_expr.op2->symbol.token->id, TokenId::INT_LIT);
    ASSERT_EQ(bin_op1->binary_expr.op1->parent, bin_op1);
    ASSERT_EQ(bin_op1->binary_expr.op2->parent, bin_op1);
}

TEST(ParserHappyParseAddExprTests, FullAlgebraicTest) {
    std::vector<Error> errors;
    Lexer lexer(".25f + 25 * 14 - 'g' % 15", "FullAlgebraicTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_algebraic_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::SUB);
    ASSERT_EQ(value_node->binary_expr.op1->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.op2->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);

    const auto gMod15 = value_node->binary_expr.op2; 
    ASSERT_EQ(gMod15->binary_expr.bin_op, BinaryExprType::MOD);
    ASSERT_EQ(gMod15->binary_expr.op1->symbol.token->id, TokenId::UNICODE_CHAR);
    ASSERT_EQ(gMod15->binary_expr.op2->symbol.token->id, TokenId::INT_LIT);
    ASSERT_EQ(gMod15->binary_expr.op1->parent, gMod15);
    ASSERT_EQ(gMod15->binary_expr.op2->parent, gMod15);

    const auto floatPlusMul = value_node->binary_expr.op1;
    ASSERT_EQ(floatPlusMul->binary_expr.bin_op, BinaryExprType::ADD);
    ASSERT_EQ(floatPlusMul->binary_expr.op1->symbol.token->id, TokenId::FLOAT_LIT);
    ASSERT_EQ(floatPlusMul->binary_expr.op2->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(floatPlusMul->binary_expr.op1->parent, floatPlusMul);
    ASSERT_EQ(floatPlusMul->binary_expr.op2->parent, floatPlusMul);

    const auto intMulInt = floatPlusMul->binary_expr.op2;
    ASSERT_EQ(intMulInt->binary_expr.bin_op, BinaryExprType::MUL);
    ASSERT_EQ(intMulInt->binary_expr.op1->node_type, AstNodeType::AstSymbol);
    ASSERT_EQ(intMulInt->binary_expr.op2->node_type, AstNodeType::AstSymbol);
    ASSERT_EQ(intMulInt->binary_expr.op1->parent, intMulInt);
    ASSERT_EQ(intMulInt->binary_expr.op2->parent, intMulInt);
}

TEST(ParserHappyParseAddExprTests, Mul2IdentifierTest) {
    std::vector<Error> errors;
    Lexer lexer("myVar * myVar2", "Mul2IdentifierTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_algebraic_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
    ASSERT_EQ(value_node->binary_expr.op1->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);
}

TEST(ParserHappyParseAddExprTests, MulNumberIdentifierTest) {
    std::vector<Error> errors;
    Lexer lexer("25 * myVar2", "MulNumberIdentifierTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_algebraic_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
    ASSERT_EQ(value_node->binary_expr.op1->symbol.token->id, TokenId::INT_LIT);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);
}

TEST(ParserHappyParseAddExprTests, Mul2NumbersTest) {
    std::vector<Error> errors;
    Lexer lexer("25 * 21.5f", "Mul2NumbersTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_algebraic_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
    ASSERT_EQ(value_node->binary_expr.op1->symbol.token->id, TokenId::INT_LIT);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::FLOAT_LIT);
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);
}

TEST(ParserHappyParseAddExprTests, Mul2NumbersAndCharTest) {
    std::vector<Error> errors;
    Lexer lexer("25 * 21.5f / 'g'", "Mul2NumbersAndCharTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_algebraic_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::DIV);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::UNICODE_CHAR);
    ASSERT_EQ(value_node->binary_expr.op1->node_type, AstNodeType::AstBinaryExpr);
    auto bin_exp_op1 = value_node->binary_expr.op1;
    ASSERT_EQ(bin_exp_op1->parent, value_node);
    ASSERT_EQ(bin_exp_op1->binary_expr.bin_op, BinaryExprType::MUL);
    ASSERT_EQ(bin_exp_op1->binary_expr.op1->symbol.token->id, TokenId::INT_LIT);
    ASSERT_EQ(bin_exp_op1->binary_expr.op2->symbol.token->id, TokenId::FLOAT_LIT);
    auto bin_exp2_op1 = bin_exp_op1->binary_expr.op1;
    auto bin_exp2_op2 = bin_exp_op1->binary_expr.op2;
    ASSERT_EQ(bin_exp2_op1->parent, bin_exp_op1);
    ASSERT_EQ(bin_exp2_op2->parent, bin_exp_op1);
}

TEST(ParserHappyParseAddExprTests, IdentifierTest) {
    std::vector<Error> errors;
    Lexer lexer("myVar", "IdentifierTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_algebraic_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->symbol.token, nullptr);
    ASSERT_EQ(value_node->symbol.token->id, TokenId::IDENTIFIER);
}

TEST(ParserHappyParseAddExprTests, FloatTest) {
    std::vector<Error> errors;
    Lexer lexer("12_547.12f", "FloatTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_algebraic_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->symbol.token, nullptr);
    ASSERT_EQ(value_node->symbol.token->id, TokenId::FLOAT_LIT);
}

TEST(ParserHappyParseAddExprTests, IntTest) {
    std::vector<Error> errors;
    Lexer lexer("12_547", "IntTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_algebraic_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->symbol.token, nullptr);
    ASSERT_EQ(value_node->symbol.token->id, TokenId::INT_LIT);
}

TEST(ParserHappyParseAddExprTests, UnicodeCharTest) {
    std::vector<Error> errors;
    Lexer lexer("\'g\'", "UnicodeCharTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_algebraic_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->symbol.token, nullptr);
    ASSERT_EQ(value_node->symbol.token->id, TokenId::UNICODE_CHAR);
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
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::EQUALS);
    ASSERT_EQ(value_node->binary_expr.op1->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);
}

TEST(ParserHappyParseCompExprTests, Comp2IdentifierAndNumberTest) {
    std::vector<Error> errors;
    Lexer lexer("myVar == myVar2 >= 45", "Add2IdentifierAndIncTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_comp_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::GREATER_OR_EQUALS);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::INT_LIT);
    ASSERT_EQ(value_node->binary_expr.op1->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);
    const auto identComp = value_node->binary_expr.op1;
    ASSERT_EQ(identComp->binary_expr.bin_op, BinaryExprType::EQUALS);
    ASSERT_EQ(identComp->binary_expr.op1->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(identComp->binary_expr.op2->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(identComp->binary_expr.op1->parent, identComp);
    ASSERT_EQ(identComp->binary_expr.op2->parent, identComp);

}

TEST(ParserHappyParseCompExprTests, FullAlgebraicCompareTest) {
    std::vector<Error> errors;
    Lexer lexer(".25f + 25 * 14 - 'g' % 15 == 3", "FullAlgebraicCompareTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_comp_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::EQUALS);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::INT_LIT);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);

    const auto algebraicExpr = value_node->binary_expr.op1;
    ASSERT_EQ(algebraicExpr->binary_expr.bin_op, BinaryExprType::SUB);
    ASSERT_EQ(algebraicExpr->binary_expr.op2->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(algebraicExpr->binary_expr.op1->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(algebraicExpr->binary_expr.op2->parent, algebraicExpr);
    ASSERT_EQ(algebraicExpr->binary_expr.op1->parent, algebraicExpr);

    const auto gMod15 = algebraicExpr->binary_expr.op2;
    ASSERT_EQ(gMod15->binary_expr.bin_op, BinaryExprType::MOD);
    ASSERT_EQ(gMod15->binary_expr.op1->symbol.token->id, TokenId::UNICODE_CHAR);
    ASSERT_EQ(gMod15->binary_expr.op2->symbol.token->id, TokenId::INT_LIT);
    ASSERT_EQ(gMod15->binary_expr.op1->parent, gMod15);
    ASSERT_EQ(gMod15->binary_expr.op2->parent, gMod15);

    const auto floatPlusMul = algebraicExpr->binary_expr.op1;
    ASSERT_EQ(floatPlusMul->binary_expr.bin_op, BinaryExprType::ADD);
    ASSERT_EQ(floatPlusMul->binary_expr.op1->symbol.token->id, TokenId::FLOAT_LIT);
    ASSERT_EQ(floatPlusMul->binary_expr.op2->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(floatPlusMul->binary_expr.op1->parent, floatPlusMul);
    ASSERT_EQ(floatPlusMul->binary_expr.op2->parent, floatPlusMul);

    const auto intMulInt = floatPlusMul->binary_expr.op2;
    ASSERT_EQ(intMulInt->binary_expr.bin_op, BinaryExprType::MUL);
    ASSERT_EQ(intMulInt->binary_expr.op1->symbol.token->id, TokenId::INT_LIT);
    ASSERT_EQ(intMulInt->binary_expr.op2->symbol.token->id, TokenId::INT_LIT);
    ASSERT_EQ(intMulInt->binary_expr.op1->parent, intMulInt);
    ASSERT_EQ(intMulInt->binary_expr.op2->parent, intMulInt);
}

TEST(ParserHappyParseCompExprTests, Add2IdentifierAndIncTest) {
    std::vector<Error> errors;
    Lexer lexer("myVar++ + myVar2", "Add2IdentifierAndIncTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_comp_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ADD);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);
    auto bin_exp_op1 = value_node->binary_expr.op1;
    ASSERT_EQ(bin_exp_op1->parent, value_node);
    ASSERT_EQ(bin_exp_op1->node_type, AstNodeType::AstUnaryExpr);
    ASSERT_EQ(bin_exp_op1->unary_expr.op, UnaryExprType::INC);
    ASSERT_EQ(bin_exp_op1->unary_expr.expr->parent, bin_exp_op1);
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
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op1->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::IDENTIFIER);
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
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op1->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::INT_LIT);
}

TEST(ParserHappyParseCompExprTests, Add2NumbersTest) {
    std::vector<Error> errors;
    Lexer lexer(".25f + 25", "Add2NumbersTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_comp_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ADD);
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op1->symbol.token->id, TokenId::FLOAT_LIT);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::INT_LIT);
}

TEST(ParserHappyParseCompExprTests, Add2NumberAndCharTest) {
    std::vector<Error> errors;
    Lexer lexer(".25f + 25 - 'g'", "Add2NumberAndCharTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_comp_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::SUB);
    auto bin_op1 = value_node->binary_expr.op1;
    auto bin_op2 = value_node->binary_expr.op2;
    ASSERT_EQ(bin_op2->symbol.token->id, TokenId::UNICODE_CHAR);
    ASSERT_EQ(bin_op2->parent, value_node);
    ASSERT_EQ(bin_op1->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(bin_op1->parent, value_node);
    ASSERT_EQ(bin_op1->binary_expr.bin_op, BinaryExprType::ADD);
    ASSERT_EQ(bin_op1->binary_expr.op1->symbol.token->id, TokenId::FLOAT_LIT);
    ASSERT_EQ(bin_op1->binary_expr.op2->symbol.token->id, TokenId::INT_LIT);
    ASSERT_EQ(bin_op1->binary_expr.op1->parent, bin_op1);
    ASSERT_EQ(bin_op1->binary_expr.op2->parent, bin_op1);
}

TEST(ParserHappyParseCompExprTests, FullAlgebraicTest) {
    std::vector<Error> errors;
    Lexer lexer(".25f + 25 * 14 - 'g' % 15", "FullAlgebraicTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_comp_expr();


    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::SUB);
    ASSERT_EQ(value_node->binary_expr.op1->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.op2->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    ASSERT_EQ(value_node->binary_expr.op2->parent, value_node);

    const auto gMod15 = value_node->binary_expr.op2;
    ASSERT_EQ(gMod15->binary_expr.bin_op, BinaryExprType::MOD);
    ASSERT_EQ(gMod15->binary_expr.op1->symbol.token->id, TokenId::UNICODE_CHAR);
    ASSERT_EQ(gMod15->binary_expr.op2->symbol.token->id, TokenId::INT_LIT);
    ASSERT_EQ(gMod15->binary_expr.op1->parent, gMod15);
    ASSERT_EQ(gMod15->binary_expr.op2->parent, gMod15);

    const auto floatPlusMul = value_node->binary_expr.op1;
    ASSERT_EQ(floatPlusMul->binary_expr.bin_op, BinaryExprType::ADD);
    ASSERT_EQ(floatPlusMul->binary_expr.op1->symbol.token->id, TokenId::FLOAT_LIT);
    ASSERT_EQ(floatPlusMul->binary_expr.op2->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(floatPlusMul->binary_expr.op1->parent, floatPlusMul);
    ASSERT_EQ(floatPlusMul->binary_expr.op2->parent, floatPlusMul);

    const auto intMulInt = floatPlusMul->binary_expr.op2;
    ASSERT_EQ(intMulInt->binary_expr.bin_op, BinaryExprType::MUL);
    ASSERT_EQ(intMulInt->binary_expr.op1->node_type, AstNodeType::AstSymbol);
    ASSERT_EQ(intMulInt->binary_expr.op2->node_type, AstNodeType::AstSymbol);
    ASSERT_EQ(intMulInt->binary_expr.op1->parent, intMulInt);
    ASSERT_EQ(intMulInt->binary_expr.op2->parent, intMulInt);
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
    ASSERT_EQ(value_node->binary_expr.op1->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::IDENTIFIER);
}

TEST(ParserHappyParseCompExprTests, MulNumberIdentifierTest) {
    std::vector<Error> errors;
    Lexer lexer("25 * myVar2", "MulNumberIdentifierTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_comp_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
    ASSERT_EQ(value_node->binary_expr.op1->symbol.token->id, TokenId::INT_LIT);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::IDENTIFIER);
}

TEST(ParserHappyParseCompExprTests, Mul2NumbersTest) {
    std::vector<Error> errors;
    Lexer lexer("25 * 21.5f", "Mul2NumbersTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_comp_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::MUL);
    ASSERT_EQ(value_node->binary_expr.op1->symbol.token->id, TokenId::INT_LIT);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::FLOAT_LIT);
}

TEST(ParserHappyParseCompExprTests, Mul2NumbersAndCharTest) {
    std::vector<Error> errors;
    Lexer lexer("25 * 21.5f / 'g'", "Mul2NumbersAndCharTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_comp_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::DIV);
    ASSERT_EQ(value_node->binary_expr.op2->symbol.token->id, TokenId::UNICODE_CHAR);
    ASSERT_EQ(value_node->binary_expr.op1->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(value_node->binary_expr.op1->binary_expr.bin_op, BinaryExprType::MUL);
    ASSERT_EQ(value_node->binary_expr.op1->binary_expr.op1->symbol.token->id, TokenId::INT_LIT);
    ASSERT_EQ(value_node->binary_expr.op1->binary_expr.op2->symbol.token->id, TokenId::FLOAT_LIT);
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
}

TEST(ParserHappyParseCompExprTests, FloatTest) {
    std::vector<Error> errors;
    Lexer lexer("12_547.12f", "FloatTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_comp_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->symbol.token, nullptr);
    ASSERT_EQ(value_node->symbol.token->id, TokenId::FLOAT_LIT);
}

TEST(ParserHappyParseCompExprTests, IntTest) {
    std::vector<Error> errors;
    Lexer lexer("12_547", "IntTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_comp_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->symbol.token, nullptr);
    ASSERT_EQ(value_node->symbol.token->id, TokenId::INT_LIT);
}

TEST(ParserHappyParseCompExprTests, UnicodeCharTest) {
    std::vector<Error> errors;
    Lexer lexer("\'g\'", "UnicodeCharTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_comp_expr();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->symbol.token, nullptr);
    ASSERT_EQ(value_node->symbol.token->id, TokenId::UNICODE_CHAR);
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
    ASSERT_EQ(value_node->binary_expr.op1->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(value_node->binary_expr.op1->parent, value_node);
    const auto sum_node = value_node->binary_expr.op2;
    ASSERT_EQ(sum_node->node_type, AstNodeType::AstBinaryExpr);
    ASSERT_EQ(sum_node->parent, value_node);
    ASSERT_EQ(sum_node->binary_expr.op1->symbol.token->id, TokenId::IDENTIFIER);
    ASSERT_EQ(sum_node->binary_expr.op2->symbol.token->id, TokenId::INT_LIT);
    ASSERT_EQ(sum_node->binary_expr.op1->parent, sum_node);
    ASSERT_EQ(sum_node->binary_expr.op2->parent, sum_node);
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
