#include <gtest/gtest.h>
#include "../../src/parser.cpp"


//==================================================================================
//          PARSE PRODUCT EXPRESSIONS FUNCTIONS
//==================================================================================

TEST(ParserHappyParseMulExprTests, IdentifierTest) {
    std::vector<Error> errors;
    Lexer lexer("myVar", "IdentifierTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_value();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->data.symbol->token, nullptr);
    ASSERT_EQ(value_node->data.symbol->token->id, TokenId::IDENTIFIER);
}


//==================================================================================
//          PARSE VALUE FUNCTIONS
//==================================================================================

TEST(ParserHappyParseValueTests, IdentifierTest) {
    std::vector<Error> errors;
    Lexer lexer("myVar", "IdentifierTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_value();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->data.symbol->token, nullptr);
    ASSERT_EQ(value_node->data.symbol->token->id, TokenId::IDENTIFIER);
}

TEST(ParserHappyParseValueTests, FloatTest) {
    std::vector<Error> errors;
    Lexer lexer("12_547.12f", "FloatTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_value();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->data.symbol->token, nullptr);
    ASSERT_EQ(value_node->data.symbol->token->id, TokenId::FLOAT_LIT);
}

TEST(ParserHappyParseValueTests, IntTest) {
    std::vector<Error> errors;
    Lexer lexer("12_547", "IntTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_value();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->data.symbol->token, nullptr);
    ASSERT_EQ(value_node->data.symbol->token->id, TokenId::INT_LIT);
}

TEST(ParserHappyParseValueTests, UnicodeCharTest) {
    std::vector<Error> errors;
    Lexer lexer("\'g\'", "UnicodeCharTest", errors);
    lexer.tokenize();

    Parser parser(lexer, errors);
    auto value_node = parser.parse_value();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(value_node->node_type, AstNodeType::AstSymbol);
    ASSERT_NE(value_node->data.symbol->token, nullptr);
    ASSERT_EQ(value_node->data.symbol->token->id, TokenId::UNICODE_CHAR);

}


//==================================================================================
//          UTILS
//==================================================================================

TEST(ParserHappyUtilTests, MatchFuncTest) {
    Token token;
    token.id = TokenId::FN;
    ASSERT_EQ(match(token, TokenId::AND, TokenId::_EOF, TokenId::FN, TokenId(size_t(TokenId::_EOF) + 1)), true);
}

TEST(ParserHappyUtilTests, MatchMacroTest) {
    Token token;
    token.id = TokenId::FN;
    ASSERT_EQ(MATCH(token, TokenId::AND, TokenId::_EOF, TokenId::FN), true);
}

TEST(ParserHappyUtilTests, MatchMacroNoMatchTest) {
    Token token;
    token.id = TokenId::FN;
    ASSERT_EQ(MATCH(token, TokenId::AND, TokenId::_EOF, TokenId::OR), false);
}

