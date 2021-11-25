#include "src_code_repository.hpp"

static RepositorySrcCode *repository = nullptr;

RepositorySrcCode &RepositorySrcCode::get() {
  if (!repository) {
    repository = new RepositorySrcCode();
  }
  return *repository;
}

void RepositorySrcCode::clean() {
  if (!repository)
    return;

  repository->src_codes.erase(repository->src_codes.begin(), repository->src_codes.end());
}

bool RepositorySrcCode::has_file(const std::string &in_file_name) const {
  return this->src_codes.find(in_file_name) != this->src_codes.end();
}

AstSourceCode *RepositorySrcCode::get_source_code(const std::string &in_file_name) {
  return this->src_codes.at(in_file_name);
}

void RepositorySrcCode::_add_source_code(const std::string &in_file_name, AstSourceCode *in_source_code) {
  if (!has_file(in_file_name)) {
    this->src_codes.emplace(in_file_name, in_source_code);
  }
}
