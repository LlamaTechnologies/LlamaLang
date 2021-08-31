#include "ir.hpp"

#include "ast_nodes.hpp"
#include "common_defs.hpp"
#include "console.hpp"
#include "lexer.hpp"

#include <iostream>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/IR/Verifier.h>
#include <stdarg.h>

static llvm::Constant *getConstantDefaultValue(const AstType &in_type, llvm::Type *in_llvm_type);

static const char *GetDataLayout() {
#ifdef LL_VISUALSTUDIO
  return "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128";
#else
  return "e-m:e-i64:64-f80:128-n8:16:32:64-S128";
#endif
}

static const char *GetTargetTriple() {
#ifdef LL_VISUALSTUDIO
  return "x86_64-pc-windows-msvc";
#else
  return "x86_64-pc-linux-gnu";
#endif
}

LlvmIrGenerator::LlvmIrGenerator(const std::string &_output_directory, const std::string &_executable_name)
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

void LlvmIrGenerator::gen_fn_proto(const AstFnProto &in_func_proto, AstFnDef *in_function) {
  // TODO(pablo96): Temp printf.
  if (in_func_proto.name == "printf") {
    _gen_printf_decl();
    return;
  }

  // Function return type
  llvm::Type *ret_type = _translate_type(in_func_proto.return_type->ast_type);

  // Function parameters
  auto node_params = in_func_proto.params;
  std::vector<llvm::Type *> parameters;
  for (auto param : node_params) {
    auto type = _translate_type(param->param_decl.type->ast_type);
    parameters.push_back(type);
  }

  // Function type
  llvm::FunctionType *fn_type =
    parameters.size() ? llvm::FunctionType::get(ret_type, parameters, false) : llvm::FunctionType::get(ret_type, false);

  // Function linkage type
  llvm::Function::LinkageTypes linkage_type = llvm::Function::LinkageTypes::LinkOnceODRLinkage;

  // Create the function
  llvm::Function *function =
    llvm::Function::Create(fn_type, linkage_type, std::string(in_func_proto.name), code_module);
  function->setCallingConv(llvm::CallingConv::C);

  // Add to symbols
  if (in_function) {
    in_function->llvm_value = function;
  }
}

bool LlvmIrGenerator::gen_fn_block(const AstBlock &in_func_block, AstFnDef &in_function) {
  // Create a new basic block to start insertion into.
  llvm::BasicBlock *BB = llvm::BasicBlock::Create(context, "", in_function.llvm_value);
  builder->SetInsertPoint(BB);

  // Store params in local variables to avoid problems
  int i = 0;
  auto args = in_function.llvm_value->args();
  auto it = args.begin();

  for (; it != args.end(); it++, i++) {
    auto var = builder->CreateAlloca(it->getType());
    builder->CreateStore(it, var);
    in_function.proto->function_proto.params.at(i)->param_decl.llvm_value = var;
  }

  // Genereate body and finish the function with the return value
  for (auto stmnt : in_func_block.statements) {
    switch (stmnt->node_type) {
    case AstNodeType::AST_VAR_DEF:
      gen_var_def(stmnt->var_def, false);
      break;
    case AstNodeType::AST_UNARY_EXPR:
      gen_unary_expr(stmnt->unary_expr);
      break;
    case AstNodeType::AST_BINARY_EXPR:
      gen_binary_expr(stmnt->binary_expr);
      break;
    case AstNodeType::AST_FN_CALL_EXPR:
      gen_fn_call_expr(stmnt->func_call);
      break;
    default:
      // ignore stmnt:
      ///// const_value;
      ///// symbol;
      break;
    }
  }

  if (in_function.proto->function_proto.return_type->ast_type.type_id == AstTypeId::VOID) {
    builder->CreateRetVoid();
  }

  std::string error_msg;
  auto llvm_output_file = llvm::raw_string_ostream(error_msg);
  // Validate the generated code, checking for consistency.
  if (llvm::verifyFunction(*in_function.llvm_value, &llvm_output_file)) {
    console::write_line();
    console::write_line("Error in generated function");
    console::write_line(error_msg);
    console::write_line();

    in_function.llvm_value->dump();

    console::write_line();

    // Error reading body, remove function.
    in_function.llvm_value->eraseFromParent();
    return false;
  }
  return true;
}

llvm::Value *LlvmIrGenerator::gen_expr(const AstNode *in_expr) {
  switch (in_expr->node_type) {
  case AstNodeType::AST_UNARY_EXPR:
    return gen_unary_expr(in_expr->unary_expr);
  case AstNodeType::AST_BINARY_EXPR:
    return gen_binary_expr(in_expr->binary_expr);
  case AstNodeType::AST_FN_CALL_EXPR:
    return gen_fn_call_expr(in_expr->func_call);
  case AstNodeType::AST_SYMBOL:
    return gen_symbol_expr(in_expr->symbol);
  case AstNodeType::AST_CONST_VALUE:
    return _translate_constant(in_expr->const_value);
  default:
    LL_UNREACHEABLE;
  }
}

void LlvmIrGenerator::gen_var_def(const AstVarDef &in_var_def, const bool is_global) {
  auto type = _translate_type(in_var_def.type->ast_type);
  std::string name = std::string(in_var_def.name);

  if (is_global) {
    code_module->getOrInsertGlobal(name, type);
    auto global_var = code_module->getNamedGlobal(name);
    in_var_def.llvm_value = global_var;

    llvm::Constant *init_value;
    if (in_var_def.initializer) {
      assert(in_var_def.initializer->node_type == AstNodeType::AST_CONST_VALUE);
      init_value = _translate_constant(in_var_def.initializer->const_value);
    } else {
      init_value = getConstantDefaultValue(in_var_def.type->ast_type, type);
    }

    global_var->setInitializer(init_value);
  } else {
    in_var_def.llvm_value = builder->CreateAlloca(type, nullptr, name);

    if (in_var_def.initializer) {
      llvm::Value *init_value = gen_expr(in_var_def.initializer);
      builder->CreateStore(init_value, in_var_def.llvm_value);
    }
  }
}

llvm::Value *LlvmIrGenerator::gen_unary_expr(const AstUnaryExpr &in_unary_expr) {
  // TODO(pablo96): Add support for fp. This only works for integer values.
  switch (in_unary_expr.op) {
  case UnaryExprType::RET: {
    if (in_unary_expr.expr) {
      auto ret_val = gen_expr(in_unary_expr.expr);
      return builder->CreateRet(ret_val);
    }
    return builder->CreateRetVoid();
  }
  case UnaryExprType::INC: {
    auto symbol_ref = gen_expr(in_unary_expr.expr);
    return builder->CreateAdd(symbol_ref, llvm::ConstantInt::get(context, llvm::APInt(128, 1)));
  }
  case UnaryExprType::DEC: {
    auto symbol_ref = gen_expr(in_unary_expr.expr);
    return builder->CreateSub(symbol_ref, llvm::ConstantInt::get(context, llvm::APInt(128, 1)));
  }
  case UnaryExprType::NEG: {
    auto symbol_ref = gen_expr(in_unary_expr.expr);
    return builder->CreateSub(symbol_ref, llvm::ConstantInt::get(context, llvm::APInt(128, 0)), "", false, true);
  }
  case UnaryExprType::NOT:
  case UnaryExprType::BIT_INV: {
    auto symbol_ref = gen_expr(in_unary_expr.expr);
    return builder->CreateNot(symbol_ref);
  }
  default:
    LL_UNREACHEABLE;
  }
}

llvm::Value *LlvmIrGenerator::gen_binary_expr(const AstBinaryExpr &in_binary_expr) {
  switch (in_binary_expr.bin_op) {
  case BinaryExprType::ASSIGN: {
    auto l_expr_llvm_value = in_binary_expr.left_expr->symbol.data->var_def.llvm_value;
    auto r_expr_llvm_value = gen_expr(in_binary_expr.right_expr);
    return builder->CreateStore(r_expr_llvm_value, l_expr_llvm_value);
  }
  case BinaryExprType::BIT_AND: {
    auto l_expr_llvm_value = gen_expr(in_binary_expr.left_expr);
    auto r_expr_llvm_value = gen_expr(in_binary_expr.right_expr);
    return builder->CreateAnd(l_expr_llvm_value, r_expr_llvm_value);
  }
  case BinaryExprType::BIT_XOR: {
    auto l_expr_llvm_value = gen_expr(in_binary_expr.left_expr);
    auto r_expr_llvm_value = gen_expr(in_binary_expr.right_expr);
    return builder->CreateOr(l_expr_llvm_value, r_expr_llvm_value);
  }
  case BinaryExprType::LSHIFT: {
    auto l_expr_llvm_value = gen_expr(in_binary_expr.left_expr);
    auto r_expr_llvm_value = gen_expr(in_binary_expr.right_expr);
    return builder->CreateShl(l_expr_llvm_value, r_expr_llvm_value);
  }
  case BinaryExprType::RSHIFT: {
    auto l_expr_llvm_value = gen_expr(in_binary_expr.left_expr);
    auto r_expr_llvm_value = gen_expr(in_binary_expr.right_expr);
    return builder->CreateLShr(l_expr_llvm_value, r_expr_llvm_value);
  }
  // TODO(pablo96): support fp operations
  case BinaryExprType::EQUALS: {
    auto l_expr_llvm_value = gen_expr(in_binary_expr.left_expr);
    auto r_expr_llvm_value = gen_expr(in_binary_expr.right_expr);
    return builder->CreateICmpEQ(l_expr_llvm_value, r_expr_llvm_value);
  }
  case BinaryExprType::NOT_EQUALS: {
    auto l_expr_llvm_value = gen_expr(in_binary_expr.left_expr);
    auto r_expr_llvm_value = gen_expr(in_binary_expr.right_expr);
    return builder->CreateICmpNE(l_expr_llvm_value, r_expr_llvm_value);
  }
  // TODO(pablo96): support unsigned operations
  case BinaryExprType::GREATER: {
    auto l_expr_llvm_value = gen_expr(in_binary_expr.left_expr);
    auto r_expr_llvm_value = gen_expr(in_binary_expr.right_expr);
    return builder->CreateICmpSGT(l_expr_llvm_value, r_expr_llvm_value);
  }
  case BinaryExprType::GREATER_OR_EQUALS: {
    auto l_expr_llvm_value = gen_expr(in_binary_expr.left_expr);
    auto r_expr_llvm_value = gen_expr(in_binary_expr.right_expr);
    return builder->CreateICmpSGE(l_expr_llvm_value, r_expr_llvm_value);
  }
  case BinaryExprType::LESS: {
    auto l_expr_llvm_value = gen_expr(in_binary_expr.left_expr);
    auto r_expr_llvm_value = gen_expr(in_binary_expr.right_expr);
    return builder->CreateICmpSLT(l_expr_llvm_value, r_expr_llvm_value);
  }
  case BinaryExprType::LESS_OR_EQUALS: {
    auto l_expr_llvm_value = gen_expr(in_binary_expr.left_expr);
    auto r_expr_llvm_value = gen_expr(in_binary_expr.right_expr);
    return builder->CreateICmpSLE(l_expr_llvm_value, r_expr_llvm_value);
  }
  // TODO(pablo96): support fp operations
  case BinaryExprType::ADD: {
    auto l_expr_llvm_value = gen_expr(in_binary_expr.left_expr);
    auto r_expr_llvm_value = gen_expr(in_binary_expr.right_expr);
    return builder->CreateAdd(l_expr_llvm_value, r_expr_llvm_value);
  }
  case BinaryExprType::SUB: {
    auto l_expr_llvm_value = gen_expr(in_binary_expr.left_expr);
    auto r_expr_llvm_value = gen_expr(in_binary_expr.right_expr);
    return builder->CreateSub(l_expr_llvm_value, r_expr_llvm_value);
  }
  case BinaryExprType::MUL: {
    auto l_expr_llvm_value = gen_expr(in_binary_expr.left_expr);
    auto r_expr_llvm_value = gen_expr(in_binary_expr.right_expr);
    return builder->CreateMul(l_expr_llvm_value, r_expr_llvm_value);
  }
  // TODO(pablo96): support fp operations and unsigned operations
  case BinaryExprType::DIV: {
    auto l_expr_llvm_value = gen_expr(in_binary_expr.left_expr);
    auto r_expr_llvm_value = gen_expr(in_binary_expr.right_expr);
    return builder->CreateSDiv(l_expr_llvm_value, r_expr_llvm_value);
  }
  case BinaryExprType::MOD: {
    auto l_expr_llvm_value = gen_expr(in_binary_expr.left_expr);
    auto r_expr_llvm_value = gen_expr(in_binary_expr.right_expr);
    return builder->CreateSRem(l_expr_llvm_value, r_expr_llvm_value);
  }
  default:
    LL_UNREACHEABLE;
  }
}

llvm::Value *LlvmIrGenerator::gen_symbol_expr(const AstSymbol &in_symbol) {
  switch (in_symbol.type) {
  case SymbolType::FUNC: {
    // TODO(pablo96): add support for storing function pointers
    return nullptr;
  }
  case SymbolType::VAR: {
    if (in_symbol.data->node_type == AstNodeType::AST_VAR_DEF)
      return builder->CreateLoad(in_symbol.data->var_def.llvm_value);
    if (in_symbol.data->node_type == AstNodeType::AST_PARAM_DECL)
      return builder->CreateLoad(in_symbol.data->param_decl.llvm_value);
  }
    LL_FALLTHROUGH
  default:
    LL_UNREACHEABLE;
  }
}

llvm::Value *LlvmIrGenerator::gen_fn_call_expr(const AstFnCallExpr &in_call_expr) {
  if (in_call_expr.fn_name == "printf") {
    std::vector<llvm::Value *> args;
    for (auto arg_node : in_call_expr.args) {
      auto arg_value = gen_expr(arg_node);
      args.push_back(arg_value);
    }
    return _gen_printf_call("%d\n", args);
  }

  auto llvm_fn = code_module->getFunction(std::string(in_call_expr.fn_name));
  if (!llvm_fn) {
    return nullptr;
  }

  std::vector<llvm::Value *> args;
  for (auto arg_node : in_call_expr.args) {
    auto arg_value = gen_expr(arg_node);
    args.push_back(arg_value);
  }

  return builder->CreateCall(llvm_fn, args);
}

void LlvmIrGenerator::flush() {
#ifndef LL_TESTS
  #ifdef _DEBUG
  console::write_line();
  code_module->dump();
  #endif
#endif

  std::error_code error_code;
  auto llvm_output_file = llvm::raw_fd_ostream(output_directory + "/" + output_file_name + ".bc", error_code);

  llvm::WriteBitcodeToFile(*code_module, llvm_output_file);

  llvm_output_file.flush();
  llvm_output_file.close();
}

llvm::Type *LlvmIrGenerator::_translate_type(const AstType &in_type) {
  switch (in_type.type_id) {
  case AstTypeId::VOID:
    return llvm::Type::getVoidTy(context);
  case AstTypeId::BOOL:
    return llvm::Type::getInt1Ty(context);
  case AstTypeId::INTEGER:
    switch (in_type.type_info.bit_size) {
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
      LL_UNREACHEABLE;
    }
    break;
  case AstTypeId::FLOATING_POINT:
    if (in_type.type_info.bit_size == 32)
      return llvm::Type::getFloatTy(context);
    if (in_type.type_info.bit_size == 64)
      return llvm::Type::getDoubleTy(context);
    if (in_type.type_info.bit_size == 128)
      return llvm::Type::getFP128Ty(context);
    LL_FALLTHROUGH
  default:
    LL_UNREACHEABLE;
  }
}

llvm::Constant *LlvmIrGenerator::_translate_constant(const AstConstValue &in_const) {
  switch (in_const.type) {
  case ConstValueType::BOOL:
    return in_const.boolean ? llvm::ConstantInt::getTrue(context) : llvm::ConstantInt::getFalse(context);
  case ConstValueType::INT: {
    llvm::IntegerType *llvm_ty = nullptr;
    switch (in_const.bit_size) {
    case 1:
      return strcmp(in_const.number, "0") == 0 ? llvm::ConstantInt::getFalse(context)
                                               : llvm::ConstantInt::getTrue(context);
    case 8:
      llvm_ty = llvm::Type::getInt8Ty(context);
      break;
    case 16:
      llvm_ty = llvm::Type::getInt16Ty(context);
      break;
    case 32:
      llvm_ty = llvm::Type::getInt32Ty(context);
      break;
    case 64:
      llvm_ty = llvm::Type::getInt64Ty(context);
      break;
    case 128:
      llvm_ty = llvm::Type::getInt128Ty(context);
      break;
    default:
      LL_UNREACHEABLE;
    }
    return llvm::ConstantInt::get(llvm_ty, in_const.number, 10);
  }
  case ConstValueType::FLOAT: {
    auto llvm_ty = llvm::Type::getInt128Ty(context);
    return llvm::ConstantFP::get(llvm_ty, in_const.number);
  }
  case ConstValueType::CHAR: {
    const uint32_t char_val = in_const.unicode_char;
    return llvm::ConstantInt::get(context, llvm::APInt(32, char_val));
  }
  default:
    // wrong token
    LL_UNREACHEABLE;
  }
}

llvm::Constant *getConstantDefaultValue(const AstType &in_type, llvm::Type *in_llvm_type) {
  switch (in_type.type_id) {
  case AstTypeId::BOOL:
  case AstTypeId::INTEGER:
    return llvm::ConstantInt::get(in_llvm_type, 0, in_type.type_info.is_signed);
  case AstTypeId::FLOATING_POINT:
    return llvm::ConstantFP::get(in_llvm_type, 0.0);
  case AstTypeId::VOID:
  default:
    LL_UNREACHEABLE;
  }
}

llvm::Value *LlvmIrGenerator::_gen_printf_decl() {
  std::vector<llvm::Type *> params;

  llvm::Type *pty = llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0);
  params.push_back(pty);

  llvm::FunctionType *fn_type = llvm::FunctionType::get(llvm::IntegerType::getInt32Ty(context), params, true);

  auto func_printf = llvm::Function::Create(fn_type, llvm::GlobalValue::ExternalLinkage, "printf", code_module);
  func_printf->setCallingConv(llvm::CallingConv::C);

  return func_printf;
}

llvm::Value *LlvmIrGenerator::_gen_printf_call(const char *format, const std::vector<llvm::Value *> &args) {
  llvm::Function *func_printf = code_module->getFunction("printf");

  llvm::Value *str = builder->CreateGlobalStringPtr(format);
  std::vector<llvm::Value *> call_params;
  call_params.reserve(args.size() + 1);
  call_params.push_back(str);
  call_params.insert(call_params.end(), args.begin(), args.end());

  return builder->CreateCall(func_printf, call_params);
}
