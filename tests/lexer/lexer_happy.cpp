#include "../../src/lexer.hpp"

#include <gtest/gtest.h>
#include <string.h>

//==================================================================================
//          IDENTIFIER
//==================================================================================

TEST(LexerHappyIdentifierTests, IdentifierSingleUnderscoreTest) {
  std::vector<Error> errors;
  Lexer lexer("_", "file/directory", "SingleUnderscoreTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::IDENTIFIER);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIdentifierTests, IdentifierAlphaOnlyTest) {
  std::vector<Error> errors;
  Lexer lexer("thisIsMyIdentifier", "file/directory", "AlphaOnlyTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::IDENTIFIER);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIdentifierTests, IdentifierAlphaNumericTest) {
  std::vector<Error> errors;
  Lexer lexer("myIdentifier214ok", "file/directory", "AlphaNumericTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::IDENTIFIER);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIdentifierTests, IdentifierUnderscorePrefixTest) {
  std::vector<Error> errors;
  Lexer lexer("_myIdentifier214ok", "file/directory", "UnderscorePrefixTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::IDENTIFIER);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIdentifierTests, IdentifierCompleteTest) {
  std::vector<Error> errors;
  Lexer lexer("myIdentifier214_ok", "file/directory", "IdentifierCompleteTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::IDENTIFIER);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIdentifierTests, IdentifierCompleteUnderscorePrefixTest) {
  std::vector<Error> errors;
  Lexer lexer("_myIdentifier214_ok", "file/directory", "CompleteUnderscorePrefixTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::IDENTIFIER);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

//==================================================================================
//          KEYWORDS
//==================================================================================

TEST(LexerHappyKeywordsTests, KeywordFnTest) {
  std::vector<Error> errors;
  Lexer lexer("fn", "file/directory", "KeywordFnTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::FN);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyKeywordsTests, KeywordRetTest) {
  std::vector<Error> errors;
  Lexer lexer("ret", "file/directory", "KeywordRetTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::RET);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyKeywordsTests, KeywordAndTest) {
  std::vector<Error> errors;
  Lexer lexer("and", "file/directory", "KeywordAndTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::AND);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyKeywordsTests, KeywordOrTest) {
  std::vector<Error> errors;
  Lexer lexer("or", "file/directory", "KeywordOrTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::OR);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyKeywordsTests, KeywordIfTest) {
  std::vector<Error> errors;
  Lexer lexer("if", "file/directory", "KeywordFnTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::IF);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyKeywordsTests, KeywordFnNewLineTest) {
  std::vector<Error> errors;
  Lexer lexer("fn\n", "file/directory", "KeywordFnNewLineTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::FN);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyKeywordsTests, KeywordRetNewLineTest) {
  std::vector<Error> errors;
  Lexer lexer("ret \n", "file/directory", "KeywordRetNewLineTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::RET);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyKeywordsTests, KeywordAndNewLineTest) {
  std::vector<Error> errors;
  Lexer lexer("and\n", "file/directory", "KeywordAndNewLineTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::AND);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyKeywordsTests, KeywordOrNewLineTest) {
  std::vector<Error> errors;
  Lexer lexer("or\n", "file/directory", "KeywordOrNewLineTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::OR);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

//==================================================================================
//          OPERATORS
//==================================================================================

TEST(LexerHappyOperatorsTests, HashOperatorTest) {
  std::vector<Error> errors;
  Lexer lexer("#", "file/directory", "KeywordRetTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::HASH);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyOperatorsTests, OperatorsTest) {
  std::vector<Error> errors;
  Lexer lexer("= ++ -- ! || && == != < <= > >= + - * / % += -= *= /= %= & ^ | ~ << >>", "file/directory",
              "OperatorsTest", errors);
  lexer.tokenize();

  // do not change order
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::ASSIGN);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::PLUS_PLUS);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::MINUS_MINUS);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::NOT);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::OR);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::AND);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::EQUALS);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::NOT_EQUALS);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::LESS);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::LESS_OR_EQUALS);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::GREATER);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::GREATER_OR_EQUALS);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::PLUS);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::MINUS);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::MUL);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::DIV);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::MOD);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::PLUS_ASSIGN);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::MINUS_ASSIGN);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::MUL_ASSIGN);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::DIV_ASSIGN);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::MOD_ASSIGN);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::BIT_AND);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::BIT_XOR);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::BIT_OR);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::BIT_NOT);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::LSHIFT);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::RSHIFT);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

//==================================================================================
//          INT LIT
//==================================================================================

TEST(LexerHappyIntegerTests, IntegerDigitOnlyTest) {
  std::vector<Error> errors;
  Lexer lexer("5415", "file/directory", "DigitOnlyTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  auto int_token = lexer.get_next_token();
  ASSERT_EQ(int_token.id, TokenId::INT_LIT);
  ASSERT_EQ(strcmp(int_token.int_lit.number, "5415"), 0);
  ASSERT_EQ(int_token.int_lit.base, INT_BASE::DECIMAL);
  ASSERT_EQ(int_token.int_lit.is_negative, false);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerDigitOnlyZeroPrefixTest) {
  std::vector<Error> errors;
  Lexer lexer("0o5415", "file/directory", "DigitOnlyZeroPrefixTest", errors);
  lexer.tokenize();

  auto int_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(int_token.id, TokenId::INT_LIT);
  ASSERT_EQ(strcmp(int_token.int_lit.number, "05415"), 0);
  ASSERT_EQ(int_token.int_lit.base, INT_BASE::OCTAL);
  ASSERT_EQ(int_token.int_lit.is_negative, false);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerUnderscoreSeparatorTest) {
  std::vector<Error> errors;
  Lexer lexer("5_415", "file/directory", "UnderscoreSeparatorTest", errors);
  lexer.tokenize();

  auto int_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(int_token.id, TokenId::INT_LIT);
  ASSERT_EQ(strcmp(int_token.int_lit.number, "5415"), 0);
  ASSERT_EQ(int_token.int_lit.base, INT_BASE::DECIMAL);
  ASSERT_EQ(int_token.int_lit.is_negative, false);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerUnsignedSpecifierTest) {
  std::vector<Error> errors;
  Lexer lexer("5415u", "file/directory", "UnsignedSpecifierTest", errors);
  lexer.tokenize();

  auto int_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(int_token.id, TokenId::INT_LIT);
  ASSERT_EQ(strcmp(int_token.int_lit.number, "5415"), 0);
  ASSERT_EQ(int_token.int_lit.base, INT_BASE::DECIMAL);
  ASSERT_EQ(int_token.int_lit.is_negative, false);
  ASSERT_EQ(lexer.source.at(int_token.end_pos), 'u');
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerByteTypeSpecifierTest) {
  std::vector<Error> errors;
  Lexer lexer("5415b", "file/directory", "ByteTypeSpecifierTest", errors);
  lexer.tokenize();

  auto int_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(int_token.id, TokenId::INT_LIT);
  ASSERT_EQ(strcmp(int_token.int_lit.number, "5415"), 0);
  ASSERT_EQ(int_token.int_lit.base, INT_BASE::DECIMAL);
  ASSERT_EQ(int_token.int_lit.is_negative, false);
  ASSERT_EQ(lexer.source.at(int_token.end_pos), 'b');
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerUnsignByteTypeSpecifierTest) {
  std::vector<Error> errors;
  Lexer lexer("5415ub", "file/directory", "UnsignByteTypeSpecifierTest", errors);
  lexer.tokenize();

  auto int_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(int_token.id, TokenId::INT_LIT);
  ASSERT_EQ(strcmp(int_token.int_lit.number, "5415"), 0);
  ASSERT_EQ(int_token.int_lit.base, INT_BASE::DECIMAL);
  ASSERT_EQ(int_token.int_lit.is_negative, false);
  ASSERT_EQ(lexer.source.at(int_token.end_pos - 1), 'u');
  ASSERT_EQ(lexer.source.at(int_token.end_pos), 'b');
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerWordTypeSpecifierTest) {
  std::vector<Error> errors;
  Lexer lexer("5415w", "file/directory", "WordTypeSpecifierTest", errors);
  lexer.tokenize();

  auto int_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(int_token.id, TokenId::INT_LIT);
  ASSERT_EQ(strcmp(int_token.int_lit.number, "5415"), 0);
  ASSERT_EQ(int_token.int_lit.base, INT_BASE::DECIMAL);
  ASSERT_EQ(int_token.int_lit.is_negative, false);
  ASSERT_EQ(lexer.source.at(int_token.end_pos), 'w');
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerUnsignWordTypeSpecifierTest) {
  std::vector<Error> errors;
  Lexer lexer("5415uw", "file/directory", "UnsignWordTypeSpecifierTest", errors);
  lexer.tokenize();

  auto int_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(int_token.id, TokenId::INT_LIT);
  ASSERT_EQ(strcmp(int_token.int_lit.number, "5415"), 0);
  ASSERT_EQ(int_token.int_lit.base, INT_BASE::DECIMAL);
  ASSERT_EQ(int_token.int_lit.is_negative, false);
  ASSERT_EQ(lexer.source.at(int_token.end_pos - 1), 'u');
  ASSERT_EQ(lexer.source.at(int_token.end_pos), 'w');
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerLongTypeSpecifierTest) {
  std::vector<Error> errors;
  Lexer lexer("8l", "file/directory", "LongTypeSpecifierTest", errors);
  lexer.tokenize();

  auto int_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(int_token.id, TokenId::INT_LIT);
  ASSERT_EQ(strcmp(int_token.int_lit.number, "8"), 0);
  ASSERT_EQ(int_token.int_lit.base, INT_BASE::DECIMAL);
  ASSERT_EQ(int_token.int_lit.is_negative, false);
  ASSERT_EQ(lexer.source.at(int_token.end_pos), 'l');
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerLongTypeSpecifierSpaceTest) {
  std::vector<Error> errors;
  Lexer lexer("8l ", "file/directory", "LongTypeSpecifierSpaceTest", errors);
  lexer.tokenize();

  auto int_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(int_token.id, TokenId::INT_LIT);
  ASSERT_EQ(strcmp(int_token.int_lit.number, "8"), 0);
  ASSERT_EQ(int_token.int_lit.base, INT_BASE::DECIMAL);
  ASSERT_EQ(int_token.int_lit.is_negative, false);
  ASSERT_EQ(lexer.source.at(int_token.end_pos), 'l');
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerUnsignLongTypeSpecifierTest) {
  std::vector<Error> errors;
  Lexer lexer("5415ul", "file/directory", "UnsignLongTypeSpecifierTest", errors);
  lexer.tokenize();

  auto int_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(int_token.id, TokenId::INT_LIT);
  ASSERT_EQ(strcmp(int_token.int_lit.number, "5415"), 0);
  ASSERT_EQ(int_token.int_lit.base, INT_BASE::DECIMAL);
  ASSERT_EQ(int_token.int_lit.is_negative, false);
  ASSERT_EQ(lexer.source.at(int_token.end_pos - 1), 'u');
  ASSERT_EQ(lexer.source.at(int_token.end_pos), 'l');
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyIntegerTests, IntegerUnderscoreUnsignLongTypeSpecifierTest) {
  std::vector<Error> errors;
  Lexer lexer("5415_ul", "file/directory", "UnderscoreUnsignLongTypeSpecifierTest", errors);
  lexer.tokenize();

  auto int_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(int_token.id, TokenId::INT_LIT);
  ASSERT_EQ(strcmp(int_token.int_lit.number, "5415"), 0);
  ASSERT_EQ(int_token.int_lit.base, INT_BASE::DECIMAL);
  ASSERT_EQ(int_token.int_lit.is_negative, false);
  ASSERT_EQ(lexer.source.at(int_token.end_pos - 2), '_');
  ASSERT_EQ(lexer.source.at(int_token.end_pos - 1), 'u');
  ASSERT_EQ(lexer.source.at(int_token.end_pos), 'l');
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

//==================================================================================
//          FLOAT LIT
//==================================================================================

TEST(LexerHappyFloatTests, FloatNoDecimalsTest) {
  std::vector<Error> errors;
  Lexer lexer("52.", "file/directory", "NoDecimalsTest", errors);
  lexer.tokenize();

  auto float_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(float_token.id, TokenId::FLOAT_LIT);
  ASSERT_EQ(lexer.source.at(float_token.end_pos), '.');
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyFloatTests, FloatNoDecimalsFloatIdentifierTest) {
  std::vector<Error> errors;
  Lexer lexer("52.f", "file/directory", "NoDecimalsFloatIdentifierTest", errors);
  lexer.tokenize();

  auto float_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(float_token.id, TokenId::FLOAT_LIT);
  ASSERT_EQ(lexer.source.at(float_token.end_pos - 1), '.');
  ASSERT_EQ(lexer.source.at(float_token.end_pos), 'f');
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyFloatTests, FloatNoDecimalsFollowedByEOSTest) {
  std::vector<Error> errors;
  Lexer lexer("52.\n", "file/directory", "NoDecimalsFollowedByEOSTest", errors);
  lexer.tokenize();

  auto float_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(float_token.id, TokenId::FLOAT_LIT);
  ASSERT_EQ(lexer.source.at(float_token.end_pos), '.');
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyFloatTests, FloatNoIntegerPartTest) {
  std::vector<Error> errors;
  Lexer lexer(".52", "file/directory", "NoIntegerPartTest", errors);
  lexer.tokenize();

  auto float_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(float_token.id, TokenId::FLOAT_LIT);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyFloatTests, FloatNegativeNoIntegerPartTest) {
  std::vector<Error> errors;
  Lexer lexer("-.52", "file/directory", "FloatNegativeNoIntegerPartTest", errors);
  lexer.tokenize();

  auto minusToken = lexer.get_next_token();
  auto float_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(float_token.id, TokenId::FLOAT_LIT);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyFloatTests, FloatCompleteTest) {
  std::vector<Error> errors;
  Lexer lexer("1.52", "file/directory", "FloatCompleteTest", errors);
  lexer.tokenize();

  auto float_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(float_token.id, TokenId::FLOAT_LIT);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyFloatTests, FloatCompleteFloatSpecifierTest) {
  std::vector<Error> errors;
  Lexer lexer("1.52f", "file/directory", "FloatCompleteFloatSpecifierTest", errors);
  lexer.tokenize();

  auto float_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(float_token.id, TokenId::FLOAT_LIT);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyFloatTests, FloatCompleteExponentTest) {
  std::vector<Error> errors;
  Lexer lexer("6.022e23", "file/directory", "FloatCompleteExponentTest", errors);
  lexer.tokenize();

  auto float_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(float_token.id, TokenId::FLOAT_LIT);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyFloatTests, FloatCompleteNegativExponentTest) {
  std::vector<Error> errors;
  Lexer lexer("1.6e-19", "file/directory", "FloatCompleteNegativExponentTest", errors);
  lexer.tokenize();

  auto float_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(float_token.id, TokenId::FLOAT_LIT);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyFloatTests, FloatCompleteNegativeExponentTest) {
  std::vector<Error> errors;
  Lexer lexer("1.6e-19", "file/directory", "FloatCompleteNegativeExponentTest", errors);
  lexer.tokenize();

  auto float_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(float_token.id, TokenId::FLOAT_LIT);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyFloatTests, FloatCompleteNegativeExponentFloatSpecifierTest) {
  std::vector<Error> errors;
  Lexer lexer("1.6e-19f", "file/directory", "FloatCompleteNegativeExponentFloatSpecifierTest", errors);
  lexer.tokenize();

  auto float_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(float_token.id, TokenId::FLOAT_LIT);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyFloatTests, FloatNegativeCompleteNegativeExponentTest) {
  std::vector<Error> errors;
  Lexer lexer("-1.6e-19", "file/directory", "FloatNegativeCompleteNegativeExponentTest", errors);
  lexer.tokenize();

  auto float_token = lexer.get_next_token();
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(float_token.id, TokenId::FLOAT_LIT);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyFloatTests, FloatDecimalPartOnlyUnderscoreTest) {
  std::vector<Error> errors;
  Lexer lexer(".619_485", "file/directory", "FloatDecimalPartOnlyUnderscoreTest", errors);
  lexer.tokenize();

  auto float_token = lexer.get_next_token();
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(float_token.id, TokenId::FLOAT_LIT);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyFloatTests, FloatDecimalPartOnlyUnderscoreFloatSpecifierTest) {
  std::vector<Error> errors;
  Lexer lexer(".619_485f", "file/directory", "FloatDecimalPartOnlyUnderscoreFloatSpecifierTest", errors);
  lexer.tokenize();

  auto float_token = lexer.get_next_token();
  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(float_token.id, TokenId::FLOAT_LIT);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyFloatTests, FloatIntegerPartOnlyUnderscoresTest) {
  std::vector<Error> errors;
  Lexer lexer("417_528.", "file/directory", "FloatIntegerPartOnlyUnderscoresTest", errors);
  lexer.tokenize();

  auto float_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(float_token.id, TokenId::FLOAT_LIT);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyFloatTests, FloatCompleteUnderscoresTest) {
  std::vector<Error> errors;
  Lexer lexer("417_528.213_546f", "file/directory", "FloatCompleteUnderscoresTest", errors);
  lexer.tokenize();

  auto float_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(float_token.id, TokenId::FLOAT_LIT);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

//==================================================================================
//          STRING | CHARS
//==================================================================================

TEST(LexerHappyStringCharTests, EmptyStringTest) {
  std::vector<Error> errors;
  Lexer lexer("\"\"", "file/directory", "EmptyStringTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::STRING);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyStringCharTests, StringTest) {
  std::vector<Error> errors;
  Lexer lexer(" \"Hello world for 1st time!\" ", "file/directory", "StringTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::STRING);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyStringCharTests, CommentInsideStringTest) {
  std::vector<Error> errors;
  Lexer lexer(" \"Hello world for /*1st*/ time!\" ", "file/directory", "CommentInsideStringTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::STRING);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyStringCharTests, EscapedCharStringTest) {
  std::vector<Error> errors;
  Lexer lexer(" \"\'Hello world\\' \\n \\'for 1st time!\'\" ", "file/directory", "EscapedCharStringTest", errors);
  lexer.tokenize();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::STRING);
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyStringCharTests, EscapedCharTest) {
  std::vector<Error> errors;
  Lexer lexer("\'\\r\'", "file/directory", "EscapedCharTest", errors);
  lexer.tokenize();

  auto char_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(char_token.id, TokenId::UNICODE_CHAR);
  ASSERT_EQ(char_token.char_lit, '\r');
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyStringCharTests, EscapedCharCodeTest) {
  std::vector<Error> errors;
  Lexer lexer("\'\\x42\'", "file/directory", "EscapedCharTest", errors);
  lexer.tokenize();

  auto char_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(char_token.id, TokenId::UNICODE_CHAR);
  ASSERT_EQ(char_token.char_lit, '\x42');
  ASSERT_EQ(char_token.char_lit, 'B');
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

TEST(LexerHappyStringCharTests, EscapedCharUnicodeTest) {
  std::vector<Error> errors;
  Lexer lexer("\'\\u{00B6}\'", "file/directory", "EscapedCharUnicodeTest", errors);
  lexer.tokenize();

  auto char_token = lexer.get_next_token();

  ASSERT_EQ(errors.size(), 0L);
  ASSERT_EQ(char_token.id, TokenId::UNICODE_CHAR);
  ASSERT_EQ(char_token.char_lit, L'\u00B6');
  ASSERT_EQ(lexer.get_next_token().id, TokenId::_EOF);
}

//==================================================================================
//          PRINT
//==================================================================================
