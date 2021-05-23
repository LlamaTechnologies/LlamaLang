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


TEST(LexerHappyIntegerTests, IntegerUnsignedSpecifierTest) {
    std::vector<Error> errors;
    Lexer lexer("5415u", "UnsignedSpecifierTest", errors);
    lexer.tokenize();

    BigInt okInt;
    bigint_init_unsigned(&okInt, 5415);
    auto int_token = lexer.get_current_token();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(int_token.id, TokenId::INT_LIT);
    ASSERT_EQ(int_token.int_lit, okInt);
    ASSERT_EQ(lexer.source.at(int_token.end_pos), 'u');
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerByteTypeSpecifierTest) {
    std::vector<Error> errors;
    Lexer lexer("5415b", "ByteTypeSpecifierTest", errors);
    lexer.tokenize();

    BigInt okInt;
    bigint_init_unsigned(&okInt, 5415);
    auto int_token = lexer.get_current_token();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(int_token.id, TokenId::INT_LIT);
    ASSERT_EQ(int_token.int_lit, okInt);
    ASSERT_EQ(lexer.source.at(int_token.end_pos), 'b');
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerUnsignByteTypeSpecifierTest) {
    std::vector<Error> errors;
    Lexer lexer("5415ub", "UnsignByteTypeSpecifierTest", errors);
    lexer.tokenize();

    BigInt okInt;
    bigint_init_unsigned(&okInt, 5415);
    auto int_token = lexer.get_current_token();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(int_token.id, TokenId::INT_LIT);
    ASSERT_EQ(int_token.int_lit, okInt);
    ASSERT_EQ(lexer.source.at(int_token.end_pos - 1), 'u');
    ASSERT_EQ(lexer.source.at(int_token.end_pos), 'b');
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerWordTypeSpecifierTest) {
    std::vector<Error> errors;
    Lexer lexer("5415w", "WordTypeSpecifierTest", errors);
    lexer.tokenize();

    BigInt okInt;
    bigint_init_unsigned(&okInt, 5415);
    auto int_token = lexer.get_current_token();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(int_token.id, TokenId::INT_LIT);
    ASSERT_EQ(int_token.int_lit, okInt);
    ASSERT_EQ(lexer.source.at(int_token.end_pos), 'w');
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerUnsignWordTypeSpecifierTest) {
    std::vector<Error> errors;
    Lexer lexer("5415uw", "UnsignWordTypeSpecifierTest", errors);
    lexer.tokenize();

    BigInt okInt;
    bigint_init_unsigned(&okInt, 5415);
    auto int_token = lexer.get_current_token();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(int_token.id, TokenId::INT_LIT);
    ASSERT_EQ(int_token.int_lit, okInt);
    ASSERT_EQ(lexer.source.at(int_token.end_pos - 1), 'u');
    ASSERT_EQ(lexer.source.at(int_token.end_pos), 'w');
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerLongTypeSpecifierTest) {
    std::vector<Error> errors;
    Lexer lexer("5415l", "LongTypeSpecifierTest", errors);
    lexer.tokenize();

    BigInt okInt;
    bigint_init_unsigned(&okInt, 5415);
    auto int_token = lexer.get_current_token();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(int_token.id, TokenId::INT_LIT);
    ASSERT_EQ(int_token.int_lit, okInt);
    ASSERT_EQ(lexer.source.at(int_token.end_pos), 'l');
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerUnsignLongTypeSpecifierTest) {
    std::vector<Error> errors;
    Lexer lexer("5415ul", "UnsignLongTypeSpecifierTest", errors);
    lexer.tokenize();

    BigInt okInt;
    bigint_init_unsigned(&okInt, 5415);
    auto int_token = lexer.get_current_token();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(int_token.id, TokenId::INT_LIT);
    ASSERT_EQ(int_token.int_lit, okInt);
    ASSERT_EQ(lexer.source.at(int_token.end_pos - 1), 'u');
    ASSERT_EQ(lexer.source.at(int_token.end_pos), 'l');
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerUnderscoreUnsignLongTypeSpecifierTest) {
    std::vector<Error> errors;
    Lexer lexer("5415_ul", "UnderscoreUnsignLongTypeSpecifierTest", errors);
    lexer.tokenize();

    BigInt okInt;
    bigint_init_unsigned(&okInt, 5415);
    auto int_token = lexer.get_current_token();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(int_token.id, TokenId::INT_LIT);
    ASSERT_EQ(int_token.int_lit, okInt);
    ASSERT_EQ(lexer.source.at(int_token.end_pos - 2), '_');
    ASSERT_EQ(lexer.source.at(int_token.end_pos - 1), 'u');
    ASSERT_EQ(lexer.source.at(int_token.end_pos), 'l');
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

