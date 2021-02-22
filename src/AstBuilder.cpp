#include "AstBuilder.hpp"
#include "ast/Node.hpp"
#include "ast/ProgramNode.hpp"
#include "ast/VariableDeclNode.hpp"
#include "ast/FunctionDefNode.hpp"
#include "ast/UnaryOperationNode.hpp"
#include "ast/ConstantNode.hpp"

using namespace llang;

antlrcpp::Any AstBuilder::visitSourceFile(LlamaLangParser::SourceFileContext *context) {
    context->AstNode = ASTree;

    // add program source to the program tree
    visitChildren(context);

    return ASTree;
}

antlrcpp::Any AstBuilder::visitFunctionDecl(LlamaLangParser::FunctionDeclContext *context) {
    if( context->isEmpty() || context->exception != nullptr )
        return nullptr;

    auto parentContext = (LlamaLangParseContext *) context->parent;
    context->AstNode = std::make_shared<ast::FunctionDefNode>();

    auto funcNode = CastNode<ast::FunctionDefNode>(context->AstNode);

    funcNode->FileName = FileName;
    funcNode->Line = context->start->getLine();
    funcNode->Name = context->IDENTIFIER()->getText();
    funcNode->ReturnType = context->type_()->getText();

    parentContext->AstNode->children.push_back(funcNode);

    visitChildren(context);

    return nullptr;
}

antlrcpp::Any AstBuilder::visitSignature(LlamaLangParser::SignatureContext *context) {
    if( context->isEmpty() || context->exception != nullptr )
        return nullptr;

    auto parentContext = (LlamaLangParseContext *) context->parent;
    context->AstNode = parentContext->AstNode;
    return visitChildren(context);
}

antlrcpp::Any AstBuilder::VisitParameters(LlamaLangParser::ParametersContext *context) {
    if( context->isEmpty() || context->exception != nullptr )
        return nullptr;

    auto parentContext = (LlamaLangParseContext *) context->parent;
    auto funcNode = CastNode<ast::FunctionDefNode>(parentContext->AstNode);

    auto parameters = context->parameterDecl();

    for( auto paramContext : parameters ) {
        if( paramContext->isEmpty() || paramContext->exception != nullptr )
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
    if( context->isEmpty() || context->exception != nullptr )
        return nullptr;

    auto parentContext = (LlamaLangParseContext *) context->parent;
    context->AstNode = parentContext->AstNode;
    return visitChildren(context);
}

antlrcpp::Any llang::AstBuilder::visitStatementList(
    LlamaLangParser::StatementListContext *context) {
    if( context->isEmpty() || context->exception != nullptr )
        return nullptr;

    auto parentContext = (LlamaLangParseContext *) context->parent;
    auto funcNode = CastNode<ast::FunctionDefNode>(parentContext->AstNode);
    auto statementContexts = context->statement();

    for( auto statementContext : statementContexts ) {
        if( statementContext->isEmpty() || statementContext->exception != nullptr )
            continue;

        statementContext->AstNode = funcNode;
        visitChildren(statementContext);
    }

    return funcNode;
}

antlrcpp::Any AstBuilder::visitReturnStmt(LlamaLangParser::ReturnStmtContext *context) {
    if( context->isEmpty() || context->exception != nullptr )
        return nullptr;

    auto parentContext = (LlamaLangParseContext *) context->parent;
    auto funcNode = CastNode<ast::FunctionDefNode>(parentContext->AstNode);

    auto statement = std::make_shared<ast::UnaryOperationNode>(ast::UNARY_STATEMENT_TYPE::RETURN);
    statement->FileName = FileName;
    statement->Line = context->start->getLine();

    funcNode->Block.push_back(statement);
    context->AstNode = statement;

    visitChildren(context);

    return nullptr;
}

/*
* Can be:
* -(NUMBER/STRING)
* +(NUMBER/STRING)
* (NUMBER/STRING)++
* (NUMBER/STRING)--
* IDENTIFIER/NUMBER/STRING
*/
antlrcpp::Any AstBuilder::visitUnaryExpr(LlamaLangParser::UnaryExprContext *context) {
    if( context->isEmpty() || context->exception != nullptr )
        return nullptr;

    auto parentContext = (LlamaLangParseContext *) context->parent;
    auto returnStnt = CastNode<ast::UnaryOperationNode>(parentContext->AstNode);
    context->AstNode = returnStnt;

    auto childNode = visitChildren(context);
    if( childNode.is<std::shared_ptr<ast::ConstantNode>>() ) {
        auto constNode = childNode.as<std::shared_ptr<ast::ConstantNode>>();
        constNode->Value = context->unaryOp()->getText() + constNode->Value;
    }

    return childNode;
}

antlrcpp::Any AstBuilder::visitExpression(LlamaLangParser::ExpressionContext *context) {
    if( context->isEmpty() || context->exception != nullptr )
        return nullptr;

    auto parentContext = (LlamaLangParseContext *) context->parent;
    auto returnStnt = CastNode<ast::UnaryOperationNode>(parentContext->AstNode);
    context->AstNode = returnStnt;

    auto exprNode = visitChildren(context);

    // Unary expression
    if( exprNode.is<ast::UnaryOperationNode>() ) {
        auto rightValue = CastNode<ast::UnaryOperationNode>(exprNode);
        returnStnt->Right = rightValue;
    }

    return returnStnt;
}

antlrcpp::Any AstBuilder::visitBasicLit(LlamaLangParser::BasicLitContext *context) {
    if( context->isEmpty() || context->exception != nullptr )
        return nullptr;

    std::shared_ptr<ast::ConstantNode> constantNode = nullptr;


    if( context->integer() != nullptr ) {
        ( ast::CONSTANT_TYPE::INTEGER );
        constantNode->Value = context->integer()->getText();
    } else if( context->FLOAT_LIT() != nullptr ) {
        constantNode = std::make_shared<ast::ConstantNode>(ast::CONSTANT_TYPE::FLOAT);
        constantNode->Value = context->FLOAT_LIT()->getText();
    } else if( context->RUNE_LIT() != nullptr ) {
        constantNode =
            std::make_shared<ast::ConstantNode>(ast::CONSTANT_TYPE::CHAR);
        constantNode->Value = context->RUNE_LIT()->getText();
    } else {
        std::make_shared<ast::ConstantNode>(ast::CONSTANT_TYPE::CHAR);
        ( ast::CONSTANT_TYPE::STRING );
        constantNode->Value = context->string_()->getText();
    }

    if( constantNode != nullptr ) {
        constantNode->FileName = FileName;
        constantNode->Line = context->start->getLine();
    }

    return constantNode;
}
