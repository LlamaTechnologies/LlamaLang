#include <gtest/gtest.h>
#include "../../src/compiler.hpp"
#include "../../src/error.hpp"
#include "../../src/lexer.hpp"
#include "../../src/parser.hpp"

TEST(Integrations, FromCompiler) {
    const char* output_path = "";
    const char* executable_name = "MyProgram";
    const char* source_name = "MyProgram.llang";
    const char* source_code = "//#module MyProgram\n" 
    "//#load \"MyProgram.llang\"\n"
    "\n"
    "f i32 = 0\n"
    "\n"
    "fn myProgramInit(param1 i32) i32 {\n"
    "\t// f = myFunction(f + 5)\n"
    "\t// should return 42\n"
    "\tret f\n"
    "}\n"; 
    
    std::vector<Error> errors;
    
    Lexer lexer(source_code, source_name, errors);
    lexer.tokenize();
    
    Parser parser(lexer, errors);
    auto source_code_node = parser.parse();

    compiler::compile(output_path, executable_name, source_code_node, errors);

    ASSERT_EQ(errors.size(), 0L);
}
