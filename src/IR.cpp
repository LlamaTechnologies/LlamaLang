#include "IR.hpp"
#include "Console.hpp"
#include "ast/StatementNode.hpp"
#include "ast/FunctionDefNode.hpp"
#include "ast/ConstantNode.hpp"
#include "ast/BinaryOperationNode.hpp"
#include "ast/UnaryOperationNode.hpp"
#include "ast/VariableDeclNode.hpp"
#include "ast/VariableRefNode.hpp"
#include "Primitives.hpp"
#include "symbol_table/Symbol.hpp"
#include <system_error>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Bitcode/BitcodeWriter.h>


namespace llang::IR
{
    static llvm::Function *TranslateNode(std::shared_ptr<ast::FunctionDefNode> function);
    static llvm::Value *TranslateNode(std::shared_ptr<ast::ConstantNode> constant);
    /**
     * @brief Translate a binary operation
     * @param binStmnt the binary operation
     * @param isInt if the result of the operation is an integer or not
     * @return the llvm value representing the return value of the binary operation
    */
    static llvm::Value *TranslateNode(std::shared_ptr<ast::BinaryOperationNode> binStmnt, bool *isInt = nullptr);
    static llvm::Value *TranslateNode(std::shared_ptr<ast::UnaryOperationNode> unStmnt, bool &isRet);
    /**
     * @brief translates a statement inside a function
     * @param stmnt: the statement to translate
     * @return the llvm value representing the statement result
    */
    static llvm::Value *TranslateNode(std::shared_ptr<ast::StatementNode> stmnt, bool *isRet = nullptr);

    static llvm::Value *TranslateNode(std::shared_ptr<ast::VariableRefNode> varRef);

    // Helpers
    static llvm::Type *TranslateType(std::string &type);
    static std::vector<llvm::Type *> TranslateParameters(std::vector<std::shared_ptr<ast::VariableDeclNode>> &params);
    /**
     * @brief Translate a function block/body
     * @param stmnts statements in body
     * @return returns the return statement
    */
    static llvm::Value *TranslateFunctionBlock(std::vector<std::shared_ptr<ast::StatementNode>> &stmnts);
    static llvm::Value *TranslateAdd(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types);
    static llvm::Value *TranslateSub(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types);
    static llvm::Value *TranslateMul(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types);
    static llvm::Value *TranslateDiv(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types);
    static llvm::Value *TranslateMod(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types);
    static llvm::Value *TranslateOperand(std::shared_ptr<ast::StatementNode> operand, bool &isInt);
} // namespace llang::IR

namespace llang::IR
{

    static llvm::LLVMContext TheContext;
    static llvm::IRBuilder<> Builder(TheContext);
    static std::unique_ptr<llvm::Module> TheModule;

    void Translate(std::shared_ptr<ast::ProgramNode> program, const std::string &outputFileName) {
        // Make the module, which holds all the code.
        TheModule = std::make_unique<llvm::Module>(program->FileName, TheContext);

        for( auto node : program->children ) {
            auto nodeType = node->GetType();

            switch( nodeType ) {
            case ast::AST_TYPE::FunctionDefNode:
            {
                auto funcNode = CastNode<ast::FunctionDefNode>(node);
                TranslateNode(funcNode);
                break;
            }
            default:
                Console::WriteLine(ast::GetAstTypeName(nodeType) + " Not implemented");
                break;
            }
        }

    #ifdef _DEBUG
        TheModule->dump();
    #endif
        std::error_code errorCode(1, std::iostream_category());
        llvm::raw_ostream& llvmOutputFile = llvm::raw_fd_ostream(outputFileName, errorCode);
        llvm::WriteBitcodeToFile(*TheModule, llvmOutputFile);
    }

    llvm::Function *TranslateNode(std::shared_ptr<ast::FunctionDefNode> functionNode) {
        llvm::Type *returnType = TranslateType(functionNode->ReturnType);
        auto nodeParams = functionNode->Parameters;
        std::vector<llvm::Type *> parameters = TranslateParameters(nodeParams);

        llvm::FunctionType *functionType;
        if( parameters.size() )
            functionType = llvm::FunctionType::get(returnType, parameters, false);
        else
            functionType = llvm::FunctionType::get(returnType, false);

        llvm::Function *function =
            llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, functionNode->Name, TheModule.get());

        // Set arguments names in IR to better IR reading
        unsigned int i = 0;
        for( auto &arg : function->args() )
            arg.setName(nodeParams.at(i++)->Name);

        // Create a new basic block to start insertion into.
        llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", function);
        Builder.SetInsertPoint(BB);

        // Genereate body and finish the function with the return value
        llvm::Value *retVal = TranslateFunctionBlock(functionNode->Block);
        if( retVal )
            Builder.CreateRet(retVal);
        else
            Builder.CreateRetVoid();

        // Validate the generated code, checking for consistency.
        if( !llvm::verifyFunction(*function) ) {
            // Error reading body, remove function.
            function->eraseFromParent();
            return nullptr;
        }

        return function;
    }

    llvm::Value *TranslateNode(std::shared_ptr<ast::ConstantNode> constant) {
        switch( constant->ConstType ) {
        case ast::CONSTANT_TYPE::FLOAT:
            return llvm::ConstantFP::get(TheContext, llvm::APFloat(std::stof(constant->Value)));
        case ast::CONSTANT_TYPE::CHAR:
            return llvm::ConstantInt::get(TheContext, llvm::APInt(8, std::stol(constant->Value), false));
        case ast::CONSTANT_TYPE::INTEGER:
            return llvm::ConstantInt::get(TheContext, llvm::APInt(64, std::stol(constant->Value), true));
        case ast::CONSTANT_TYPE::STRING:
        default:
            return nullptr;;
        }
    }

    llvm::Value *TranslateNode(std::shared_ptr<ast::BinaryOperationNode> binStmnt, bool *isInt) {
        bool isLint, isRint;

        llvm::Value *L = TranslateOperand(binStmnt->Left, isLint);
        llvm::Value *R = TranslateOperand(binStmnt->Right, isRint);
        if( !L || !R )
            return nullptr;

        auto areInt = ast::BINARY_OPERANDS_TYPES(isLint << 1 & isRint);

        if( isInt != nullptr )
            *isInt = areInt == ast::BINARY_OPERANDS_TYPES::INT_INT;

        switch( binStmnt->Op ) {
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

    llvm::Value *TranslateNode(std::shared_ptr<ast::UnaryOperationNode> unStmnt, bool &isRet) {
        switch( unStmnt->Op ) {
        case ast::UNARY_STATEMENT_TYPE::RETURN:
        {
            isRet = true;
            auto retVal = unStmnt->Right;
            return retVal ? TranslateNode(retVal) : nullptr;
        }
        case ast::UNARY_STATEMENT_TYPE::DECREMENT:
        case ast::UNARY_STATEMENT_TYPE::INCREMENT:
        default:
            return nullptr;
        }
    }

    llvm::Value *TranslateNode(std::shared_ptr<ast::StatementNode> stmnt, bool *isRet) {
        if( isRet )
            *isRet = false;

        auto stmntType = stmnt->GetType();
        switch( stmntType ) {
        case llang::ast::AST_TYPE::BinaryOperationNode:
        {
            auto binStmnt = CastNode<ast::BinaryOperationNode>(stmnt);
            return TranslateNode(binStmnt);
        }
        case llang::ast::AST_TYPE::ConstantNode:
        {
            auto constant = CastNode<ast::ConstantNode>(stmnt);
            return TranslateNode(constant);
        }
        case llang::ast::AST_TYPE::UnaryOperationNode:
        {
            if( !isRet )
                return nullptr;

            auto unaryStmnt = CastNode<ast::UnaryOperationNode>(stmnt);
            return TranslateNode(unaryStmnt, *isRet);
        }
        default:
            // Not a statement
            return nullptr;
        }
    }

    llvm::Value *TranslateNode(std::shared_ptr<ast::VariableRefNode> varRef) {
        return nullptr;
    }

    llvm::Value *TranslateOperand(std::shared_ptr<ast::StatementNode> operand, bool &isInt) {
        switch( operand->StmntType ) {
        case ast::STATEMENT_TYPE::CONSTANT:
        {
            auto constant = CastNode<ast::ConstantNode>(operand);
            isInt = constant->ConstType == ast::CONSTANT_TYPE::INTEGER;
            return TranslateNode(constant);
        }
        case ast::STATEMENT_TYPE::VAR_REF:
        {
            auto varRef = CastNode<ast::VariableRefNode>(operand);
            auto varType = varRef->Var->VarType;

            if( symbol_table::Symbol::IsPrimitive(varType) ) {
                auto primitive = Primitives::Get(varType);
                isInt = primitive <= PRIMITIVE_TYPE::UINT64;
            } else {
                isInt = false;
            }

            return TranslateNode(varRef);
        }
        case ast::STATEMENT_TYPE::UNARY_OP:
            return nullptr;
        case ast::STATEMENT_TYPE::BINARY_OP:
        {
            auto binStmnt = CastNode<ast::BinaryOperationNode>(operand);
            return TranslateNode(binStmnt, &isInt);
        }
        case ast::STATEMENT_TYPE::CALL:
        default:
            // unsuported statment
            return nullptr;
        }
    }

    llvm::Type *TranslateType(std::string &type) {
        if( symbol_table::Symbol::IsPrimitive(type) ) {
            auto primitiveType = Primitives::Get(type);
            switch( primitiveType ) {
            case llang::PRIMITIVE_TYPE::VOID:
                return llvm::Type::getVoidTy(TheContext);
            case llang::PRIMITIVE_TYPE::BOOL:
                return llvm::Type::getInt1Ty(TheContext);
            case llang::PRIMITIVE_TYPE::SCHAR:
            case llang::PRIMITIVE_TYPE::CHAR:
            case llang::PRIMITIVE_TYPE::BYTE:
            case llang::PRIMITIVE_TYPE::INT8:
            case llang::PRIMITIVE_TYPE::UINT8:
                return llvm::Type::getInt8Ty(TheContext);
            case llang::PRIMITIVE_TYPE::WCHAR:
            case llang::PRIMITIVE_TYPE::INT16:
            case llang::PRIMITIVE_TYPE::UINT16:
                return llvm::Type::getInt16Ty(TheContext);
            case llang::PRIMITIVE_TYPE::UCHAR:
            case llang::PRIMITIVE_TYPE::INT32:
            case llang::PRIMITIVE_TYPE::UINT32:
                return llvm::Type::getInt32Ty(TheContext);
            case llang::PRIMITIVE_TYPE::INT64:
            case llang::PRIMITIVE_TYPE::UINT64:
                return llvm::Type::getInt64Ty(TheContext);
            case llang::PRIMITIVE_TYPE::FLOAT32:
                return llvm::Type::getFloatTy(TheContext);
            case llang::PRIMITIVE_TYPE::FLOAT64:
                return llvm::Type::getDoubleTy(TheContext);
            }
        } else {
            /// user defined type
            return nullptr;
        }
    }

    std::vector<llvm::Type *> TranslateParameters(std::vector<std::shared_ptr<ast::VariableDeclNode>> &params) {
        std::vector<llvm::Type *> types;
        for( auto param : params ) {
            auto type = TranslateType(param->VarType);
            types.push_back(type);
        }
        return types;
    }

    llvm::Value *TranslateFunctionBlock(std::vector<std::shared_ptr<ast::StatementNode>> &stmnts) {
        if( stmnts.empty() )
            return nullptr;

        llvm::Value *lastVal = nullptr;
        for( auto stmnt : stmnts ) {
            bool isRet;
            auto retVal = TranslateNode(stmnt, &isRet);
            if( isRet )
                lastVal = retVal;
        }
        return lastVal;
    }

    llvm::Value *TranslateAdd(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types) {
        switch( types ) {
        case ast::BINARY_OPERANDS_TYPES::INT_INT:
            return Builder.CreateAdd(L, R, "addtmp");
        case ast::BINARY_OPERANDS_TYPES::FLOAT_FLOAT:
            return Builder.CreateFAdd(L, R, "addtmp");
        default:
            // unsuported operands
            return nullptr;
        }
    }

    llvm::Value *TranslateSub(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types) {
        return nullptr;
    }

    llvm::Value *TranslateMul(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types) {
        return nullptr;
    }

    llvm::Value *TranslateDiv(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types) {
        return nullptr;
    }

    llvm::Value *TranslateMod(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types) {
        switch( types ) {
        case ast::BINARY_OPERANDS_TYPES::INT_INT:
            // unsigned divition
            return Builder.CreateURem(L, R, "modtmp");
        case ast::BINARY_OPERANDS_TYPES::FLOAT_FLOAT:
            return Builder.CreateFRem(L, R, "modtmp");
        default:
            // unsuported operands
            return nullptr;
        }
    }
} // namespace llang::IR