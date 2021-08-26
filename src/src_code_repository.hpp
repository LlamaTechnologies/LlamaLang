#pragma once

/**
 * This file contains the source codes repositories.
 * This is used to store parsed source code files from #load
 **/

#include "ast_nodes.hpp"

#include <string_view>
#include <unordered_map>

class Parser;

struct RepositorySrcCode {
  static RepositorySrcCode &get();
  bool has_file(std::string_view in_file_name);
  AstNode *get_source_code(std::string_view in_file_name);

private:
  explicit RepositorySrcCode() = default;
  friend class Parser;

  void add_source_code(const std::string_view in_file_name, AstNode *in_source_code);

private:
  std::unordered_map<std::string_view, AstNode *> src_codes;
};
