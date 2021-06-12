#pragma once
#include <string>
#include <llvm/IR/IRBuilder.h>
#include "ast_nodes.hpp"


/*
* Translates the AST to LLVM intermediate representation
*/
class LlvmIrGenerator {
    llvm::LLVMContext   context;
    llvm::IRBuilder<>*  builder;
    // outputs one llvm module per executable
    llvm::Module*       code_module;

    const std::string&  output_file_name;
    const std::string&  output_directory;

public:
    LlvmIrGenerator(const std::string& _output_directory, const std::string& _executable_name);
    ~LlvmIrGenerator();
    
    void generateFuncProto(const AstFuncProto& in_func_proto, AstFuncDef* in_function);
    void generateFuncBlock(const AstBlock& in_func_block, AstFuncDef& in_function);
    void generateVarDef(const AstVarDef& in_var_def, const bool is_global);

    void flush();

private:
    llvm::Type* translateType(AstType& in_type);
};
