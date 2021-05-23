#include <gtest/gtest.h>
#include "../../src/tokenize.hpp"

TEST(LexerSadIdentifierTests, IdentifierNumOnlyTest) {
    std::vector<Error> errors;
    Lexer lexer("1542", "NumOnlyTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_NE(lexer.get_current_token().id, TokenId::IDENTIFIER);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerSadIdentifierTests, IdentifierNumPrefixTest) {
    std::vector<Error> errors;
    Lexer lexer("1dentifier", "NumPrefixTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 1L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::ERROR);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerSadIdentifierTests, IdentifierInvalidCharPrefixTest) {
    std::vector<Error> errors;
    Lexer lexer("$identifier", "InvalidCharPrefixTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 1L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::ERROR);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerContiguousUnderscoreSeparatorTest) {
    std::vector<Error> errors;
    Lexer lexer("54__15", "ContiguousUnderscoreSeparatorTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 1L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::ERROR);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerUnexpectedCharTest) {
    std::vector<Error> errors;
    Lexer lexer("54$15", "UnexpectedCharTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 1L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::ERROR);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

/*
TEST(LexerSadIntegerTests, IntegerFollowedByIdTest) {
    std::vector<Error> errors;
    Lexer lexer("5_415_identifier", "FollowedByIdTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 1L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::ERROR);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}
*/

