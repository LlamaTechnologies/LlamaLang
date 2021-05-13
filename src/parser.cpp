#include "parser.hpp"
#include "tokenize.hpp"
#include "ast_nodes.hpp"
#include "parse_error_msgs.hpp"
#include <cstdarg>
#include <cassert>

static bool is_symbol_start_char(const char _char) noexcept;
static bool is_whitespace_char(const char _char) noexcept;

Parser::Parser(const Lexer& in_lexer, std::vector<Error>& in_error_vec)
    : lexer(in_lexer), error_vec(in_error_vec)
{}

AstNode* Parser::parse() noexcept {
    return parse_source_code();
}


AstNode* Parser::parse_source_code() noexcept {
    AstNode* source_code = new AstNode(AstNodeType::AstSourceCode, 0L, 0L);
    
next_sibling:
    if (!lexer.has_tokens())
        return source_code;
    const auto& token = lexer.get_next_token();
    const auto token_id = token.id;
    
    const char next_char = lexer.source[token.end_pos + 1];

    switch (token_id) {
    case TokenId::HASH:
        if (!is_symbol_start_char(next_char))
            return parse_error(token, ERROR_MULTILINE_DIRECTIVE);

        auto directive =  parse_basic_directive();
        source_code->data.source_code->children.push_back(directive);
        goto next_sibling;
    case TokenId::FN:
        auto func_def = parse_function_def();
        source_code->data.source_code->children.push_back(func_def);
        goto next_sibling;
    case TokenId::IDENTIFIER:
        auto var_def = parse_variable_def();
        source_code->data.source_code->children.push_back(var_def);
        goto next_sibling;
    case TokenId::_EOF:
        goto next_sibling;
    default:
        parse_error(token, ERROR_NO_MAIN_STMNT, token.value);
        goto next_sibling;
    }

    // ERROR
    return nullptr;
}

/*
* Parses basic directive
* '#' IDENTIFIER (IDENTIFIER | STRING)?
*/
AstNode* Parser::parse_basic_directive() noexcept {
    bool no_error   = true;
    bool no_end     = true;
    
    Token token = lexer.get_next_token();
    AstNode* node = new AstNode(AstNodeType::AstDirective, token.start_line, token.start_column);

    for (size_t i = 0; no_error && no_end; ++i) {
        const auto token_id = token.id;

        switch (token_id) {
        case TokenId::IDENTIFIER: {
            const char next_char = lexer.source[token.end_pos + 1];

            // name cant be in next line after #
            if (i == 0) {
                if (next_char != '\"' && !is_symbol_start_char(next_char) && next_char != ' ')
                    return parse_error(token, ERROR_MULTILINE_DIRECTIVE);
                // set directive type

                if (token.value == get_directive_type_name(DirectiveType::CompTimeOnly))
                    node->data.directive->directive_type = DirectiveType::CompTimeOnly;
                else if (token.value == get_directive_type_name(DirectiveType::Run))
                    node->data.directive->directive_type = DirectiveType::Run;
                else if (token.value == get_directive_type_name(DirectiveType::Load))
                    node->data.directive->directive_type = DirectiveType::Load;
                else
                    return parse_error(token, ERROR_UNKNOWN_DIRECTIVE);
                break;
            }
            // cant have more than 2 arguments
            if (i == 2)
                return parse_error(token, ERROR_MORE_THAN_ONE_DIRECTIVE_ARG);

            // IDENTIFIER is an argument
            node->data.directive->argument = token.value;
            break;
        }
        case TokenId::STRING: {
            if (i == 0)
                return parse_error(token, ERROR_DIRECTIVE_NAME_STRING);
            node->data.directive->argument = token.value;
            break;
        }
        default:
            // end
            lexer.return_last_token();
            no_end = false;
            return node;
        }

        token = lexer.get_next_token();
    }

    UNREACHEABLE;
}

AstNode* Parser::parse_function_def() noexcept {
    return nullptr;
}

AstNode* Parser::parse_variable_def() noexcept {
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
