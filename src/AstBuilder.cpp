#include "AstBuilder.hpp"
#include "ast/Node.hpp"
#include "ast/ProgramNode.hpp"
#include "ast/VariableDeclNode.hpp"
#include "ast/FunctionDefNode.hpp"
#include "ast/UnaryOperationNode.hpp"
#include "ast/ConstantNode.hpp"
#include <sstream>

using namespace llang;

antlrcpp::Any AstBuilder::visitBlockChildren(LlamaLangParser::BlockContext *node) {
    auto result = ast::FunctionDefNode::BlockType();
    auto stmntList = node->statementList();
    auto children = stmntList->children;

    for( size_t i = 0; i < children.size(); i++ ) {
        auto child = children[i];
        if( child == stmntList->eos(0) )
            break;

        antlrcpp::Any childResultAny = child->accept(this);
        auto childResult = childResultAny.as<ast::FunctionDefNode::StatementType>();
        result.push_back(childResult);
    }

    return result;
}

antlrcpp::Any AstBuilder::visitSourceFile(LlamaLangParser::SourceFileContext *context) {
    context->AstNode = ASTree;

    // add program source to the program tree
    visitChildren(context);

    return ASTree;
}

antlrcpp::Any AstBuilder::visitFunctionDecl(LlamaLangParser::FunctionDeclContext *context) {
    auto parentContext = (LlamaLangParseContext *) context->parent;
    context->AstNode = std::make_shared<ast::FunctionDefNode>();

    auto funcNode = CastNode<ast::FunctionDefNode>(context->AstNode);

    funcNode->FileName = FileName;
    funcNode->Line = context->start->getLine();
    funcNode->Name = context->IDENTIFIER()->getText();
    funcNode->ReturnType = context->type_()->getText();

    parentContext->AstNode->children.push_back(funcNode);

    /* Get the result of the last child visited (block) */
    auto blockAny = visitChildren(context);

    if( blockAny.is<ast::FunctionDefNode::BlockType*>() ) {
        funcNode->Block = *blockAny.as<ast::FunctionDefNode::BlockType*>();
    }

    return nullptr;
}

antlrcpp::Any AstBuilder::visitSignature(LlamaLangParser::SignatureContext *context) {
    auto parentContext = (LlamaLangParseContext *) context->parent;
    // parent context ASTnode is a FunctionDefNode
    context->AstNode = parentContext->AstNode;

    // set parameters
    visitChildren(context);

    return nullptr;
}

antlrcpp::Any AstBuilder::VisitParameters(LlamaLangParser::ParametersContext *context) {
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

    // It is not necesary to visit children after this visit
    return nullptr;
}

antlrcpp::Any AstBuilder::visitBlock(LlamaLangParser::BlockContext *context) {
    // returns the std::vector from visitStatementList
    return visitChildren(context);
    //return visitBlockChildren(context);
}

antlrcpp::Any llang::AstBuilder::visitStatementList(LlamaLangParser::StatementListContext *context) {
    auto statementContexts = context->statement();

    auto stmntList = new ast::FunctionDefNode::BlockType();

    for( auto statementContext : statementContexts ) {
        if( statementContext->isEmpty() || statementContext->exception != nullptr )
            continue;

        auto stmntAny = visitChildren(statementContext);
        if( stmntAny.is<ast::FunctionDefNode::StatementType>() ) {
            auto stmnt = stmntAny.as<ast::FunctionDefNode::StatementType>();
            stmntList->push_back(stmnt);
        }
    }

    return stmntList;
}

antlrcpp::Any AstBuilder::visitReturnStmt(LlamaLangParser::ReturnStmtContext *context) {
    auto retStmnt = std::make_shared<ast::UnaryOperationNode>(ast::UNARY_STATEMENT_TYPE::RETURN);
    retStmnt->FileName = FileName;
    retStmnt->Line = context->start->getLine();

    context->AstNode = retStmnt;

    auto rightAny = visitChildren(context);

    if( rightAny.isNotNull() ) {
        auto rightStmnt = rightAny.as<std::shared_ptr<ast::StatementNode>>();
        retStmnt->Right = rightStmnt;
    }

    return CastNode<ast::StatementNode>(retStmnt);
}

antlrcpp::Any AstBuilder::visitExpression(LlamaLangParser::ExpressionContext *context) {
    // Unary || Primary expression
    // returns ConstantNode, UnaryOperationNode
    return visitChildren(context);
}

/*
* Can be:
* -(IDENTIFIER|NUMBER|STRING)
* +(IDENTIFIER|NUMBER|STRING)
* (IDENTIFIER|NUMBER|STRING)++
* (IDENTIFIER|NUMBER|STRING)--
*/
antlrcpp::Any AstBuilder::visitUnaryExpr(LlamaLangParser::UnaryExprContext *context) {
    auto childAny = visitChildren(context);

    if( childAny.is<std::shared_ptr<ast::ConstantNode>>() ) {
        auto constChild = childAny.as<std::shared_ptr<ast::ConstantNode>>();
        if( context->unaryOp()->MINUS() ) {
            constChild->Value = "-" + constChild->Value;
        }
        return constChild;
    }

    std::shared_ptr<ast::UnaryOperationNode> unaryStmnt;

    /*
    if( context->unaryOp()->MINUSMINUS() )
        unaryStmnt = std::make_shared<ast::UnaryOperationNode>(ast::UNARY_STATEMENT_TYPE::DECREMENT);
    else if( context->unaryOp()->PLUSPLUS() )
        unaryStmnt = std::make_shared<ast::UnaryOperationNode>(ast::UNARY_STATEMENT_TYPE::INCREMENT);
    */

    unaryStmnt->Right = childAny.as<std::shared_ptr<ast::StatementNode>>();
    return unaryStmnt;
}

antlrcpp::Any AstBuilder::visitBasicLit(LlamaLangParser::BasicLitContext *context) {
    std::shared_ptr<ast::ConstantNode> constantNode = nullptr;

    if( context->integer() != nullptr ) {
        auto textStream = std::istringstream(context->integer()->getText());
        int8_t isInt8;
        int16_t isInt16;
        int32_t isInt32;
        int64_t isInt64;
        ast::CONSTANT_TYPE intType;
        if( textStream >> isInt8  && !textStream.fail() ) {
            intType = ast::CONSTANT_TYPE::I8;
        } else if( textStream >> isInt16 ) {
            intType = ast::CONSTANT_TYPE::I16;
        } else if( textStream >> isInt32 ) {
            intType = ast::CONSTANT_TYPE::I32;
        } else if( textStream >> isInt64 ) {
            intType = ast::CONSTANT_TYPE::I64;
        } else {
            return nullptr;
        }
        constantNode = std::make_shared<ast::ConstantNode>(intType);
        constantNode->Value = context->integer()->getText();
    } else if( context->floatingPoint() != nullptr ) {
        auto text = context->floatingPoint()->getText();
        ast::CONSTANT_TYPE floatingPointType;
        if( tolower(text.back()) == 'f' ) {
            text.pop_back();
            floatingPointType = ast::CONSTANT_TYPE::FLOAT;
        } else 
            floatingPointType = ast::CONSTANT_TYPE::DOUBLE;

        constantNode = std::make_shared<ast::ConstantNode>(floatingPointType);
        constantNode->Value = text;
    } else if( context->RUNE_LIT() != nullptr ) {
        constantNode = std::make_shared<ast::ConstantNode>(ast::CONSTANT_TYPE::CHAR);
        constantNode->Value = context->RUNE_LIT()->getText();
    } else if ( context->string_() ){
        constantNode = std::make_shared<ast::ConstantNode>(ast::CONSTANT_TYPE::STRING);
        constantNode->Value = context->string_()->getText();
    }

    if( constantNode != nullptr ) {
        constantNode->FileName = FileName;
        constantNode->Line = context->start->getLine();
        return CastNode<ast::StatementNode>(constantNode);
    }

    return nullptr;
}



/*****************************
* TREE BEHAVIOUR OVERRIDES
*****************************/

antlrcpp::Any AstBuilder::visitChildren(antlr4::tree::ParseTree *node) {
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

bool llang::AstBuilder::shouldVisitNextChild(antlr4::tree::ParseTree *node, const antlrcpp::Any &currentResult) {
    auto terminalNode = dynamic_cast<antlr4::tree::TerminalNode *>( node );

    if( terminalNode )
        return false;

    auto context = dynamic_cast<antlr4::ParserRuleContext *>( node );
    if( context )
        return context->children.size() > 0 || context->isEmpty() || context->exception != nullptr;

    return false;
}

antlrcpp::Any& llang::AstBuilder::aggregateResult(antlrcpp::Any &result, antlrcpp::Any &nextResult) {
    if( result.isNotNull() && nextResult.isNull() )
        return result;
    return nextResult;
}
