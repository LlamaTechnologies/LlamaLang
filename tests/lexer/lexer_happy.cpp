#include <gtest/gtest.h>
#include "../../src/tokenize.hpp"

TEST(LexerHappyIdentifierTests, IdentifierSingleUnderscoreTest) {
    std::vector<Error> errors;
    Lexer lexer("_", "SingleUnderscoreTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::IDENTIFIER);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}


TEST(LexerHappyIdentifierTests, IdentifierAlphaOnlyTest) {
    std::vector<Error> errors;
    Lexer lexer("thisIsMyIdentifier", "AlphaOnlyTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::IDENTIFIER);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIdentifierTests, IdentifierAlphaNumericTest) {
    std::vector<Error> errors;
    Lexer lexer("myIdentifier214ok", "AlphaNumericTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::IDENTIFIER);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIdentifierTests, IdentifierUnderscorePrefixTest) {
    std::vector<Error> errors;
    Lexer lexer("_myIdentifier214ok", "UnderscorePrefixTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::IDENTIFIER);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIdentifierTests, IdentifierCompleteTest) {
    std::vector<Error> errors;
    Lexer lexer("myIdentifier214_ok", "IdentifierCompleteTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::IDENTIFIER);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIdentifierTests, IdentifierCompleteUnderscorePrefixTest) {
    std::vector<Error> errors;
    Lexer lexer("_myIdentifier214_ok", "CompleteUnderscorePrefixTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::IDENTIFIER);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}


TEST(LexerHappyIntegerTests, IntegerDigitOnlyTest) {
    std::vector<Error> errors;
    Lexer lexer("5415", "DigitOnlyTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::INT_LIT);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerDigitOnlyZeroPrefixTest) {
    std::vector<Error> errors;
    Lexer lexer("05415", "DigitOnlyZeroPrefixTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::INT_LIT);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerUnderscoreSeparatorTest) {
    std::vector<Error> errors;
    Lexer lexer("5_415", "UnderscoreSeparatorTest", errors);
    lexer.tokenize();

    BigInt okInt;
    bigint_init_unsigned(&okInt, 5415);
    auto int_token = lexer.get_current_token();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(int_token.id, TokenId::INT_LIT);
    ASSERT_EQ(int_token.int_lit, okInt);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}


