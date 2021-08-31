#pragma once
#include "error.hpp"
#include "scope.hpp"

#include <string>
#include <unordered_map>
#include <vector>

struct AstFnDef;
struct AstBlock;
struct AstNode;
enum class SymbolType;

class SemanticAnalyzer {
  Table *global_symbol_table = new Table("global_scope", nullptr);
  Table *symbol_table = global_symbol_table;
  std::vector<Error> &errors;

public:
  SemanticAnalyzer(std::vector<Error> &in_errors) : errors(in_errors) {}

  /* Returns true if:
   * - global variable has initializer.
   * - initializer expr is compatible with variable's type.
   * - variable is not shadowing another in the same scope.
   */
  bool analizeVarDef(const AstNode *in_node, const bool is_global);

  /* TEMP: Returns true.
   * NOTE: Here we will check fn_type
   */
  bool analizeFuncProto(const AstNode *in_func_proto);

  /* Returns true if:
   * - all middle statements are OK.
   * - return statment type match function's return type
   */
  bool analizeFuncBlock(const AstBlock &in_func_block, AstFnDef &in_function);

  /* Returns true if:
   * - all refered variables had been defined
   * - all constants types are valid to be in the same operation
   */
  bool analizeExpr(const AstNode *in_expr);

  bool check_and_set_type(const AstNode *in_node, const AstNode *l_type_node, const AstNode *expr_node);
};

/* Returns true if:
 * - types are the same type_id
 * - types are the same bit size
 * - both integers are signed or uinsigned
 * - pointers|arrays types have the same sub type_id
 */
bool check_types(std::vector<Error> &errors, const AstNode *type_node0, const AstNode *type_node1,
                 const AstNode *expr_node);

/* Returns true if:
 * - types are the same bit size
 * - both integers are signed or uinsigned
 * - pointers|arrays types have the same sub type_id
 */
bool check_compatible_types(std::vector<Error> &errors, const AstNode *type_node0, const AstNode *type_node1,
                            const AstNode *expr_node);

/* Returns a node representing the expression type */
const AstNode *get_expr_type(std::vector<Error> &errors, const Table *symbol_table, const AstNode *expr);

/* Returns a node representing the variable or the function found by the name */
const AstNode *resolve_function_variable(std::vector<Error> &errors, const Table *symbol_table,
                                         const std::string &in_name, const AstNode *in_parent_node,
                                         SymbolType *out_symbol_type = nullptr);

void add_semantic_error(std::vector<Error> &errors, const AstNode *in_node, const char *in_msg, ...);
