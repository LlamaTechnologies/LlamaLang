#pragma once
#include "error.hpp"

#include <string>
#include <unordered_map>
#include <vector>

struct AstFuncDef;
struct AstBlock;
struct AstNode;
enum class SymbolType;

struct Symbol {
  const std::string &name;
  const SymbolType type;
  const AstNode *const data_node;

  Symbol(const std::string &in_name, const SymbolType in_type, const AstNode *in_data)
      : name(in_name), type(in_type), data_node(in_data) {}
};

struct Table {
  using SymbolMap = std::unordered_map<std::string, Symbol>;
  using TableMap = std::unordered_map<std::string, Table *>;

  Table *parent;
  std::string name;

  Table(const std::string &in_name, Table *in_parent) : parent(in_parent), name(in_name) {}

  Table *create_child(const std::string &in_name);

  bool has_child(const std::string &in_name);

  bool has_symbol(const std::string &in_name);

  const Symbol &get_symbol(const std::string &in_name);

  Table *get_child(const std::string &in_name);

  void remove_last_child();

  void add_symbol(const std::string &in_name, const SymbolType in_type, const AstNode *in_data);

  void remove_last_symbol();

private:
  TableMap children_scopes;
  SymbolMap symbols;
  std::string last_symbol_key;
  std::string last_child_key;
};

class SemanticAnalyzer {
  Table *global_symbol_table = new Table("global_scope", nullptr);
  Table *symbol_table = global_symbol_table;
  std::vector<Error> &errors;

public:
  SemanticAnalyzer(std::vector<Error> &in_errors) : errors(in_errors) {}

  /* Returns true. */
  bool analizeFuncProto(const AstNode *in_func_proto);

  /* Returns true if:
   * - all middle statements are OK.
   * - return statment type match function's return type
   */
  bool analizeFuncBlock(const AstBlock &in_func_block, AstFuncDef &in_function);

  /* Returns true if:
   * - global variable has initializer.
   * - initializer expr is compatible with variable's type.
   * - variable is not shadowing another in the same scope.
   */
  bool analizeVarDef(const AstNode *in_node, const bool is_global);

  /* Returns true if:
   * - all refered variables had been defined
   * - all constants types are valid to be in the same operation
   */
  bool analizeExpr(const AstNode *in_expr);

  /* Returns true if:
   * - types are the same type_id
   * - if pointers types have the same sub type_id
   */
  bool check_type_compat(const AstNode *type_node0, const AstNode *type_node1, const AstNode *expr_node);

private:
  const AstNode *get_expr_type(const AstNode *expr);

  const AstNode *resolve_function_variable(const std::string &in_name, const AstNode *in_parent_node,
                                           SymbolType *out_symbol_type = nullptr);

  void add_semantic_error(const AstNode *in_node, const char *in_msg, ...);
};
