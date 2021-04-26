#include "AstBuilder.hpp"
#include "Node.hpp"
#include "ProgramNode.hpp"
#include "AssignNode.hpp"
#include "VariableDefNode.hpp"
#include "VariableRefNode.hpp"
#include "FunctionDefNode.hpp"
#include "FunctionCallNode.hpp"
#include "UnaryOperationNode.hpp"
#include "BinaryOperationNode.hpp"
#include "ConstantNode.hpp"
#include <sstream>
#include <memory>
#include "../Console.hpp"

using namespace llang;
using namespace ast;

antlrcpp::Any AstBuilder::visitSourceFile(LlamaLangParser::SourceFileContext *context)
{
  context->AstNode = ASTree;
  ASTree->GlobalScope = globalScope;

  // Visit global scope first
  for( auto child : context->children ) {
    auto result = visit(child);

    if( result.isNotNull() ) {
      auto resultVal = result.as<Node::ChildType>();
      ASTree->children.push_back(result);
    }
  }

  // Visit innerScopes
  visitChildren(context);

  return ASTree;
}

antlrcpp::Any AstBuilder::visitFunctionDef(LlamaLangParser::FunctionDefContext *context)
{
  auto funcNode = CastNode<FunctionDefNode>(context->AstNode);

  // second visit to retrieve more info
  if (funcNode) {
    // before visit children set the function scope
    currentScope = funcNode->InnerScope;

    auto blockAny = visit(context->block());

    if (blockAny.is<FunctionDefNode::BlockType *>()) {
      funcNode->Block = *blockAny.as<FunctionDefNode::BlockType *>();
    }

    // We are now exiting the function
    // return the scope to the parent scope
    currentScope = currentScope->Parent;
  }
  // First visit from parent
  else {
    funcNode = std::make_shared<FunctionDefNode>();
    funcNode->FileName = FileName;
    funcNode->Line = context->start->getLine();
    funcNode->Name = context->IDENTIFIER()->getText();
    funcNode->ReturnType = context->type_()->getText();
    // Add function scope to parent scope
    funcNode->InnerScope = currentScope->addChild(symbol_table::SCOPE_TYPE::FUNC, funcNode->Name, funcNode).back();

    context->AstNode = funcNode;

    // before visit children set the function scope
    currentScope = funcNode->InnerScope;
    visit(context->signature());
    // We are now exiting the function
    // return the scope to the parent scope
    currentScope = currentScope->Parent;
  }

  return CastNode<Node>(funcNode);
}

antlrcpp::Any AstBuilder::visitSignature(LlamaLangParser::SignatureContext *context)
{
  auto parentContext = (LlamaLangParseContext *) context->parent;
  // parent context ASTnode is a FunctionDefNode
  context->AstNode = parentContext->AstNode;

  // set parameters
  visitChildren(context);

  return nullptr;
}

antlrcpp::Any AstBuilder::visitParameters(LlamaLangParser::ParametersContext *context)
{
  auto parentContext = (LlamaLangParseContext *) context->parent;
  auto funcNode = CastNode<FunctionDefNode>(parentContext->AstNode);

  auto parameters = context->parameterDecl();

  for( auto paramContext : parameters ) {
    if( paramContext->isEmpty() || paramContext->exception != nullptr )
      continue;

    // variable definition
    auto param = std::make_shared<VariableDefNode>();
    param->FileName = FileName;
    param->Line = paramContext->start->getLine();
    param->Name = paramContext->IDENTIFIER()->getText();
    param->VarType = paramContext->type_()->getText();
    param->isGlobal = currentScope == globalScope;
    // Add symbol
    currentScope->addSymbol(param->Name, param);

    funcNode->Parameters.push_back(param);
  }

  // It is not necesary to visit children after this visit
  return nullptr;
}

antlrcpp::Any AstBuilder::visitBlock(LlamaLangParser::BlockContext *context)
{
  // returns the std::vector from visitStatementList
  return visitChildren(context);
}

antlrcpp::Any AstBuilder::visitStatementList(LlamaLangParser::StatementListContext *context)
{
  auto statementContexts = context->statement();

  auto stmntList = new FunctionDefNode::BlockType();

  for( auto statementContext : statementContexts ) {
    if( statementContext->isEmpty() || statementContext->exception != nullptr )
      continue;

    auto stmntAny = visitChildren(statementContext);

    if( stmntAny.is<FunctionDefNode::StatementType>() ) {
      auto stmnt = stmntAny.as<FunctionDefNode::StatementType>();
      stmntList->push_back(stmnt);
    }
  }

  return stmntList;
}

antlrcpp::Any AstBuilder::visitReturnStmt(LlamaLangParser::ReturnStmtContext *context)
{
  auto retStmnt = std::make_shared<UnaryOperationNode>(UNARY_STATEMENT_TYPE::RETURN);
  retStmnt->FileName = FileName;
  retStmnt->Line = context->start->getLine();

  context->AstNode = retStmnt;

  auto rightAny = visitChildren(context);

  if( rightAny.isNotNull() ) {
    auto rightStmnt = rightAny.as<std::shared_ptr<StatementNode>>();
    retStmnt->Right = rightStmnt;
  }

  return CastNode<StatementNode>(retStmnt);
}

antlrcpp::Any AstBuilder::visitExpression(LlamaLangParser::ExpressionContext *context)
{
  if( context->right && context->left ) {
    auto rightNodeAny = visit(context->right);
    auto leftNodeAny = visit(context->left);

    if( rightNodeAny.isNull() || leftNodeAny.isNull() )
      return nullptr;

    auto rightNode = rightNodeAny.as<std::shared_ptr<StatementNode>>();
    auto leftNode = leftNodeAny.as<std::shared_ptr<StatementNode>>();
    std::shared_ptr<BinaryOperationNode> binOpNode;

    if( context->PLUS() ) {
      // ADDITION EXPRESSION
      binOpNode = std::make_shared<BinaryOperationNode>(BINARY_OPERATION::ADD);
    } else if( context->MINUS() ) {
      // SUBSTRACTION EXPRESSION
      binOpNode = std::make_shared<BinaryOperationNode>(BINARY_OPERATION::SUB);
    } else if( context->STAR() ) {
      // MULTIPLICATION EXPRESSION
      binOpNode = std::make_shared<BinaryOperationNode>(BINARY_OPERATION::MUL);
    } else if( context->DIV() ) {
      // DIVITION EXPRESSION
      binOpNode = std::make_shared<BinaryOperationNode>(BINARY_OPERATION::DIV);
    } else if( context->MOD() ) {
      // MODULOUS EXPRESSION
      binOpNode = std::make_shared<BinaryOperationNode>(BINARY_OPERATION::MOD);
    }

    if( binOpNode != nullptr ) {
      binOpNode->Right = rightNode;
      binOpNode->Left = leftNode;
    }

    return CastNode<StatementNode>(binOpNode);
  } else {
    // Unary || Primary expression
    // returns ConstantNode, UnaryOperationNode
    return visitChildren(context);
  }
}

antlrcpp::Any AstBuilder::visitPrimaryExpr(LlamaLangParser::PrimaryExprContext *context)
{
  auto funcCallNode = std::make_shared<FunctionCallNode>();

  // Function call
  if (context->primaryExpr() != nullptr && context->arguments() != nullptr) {
    // wrongly assigned to a variable reference as antlr4::any
    auto varRefAny = visitChildren(context->primaryExpr());

    if (varRefAny.isNull())
      return nullptr;

    // get the name and delete the ptr
    auto varRefNode = varRefAny.as<std::shared_ptr<VariableRefNode>>();
    funcCallNode->Name = varRefNode->Var->Name;

    // Set funcCallNode as the astNode
    // to then get the Arguments
    context->AstNode = funcCallNode;
    visit(context->arguments());

    return CastNode<StatementNode>(funcCallNode);
  }

  // else keep going down the tree
  return visitChildren(context);
}

antlrcpp::Any AstBuilder::visitVarDef(LlamaLangParser::VarDefContext *context)
{
  if (context->AstNode)
    return context->AstNode;

  // variable definition
  auto varDefNode = std::make_shared<VariableDefNode>();
  varDefNode->FileName = FileName;
  varDefNode->Line = context->start->getLine();
  varDefNode->Name = context->IDENTIFIER()->getText();
  varDefNode->VarType = context->type_()->getText();
  varDefNode->isGlobal = currentScope == globalScope;
  context->AstNode = varDefNode;
  // Add symbol
  currentScope->addSymbol(varDefNode->Name, varDefNode);

  if( context->ASSIGN() ) {
    auto assignmentStmnt = std::make_shared<AssignNode>();
    assignmentStmnt->Left = std::make_shared<VariableRefNode>();
    assignmentStmnt->Left->WasFound = true;
    assignmentStmnt->Left->Var = varDefNode;
    assignmentStmnt->Right = visit(context->expressionList()).as<std::shared_ptr<StatementNode>>();
    varDefNode->assignmentStmnt = assignmentStmnt;
  }



  return CastNode<StatementNode>(varDefNode);
}

antlrcpp::Any AstBuilder::visitAssignment(LlamaLangParser::AssignmentContext *context)
{
  auto assignmentStmnt = std::make_shared<AssignNode>();

  auto varName = context->IDENTIFIER()->getText();
  auto varRefNode = std::make_shared<VariableRefNode>();
  varRefNode->SetAsNotFound(varName);

  assignmentStmnt->Left = varRefNode;
  assignmentStmnt->Right = visit(context->expressionList());

  return CastNode<StatementNode>(assignmentStmnt);
}

antlrcpp::Any AstBuilder::visitOperandName(LlamaLangParser::OperandNameContext *context)
{
  auto varName = context->IDENTIFIER()->getText();

  auto varRefNode = std::make_shared<VariableRefNode>();
  varRefNode->SetAsNotFound(varName);

  return varRefNode;
}

/*
* Can be:
* -(IDENTIFIER|NUMBER|STRING)
* +(IDENTIFIER|NUMBER|STRING)
* (IDENTIFIER|NUMBER|STRING)++
* (IDENTIFIER|NUMBER|STRING)--
*/
antlrcpp::Any AstBuilder::visitUnaryExpr(LlamaLangParser::UnaryExprContext *context)
{
  auto childAny = visitChildren(context);

  if( childAny.is<std::shared_ptr<ConstantNode>>() ) {
    auto constChild = childAny.as<std::shared_ptr<ConstantNode>>();

    if( context->unaryOp()->MINUS() ) {
      constChild->Value = "-" + constChild->Value;
    }

    return constChild;
  }

  std::shared_ptr<UnaryOperationNode> unaryStmnt;

  /*
  if( context->unaryOp()->MINUSMINUS() )
      unaryStmnt = std::make_shared<UnaryOperationNode>(UNARY_STATEMENT_TYPE::DECREMENT);
  else if( context->unaryOp()->PLUSPLUS() )
      unaryStmnt = std::make_shared<UnaryOperationNode>(UNARY_STATEMENT_TYPE::INCREMENT);
  */

  unaryStmnt->Right = childAny.as<std::shared_ptr<StatementNode>>();
  return unaryStmnt;
}

antlrcpp::Any AstBuilder::visitArguments(LlamaLangParser::ArgumentsContext *context)
{
  // set parent astNode as the ctx ast node
  auto parent = (LlamaLangParseContext *) context->parent;
  context->AstNode = parent->AstNode;

  visitChildren(context);
  return nullptr;
}

antlrcpp::Any AstBuilder::visitExpressionList(LlamaLangParser::ExpressionListContext *context)
{
  auto parentContext = (LlamaLangParseContext *)context->parent;

  if (parentContext->AstNode && parentContext->AstNode->GetType() == AST_TYPE::FunctionCallNode) {
    auto funcCallNode = CastNode<FunctionCallNode>(parentContext->AstNode);

    auto args = context->expression();

    for (auto argCtx : args) {
      if (argCtx->isEmpty() || argCtx->exception != nullptr)
        continue;

      auto argAny = visit(argCtx);

      if (argAny.isNotNull()) {
        auto argNode = argAny.as<std::shared_ptr<StatementNode>>();
        funcCallNode->Arguments.push_back(argNode);
      }
    }
  }

  return visitChildren(context);
}

antlrcpp::Any AstBuilder::visitBasicLit(LlamaLangParser::BasicLitContext *context)
{
  std::shared_ptr<ConstantNode> constantNode = nullptr;

  if( context->RUNE_LIT() != nullptr ) {
    constantNode = std::make_shared<ConstantNode>(CONSTANT_TYPE::CHAR);
    constantNode->Value = context->RUNE_LIT()->getText();
  } else if( context->string_() != nullptr) {
    constantNode = std::make_shared<ConstantNode>(CONSTANT_TYPE::STRING);
    constantNode->Value = context->string_()->getText();
  }
  else {
      return visitChildren(context);
  }

  if( constantNode != nullptr ) {
    constantNode->FileName = FileName;
    constantNode->Line = context->start->getLine();
    return CastNode<StatementNode>(constantNode);
  }

  return nullptr;
}

antlrcpp::Any llang::ast::AstBuilder::visitInteger(LlamaLangParser::IntegerContext* context)
{
    static const size_t str_size = 21;
    static char str[str_size];
    std::shared_ptr<ConstantNode> constantNode = nullptr;

    auto text_value = context->getText();

    bool isByte = text_value.back() == 'b';
    bool isWord = text_value.back() == 'w';
    bool isLong = text_value.back() == 'l';

    if (isByte || isWord || isLong)
        text_value.pop_back();

    bool isUnsigned = text_value.back() == 'u';
    if (isUnsigned)
        text_value.pop_back();

    bool isBin = text_value[1] == 'b' || text_value[1] == 'B';
    bool isOct = text_value.front() == '0';
    bool isHex = text_value[1] == 'x' || text_value[1] == 'X';

    if (isHex || isBin)
        text_value = text_value.substr(2);
    else if (isOct)
        text_value = text_value.substr(1);

    CONSTANT_TYPE intType;

    if (isUnsigned) {
        uint64_t number = 0;
        try {
            if (isBin)
                number = std::stoull(text_value, 0, 2);
            else if (isHex)
                number = std::stoull(text_value, 0, 16);
            else if (isOct)
                number = std::stoull(text_value, 0, 8);
            else
                number = std::stoull(text_value, 0, 10);
        }
        catch (std::out_of_range&) {
            // overflow
            Console::WriteLine("Constant can not be stored in any known data type (overflow uint64): " + text_value);
            number = strtoull(text_value.c_str(), nullptr, 10);
        }

        if (number <= std::numeric_limits<uint8_t>::max()) {
            intType = CONSTANT_TYPE::U8;
        } else if (isByte) {
            Console::WriteLine("Byte overflow: " + text_value);
            intType = CONSTANT_TYPE::U8;
            number = (uint8_t)number;
        } else if (number <= std::numeric_limits<uint16_t>::max()) {
            intType = CONSTANT_TYPE::U16;
        } else if (isWord) {
            Console::WriteLine("Word overflow: " + text_value);
            number = (uint16_t)number;
            intType = CONSTANT_TYPE::U16;
        } else if (number <= std::numeric_limits<uint32_t>::max()) {
            intType = CONSTANT_TYPE::U32;
        } else if (isLong) {
            Console::WriteLine("Long overflow: " + text_value);
            intType = CONSTANT_TYPE::U32;
            number = (uint32_t)number;
        } else {
            intType = CONSTANT_TYPE::U64;
        }

        snprintf(str, str_size, "%llu", number);
        text_value = str;
    } else {
        int64_t number = 0;
        try {
            if (isBin) 
                number = std::stoll(text_value, 0, 2);
            else if (isHex)
                number = std::stoll(text_value, 0, 16);
            else  if (isOct)
                number = std::stoll(text_value, 0, 8);
            else
                number = std::stoll(text_value, 0, 10);
        }
        catch (std::out_of_range&) {
            // overflow
            Console::WriteLine("Constant can not be stored in any known data type (overflow int64): " + text_value);
            number = strtoll(text_value.c_str(), nullptr, 10);
        }

        
        if (number <= std::numeric_limits<int8_t>::max() && number >= std::numeric_limits<int8_t>::min()) {
           intType = CONSTANT_TYPE::I8;
        } else if (isByte) {
            Console::WriteLine("Byte overflow: " + text_value);
            intType = CONSTANT_TYPE::I8;
            number = (uint8_t)number; // here unsigned cast is necessary to zero extend the number value.
        } else if (number <= std::numeric_limits<int16_t>::max() && number >= std::numeric_limits<int16_t>::min()) {
            intType = CONSTANT_TYPE::I16;
        } else if (isWord) {
            Console::WriteLine("Word overflow: " + text_value);
            intType = CONSTANT_TYPE::I16;
            number = (uint16_t)number;
        } else if (number <= std::numeric_limits<int32_t>::max() && number >= std::numeric_limits<int32_t>::min()) {
            intType = CONSTANT_TYPE::I32;
        } else if (isLong) {
            Console::WriteLine("Long overflow: " + text_value);
            intType = CONSTANT_TYPE::I32;
            number = (uint32_t)number;
        } 
        else {
            intType = CONSTANT_TYPE::I64;
        }

        snprintf(str, str_size, "%lld", number);
        text_value = str;
    }
    
    constantNode = std::make_shared<ConstantNode>(intType);
    constantNode->Value = text_value;
    return constantNode;
}

antlrcpp::Any llang::ast::AstBuilder::visitFloatingPoint(LlamaLangParser::FloatingPointContext* context)
{
    auto text = context->getText();
    CONSTANT_TYPE floatingPointType;

    if (text.back() == 'f') {
        text.pop_back();
        try {
            std::stof(text);
            floatingPointType = CONSTANT_TYPE::FLOAT;
        } catch (std::out_of_range&) {
            //overflows float
            Console::WriteLine("Float overflow: " + text);
            return nullptr;
        }
    }
    else {
        try {
            double number = std::stod(text);
            if (number <= std::numeric_limits<float>::max() && number >= std::numeric_limits<float>::min())
                floatingPointType = CONSTANT_TYPE::FLOAT;
            else 
                floatingPointType = CONSTANT_TYPE::DOUBLE;
        }
        catch (std::out_of_range&) {
            Console::WriteLine("Constant can not be stored in any known data type (overflow float64): " + text);
            return nullptr;
        }
    }

    auto constantNode = std::make_shared<ConstantNode>(floatingPointType);
    constantNode->Value = text;
    return constantNode;
}



/*****************************
* TREE BEHAVIOUR OVERRIDES
*****************************/

antlrcpp::Any AstBuilder::visitChildren(antlr4::tree::ParseTree *node)
{
  antlrcpp::Any result = defaultResult();

  for( size_t i = 0; i < node->children.size(); i++ ) {
    auto child = node->children[i];

    if( !shouldVisitNextChild(child, result) ) {
      continue;
    }

    antlrcpp::Any childResult = child->accept(this);
    result = this->aggregateResult(result, childResult);
  }

  return result;
}

bool AstBuilder::shouldVisitNextChild(antlr4::tree::ParseTree *node, const antlrcpp::Any &currentResult)
{
  auto terminalNode = dynamic_cast<antlr4::tree::TerminalNode *>( node );

  if( terminalNode )
    return false;

  auto context = dynamic_cast<antlr4::ParserRuleContext *>( node );

  if( context )
    return context->children.size() > 0 || context->isEmpty() || context->exception != nullptr;

  return false;
}

antlrcpp::Any &AstBuilder::aggregateResult(antlrcpp::Any &result, antlrcpp::Any &nextResult)
{
  if( result.isNotNull() && nextResult.isNull() )
    return result;

  return nextResult;
}
