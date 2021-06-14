#include "compiler.hpp"
#include "ir.hpp"
#include "semantic_analyzer.hpp"

void compiler::compile(const std::string& in_output_directory, const std::string& in_executable_name, AstNode* in_source_code_node, std::vector<Error>& errors) {
    assert(in_source_code_node->node_type == AstNodeType::AstSourceCode);
    static LlvmIrGenerator generator(in_output_directory, in_executable_name);
    static SemanticAnalyzer analyzer(errors);

    // first pass
    for (auto child : in_source_code_node->source_code.children) {
        switch (child->node_type) {
        case AstNodeType::AstFuncDef:
            if (analyzer.analizeFuncProto(child->function_def.proto->function_proto, &child->function_def))
                generator.generateFuncProto(child->function_def.proto->function_proto, &child->function_def);
            break;
        case AstNodeType::AstFuncProto:
            if (analyzer.analizeFuncProto(child->function_proto, nullptr))
                generator.generateFuncProto(child->function_proto, nullptr);
            break;
        case AstNodeType::AstVarDef:
            // global variables
            if (analyzer.analizeVarDef(child, true))
                generator.generateVarDef(child->var_def, true);
            break;
        default:
            break;
        }
    }

    // second pass
    for (auto child : in_source_code_node->source_code.children) {
        switch (child->node_type) {
        case AstNodeType::AstFuncDef:
            if (analyzer.analizeFuncBlock(child->function_def.block->block, child->function_def))
                generator.generateFuncBlock(child->function_def.block->block, child->function_def);
            break; 
        default:
            continue;
        }
    }

    // generate IR output
    generator.flush();

    // generate exe|lib|dll output
}
