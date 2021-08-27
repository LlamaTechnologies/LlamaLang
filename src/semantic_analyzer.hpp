#pragma once
#include "error.hpp"
#include "scope.hpp"

#include <string>
#include <unordered_map>
#include <vector>

struct AstFuncDef;
struct AstBlock;
struct AstNode;
enum class SymbolType;

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
