#include "parser.hpp"
#include "tokenize.hpp"
#include "ast_nodes.hpp"
#include "parse_error_msgs.hpp"
#include <cstdarg>
#include <cassert>

static bool is_type_start_token_id(TokenId token_id) noexcept;
static bool is_symbol_start_char(const char _char) noexcept;
static bool is_whitespace_char(const char _char) noexcept;

Parser::Parser(const Lexer& in_lexer, std::vector<Error>& in_error_vec)
    : lexer(in_lexer), error_vec(in_error_vec)
{
    lexer.return_last_token();
}

AstNode* Parser::parse() noexcept {
    return parse_source_code();
}


AstNode* Parser::parse_source_code() noexcept {

    AstNode* source_code = new AstNode(AstNodeType::AstSourceCode, 0L, 0L);
    
    while (lexer.has_tokens()) {
        const auto& token = lexer.get_next_token();
        const auto token_id = token.id;

        switch (token_id) {
        case TokenId::HASH: {
            const char next_char = lexer.source[token.end_pos + 1];
            if (!is_symbol_start_char(next_char))
                return parse_error(token, ERROR_MULTILINE_DIRECTIVE);

            auto directive = parse_basic_directive();
            if (directive) {
                directive->parent = source_code;
                source_code->data.source_code->children.push_back(directive);
            }
        }
            break;
        case TokenId::FN: {
            auto func_def = parse_function_def();
            if (func_def) {
                func_def->parent = source_code;
                source_code->data.source_code->children.push_back(func_def);
            }
        }
            break;
        case TokenId::IDENTIFIER: {
            auto var_def = parse_variable_def();
            if (var_def){
                var_def->parent = source_code;
                source_code->data.source_code->children.push_back(var_def);
            }
        }
            break;
        case TokenId::_EOF:
            break;
        default:
            parse_error(token, ERROR_NO_MAIN_STMNT, token.value);
            break;
        }
    }
    return source_code;
}

/*
* Parses basic directive
* '#' IDENTIFIER (IDENTIFIER | STRING)?
*/
AstNode* Parser::parse_basic_directive() noexcept {
    bool no_error   = true;
    
    Token hash_token = lexer.get_current_token();
    AstNode* node = new AstNode(AstNodeType::AstDirective, hash_token.start_line, hash_token.start_column);

    for (size_t i = 0; no_error && lexer.has_tokens(); ++i) {
        Token token = lexer.get_next_token();
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
                else {
                    delete node;
                    return parse_error(token, ERROR_UNKNOWN_DIRECTIVE);
                }
                break;
            }
            // cant have more than 2 arguments
            if (i == 2) {
                delete node;
                return parse_error(token, ERROR_MORE_THAN_ONE_DIRECTIVE_ARG);
            }

            // IDENTIFIER is an argument
            node->data.directive->argument = token.value;
            break;
        }
        case TokenId::STRING: {
            if (i == 0) {
                delete node;
                return parse_error(token, ERROR_DIRECTIVE_NAME_STRING);
            }
            node->data.directive->argument = token.value;
            break;
        }
        default:
            // end
            lexer.return_last_token();
            return node;
        }

        token = lexer.get_next_token();
    }

    UNREACHEABLE;
}

/*
* Parses fn definition or only fn proto
* 'fn' IDENTIFIER '(' (paramDecl (',' paramDecl)*)? ')' block?
*/
AstNode* Parser::parse_function_def() noexcept {
    Token fn_token = lexer.get_current_token();

    AstNode* fn_node = new AstNode(AstNodeType::AstFuncDef, fn_token.start_line, fn_token.start_column);

    if (lexer.has_tokens()) {
        // return fn token so it can be parsed by the proto
        lexer.return_last_token();
        fn_node->data.function_def->proto = parse_function_proto();

        // error in parse_function_proto
        if (!fn_node->data.function_def->proto)
            return nullptr;
    }
    else {
        // error fn eof
        delete fn_node;
        return parse_error(fn_token, ERROR_UNEXPECTED_EOF, fn_token.value);
    }

    if (lexer.has_tokens()) {
        if (lexer.get_next_token().id == TokenId::L_CURLY) {
            fn_node->data.function_def->block = parse_block();

            // if no error
            if (fn_node->data.function_def->block)
                return fn_node;

            // error in parse_block
            delete fn_node;
            return nullptr;
        }
        // return last token since it was not ours
        lexer.return_last_token();
    }
    
    // fn proto
    return fn_node->data.function_def->proto;
}

/*
* Parses fn proto
* 'fn' IDENTIFIER '(' (paramDecl (',' paramDecl)*)? ')'
*/
AstNode* Parser::parse_function_proto() noexcept {
    Token fn_token = lexer.get_next_token();
    AstNode* fn_proto_node = new AstNode(AstNodeType::AstFuncProto, fn_token.start_line, fn_token.start_column);

    if (!lexer.has_tokens()) {
        delete fn_proto_node;
        return parse_error(fn_token, ERROR_UNEXPECTED_EOF, fn_token.value);
    }

    Token fn_name_token = lexer.get_next_token();
    if (fn_name_token.id != TokenId::IDENTIFIER) {
        delete fn_proto_node;
        return parse_error(fn_name_token, ERROR_EXPECTED_FUNCTION_NAME_INSTEAD, fn_name_token.value);
    }
    fn_proto_node->data.function_proto->name = fn_name_token.value;

    if (!lexer.has_tokens()) {
        delete fn_proto_node;
        return parse_error(fn_name_token, ERROR_UNEXPECTED_EOF, fn_name_token.value);
    }

    //------------------------
    // SIGNATURE
    //------------------------

    Token l_paren_token = lexer.get_next_token();
    if (l_paren_token.id != TokenId::L_PAREN) {
        delete fn_proto_node;
        return parse_error(l_paren_token, ERROR_EXPECTED_FUNCTION_SIGNATURE_INSTEAD, '(', l_paren_token.value);
    }

    if (!lexer.has_tokens()) {
        delete fn_proto_node;
        return parse_error(l_paren_token, ERROR_UNEXPECTED_EOF, l_paren_token.value);
    }

    enum class ParamDeclState {
        param_decl_start,
        param_decl_type,
        param_decl_end,
    } param_decl_state = ParamDeclState::param_decl_start;

    bool no_end = true;
    // identifier or R_PAREN
    Token current_token = lexer.get_next_token();
    AstNode* current_param;
    std::vector<AstNode*> params;

    for (size_t i = 0; current_token.id != TokenId::R_PAREN && no_end; i++) {
        switch (param_decl_state) {
        case ParamDeclState::param_decl_start:
            if (current_token.id == TokenId::IDENTIFIER) {
                param_decl_state = ParamDeclState::param_decl_type;
                current_param = new AstNode(AstNodeType::AstParamDecl, current_token.start_line, current_token.start_column);
                current_param->data.param_decl->name = current_token.value;
            }
            else if (current_token.id == TokenId::R_PAREN) {
                no_end = false;
            }
            else {
                no_end = false;
                lexer.return_last_token();
                parse_error(current_token, ERROR_PARAM_NAME_SIGNATURE_END_EXPECTED, current_token.value);
            } 
            break;
        case ParamDeclState::param_decl_type:
            if (is_type_start_token_id(current_token.id)) {
                auto type = parse_type();
                if (type) {
                    param_decl_state = ParamDeclState::param_decl_end;
                    current_param->data.param_decl->type = type;
                }
                else {
                    // try to recover from error.
                    lexer.return_last_token();
                    param_decl_state = ParamDeclState::param_decl_start;
                    delete current_param;
                }
            }            
            else {
                delete current_param;
                // try to recover from error.
                if (current_token.id == TokenId::IDENTIFIER) {
                    lexer.return_last_token();
                    param_decl_state = ParamDeclState::param_decl_start;
                }
                else
                {
                    lexer.return_last_token();
                    parse_error(current_token, ERROR_PARAM_DECL_TYPE_EXPECTED, current_token.value);
                    no_end = false;
                }
            }
            break;
        case ParamDeclState::param_decl_end:
            switch (current_token.id) {
                case TokenId::COMMA: {
                    param_decl_state = ParamDeclState::param_decl_start;
                    params.push_back(current_param);
                } break;
                default: {
                    delete current_param;
                    // try to recover from error.
                    if (current_token.id == TokenId::IDENTIFIER) {
                        lexer.return_last_token();
                        param_decl_state = ParamDeclState::param_decl_start;
                    }
                    else
                    {
                        lexer.return_last_token();
                        parse_error(current_token, ERROR_SIGNATURE_END_EXPECTED, current_token.value);
                        no_end = false;
                    } 
                } break;
            }
            break;
        default:
            UNREACHEABLE;
        }
        
        if (lexer.has_tokens()) {
            current_token = lexer.get_next_token();
        }
        else {
            return parse_error(current_token, ERROR_UNEXPECTED_EOF, current_token.value);
        }
    }


    //------------------------
    // RETURN TYPE
    //------------------------

    auto type_start_token = lexer.get_next_token();
    if (is_type_start_token_id(type_start_token.id)) {
        fn_proto_node->data.function_proto->return_type = parse_type();
        if (!fn_proto_node->data.function_proto->return_type) {
            // error in parse_type
            delete fn_proto_node;
            return nullptr;
        }
    }
    else {
        delete fn_proto_node;
        return parse_error(fn_name_token, ERROR_EXPECTED_FUNCTION_RET_TYPE, fn_name_token.value);
    }

    return fn_proto_node;
}


/*
* Parses a block
* '{' statements '}'
*/
AstNode* Parser::parse_block() noexcept
{
    return nullptr;
}

/*
* Parses a variable definition
* IDENTIFIER type
*/
AstNode* Parser::parse_variable_def() noexcept {
    return nullptr;
}

/*
* Parses a type
* IDENTIFIER | '[' ']' type | '*' type
*/
AstNode* Parser::parse_type() noexcept
{
    Token token = lexer.get_current_token();
    AstNode* type_node = new AstNode(AstNodeType::AstType, token.start_line, token.start_column);

    switch (token.id) {
    case TokenId::L_BRACKET:
        type_node->data.ast_type->type = AstTypeType::Array;
        token = lexer.get_next_token();
        if (token.id != TokenId::R_BRACKET)
            break;
        type_node->data.ast_type->data_type = parse_type();
        if (type_node->data.ast_type->data_type != nullptr)
            return type_node;
        break;
    case TokenId::MUL:
        type_node->data.ast_type->type = AstTypeType::Pointer;
        type_node->data.ast_type->data_type = parse_type();
        if (type_node->data.ast_type->data_type != nullptr)
            return type_node;
        break;
    case TokenId::IDENTIFIER:
        type_node->data.ast_type->type = AstTypeType::DataType;
        type_node->data.ast_type->name = token.value;
        return type_node;
    default:
        break;
    }

    delete type_node;
    return parse_error(token, ERROR_INVALID_TYPE, token.value);
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

bool is_type_start_token_id(TokenId token_id) noexcept {
    switch (token_id) {
    case TokenId::L_BRACKET:    // array type
    case TokenId::MUL:          // pointer type
    case TokenId::IDENTIFIER:   // type's name
        return true;
    default:
        return false;
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
