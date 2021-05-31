#include "parser.hpp"
#include "tokenize.hpp"
#include "ast_nodes.hpp"
#include "parse_error_msgs.hpp"
#include <cstdarg>
#include <cassert>

static BinaryExprType get_binary_op(const Token& token) noexcept;
static bool is_symbol_start_char(const char _char) noexcept;
static bool is_whitespace_char(const char _char) noexcept;
static bool match(const Token token, const TokenId ...) noexcept;

#define MATCH(token, ...) match(token, __VA_ARGS__, TokenId(size_t(TokenId::_EOF) + 1))
#define MAIN_OBJECTS \
TokenId::HASH:\
case TokenId::FN:\
case TokenId::IDENTIFIER



Parser::Parser(const Lexer& in_lexer, std::vector<Error>& in_error_vec)
    : lexer(in_lexer), error_vec(in_error_vec)
{
}

AstNode* Parser::parse() noexcept {
    return nullptr;
}


/*
* Parses any supported expresion
*/
AstNode* Parser::parse_expr() noexcept
{
    return parse_comp_expr();
}

/*
* Parses comparative expresions
* compExpr
*   : unaryExpr ('=='|'!=') unaryExpr
*   | unaryExpr ('>='|'<='|'<'|'>') unaryExpr}
*   | unaryExpr
*   ;
*/
AstNode* Parser::parse_comp_expr() noexcept
{
    auto root_node = parse_unary_expr();

    for (const Token token = lexer.get_next_token(); MATCH(token, TokenId::EQUALS, TokenId::NOT_EQUALS); ) {
        if (token.id == TokenId::_EOF) {
            parse_error(token, ERROR_UNEXPECTED_EOF, lexer.get_token_value(lexer.get_previous_token()));
        }

        AstNode* unary_expr = parse_unary_expr();
        for (const Token inner_token = lexer.get_next_token(); MATCH(token, TokenId::GREATER, TokenId::GREATER_OR_EQUALS, TokenId::LESS, TokenId::LESS_OR_EQUALS); ) {
            AstNode* inner_unary_expr = parse_unary_expr();
            if (!inner_unary_expr) {
                //TODO(pablo96): error in unary_expr => sync parsing
            }

            // create binary node
            auto binary_expr = new AstNode(AstNodeType::AstBinaryExpr, inner_token.start_line, inner_token.start_column);
            binary_expr->data.binary_expr->op1 = unary_expr;
            binary_expr->data.binary_expr->bin_op = get_binary_op(inner_token);
            binary_expr->data.binary_expr->op2 = inner_unary_expr;

            // set the new node as root.
            unary_expr = binary_expr;
        }

        if (!unary_expr) {
            //TODO(pablo96): error in unary_expr => sync parsing
        }

        // create binary node
        auto binary_expr = new AstNode(AstNodeType::AstBinaryExpr, token.start_line, token.start_column);
        binary_expr->data.binary_expr->op1 = root_node;
        binary_expr->data.binary_expr->bin_op = get_binary_op(token);
        binary_expr->data.binary_expr->op2 = unary_expr;

        // set the new node as root.
        root_node = binary_expr;
    }
    lexer.get_back();

    return root_node;
}

/*
* Parses unary expresions
* unaryExpr
*   : ('!' | '~' | '--' | '++') algebraicExpr
*   | algebraicExpr
*   ;
*/
AstNode* Parser::parse_unary_expr() noexcept
{
    const Token token = lexer.get_current_token();
    if (MATCH(token, TokenId::NOT, TokenId::BIT_NOT, TokenId::PLUS_PLUS, TokenId::MINUS_MINUS)) {
        AstNode* node = new AstNode(AstNodeType::AstUnaryExpr, token.start_line, token.start_column);
        AstNode* algebraic_expr = parse_algebraic_expr();
        if (!algebraic_expr) {
            //TODO(pablo96): error in algebraic_expr => sync parsing
        }
        node->data.unary_expr->primary_expr = algebraic_expr;
    }

    return parse_algebraic_expr();
}

/*
* Parses addition like expressions
* algebraicExpr
*   : termExpr ('+' | '-' | '|') termExpr
*   | termExpr
*/
AstNode* Parser::parse_algebraic_expr() noexcept {
    auto root_node = parse_term_expr();
    if (!root_node) {
        const auto& token = lexer.get_current_token();
        if (token.id == TokenId::_EOF) {
            parse_error(token, ERROR_UNEXPECTED_EOF, lexer.get_token_value(lexer.get_previous_token()));
        }
        //TODO(pablo96): error in primary expr => sync parsing
    }

    for (Token token = lexer.get_next_token(); MATCH(token, TokenId::PLUS, TokenId::MINUS, TokenId::BIT_OR); token = lexer.get_next_token()) {
        if (token.id == TokenId::_EOF) {
            parse_error(token, ERROR_UNEXPECTED_EOF, lexer.get_token_value(lexer.get_previous_token()));
        }
        lexer.advance();

        auto term_expr = parse_term_expr();
        if (!term_expr) {
            //TODO(pablo96): error in term_expr => sync parsing
        }

        // create binary node
        auto binary_expr = new AstNode(AstNodeType::AstBinaryExpr, token.start_line, token.start_column);
        binary_expr->data.binary_expr->op1 = root_node;
        binary_expr->data.binary_expr->bin_op = get_binary_op(token);
        binary_expr->data.binary_expr->op2 = term_expr;

        // set the new node as root.
        root_node = binary_expr;
    }
    lexer.get_back();

    return root_node;
}

/*
* Parses multiplication like expressions
* termExpr
*   : primaryExpr ('*' | '/' | '%' | '<<' | '>>' | '&' | '^') primaryExpr
*   | primaryExpr
*/
AstNode* Parser::parse_term_expr() noexcept {
    auto root_node = parse_primary_expr();
    
    if (!root_node) {
        const auto& token = lexer.get_current_token();
        if (token.id == TokenId::_EOF) {
            parse_error(token, ERROR_UNEXPECTED_EOF, lexer.get_token_value(lexer.get_previous_token()));
        }
        //TODO(pablo96): error in primary expr => sync parsing
    }

    for (Token token = lexer.get_next_token();
        MATCH(token, TokenId::MUL, TokenId::DIV, TokenId::MOD, TokenId::LSHIFT, TokenId::RSHIFT, TokenId::BIT_AND, TokenId::BIT_XOR);
        token = lexer.get_next_token()) {

        if (token.id == TokenId::_EOF) {
            parse_error(token, ERROR_UNEXPECTED_EOF, lexer.get_token_value(lexer.get_previous_token()));
        }
        lexer.advance();

        auto symbol_token = parse_primary_expr();
        if (!symbol_token) {
            //TODO(pablo96): error in primary expr => sync parsing
        }

        // create binary node
        auto binary_expr = new AstNode(AstNodeType::AstBinaryExpr, token.start_line, token.start_column);
        binary_expr->data.binary_expr->op1 = root_node;
        binary_expr->data.binary_expr->bin_op = get_binary_op(token);
        binary_expr->data.binary_expr->op2 = symbol_token;
        
        // set the new node as root.
        root_node = binary_expr;
    }
    lexer.get_back();

    return root_node;
}

/*
* Parses primary expressions
* CALL_EXPR | IDENTIFIER | FLOAT_LIT | INT_LIT | UNICODE_CHAR
*/
AstNode* Parser::parse_primary_expr() noexcept {
    const Token& token = lexer.get_current_token();
    auto token_value = lexer.get_token_value(token);

    if (token.id == TokenId::_EOF) {
        parse_error(token, ERROR_UNEXPECTED_EOF, lexer.get_token_value(lexer.get_previous_token()));
        return nullptr;
    }

    if (token.id == TokenId::IDENTIFIER) {
        // CALL EXPR?
        // else
        goto parse_literal;
    }

    if (MATCH(token, TokenId::FLOAT_LIT, TokenId::INT_LIT, TokenId::UNICODE_CHAR)) {
parse_literal:
        AstNode* symbol_node = new AstNode(AstNodeType::AstSymbol, token.start_line, token.start_column);
        symbol_node->data.symbol->token = &token;
        return symbol_node;
    }

    parse_error(token, ERROR_EXPECTED_VALUE_TOKEN, token_value, token_id_name(token.id));
    return nullptr;
}

AstNode* Parser::parse_error(const Token& token, const char* format, ...) noexcept {
    va_list ap, ap2;
    va_start(ap, format);
    va_copy(ap2, ap);

    int len1 = snprintf(nullptr, 0, format, ap);
    assert(len1 >= 0);

    std::string msg;
    msg.reserve(len1);

    int len2 = snprintf(msg.data(), len1, format, ap2);
    assert(len2 == len1);

    va_end(ap);

    Error error(ERROR_TYPE::ERROR,
        token.start_line,
        token.start_column,
        lexer.file_name, msg);
    return nullptr;
}

BinaryExprType get_binary_op(const Token& token) noexcept {
    switch (token.id)
    {
    case TokenId::PLUS:
        return BinaryExprType::ADD;
    case TokenId::MINUS:
        return BinaryExprType::SUB;
    case TokenId::MUL:
        return BinaryExprType::MUL;
    case TokenId::DIV:
        return BinaryExprType::DIV;
    case TokenId::MOD:
        return BinaryExprType::MOD;
    case TokenId::LSHIFT:
        return BinaryExprType::LSHIFT;
    case TokenId::RSHIFT:
        return BinaryExprType::RSHIFT;
    case TokenId::BIT_XOR:
        return BinaryExprType::BIT_XOR;
    case TokenId::BIT_AND:
        return BinaryExprType::BIT_AND;
    case TokenId::ASSIGN:
        return BinaryExprType::ASSIGN;
    case TokenId::EQUALS:
        return BinaryExprType::EQUALS;
    case TokenId::NOT_EQUALS:
        return BinaryExprType::NOT_EQUALS;
    case TokenId::GREATER_OR_EQUALS:
        return BinaryExprType::GREATER_OR_EQUALS;
    case TokenId::GREATER:
        return BinaryExprType::GREATER;
    case TokenId::LESS:
        return BinaryExprType::LESS;
    case TokenId::LESS_OR_EQUALS:
        return BinaryExprType::LESS_OR_EQUALS;
    default:
        UNREACHEABLE;
    }
}

bool is_symbol_start_char(const char next_char) noexcept {
    return (next_char >= 'a' && next_char <= 'z') ||
        (next_char >= 'A' && next_char <= 'Z') ||
        next_char == '_';
}

bool is_whitespace_char(const char _char) noexcept {
    switch (_char) {
    case '\t':
    case '\r':
    case '\n':
    case ' ':
        return true;
    default:
        return false;
    }
}

bool match(const Token token, const TokenId ...) noexcept
{
    va_list vargv;
    va_start(vargv, token);

    for (;;)
    {
        const TokenId id = va_arg(vargv, TokenId);

        // run out of token_ids
        if (size_t(id) > size_t(TokenId::_EOF)) {
            va_end(vargv);
            return false;
        }

        // found match
        if (token.id == id) {
            va_end(vargv);
            return true;
        }
    }
}
