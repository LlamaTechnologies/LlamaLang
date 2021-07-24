#include "ir.hpp"
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/IR/Verifier.h>
#include "console.hpp"
#include "lexer.hpp"
#include "common_defs.hpp"

static llvm::Constant* getConstantDefaultValue(const AstType& in_type, llvm::Type* in_llvm_type);

static const char* GetDataLayout() {
#ifdef LL_VISUALSTUDIO
    return "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128";
#else
    return "e-m:e-i64:64-f80:128-n8:16:32:64-S128";
#endif
}

static const char * GetTargetTriple() {
#ifdef LL_VISUALSTUDIO
    return "x86_64-pc-windows-msvc19.28.29913";
#else
    return "x86_64-pc-linux-gnu";
#endif

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

bool LlvmIrGenerator::generateFuncBlock(const AstBlock& in_func_block, AstFuncDef& in_function) {
    // Create a new basic block to start insertion into.
    llvm::BasicBlock* BB = llvm::BasicBlock::Create(context, "entry", in_function.function);
    builder->SetInsertPoint(BB);

    // Store params in local variables to avoid problems
    int i = 0;
    auto args = in_function.function->args();
    auto it = args.begin();

    for(; it != args.end(); it++, i++) {
        auto var = builder->CreateAlloca(it->getType());
        builder->CreateStore(it, var);
        in_function.proto->function_proto.params.at(i)->param_decl.llvm_value = var;
    }

    // Genereate body and finish the function with the return value
    for (auto stmnt : in_func_block.statements) {
        switch (stmnt->node_type) {
        case AstNodeType::AstVarDef:
            generateVarDef(stmnt->var_def, false);
            break;
        case AstNodeType::AstUnaryExpr:
            generateUnaryExpr(stmnt->unary_expr);
            break;
        case AstNodeType::AstBinaryExpr:
            generateBinaryExpr(stmnt->binary_expr);
            break;
        case AstNodeType::AstFuncCallExpr:
            generateFuncCallExpr(stmnt->func_call);
            break;
        default: 
            // ignore stmnt:
            ///// const_value;
            ///// symbol;
            break;
        }
    }

    // Validate the generated code, checking for consistency.
    if (llvm::verifyFunction(*in_function.function)) {

        console::WriteLine();
        console::WriteLine("Error in generated function");
        console::WriteLine();

        in_function.function->dump();

        console::WriteLine();

        // Error reading body, remove function.
        in_function.function->eraseFromParent();
        return false;
    }
    return true;
}

llvm::Value* LlvmIrGenerator::generateExpr(const AstNode* in_expr) {
    switch (in_expr->node_type) {
    case AstNodeType::AstUnaryExpr:
        return generateUnaryExpr(in_expr->unary_expr);
    case AstNodeType::AstBinaryExpr:
        return generateBinaryExpr(in_expr->binary_expr);
    case AstNodeType::AstFuncCallExpr:
        return generateFuncCallExpr(in_expr->func_call);
    case AstNodeType::AstSymbol:
        return generateSymbolExpr(in_expr->symbol);
    case AstNodeType::AstConstValue:
        return translateConstant(in_expr->const_value);
    default:
        UNREACHEABLE;
    }
}

void LlvmIrGenerator::generateVarDef(const AstVarDef& in_var_def, const bool is_global) {
    auto type = translateType(in_var_def.type->ast_type);
    std::string name = std::string(in_var_def.name);

    if (is_global) {
        code_module->getOrInsertGlobal(name, type);
        auto globalVar = code_module->getNamedGlobal(name);
        in_var_def.llvm_value = globalVar;

        auto assignStmntNode = in_var_def.initializer;
        llvm::Constant* init_value;
        
        if (assignStmntNode) {
            auto& assignStmnt = assignStmntNode->binary_expr;
            assert(assignStmnt.bin_op == BinaryExprType::ASSIGN);
            init_value = translateConstant(assignStmnt.right_expr->const_value);
        } else {
            init_value = getConstantDefaultValue(in_var_def.type->ast_type, type);
        }

        globalVar->setInitializer(init_value);
    }
    else {
        auto* varInst = builder->CreateAlloca(type, nullptr, name);
        in_var_def.llvm_value = varInst;

        auto assignStmntNode = in_var_def.initializer;

        if (assignStmntNode) {
            auto& assignStmnt = assignStmntNode->binary_expr;
            assert(assignStmnt.bin_op == BinaryExprType::ASSIGN);
            //generateBinaryExpr(assignStmnt);
        }
    }
}

llvm::Value* LlvmIrGenerator::generateUnaryExpr(const AstUnaryExpr& in_unary_expr) {
    // TODO(pablo96): Add support for fp. This only works for integer values.
    switch (in_unary_expr.op)
    {
    case UnaryExprType::RET: {
        if (in_unary_expr.expr) {
            auto retval = generateExpr(in_unary_expr.expr);
            return builder->CreateRet(retval);
        }
        return builder->CreateRetVoid();
    }
    case UnaryExprType::INC: {
        auto symbol_ref = generateExpr(in_unary_expr.expr);
        return builder->CreateAdd(symbol_ref, llvm::ConstantInt::get(context, llvm::APInt(128, 1)));
    }
    case UnaryExprType::DEC: {
        auto symbol_ref = generateExpr(in_unary_expr.expr);
        return builder->CreateSub(symbol_ref, llvm::ConstantInt::get(context, llvm::APInt(128, 1)));
    }
    case UnaryExprType::NEG: {
        auto symbol_ref = generateExpr(in_unary_expr.expr);
        return builder->CreateSub(symbol_ref, llvm::ConstantInt::get(context, llvm::APInt(128, 0)), "", false, true);
    }
    case UnaryExprType::NOT:
    case UnaryExprType::BIT_INV: {
        auto symbol_ref = generateExpr(in_unary_expr.expr);
        return builder->CreateNot(symbol_ref);
    }
    default:
        UNREACHEABLE;
    }
}

llvm::Value* LlvmIrGenerator::generateBinaryExpr(const AstBinaryExpr& in_binary_expr) {
    switch(in_binary_expr.bin_op) {
        case BinaryExprType::ASSIGN: {
            auto l_expr_llvm_value = in_binary_expr.left_expr->symbol.data->var_def.llvm_value;
            auto r_expr_llvm_value = generateExpr(in_binary_expr.right_expr);
            return builder->CreateStore(r_expr_llvm_value, l_expr_llvm_value);
        }
        case BinaryExprType::BIT_AND: {
            auto l_expr_llvm_value = generateExpr(in_binary_expr.left_expr);
            auto r_expr_llvm_value = generateExpr(in_binary_expr.right_expr);
            return builder->CreateAnd(l_expr_llvm_value, r_expr_llvm_value);
        }
        case BinaryExprType::BIT_XOR: {
            auto l_expr_llvm_value = generateExpr(in_binary_expr.left_expr);
            auto r_expr_llvm_value = generateExpr(in_binary_expr.right_expr);
            return builder->CreateOr(l_expr_llvm_value, r_expr_llvm_value);
        }
        case BinaryExprType::LSHIFT: {
            auto l_expr_llvm_value = generateExpr(in_binary_expr.left_expr);
            auto r_expr_llvm_value = generateExpr(in_binary_expr.right_expr);
            return builder->CreateShl(l_expr_llvm_value, r_expr_llvm_value);
        }
        case BinaryExprType::RSHIFT: {
            auto l_expr_llvm_value = generateExpr(in_binary_expr.left_expr);
            auto r_expr_llvm_value = generateExpr(in_binary_expr.right_expr);
            return builder->CreateLShr(l_expr_llvm_value, r_expr_llvm_value);
        }
        // TODO(pablo96): support fp operations
        case BinaryExprType::EQUALS: {
            auto l_expr_llvm_value = generateExpr(in_binary_expr.left_expr);
            auto r_expr_llvm_value = generateExpr(in_binary_expr.right_expr);
            return builder->CreateICmpEQ(l_expr_llvm_value, r_expr_llvm_value);
        }
        case BinaryExprType::NOT_EQUALS: {
            auto l_expr_llvm_value = generateExpr(in_binary_expr.left_expr);
            auto r_expr_llvm_value = generateExpr(in_binary_expr.right_expr);
            return builder->CreateICmpNE(l_expr_llvm_value, r_expr_llvm_value);
        }
        // TODO(pablo96): support unsigned operations
        case BinaryExprType::GREATER: {
            auto l_expr_llvm_value = generateExpr(in_binary_expr.left_expr);
            auto r_expr_llvm_value = generateExpr(in_binary_expr.right_expr);
            return builder->CreateICmpSGT(l_expr_llvm_value, r_expr_llvm_value);
        }
        case BinaryExprType::GREATER_OR_EQUALS:  {
            auto l_expr_llvm_value = generateExpr(in_binary_expr.left_expr);
            auto r_expr_llvm_value = generateExpr(in_binary_expr.right_expr);
            return builder->CreateICmpSGE(l_expr_llvm_value, r_expr_llvm_value);
        }
        case BinaryExprType::LESS: {
            auto l_expr_llvm_value = generateExpr(in_binary_expr.left_expr);
            auto r_expr_llvm_value = generateExpr(in_binary_expr.right_expr);
            return builder->CreateICmpSLT(l_expr_llvm_value, r_expr_llvm_value);
        }
        case BinaryExprType::LESS_OR_EQUALS: {
            auto l_expr_llvm_value = generateExpr(in_binary_expr.left_expr);
            auto r_expr_llvm_value = generateExpr(in_binary_expr.right_expr);
            return builder->CreateICmpSLE(l_expr_llvm_value, r_expr_llvm_value);
        }
        // TODO(pablo96): support fp operations
        case BinaryExprType::ADD: {
            auto l_expr_llvm_value = generateExpr(in_binary_expr.left_expr);
            auto r_expr_llvm_value = generateExpr(in_binary_expr.right_expr);
            return builder->CreateAdd(l_expr_llvm_value, r_expr_llvm_value);
        }
        case BinaryExprType::SUB:{
            auto l_expr_llvm_value = generateExpr(in_binary_expr.left_expr);
            auto r_expr_llvm_value = generateExpr(in_binary_expr.right_expr);
            return builder->CreateSub(l_expr_llvm_value, r_expr_llvm_value);
        }
        case BinaryExprType::MUL: {
            auto l_expr_llvm_value = generateExpr(in_binary_expr.left_expr);
            auto r_expr_llvm_value = generateExpr(in_binary_expr.right_expr);
            return builder->CreateMul(l_expr_llvm_value, r_expr_llvm_value);
        }
        // TODO(pablo96): support fp operations and unsigned operations
        case BinaryExprType::DIV: {
            auto l_expr_llvm_value = generateExpr(in_binary_expr.left_expr);
            auto r_expr_llvm_value = generateExpr(in_binary_expr.right_expr);
            return builder->CreateSDiv(l_expr_llvm_value, r_expr_llvm_value);
        }
        case BinaryExprType::MOD: {
            auto l_expr_llvm_value = generateExpr(in_binary_expr.left_expr);
            auto r_expr_llvm_value = generateExpr(in_binary_expr.right_expr);
            return builder->CreateSRem(l_expr_llvm_value, r_expr_llvm_value);
        }
        default:
            UNREACHEABLE;
    }
}

llvm::Value* LlvmIrGenerator::generateSymbolExpr(const AstSymbol& in_symbol) {
    switch (in_symbol.type) {
    case SymbolType::FUNC: {
        return nullptr;
    }
    case SymbolType::VAR: {
        if (in_symbol.data->node_type == AstNodeType::AstVarDef)
            return builder->CreateLoad(in_symbol.data->var_def.llvm_value);
        if (in_symbol.data->node_type == AstNodeType::AstParamDecl)
            return builder->CreateLoad(in_symbol.data->param_decl.llvm_value);
    }
    LL_FALLTHROUGH
    default: 
        UNREACHEABLE;
    }
}

llvm::Value* LlvmIrGenerator::generateFuncCallExpr(const AstFuncCallExpr& in_call_expr) {
    auto llvm_fn = code_module->getFunction(in_call_expr.fn_name);
    
    std::vector<llvm::Value*> args;
    for (auto arg_node : in_call_expr.params) {
        auto arg_value = generateExpr(arg_node);
        args.push_back(arg_value);
    }

    return builder->CreateCall(llvm_fn, args);
}

void LlvmIrGenerator::flush() {
#ifndef LL_TESTS
#ifdef _DEBUG
    console::WriteLine();
    code_module->dump();
#endif
#endif

    std::error_code error_code;
    auto llvm_output_file = llvm::raw_fd_ostream(output_directory + output_file_name + ".bc", error_code);

    llvm::WriteBitcodeToFile(*code_module, llvm_output_file);

    llvm_output_file.flush();
    llvm_output_file.close();
}

llvm::Type* LlvmIrGenerator::translateType(const AstType& in_type) {
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
        case 128:
            return llvm::Type::getInt128Ty(context);
        default:
            UNREACHEABLE;
        } break;
    case AstTypeId::FloatingPoint:
        if (in_type.type_info->bit_size == 32)
            return llvm::Type::getFloatTy(context);
        if (in_type.type_info->bit_size == 64)
            return llvm::Type::getDoubleTy(context);
        if (in_type.type_info->bit_size == 128)
            return llvm::Type::getFP128Ty(context);
        LL_FALLTHROUGH
    default:
        UNREACHEABLE;
    }
}

llvm::Constant* LlvmIrGenerator::translateConstant(const AstConstValue& in_const) {
    ConstValueType r_value_type = in_const.type;
    if (r_value_type == ConstValueType::INT) {
        const BigInt& int_val = in_const.integer;
        return llvm::ConstantInt::get(context, llvm::APInt(32, 0));
    }
    else if (r_value_type == ConstValueType::FLOAT) {
        const BigFloat& float_val = in_const.floating_point;
        return llvm::ConstantFP::get(context, llvm::APFloat(0.0f));
    }
    else if (r_value_type == ConstValueType::CHAR) {
        const uint32_t char_val = in_const.unicode_char;
        return llvm::ConstantInt::get(context, llvm::APInt(32, 0));
    }
 
    // wrong token
    UNREACHEABLE;
}

llvm::Constant* getConstantDefaultValue(const AstType& in_type, llvm::Type* in_llvm_type) {
    switch (in_type.type_id) {
    case AstTypeId::Bool:
    case AstTypeId::Integer:
            return llvm::ConstantInt::get(in_llvm_type, 0, in_type.type_info->is_signed);
    case AstTypeId::FloatingPoint:
            return llvm::ConstantFP::get(in_llvm_type, 0.0);
    case AstTypeId::Void:
    default:
        UNREACHEABLE;
    }
}

/*
void kprintf(Module *mod, BasicBlock *bb, const char *format, ...)
{
    Function *func_printf = mod->getFunction("printf");
    if (!func_printf) {
        PointerType *Pty = PointerType::get(IntegerType::get(mod->getContext(), 8), 0);
        FunctionType *FuncTy9 = FunctionType::get(IntegerType::get(mod->getContext(), 32), true);

        func_printf = Function::Create(FuncTy9, GlobalValue::ExternalLinkage, "printf", mod);
        func_printf->setCallingConv(CallingConv::C);

        AttrListPtr func_printf_PAL;
        func_printf->setAttributes(func_printf_PAL);
    }

    IRBuilder <> builder(mod->getContext());
    builder.SetInsertPoint(bb);

    Value *str = builder.CreateGlobalStringPtr(format);
    std::vector <Value *> int32_call_params;
    int32_call_params.push_back(str);

    va_list ap;
    va_start(ap, format);

    char *str_ptr = va_arg(ap, char*);
    Value *format_ptr = builder.CreateGlobalStringPtr(str_ptr);
    int32_call_params.push_back(format_ptr);

    std::vector<llvm::Value*> extra;
    do {
        llvm::Value *op = va_arg(ap, llvm::Value*);
        if (op) {
            int32_call_params.push_back(op);
        } else {
            break;
        }
    } while (1);
    va_end(ap);

    CallInst * int32_call = CallInst::Create(func_printf, int32_call_params, "call", bb);
}
#define oprintf(...) kprintf(__VA_ARGS__)
#define llvm_printf(...) oprintf(mod, bb, __VA_ARGS__, NULL)

llvm_printf("Output: 0x%08X %f %d\n", 0x12345678, 3.1415926, 12345);
*/