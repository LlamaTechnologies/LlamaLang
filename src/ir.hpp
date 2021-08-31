#pragma once
#include "ast_nodes.hpp"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>
#include <string>

/*
 * Translates the AST to LLVM intermediate representation
 */
class LlvmIrGenerator {
  llvm::LLVMContext context;
  llvm::IRBuilder<> *builder;
  // outputs one llvm module per executable
  llvm::Module *code_module;

  const std::string &output_file_name;
  const std::string &output_directory;

public:
  LlvmIrGenerator(const std::string &_output_directory, const std::string &_executable_name);
  ~LlvmIrGenerator();

  void generateFuncProto(const AstFnProto &in_func_proto, AstFnDef *in_function);
  bool generateFuncBlock(const AstBlock &in_func_block, AstFnDef &in_function);
  void generateVarDef(const AstVarDef &in_var_def, const bool is_global);
  llvm::Value *generateUnaryExpr(const AstUnaryExpr &in_unary_expr);
  llvm::Value *generateBinaryExpr(const AstBinaryExpr &in_binary_expr);
  llvm::Value *generateFuncCallExpr(const AstFnCallExpr &in_call_expr);
  llvm::Value *generateExpr(const AstNode *in_expr);
  llvm::Value *generateSymbolExpr(const AstSymbol &in_symbol);
  void flush();

private:
  llvm::Value *generatePrintfCall(const char *format, const std::vector<llvm::Value *> &args);
  llvm::Value *generatePrintfDeclaration();
  llvm::Type *translateType(const AstType &in_type);
  llvm::Constant *translateConstant(const AstConstValue &in_const);
};
