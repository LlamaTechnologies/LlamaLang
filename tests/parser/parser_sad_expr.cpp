#include "../../src/Types.hpp"
#include "../../src/ast_nodes.hpp"
#include "../../src/lexer.hpp"
#include "../../src/parser.hpp"

#include <gtest/gtest.h>
#include <string.h>

TEST(ParserSadTypeStmntTests, ArrayExprSize) {
  std::vector<Error> errors;
  Lexer lexer("[array_size + array_size_2]s32", "file/directory", "TypeArrayParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstType *value_node = parser.parse_type(lexer);

  ASSERT_EQ(errors.size(), 1L);
  ASSERT_EQ(value_node, nullptr);
}

TEST(ParserSadTypeStmntTests, ArrayLitFloatingPointSize) {
  std::vector<Error> errors;
  Lexer lexer("[25.0]s32", "file/directory", "TypeArrayParse", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstType *value_node = parser.parse_type(lexer);

  ASSERT_EQ(errors.size(), 1L);
  ASSERT_EQ(value_node, nullptr);
}

TEST(ParserSadVarDefStmntTests, InitArraySizeConstFP) {
  TypesRepository types_repository = TypesRepository::get();

  const char *src_code_str = "array [7.0]s32";

  std::vector<Error> errors;
  Lexer lexer(src_code_str, "internal/tests", "InitArraySizeConstFP", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstSourceCode *src_code = parser.parse(lexer);
  // then:
  ASSERT_NE(errors.size(), 0L);
  ASSERT_NE(src_code, nullptr);

  delete src_code;
}

TEST(ParserSadVarDefStmntTests, InitArraySizeConstBool) {
  TypesRepository types_repository = TypesRepository::get();

  const char *src_code_str = "array [true]s32";

  std::vector<Error> errors;
  Lexer lexer(src_code_str, "internal/tests", "InitArraySizeConstBool", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstSourceCode *src_code = parser.parse(lexer);

  // then:
  ASSERT_NE(errors.size(), 0L);
  ASSERT_NE(src_code, nullptr);

  delete src_code;
}

TEST(ParserSadVarDefStmntTests, InitArraySizeConstChar) {
  TypesRepository types_repository = TypesRepository::get();

  const char *src_code_str = "array ['5']s32";

  std::vector<Error> errors;
  Lexer lexer(src_code_str, "internal/tests", "InitArraySizeConstChar", errors);
  lexer.tokenize();

  Parser parser(errors);
  const AstSourceCode *src_code = parser.parse(lexer);

  // then:
  ASSERT_NE(errors.size(), 0L);
  ASSERT_NE(src_code, nullptr);

  delete src_code;
}
