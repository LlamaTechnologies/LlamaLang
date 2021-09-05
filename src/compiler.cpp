#include "compiler.hpp"

#include "console.hpp"
#include "ir.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "semantic_analyzer.hpp"

#include <stack>

static void _print_errors(const std::vector<Error> &errors);

bool compiler::compile(const std::string &in_output_directory, const std::string &in_executable_name,
                       const std::string &in_source_code, const std::string &in_source_name) {
  LL_ASSERT(in_source_code.size() > 0);
  LL_ASSERT(in_source_name.size() > 0);

  std::vector<Error> errors;

  Lexer lexer(in_source_code, in_source_name, errors);
  lexer.tokenize();

  if (!errors.empty()) {
    console::write_line("LEXER");
    _print_errors(errors);
    return false;
  }

  Parser parser(errors);
  AstSourceCode *const source_code_node = parser.parse(lexer);

  if (!errors.empty()) {
    console::write_line("PARSER");
    _print_errors(errors);
    return false;
  }

  LlvmIrGenerator generator(in_output_directory, in_executable_name);
  return compile_node(&generator, in_output_directory, in_executable_name, errors, source_code_node);
}

bool compiler::compile_node(GeneratorInterface *in_generator, const std::string &in_output_directory,
                            const std::string &in_executable_name, std::vector<Error> &errors,
                            AstSourceCode *const in_source_code) {
  LL_ASSERT(in_generator != nullptr);
  LL_ASSERT(in_source_code != nullptr);
  LL_ASSERT(in_output_directory.size() > 0);
  LL_ASSERT(in_executable_name.size() > 0);

  SemanticAnalyzer analyzer(errors);

  auto stack = std::stack<AstSourceCode *>();
  bool has_no_errors = true;

  AstSourceCode *src_node = in_source_code;
  // do this for every load directive
  do {
    // first pass
    for (AstNode *child : src_node->source_code()->children) {
      switch (child->node_type) {
      case AstNodeType::AST_DIRECTIVE: {
        // if it is a load directive we should analyze all the source code.
        AstDirective *directive_node = child->directive();
        if (directive_node->directive_type == DirectiveType::LOAD) {
          auto new_src_node = directive_node->argument.ast_node->source_code();
          if (new_src_node->is_analyzed == false) {
            stack.push(src_node);
            src_node = new_src_node;
          }
        }
      } break;
      case AstNodeType::AST_FN_DEF:
        if (analyzer.analize_fn_proto(child->fn_def()->proto))
          in_generator->gen_fn_proto(child->fn_def()->proto->fn_proto(), child->fn_def());
        break;
      case AstNodeType::AST_FN_PROTO:
        if (analyzer.analize_fn_proto(child->fn_proto()))
          in_generator->gen_fn_proto(child->fn_proto(), nullptr);
        break;
      case AstNodeType::AST_VAR_DEF:
        // global variables
        if (analyzer.analize_var_def(child->var_def(), true))
          in_generator->gen_var_def(child->var_def(), true);
        break;
      default:
        break;
      }
    }

    // second pass
    for (auto child : src_node->source_code()->children) {
      switch (child->node_type) {
      case AstNodeType::AST_FN_DEF:
        if (analyzer.analize_fn_block(child->fn_def()->block, child->fn_def())) {
          bool block_no_error = in_generator->gen_fn_block(child->fn_def()->block->block(), child->fn_def());
          has_no_errors = has_no_errors && block_no_error;
        }
        break;

      default:
        continue;
      }
    }

    // file was analyzed
    src_node->is_analyzed = true;

    // after the file has been analyzed
    if (!stack.empty()) {
      // pop the loader file
      src_node = stack.top();
      stack.pop();
    } else {
      // was the main file
      src_node = nullptr;
    }

  } while (src_node != nullptr);

  if (!errors.empty()) {
    console::write_line("SEMANTIC/IR");
    _print_errors(errors);
    return false;
  }

  // generate IR output
  in_generator->flush();

  return has_no_errors;
}

void _print_errors(const std::vector<Error> &errors) {
  for (const Error &error : errors) {
    std::string str;
    to_string(error, str);
    console::write_line(str);
  }
}
