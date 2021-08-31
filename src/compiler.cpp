#include "compiler.hpp"

#include "console.hpp"
#include "ir.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "semantic_analyzer.hpp"

static void printErrors(const std::vector<Error> &errors);

bool compiler::compile(const std::string &in_output_directory, const std::string &in_executable_name,
                       const std::string &in_source_code, const std::string &in_source_name) {
  std::vector<Error> errors;

  Lexer lexer(in_source_code, in_source_name, errors);
  lexer.tokenize();

  if (!errors.empty()) {
    console::write_line("LEXER");
    printErrors(errors);
    return false;
  }

  Parser parser(errors);
  auto source_code_node = parser.parse(lexer);

  if (!errors.empty()) {
    console::write_line("PARSER");
    printErrors(errors);
    return false;
  }

  LlvmIrGenerator generator(in_output_directory, in_executable_name);
  SemanticAnalyzer analyzer(errors);

  // first pass
  for (auto child : source_code_node->source_code.children) {
    switch (child->node_type) {
    case AstNodeType::AST_FUNC_DEF:
      if (analyzer.analizeFuncProto(child->function_def.proto))
        generator.gen_fn_proto(child->function_def.proto->function_proto, &child->function_def);
      break;
    case AstNodeType::AST_FUNC_PROTO:
      if (analyzer.analizeFuncProto(child))
        generator.gen_fn_proto(child->function_proto, nullptr);
      break;
    case AstNodeType::AST_VAR_DEF:
      // global variables
      if (analyzer.analizeVarDef(child, true))
        generator.gen_var_def(child->var_def, true);
      break;
    default:
      break;
    }
  }

  bool has_no_errors = true;
  // second pass
  for (auto child : source_code_node->source_code.children) {
    switch (child->node_type) {
    case AstNodeType::AST_FUNC_DEF:
      if (analyzer.analizeFuncBlock(child->function_def.block->block, child->function_def)) {
        bool block_no_error = generator.gen_fn_block(child->function_def.block->block, child->function_def);
        has_no_errors = has_no_errors && block_no_error;
      }
      break;

    default:
      continue;
    }
  }

  if (!errors.empty()) {
    console::write_line("SEMANTIC/IR");
    printErrors(errors);
    return false;
  }

  // generate IR output
  generator.flush();

  return has_no_errors;
}

void printErrors(const std::vector<Error> &errors) {
  for (const Error &error : errors) {
    std::string str;
    to_string(error, str);
    console::write_line(str);
  }
}
