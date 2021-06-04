#include "parser.hpp"
#include "lexer.hpp"
#include "ast_nodes.hpp"
#include "parse_error_msgs.hpp"
#include <cstdarg>
#include <cassert>

static BinaryExprType get_binary_op(const Token& token) noexcept;
static UnaryExprType get_unary_op(const Token& token) noexcept;
static bool is_type_start_token(const Token& token) noexcept;
static bool is_expr_token(const Token& token) noexcept;
static bool is_symbol_start_char(const char _char) noexcept;
static bool is_whitespace_char(const char _char) noexcept;

#define MAIN_OBJECTS \
TokenId::HASH:\
case TokenId::FN:\
case TokenId::IDENTIFIER



Parser::Parser(const Lexer& in_lexer, std::vector<Error>& in_error_vec)
    : lexer(in_lexer), error_vec(in_error_vec) {}

AstNode* Parser::parse() noexcept {
    return nullptr;
}

/*
* Parses a (function | if-else stmnt) block
* block
*   : '{' (statement eos)* '}'
*   ;
*/
AstNode* Parser::parse_block() noexcept {
    Token token = lexer.get_next_token();
    if (token.id != TokenId::L_CURLY) {
        // bad_prediction
        UNREACHEABLE;
    }

    AstNode* block_node = new AstNode(AstNodeType::AstBlock, token.start_line, token.start_column);

    token = lexer.get_next_token();
    if (token.id == TokenId::R_CURLY) {
        // Empty block
        return block_node;
    }
    
    do {
        if (token.id == TokenId::_EOF) {
            const Token& prev_token = lexer.get_previous_token();
            parse_error(prev_token, ERROR_UNEXPECTED_EOF_AFTER, lexer.get_token_value(prev_token));
            delete block_node;
            return nullptr;
        }

        lexer.get_back();
        AstNode* stmnt = parse_statement();
        if (!stmnt) {
            // TODO(pablo96): handle error in statement parsing
            delete block_node;
            return nullptr;
        }

        const Token& semicolon_token = lexer.get_next_token();
        if (token.id != TokenId::SEMI) {
            bool has_new_line = is_new_line_between(token.end_pos, semicolon_token.start_pos);
            // checking for r_curly allows for '{stmnt}' as block
            if (token.id != TokenId::R_CURLY && !has_new_line) {
                // statement wrong ending
                parse_error(token, ERROR_EXPECTED_NEWLINE_OR_SEMICOLON_AFTER, lexer.get_token_value(token));
                delete block_node;
                return nullptr;
            }

            // was not a semicolon.
            lexer.get_back();
        }

        block_node->block.statements.push_back(stmnt);

        token = lexer.get_next_token();
    } while (token.id != TokenId::R_CURLY);

    return block_node;
}

/*
* This predicts what statement to parse
* statement
*   : varDef
*   | expression
*   | assignmentStmnt
*   | returnStmt
*   | block
*   | emptyStmt
*   ;
*/
AstNode* Parser::parse_statement() noexcept {
    const Token& token = lexer.get_next_token();
    switch (token.id) {
    case TokenId::IDENTIFIER: {
        const Token& second_token = lexer.get_next_token();
        if (second_token.id == TokenId::ASSIGN) {
            if (lexer.get_next_token().id != TokenId::ASSIGN) {
                lexer.get_back();
                lexer.get_back(); // second_token
                lexer.get_back(); // token
                return parse_assign_stmnt();
            }
            // is '==' expression
            lexer.get_back();
            goto stmnt_expr;
        }
        else if (is_type_start_token(second_token)) {
            lexer.get_back(); // second_token
            lexer.get_back(); // token
            return parse_vardef_stmnt();
        }
    stmnt_expr:
        lexer.get_back(); // second_token
        lexer.get_back(); // token
        return parse_expr();
    }
    case TokenId::RET:
        lexer.get_back();
        return parse_ret_stmnt();
    case TokenId::L_CURLY:
        // TODO(pablo96): block
        return nullptr;
    case TokenId::SEMI:
        // empty_statement
        // consume the token and predict again
        return parse_statement();
    case TokenId::_EOF:
        return nullptr;
    default:
        UNREACHEABLE;
    }
}

/*
* parses a variable definition/initialization
* varDef
*   : IDENTIFIER type_name ('=' expression)?
*   ;
*/
AstNode* Parser::parse_vardef_stmnt() noexcept {
    const Token& token_symbol_name = lexer.get_next_token();

    if (token_symbol_name.id == TokenId::IDENTIFIER) {
        AstNode* type_node = parse_type();
        
        if (!type_node) {
            // TODO(pablo96): Handle error
            // wrong expression expected type name
            return nullptr;
        }

        AstNode* var_def_node = new AstNode(AstNodeType::AstVarDef, token_symbol_name.start_line, token_symbol_name.start_column);
        var_def_node->var_def.name = lexer.get_token_value(token_symbol_name);
        var_def_node->var_def.type = type_node;

        if (lexer.get_next_token().id == TokenId::ASSIGN) {
            // TODO(pablo96): parse assignment
        }
        else {
            var_def_node->var_def.initializer = nullptr;
        }

        return var_def_node;
    }

    // Bad prediction
    UNREACHEABLE;
}

/*
* type_name
*   : '*' type_name
*   | '[' ']' type_name
*   | IDENTIFIER
*   ;
*/
AstNode* Parser::parse_type() noexcept {
    const Token& token = lexer.get_next_token();
    if (token.id == TokenId::MUL) {
        // POINTER TYPE
        AstNode* type_node = new AstNode(AstNodeType::AstType, token.start_line, token.start_column);
        type_node->ast_type.type = AstTypeType::Pointer;
        const Token& next_token = lexer.get_next_token();
        if (!is_type_start_token(next_token)) {
            parse_error(next_token, ERROR_EXPECTED_TYPE_EXPR_INSTEAD_OF, lexer.get_token_value(next_token));
            lexer.get_back();
            delete type_node;
            return nullptr;
        }
        lexer.get_back();
        type_node->ast_type.data_type = parse_type();

        return type_node;
    }
    else if (token.id == TokenId::L_BRACKET) {
        // ARRAY TYPE
        const Token& r_braket_token = lexer.get_next_token();
        if (r_braket_token.id != TokenId::R_BRACKET) {
            parse_error(r_braket_token, ERROR_EXPECTED_CLOSING_BRAKET_BEFORE, lexer.get_token_value(r_braket_token));
            lexer.get_back();
            return nullptr;
        }
        AstNode* type_node = new AstNode(AstNodeType::AstType, token.start_line, token.start_column);
        type_node->ast_type.type = AstTypeType::Array;

        const Token& next_token = lexer.get_next_token();
        if (!is_type_start_token(next_token)) {
            parse_error(next_token, ERROR_EXPECTED_TYPE_EXPR_INSTEAD_OF, lexer.get_token_value(next_token));
            lexer.get_back();
            delete type_node;
            return nullptr;
        }
        lexer.get_back();
        type_node->ast_type.data_type = parse_type();

        return type_node;
    }
    else if (token.id == TokenId::IDENTIFIER) {
        AstNode* type_node = new AstNode(AstNodeType::AstType, token.start_line, token.start_column);
        type_node->ast_type.type = AstTypeType::DataType;
        type_node->ast_type.name = lexer.get_token_value(token);
        return type_node;
    }
    else if (token.id == TokenId::_EOF) {
        const Token& prev_token = lexer.get_previous_token();
        parse_error(prev_token, ERROR_UNEXPECTED_EOF_AFTER, lexer.get_token_value(prev_token));
        return nullptr;
    }
    // Bad prediction
    UNREACHEABLE;
}

/*
* Parses an assignment
* 
* assignmentStmnt
*   : IDENTIFIER assign_op expression
*   ;
*/
AstNode* Parser::parse_assign_stmnt() noexcept {
    auto identifier_node = parse_primary_expr();
    if (!identifier_node) {
        //TODO(pablo96): error in unary_expr => sync parsing
        return nullptr;
    }

    const Token& token = lexer.get_next_token();
    if (token.id == TokenId::ASSIGN) {
        auto expr = parse_expr();
        if (!expr) {
            //TODO(pablo96): error in unary_expr => sync parsing
            return nullptr;
        }
        AstNode* node = new AstNode(AstNodeType::AstBinaryExpr, token.start_line, token.start_column);
        node->binary_expr.bin_op = get_binary_op(token);
        node->binary_expr.op1 = identifier_node;
        node->binary_expr.op2 = expr;
        return node;
    }

    // Getting here means the prediction failed.
    UNREACHEABLE;
}

/*
* Parses a return statement
* returnStmt
*   | 'ret' expression?
*   ;
*/
AstNode* Parser::parse_ret_stmnt() noexcept {
    const Token& token = lexer.get_next_token();
    if (token.id == TokenId::RET) {
        AstNode* node = new AstNode(AstNodeType::AstUnaryExpr, token.start_line, token.start_column);
        node->unary_expr.op = get_unary_op(token);

        if (is_expr_token(lexer.get_next_token())) {
            lexer.get_back();

            auto expr = parse_expr();
            if (!expr) {
                //TODO(pablo96): error in unary_expr => sync parsing
                return nullptr;
            }

            node->unary_expr.expr = expr;
        }
        else {
            node->unary_expr.expr = nullptr;
            lexer.get_back();
        }

        return node;
    }

    // error
    return nullptr;
}

/*
* Parses any supported expresion
* expression
*   : '(' expression ')'
*   | compExpr
*   ;
*/
AstNode* Parser::parse_expr() noexcept
{
    if (lexer.get_next_token().id == TokenId::L_PAREN) {
        auto expression = parse_expr();
        if (lexer.get_next_token().id != TokenId::R_PAREN) {
            const Token& prev_token = lexer.get_previous_token();
            parse_error(prev_token, ERROR_EXPECTED_R_PAREN_AFTER, lexer.get_token_value(prev_token));
            return nullptr;
        }
        return expression;
    }
    // Was not a ')'
    lexer.get_back();
    return parse_comp_expr();
}

/*
* Parses comparative expresions
* compExpr
*   : algebraicExpr ('=='|'!=' | '>='| '<=' | '<'| '>') algebraicExpr
*   | algebraicExpr
*   ;
*/
AstNode* Parser::parse_comp_expr() noexcept {
    auto root_node = parse_algebraic_expr();
    if (!root_node) {
        //TODO(pablo96): error in primary expr => sync parsing
        return nullptr;
    }
    
    do {
        Token token = lexer.get_next_token();

        if (!MATCH(token, TokenId::EQUALS, TokenId::NOT_EQUALS, TokenId::GREATER, TokenId::GREATER_OR_EQUALS, TokenId::LESS, TokenId::LESS_OR_EQUALS)) {
            // Not my token
            lexer.get_back();
            break;
        }

        AstNode* unary_expr = parse_algebraic_expr();
        if (!unary_expr) {
            //TODO(pablo96): error in unary_expr => sync parsing
            break;
        }

        // create binary node
        auto binary_expr = new AstNode(AstNodeType::AstBinaryExpr, token.start_line, token.start_column);
        binary_expr->binary_expr.op1 = root_node;
        binary_expr->binary_expr.bin_op = get_binary_op(token);
        binary_expr->binary_expr.op2 = unary_expr;

        // set the new node as root.
        root_node = binary_expr;
    } while (true);

    return root_node;
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
        //TODO(pablo96): error in primary expr => sync parsing
        return nullptr;
    }
    
    do {
        Token token = lexer.get_next_token();
        if (!MATCH(token, TokenId::PLUS, TokenId::MINUS, TokenId::BIT_OR)) {
            // Not my token
            lexer.get_back();
            break;
        }

        auto term_expr = parse_term_expr();
        if (!term_expr) {
            //TODO(pablo96): error in term_expr => sync parsing
            break;
        }

        // create binary node
        auto binary_expr = new AstNode(AstNodeType::AstBinaryExpr, token.start_line, token.start_column);
        binary_expr->binary_expr.op1 = root_node;
        binary_expr->binary_expr.bin_op = get_binary_op(token);
        binary_expr->binary_expr.op2 = term_expr;

        // set the new node as root.
        root_node = binary_expr;
    } while (true);

    return root_node;
}

/*
* Parses multiplication like expressions
* termExpr
*   : primaryExpr ('*' | '/' | '%' | '<<' | '>>' | '&' | '^') primaryExpr
*   | primaryExpr
*/
AstNode* Parser::parse_term_expr() noexcept {
    auto root_node = parse_unary_expr();
    
    if (!root_node) {
        //TODO(pablo96): error in primary expr => sync parsing
        return nullptr;
    }

    do {
        Token token = lexer.get_next_token();
        if (!MATCH(token, TokenId::MUL, TokenId::DIV, TokenId::MOD, TokenId::LSHIFT, TokenId::RSHIFT, TokenId::BIT_AND, TokenId::BIT_XOR)) {
            // Not my token
            lexer.get_back();
            break;
        }

        auto symbol_token = parse_unary_expr();
        if (!symbol_token) {
            //TODO(pablo96): error in primary expr => sync parsing
            break;
        }

        // create binary node
        auto binary_expr = new AstNode(AstNodeType::AstBinaryExpr, token.start_line, token.start_column);
        binary_expr->binary_expr.op1 = root_node;
        binary_expr->binary_expr.bin_op = get_binary_op(token);
        binary_expr->binary_expr.op2 = symbol_token;

        // set the new node as root.
        root_node = binary_expr;
    } while (true);

    return root_node;
}

/*
* Parses unary expresions
* unaryExpr
*   : ('!' | '~' | '--' | '++') primaryExpr
*   | primaryExpr ('!' | '~' | '--' | '++')
*   | primaryExpr
*   ;
*/
AstNode* Parser::parse_unary_expr() noexcept {
    Token token = lexer.get_next_token();
    
    if (token.id == TokenId::_EOF) {
        const Token& prev_token = lexer.get_previous_token();
        parse_error(prev_token, ERROR_UNEXPECTED_EOF_AFTER, lexer.get_token_value(prev_token));
        return nullptr;
    }

    // op primary_expr
    if (MATCH(token, TokenId::NOT, TokenId::BIT_NOT, TokenId::PLUS_PLUS, TokenId::MINUS_MINUS)) {
        AstNode* node = new AstNode(AstNodeType::AstUnaryExpr, token.start_line, token.start_column);
        AstNode* primary_expr = parse_primary_expr();
        if (!primary_expr) {
            //TODO(pablo96): error in algebraic_expr => sync parsing
        }
        node->unary_expr.op = get_unary_op(token);
        node->unary_expr.expr = primary_expr;
        return node;
    }

    lexer.get_back();
    AstNode* primary_expr = parse_primary_expr();
    if (!primary_expr) {
        //TODO(pablo96): error in algebraic_expr => sync parsing
    }

    token = lexer.get_next_token();
    // primary_expr op
    if (MATCH(token, TokenId::NOT, TokenId::BIT_NOT, TokenId::PLUS_PLUS, TokenId::MINUS_MINUS)) {
        AstNode* node = new AstNode(AstNodeType::AstUnaryExpr, token.start_line, token.start_column);
        node->unary_expr.expr = primary_expr;
        node->unary_expr.op = get_unary_op(token);
        return node;
    }
    lexer.get_back();

    return primary_expr;
}

/*
* Parses primary expressions
* primary_expr
*   : expression
*   | call_expr
*   | IDENTIFIER
*   | FLOAT_LIT
*   | INT_LIT
*   | UNICODE_CHAR
*/
AstNode* Parser::parse_primary_expr() noexcept {
    const Token& token = lexer.get_next_token();
    auto token_value = lexer.get_token_value(token);

    if (token.id == TokenId::_EOF) {
        const Token& prev_token = lexer.get_previous_token();
        parse_error(prev_token, ERROR_UNEXPECTED_EOF_AFTER, lexer.get_token_value(prev_token));
        return nullptr;
    }

    if (token.id == TokenId::L_PAREN) {
        lexer.get_back();
        return parse_expr();
    }

    if (token.id == TokenId::IDENTIFIER) {
        // CALL EXPR?
        // else
        goto parse_literal;
    }

    if (MATCH(token, TokenId::FLOAT_LIT, TokenId::INT_LIT, TokenId::UNICODE_CHAR)) {
parse_literal:
        AstNode* symbol_node = new AstNode(AstNodeType::AstSymbol, token.start_line, token.start_column);
        symbol_node->symbol.token = &token;
        return symbol_node;
    }

    parse_error(token, ERROR_EXPECTED_NUMBER_IDENTIFIER_CHAR_TOKEN, token_value, token_id_name(token.id));
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

bool Parser::is_new_line_between(const size_t start_pos, const size_t end_pos) {
    auto start_it = lexer.source.begin();
    auto str_view = std::string_view(start_it + start_pos, start_it + end_pos);

    return str_view.find_first_of('\n') != str_view.npos;
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

UnaryExprType get_unary_op(const Token& token) noexcept {
    switch (token.id) {
    case TokenId::PLUS_PLUS:
        return UnaryExprType::INC;
    case TokenId::MINUS_MINUS:
        return UnaryExprType::DEC;
    case TokenId::BIT_NOT:
        return UnaryExprType::NEG;
    case TokenId::RET:
        return UnaryExprType::RET;
    default:
        UNREACHEABLE;
    }
}

bool is_type_start_token(const Token& token) noexcept {
    switch (token.id) {
    case TokenId::IDENTIFIER:
    case TokenId::L_BRACKET:
    case TokenId::MUL:
        return true;
    default:
        return false;
    }
}

bool is_expr_token(const Token& token) noexcept {
    switch (token.id) {
    case TokenId::IDENTIFIER:
    case TokenId::FLOAT_LIT:
    case TokenId::INT_LIT:
    case TokenId::UNICODE_CHAR:
    case TokenId::NOT:
    case TokenId::BIT_NOT:
    case TokenId::PLUS_PLUS:
    case TokenId::MINUS_MINUS:
    case TokenId::MUL:
    case TokenId::DIV:
    case TokenId::MOD:
    case TokenId::LSHIFT:
    case TokenId::RSHIFT:
    case TokenId::BIT_AND:
    case TokenId::BIT_XOR:
    case TokenId::PLUS:
    case TokenId::MINUS:
    case TokenId::BIT_OR:
    case TokenId::EQUALS:
    case TokenId::NOT_EQUALS:
    case TokenId::GREATER:
    case TokenId::GREATER_OR_EQUALS:
    case TokenId::LESS:
    case TokenId::LESS_OR_EQUALS:
    case TokenId::L_PAREN:
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
