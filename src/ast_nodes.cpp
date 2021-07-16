#include "ast_nodes.hpp"

static std::vector<const char*> directives_keywords = {
    "run",
    "load",
    "compile_only"
};

const std::string get_directive_type_name(const DirectiveType directive_type) noexcept {
    assert(directive_type <= DirectiveType::CompTimeOnly);
    return directives_keywords.at((size_t)directive_type);
}

static std::vector<const char*> unary_operators_symbols = {
    "++",
    "--",
    "-",
    "!",
    "~",
};

const std::string get_unary_op_symbol(const UnaryExprType op_type) noexcept {
    assert(op_type < UnaryExprType::RET);
    return unary_operators_symbols[(size_t)op_type];
}
