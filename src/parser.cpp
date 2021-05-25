#include "parser.hpp"
#include "tokenize.hpp"
#include "ast_nodes.hpp"
#include "parse_error_msgs.hpp"
#include <cstdarg>
#include <cassert>

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
* Parses math expressions
* mulexpr ([+-] mulexpr)*
*/
AstNode* Parser::parse_expr() noexcept
{
    const Token& token = lexer.get_current_token();
    
    while (MATCH(token, TokenId::PLUS, TokenId::MINUS)) {

    }

    return nullptr;
}

/*
* Parses math expressions
* value ([/*%] value)*
*/
AstNode* Parser::parse_mulexpr() noexcept
{

    const Token& token = lexer.get_current_token();

    while (MATCH(token, TokenId::MUL, TokenId::DIV, TokenId::MOD)) {

    }

    return nullptr;
}

/*
* Parses math expressions
* IDENTIFIER | FLOAT_LIT | INT_LIT | UNICODE_CHAR
*/
AstNode* Parser::parse_value() noexcept
{
    const Token& token = lexer.get_current_token();
    auto token_value = lexer.get_token_value(token);

    if (MATCH(token, TokenId::IDENTIFIER, TokenId::FLOAT_LIT, TokenId::INT_LIT, TokenId::UNICODE_CHAR)) {
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
