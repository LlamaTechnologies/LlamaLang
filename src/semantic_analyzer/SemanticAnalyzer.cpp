#include "SemanticAnalyzer.hpp"
#include "../ast/UnaryOperationNode.hpp"
#include "../ast/FunctionDefNode.hpp"
#include "../ast/ConstantNode.hpp"

namespace llang::semantics
{
    SemanticAnalyzer::ErrorList *SemanticAnalyzer::errors = nullptr;
    std::shared_ptr<ast::ProgramNode> SemanticAnalyzer::ast = nullptr;

    SemanticAnalyzer::SemanticAnalyzer(std::shared_ptr<ast::ProgramNode> ast,
                                       ErrorList &errors) {
        SemanticAnalyzer::ast = ast;
        SemanticAnalyzer::errors = &errors;
    }

    std::shared_ptr<ast::ProgramNode> SemanticAnalyzer::check() {
        ast->ForEachDeep([](ast::Node::ChildType child) { checkNode(child); });

        return ast;
    }

    bool SemanticAnalyzer::checkNode(ast::Node::ChildType node) {
        switch( node->GetType() ) {
        case ast::AST_TYPE::FunctionDefNode:
        {
            return checkNode(std::static_pointer_cast<ast::FunctionDefNode, ast::Node>( node ));
        } break;
        }
        return false;
    }

    bool SemanticAnalyzer::checkNode(std::shared_ptr<ast::FunctionDefNode> funcNode) {
        auto returnStmnt = std::static_pointer_cast<ast::UnaryOperationNode, ast::StatementNode>( funcNode->Block.back() );

        // return type is not void and the last statement isnt a return
        if( funcNode->ReturnType != "void" && returnStmnt->Op != ast::UNARY_STATEMENT_TYPE::RETURN ) {
            error_handling::Error error(funcNode->Line, funcNode->FileName,
                                        "Function " + funcNode->Name +
                                        " needs a return statement");
            errors->emplace_back(error);
            return false;
        }

        if( symbol_table::Symbol::IsPrimitive(funcNode->ReturnType) ) {
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
                case PRIMITIVE_TYPE::WCHAR:
                case PRIMITIVE_TYPE::UCHAR:
                case PRIMITIVE_TYPE::BYTE:
                case PRIMITIVE_TYPE::UINT8:
                case PRIMITIVE_TYPE::UINT16:
                case PRIMITIVE_TYPE::UINT32:
                case PRIMITIVE_TYPE::UINT64:
                    if( constStmnt->Value[0] == '-' ) {
                        auto name = constStmnt->FileName + ":" + funcNode->Name;
                        error_handling::Error error(
                            constStmnt->Line, name,
                            "Function return type is unsigned, signed type is returned");
                        errors->emplace_back(error);
                        return false;
                    }
                case PRIMITIVE_TYPE::SCHAR:
                case PRIMITIVE_TYPE::INT8:
                case PRIMITIVE_TYPE::INT16:
                case PRIMITIVE_TYPE::INT32:
                case PRIMITIVE_TYPE::INT64:
                    if( constStmnt->ConstType != ast::CONSTANT_TYPE::INTEGER &&
                       constStmnt->ConstType != ast::CONSTANT_TYPE::CHAR ) {
                        auto name = constStmnt->FileName + ":" + funcNode->Name;
                        error_handling::Error error(
                            (int) constStmnt->Line, name,
                            "Function return type is Integer, no integer is returned");
                        errors->emplace_back(error);
                        return false;
                    }
                    break;
                case PRIMITIVE_TYPE::FLOAT32:
                case PRIMITIVE_TYPE::FLOAT64:
                    if( constStmnt->ConstType != ast::CONSTANT_TYPE::FLOAT ) {
                        auto name = constStmnt->FileName + ":" + funcNode->Name;
                        error_handling::Error error(
                            constStmnt->Line, name,
                            "Function return type is Floating "
                            "point, no floating point is returned");
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
        return true;
    }
} // namespace llang::semantics