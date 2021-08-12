#include "../../src/compiler.hpp"
#include "../../src/error.hpp"
#include "../../src/lexer.hpp"
#include "../../src/parser.hpp"

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

  bool error_code = compiler::compile(output_path, executable_name, source_code, source_name);

  ASSERT_EQ(error_code, 0);
}

TEST(Integrations, MultipleFuncDefinitions) {
  const char *output_path = "";
  const char *executable_name = "MyProgram";
  const char *source_name = "MyProgram.llang";
  const char *source_code = "//#module MyProgram\n"
                            "//#load \"MyProgram.llang\"\n"
                            "\n"
                            "f i32 = 0\n"
                            "\n"
                            "fn getX(x i32) i32 {\n"
                            "\tret x\n"
                            "}\n"
                            "\n"
                            "fn myProgramInit() i32 {\n"
                            "\tret f\n"
                            "}\n";

  bool error_code = compiler::compile(output_path, executable_name, source_code, source_name);

  ASSERT_EQ(error_code, 0);
}

TEST(Integrations, FuncCalls) {
  const char *output_path = "";
  const char *executable_name = "MyProgram";
  const char *source_name = "MyProgram.llang";
  const char *source_code = "//#module MyProgram\n"
                            "//#load \"MyProgram.llang\"\n"
                            "\n"
                            "f i32 = 0\n"
                            "\n"
                            "fn getX(x i32) i32 {\n"
                            "\tret x\n"
                            "}\n"
                            "\n"
                            "fn myProgramInit() i32 {\n"
                            "\tf = getX(5)\n"
                            "\tret f\n"
                            "}\n";

  bool error_code = compiler::compile(output_path, executable_name, source_code, source_name);

  ASSERT_EQ(error_code, 0);
}

TEST(Integrations, FuncCallsWithAritmeticsArgs) {
  const char *output_path = "";
  const char *executable_name = "MyProgram";
  const char *source_name = "MyProgram.llang";
  const char *source_code = "//#module MyProgram\n"
                            "//#load \"MyProgram.llang\"\n"
                            "\n"
                            "f i32 = 0\n"
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

  bool error_code = compiler::compile(output_path, executable_name, source_code, source_name);

  ASSERT_EQ(error_code, 0);
}

TEST(Integrations, FuncCallsInAritmeticsArgs) {
  const char *output_path = "";
  const char *executable_name = "MyProgram";
  const char *source_name = "MyProgram.llang";
  const char *source_code = "//#module MyProgram\n"
                            "//#load \"MyProgram.llang\"\n"
                            "\n"
                            "f i32 = 0\n"
                            "\n"
                            "fn getX(x i32) i32 {\n"
                            "\tret x\n"
                            "}\n"
                            "\n"
                            "fn myProgramInit() i32 {\n"
                            "\tf = 85 + getX(5)\n"
                            "\tret f\n"
                            "}\n";

  bool error_code = compiler::compile(output_path, executable_name, source_code, source_name);

  ASSERT_EQ(error_code, 0);
}

TEST(Integrations, RetComplexExpretion) {
  const char *output_path = "";
  const char *executable_name = "MyProgram";
  const char *source_name = "MyProgram.llang";
  const char *source_code = "//#module MyProgram\n"
                            "//#load \"MyProgram.llang\"\n"
                            "\n"
                            "f i32 = 0\n"
                            "\n"
                            "fn getX(x i32) i32 {\n"
                            "\tret x\n"
                            "}\n"
                            "\n"
                            "fn myProgramInit() i32 {\n"
                            "\tret 85 + getX(5)\n"
                            "}\n";

  bool error_code = compiler::compile(output_path, executable_name, source_code, source_name);

  ASSERT_EQ(error_code, 0);
}
