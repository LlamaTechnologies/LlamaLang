﻿#pragma once
#include <vector>
#include <memory>
#include "../ast/ProgramNode.hpp"
#include "../Primitives.hpp"
#include "../built-in/Built-inTypes.hpp"
#include "../error_handling/ErrorDescription.hpp"
#include "../symbol_table/SymbolTableScope.hpp"

namespace llang::ast
{
    struct FunctionDefNode;
    struct VariableDefNode;
    struct VariableRefNode;
    struct AssignNode;
}

namespace llang::semantics
{
    struct SemanticAnalyzer {
        using ErrorList = std::vector<error_handling::Error>;
        using Scope = std::shared_ptr<symbol_table::SymbolTableScope>;
        static std::shared_ptr<ast::ProgramNode> ast;
        static ErrorList* errors;
        static Scope globalScope;

        SemanticAnalyzer(std::shared_ptr<ast::ProgramNode> ast, Scope globalScope, ErrorList& errors);

        std::shared_ptr<ast::ProgramNode> check();

    private:
        static bool checkNode(ast::Node::ChildType node, Scope scope = nullptr);

        static bool checkNode(std::shared_ptr<ast::FunctionDefNode> funcNode, Scope scope);

        static bool checkNode(std::shared_ptr<ast::StatementNode> node, Scope scope);

        static bool checkNode(std::shared_ptr<ast::VariableDefNode> varDefNode, Scope scope);

        static bool checkNode(std::shared_ptr<ast::FunctionCallNode> funcNode, Scope scope);

        static bool checkNode(std::shared_ptr<ast::VariableRefNode> varRefNode, Scope scope);

        static bool checkNode(std::shared_ptr<ast::UnaryOperationNode> unaryOpNode, Scope scope);

        static bool checkNode(std::shared_ptr<ast::BinaryOperationNode> binaryOpNode, Scope scope);

        static bool checkNode(std::shared_ptr<ast::AssignNode> assignmentNode, Scope scope);
       
    private: // UTILS
        static void modAstWithCast(std::shared_ptr<ast::AssignNode> assignNode, std::shared_ptr<ast::StatementNode> nodeToMod, PRIMITIVE_TYPE resultType);
        static void modAstWithCast(std::shared_ptr<ast::BinaryOperationNode> binOpNode, std::shared_ptr<ast::StatementNode> nodeToMod, PRIMITIVE_TYPE resultType);
        static void modAstWithCast(std::shared_ptr<ast::UnaryOperationNode> retNode, std::shared_ptr<ast::StatementNode> nodeToMod, PRIMITIVE_TYPE resultType);
        static bool checkCastTypes(const PRIMITIVE_TYPE type0, const PRIMITIVE_TYPE type1, PRIMITIVE_TYPE& resultType, bool& shouldCast);
        static bool checkCastVarAndVar(std::shared_ptr<ast::VariableRefNode> varRefL, std::shared_ptr<ast::VariableRefNode> varRefR, PRIMITIVE_TYPE &type, std::shared_ptr<ast::VariableRefNode> &nodeToMod);
        static bool checkCastVarAndConst(std::shared_ptr<ast::VariableRefNode> varRef, std::shared_ptr<ast::ConstantNode> constant);
        static bool checkCastBinOpAndVar(std::shared_ptr<ast::BinaryOperationNode> binOp, std::shared_ptr<ast::VariableRefNode> varRef, PRIMITIVE_TYPE& resultType, std::shared_ptr<ast::StatementNode> &nodeToMod);
        static bool checkCastCallAndVar(std::shared_ptr<ast::FunctionCallNode> callOp, std::shared_ptr<ast::VariableRefNode> varRef, PRIMITIVE_TYPE& resultType, std::shared_ptr<ast::StatementNode>& nodeToMod);
        static bool checkCastRet(std::shared_ptr<ast::StatementNode> retValue, const std::string& returnType, bool &shouldCast);
        static bool checkCastBinOpAndConst(std::shared_ptr<ast::BinaryOperationNode> binOp, std::shared_ptr< ast::ConstantNode> constant);
    };
} // namespace llang::semantics

