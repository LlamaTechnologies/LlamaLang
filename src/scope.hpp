#pragma once
#include <string>
#include <unordered_map>

enum class SymbolType;
struct AstNode;

/**
 * Symbol:
 *   Represents a variable, function or
 * any other referenceable language object.
 * It is stored in the symbol Table.
 * It also has the resolved data at which
 * the symbol reference.
 **/
struct Symbol {
  const std::string &name;
  const SymbolType type;
  const AstNode *const data_node;

  Symbol(const std::string &in_name, const SymbolType in_type, const AstNode *in_data)
      : name(in_name), type(in_type), data_node(in_data) {}
};

/**
 * Table (a.k.a Symbols Table):
 * NOTE: This could be a scope|block
 **/
struct Table {
  using SymbolMap = std::unordered_map<std::string, Symbol>;
  using TableMap = std::unordered_map<std::string, Table *>;

  Table *parent;
  const std::string name;

  Table(const std::string &in_name, Table *in_parent) : parent(in_parent), name(in_name) {}

  Table *create_child(const std::string &in_name);

  bool has_child(const std::string &in_name) const;

  bool has_symbol(const std::string &in_name) const;

  const Symbol &get_symbol(const std::string &in_name) const;

  Table *get_child(const std::string &in_name);

  void remove_last_child();

  void add_symbol(const std::string &in_name, const SymbolType in_type, const AstNode *in_data);

  void remove_last_symbol();

private:
  std::string last_symbol_key;
  std::string last_child_key;
  TableMap children_scopes;
  SymbolMap symbols;
};