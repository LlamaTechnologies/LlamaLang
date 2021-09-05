#include "../../src/compiler.hpp"
#include "../../src/error.hpp"
#include "../../src/lexer.hpp"
#include "../../src/parser.hpp"

#include <gtest/gtest.h>

#define LL_INTEGRATIONS_TESTS 0

//==================================================================================
//          SEMANTIC LOADED FILE
//==================================================================================

TEST(Integrations, FunctionDeclarationExternFile) {
  std::vector<Error> errors;
  Parser parser = Parser(errors);

  // given: extern file
  Lexer ext_lexer = Lexer("extern fn my_func() i32\n", "extern_file", errors);
  ext_lexer.tokenize();

  AstSourceCode *ext_src_code = parser.parse(ext_lexer);

  //// given: loader file
  const char *main_file_name = "main.llama";
  // given: load directive :: #load "extern_file"
  AstDirective *load_directive = new AstDirective(0, 0, main_file_name);
  load_directive->directive_type = DirectiveType::LOAD;
  load_directive->argument.ast_node = ext_src_code;

  // given: main fn
  // fn main() i32 {
  //   my_func()
  //   ret 0
  // }
  const char *source_code = "//oad \"extern_file\"\n"
                            "\n"
                            "fn main() i32 {\n"
                            "\tmy_func()\n"
                            "\tret 0\n"
                            "}\n";

  // given: loader file lexer
  Lexer lexer = Lexer(source_code, main_file_name, errors);
  lexer.tokenize();

  // given: parsed loader file
  AstSourceCode *main_src_code = parser.parse(lexer);
  main_src_code->children.insert(main_src_code->children.begin(), load_directive);

  // when: analyze ext_src_code
  VoidGenerator generator;
  bool program_ok = compiler::compile_node(&generator, "output", "executable_name", errors, main_src_code);

  // then:
  ASSERT_EQ(errors.size(), 0);
  ASSERT_TRUE(program_ok);

  delete main_src_code;
  delete ext_src_code;
}

#if (LL_INTEGRATIONS_TESTS == 1)

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
