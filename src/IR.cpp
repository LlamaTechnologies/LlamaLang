#include "IR.hpp"
#include "Console.hpp"
#include "ast/ProgramNode.hpp"
#include "ast/StatementNode.hpp"
#include "ast/FunctionDefNode.hpp"
#include "ast/ConstantNode.hpp"
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
static std::unordered_map<std::string, llvm::Value*> symbols;

void IR::Translate(std::shared_ptr<ast::ProgramNode> program, const std::string& outputFileName) {
    // Make the module, which holds all the code.
    TheModule = std::make_unique<llvm::Module>(program->FileName, TheContext);

    for (auto node : program->children) {
        auto nodeType = node->GetType();

        switch (nodeType) {
        case ast::AST_TYPE::FunctionDefNode:
        {
            auto funcNode = CastNode<ast::FunctionDefNode>(node);
            TranslateNode(funcNode);
            break;
        }
        case ast::AST_TYPE::VariableDefNode:
        {
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
    auto llvmOutputFile = llvm::raw_fd_ostream(outputFileName, errorCode);
    llvm::WriteBitcodeToFile(*TheModule, llvmOutputFile);
}

llvm::Function* IR::TranslateNode(std::shared_ptr<ast::FunctionDefNode> functionNode) {
    ast::CONSTANT_TYPE retType;
    llvm::Type* returnType = TranslateType(functionNode->ReturnType, &retType);
    auto nodeParams = functionNode->Parameters;
    std::vector<llvm::Type*> parameters = TranslateParameters(nodeParams);

    llvm::FunctionType* functionType;
    if (parameters.size())
        functionType = llvm::FunctionType::get(returnType, parameters, false);
    else
        functionType = llvm::FunctionType::get(returnType, false);

    llvm::Function* function =
        llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, functionNode->Name, TheModule.get());

    // Set arguments names in IR to better IR reading
    unsigned int i = 0;
    for (auto& arg : function->args())
        arg.setName(nodeParams.at(i++)->Name);

    // Create a new basic block to start insertion into.
    llvm::BasicBlock* BB = llvm::BasicBlock::Create(TheContext, "entry", function);
    Builder.SetInsertPoint(BB);

    // Genereate body and finish the function with the return value
    llvm::Value* retVal = TranslateFunctionBlock(*BB, functionNode->Block, retType);
    if (retVal)
        Builder.CreateRet(retVal);
    else
        Builder.CreateRetVoid();

    // Validate the generated code, checking for consistency.
    if (llvm::verifyFunction(*function)) {
        Console::WriteLine();
        Console::WriteLine("Error in generated function");
        Console::WriteLine();

        function->dump();

        Console::WriteLine();

        // Error reading body, remove function.
        function->eraseFromParent();
        return nullptr;
    }

    return function;
}

llvm::Constant* IR::TranslateNode(std::shared_ptr<ast::ConstantNode> constant, IR_INFO* irInfo) {
    ast::CONSTANT_TYPE constType;
    if (irInfo && irInfo->isReturnStmnt) {
        auto retType = irInfo->retType;
        if (retType == ast::CONSTANT_TYPE::FLOAT || retType == ast::CONSTANT_TYPE::DOUBLE) {
            constType = constant->ConstType > retType ? constant->ConstType : retType;
        }
        else if (retType < ast::CONSTANT_TYPE::FLOAT) {
            constType = constant->ConstType > retType ? constant->ConstType : retType;
        }
        else {
            constType = constant->ConstType;
        }
    }
    else {
        constType = constant->ConstType;
    }
    llvm::Constant* constantLLVM = nullptr;
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

llvm::Value* IR::TranslateNode(std::shared_ptr<ast::BinaryOperationNode> binStmnt, IR_INFO* irInfo) {
    IR_INFO isLint = { 0 };
    IR_INFO isRint = { 0 };

    llvm::Value* L = TranslateOperand(binStmnt->Left, &isLint);
    llvm::Value* R = TranslateOperand(binStmnt->Right, &isRint);
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

llvm::Value* IR::TranslateNode(std::shared_ptr<ast::UnaryOperationNode> unStmnt, IR_INFO* irInfo) {
    // TODO implements complete Unary Statement
    // we only handle ret statements for now
    switch (unStmnt->Op) {
    case ast::UNARY_STATEMENT_TYPE::RETURN:
    {
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

llvm::Value* IR::TranslateNode(std::shared_ptr<ast::StatementNode> stmnt, IR_INFO* irInfo) {
    auto stmntType = stmnt->GetType();
    switch (stmntType) {
    case llang::ast::AST_TYPE::BinaryOperationNode:
    {
        auto binStmnt = CastNode<ast::BinaryOperationNode>(stmnt);
        return TranslateNode(binStmnt, irInfo);
    }
    case llang::ast::AST_TYPE::ConstantNode:
    {
        auto constant = CastNode<ast::ConstantNode>(stmnt);
        return TranslateNode(constant, irInfo);
    }
    case llang::ast::AST_TYPE::VariableDefNode:
    {
        auto constant = CastNode<ast::VariableDefNode>(stmnt);
        return TranslateNode(constant, irInfo);
    }
    case llang::ast::AST_TYPE::VariableRefNode:
    {
        auto constant = CastNode<ast::VariableRefNode>(stmnt);
        return TranslateNode(constant, irInfo);
    }
    case llang::ast::AST_TYPE::AssignNode:
    {
        auto constant = CastNode<ast::AssignNode>(stmnt);
        return TranslateNode(constant, irInfo);
    }
    case llang::ast::AST_TYPE::UnaryOperationNode:
    {
        if (!irInfo)
            return nullptr;

        auto unaryStmnt = CastNode<ast::UnaryOperationNode>(stmnt);
        return TranslateNode(unaryStmnt, irInfo);
    }
    default:
        // Not a statement
        return nullptr;
    }
}

llvm::Value* IR::TranslateNode(std::shared_ptr<ast::VariableDefNode> varDef, IR_INFO* irInfo) {
    if (!irInfo) {
        if (!varDef->isGlobal) {
            Console::WriteLine("IR_Info is null!");
            return nullptr;
        }
        irInfo = new IR_INFO();
    }

    auto name = varDef->Name;
    auto type = TranslateType(varDef->VarType, nullptr);

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

    auto* varInst = Builder.CreateAlloca(type, nullptr, name);
    symbols.insert({ name , varInst });

    auto assignStmnt = varDef->assignmentStmnt;
    if (assignStmnt) {
        TranslateNode(varDef->assignmentStmnt, irInfo);
    }

    return varInst;
}

llvm::Value* IR::TranslateNode(std::shared_ptr<ast::VariableRefNode> varRef, IR_INFO* irInfo) {
    auto varName = varRef->Var->Name;
    llvm::Value* varInst = nullptr;
    if (varRef->Var->isGlobal)
        varInst = TheModule->getNamedGlobal(varName);
    else
        varInst = symbols.at(varName);

    if (!irInfo->isReturnStmnt) {
        return varInst;
    }

    auto name = std::string("tmp" + varName);
    auto type = varInst->getType();

    auto loadInst = Builder.CreateLoad(type->getPointerElementType(), varInst, name);
    return loadInst;
}

llvm::Value* IR::TranslateNode(std::shared_ptr<ast::AssignNode> assignmentNode, IR_INFO* irInfo) {
    llvm::Value* left = TranslateNode(assignmentNode->Left, irInfo);
    llvm::Value* right = nullptr;
    if (assignmentNode->Right->GetType() == ast::AST_TYPE::ConstantNode) {
        auto retStmntBackup = irInfo->isReturnStmnt;
        auto retTypeBackup = irInfo->retType;
        irInfo->isReturnStmnt = true;
        irInfo->retType = ast::GetConstantType(Primitives::Get(assignmentNode->Left->Var->VarType));

        auto constant = CastNode<ast::ConstantNode>(assignmentNode->Right);
        right = TranslateNode(constant, irInfo);

        irInfo->isReturnStmnt = retStmntBackup;
        irInfo->retType = retTypeBackup;
    }
    else if (assignmentNode->Right->GetType() == ast::AST_TYPE::VariableRefNode) {
        auto variableRef = CastNode<ast::VariableRefNode>(assignmentNode->Right);
        auto rightPtr = TranslateNode(variableRef, irInfo);
        right = Builder.CreateLoad(rightPtr->getType()->getPointerElementType(), rightPtr, "tmp" + variableRef->Var->Name);
    }
    else if (assignmentNode->Right->GetType() == ast::AST_TYPE::BinaryOperationNode) {
        auto binOp = CastNode<ast::BinaryOperationNode>(assignmentNode->Right);
        right = TranslateNode(binOp, irInfo);
    }

    if (right)
        return Builder.CreateStore(right, left);
    return nullptr;
}

llvm::Value* IR::TranslateOperand(std::shared_ptr<ast::StatementNode> operand, IR_INFO* irInfo) {
    switch (operand->StmntType) {
    case ast::STATEMENT_TYPE::CONSTANT:
    {
        auto constant = CastNode<ast::ConstantNode>(operand);
        irInfo->isInt = constant->ConstType <= ast::CONSTANT_TYPE::I64;
        return TranslateNode(constant, nullptr);
    }
    case ast::STATEMENT_TYPE::VAR_REF:
    {
        auto varRef = CastNode<ast::VariableRefNode>(operand);
        if (Primitives::Exists(varRef->Var->VarType)) {
            auto prim = Primitives::Get(varRef->Var->VarType);
            irInfo->isInt = prim < PRIMITIVE_TYPE::FLOAT32;
        }
        return TranslateNode(varRef, irInfo);
    }
    case ast::STATEMENT_TYPE::BINARY_OP:
    {
        auto binStmnt = CastNode<ast::BinaryOperationNode>(operand);
        return TranslateNode(binStmnt, irInfo);
    }
    case ast::STATEMENT_TYPE::ASSIGN:
    {
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

llvm::Type* IR::TranslateType(std::string& type, ast::CONSTANT_TYPE* retType) {
    if (Primitives::Exists(type)) {
        auto primitiveType = Primitives::Get(type);
        switch (primitiveType) {
        case llang::PRIMITIVE_TYPE::VOID:
            if (retType) {
                *retType = ast::CONSTANT_TYPE::_COUNT;
            }
            return llvm::Type::getVoidTy(TheContext);
        case llang::PRIMITIVE_TYPE::BOOL:
            if (retType) {
                *retType = ast::CONSTANT_TYPE::I8;
            }
            return llvm::Type::getInt1Ty(TheContext);
        case llang::PRIMITIVE_TYPE::SCHAR:
        case llang::PRIMITIVE_TYPE::CHAR:
        case llang::PRIMITIVE_TYPE::BYTE:
        case llang::PRIMITIVE_TYPE::INT8:
        case llang::PRIMITIVE_TYPE::UINT8:
            if (retType) {
                *retType = ast::CONSTANT_TYPE::I8;
            }
            return llvm::Type::getInt8Ty(TheContext);
        case llang::PRIMITIVE_TYPE::WCHAR:
        case llang::PRIMITIVE_TYPE::INT16:
        case llang::PRIMITIVE_TYPE::UINT16:
            if (retType) {
                *retType = ast::CONSTANT_TYPE::I16;
            }
            return llvm::Type::getInt16Ty(TheContext);
        case llang::PRIMITIVE_TYPE::UCHAR:
        case llang::PRIMITIVE_TYPE::INT32:
        case llang::PRIMITIVE_TYPE::UINT32:
            if (retType) {
                *retType = ast::CONSTANT_TYPE::I32;
            }
            return llvm::Type::getInt32Ty(TheContext);
        case llang::PRIMITIVE_TYPE::INT64:
        case llang::PRIMITIVE_TYPE::UINT64:
            if (retType) {
                *retType = ast::CONSTANT_TYPE::I64;
            }
            return llvm::Type::getInt64Ty(TheContext);
        case llang::PRIMITIVE_TYPE::FLOAT32:
            if (retType) {
                *retType = ast::CONSTANT_TYPE::FLOAT;
            }
            return llvm::Type::getFloatTy(TheContext);
        case llang::PRIMITIVE_TYPE::FLOAT64:
        default:
            if (retType) {
                *retType = ast::CONSTANT_TYPE::DOUBLE;
            }
            return llvm::Type::getDoubleTy(TheContext);
        }
    }
    else {
        /// user defined type
        return nullptr;
    }
}

std::vector<llvm::Type*> IR::TranslateParameters(std::vector<std::shared_ptr<ast::VariableDefNode>>& params) {
    std::vector<llvm::Type*> types;
    for (auto param : params) {
        auto type = TranslateType(param->VarType, nullptr);
        types.push_back(type);
    }
    return types;
}

llvm::Value* IR::TranslateFunctionBlock(llvm::BasicBlock& block, std::vector<std::shared_ptr<ast::StatementNode>>& stmnts, ast::CONSTANT_TYPE retType) {
    if (stmnts.empty())
        return nullptr;

    IR_INFO irInfo = { 0 };
    irInfo.block = &block;
    irInfo.retType = retType;

    llvm::Value* lastVal = nullptr;
    for (auto stmnt : stmnts) {
        irInfo.isReturnStmnt = false;
        auto retVal = TranslateNode(stmnt, &irInfo);
        if (irInfo.isReturnStmnt)
            lastVal = retVal;
    }
    return lastVal;
}

llvm::Value* IR::TranslateAdd(llvm::Value* L, llvm::Value* R, ast::BINARY_OPERANDS_TYPES types) {
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

llvm::Value* IR::TranslateSub(llvm::Value* L, llvm::Value* R, ast::BINARY_OPERANDS_TYPES types) {
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

llvm::Value* IR::TranslateMul(llvm::Value* L, llvm::Value* R, ast::BINARY_OPERANDS_TYPES types) {
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

llvm::Value* IR::TranslateDiv(llvm::Value* L, llvm::Value* R, ast::BINARY_OPERANDS_TYPES types) {
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

llvm::Value* IR::TranslateMod(llvm::Value* L, llvm::Value* R, ast::BINARY_OPERANDS_TYPES types) {
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
