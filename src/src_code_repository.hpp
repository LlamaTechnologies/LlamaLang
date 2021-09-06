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
  static void clean();

  bool has_file(std::string_view in_file_name) const;
  AstSourceCode *get_source_code(std::string_view in_file_name);

private:
  explicit RepositorySrcCode() = default;
  friend class Parser;

  void _add_source_code(const std::string_view in_file_name, AstSourceCode *in_source_code);

private:
  std::unordered_map<std::string_view, AstSourceCode *> src_codes;
};
