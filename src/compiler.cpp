#include "compiler.hpp"
#include "ir.hpp"

void compiler::compile(const std::string& in_output_directory, const std::string& in_executable_name, AstNode* in_source_code_node) {
    assert(in_source_code_node->node_type == AstNodeType::AstSourceCode);
    static LlvmIrGenerator generator(in_output_directory, in_executable_name);

    // first pass
    for (auto child : in_source_code_node->source_code.children) {
        switch (child->node_type) {
        case AstNodeType::AstFuncDef:
            //if (analyzer.analizeFuncProto(child->function_def.proto->function_proto))
            generator.generateFuncProto(child->function_def.proto->function_proto, &child->function_def);
            break;
        case AstNodeType::AstFuncProto:
            //if (analyzer.analizeFuncProto(child->function_proto))
            generator.generateFuncProto(child->function_proto, nullptr);
            break;
        case AstNodeType::AstVarDef:
            // global variables
            //if (analyzer.analizeVarDef(child->var_def))
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
            //if (analyzer.analizeFuncBlock(child->function_def.block->block))
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
