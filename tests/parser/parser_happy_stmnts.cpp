#include <gtest/gtest.h>
#include "../../src/ast_nodes.hpp"
#include "../../src/lexer.hpp"
#include "../../src/parser.hpp"

//==================================================================================
//          PARSE RETURN STATEMENTS
//==================================================================================

TEST(ParserHappyStmntTests, RetStmnt) {
    std::vector<Error> errors;
    Lexer lexer("ret myVar * (5 + 8)", "Ret stmnt", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_ret_stmnt();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstUnaryExpr);
    ASSERT_EQ(value_node->unary_expr.op, UnaryExprType::RET);
    ASSERT_NE(value_node->unary_expr.expr, nullptr);
}

TEST(ParserHappyStmntTests, RetEmptyStmnt) {
    std::vector<Error> errors;
    Lexer lexer("ret ", "RetEmptyStmnt", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_ret_stmnt();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstUnaryExpr);
    ASSERT_EQ(value_node->unary_expr.op, UnaryExprType::RET);
    ASSERT_EQ(value_node->unary_expr.expr, nullptr);
}

//==================================================================================
//          PARSE ASSIGN STATEMENTS
//==================================================================================

TEST(ParserHappyStmntTests, AssignStmntTest) {
    std::vector<Error> errors;
    Lexer lexer("myVar = 5 * (6 + 9)", "AssignStmntTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_assign_stmnt();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstBinaryExpr); 
    ASSERT_EQ(value_node->binary_expr.bin_op, BinaryExprType::ASSIGN);
    ASSERT_EQ(value_node->binary_expr.op1->node_type, AstNodeType::AstSymbol);
    ASSERT_EQ(value_node->binary_expr.op2->node_type, AstNodeType::AstBinaryExpr);
}

//==================================================================================
//          PARSE TYPES
//==================================================================================

TEST(ParserHappyStmntTests, TypeNameParse) {
    std::vector<Error> errors;
    Lexer lexer("MyType", "TypeNameParse", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_type();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(value_node->ast_type.type, AstTypeType::DataType);
    ASSERT_EQ(value_node->ast_type.name, "MyType");
}


TEST(ParserHappyStmntTests, TypeArrayParse) {
    std::vector<Error> errors;
    Lexer lexer("[]MyType", "TypeArrayParse", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_type();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(value_node->ast_type.type, AstTypeType::Array);
    ASSERT_NE(value_node->ast_type.data_type, nullptr);
    ASSERT_EQ(value_node->ast_type.data_type->node_type, AstNodeType::AstType);
    ASSERT_EQ(value_node->ast_type.data_type->ast_type.type, AstTypeType::DataType);
    ASSERT_EQ(value_node->ast_type.data_type->ast_type.name, "MyType");
}

TEST(ParserHappyStmntTests, TypePointerParse) {
    std::vector<Error> errors;
    Lexer lexer("*MyType", "TypePointerParse", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_type();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstType);
    ASSERT_EQ(value_node->ast_type.type, AstTypeType::Pointer);
    ASSERT_NE(value_node->ast_type.data_type, nullptr);
    ASSERT_EQ(value_node->ast_type.data_type->node_type, AstNodeType::AstType);
    ASSERT_EQ(value_node->ast_type.data_type->ast_type.type, AstTypeType::DataType);
    ASSERT_EQ(value_node->ast_type.data_type->ast_type.name, "MyType");
}
