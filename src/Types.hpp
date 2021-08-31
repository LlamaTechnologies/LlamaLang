#pragma once
#include "ast_nodes.hpp"

#include <string>

/**
 * Get the type id of the type with name 'in_name'
 **/
const AstTypeId get_type_id(std::string_view in_name) noexcept;

/**
 * Get the ast node (AstType) of the type with name 'in_name'
 **/
AstNode *get_type_node(std::string_view in_name) noexcept;
