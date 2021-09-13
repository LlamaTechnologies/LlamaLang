#pragma once
#include "ast_nodes.hpp"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>
#include <string>

struct GeneratorInterface {
  virtual void gen_fn_proto(const AstFnProto *in_func_proto, AstFnDef *in_function) = 0;
  virtual bool gen_fn_block(const AstBlock *in_func_block, AstFnDef *in_function) = 0;
  virtual void gen_var_def(const AstVarDef *in_var_def, const bool is_global) = 0;
  virtual llvm::Value *gen_unary_expr(const AstUnaryExpr *in_unary_expr) = 0;
  virtual llvm::Value *gen_binary_expr(const AstBinaryExpr *in_binary_expr) = 0;
  virtual llvm::Value *gen_fn_call_expr(const AstFnCallExpr *in_call_expr) = 0;
  virtual llvm::Value *gen_expr(const AstNode *in_expr) = 0;
  virtual llvm::Value *gen_symbol_expr(const AstSymbol *in_symbol) = 0;
  virtual void flush() = 0;

  virtual ~GeneratorInterface() {}
};

struct VoidGenerator : public GeneratorInterface {
  void gen_fn_proto(const AstFnProto *in_func_proto, AstFnDef *in_function) override {}
  bool gen_fn_block(const AstBlock *in_func_block, AstFnDef *in_function) override { return true; }
  void gen_var_def(const AstVarDef *in_var_def, const bool is_global) override {}
  llvm::Value *gen_unary_expr(const AstUnaryExpr *in_unary_expr) override { return nullptr; }
  llvm::Value *gen_binary_expr(const AstBinaryExpr *in_binary_expr) override { return nullptr; }
  llvm::Value *gen_fn_call_expr(const AstFnCallExpr *in_call_expr) override { return nullptr; }
  llvm::Value *gen_expr(const AstNode *in_expr) override { return nullptr; }
  llvm::Value *gen_symbol_expr(const AstSymbol *in_symbol) override { return nullptr; }
  void flush() override {}
};

/*
 * Translates the AST to LLVM intermediate representation
 */
struct LlvmIrGenerator : public GeneratorInterface {
  LlvmIrGenerator(const std::string &_output_directory, const std::string &_executable_name);
  ~LlvmIrGenerator();

  void gen_fn_proto(const AstFnProto *in_func_proto, AstFnDef *in_function) override;
  bool gen_fn_block(const AstBlock *in_func_block, AstFnDef *in_function) override;
  void gen_var_def(const AstVarDef *in_var_def, const bool is_global) override;
  llvm::Value *gen_unary_expr(const AstUnaryExpr *in_unary_expr) override;
  llvm::Value *gen_binary_expr(const AstBinaryExpr *in_binary_expr) override;
  llvm::Value *gen_fn_call_expr(const AstFnCallExpr *in_call_expr) override;
  llvm::Value *gen_expr(const AstNode *in_expr) override;
  llvm::Value *gen_symbol_expr(const AstSymbol *in_symbol) override;
  void flush() override;

private:
  llvm::LLVMContext context;
  llvm::IRBuilder<> *builder;
  // outputs one llvm module per executable
  llvm::Module *code_module;

  const std::string &output_file_name;
  const std::string &output_directory;

  llvm::Value *_gen_if_stmnt(const AstIfStmnt *in_if_stmnt);
  llvm::BasicBlock *_gen_block(const AstBlock *in_block, llvm::BasicBlock *in_next_block);
  void _gen_stmnts(const std::vector<AstNode *> &in_stmnts);
  llvm::Value *_gen_printf_call(const char *format, const std::vector<llvm::Value *> &args);
  llvm::Value *_gen_printf_decl();
  llvm::Type *_translate_type(const AstType *in_type);
  llvm::Constant *_translate_constant(const AstConstValue *in_const);
};
