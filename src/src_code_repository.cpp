#include "src_code_repository.hpp"

static RepositorySrcCode *repository = nullptr;

RepositorySrcCode &RepositorySrcCode::get() {
  if (!repository) {
    repository = new RepositorySrcCode();
  }
  return *repository;
}

bool RepositorySrcCode::has_file(std::string_view in_file_name) const {
  return this->src_codes.find(in_file_name) != this->src_codes.end();
}

AstNode *RepositorySrcCode::get_source_code(std::string_view in_file_name) { return this->src_codes.at(in_file_name); }

void RepositorySrcCode::_add_source_code(const std::string_view in_file_name, AstNode *in_source_code) {
  if (!has_file(in_file_name)) {
    this->src_codes.emplace(in_file_name, in_source_code);
  }
}
