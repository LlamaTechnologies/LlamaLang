#pragma once
#include "ast_nodes.hpp"

#include <string>

struct TypesRepository {
  static TypesRepository &get();

  /**
   * Get the type id of the type with name 'in_name'
   **/
  const AstTypeId get_type_id(std::string_view in_name) noexcept;

  /**
   * Get the ast node (AstType) of the type with name 'in_name'
   **/
  AstType *get_type_node(std::string_view in_name) noexcept;

  /**
   * Get the type information of the type with name 'in_name'
   **/
  const TypeInfo *get_type(std::string_view in_name) noexcept;

  /**
   * Register a new type
   * return: true onm success and false if already existed a type with the same name.
   **/
  bool register_type(std::string_view in_name) noexcept;

private:
  std::unordered_map<std::string_view, const TypeInfo> types_id;
};
