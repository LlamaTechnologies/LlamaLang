#pragma once
#include <memory>

namespace llang {
    namespace ast {
        struct AssignNode;
        struct BinaryOperationNode;
        struct ConstantNode;
        struct FunctionCallNode;
        struct FunctionDefNode;
        struct ProgramNode;
        struct StatementNode;
        struct UnaryOperationNode;
        struct VariableDefNode;
        struct VariableRefNode;
    }

    class IR {
        struct IR_INFO {
            bool isReturnStmnt;
            bool isInt;
            bool isGlobal;
            ast::CONSTANT_TYPE retType;
            llvm::BasicBlock* block;
        };

    public:
        void Translate(std::shared_ptr<ast::ProgramNode> program, const std::string& outputFileName);
    
    private:
        static llvm::Function* TranslateNode(std::shared_ptr<ast::FunctionDefNode> function);
        static llvm::Constant* TranslateNode(std::shared_ptr<ast::ConstantNode> constant, IR_INFO* irInfo);
        /**
         * @brief Translate a binary operation
         * @param binStmnt the binary operation
         * @param isInt if the result of the operation is an integer or not
         * @return the llvm value representing the return value of the binary operation
        */
        static llvm::Value* TranslateNode(std::shared_ptr<ast::BinaryOperationNode> binStmnt, IR_INFO* irInfo);
        static llvm::Value* TranslateNode(std::shared_ptr<ast::UnaryOperationNode> unStmnt, IR_INFO* irInfo);
        /**
         * @brief translates a statement inside a function
         * @param stmnt: the statement to translate
         * @return the llvm value representing the statement result
        */
        static llvm::Value* TranslateNode(std::shared_ptr<ast::StatementNode> stmnt, IR_INFO* irInfo);
        static llvm::Value* TranslateNode(std::shared_ptr<ast::VariableDefNode> varDef, IR_INFO* irInfo);
        static llvm::Value* TranslateNode(std::shared_ptr<ast::VariableRefNode> varRef, IR_INFO* irInfo);
        static llvm::Value* TranslateNode(std::shared_ptr<ast::AssignNode> assignmentNode, IR_INFO* irInfo);

        // Helpers
        static llvm::Type* TranslateType(std::string& type, ast::CONSTANT_TYPE* retType);
        static std::vector<llvm::Type*> TranslateParameters(std::vector<std::shared_ptr<ast::VariableDefNode>>& params);
        /**
         * @brief Translate a function block/body
         * @param stmnts statements in body
         * @return returns the return statement
        */
        static llvm::Value* TranslateFunctionBlock(llvm::BasicBlock& block, std::vector<std::shared_ptr<ast::StatementNode>>& stmnts, ast::CONSTANT_TYPE retType);
        static llvm::Value* TranslateAdd(llvm::Value* L, llvm::Value* R, ast::BINARY_OPERANDS_TYPES types);
        static llvm::Value* TranslateSub(llvm::Value* L, llvm::Value* R, ast::BINARY_OPERANDS_TYPES types);
        static llvm::Value* TranslateMul(llvm::Value* L, llvm::Value* R, ast::BINARY_OPERANDS_TYPES types);
        static llvm::Value* TranslateDiv(llvm::Value* L, llvm::Value* R, ast::BINARY_OPERANDS_TYPES types);
        static llvm::Value* TranslateMod(llvm::Value* L, llvm::Value* R, ast::BINARY_OPERANDS_TYPES types);
        static llvm::Value* TranslateOperand(std::shared_ptr<ast::StatementNode> operand, IR_INFO* irInfo);
    }
}