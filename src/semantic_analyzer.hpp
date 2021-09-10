#pragma once
#include "error.hpp"
#include "scope.hpp"

#include <string>
#include <unordered_map>
#include <vector>

struct AstNode;
struct AstType;
struct AstVarDef;
struct AstFnProto;
struct AstFnDef;
struct AstFnCallExpr;
struct AstUnaryExpr;
struct AstBinaryExpr;
struct AstBlock;
enum class SymbolType;

class SemanticAnalyzer {
  Table *global_symbol_table = new Table("global_scope", nullptr, nullptr);
  Table *symbol_table = global_symbol_table;
  std::vector<Error> &errors;

public:
  SemanticAnalyzer(std::vector<Error> &in_errors) : errors(in_errors) {}

  /* Returns true if:
   * - initializer expr is compatible with variable's type.
   * - variable is not shadowing another in the same scope.
   */
  bool analize_var_def(const AstVarDef *in_node);

  /* Returns true if:
   * - variable has initializer.
   * - initializer expr is compatible with variable's type.
   * - variable is not shadowing another in the same scope.
   */
  bool analize_global_var_def(const AstVarDef *in_node);

  /**
   * TEMP: Returns true.
   * NOTE: Here we will check fn_type
   */
  bool analize_fn_proto(const AstFnProto *in_fn_proto);

  /**
   * set current scope to be the one of the function passed
   */
  void enter_fn_scope(AstFnDef *in_function);

  /**
   * set current scope to be the parent of the current scope
   */
  void exit_fn_scope();

  /* Returns true if:
   * - all middle statements are OK.
   * - return statment type match function's return type
   */
  bool analize_fn_block(const AstBlock *in_fn_block);

  /* Returns true if:
   * - all middle statements are OK.
   */
  bool analize_block(const AstBlock *in_block, bool is_first_level_block = false);

  /* Returns true if:
   * - conditional expression is of boolean type
   * - block has no errors
   * - none of its else block has errors
   */
  bool analize_if_stmnt(const AstNode *in_expr);

  /* Returns true if:
   * - all refered variables had been defined
   * - all constants types are valid to be in the same operation
   */
  bool analize_expr(const AstNode *in_expr);

  bool check_and_set_type(const AstNode *in_node, const AstType *l_type_node, const AstNode *expr_node);

private:
  inline bool _analize_call_expr(const AstFnCallExpr *in_fn_call);
  inline bool _analize_unary_expr(const AstUnaryExpr *in_unary_expr);
  inline bool _analize_binary_expr(const AstBinaryExpr *in_binary_expr);
};

/* Returns true if:
 * - types are the same type_id
 * - types are the same bit size
 * - both integers are signed or uinsigned
 * - pointers|arrays types have the same sub type_id
 */
bool check_types(std::vector<Error> &errors, const AstType *type_node0, const AstType *type_node1,
                 const AstNode *expr_node);

/* Returns true if:
 * - types are the same bit size
 * - both integers are signed or uinsigned
 * - pointers|arrays types have the same sub type_id
 */
bool check_compatible_types(std::vector<Error> &errors, const AstType *type_node0, const AstType *type_node1,
                            const AstNode *expr_node);

/* Returns a node representing the expression type */
const AstType *get_expr_type(std::vector<Error> &errors, const Table *symbol_table, const AstNode *expr);

/* Returns a node representing the variable or the function found by the name:
 * Can be an AstVarDef|AstParamDef or an AstFnProto for a function.*/
const AstNode *resolve_function_variable(std::vector<Error> &errors, const Table *symbol_table,
                                         const std::string &in_name, const AstNode *in_parent_node,
                                         SymbolType *out_symbol_type = nullptr);

void add_semantic_error(std::vector<Error> &errors, const AstNode *in_node, const char *in_msg, ...);
