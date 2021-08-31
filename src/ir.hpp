#pragma once
#include "ast_nodes.hpp"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>
#include <string>

/*
 * Translates the AST to LLVM intermediate representation
 */
struct LlvmIrGenerator {
  LlvmIrGenerator(const std::string &_output_directory, const std::string &_executable_name);
  ~LlvmIrGenerator();

  void gen_fn_proto(const AstFnProto &in_func_proto, AstFnDef *in_function);
  bool gen_fn_block(const AstBlock &in_func_block, AstFnDef &in_function);
  void gen_var_def(const AstVarDef &in_var_def, const bool is_global);
  llvm::Value *gen_unary_expr(const AstUnaryExpr &in_unary_expr);
  llvm::Value *gen_binary_expr(const AstBinaryExpr &in_binary_expr);
  llvm::Value *gen_fn_call_expr(const AstFnCallExpr &in_call_expr);
  llvm::Value *gen_expr(const AstNode *in_expr);
  llvm::Value *gen_symbol_expr(const AstSymbol &in_symbol);
  void flush();

private:
  llvm::LLVMContext context;
  llvm::IRBuilder<> *builder;
  // outputs one llvm module per executable
  llvm::Module *code_module;

  const std::string &output_file_name;
  const std::string &output_directory;

  llvm::Value *_gen_printf_call(const char *format, const std::vector<llvm::Value *> &args);
  llvm::Value *_gen_printf_decl();
  llvm::Type *_translate_type(const AstType &in_type);
  llvm::Constant *_translate_constant(const AstConstValue &in_const);
};
