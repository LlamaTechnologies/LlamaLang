#include "IR.hpp"
#include "Console.hpp"
#include "ast/FunctionNode.hpp"

using namespace llang;
using namespace llang::IR;

static void *llvm_module;
static void *llvm_builder;

void Translate(std::shared_ptr<ast::ProgramNode> program) {
  llvm_module = LLVM.ModuleCreateWithName(program->FileName);
  llvm_builder = LLVM.CreateBuilder();

  for (auto node : program->children) {
    auto nodeType = node->GetType();
    switch (nodeType) {
    case ast::AST_TYPE::FunctionNode:
      auto funcNode = CastNode<ast::FunctionNode>(node);
      TranslateNode(funcNode);
      break;
    default:
      Console::WriteLine(ast::GetAstTypeName(nodeType) + " Not implemented");
      break;
    }
  }

  LLVM.DumpModule(llvm_module);
  LLVM.DisposeBuilder(llvm_builder);
}
