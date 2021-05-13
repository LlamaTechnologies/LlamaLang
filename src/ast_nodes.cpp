#include "ast_nodes.hpp"

static std::vector<const char*> directives_keywords = {
    "run",
    "load",
    "compile_only"
};

const std::string get_directive_type_name(const DirectiveType directive_type) noexcept
{
    return directives_keywords.at((size_t)directive_type);
}
