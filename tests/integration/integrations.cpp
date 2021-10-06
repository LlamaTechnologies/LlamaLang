#include "../../src/compiler.hpp"
#include "../../src/error.hpp"
#include "../../src/file_utils.hpp"
#include "../../src/lexer.hpp"
#include "../../src/parser.hpp"
#include "../../src/src_code_repository.hpp"

#include <gtest/gtest.h>

#define LL_INTEGRATIONS_TESTS 0

//==================================================================================
//          SEMANTIC LOADED FILE
//==================================================================================

TEST(Integrations, FunctionDeclarationExternFile) {
  static const char *external_file = "extern fn my_func() s32\n";

  struct FileInputMock : FileInput {
    FILE_PATH_STATUS verify_file_path(std::filesystem::path &in_path) const { return FILE_PATH_STATUS::OK; }
    std::string open_and_read_file(const std::string &in_file_path) const { return std::string(external_file); }
  } file_input;

  std::vector<Error> errors;
  Parser parser = Parser(errors, file_input);

  // given: source code
  const char *source_code = "#load \"extern_file\"\n"
                            "\n"
                            "fn main() s32 {\n"
                            "\tmy_func()\n"
                            "\tret 0\n"
                            "}\n";

  // given: loader file lexer
  Lexer lexer = Lexer(source_code, "file/directory", "main.llama", errors);
  lexer.tokenize();

  // given: parsed loader file
  AstSourceCode *main_src_code = parser.parse(lexer);

  // when: analyze ext_src_code
  VoidGenerator generator;
  bool program_ok = compiler::compile_node(&generator, errors, main_src_code);

  // then:
  ASSERT_EQ(errors.size(), 0);
  ASSERT_TRUE(program_ok);

  RepositorySrcCode::clean();
  delete main_src_code;
}

#if (LL_INTEGRATIONS_TESTS == 1)

TEST(Integrations, FromCompiler) {
  const char *output_path = "";
  const char *executable_name = "MyProgram";
  const char *source_name = "MyProgram.llang";
  const char *source_code = "f s32 = 0\n"
                            "\n"
                            "fn myProgramInit(param1 s32) s32 {\n"
                            "\tret f\n"
                            "}\n";

  bool has_no_errors = compiler::compile(output_path, executable_name, source_code, source_name);

  ASSERT_EQ(has_no_errors, true);
}

TEST(Integrations, RetComplexExpression) {
  const char *output_path = "";
  const char *executable_name = "MyProgram";
  const char *source_name = "MyProgram.llang";
  const char *source_code = "f s32 = 0\n"
                            "\n"
                            "fn getX(x s32) s32 {\n"
                            "\tret x\n"
                            "}\n"
                            "\n"
                            "fn myProgramInit() s32 {\n"
                            "\tret 85 + getX(5)\n"
                            "}\n";

  bool has_no_errors = compiler::compile(output_path, executable_name, source_code, source_name);

  ASSERT_EQ(has_no_errors, true);
}

TEST(Integrations, MultipleFuncDefinitions) {
  const char *output_path = "";
  const char *executable_name = "MyProgram";
  const char *source_name = "MyProgram.llang";
  const char *source_code = "f s32 = -645\n"
                            "\n"
                            "fn getX(x s32) s32 {\n"
                            "\tret x\n"
                            "}\n"
                            "\n"
                            "fn myProgramInit() s32 {\n"
                            "\tret f\n"
                            "}\n";

  bool has_no_errors = compiler::compile(output_path, executable_name, source_code, source_name);

  ASSERT_EQ(has_no_errors, true);
}

TEST(Integrations, FuncCalls) {
  const char *output_path = "";
  const char *executable_name = "MyProgram";
  const char *source_name = "MyProgram.llang";
  const char *source_code = "f s32 = 1447\n"
                            "\n"
                            "fn getX(x s32) s32 {\n"
                            "\tret x\n"
                            "}\n"
                            "\n"
                            "fn myProgramInit() s32 {\n"
                            "\tf = getX(5)\n"
                            "\tret f\n"
                            "}\n";

  bool has_no_errors = compiler::compile(output_path, executable_name, source_code, source_name);

  ASSERT_EQ(has_no_errors, true);
}

TEST(Integrations, FuncCallsWithAritmeticsArgs) {
  const char *output_path = "";
  const char *executable_name = "MyProgram";
  const char *source_name = "MyProgram.llang";
  const char *source_code = "f s32 = 0\n"
                            "\n"
                            "fn getX(x s32) s32 {\n"
                            "\tret x\n"
                            "}\n"
                            "\n"
                            "fn myProgramInit() s32 {\n"
                            "\tvar s32 = 85\n"
                            "\tf = getX(5 + var)\n"
                            "\tret f\n"
                            "}\n";

  bool has_no_errors = compiler::compile(output_path, executable_name, source_code, source_name);

  ASSERT_EQ(has_no_errors, true);
}

TEST(Integrations, FuncCallsInAritmeticsArgs) {
  const char *output_path = "";
  const char *executable_name = "MyProgram";
  const char *source_name = "MyProgram.llang";
  const char *source_code = "f s32 = 0\n"
                            "\n"
                            "fn getX(x s32) s32 {\n"
                            "\tret x\n"
                            "}\n"
                            "\n"
                            "fn myProgramInit() s32 {\n"
                            "\tf = 85 + getX(5)\n"
                            "\tret f\n"
                            "}\n";

  bool has_no_errors = compiler::compile(output_path, executable_name, source_code, source_name);

  ASSERT_EQ(has_no_errors, true);
}

TEST(Integrations, PrintfDecl) {
  const char *output_path = "";
  const char *executable_name = "MyProgram";
  const char *source_name = "MyProgram.llang";
  const char *source_code = "extern fn printf() void\n";

  bool has_no_errors = compiler::compile(output_path, executable_name, source_code, source_name);

  ASSERT_EQ(has_no_errors, true);
}

TEST(Integrations, PrintfCall) {
  const char *output_path = "";
  const char *executable_name = "MyProgram";
  const char *source_name = "MyProgram.llang";
  const char *source_code = "extern fn printf() void\n"
                            "\n"
                            "fn myProgramInit() void {\n"
                            "\tprintf(5)\n"
                            "}\n";

  bool has_no_errors = compiler::compile(output_path, executable_name, source_code, source_name);

  ASSERT_EQ(has_no_errors, true);
}
#endif // LL_INTEGRATIONS_TESTS
