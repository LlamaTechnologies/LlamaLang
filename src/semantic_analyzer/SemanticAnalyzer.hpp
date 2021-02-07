#pragma once
#include <vector>
#include <memory>
#include "ast/ProgramNode.hpp"
#include "ast/FunctionNode.hpp"
#include "ast/UnaryStatementNode.hpp"
#include "error_handling/ErrorDescription.hpp"
#include "symbol_table/Symbol.hpp"

namespace llang::semantics
{
    struct SemanticAnalyzer
    {
        using ErrorList = std::vector<std::shared_ptr<error_handling::Error>>;
        std::shared_ptr<ast::ProgramNode> ast;
        ErrorList errors;

        SemanticAnalyzer(std::shared_ptr<ast::ProgramNode> ast, ErrorList &errors)
        : ast(ast), errors(errors) {}

        std::shared_ptr<ast::ProgramNode> check()
        {
            ast->ForEachDeep(
                [] (auto child) {
                    checkNode(child);
                });
            
            return ast;
        }

    private:
        bool checkNode(std::shared_ptr<ast::RegisterAstType> node) {
            switch (node->AstType)
            {
                case ast::AST_TYPE::FunctionNode:
                {
                    checkNode(std::static_pointer_cast<ast::FunctionNode, ast::RegisterAstType>(node));
                } break;
            }
        }

        bool checkNode(std::shared_ptr<ast::FunctionNode> funcNode)
        {
            auto returnStmnt = (ast::UnaryStatementNode)funcNode.Block.FindLast(stmnt => stmnt.StmntType == STATEMENT_TYPE.RETURN);

            if (symbol_table::Symbol.IsPrimitive(funcNode.ReturnType))
            {
                auto retType = Primitives.Get(funcNode.ReturnType);
                
                if (retType == PRIMITIVE_TYPE.VOID)
                    return returnStmnt.Right.StmntType == STATEMENT_TYPE.EMPTY;

                else if (returnStmnt.Right.StmntType == STATEMENT_TYPE.CONSTANT)
                {
                    auto constStmnt = (ASTConstantNode)returnStmnt.Right;
                    switch (retType)
                    {
                        case PRIMITIVE_TYPE.CHAR:
                        case PRIMITIVE_TYPE.WCHAR:
                        case PRIMITIVE_TYPE.UCHAR:
                        case PRIMITIVE_TYPE.BYTE:
                        case PRIMITIVE_TYPE.UINT8:
                        case PRIMITIVE_TYPE.UINT16:
                        case PRIMITIVE_TYPE.UINT32:
                        case PRIMITIVE_TYPE.UINT64:
                            if (constStmnt.Value[0] == '-')
                            {
                                var name = constStmnt.File + ":" + funcNode.Name;
                                var error = new ErrorDescription(constStmnt.Line, name, "Function return type is unsigned, signed type is returned");
                                errors.Add(error);
                                return false;
                            }
                            goto case(PRIMITIVE_TYPE.INT8);
                        case PRIMITIVE_TYPE.SCHAR:
                        case PRIMITIVE_TYPE.INT8:
                        case PRIMITIVE_TYPE.INT16:
                        case PRIMITIVE_TYPE.INT32:
                        case PRIMITIVE_TYPE.INT64:
                            if (constStmnt.ConstType != CONSTANT_TYPE.INTEGER && constStmnt.ConstType != CONSTANT_TYPE.CHAR)
                            {
                                var name = constStmnt.File + ":" + funcNode.Name;
                                var error = new ErrorDescription(constStmnt.Line, name, "Function return type is Integer, no integer is returned");
                                errors.Add(error);
                                return false;
                            }
                            break;
                        case PRIMITIVE_TYPE.FLOAT32:
                        case PRIMITIVE_TYPE.FLOAT64:
                            if (constStmnt.ConstType != CONSTANT_TYPE.FLOAT)
                            {
                                var name = constStmnt.File + ":" + funcNode.Name;
                                var error = new ErrorDescription(constStmnt.Line, name, "Function return type is Floating point, no floating point is returned");
                                errors.Add(error);
                                return false;
                            }
                            break;
                    }
                }
                else // Is returning a variable | check variable type
                {
                    
                }
            }
            // Is Custom or built-in Type
            else
            {
                // Is returning a constant
                if (returnStmnt.Right.StmntType == STATEMENT_TYPE.CONSTANT) {
                    var constStmnt = (ASTConstantNode)returnStmnt.Right;
                    var returnType = constStmnt.ConstType;
                    if (funcNode.ReturnType == Enum.GetName(typeof(BUILTIN_TYPE), BUILTIN_TYPE.STRING)
                        && returnType != CONSTANT_TYPE.STRING)
                    {
                        var name = constStmnt.File + ":" + funcNode.Name;
                        var error = new ErrorDescription(constStmnt.Line, name, "Function return type is string, no string type is returned");
                        errors.Add(error);
                        return false;
                    }
                }
                    
            }
            return true;
        }
        
    }
}