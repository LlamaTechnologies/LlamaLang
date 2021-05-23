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

//==================================================================================
//          KEYWORDS
//==================================================================================

TEST(LexerSadKeywordsTests, KeywordFnUpperCaseTest) {
    std::vector<Error> errors;
    Lexer lexer("FN", "KeywordFnUpperCaseTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::IDENTIFIER);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerSadKeywordsTests, KeywordRetUpperCaseTest) {
    std::vector<Error> errors;
    Lexer lexer("RET", "KeywordRetUpperCaseTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::IDENTIFIER);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerSadKeywordsTests, KeywordAndUpperCaseTest) {
    std::vector<Error> errors;
    Lexer lexer("AND", "KeywordAndUpperCaseTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::IDENTIFIER);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerSadKeywordsTests, KeywordOrUpperCaseTest) {
    std::vector<Error> errors;
    Lexer lexer("OR", "KeywordOrUpperCaseTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 0L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::IDENTIFIER);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

//==================================================================================
//          OPERATORS
//==================================================================================

/* TODO: implement this functionality
TEST(LexerHappyOperatorsTests, ForbiddenOperatorsTest) {
    std::vector<Error> errors;
    Lexer lexer("## %% ||| &&& === +++ --- ** /// ++= --= **= %%= ^^ ~~ <<< >>> >>< <<> <>< ><>",
        "ForbiddenOperatorsTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 0L);
    
    // Multiple hashes
    ASSERT_EQ(lexer.get_current_token().id, TokenId::ERROR);
    // Multiple percents
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);
    // 2+ verticals bars
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);
    // 2+ ampersands
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);
    // 2+ equals
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);
    // 2+ less
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);
    // 2+ greaters
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);
    // 2+ plus
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);
    // 2+ dashes
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);
    // Multiple stars
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);
    // 2+ slashes
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);
    // assign and increment is not supported
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);
    // assign and decrement is not supported
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);

    // error 2+ stars and assign
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ASSIGN);

    // error 1+ percents and assign
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ASSIGN);

    // Multiple carets
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);

    // Multiple tildes
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);

    // multiple less/greater signs
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::ERROR);

    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}
*/

//==================================================================================
//          INT LIT
//==================================================================================

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

TEST(LexerSadIntegerTests, IntegerFollowedByIdTest) {
    std::vector<Error> errors;
    Lexer lexer("5_415_identifier", "FollowedByIdTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 1L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::ERROR);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

//==================================================================================
//          FLOAT LIT
//==================================================================================

// TODO: 

//==================================================================================
//          STRING | CHARS
//==================================================================================

TEST(LexerSadStringCharTests, EmptyCharTest) {
    std::vector<Error> errors;
    Lexer lexer("\'\'", "EmptyStringTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 1L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::ERROR);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerSadStringCharTests, MultilineStringTest) {
    std::vector<Error> errors;
    Lexer lexer(" \"Hello world for...\n1st time!\" ", "MultilineStringTest", errors);
    lexer.tokenize();

    ASSERT_EQ(errors.size(), 1L);
    ASSERT_EQ(lexer.get_current_token().id, TokenId::ERROR);
    ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

