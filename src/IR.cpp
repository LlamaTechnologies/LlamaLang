#include "IR.hpp"
#include "Console.hpp"
#include "ast/ProgramNode.hpp"
#include "ast/StatementNode.hpp"
#include "ast/FunctionDefNode.hpp"
#include "ast/FunctionCallNode.hpp"
#include "ast/ConstantNode.hpp"
#include "ast/CastOperationNode.hpp"
#include "ast/AssignNode.hpp"
#include "ast/BinaryOperationNode.hpp"
#include "ast/UnaryOperationNode.hpp"
#include "ast/VariableDefNode.hpp"
#include "ast/VariableRefNode.hpp"
#include "Primitives.hpp"
#include <system_error>


using namespace llang;

static llvm::LLVMContext TheContext;
static llvm::IRBuilder<> Builder(TheContext);
static std::unique_ptr<llvm::Module> TheModule;
static std::unordered_map<std::string, llvm::Value *> symbols;
static std::shared_ptr<symbol_table::SymbolTableScope> currentScope;

static std::string MangleName(std::shared_ptr<symbol_table::SymbolTableScope> scope, const std::string &argName)
{
  if (scope->Name != "") {
    return scope->Name + "_" + argName;
  }

  return argName;
}

void IR::Translate(std::shared_ptr<ast::ProgramNode> program, const std::string &outputFileName)
{
  // Make the module, which holds all the code.
  TheModule = std::make_unique<llvm::Module>(program->ModuleName, TheContext);
  auto dl = llvm::DataLayout("e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128");
  TheModule->setDataLayout(dl);
  TheModule->setTargetTriple("x86_64-pc-windows-msvc19.28.29913");

  currentScope = program->GlobalScope;

  for (auto node : program->children) {
    auto nodeType = node->GetType();

    switch (nodeType) {
    case ast::AST_TYPE::FunctionDefNode: {
      auto funcNode = CastNode<ast::FunctionDefNode>(node);
      TranslateNode(funcNode);
      break;
    }

    case ast::AST_TYPE::VariableDefNode: {
      auto varDefNode = CastNode<ast::VariableDefNode>(node);
      TranslateNode(varDefNode, nullptr);
      break;
    }

    default:
      Console::WriteLine(ast::GetAstTypeName(nodeType) + " Not implemented");
      break;
    }
  }

  for (auto node : program->children) {
    auto nodeType = node->GetType();

    switch (nodeType) {
    case ast::AST_TYPE::FunctionDefNode: {
      auto funcNode = CastNode<ast::FunctionDefNode>(node);
      TranslateNode(funcNode);
      break;
    }

    case ast::AST_TYPE::VariableDefNode: {
      auto varDefNode = CastNode<ast::VariableDefNode>(node);
      TranslateNode(varDefNode, nullptr);
      break;
    }

    default:
      Console::WriteLine(ast::GetAstTypeName(nodeType) + " Not implemented");
      break;
    }
  }

#ifdef _DEBUG
  Console::WriteLine();
  TheModule->dump();
#endif

  std::error_code errorCode(1, std::iostream_category());
  auto llvmOutputFile = llvm::raw_fd_ostream(outputFileName + ".bc", errorCode);
  llvm::WriteBitcodeToFile(*TheModule, llvmOutputFile);
}

llvm::Function *IR::TranslateNode(std::shared_ptr<ast::FunctionDefNode> functionNode)
{
  bool isMain = functionNode->Name == "main";
  ast::CONSTANT_TYPE retType = ast::GetConstantType(Primitives::Get(functionNode->ReturnType));

  // 1st iteration
  if (symbols.find(functionNode->Name) == symbols.end()) {
    // Function return type
    llvm::Type *returnType = TranslateType(functionNode->ReturnType);

    // Function parameters
    auto nodeParams = functionNode->Parameters;
    std::vector<llvm::Type *> parameters = TranslateParameters(nodeParams);

    // Function type
    llvm::FunctionType *functionType = parameters.size()
                                       ? llvm::FunctionType::get(returnType, parameters, false)
                                       : llvm::FunctionType::get(returnType, false);

    // Function linkage type
    llvm::Function::LinkageTypes linkageType = llvm::Function::LinkageTypes::LinkOnceODRLinkage;

    // Create the function
    llvm::Function *function =
      llvm::Function::Create(functionType, linkageType, functionNode->Name, TheModule.get());
    function->setCallingConv(llvm::CallingConv::C);

    // Add to symbols
    symbols.insert({ functionNode->Name, function });
    return function;
  }
  // 2nd iteration
  else {
    currentScope = functionNode->InnerScope;

    auto function = llvm::cast<llvm::Function>(symbols.at(functionNode->Name));
    auto nodeParams = functionNode->Parameters;

    // Create a new basic block to start insertion into.
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", function);
    Builder.SetInsertPoint(BB);

    // Set arguments names in IR to better IR reading
    for (auto i = 0; i < nodeParams.size(); ++i) {
      auto arg = function->getArg(i);
      auto param = nodeParams.at(i++);

      auto mangledName = MangleName(currentScope, param->Name);

      if (!arg->getType()->isPointerTy()) {
        arg->setName(mangledName + ".val");

        auto alloc = Builder.CreateAlloca(arg->getType(), nullptr, mangledName);
        Builder.CreateStore(arg, alloc);

        symbols.insert({ mangledName, alloc });
      } else {
        arg->setName(mangledName);
        symbols.insert({ mangledName, arg });
      }
    }

    // Genereate body and finish the function with the return value
    llvm::Value *retVal = TranslateFunctionBlock(*BB, functionNode->Block, retType);

    if (retVal)
      Builder.CreateRet(retVal);
    else
      Builder.CreateRetVoid();

    // Validate the generated code, checking for consistency.
    if (llvm::verifyFunction(*function)) {
      currentScope = functionNode->InnerScope->Parent;

      Console::WriteLine();
      Console::WriteLine("Error in generated function");
      Console::WriteLine();

      function->dump();

      Console::WriteLine();

      // Error reading body, remove function.
      function->eraseFromParent();
      return nullptr;
    }

    // set teh current scrope back to parent
    currentScope = functionNode->InnerScope->Parent;
    return function;
  }
}

llvm::CallInst *IR::TranslateNode(std::shared_ptr<ast::FunctionCallNode> functionCall, IR_INFO *irInfo)
{
  if (functionCall->functionFound == nullptr)
    return nullptr;

  // Function definition not found
  if (symbols.find(functionCall->Name) == symbols.end()) {
    return nullptr;
  }

  auto functionDef = llvm::cast<llvm::Function>(symbols.at(functionCall->Name));
  auto functType = functionDef->getFunctionType();
  std::vector<llvm::Value *> args(functionCall->functionFound->Parameters.size());

  for (auto i = 0; i < functionCall->functionFound->Parameters.size(); ++i) {
    auto param = functionCall->functionFound->Parameters.at(i);
    auto argNode = functionCall->Arguments.at(i);
    IR_INFO copy = IR_INFO(*irInfo);

    switch (argNode->GetType()) {
    case ast::AST_TYPE::ConstantNode: {
      auto constArg = CastNode<ast::ConstantNode>(argNode);

      irInfo->isReturnStmnt = true;
      irInfo->retType = ast::GetConstantType(Primitives::Get(param->VarType));
      auto argValue = TranslateNode(constArg, irInfo);
      args.at(i) = argValue;
    }
    break;

    case ast::AST_TYPE::VariableRefNode: {

    } break;

    case ast::AST_TYPE::BinaryOperationNode: {
      auto binOpArg = CastNode<ast::BinaryOperationNode>(argNode);
      irInfo->isReturnStmnt = true;
      irInfo->retType = ast::GetConstantType(Primitives::Get(param->VarType));
      auto argValue = TranslateNode(binOpArg, irInfo);
      args.at(i) = argValue;
    }
    break;

    case ast::AST_TYPE::UnaryOperationNode: {

    } break;

    case ast::AST_TYPE::FunctionCallNode: {

    } break;
    }

    irInfo->retType = copy.retType;
    irInfo->isReturnStmnt = copy.isReturnStmnt;
  }

  return Builder.CreateCall(functType, functionDef, args);
}

llvm::Constant *IR::TranslateNode(std::shared_ptr<ast::ConstantNode> constant, IR_INFO *irInfo)
{
  ast::CONSTANT_TYPE constType;

  if (irInfo && irInfo->isReturnStmnt) {
    auto retType = irInfo->retType;

    if (retType == ast::CONSTANT_TYPE::FLOAT || retType == ast::CONSTANT_TYPE::DOUBLE) {
      constType = constant->ConstType > retType ? constant->ConstType : retType;
    } else if (retType < ast::CONSTANT_TYPE::FLOAT) {
      constType = constant->ConstType > retType ? constant->ConstType : retType;
    } else {
      constType = constant->ConstType;
    }
  } else {
    constType = constant->ConstType;
  }

  llvm::Constant *constantLLVM = nullptr;

  switch (constType) {
  case ast::CONSTANT_TYPE::I8:
    constantLLVM = llvm::ConstantInt::get(TheContext, llvm::APInt(8, std::stol(constant->Value), false));
    break;

  case ast::CONSTANT_TYPE::I16:
    constantLLVM = llvm::ConstantInt::get(TheContext, llvm::APInt(16, std::stol(constant->Value), true));
    break;

  case ast::CONSTANT_TYPE::I32:
    constantLLVM = llvm::ConstantInt::get(TheContext, llvm::APInt(32, std::stol(constant->Value), true));
    break;

  case ast::CONSTANT_TYPE::I64:
    constantLLVM = llvm::ConstantInt::get(TheContext, llvm::APInt(64, std::stol(constant->Value), true));
    break;

  case ast::CONSTANT_TYPE::FLOAT:
    constantLLVM = llvm::ConstantFP::get(TheContext, llvm::APFloat(std::stof(constant->Value)));
    break;

  case ast::CONSTANT_TYPE::DOUBLE:
    constantLLVM = llvm::ConstantFP::get(TheContext, llvm::APFloat(std::stod(constant->Value)));
    break;

  case ast::CONSTANT_TYPE::STRING:
  default:
    break;
  }

  return constantLLVM;
}

llvm::Value *IR::TranslateNode(std::shared_ptr<ast::BinaryOperationNode> binStmnt, IR_INFO *irInfo)
{
  IR_INFO isLint = { 0 };
  IR_INFO isRint = { 0 };

  llvm::Value *L = TranslateOperand(binStmnt->Left, &isLint);
  llvm::Value *R = TranslateOperand(binStmnt->Right, &isRint);

  if (!L || !R)
    return nullptr;

  auto areInt = ast::BINARY_OPERANDS_TYPES(isLint.isInt << 1 & isRint.isInt);

  if (irInfo != nullptr)
    irInfo->isInt = areInt == ast::BINARY_OPERANDS_TYPES::INT_INT;

  if (L->getType()->isPointerTy())
    L = Builder.CreateLoad(L->getType()->getPointerElementType(), L, "left");

  if (R->getType()->isPointerTy())
    R = Builder.CreateLoad(R->getType()->getPointerElementType(), R, "right");

  switch (binStmnt->Op) {
  case llang::ast::BINARY_OPERATION::ADD:
    return TranslateAdd(L, R, areInt);

  case llang::ast::BINARY_OPERATION::SUB:
    return TranslateSub(L, R, areInt);

  case llang::ast::BINARY_OPERATION::MUL:
    return TranslateMul(L, R, areInt);

  case llang::ast::BINARY_OPERATION::DIV:
    return TranslateDiv(L, R, areInt);

  case llang::ast::BINARY_OPERATION::MOD:
    return TranslateMod(L, R, areInt);

  default:
    // Not a binary operation or not supported yet
    return nullptr;
  }
}

llvm::Value *IR::TranslateNode(std::shared_ptr<ast::UnaryOperationNode> unStmnt, IR_INFO *irInfo)
{
  // TODO implements complete Unary Statement
  // we only handle ret statements for now
  switch (unStmnt->Op) {
  case ast::UNARY_STATEMENT_TYPE::RETURN: {
    irInfo->isReturnStmnt = true;
    auto retVal = unStmnt->Right;
    return retVal ? TranslateNode(retVal, irInfo) : nullptr;
  }

  case ast::UNARY_STATEMENT_TYPE::DECREMENT:
  case ast::UNARY_STATEMENT_TYPE::INCREMENT:
  default:
    return nullptr;
  }
}

llvm::Value *IR::TranslateNode(std::shared_ptr<ast::StatementNode> stmnt, IR_INFO *irInfo)
{
  auto stmntType = stmnt->GetType();

  switch (stmntType) {
  case llang::ast::AST_TYPE::BinaryOperationNode: {
    auto binStmnt = CastNode<ast::BinaryOperationNode>(stmnt);
    return TranslateNode(binStmnt, irInfo);
  }

  case llang::ast::AST_TYPE::ConstantNode: {
    auto constant = CastNode<ast::ConstantNode>(stmnt);
    return TranslateNode(constant, irInfo);
  }

  case llang::ast::AST_TYPE::VariableDefNode: {
    auto constant = CastNode<ast::VariableDefNode>(stmnt);
    return TranslateNode(constant, irInfo);
  }

  case llang::ast::AST_TYPE::VariableRefNode: {
    auto constant = CastNode<ast::VariableRefNode>(stmnt);
    return TranslateNode(constant, irInfo);
  }

  case llang::ast::AST_TYPE::AssignNode: {
    auto constant = CastNode<ast::AssignNode>(stmnt);
    return TranslateNode(constant, irInfo);
  }

  case llang::ast::AST_TYPE::UnaryOperationNode: {
    if (!irInfo)
      return nullptr;

    auto unaryStmnt = CastNode<ast::UnaryOperationNode>(stmnt);
    return TranslateNode(unaryStmnt, irInfo);
  }

  case llang::ast::AST_TYPE::FunctionCallNode: {
    if (!irInfo)
      return nullptr;

    auto callStmnt = CastNode<ast::FunctionCallNode>(stmnt);
    return TranslateNode(callStmnt, irInfo);
  }

  default:
    // Not a statement
    return nullptr;
  }
}

llvm::Value *IR::TranslateNode(std::shared_ptr<ast::VariableDefNode> varDef, IR_INFO *irInfo)
{
  if (!irInfo) {
    if (!varDef->isGlobal) {
      Console::WriteLine("IR_Info is null!");
      return nullptr;
    }

    irInfo = new IR_INFO();
  }

  // mangled name, if global currentScope.Name is empty string
  auto name = MangleName(currentScope, varDef->Name);
  auto type = TranslateType(varDef->VarType);

  if (varDef->isGlobal) {
    TheModule->getOrInsertGlobal(name, type);
    auto globalVar = TheModule->getNamedGlobal(name);
    auto assignStmnt = varDef->assignmentStmnt;

    if (assignStmnt) {
      if (assignStmnt->Right->GetType() == ast::AST_TYPE::ConstantNode) {
        IR_INFO backUp;
        backUp.isGlobal = irInfo->isGlobal;
        backUp.isReturnStmnt = irInfo->isReturnStmnt;
        backUp.retType = irInfo->retType;
        irInfo->isReturnStmnt = irInfo->isGlobal = true;
        irInfo->retType = ast::GetConstantType(Primitives::Get(varDef->VarType));
        auto initVal = TranslateNode(CastNode<ast::ConstantNode>(assignStmnt->Right), irInfo);

        if (initVal->getType()->isPointerTy()) {
          Console::WriteLine("Is pointer");
        }

        globalVar->setInitializer(initVal);
        irInfo->isGlobal = backUp.isGlobal;
        irInfo->isReturnStmnt = backUp.isReturnStmnt;
        irInfo->retType = backUp.retType;
      }
    }

    delete irInfo;
    irInfo = nullptr;
    return globalVar;
  }

  auto *varInst = Builder.CreateAlloca(type, nullptr, name);

  if (!varDef->isGlobal)
    symbols.insert({ name, varInst });

  auto assignStmnt = varDef->assignmentStmnt;

  if (assignStmnt) {
    TranslateNode(varDef->assignmentStmnt, irInfo);
  }

  return varInst;
}

llvm::Value *IR::TranslateNode(std::shared_ptr<ast::VariableRefNode> varRef, IR_INFO *irInfo)
{
  auto varName = varRef->Var->Name;
  llvm::Value *varInst = nullptr;

  if (varRef->Var->isGlobal)
    varInst = TheModule->getNamedGlobal(varName);
  else {
    auto mangledName = MangleName(currentScope, varName);

    if (symbols.find(mangledName) != symbols.end())
      varInst = symbols.at(mangledName);
  }

  if (!irInfo->isReturnStmnt || varInst == nullptr) {
    return varInst;
  }

  auto name = std::string("tmp" + varName);
  auto type = varInst->getType();

  auto loadInst = Builder.CreateLoad(type->getPointerElementType(), varInst, name);
  return loadInst;
}

llvm::Value *IR::TranslateNode(std::shared_ptr<ast::AssignNode> assignmentNode, IR_INFO *irInfo)
{
  if (!assignmentNode->Left || !assignmentNode->Right)
    return nullptr;

  llvm::Value *left = TranslateNode(assignmentNode->Left, irInfo);  
  llvm::Value *right = nullptr;

  if (assignmentNode->Right->GetType() == ast::AST_TYPE::ConstantNode) {
    auto retStmntBackup = irInfo->isReturnStmnt;
    auto retTypeBackup = irInfo->retType;
    irInfo->isReturnStmnt = true;
    irInfo->retType = ast::GetConstantType(Primitives::Get(assignmentNode->Left->Var->VarType));

    auto constant = CastNode<ast::ConstantNode>(assignmentNode->Right);
    right = TranslateNode(constant, irInfo);

    irInfo->isReturnStmnt = retStmntBackup;
    irInfo->retType = retTypeBackup;
  } else if (assignmentNode->Right->GetType() == ast::AST_TYPE::VariableRefNode) {
    auto variableRef = CastNode<ast::VariableRefNode>(assignmentNode->Right);
    auto mangledName = MangleName(currentScope, variableRef->Var->Name);
    auto rightPtr = TranslateNode(variableRef, irInfo);

    if (rightPtr->getType()->isPointerTy())
      right = Builder.CreateLoad(rightPtr->getType()->getPointerElementType(), rightPtr, "tmp" + mangledName);
    else
      right = rightPtr;
  } else if (assignmentNode->Right->GetType() == ast::AST_TYPE::BinaryOperationNode) {
    auto binOp = CastNode<ast::BinaryOperationNode>(assignmentNode->Right);
    right = TranslateNode(binOp, irInfo);
  } else if (assignmentNode->Right->GetType() == ast::AST_TYPE::FunctionCallNode) {
    auto callOp = CastNode<ast::FunctionCallNode>(assignmentNode->Right);
    right = TranslateNode(callOp, irInfo);
  } else if (assignmentNode->Right->GetType() == ast::AST_TYPE::CastOperationNode) {
      auto callOp = CastNode<ast::CastOperationNode>(assignmentNode->Right);
      right = TranslateNode(callOp, irInfo);
  }

  if (right)
    return Builder.CreateStore(right, left);

  return nullptr;
}

llvm::Value *IR::TranslateNode(std::shared_ptr<ast::CastOperationNode> castOpNode, IR_INFO* irInfo) {
    auto opTyTo = castOpNode->TypeTo;
    auto opTyFrom = castOpNode->TypeFrom;
    
    auto value = TranslateNode(castOpNode->Value, irInfo);
    if (value == nullptr)
        return nullptr;

    if (value->getType()->isPointerTy()) {

        value = Builder.CreateLoad(value);
    }
    
    auto tyTo = TranslateType(opTyTo);

    bool intTo = opTyTo >= PRIMITIVE_TYPE::INT8 && opTyTo <= PRIMITIVE_TYPE::INT64;
    bool fpTo = opTyTo >= PRIMITIVE_TYPE::FLOAT32 && opTyTo <= PRIMITIVE_TYPE::FLOAT64;
    bool uintTo = opTyTo >= PRIMITIVE_TYPE::UINT8 && opTyTo <= PRIMITIVE_TYPE::UINT64;
    bool intFrom = opTyFrom >= PRIMITIVE_TYPE::INT8 && opTyFrom <= PRIMITIVE_TYPE::INT64;
    bool fpFrom = opTyFrom >= PRIMITIVE_TYPE::FLOAT32 && opTyFrom <= PRIMITIVE_TYPE::FLOAT64;
    bool uintFrom = opTyFrom >= PRIMITIVE_TYPE::UINT8 && opTyFrom <= PRIMITIVE_TYPE::UINT64;

    if (intTo && intFrom) {
        if (opTyTo < opTyFrom)
            return Builder.CreateTrunc(value, tyTo);
        else
            return Builder.CreateZExt(value, tyTo);
    } else if (intTo && fpFrom) {
        return Builder.CreateSIToFP(value, tyTo);
    } else if (intTo && uintFrom) {
        if (opTyTo < PRIMITIVE_TYPE(int(opTyFrom) - 4))
            return Builder.CreateTrunc(value, tyTo);
        else
            return Builder.CreateZExt(value, tyTo);
    } else if (fpTo && fpFrom) {
        if (opTyTo < opTyFrom)
            return Builder.CreateFPTrunc(value, tyTo);
        else
            return Builder.CreateFPExt(value, tyTo);
    } else if (fpTo && intFrom) {
        return Builder.CreateFPToSI(value, tyTo);
    } else if (fpTo && uintFrom) {
        return Builder.CreateFPToUI(value, tyTo);
    } else if (uintTo && fpFrom) {
        return Builder.CreateUIToFP(value, tyTo);
    } else if (uintTo && intFrom) {
        if (PRIMITIVE_TYPE(int(opTyTo) - 4) < opTyFrom)
            return Builder.CreateTrunc(value, tyTo);
        else
            return Builder.CreateZExt(value, tyTo);
    } else if (uintTo && uintFrom) {
        if (opTyTo < opTyFrom)
            return Builder.CreateTrunc(value, tyTo);
        else
            return Builder.CreateZExt(value, tyTo);
    }

    return nullptr;
}

llvm::Value *IR::TranslateOperand(std::shared_ptr<ast::StatementNode> operand, IR_INFO *irInfo)
{
  switch (operand->StmntType) {
  case ast::STATEMENT_TYPE::CONSTANT: {
    auto constant = CastNode<ast::ConstantNode>(operand);
    irInfo->isInt = constant->ConstType <= ast::CONSTANT_TYPE::I64;
    return TranslateNode(constant, nullptr);
  }

  case ast::STATEMENT_TYPE::VAR_REF: {
    auto varRef = CastNode<ast::VariableRefNode>(operand);

    if (Primitives::Exists(varRef->Var->VarType)) {
      auto prim = Primitives::Get(varRef->Var->VarType);
      irInfo->isInt = prim < PRIMITIVE_TYPE::FLOAT32;
    }

    return TranslateNode(varRef, irInfo);
  }

  case ast::STATEMENT_TYPE::BINARY_OP: {
    auto binStmnt = CastNode<ast::BinaryOperationNode>(operand);
    return TranslateNode(binStmnt, irInfo);
  }

  case ast::STATEMENT_TYPE::ASSIGN: {
    auto constant = CastNode<ast::AssignNode>(operand);
    return TranslateNode(constant, irInfo);
  }

  case ast::STATEMENT_TYPE::UNARY_OP:
  case ast::STATEMENT_TYPE::CALL:
  default:
    // unsuported statment
    return nullptr;
  }
}

llvm::Type* IR::TranslateType(PRIMITIVE_TYPE primitiveType) {
    switch (primitiveType) {
    case llang::PRIMITIVE_TYPE::VOID:
        return llvm::Type::getVoidTy(TheContext);

    case llang::PRIMITIVE_TYPE::BOOL:
        return llvm::Type::getInt1Ty(TheContext);

    case llang::PRIMITIVE_TYPE::INT8:
    case llang::PRIMITIVE_TYPE::UINT8:
        return llvm::Type::getInt8Ty(TheContext);

    case llang::PRIMITIVE_TYPE::INT16:
    case llang::PRIMITIVE_TYPE::UINT16:
        return llvm::Type::getInt16Ty(TheContext);

    case llang::PRIMITIVE_TYPE::INT32:
    case llang::PRIMITIVE_TYPE::UINT32:
        return llvm::Type::getInt32Ty(TheContext);

    case llang::PRIMITIVE_TYPE::INT64:
    case llang::PRIMITIVE_TYPE::UINT64:
        return llvm::Type::getInt64Ty(TheContext);

    case llang::PRIMITIVE_TYPE::FLOAT32:
        return llvm::Type::getFloatTy(TheContext);

    case llang::PRIMITIVE_TYPE::FLOAT64:
    default:
        return llvm::Type::getDoubleTy(TheContext);
    }
}

llvm::Type *IR::TranslateType(std::string &type)
{
  if (Primitives::Exists(type)) {
    auto primitiveType = Primitives::Get(type);

    return TranslateType(primitiveType);
  } else {
    /// user defined type
    return nullptr;
  }
}

std::vector<llvm::Type *> IR::TranslateParameters(std::vector<std::shared_ptr<ast::VariableDefNode>> &params)
{
  std::vector<llvm::Type *> types;

  for (auto param : params) {
    auto type = TranslateType(param->VarType);
    types.push_back(type);
  }

  return types;
}

llvm::Value *IR::TranslateFunctionBlock(llvm::BasicBlock &block, std::vector<std::shared_ptr<ast::StatementNode>> &stmnts, ast::CONSTANT_TYPE retType)
{
  if (stmnts.empty())
    return nullptr;

  IR_INFO irInfo = { 0 };
  irInfo.block = &block;
  irInfo.retType = retType;

  llvm::Value *lastVal = nullptr;

  for (auto stmnt : stmnts) {
    irInfo.isReturnStmnt = false;
    auto retVal = TranslateNode(stmnt, &irInfo);

    if (irInfo.isReturnStmnt)
      lastVal = retVal;
  }

  return lastVal;
}

llvm::Value *IR::TranslateAdd(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types)
{
  switch (types) {
  case ast::BINARY_OPERANDS_TYPES::UINT_UINT:
  case ast::BINARY_OPERANDS_TYPES::UINT_INT:
  case ast::BINARY_OPERANDS_TYPES::INT_UINT:
  case ast::BINARY_OPERANDS_TYPES::INT_INT:
    return Builder.CreateAdd(L, R, "addtmp");

  case ast::BINARY_OPERANDS_TYPES::FLOAT_FLOAT:
    return Builder.CreateFAdd(L, R, "addtmp");

  default:
    Console::WriteLine("Unsuported operands!");
    return nullptr;
  }
}

llvm::Value *IR::TranslateSub(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types)
{
  switch (types) {
  case ast::BINARY_OPERANDS_TYPES::UINT_UINT:
  case ast::BINARY_OPERANDS_TYPES::UINT_INT:
  case ast::BINARY_OPERANDS_TYPES::INT_UINT:
  case ast::BINARY_OPERANDS_TYPES::INT_INT:
    return Builder.CreateAdd(L, R, "subtmp");

  case ast::BINARY_OPERANDS_TYPES::FLOAT_FLOAT:
    return Builder.CreateFAdd(L, R, "subtmp");

  default:
    Console::WriteLine("Unsuported operands!");
    return nullptr;
  }
}

llvm::Value *IR::TranslateMul(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types)
{
  switch (types) {
  case ast::BINARY_OPERANDS_TYPES::UINT_UINT:
  case ast::BINARY_OPERANDS_TYPES::UINT_INT:
  case ast::BINARY_OPERANDS_TYPES::INT_UINT:
  case ast::BINARY_OPERANDS_TYPES::INT_INT:
    return Builder.CreateMul(L, R, "multmp");

  case ast::BINARY_OPERANDS_TYPES::FLOAT_FLOAT:
    return Builder.CreateFMul(L, R, "multmp");

  default:
    Console::WriteLine("Unsuported operands!");
    return nullptr;
  }
}

llvm::Value *IR::TranslateDiv(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types)
{
  switch (types) {
  case ast::BINARY_OPERANDS_TYPES::UINT_UINT:
    return Builder.CreateUDiv(L, R, "divtmp");

  case ast::BINARY_OPERANDS_TYPES::INT_INT:
    return Builder.CreateSDiv(L, R, "divtmp");

  case ast::BINARY_OPERANDS_TYPES::FLOAT_FLOAT:
    return Builder.CreateFAdd(L, R, "divtmp");

  default:
    Console::WriteLine("Unsuported operands!");
    return nullptr;
  }
}

llvm::Value *IR::TranslateMod(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types)
{
  switch (types) {
  case ast::BINARY_OPERANDS_TYPES::UINT_UINT:
    return Builder.CreateURem(L, R, "modtmp");

  case ast::BINARY_OPERANDS_TYPES::INT_INT:
    return Builder.CreateSRem(L, R, "modtmp");

  case ast::BINARY_OPERANDS_TYPES::FLOAT_FLOAT:
    return Builder.CreateFRem(L, R, "modtmp");

  default:
    Console::WriteLine("Unsuported operands!");
    return nullptr;
  }
}
