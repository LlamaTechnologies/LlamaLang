#pragma once
#include "../common_defs.hpp"
#include "../code_gen/ir.hpp"

#include <string>
#include <unordered_map>

namespace llvm {
struct Function;
}

struct BuiltInsRepository {
  static BuiltInsRepository &get(LlvmIrGenerator *in_ir);

  llvm::Function *getFnIR(const std::string &in_fn_name);
  LL_NODISCARD bool isBuiltInFn(const std::string &in_fn_name);

private:
  std::unordered_map<std::string, llvm::Function *> _fn_irs;

  llvm::LLVMContext &context;
  llvm::Module *code_module;
  LlvmIrGenerator *ir;

  BuiltInsRepository(LlvmIrGenerator *in_ir);
  void _set_fn_irs();
  llvm::Function *_create_fn_ir(std::string in_name, std::string ret_type, std::vector<std::string> in_params);
};
