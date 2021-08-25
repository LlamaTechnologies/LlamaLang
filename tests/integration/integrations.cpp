#include "../../src/compiler.hpp"
#include "../../src/error.hpp"
#include "../../src/lexer.hpp"
#include "../../src/parser.hpp"

#define LL_INTEGRATIONS_TESTS 0
#if (LL_INTEGRATIONS_TESTS == 1)
  #include <gtest/gtest.h>

TEST(Integrations, FromCompiler) {
  const char *output_path = "";
  const char *executable_name = "MyProgram";
  const char *source_name = "MyProgram.llang";
  const char *source_code = "f i32 = 0\n"
                            "\n"
                            "fn myProgramInit(param1 i32) i32 {\n"
                            "\tret f\n"
                            "}\n";

  bool has_no_errors = compiler::compile(output_path, executable_name, source_code, source_name);

  ASSERT_EQ(has_no_errors, true);
}

TEST(Integrations, RetComplexExpression) {
  const char *output_path = "";
  const char *executable_name = "MyProgram";
  const char *source_name = "MyProgram.llang";
  const char *source_code = "f i32 = 0\n"
                            "\n"
                            "fn getX(x i32) i32 {\n"
                            "\tret x\n"
                            "}\n"
                            "\n"
                            "fn myProgramInit() i32 {\n"
                            "\tret 85 + getX(5)\n"
                            "}\n";

  bool has_no_errors = compiler::compile(output_path, executable_name, source_code, source_name);

  ASSERT_EQ(has_no_errors, true);
}

TEST(Integrations, MultipleFuncDefinitions) {
  const char *output_path = "";
  const char *executable_name = "MyProgram";
  const char *source_name = "MyProgram.llang";
  const char *source_code = "f i32 = -645\n"
                            "\n"
                            "fn getX(x i32) i32 {\n"
                            "\tret x\n"
                            "}\n"
                            "\n"
                            "fn myProgramInit() i32 {\n"
                            "\tret f\n"
                            "}\n";

  bool has_no_errors = compiler::compile(output_path, executable_name, source_code, source_name);

  ASSERT_EQ(has_no_errors, true);
}

TEST(Integrations, FuncCalls) {
  const char *output_path = "";
  const char *executable_name = "MyProgram";
  const char *source_name = "MyProgram.llang";
  const char *source_code = "f i32 = 1447\n"
                            "\n"
                            "fn getX(x i32) i32 {\n"
                            "\tret x\n"
                            "}\n"
                            "\n"
                            "fn myProgramInit() i32 {\n"
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
  const char *source_code = "f i32 = 0\n"
                            "\n"
                            "fn getX(x i32) i32 {\n"
                            "\tret x\n"
                            "}\n"
                            "\n"
                            "fn myProgramInit() i32 {\n"
                            "\tvar i32 = 85\n"
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
  const char *source_code = "f i32 = 0\n"
                            "\n"
                            "fn getX(x i32) i32 {\n"
                            "\tret x\n"
                            "}\n"
                            "\n"
                            "fn myProgramInit() i32 {\n"
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
