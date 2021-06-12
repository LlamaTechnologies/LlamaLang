#include "ir.hpp"
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/IR/Verifier.h>
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

void LlvmIrGenerator::generateFuncProto(const AstFuncProto& in_func_proto, AstFuncDef* in_function) {
    // Function return type
    llvm::Type* returnType = translateType(in_func_proto.return_type->ast_type);

    // Function parameters
    auto nodeParams = in_func_proto.params;
    std::vector<llvm::Type*> parameters;
    for (auto param : nodeParams) {
        auto type = translateType(param->param_decl.type->ast_type);
        parameters.push_back(type);
    }

    // Function type
    llvm::FunctionType* functionType = parameters.size()
        ? llvm::FunctionType::get(returnType, parameters, false)
        : llvm::FunctionType::get(returnType, false);

    // Function linkage type
    llvm::Function::LinkageTypes linkageType = llvm::Function::LinkageTypes::LinkOnceODRLinkage;

    // Create the function
    llvm::Function* function = llvm::Function::Create(functionType, linkageType, std::string(in_func_proto.name), code_module);
    function->setCallingConv(llvm::CallingConv::C);

    // Add to symbols
    if (in_function) {
        in_function->function = function;
    }
    
}

void LlvmIrGenerator::generateFuncBlock(const AstBlock& in_func_block, AstFuncDef& in_function) {
    // Create a new basic block to start insertion into.
    llvm::BasicBlock* BB = llvm::BasicBlock::Create(context, "entry", in_function.function);
    builder->SetInsertPoint(BB);

    // Genereate body and finish the function with the return value
    uint32_t bit_size = in_function.proto->function_proto.return_type->ast_type.type_info->bit_size;
    llvm::Value* retVal = llvm::ConstantInt::get(context, llvm::APInt(bit_size, std::stol("1"), false));;

    if (retVal)
        builder->CreateRet(retVal);
    else
        builder->CreateRetVoid();

    // Validate the generated code, checking for consistency.
    if (llvm::verifyFunction(*in_function.function)) {

        console::WriteLine();
        console::WriteLine("Error in generated function");
        console::WriteLine();

        in_function.function->dump();

        console::WriteLine();

        // Error reading body, remove function.
        in_function.function->eraseFromParent();
    }
}

void LlvmIrGenerator::generateVarDef(const AstVarDef& in_var_def, const bool is_global) {

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

llvm::Type* LlvmIrGenerator::translateType(AstType& in_type) {
    switch (in_type.type_id) {
    case AstTypeId::Void:
        return llvm::Type::getVoidTy(context);
    case AstTypeId::Bool:
        return llvm::Type::getInt1Ty(context);
    case AstTypeId::Integer:
        switch (in_type.type_info->bit_size) {
        case 8:
            return llvm::Type::getInt8Ty(context);
        case 16:
            return llvm::Type::getInt16Ty(context);
        case 32:
            return llvm::Type::getInt32Ty(context);
        case 64:
            return llvm::Type::getInt64Ty(context);
        default:
            UNREACHEABLE;
        } break;
    case AstTypeId::FloatingPoint:
        if (in_type.type_info->bit_size == 32)
            return llvm::Type::getFloatTy(context);
        if (in_type.type_info->bit_size == 64)
            return llvm::Type::getDoubleTy(context);
        LL_FALLTHROUGH
    default:
        UNREACHEABLE;
    }
}
