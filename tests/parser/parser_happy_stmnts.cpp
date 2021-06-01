#include <gtest/gtest.h>
#include "../../src/ast_nodes.hpp"
#include "../../src/lexer.hpp"
#include "../../src/parser.hpp"

TEST(ParserHappyStmntTests, RetStmnt) {
    std::vector<Error> errors;
    Lexer lexer("ret myVar * (5 + 8)", "Ret stmnt", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_ret_stmnt();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstUnaryExpr);
    ASSERT_EQ(value_node->data.unary_expr->op, UnaryExprType::RET);
    ASSERT_NE(value_node->data.unary_expr->expr, nullptr);
}

TEST(ParserHappyStmntTests, RetEmptyStmnt) {
    std::vector<Error> errors;
    Lexer lexer("ret ", "RetEmptyStmnt", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_ret_stmnt();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstUnaryExpr);
    ASSERT_EQ(value_node->data.unary_expr->op, UnaryExprType::RET);
    ASSERT_EQ(value_node->data.unary_expr->expr, nullptr);
}
