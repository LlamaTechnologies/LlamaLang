#include "builtins.hpp"

#include "../ast/ast_nodes.hpp"
#include "types.hpp"

static BuiltInsRepository *repository = nullptr;

BuiltInsRepository::BuiltInsRepository(LlvmIrGenerator *in_ir)
    : context(in_ir->context), code_module(in_ir->code_module), ir(in_ir) {}

BuiltInsRepository &BuiltInsRepository::get(LlvmIrGenerator *in_ir) {
  if (repository == nullptr) {
    repository = new BuiltInsRepository(in_ir);
    repository->_set_fn_irs();
  }
  return *repository;
}

llvm::Function *BuiltInsRepository::getFnIR(const std::string &in_fn_name) { return _fn_irs.at(in_fn_name); }

bool BuiltInsRepository::isBuiltInFn(const std::string &in_fn_name) {
  return _fn_irs.find(in_fn_name) != _fn_irs.end();
}

void BuiltInsRepository::_set_fn_irs() {
  std::string malloc_name = "malloc";
  llvm::Function *malloc_fn = _create_fn_ir(malloc_name, "*void", { "u64" });
  _fn_irs.emplace(malloc_name, malloc_fn);

  std::string memset_name = "memset";
  llvm::Function *memset_fn = _create_fn_ir(memset_name, "*void", { "*void", "s32", "u64" });
  _fn_irs.emplace(memset_name, memset_fn);

  std::string free_name = "free";
  llvm::Function *free_fn = _create_fn_ir(free_name, "void", { "*void" });
  _fn_irs.emplace(free_name, free_fn);
}

llvm::Function *BuiltInsRepository::_create_fn_ir(std::string in_name, std::string in_ret_type_name,
                                                  std::vector<std::string> in_params) {
  TypesRepository &type_repo = TypesRepository::get();

  std::vector<llvm::Type *> params;
  for (auto &param_type_name : in_params) {
    AstType *param_type_node = type_repo.get_type_node(param_type_name);
    llvm::Type *param_type = ir->_translate_type(param_type_node);
    params.push_back(param_type);
  }

  AstType *ret_type_node = type_repo.get_type_node(in_ret_type_name);
  llvm::Type *ret_type = ir->_translate_type(ret_type_node);
  llvm::FunctionType *fn_type = llvm::FunctionType::get(ret_type, params, false);

  llvm::Function *func_printf =
    llvm::Function::Create(fn_type, llvm::GlobalValue::ExternalLinkage, in_name, this->code_module);
  func_printf->setCallingConv(llvm::CallingConv::C);

  return func_printf;
}
