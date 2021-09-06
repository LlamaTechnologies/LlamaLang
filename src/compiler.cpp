#include "compiler.hpp"

#include "console.hpp"
#include "ir.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "semantic_analyzer.hpp"
#include "src_code_repository.hpp"

static void _print_errors(const std::vector<Error> &errors);
static bool _recursive_file_analysis_and_ir(SemanticAnalyzer &in_analyzer, GeneratorInterface *in_generator,
                                            AstSourceCode *const in_source_code);

bool compiler::compile(const std::string &in_input_directory, const std::string &in_output_directory,
                       const std::string &in_executable_name, const std::string &in_source_code,
                       const std::string &in_source_name) {
  LL_ASSERT(in_source_code.size() > 0);
  LL_ASSERT(in_source_name.size() > 0);

  std::vector<Error> errors;

  Lexer lexer(in_source_code, in_input_directory, in_source_name, errors);
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
  return compile_node(&generator, errors, source_code_node);
}

bool compiler::compile_node(GeneratorInterface *in_generator, std::vector<Error> &errors,
                            AstSourceCode *const in_source_code) {
  LL_ASSERT(in_generator != nullptr);
  LL_ASSERT(in_source_code != nullptr);

  SemanticAnalyzer analyzer(errors);

  _recursive_file_analysis_and_ir(analyzer, in_generator, in_source_code);

  if (!errors.empty()) {
    console::write_line("SEMANTIC/IR");
    _print_errors(errors);
    return false;
  }

  in_generator->flush();

  return true;
}

bool _recursive_file_analysis_and_ir(SemanticAnalyzer &in_analyzer, GeneratorInterface *in_generator,
                                     AstSourceCode *const in_source_code) {
  bool has_no_errors = true;
  // first pass
  for (int32_t i = 0; i < in_source_code->source_code()->children.size(); ++i) {
    AstNode *child = in_source_code->source_code()->children.at(i);

    switch (child->node_type) {
    case AstNodeType::AST_DIRECTIVE: {
      AstDirective *directive_node = child->directive();
      // if it is a load directive we should analyze all the source code.
      if (directive_node->directive_type == DirectiveType::LOAD) {
        const std::string &file_name = directive_node->argument.str;
        RepositorySrcCode code_repository = RepositorySrcCode::get();
        // if we have found and parsed the file...
        if (code_repository.has_file(file_name)) {
          AstSourceCode *new_src_node = code_repository.get_source_code(file_name);
          // if it is not already analyzed we procced to do so
          if (new_src_node->is_analyzed == false) {
            has_no_errors = has_no_errors && _recursive_file_analysis_and_ir(in_analyzer, in_generator, new_src_node);
          }
        } // if loaded file
      }   // if load directive
    } break;
    case AstNodeType::AST_FN_DEF:
      if (in_analyzer.analize_fn_proto(child->fn_def()->proto))
        in_generator->gen_fn_proto(child->fn_def()->proto->fn_proto(), child->fn_def());
      break;
    case AstNodeType::AST_FN_PROTO:
      if (in_analyzer.analize_fn_proto(child->fn_proto()))
        in_generator->gen_fn_proto(child->fn_proto(), nullptr);
      break;
    case AstNodeType::AST_VAR_DEF:
      // global variables
      if (in_analyzer.analize_var_def(child->var_def(), true))
        in_generator->gen_var_def(child->var_def(), true);
      break;
    default:
      break;
    }
  }

  // second pass
  for (auto child : in_source_code->source_code()->children) {
    switch (child->node_type) {
    case AstNodeType::AST_FN_DEF:
      if (in_analyzer.analize_fn_block(child->fn_def()->block, child->fn_def())) {
        bool block_no_error = in_generator->gen_fn_block(child->fn_def()->block->block(), child->fn_def());
        has_no_errors = has_no_errors && block_no_error;
      }
      break;

    default:
      continue;
    }
  }

  // file was analyzed
  in_source_code->is_analyzed = true;

  return has_no_errors;
}

void _print_errors(const std::vector<Error> &errors) {
  for (const Error &error : errors) {
    std::string str;
    to_string(error, str);
    console::write_line(str);
  }
}
