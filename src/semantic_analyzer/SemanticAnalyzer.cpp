﻿#include "SemanticAnalyzer.hpp"
#include "../ast/UnaryOperationNode.hpp"
#include "../ast/FunctionDefNode.hpp"
#include "../ast/ConstantNode.hpp"
#include "../ast/VariableDefNode.hpp"
#include "../ast/VariableRefNode.hpp"
#include "../ast/AssignNode.hpp"
#include "../ast/StatementNode.hpp"

using namespace llang;
using namespace semantics;
SemanticAnalyzer::ErrorList *SemanticAnalyzer::errors = nullptr;
std::shared_ptr<ast::ProgramNode> SemanticAnalyzer::ast = nullptr;
SemanticAnalyzer::Scope SemanticAnalyzer::globalScope = nullptr;

SemanticAnalyzer::SemanticAnalyzer(std::shared_ptr<ast::ProgramNode> ast,
                                   Scope scope,
                                   ErrorList &errors) {
    SemanticAnalyzer::ast = ast;
    SemanticAnalyzer::globalScope = scope;
    SemanticAnalyzer::errors = &errors;
}

std::shared_ptr<ast::ProgramNode> SemanticAnalyzer::check() {
    ast->ForEachDeep([](ast::Node::ChildType child) { checkNode(child); });

    return ast;
}

bool SemanticAnalyzer::checkNode(ast::Node::ChildType node, Scope scope) {
    if( scope == nullptr )
        scope = globalScope;

    switch( node->GetType() ) {
    case ast::AST_TYPE::ProgramNode:
        return false;
    case ast::AST_TYPE::FunctionDefNode:
        return checkNode(CastNode<ast::FunctionDefNode>(node), scope);
    default:
        return checkNode(CastNode<ast::StatementNode>(node), scope);
    }
}

bool SemanticAnalyzer::checkNode(std::shared_ptr<ast::StatementNode> node, Scope scope) {
    if( !scope )
        scope = globalScope;

    switch( node->GetType() ) {
    case ast::AST_TYPE::VariableDefNode:
        return checkNode(CastNode<ast::VariableDefNode>(node), scope);
    case ast::AST_TYPE::AssignNode:
        return checkNode(CastNode<ast::AssignNode>(node), scope);
    default:
        return false;
    }
}

bool SemanticAnalyzer::checkNode(std::shared_ptr<ast::FunctionDefNode> funcNode, Scope scope) {
    auto returnStmnt = std::static_pointer_cast<ast::UnaryOperationNode, ast::StatementNode>( funcNode->Block.back() );

    // Check redefinition
    auto scopesVec = scope->children.at(funcNode->Name);
    if( scopesVec.size() > 1 ) {
        return false;
    }

    auto funcScopeNode = scopesVec.front();

    {   // Function Signature and return type check
        // return type is not void and the last statement isnt a return
        if( funcNode->ReturnType != "void" && returnStmnt->Op != ast::UNARY_STATEMENT_TYPE::RETURN ) {
            error_handling::Error error(funcNode->Line, funcNode->FileName,
                                        "Function " + funcNode->Name +
                                        " needs a return statement");
            errors->emplace_back(error);
            return false;
        }

        if( Primitives::Exists(funcNode->ReturnType) ) {
            auto retType = Primitives::Get(funcNode->ReturnType);

            // return type is void and the return statement has a return value
            if( retType == PRIMITIVE_TYPE::VOID ) {
                if( returnStmnt->Right != nullptr ) {
                    error_handling::Error error(funcNode->Line, funcNode->FileName,
                                                "Function " + funcNode->Name +
                                                " should return void");
                    errors->emplace_back(error);
                    return false;
                }
                return true;
            }
            // Return a constant and is not the right type
            else if( returnStmnt->Right->StmntType == ast::STATEMENT_TYPE::CONSTANT ) {
                auto constStmnt = std::static_pointer_cast<ast::ConstantNode, ast::StatementNode>( returnStmnt->Right );
                switch( retType ) {
                case PRIMITIVE_TYPE::CHAR:
                case PRIMITIVE_TYPE::BYTE:
                case PRIMITIVE_TYPE::UINT8:
                case PRIMITIVE_TYPE::WCHAR:
                case PRIMITIVE_TYPE::UINT16:
                case PRIMITIVE_TYPE::UCHAR:
                case PRIMITIVE_TYPE::UINT32:
                case PRIMITIVE_TYPE::UINT64:
                    if( constStmnt->Value[0] == '-' ) {
                        auto name = constStmnt->FileName + ":" + funcNode->Name;
                        error_handling::Error error(
                            constStmnt->Line, name,
                            "SIGN MISMATCH :: Function return type is unsigned, signed type is returned");
                        errors->emplace_back(error);
                        return false;
                    } break;
                case PRIMITIVE_TYPE::SCHAR:
                case PRIMITIVE_TYPE::INT8:
                {
                    auto constantName = ast::GetConstantTypeName(ast::CONSTANT_TYPE::I8);
                    auto actualConstName = ast::GetConstantTypeName(constStmnt->ConstType);
                    auto name = constStmnt->FileName + ":" + funcNode->Name;
                    if( constStmnt->ConstType > ast::CONSTANT_TYPE::I64 ) {
                        error_handling::Error error(
                            (int) constStmnt->Line, name,
                            "TYPE MISMATCH :: Function return type is " + constantName + ", " + actualConstName + " is returned");
                        errors->emplace_back(error);
                        return false;
                    } else if( constStmnt->ConstType != ast::CONSTANT_TYPE::I8 ) {
                        error_handling::Error error(
                            (int) constStmnt->Line, name,
                            "I8 OVERFLOW :: Function return type is " + constantName + ", " + actualConstName + " is returned");
                        errors->emplace_back(error);
                        return false;
                    }
                } break;
                case PRIMITIVE_TYPE::INT16:
                {
                    auto constantName = ast::GetConstantTypeName(ast::CONSTANT_TYPE::I16);
                    auto actualConstName = ast::GetConstantTypeName(constStmnt->ConstType);
                    auto name = constStmnt->FileName + ":" + funcNode->Name;

                    if( constStmnt->ConstType > ast::CONSTANT_TYPE::I64 ) {
                        error_handling::Error error(
                            (int) constStmnt->Line, name,
                            "TYPE MISMATCH :: Function return type is " + constantName + ", " + actualConstName + " is returned");
                        errors->emplace_back(error);
                        return false;
                    } else if( constStmnt->ConstType > ast::CONSTANT_TYPE::I16 ) {
                        error_handling::Error error(
                            (int) constStmnt->Line, name,
                            "I16 OVERFLOW :: Function return type is " + constantName + ", " + actualConstName + " is returned");
                        errors->emplace_back(error);
                        return false;
                    }
                } break;
                case PRIMITIVE_TYPE::INT32:
                {
                    auto constantName = ast::GetConstantTypeName(ast::CONSTANT_TYPE::I32);
                    auto actualConstName = ast::GetConstantTypeName(constStmnt->ConstType);
                    auto name = constStmnt->FileName + ":" + funcNode->Name;

                    if( constStmnt->ConstType > ast::CONSTANT_TYPE::I64 ) {
                        error_handling::Error error(
                            (int) constStmnt->Line, name,
                            "TYPE MISMATCH :: Function return type is " + constantName + ", " + actualConstName + " is returned");
                        errors->emplace_back(error);
                        return false;
                    } else if( constStmnt->ConstType > ast::CONSTANT_TYPE::I32 ) {
                        error_handling::Error error(
                            (int) constStmnt->Line, name,
                            "I32 OVERFLOW :: Function return type is " + constantName + ", " + actualConstName + " is returned");
                        errors->emplace_back(error);
                        return false;
                    }
                } break;
                case PRIMITIVE_TYPE::INT64:
                {
                    auto constantName = ast::GetConstantTypeName(ast::CONSTANT_TYPE::I64);
                    auto actualConstName = ast::GetConstantTypeName(constStmnt->ConstType);
                    auto name = constStmnt->FileName + ":" + funcNode->Name;

                    if( constStmnt->ConstType > ast::CONSTANT_TYPE::I64 ) {
                        error_handling::Error error(
                            (int) constStmnt->Line, name,
                            "TYPE MISMATCH :: Function return type is " + constantName + ", " + actualConstName + " is returned");
                        errors->emplace_back(error);
                        return false;
                    }
                } break;
                case PRIMITIVE_TYPE::FLOAT32:
                {
                    auto actualConstName = ast::GetConstantTypeName(constStmnt->ConstType);
                    auto name = constStmnt->FileName + ":" + funcNode->Name;

                    if( constStmnt->ConstType < ast::CONSTANT_TYPE::FLOAT ) {
                        error_handling::Error error(
                            constStmnt->Line, name,
                            "TYPE MISMATCH :: Function return type is float, " + actualConstName + " is returned");
                        errors->emplace_back(error);
                        return false;
                    } else if( constStmnt->ConstType > ast::CONSTANT_TYPE::FLOAT ) {
                        error_handling::Error error(
                            constStmnt->Line, name,
                            "F32 OVERFLOW :: Function return type is float, " + actualConstName + " is returned");
                        errors->emplace_back(error);
                        return false;
                    }
                }  break;
                case PRIMITIVE_TYPE::FLOAT64:
                    if( constStmnt->ConstType != ast::CONSTANT_TYPE::DOUBLE && constStmnt->ConstType != ast::CONSTANT_TYPE::FLOAT ) {
                        auto actualConstName = ast::GetConstantTypeName(constStmnt->ConstType);
                        auto name = constStmnt->FileName + ":" + funcNode->Name;
                        error_handling::Error error(
                            constStmnt->Line, name,
                            "Function return type is double point, " + actualConstName + " is returned");
                        errors->emplace_back(error);
                        return false;
                    }
                    break;
                }
            } else // Is returning a variable | check variable type
            {
            }
        }
        // Return type is Custom Type or built-in Type
        else {
            // Is returning a constant
            if( returnStmnt->Right->StmntType == ast::STATEMENT_TYPE::CONSTANT ) {
                auto constStmnt = std::static_pointer_cast<ast::ConstantNode, ast::StatementNode>( returnStmnt->Right );
                auto returnType = constStmnt->ConstType;
                if( funcNode->ReturnType == GetEnumValueName<built_in::BUILTIN_TYPE, built_in::BUILTIN_TYPE::STRING>()
                   && returnType != ast::CONSTANT_TYPE::STRING ) {
                    auto name = constStmnt->FileName + ":" + funcNode->Name;
                    error_handling::Error error(constStmnt->Line, name,
                                                "Function return type is string, no string type is returned");
                    errors->emplace_back(error);
                    return false;
                }
            }
        }
    }


    // check statements inside the block
    bool noInternalErrors = true;
    for( auto stmnt : funcNode->Block ) {
        bool noError = checkNode(stmnt, funcScopeNode);
        if( !noError )
            noInternalErrors = false;
    }

    return noInternalErrors;
}

bool SemanticAnalyzer::checkNode(std::shared_ptr<ast::VariableDefNode> varDefNode, Scope scope) {
    auto name = varDefNode->Name;
    auto symbolVec = scope->findSymbol(name, false);
    if( symbolVec.size() > 1 ) {
        // Redefinition error
        error_handling::Error error(varDefNode->Line, name, "Variable redefinition");
        errors->emplace_back(error);
        return false;
    }

    // TODO: Check if type exists

    // If it is a define/assign then check assignment
    if( varDefNode->assignmentStmnt ) {
        return checkNode(varDefNode->assignmentStmnt, scope);
    }

    return true;
}

bool SemanticAnalyzer::checkNode(std::shared_ptr<ast::VariableRefNode> varRefNode, Scope scope) {
    if(!varRefNode->WasFound) {
        auto name = varRefNode->Var->Name;
        auto symbolVec = scope->findSymbol(name, true);
        if( symbolVec.empty() ) {
            // Variable NotFound error
            error_handling::Error error(varRefNode->Line, name, "Variable not found");
            errors->emplace_back(error);
            return false;
        }
        varRefNode->Var = CastNode<ast::VariableDefNode>(symbolVec.front());
        varRefNode->WasFound = true;
    }

    return true;
}


bool SemanticAnalyzer::checkNode(std::shared_ptr<ast::AssignNode> assignmentNode, Scope scope) {
    // TODO: Check if types match

    // Check left node
    if( !checkNode(assignmentNode->Left, scope) )
        return false;

    // Check right node
    switch( assignmentNode->Right->GetType() ) {
    case ast::AST_TYPE::VariableRefNode:
    {
        auto rightNode = CastNode<ast::VariableRefNode>(assignmentNode->Right);
        if( !checkNode(rightNode, scope) )
            return false;
    } break;
    default:
        break;
    }

    return true;
}
