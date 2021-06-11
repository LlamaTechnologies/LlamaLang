#include "ir.hpp"
#include <llvm/Bitcode/BitcodeWriter.h>
#include "console.hpp"

static const char* GetDataLayout() {
    return "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128";
}

static const char * GetTargetTriple() {
    return "x86_64-pc-windows-msvc19.28.29913";
}

LlvmIrGenerator::LlvmIrGenerator(const std::string& _output_directory, const std::string& _executable_name) 
: output_file_name(_executable_name), output_directory(_output_directory) {
    // create IR builder helper
    builder = new llvm::IRBuilder<>(context);
    // Make the module, which holds all the code.
    code_module = new llvm::Module(output_file_name, context);

    auto dl = llvm::DataLayout(GetDataLayout());
    code_module->setDataLayout(dl);
    code_module->setTargetTriple(GetTargetTriple());
}

LlvmIrGenerator::~LlvmIrGenerator() {
    delete builder;
    delete code_module;
}

void LlvmIrGenerator::generate(const AstNode* in_node) {
    switch (in_node->node_type) {
    case AstNodeType::AstFuncProto:
        generateFuncProto(in_node->function_proto);
        break;
    default:
        // not implemented
        break;
        // production compiler should never get here
        //UNREACHEABLE;
    }
}

void LlvmIrGenerator::flush() {
#ifdef _DEBUG
    console::WriteLine();
    code_module->dump();
#endif

    std::error_code error_code;
    auto llvm_output_file = llvm::raw_fd_ostream(output_directory + output_file_name + ".bc", error_code);
    
    llvm::WriteBitcodeToFile(*code_module, llvm_output_file);

    llvm_output_file.flush();
    llvm_output_file.close();
}

void LlvmIrGenerator::generateFuncProto(const AstFuncProto& in_funcProto) {
}


