#pragma once
#include "ast_nodes.hpp"

#include <string>

AstTypeId get_type_id(std::string_view in_name, TypeInfo **info) noexcept;

AstNode *get_type_node(std::string_view in_name) noexcept;
