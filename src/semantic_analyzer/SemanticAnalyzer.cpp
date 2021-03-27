#include "SemanticAnalyzer.hpp"

#include "../ast/UnaryOperationNode.hpp"
#include "../ast/FunctionDefNode.hpp"
#include "../ast/FunctionCallNode.hpp"
#include "../ast/ConstantNode.hpp"
#include "../ast/VariableDefNode.hpp"
#include "../ast/VariableRefNode.hpp"
#include "../ast/UnaryOperationNode.hpp"
#include "../ast/AssignNode.hpp"
#include "../ast/StatementNode.hpp"
#include "../Console.hpp"

#include <exception>

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
    case ast::AST_TYPE::UnaryOperationNode:
        return checkNode(CastNode<ast::UnaryOperationNode>(node), scope);
    case ast::AST_TYPE::FunctionCallNode:
        return checkNode(CastNode<ast::FunctionCallNode>(node), scope);
    case ast::AST_TYPE::VariableRefNode:
        return checkNode(CastNode<ast::VariableRefNode>(node), scope);
    case ast::AST_TYPE::BinaryOperationNode:
        return checkNode(CastNode<ast::BinaryOperationNode>(node), scope);
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
    if( scope->getSymbolCount(name, false) > 1) {
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

bool SemanticAnalyzer::checkNode(std::shared_ptr<ast::FunctionCallNode> funcNode, Scope scope) {
    if (funcNode->functionFound == nullptr)
        return true;

    auto name = funcNode->Name;
    auto symbol = scope->findSymbol(name, true);
    if (symbol == nullptr) {
        // function NotFound error
        error_handling::Error error(funcNode->Line, funcNode->FileName, "Function not found: " + name);
        errors->emplace_back(error);
        return false;
    }

    funcNode->functionFound = CastNode<ast::FunctionDefNode>(symbol);

    // Check call
    auto expectedArgsCount = funcNode->functionFound->Parameters.size();
    auto argsCount = funcNode->Arguments.size();
    if (argsCount != expectedArgsCount) {
        // wrong amount of args error
        error_handling::Error error(funcNode->Line, funcNode->FileName,
            "Function " + name +" expect " + std::to_string(expectedArgsCount) + " args, passed " + std::to_string(argsCount) + " args.");
        errors->emplace_back(error);
        return false;
    }

    // check arguments types
    bool noErrorFound = true;
    for (auto i = 0; i < expectedArgsCount; ++i) {
        auto param = funcNode->functionFound->Parameters.at(i);
        auto argNode = funcNode->Arguments.at(i);
        std::string argType;

        switch (argNode->GetType()) {
        case ast::AST_TYPE::BinaryOperationNode: {
            auto binOp = CastNode<ast::BinaryOperationNode>(argNode);
            if (!checkNode(binOp, scope))
                break;
            argType = binOp->ResultType;
        } break;
        case ast::AST_TYPE::ConstantNode: {
            auto constNode = CastNode<ast::ConstantNode>(argNode);
            argType = Primitives::GetName(constNode->ConstType);
        }
        case ast::AST_TYPE::VariableRefNode: {
            auto varRefNode = CastNode<ast::VariableRefNode>(argNode);
            if (!checkNode(varRefNode, scope))
                break;
            argType = varRefNode->Var->VarType;
        } break;
        case ast::AST_TYPE::FunctionCallNode: {
            auto funcCallNodeArg = CastNode<ast::FunctionCallNode>(argNode);
            if (!checkNode(funcCallNodeArg, scope))
                break;
            argType = funcCallNodeArg->functionFound->ReturnType;
        } break;
        case ast::AST_TYPE::UnaryOperationNode:
            Console::WriteLine("UnaryOperation statement as argument is unsupported : line<" + std::to_string(funcNode->Line) + ">");
            break;
        default:
            Console::WriteLine("Illegal statement in argument <" + std::to_string(i) +
                "> in function <" + name + 
                "> in file <" + funcNode->FileName + 
                "> at line <" + std::to_string(funcNode->Line) + ">");
            break;
        }

        if (param->VarType != argType) {
            // wrong type error
            error_handling::Error error(funcNode->Line, funcNode->FileName,
                "Function " + name + " expect arg [" + std::to_string(i) + "] type to be <" + param->VarType + ">. Arg of type <" + argType + "> passed.");
            errors->emplace_back(error);
        }
    }

    return noErrorFound;
}

bool SemanticAnalyzer::checkNode(std::shared_ptr<ast::VariableRefNode> varRefNode, Scope scope) {
    if(!varRefNode->WasFound) {
        auto name = varRefNode->Var->Name;
        auto symbol = scope->findSymbol(name, true);
        if( symbol == nullptr) {
            // Variable NotFound error
            error_handling::Error error(varRefNode->Line, varRefNode->FileName, "Variable not found: " + name);
            errors->emplace_back(error);
            return false;
        }
        varRefNode->Var = CastNode<ast::VariableDefNode>(symbol);
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
    case ast::AST_TYPE::BinaryOperationNode:
    {
        auto rightNode = CastNode<ast::BinaryOperationNode>(assignmentNode->Right);
        if (!checkNode(rightNode, scope))
            return false;
    } break;
    default:
        break;
    }

    return true;
}

bool SemanticAnalyzer::checkNode(std::shared_ptr<ast::UnaryOperationNode> unaryOpNode, Scope scope) {
    switch (unaryOpNode->Op) {
    case ast::UNARY_STATEMENT_TYPE::RETURN:
        return checkNode(unaryOpNode->Right);
    default:
        return true;
    }
}

bool SemanticAnalyzer::checkNode(std::shared_ptr<ast::BinaryOperationNode> binaryOpNode, Scope scope) {
    auto leftOp = binaryOpNode->Left;
    auto rightOp = binaryOpNode->Right;

    // const & const
    if (leftOp->StmntType == ast::STATEMENT_TYPE::CONSTANT && rightOp->StmntType == ast::STATEMENT_TYPE::CONSTANT) {
        auto left = CastNode<ast::ConstantNode>(leftOp);
        auto right = CastNode<ast::ConstantNode>(rightOp);
        
        if (left->ConstType == right->ConstType) {
            binaryOpNode->ResultType = Primitives::GetName(right->ConstType);
            return true;
        }

        // fp & int
        if (left->ConstType >= ast::CONSTANT_TYPE::FLOAT && right->ConstType < ast::CONSTANT_TYPE::FLOAT) {
            binaryOpNode->ResultType = Primitives::GetName(left->ConstType);
            return false;
        }

        // int & fp
        if (left->ConstType >= ast::CONSTANT_TYPE::FLOAT && right->ConstType < ast::CONSTANT_TYPE::FLOAT) {
            binaryOpNode->ResultType = Primitives::GetName(right->ConstType);
            return false;
        }

        auto resultType = left->ConstType > right->ConstType ? left->ConstType : right->ConstType;
        left->ConstType = right->ConstType = resultType;
        binaryOpNode->ResultType = Primitives::GetName(resultType);
        return true;
    }

    // var & var
    if (leftOp->StmntType == ast::STATEMENT_TYPE::VAR_REF && rightOp->StmntType == ast::STATEMENT_TYPE::VAR_REF) {
        PRIMITIVE_TYPE left;
        PRIMITIVE_TYPE right;
        auto varL = CastNode<ast::VariableRefNode>(leftOp);
        auto varR = CastNode<ast::VariableRefNode>(rightOp);
        
        if (!checkNode(varL, scope) || !checkNode(varR, scope))
            return false;

        auto& varTypeL = varL->Var->VarType;
        auto& varTypeR = varR->Var->VarType;
        if (!Primitives::Exists(varTypeL) || !Primitives::Exists(varTypeR)) {
            error_handling::Error error(
                binaryOpNode->Line,
                binaryOpNode->FileName,
                "Custom types not supported."
            );
            errors->push_back(error);
            binaryOpNode->ResultType = "";
            return false;
        }
        left  = Primitives::Get(varTypeL);
        right = Primitives::Get(varTypeR);


        if (left == right) {
            binaryOpNode->ResultType = Primitives::GetName(left);
            return true;
        }

        // fp & int
        if (left >= PRIMITIVE_TYPE::FLOAT64 && right < PRIMITIVE_TYPE::FLOAT32) {
            binaryOpNode->ResultType = Primitives::GetName(left);
            return false;
        }

        // int & fp
        if (left >= PRIMITIVE_TYPE::FLOAT32 && right < PRIMITIVE_TYPE::FLOAT32) {
            binaryOpNode->ResultType = Primitives::GetName(right);
            return false;
        }

        PRIMITIVE_TYPE resultType;
        if (left > right) {
            resultType = left;
            varTypeR = std::string(varTypeL);
        }
        else {
            resultType = right;
            varTypeL = std::string(varTypeR);
        }

        binaryOpNode->ResultType = Primitives::GetName(resultType);
        return true;
    }
    
    // var & const
    if (leftOp->StmntType == ast::STATEMENT_TYPE::VAR_REF && rightOp->StmntType == ast::STATEMENT_TYPE::CONSTANT) {
        auto varRef = CastNode<ast::VariableRefNode>(leftOp);
        auto constant = CastNode<ast::ConstantNode>(rightOp);
       
        if (!checkVarAndConst(varRef, constant))
            return false;

        binaryOpNode->ResultType = varRef->Var->VarType;
        return true;
    }

    // const & var
    if (rightOp->StmntType == ast::STATEMENT_TYPE::VAR_REF && leftOp->StmntType == ast::STATEMENT_TYPE::CONSTANT) {
        auto varRef = CastNode<ast::VariableRefNode>(rightOp);
        auto constant = CastNode<ast::ConstantNode>(leftOp);

        if (!checkVarAndConst(varRef, constant))
            return false;

        binaryOpNode->ResultType = varRef->Var->VarType;
        return true;
    }
    

    // bin & const
    if (leftOp->StmntType == ast::STATEMENT_TYPE::BINARY_OP && rightOp->StmntType == ast::STATEMENT_TYPE::CONSTANT) {
        auto binOp = CastNode<ast::BinaryOperationNode>(leftOp);
        auto constant = CastNode<ast::ConstantNode>(rightOp);
        
        if (!checkNode(binOp, scope))
            return false;

        if (!checkBinOpAndConst(binOp, constant, scope))
            return false;

        binaryOpNode->ResultType = binOp->ResultType;
        return true;
    }

    // const & bin
    if (rightOp->StmntType == ast::STATEMENT_TYPE::BINARY_OP && leftOp->StmntType == ast::STATEMENT_TYPE::CONSTANT) {
        auto binOp = CastNode<ast::BinaryOperationNode>(rightOp);
        auto constant = CastNode<ast::ConstantNode>(leftOp);

        if (!checkNode(binOp, scope))
            return false;

        if (!checkBinOpAndConst(binOp, constant, scope))
            return false;

        binaryOpNode->ResultType = binOp->ResultType;
        return true;
    }

    // bin & var
    if (leftOp->StmntType == ast::STATEMENT_TYPE::BINARY_OP && rightOp->StmntType == ast::STATEMENT_TYPE::VAR_REF) {
        auto binOp = CastNode<ast::BinaryOperationNode>(leftOp);
        auto varRef = CastNode<ast::VariableRefNode>(rightOp);

        if (!checkNode(varRef, scope) || !checkNode(binOp, scope))
            return false;

        if (!checkBinOpAndVar(binOp, varRef, scope))
            return false;

        binaryOpNode->ResultType = binOp->ResultType;
        return true;
    }

    // var & bin
    if (rightOp->StmntType == ast::STATEMENT_TYPE::BINARY_OP && leftOp->StmntType == ast::STATEMENT_TYPE::VAR_REF) {
        auto binOp = CastNode<ast::BinaryOperationNode>(rightOp);
        auto varRef = CastNode<ast::VariableRefNode>(leftOp);

        if (!checkNode(varRef, scope) || !checkNode(binOp, scope))
            return false;

        if (!checkBinOpAndVar(binOp, varRef, scope))
            return false;

        binaryOpNode->ResultType = binOp->ResultType;
        return true;
    }

    error_handling::Error error(
        binaryOpNode->Line,
        binaryOpNode->FileName,
        "UNsuported combination in binary operator."
    );
    errors->push_back(error);
    return false;
}

bool SemanticAnalyzer::checkVarAndConst(std::shared_ptr< ast::VariableRefNode> varRef, std::shared_ptr< ast::ConstantNode> constant)
{
    if (Primitives::Exists(varRef->Var->VarType)) {
        auto varType = Primitives::Get(varRef->Var->VarType);
        auto constType = Primitives::Get(Primitives::GetName(constant->ConstType));

        if (varType >= PRIMITIVE_TYPE::FLOAT32 && constType < PRIMITIVE_TYPE::FLOAT32 ||
            varType < PRIMITIVE_TYPE::FLOAT32 && constType >= PRIMITIVE_TYPE::FLOAT32) {
            // FLOATING POINT AND INTEGER MIX
            error_handling::Error error(
                varRef->Line,
                varRef->FileName,
                "Operands not supported: Float and Integer mix in binary operation.");
            errors->emplace_back(error);
            return false;
        }

        if (varType >= constType) {
            constant->ConstType = ast::GetConstantType(varType);
            return true;
        }
    }

    error_handling::Error error(
        varRef->Line,
        varRef->FileName,
        "Custom types not supported."
    );
    errors->push_back(error);
    return false;
}

bool SemanticAnalyzer::checkBinOpAndVar(std::shared_ptr<ast::BinaryOperationNode> binOp, std::shared_ptr<ast::VariableRefNode> varRef, Scope scope)
{
    if (Primitives::Exists(varRef->Var->VarType)) {
        auto resultType = Primitives::Get(binOp->ResultType);
        auto varType = Primitives::Get(varRef->Var->VarType);

        if (resultType >= PRIMITIVE_TYPE::FLOAT32 && varType < PRIMITIVE_TYPE::FLOAT32 ||
            resultType < PRIMITIVE_TYPE::FLOAT32 && varType >= PRIMITIVE_TYPE::FLOAT32) {
            // FLOATING POINT AND INTEGER MIX
            error_handling::Error error(binOp->Line, binOp->FileName, "Operands not supported: Float and Integer mix in binary operation.");
            errors->emplace_back(error);
            return false;
        }

        if (resultType >= varType) {
            varRef->Var->VarType = Primitives::GetName(resultType);
            return true;
        }
    }

    error_handling::Error error(
        binOp->Line,
        binOp->FileName,
        "Custom types not supported."
    );
    errors->push_back(error);
    return false;
}

bool SemanticAnalyzer::checkBinOpAndConst(std::shared_ptr<ast::BinaryOperationNode> binOp, std::shared_ptr<ast::ConstantNode> constant, Scope scope)
{
    if (binOp->ResultType.size()) {
        auto resultType = Primitives::Get(binOp->ResultType);
        auto constType = Primitives::Get(Primitives::GetName(constant->ConstType));
        if (resultType >= PRIMITIVE_TYPE::FLOAT32 && constType < PRIMITIVE_TYPE::FLOAT32 ||
            resultType < PRIMITIVE_TYPE::FLOAT32 && constType >= PRIMITIVE_TYPE::FLOAT32) {
            // FLOATING POINT AND INTEGER MIX
            error_handling::Error error(binOp->Line, binOp->FileName, "Operands not supported: Float and Integer mix in binary operation.");
            errors->emplace_back(error);
            return false;
        }

        if (resultType >= constType) {
            constant->ConstType = ast::GetConstantType(resultType);
            return true;
        }
    }

    error_handling::Error error(
        binOp->Line,
        binOp->FileName,
        "Custom types not supported."
    );
    errors->push_back(error);
    return false;
}
