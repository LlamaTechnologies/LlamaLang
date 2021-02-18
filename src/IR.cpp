#include "IR.hpp"
#include "Console.hpp"
#include "ast/StatementNode.hpp"
#include "ast/FunctionDefNode.hpp"
#include "ast/ConstantNode.hpp"
#include "ast/BinaryOperationNode.hpp"
#include "ast/VariableRefNode.hpp"
#include "Primitives.hpp"
#include "symbol_table/Symbol.hpp"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>


namespace llang::IR
{
    static void TranslateNode(std::shared_ptr<ast::FunctionDefNode> function);
    static llvm::Value *TranslateNode(std::shared_ptr<ast::ConstantNode> constant);
    /**
     * @brief Translate a binary operation
     * @param binStmnt the binary operation
     * @param isInt if the result of the operation is an integer or not
     * @return the llvm value representing the return value of the binary operation
    */
    static llvm::Value *TranslateNode(std::shared_ptr<ast::BinaryOperationNode> binStmnt, bool *isInt = nullptr);
    /**
     * @brief translates a statement inside a function
     * @param stmnt: the statement to translate
     * @return the llvm value representing the statement result
    */
    static llvm::Value *TranslateNode(std::shared_ptr<ast::StatementNode> stmnt);

    static llvm::Value *TranslateNode(std::shared_ptr<ast::VariableRefNode> varRef);

    // Helpers

    static llvm::Value *TranslateAdd(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types);
    static llvm::Value *TranslateSub(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types);
    static llvm::Value *TranslateMul(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types);
    static llvm::Value *TranslateDiv(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types);
    static llvm::Value *TranslateMod(llvm::Value *L, llvm::Value *R, ast::BINARY_OPERANDS_TYPES types);
    static llvm::Value *TranslateOperand(std::shared_ptr<ast::StatementNode> operand, bool *isInt);
} // namespace llang::IR

namespace llang::IR
{

    static llvm::LLVMContext TheContext;
    static llvm::IRBuilder<> Builder(TheContext);
    static std::unique_ptr<llvm::Module> TheModule;

    void Translate(std::shared_ptr<ast::ProgramNode> program) {
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

        TheModule->dump();
    }

    void TranslateNode(std::shared_ptr<ast::FunctionDefNode> function) {}

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

        llvm::Value *L = TranslateOperand(binStmnt->Left, &isLint);
        llvm::Value *R = TranslateOperand(binStmnt->Right, &isRint);

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

    llvm::Value *TranslateNode(std::shared_ptr<ast::StatementNode> stmnt) {
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
            auto unaryStmnt = CastNode<ast::UnaryOperationNode>(stmnt);
            return TranslateNode(unaryStmnt);
        }
        default:
            // Not a statement
            return nullptr;
        }
    }

    llvm::Value *TranslateNode(std::shared_ptr<ast::VariableRefNode> varRef) {
        return nullptr;
    }

    llvm::Value *TranslateOperand(std::shared_ptr<ast::StatementNode> operand, bool *isInt) {
        switch( operand->StmntType ) {
        case ast::STATEMENT_TYPE::CONSTANT:
        {
            auto constant = CastNode<ast::ConstantNode>(operand);
            *isInt = constant->ConstType == ast::CONSTANT_TYPE::INTEGER;
            return TranslateNode(constant);
        }
        case ast::STATEMENT_TYPE::VAR_REF:
        {
            auto varRef = CastNode<ast::VariableRefNode>(operand);
            auto varType = varRef->Var->VarType;

            if( symbol_table::Symbol::IsPrimitive(varType) ) {
                auto primitive = Primitives::Get(varType);
                *isInt = primitive <= PRIMITIVE_TYPE::UINT64;
            } else {
                *isInt = false;
            }
            
            return TranslateNode(varRef);
        }
        case ast::STATEMENT_TYPE::UNARY_OP:
            return nullptr;
        case ast::STATEMENT_TYPE::BINARY_OP:
        {
            auto binStmnt = CastNode<ast::BinaryOperationNode>(operand);
            return TranslateNode(binStmnt, isInt);
        }
        case ast::STATEMENT_TYPE::CALL:
        default:
            // unsuported statment
            return nullptr;
        }
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