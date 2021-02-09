#include "AstBuilder.hpp"
#include "Console.hpp"
#include "ast/VariableDeclNode.hpp"
#include "ast/FunctionNode.hpp"

using namespace llang;

template <typename T>
std::shared_ptr<T> constexpr CastNode(ast::Node::ChildType node) {
    return std::static_pointer_cast<T, ast::Node>(node);
}

antlrcpp::Any
AstBuilder::visitSourceFile(LlamaLangParser::SourceFileContext *context) {
    Console::WriteLine();
    Console::WriteLine("========== SOURCE FILE NODE ===========");

    context->AstNode = ASTree;

    // add program source to the program tree
    visitChildren(context);

    return ASTree;
}

antlrcpp::Any
AstBuilder::visitFunctionDecl(LlamaLangParser::FunctionDeclContext *context) {
  if (context->isEmpty() || context->exception != nullptr)
    return nullptr;

  auto parentContext = (LlamaLangParseContext*) context->parent;
  context->AstNode = std::make_shared<ast::FunctionNode>();

  std::shared_ptr<ast::FunctionNode> funcNode = CastNode<ast::FunctionNode>(context->AstNode);

  funcNode->FileName = FileName;
  funcNode->Line = context->start->getLine();
  funcNode->Name = context->IDENTIFIER()->getText();
  funcNode->ReturnType = context->type_()->getText();

  parentContext->AstNode->children.push_back(funcNode);
  
  visitChildren(context);
  
  return nullptr;
}

antlrcpp::Any
AstBuilder::visitSignature(LlamaLangParser::SignatureContext *context) {
  if (context->isEmpty() || context->exception != nullptr)
    return nullptr;

  auto parentContext = (LlamaLangParseContext*) context->parent;
  context->AstNode = parentContext->AstNode;
  return visitChildren(context);
}

antlrcpp::Any
AstBuilder::VisitParameters(LlamaLangParser::ParametersContext *context) {
  if (context->isEmpty() || context->exception != nullptr)
    return nullptr;

  auto parentContext = (LlamaLangParseContext*) context->parent;
  auto funcNode = CastNode<ast::FunctionNode>(parentContext->AstNode);

  auto parameters = context->parameterDecl();

  for (auto paramContext : parameters) {
    if (paramContext->isEmpty() || paramContext->exception != nullptr)
      continue;

    auto param = std::make_shared<ast::VariableDeclNode>();
    param->FileName = FileName;
    param->Line = context->start->getLine();
    param->Name = paramContext->IDENTIFIER()->getText();
    param->VarType = paramContext->type_()->getText();
    funcNode->Parameters.push_back(param);
  }

  return visitChildren(context);
}

antlrcpp::Any AstBuilder::visitBlock(LlamaLangParser::BlockContext *context) {
  if (context->isEmpty() || context->exception != nullptr)
    return nullptr;
  
  auto parentContext = (LlamaLangParseContext*) context->parent;
  context->AstNode = parentContext->AstNode;
  return visitChildren(context);
}

antlrcpp::Any llang::AstBuilder::visitStatementList(
    LlamaLangParser::StatementListContext *context) {
  if (context->isEmpty() || context->exception != nullptr)
    return nullptr;

  auto parentContext = (LlamaLangParseContext*)context->parent;
  auto funcNode = CastNode<ast::FunctionNode>(parentContext->AstNode);
  auto statementContexts = context->statement();

  for (auto statementContext : statementContexts) {
    if (statementContext->isEmpty() || statementContext->exception != nullptr)
      continue;

    statementContext->AstNode = funcNode;
    visitChildren(statementContext);
  }

  return funcNode;
}

antlrcpp::Any
AstBuilder::visitReturnStmt(LlamaLangParser::ReturnStmtContext *context) {
    if (context->isEmpty() || context->exception != nullptr)
      return nullptr;

    var parentContext = (LlamaLangParseContext)context.Parent;
    var funcNode = (ASTFunctionNode)parentContext.AstNode;

    string msg = context.GetType().Name +
                 "\t| Parent :: " + parentContext.GetType().Name +
                 "\t| AstNode :: " + parentContext.AstNode
        ?.GetType().Name;
    Console.WriteLine(msg);

    var statement = new ASTUnaryStatementNode(STATEMENT_TYPE.RETURN);
    statement.File = fileName;
    statement.Line = context.Start.Line;

    funcNode.Block.Add(statement);
    context.AstNode = statement;

    base.VisitChildren(context);

    return null;
}

antlrcpp::Any
AstBuilder::visitUnaryExpr(LlamaLangParser::UnaryExprContext *context) {
  if (context->isEmpty() || context->exception != nullptr)
    return nullptr;

  var parentContext = (LlamaLangParseContext)context.Parent;
  var returnStnt = (ASTUnaryStatementNode)parentContext.AstNode;
  context.AstNode = returnStnt;

  string msg =
      context.GetType().Name + "\t| Parent :: " + parentContext.GetType().Name +
      "\t| AstNode :: " + parentContext.AstNode
      ?.GetType().Name;
  Console.WriteLine(msg);

  var childNode = base.VisitChildren(context);
  var childNodeType = childNode.GetType();
  if (childNodeType == typeof(ASTUnaryStatementNode)) {
    var node = (ASTUnaryStatementNode)childNode;
    var nodeType = node.Right.GetType();
    if (nodeType == typeof(ASTConstantNode)) {
      var constNode = (ASTConstantNode)node.Right;
      constNode.Value = context.unaryOp().GetText() + constNode.Value;
    }
  }

  return childNode;
}

antlrcpp::Any
AstBuilder::visitExpression(LlamaLangParser::ExpressionContext *context) {
  if (context->isEmpty() || context->exception != nullptr)
    return nullptr;

  var parentContext = (LlamaLangParseContext)context.Parent;
  var returnStnt = (ASTUnaryStatementNode)parentContext.AstNode;
  context.AstNode = returnStnt;

  string msg =
      context.GetType().Name + "\t| Parent :: " + parentContext.GetType().Name +
      "\t| AstNode :: " + parentContext.AstNode
      ?.GetType().Name;
  Console.WriteLine(msg);

  var exprNode = base.VisitChildren(context);

  // Unary expression
  var exprType = exprNode.GetType().BaseType;
  if (exprType == typeof(ASTRightValueNode)) {
    var rightValue = (ASTRightValueNode)exprNode;
    returnStnt.Right = rightValue;
  }

  return returnStnt;
}

antlrcpp::Any
AstBuilder::visitBasicLit(LlamaLangParser::BasicLitContext *context) {
  if (context->isEmpty() || context->exception != nullptr)
    return nullptr;

  ASTConstantNode constantNode;

  if (context.integer() != null) {
    constantNode = new ASTConstantNode(CONSTANT_TYPE.INTEGER);
    constantNode.Value = context.integer().GetText();
  } else if (context.FLOAT_LIT() != null) {
    constantNode = new ASTConstantNode(CONSTANT_TYPE.FLOAT);
    constantNode.Value = context.FLOAT_LIT().GetText();
  } else if (context.RUNE_LIT() != null) {
    constantNode = new ASTConstantNode(CONSTANT_TYPE.CHAR);
    constantNode.Value = context.RUNE_LIT().GetText();
  } else {
    constantNode = new ASTConstantNode(CONSTANT_TYPE.STRING);
    constantNode.Value = context.string_().GetText();
  }

  if (constantNode != null) {
    constantNode.File = fileName;
    constantNode.Line = context.Start.Line;
  }

  return constantNode;
}
