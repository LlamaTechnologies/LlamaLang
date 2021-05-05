#include "SemanticAnalyzer.hpp"

#include "../ast/UnaryOperationNode.hpp"
#include "../ast/FunctionDefNode.hpp"
#include "../ast/FunctionCallNode.hpp"
#include "../ast/ConstantNode.hpp"
#include "../ast/CastOperationNode.hpp"
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
                                   ErrorList &errors)
{
  SemanticAnalyzer::ast = ast;
  SemanticAnalyzer::globalScope = scope;
  SemanticAnalyzer::errors = &errors;
}

std::shared_ptr<ast::ProgramNode> SemanticAnalyzer::check()
{
  for (auto child : ast->children) {
    auto nodeType = child->GetType();

    switch (nodeType) {
    case ast::AST_TYPE::FunctionDefNode:
    case ast::AST_TYPE::VariableDefNode:
      checkNode(child);
      // TODO: support struct|union
      // TODO: support type definitions
      // TODO: support namespaces
    default:
      break;
    }
  }

  return ast;
}

bool SemanticAnalyzer::checkNode(ast::Node::ChildType node, Scope scope)
{
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

bool SemanticAnalyzer::checkNode(std::shared_ptr<ast::StatementNode> node, Scope scope)
{
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

  // TODO: Add unary suppoort
  default:
    return false;
  }
}

bool SemanticAnalyzer::checkNode(std::shared_ptr<ast::FunctionDefNode> funcNode, Scope scope)
{
  auto returnStmnt = std::static_pointer_cast<ast::UnaryOperationNode, ast::StatementNode>( funcNode->Block.back() );

  // Check redefinition
  auto scopesVec = scope->children.at(funcNode->Name);

  if( scopesVec.size() > 1 ) {
    return false;
  }

  auto funcScopeNode = scopesVec.front();

  {
    // Function Signature and return type check
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
        case PRIMITIVE_TYPE::UINT8:
        case PRIMITIVE_TYPE::UINT16:
        case PRIMITIVE_TYPE::UINT32:
        case PRIMITIVE_TYPE::UINT64:
          if( constStmnt->Value[0] == '-' ) {
            auto name = constStmnt->FileName + ":" + funcNode->Name;
            error_handling::Error error(
              constStmnt->Line, name,
              "SIGN MISMATCH :: Function return type is unsigned, signed type is returned");
            errors->emplace_back(error);
            return false;
          }

          break;

        case PRIMITIVE_TYPE::INT8: {
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
        }
        break;

        case PRIMITIVE_TYPE::INT16: {
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
        }
        break;

        case PRIMITIVE_TYPE::INT32: {
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
        }
        break;

        case PRIMITIVE_TYPE::INT64: {
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
        }
        break;

        case PRIMITIVE_TYPE::FLOAT32: {
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
        }
        break;

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
      } else { // Is returning a variable | check variable type
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

bool SemanticAnalyzer::checkNode(std::shared_ptr<ast::VariableDefNode> varDefNode, Scope scope)
{
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

bool SemanticAnalyzer::checkNode(std::shared_ptr<ast::FunctionCallNode> funcNode, Scope scope)
{
  if (funcNode->functionFound != nullptr)
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
                                "Function " + name + " expect " + std::to_string(expectedArgsCount) + " args, passed " + std::to_string(argsCount) + " args.");
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
    }
    break;

    case ast::AST_TYPE::ConstantNode: {
      auto constNode = CastNode<ast::ConstantNode>(argNode);
      argType = Primitives::GetName(constNode->ConstType);
    }
    break;

    case ast::AST_TYPE::VariableRefNode: {
      auto varRefNode = CastNode<ast::VariableRefNode>(argNode);

      if (!checkNode(varRefNode, scope))
        break;

      argType = varRefNode->Var->VarType;
    }
    break;

    case ast::AST_TYPE::FunctionCallNode: {
      auto funcCallNodeArg = CastNode<ast::FunctionCallNode>(argNode);

      if (!checkNode(funcCallNodeArg, scope))
        break;

      argType = funcCallNodeArg->functionFound->ReturnType;
    }
    break;

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

bool SemanticAnalyzer::checkNode(std::shared_ptr<ast::VariableRefNode> varRefNode, Scope scope)
{
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

bool SemanticAnalyzer::checkNode(std::shared_ptr<ast::AssignNode> assignmentNode, Scope scope)
{
  auto left = assignmentNode->Left;
  auto lType = left->Var->VarType;

  // Check left node
  if (!checkNode(left, scope))
    return false;

  // Check right node
  auto right = assignmentNode->Right;

  if (!right)
    return false;

  switch (right->GetType()) {
  case ast::AST_TYPE::ConstantNode: {
    auto rightNode = CastNode<ast::ConstantNode>(assignmentNode->Right);

    // This is a constant so we just upcast the constant
    if (!checkCastVarAndConst(left, rightNode))
      return false;
  }
  break;

  case ast::AST_TYPE::VariableRefNode: {
    auto rightNode = CastNode<ast::VariableRefNode>(assignmentNode->Right);

    if (!checkNode(rightNode, scope))
      return false;

    PRIMITIVE_TYPE type = PRIMITIVE_TYPE::VOID;
    std::shared_ptr<ast::VariableRefNode> nodeToMod = nullptr;

    if (!checkCastVarAndVar(left, rightNode, type, nodeToMod))
      return false;

    // modify ast and add cast
    if (nodeToMod) {
      // since this is an assignment override values to cast to the store type
      nodeToMod = rightNode;
      type = Primitives::Get(left->Var->VarType);
      modAstWithCast(assignmentNode, nodeToMod, type);
    }
  }
  break;

  case ast::AST_TYPE::BinaryOperationNode: {
    auto rightNode = CastNode<ast::BinaryOperationNode>(assignmentNode->Right);

    if (!checkNode(rightNode, scope))
      return false;

    PRIMITIVE_TYPE type;
    std::shared_ptr<ast::StatementNode> nodeToMod = nullptr;

    if (!checkCastBinOpAndVar(rightNode, left, type, nodeToMod))
      return false;

    // modify ast and add cast
    if (nodeToMod) {
      // since this is an assignment override values to cast to the store type
      nodeToMod = rightNode;
      type = Primitives::Get(left->Var->VarType);
      modAstWithCast(assignmentNode, nodeToMod, type);
    }
  }
  break;

  case ast::AST_TYPE::FunctionCallNode: {
    auto rightNode = CastNode<ast::FunctionCallNode>(assignmentNode->Right);

    if (!checkNode(rightNode, scope))
      return false;

    PRIMITIVE_TYPE type;
    std::shared_ptr<ast::StatementNode> nodeToMod = nullptr;

    if (!checkCastCallAndVar(rightNode, left, type, nodeToMod))
      return false;

    // modify ast and add cast
    if (nodeToMod) {
      // since this is an assignment override values to cast to the store type
      nodeToMod = rightNode;
      type = Primitives::Get(left->Var->VarType);
      modAstWithCast(assignmentNode, nodeToMod, type);
    }

  }
  break;

  default:
    break;
  }

  return true;
}

bool SemanticAnalyzer::checkNode(std::shared_ptr<ast::UnaryOperationNode> unaryOpNode, Scope scope)
{
  switch (unaryOpNode->Op) {
  case ast::UNARY_STATEMENT_TYPE::RETURN: {
    if (unaryOpNode->Right->StmntType == ast::STATEMENT_TYPE::UNARY_OP &&
        CastNode<ast::UnaryOperationNode>(unaryOpNode->Right)->Op == ast::UNARY_STATEMENT_TYPE::RETURN) {
      error_handling::Error error(
        unaryOpNode->Line,
        unaryOpNode->FileName,
        "Nested return staements are not allowed!"
      );
      errors->push_back(error);
      return false;
    }

    if (!checkNode(unaryOpNode->Right, scope))
      return false;

    auto func = CastNode<ast::FunctionDefNode>(scope->Data);
    bool shouldCast = false;

    if (!checkCastRet(unaryOpNode->Right, func->ReturnType, shouldCast))
      return false;

    if (shouldCast) {
      modAstWithCast(unaryOpNode, unaryOpNode->Right, Primitives::Get(func->ReturnType));
    }

    return true;
  }
  break;

  default:
    return true;
  }
}

bool SemanticAnalyzer::checkNode(std::shared_ptr<ast::BinaryOperationNode> binaryOpNode, Scope scope)
{
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

    PRIMITIVE_TYPE resultType;
    std::shared_ptr<ast::VariableRefNode> nodeToMod;

    if (!checkCastVarAndVar(varL, varR, resultType, nodeToMod))
      return false;


    if (nodeToMod) {
      modAstWithCast(binaryOpNode, nodeToMod, resultType);
    }

    binaryOpNode->ResultType = Primitives::GetName(resultType);
    return true;
  }

  // var & const
  if (leftOp->StmntType == ast::STATEMENT_TYPE::VAR_REF && rightOp->StmntType == ast::STATEMENT_TYPE::CONSTANT) {
    auto varRef = CastNode<ast::VariableRefNode>(leftOp);
    auto constant = CastNode<ast::ConstantNode>(rightOp);

    if (!checkNode(varRef, scope))
      return false;

    if (!checkCastVarAndConst(varRef, constant))
      return false;

    binaryOpNode->ResultType = varRef->Var->VarType;
    return true;
  }

  // const & var
  if (rightOp->StmntType == ast::STATEMENT_TYPE::VAR_REF && leftOp->StmntType == ast::STATEMENT_TYPE::CONSTANT) {
    auto varRef = CastNode<ast::VariableRefNode>(rightOp);
    auto constant = CastNode<ast::ConstantNode>(leftOp);

    if (!checkNode(varRef, scope))
      return false;

    if (!checkCastVarAndConst(varRef, constant))
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

    if (!checkCastBinOpAndConst(binOp, constant))
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

    if (!checkCastBinOpAndConst(binOp, constant))
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


    PRIMITIVE_TYPE resultType;
    std::shared_ptr<ast::StatementNode> nodeToMod;

    if (!checkCastBinOpAndVar(binOp, varRef, resultType, nodeToMod))
      return false;

    if (nodeToMod) {
      modAstWithCast(binaryOpNode, nodeToMod, resultType);
    }

    binaryOpNode->ResultType = Primitives::GetName(resultType);
    return true;
  }

  // var & bin
  if (rightOp->StmntType == ast::STATEMENT_TYPE::BINARY_OP && leftOp->StmntType == ast::STATEMENT_TYPE::VAR_REF) {
    auto binOp = CastNode<ast::BinaryOperationNode>(rightOp);
    auto varRef = CastNode<ast::VariableRefNode>(leftOp);

    if (!checkNode(varRef, scope) || !checkNode(binOp, scope))
      return false;


    PRIMITIVE_TYPE resultType;
    std::shared_ptr<ast::StatementNode> nodeToMod;

    if (!checkCastBinOpAndVar(binOp, varRef, resultType, nodeToMod))
      return false;

    if (nodeToMod) {
      modAstWithCast(binaryOpNode, nodeToMod, resultType);
    }

    binaryOpNode->ResultType = Primitives::GetName(resultType);
    return true;
  }

  // TODO: add function call combination
  // func/const
  // const/func
  // func/var
  // var/func
  // func/bin
  // bin/func


  // TODO: add unary op combination

  error_handling::Error error(
    binaryOpNode->Line,
    binaryOpNode->FileName,
    "Unsuported combination in binary operator."
  );
  errors->push_back(error);
  return false;
}

bool SemanticAnalyzer::checkCastRet(std::shared_ptr<ast::StatementNode> retValue, const std::string &returnType, bool &shouldCast)
{
  if (Primitives::Exists(returnType)) {
    PRIMITIVE_TYPE resultType;

    auto retType = Primitives::Get(returnType);

    switch (retValue->StmntType) {
    case ast::STATEMENT_TYPE::CONSTANT: {
      // Cast constant to ret type
      auto constNode = CastNode<ast::ConstantNode>(retValue);
      auto constType = Primitives::Get(Primitives::GetName(constNode->ConstType));

      if (!checkCastTypes(constType, retType, resultType, shouldCast))
        return false;
    }
    break;

    case ast::STATEMENT_TYPE::VAR_REF: {
      auto varRefNode = CastNode<ast::VariableRefNode>(retValue);
      auto varType = Primitives::Get(varRefNode->Var->VarType);

      if (!checkCastTypes(varType, retType, resultType, shouldCast))
        return false;
    }
    break;

    case ast::STATEMENT_TYPE::UNARY_OP:

    // INC DEC

    case ast::STATEMENT_TYPE::BINARY_OP: {
      auto binOpNode = CastNode<ast::BinaryOperationNode>(retValue);
      auto binResType = Primitives::Get(binOpNode->ResultType);

      if (!checkCastTypes(binResType, retType, resultType, shouldCast))
        return false;
    }
    break;

    case ast::STATEMENT_TYPE::CALL: {
      auto funcCallNode = CastNode<ast::FunctionCallNode>(retValue);
      auto funcRetType = Primitives::Get(funcCallNode->functionFound->ReturnType);

      if (!checkCastTypes(funcRetType, retType, resultType, shouldCast))
        return false;
    }
    break;

    default:
      return false;
    }

    return true;
  }


  // Custom return types are not supported
  // returns false on custom types
  error_handling::Error error(
    retValue->Line,
    retValue->FileName,
    "Custom types not supported."
  );
  errors->push_back(error);
  return false;
}

bool SemanticAnalyzer::checkCastVarAndConst(std::shared_ptr<ast::VariableRefNode> varRef, std::shared_ptr<ast::ConstantNode> constant)
{
  // Var type is primitive
  if (Primitives::Exists(varRef->Var->VarType)) {
    auto varType = Primitives::Get(varRef->Var->VarType);
    auto constType = Primitives::Get(Primitives::GetName(constant->ConstType));

    // if equals then return true
    if (varType == constType) {
      return true;
    }

    // NOTE: DOES NOT SUPPORT BOOL TO OTHER TYPES IMPLICIT CASTING

    // fp|int check
    if ((varType >= PRIMITIVE_TYPE::FLOAT32 && constType < PRIMITIVE_TYPE::FLOAT32) ||
        (varType < PRIMITIVE_TYPE::FLOAT32 && constType >= PRIMITIVE_TYPE::FLOAT32)) {
      Console::WriteLine("WARNING: Float and Integer mix in binary operation.");
    }

    // sign flags
    bool isVarSigned = (varType <= PRIMITIVE_TYPE::SCHAR && varType >= PRIMITIVE_TYPE::INT8) || (varType >= PRIMITIVE_TYPE::FLOAT32 && varType <= PRIMITIVE_TYPE::FLOAT64);
    bool isConstSigned = (constType <= PRIMITIVE_TYPE::SCHAR && constType >= PRIMITIVE_TYPE::INT8) || (constType >= PRIMITIVE_TYPE::FLOAT32 && constType <= PRIMITIVE_TYPE::FLOAT64);

    // sign mix check
    if (isVarSigned) {
      if (!isConstSigned) {
        Console::WriteLine("WARN: Constant does not match signed type for variable " + varRef->Var->Name);
      } else if (constType <= PRIMITIVE_TYPE::BOOL) {
        Console::WriteLine("Constant is not a valid type for variable " + varRef->Var->Name + " of type " + Primitives::GetName(varType));
        return false;
      }
    } else {
      if (!isConstSigned) {
        Console::WriteLine("WARN: Constant does not match unsigned type for variable " + varRef->Var->Name);
      } else if (constType <= PRIMITIVE_TYPE::BOOL || constType > PRIMITIVE_TYPE::FLOAT64) {
        Console::WriteLine("Constant is not a valid type for variable " + varRef->Var->Name + " of type " + Primitives::GetName(varType));
        return false;
      }
    }

    // cast constant to variable type
    if (varType > constType) {
      constant->ConstType = ast::GetConstantType(varType);
    }

    return true;
  }

  // returns false on custom types
  error_handling::Error error(
    varRef->Line,
    varRef->FileName,
    "Custom types not supported."
  );
  errors->push_back(error);
  return false;
}

bool SemanticAnalyzer::checkCastVarAndVar(std::shared_ptr<ast::VariableRefNode> varRefL, std::shared_ptr<ast::VariableRefNode> varRefR, PRIMITIVE_TYPE &resultType, std::shared_ptr<ast::VariableRefNode> &nodeToMod)
{
  auto &varTypeL = varRefL->Var->VarType;
  auto &varTypeR = varRefR->Var->VarType;

  // Var types are primitives
  if (Primitives::Exists(varTypeL) && Primitives::Exists(varTypeR)) {
    PRIMITIVE_TYPE left = Primitives::Get(varTypeL);
    PRIMITIVE_TYPE right = Primitives::Get(varTypeR);

    // NOTE: DOES NOT SUPPORT BOOL TO OTHER TYPES IMPLICIT CASTING

    // if equals then return true
    if (left == right) {
      resultType = right;
      return true;
    }

    // fp|int check
    if ((left >= PRIMITIVE_TYPE::FLOAT32 && right < PRIMITIVE_TYPE::FLOAT32) ||
        (left < PRIMITIVE_TYPE::FLOAT32 && right >= PRIMITIVE_TYPE::FLOAT32)) {
      Console::WriteLine("WARNING: Float and Integer mix in binary operation.");
    }

    // sign flags
    bool isLeftSigned = (left >= PRIMITIVE_TYPE::INT8 && left <= PRIMITIVE_TYPE::INT64) || (left >= PRIMITIVE_TYPE::FLOAT32 && left <= PRIMITIVE_TYPE::FLOAT64);
    bool isRightSigned = (right >= PRIMITIVE_TYPE::INT8 && right <= PRIMITIVE_TYPE::INT64) || (right >= PRIMITIVE_TYPE::FLOAT32 && right <= PRIMITIVE_TYPE::FLOAT64);

    if (isLeftSigned) {
      if (!isRightSigned) {
        Console::WriteLine("Variable " + varRefR->Var->Name + " does not match signed type for variable " + varRefL->Var->Name);
      } else if (right == PRIMITIVE_TYPE::VOID || right > PRIMITIVE_TYPE::FLOAT64) {
        Console::WriteLine("Variable " + varRefR->Var->Name + " is not a valid type for variable " + varRefL->Var->Name + " of type " + Primitives::GetName(left));
        return false;
      }
    } else {
      if (!isLeftSigned) {
        Console::WriteLine("Variable " + varRefL->Var->Name + " does not match unsigned type for variable " + varRefR->Var->Name);
      } else if (left == PRIMITIVE_TYPE::VOID || left > PRIMITIVE_TYPE::FLOAT64) {
        Console::WriteLine("Variable " + varRefL->Var->Name + " is not a valid type for variable " + varRefR->Var->Name + " of type " + Primitives::GetName(left));
        return false;
      }
    }

    // cast types
    if (left > right) {
      resultType = left;
      nodeToMod = varRefR;
    } else if (right > left) {
      resultType = right;
      nodeToMod = varRefL;
    }

    // returns true and node to modify
    return true;
  }

  // returns false on custom types
  error_handling::Error error(
    varRefL->Line,
    varRefL->FileName,
    "Custom types need to be cast explicitly."
  );
  errors->push_back(error);
  return false;
}

bool SemanticAnalyzer::checkCastBinOpAndConst(std::shared_ptr<ast::BinaryOperationNode> binOp, std::shared_ptr<ast::ConstantNode> constant)
{
  // Binary op result type is primitive
  if (binOp->ResultType.size() && Primitives::Exists(binOp->ResultType)) {
    auto resultType = Primitives::Get(binOp->ResultType);

    // names
    const auto resultTypeName = Primitives::GetName(resultType);
    const auto constTypeName = Primitives::GetName(constant->ConstType);
    const auto opName = GetBinaryOperationName(binOp->Op);

    auto constType = Primitives::Get(constTypeName);

    // if equals then return true
    if (resultType == constType) {
      return true;
    }


    // fp|int check
    if ((resultType >= PRIMITIVE_TYPE::FLOAT32 && constType < PRIMITIVE_TYPE::FLOAT32) ||
        (resultType < PRIMITIVE_TYPE::FLOAT32 && constType >= PRIMITIVE_TYPE::FLOAT32)) {
      Console::WriteLine("WARNING: Float and Integer mix in binary operation.");
    }

    // sign falgs
    bool isBinSigned = (resultType >= PRIMITIVE_TYPE::INT8 && resultType <= PRIMITIVE_TYPE::INT64) || (resultType >= PRIMITIVE_TYPE::FLOAT32 && resultType <= PRIMITIVE_TYPE::FLOAT64);
    bool isConstSigned = (constType >= PRIMITIVE_TYPE::INT8 && constType <= PRIMITIVE_TYPE::INT64) || (constType >= PRIMITIVE_TYPE::FLOAT32 && constType <= PRIMITIVE_TYPE::FLOAT64);

    // signs check
    if (isBinSigned) {
      if (!isConstSigned) {
        Console::WriteLine("Const " + constant->Value + " does not match unsigned type for operation" + opName);
      } else if (constType <= PRIMITIVE_TYPE::BOOL || constType > PRIMITIVE_TYPE::FLOAT64) {
        Console::WriteLine("Const " + constant->Value + " is not a valid type for op " + opName + " of type " + resultTypeName);
        return false;
      }
    } else {
      if (!isBinSigned) {
        Console::WriteLine("Const " + constant->Value + " does not match unsigned type for operation" + opName);
      } else if (resultType <= PRIMITIVE_TYPE::BOOL || constType > PRIMITIVE_TYPE::FLOAT64) {
        Console::WriteLine("Const " + constant->Value + " is not a valid type for op " + opName + " of type " + resultTypeName);
        return false;
      }
    }

    // set bin op result type as the new constant type
    constant->ConstType = ast::GetConstantType(resultType);
    return true;
  }

  // returns false on custom types
  error_handling::Error error(
    binOp->Line,
    binOp->FileName,
    "Custom types not supported."
  );
  errors->push_back(error);
  return false;
}

bool SemanticAnalyzer::checkCastBinOpAndVar(std::shared_ptr<ast::BinaryOperationNode> binOp, std::shared_ptr<ast::VariableRefNode> varRef, PRIMITIVE_TYPE &resultType, std::shared_ptr<ast::StatementNode> &nodeToMod)
{
  // var type and bin op result type are primitives
  if (Primitives::Exists(varRef->Var->VarType) && Primitives::Exists(binOp->ResultType)) {
    auto binOpType = Primitives::Get(binOp->ResultType);
    auto varType = Primitives::Get(varRef->Var->VarType);
    const auto binTypeName = Primitives::GetName(binOpType);
    const auto opName = GetBinaryOperationName(binOp->Op);

    // if equals then return true
    if (binOpType == varType) {
      resultType = varType;
      return true;
    }

    // fp|int check
    if ((binOpType >= PRIMITIVE_TYPE::FLOAT32 && varType < PRIMITIVE_TYPE::FLOAT32) ||
        (binOpType < PRIMITIVE_TYPE::FLOAT32 && varType >= PRIMITIVE_TYPE::FLOAT32)) {
      Console::WriteLine("WARNING: Float and Integer mix in binary operation.");
    }

    // sign flags
    bool isBinSigned = (binOpType >= PRIMITIVE_TYPE::INT8 && binOpType <= PRIMITIVE_TYPE::INT64) || (binOpType >= PRIMITIVE_TYPE::FLOAT32 && binOpType <= PRIMITIVE_TYPE::FLOAT64);
    bool isVarSigned = (varType >= PRIMITIVE_TYPE::INT8 && varType <= PRIMITIVE_TYPE::INT64) || (varType >= PRIMITIVE_TYPE::FLOAT32 && varType <= PRIMITIVE_TYPE::FLOAT64);

    if (isBinSigned) {
      if (!isVarSigned) {
        Console::WriteLine("Variable " + varRef->Var->Name + " does not match signed type for op " + opName);
      } else if (varType == PRIMITIVE_TYPE::VOID || varType > PRIMITIVE_TYPE::FLOAT64) {
        Console::WriteLine("Variable " + varRef->Var->Name + " is not a valid type for op " + opName + " of type " + binTypeName);
        return false;
      }
    } else {
      if (!isBinSigned) {
        Console::WriteLine("Variable " + varRef->Var->Name + " does not match signed type for op " + opName);
      } else if (binOpType == PRIMITIVE_TYPE::VOID || binOpType > PRIMITIVE_TYPE::FLOAT64) {
        Console::WriteLine("Variable " + varRef->Var->Name + " is not a valid type for op " + opName + " of type " + binTypeName);
        return false;
      }
    }

    if (binOpType > varType) {
      resultType = binOpType;
      nodeToMod = varRef;
    } else if (varType > binOpType) {
      resultType = varType;
      nodeToMod = binOp;
    }

    return true;
  }

  // returns false on custom types
  error_handling::Error error(
    varRef->Line,
    varRef->FileName,
    "Custom types need to be cast explicitly."
  );
  errors->push_back(error);
  return false;
}

bool SemanticAnalyzer::checkCastCallAndVar(std::shared_ptr<ast::FunctionCallNode> callOp, std::shared_ptr<ast::VariableRefNode> varRef, PRIMITIVE_TYPE &resultType, std::shared_ptr<ast::StatementNode> &nodeToMod)
{
  // var type and bin op result type are primitives
  if (Primitives::Exists(varRef->Var->VarType) && Primitives::Exists(callOp->functionFound->ReturnType)) {
    auto callOpType = Primitives::Get(callOp->functionFound->ReturnType);
    auto varType = Primitives::Get(varRef->Var->VarType);
    const auto callTypeName = Primitives::GetName(callOpType);
    const auto varTypeName = Primitives::GetName(varType);

    // if equals then return true
    if (callOpType == varType) {
      resultType = varType;
      return true;
    }

    // fp|int check
    if ((callOpType >= PRIMITIVE_TYPE::FLOAT32 && varType < PRIMITIVE_TYPE::FLOAT32) ||
        (callOpType < PRIMITIVE_TYPE::FLOAT32 && varType >= PRIMITIVE_TYPE::FLOAT32)) {
      Console::WriteLine("WARNING: Float and Integer mix in binary operation.");
    }

    // sign flags
    bool isCallRetSigned = (callOpType >= PRIMITIVE_TYPE::INT8 && callOpType <= PRIMITIVE_TYPE::INT64) || (callOpType >= PRIMITIVE_TYPE::FLOAT32 && callOpType <= PRIMITIVE_TYPE::FLOAT64);
    bool isVarSigned = (varType >= PRIMITIVE_TYPE::INT8 && varType <= PRIMITIVE_TYPE::INT64) || (varType >= PRIMITIVE_TYPE::FLOAT32 && varType <= PRIMITIVE_TYPE::FLOAT64);

    if (isCallRetSigned) {
      if (!isVarSigned) {
        Console::WriteLine("Function " + callTypeName + " return type does not match signed type for variable" + varRef->Var->Name);
      } else if (varType == PRIMITIVE_TYPE::VOID || varType > PRIMITIVE_TYPE::FLOAT64) {
        Console::WriteLine("Function " + callTypeName + " return type is not valid for variable " + varRef->Var->Name + " of type " + varTypeName);
        return false;
      }
    } else {
      if (!isCallRetSigned) {
        Console::WriteLine("Function " + callTypeName + " return type does not match signed type for variable" + varRef->Var->Name);
      } else if (callOpType == PRIMITIVE_TYPE::VOID || callOpType > PRIMITIVE_TYPE::FLOAT64) {
        Console::WriteLine("Function " + callTypeName + " return type is not valid for variable " + varRef->Var->Name + " of type " + varTypeName);
        return false;
      }
    }

    if (callOpType > varType) {
      resultType = callOpType;
      nodeToMod = varRef;
    } else if (varType > callOpType) {
      resultType = varType;
      nodeToMod = callOp;
    }

    return true;
  }

  // returns false on custom types
  error_handling::Error error(
    varRef->Line,
    varRef->FileName,
    "Custom types need to be cast explicitly."
  );
  errors->push_back(error);
  return false;
}

bool SemanticAnalyzer::checkCastTypes(const PRIMITIVE_TYPE type0, const PRIMITIVE_TYPE type1, PRIMITIVE_TYPE &resultType, bool &shouldCast)
{
  const auto type0Name = Primitives::GetName(type0);
  const auto type1Name = Primitives::GetName(type1);

  shouldCast = false;

  // if equals then return true
  if (type0 == type1) {
    resultType = type0;
    return true;
  }

  // fp|int check
  if ((type0 >= PRIMITIVE_TYPE::FLOAT32 && type1 < PRIMITIVE_TYPE::FLOAT32) ||
      (type0 < PRIMITIVE_TYPE::FLOAT32 && type1 >= PRIMITIVE_TYPE::FLOAT32)) {
    Console::WriteLine("WARNING: Float and Integer mix in binary operation.");
  }

  // sign flags
  bool isType0Signed = (type0 >= PRIMITIVE_TYPE::INT8 && type0 <= PRIMITIVE_TYPE::INT64) || (type0 >= PRIMITIVE_TYPE::FLOAT32 && type0 <= PRIMITIVE_TYPE::FLOAT64);
  bool isType1Signed = (type1 >= PRIMITIVE_TYPE::INT8 && type1 <= PRIMITIVE_TYPE::INT64) || (type1 >= PRIMITIVE_TYPE::FLOAT32 && type1 <= PRIMITIVE_TYPE::FLOAT64);

  if (isType0Signed) {
    if (!isType1Signed) {
      Console::WriteLine(type0Name + " does not match sign of type " + type1Name);
    } else if (type1 <= PRIMITIVE_TYPE::BOOL || type1 > PRIMITIVE_TYPE::FLOAT64) {
      Console::WriteLine(type1Name + " is not a valid type for type " + type0Name);
      return false;
    }
  } else {
    if (!isType0Signed) {
      Console::WriteLine(type0Name + " does not match sign of type " + type1Name);
    } else if (type0 <= PRIMITIVE_TYPE::BOOL || type0 > PRIMITIVE_TYPE::FLOAT64) {
      Console::WriteLine(type0Name + " is not a valid type for type " + type1Name);
      return false;
    }
  }

  resultType = type0 > type1 ? type0 : type1;
  shouldCast = true;
  return true;
}

void SemanticAnalyzer::modAstWithCast(std::shared_ptr<ast::AssignNode> assignNode, std::shared_ptr<ast::StatementNode> nodeToMod, PRIMITIVE_TYPE resultType)
{
  PRIMITIVE_TYPE typeFrom;

  auto nodeType = nodeToMod->GetType();

  switch (nodeType) {
  case ast::AST_TYPE::VariableRefNode: {
    typeFrom = Primitives::Get(CastNode<ast::VariableRefNode>(nodeToMod)->Var->VarType);
    break;
  }

  case ast::AST_TYPE::BinaryOperationNode: {
    typeFrom = Primitives::Get(CastNode<ast::BinaryOperationNode>(nodeToMod)->ResultType);
    break;
  }

  case ast::AST_TYPE::ConstantNode: {
    // It was already casted.
    return;
  }

  case ast::AST_TYPE::FunctionCallNode: {
    typeFrom = Primitives::Get(CastNode<ast::FunctionCallNode>(nodeToMod)->functionFound->ReturnType);
    break;
  }

  default:
    Console::WriteLine("Unsupported cast for node type: " + ast::GetAstTypeName(nodeType));
    return;
  }

  auto castOpNode = std::make_shared<ast::CastOperationNode>();
  castOpNode->Value = nodeToMod;
  castOpNode->TypeTo = resultType;
  castOpNode->TypeFrom = typeFrom;
  assignNode->Right = castOpNode;
}

void SemanticAnalyzer::modAstWithCast(std::shared_ptr<ast::BinaryOperationNode> binOpNode, std::shared_ptr<ast::StatementNode> nodeToMod, PRIMITIVE_TYPE resultType)
{
  PRIMITIVE_TYPE typeFrom;

  auto nodeType = nodeToMod->GetType();

  switch (nodeType) {
  case ast::AST_TYPE::VariableRefNode: {
    auto varRefNode = CastNode<ast::VariableRefNode>(nodeToMod);
    typeFrom = Primitives::Get(varRefNode->Var->VarType);
    break;
  }

  case ast::AST_TYPE::BinaryOperationNode: {
    auto varRefNode = CastNode<ast::BinaryOperationNode>(nodeToMod);
    typeFrom = Primitives::Get(varRefNode->ResultType);
    break;
  }

  default:
    Console::WriteLine("Unsupported cast for node type: " + ast::GetAstTypeName(nodeType));
    return;
  }

  auto castOpNode = std::make_shared<ast::CastOperationNode>();
  castOpNode->Value = nodeToMod;
  castOpNode->TypeTo = resultType;
  castOpNode->TypeFrom = typeFrom;
  (binOpNode->Right == nodeToMod ? binOpNode->Right : binOpNode->Left) = castOpNode;
}

void SemanticAnalyzer::modAstWithCast(std::shared_ptr<ast::UnaryOperationNode> retNode, std::shared_ptr<ast::StatementNode> nodeToMod, PRIMITIVE_TYPE resultType)
{
  PRIMITIVE_TYPE typeFrom;

  auto nodeType = nodeToMod->GetType();

  switch (nodeType) {
  case ast::AST_TYPE::VariableRefNode: {
    typeFrom = Primitives::Get(CastNode<ast::VariableRefNode>(nodeToMod)->Var->VarType);
    break;
  }

  case ast::AST_TYPE::BinaryOperationNode: {
    typeFrom = Primitives::Get(CastNode<ast::BinaryOperationNode>(nodeToMod)->ResultType);
    break;
  }

  case ast::AST_TYPE::ConstantNode: {
    // It was already casted.
    return;
  }

  case ast::AST_TYPE::FunctionCallNode: {
    typeFrom = Primitives::Get(CastNode<ast::FunctionCallNode>(nodeToMod)->functionFound->ReturnType);
    break;
  }

  default:
    Console::WriteLine("Unsupported cast for node type: " + ast::GetAstTypeName(nodeType));
    return;
  }

  auto castOpNode = std::make_shared<ast::CastOperationNode>();
  castOpNode->Value = nodeToMod;
  castOpNode->TypeTo = resultType;
  castOpNode->TypeFrom = typeFrom;
  retNode->Right = castOpNode;
}
