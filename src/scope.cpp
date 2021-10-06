#include "scope.hpp"

#include "ast_nodes.hpp"
#include "common_defs.hpp"

Table *Table::create_child(const std::string &in_name, const AstFnProto *in_fn_proto) {
  auto child = new Table(in_name, in_fn_proto, this);
  last_child_key = in_name;
  children_scopes.emplace(in_name, child);
  return child;
}

bool Table::has_child(const std::string &in_name) const {
  return children_scopes.find(in_name) != children_scopes.end();
}

bool Table::has_symbol(const std::string &in_name) const { return symbols.find(in_name) != symbols.end(); }

Table *Table::get_child(const std::string &in_name) {
#ifdef LL_DEBUG
  if (has_child(in_name))
#endif
    return children_scopes.at(in_name);
  LL_UNREACHEABLE;
}

const Symbol &Table::get_symbol(const std::string &in_name) const {
#ifdef LL_DEBUG
  if (has_symbol(in_name))
#endif
    return symbols.at(in_name);
  LL_UNREACHEABLE;
}

void Table::remove_last_child() { children_scopes.erase(last_child_key); }

void Table::add_symbol(const std::string &in_name, const SymbolType in_type, const AstNode *in_data) {
  LL_ASSERT(in_data != nullptr);
  LL_ASSERT(in_data->node_type == AstNodeType::AST_VAR_DEF || in_data->node_type == AstNodeType::AST_PARAM_DEF ||
            in_data->node_type == AstNodeType::AST_FN_PROTO);

  Symbol symbol = Symbol(in_name, in_type, in_data);
  last_symbol_key = in_name;
  symbols.emplace(in_name, symbol);
}

void Table::remove_last_symbol() { symbols.erase(last_symbol_key); }