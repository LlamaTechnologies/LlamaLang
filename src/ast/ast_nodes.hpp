#pragma once
#include "../common_defs.hpp"
#include "../semantics/semantics_fwd.hpp"
#include "../tokenizer/tokenizer_fwd.hpp"
#include "ast_fwd.hpp"

#include <assert.h>
#include <cstddef>
#include <string>
#include <vector>

namespace llvm {
class Function;
class Value;
class BasicBlock;
} // namespace llvm

typedef struct LLVMOpaqueType *LLVMTypeRef;

struct AstFile {
  s32 id;
  u32 flags;
  std::string fullpath;
  Scope *scope;

  Tokenizer tokenizer;
  std::vector<Token> tokens;
  u64 curr_token_index;
  u64 prev_token_index;
  Token curr_token;
  Token prev_token; // previous non-comment

  std::vector<AstNode *> decls;
  std::vector<AstNode *> imports;          // 'import'
  std::vector<AstCommentGroup *> comments; // All the comments!
  AstCommentGroup *lead_comment;           // Comment (block) before the decl
  AstCommentGroup *line_comment;           // Comment after the semicolon
  AstCommentGroup *docs;                   // current docs
};

struct AstCommentGroup {
  std::vector<Token> list;
};

struct AstType {
  Token name;
  AstTypeId id;

  union {
    BasicType basic_type;
    Pointer ptr_type;
    Array array_type;
    Enum enum_type;
    Struct struct_type;
    Union union_type;
    FnType fn_type;
  };
};

struct AstVariable {
  Token name;
  AstType *type = nullptr;
  AstNode *init_expr = nullptr;
  AstNode *param_expr = nullptr;
};

struct AstIdentifier {
  Token token;
  AstNode *entity; // entity associated to the identifier
};

struct AstDirective {
  DirectiveType directive_type = DirectiveType::COMPILE;
  Token name;
};

struct AstFunction {
  AstType *type;
  AstBlock *block = nullptr;
};

struct AstBinaryExpr {
  Token op;
  AstNode *left_expr;
  AstNode *right_expr;
};

struct AstUnaryExpr {
  Token op;
  AstNode *expr;
};

struct AstCallExpr {
  AstIdentifier fn;
  std::vector<AstNode *> args;
};

//--------------------------------

struct AstIfStmnt {
  AstNode *condition_expr = nullptr;
  AstBlock *true_block = nullptr;
  AstBlock *false_block = nullptr;
  bool is_condition_checked = false;

  AstIfStmnt(size_t in_line, size_t in_column, std::string_view in_file_name)
      : AstNode(AstNodeType::AST_IF_STMNT, in_line, in_column, in_file_name) {}

  ~AstIfStmnt();
};

struct AstLoopStmnt {
  AstNode *condition_expr = nullptr;
  AstBlock *initializer_block = nullptr;
  AstBlock *header_block = nullptr;
  AstBlock *content_block = nullptr;
  AstBlock *footer_block = nullptr;
  mutable llvm::BasicBlock *next_block = nullptr;
  bool is_condition_checked = false;

  AstLoopStmnt(size_t in_line, size_t in_column, std::string_view in_file_name)
      : AstNode(AstNodeType::AST_LOOP_STMNT, in_line, in_column, in_file_name) {}

  ~AstLoopStmnt();
};

struct AstBlock {
  std::vector<AstNode *> statements;
  mutable llvm::BasicBlock *llvm_value;

  AstBlock(size_t in_line, size_t in_column, std::string_view in_file_name)
      : AstNode(AstNodeType::AST_BLOCK, in_line, in_column, in_file_name), statements(std::vector<AstNode *>()) {}

  virtual ~AstBlock();
};

struct AstConstValue {
  union {
    bool boolean;
    u32 unicode_char;
    const char *number = nullptr;
  };
  ConstValueType type = ConstValueType::BOOL;
  mutable ConstValueType child_type = ConstValueType::BOOL;
  mutable uint8_t bit_size = 1;
  bool is_negative = false;

  AstConstValue(size_t in_line, size_t in_column, std::string_view in_file_name)
      : AstNode(AstNodeType::AST_CONST_VALUE, in_line, in_column, in_file_name) {}

  virtual ~AstConstValue();
};

struct AstCtrlStmnt {
  mutable AstLoopStmnt *loop_ref = nullptr;
  const char *label = nullptr;
  size_t index = 0;
  CtrlStmntType ctrl_type;

  AstCtrlStmnt(size_t in_line, size_t in_column, std::string_view in_file_name)
      : AstNode(AstNodeType::AST_CTRL_STMNT, in_line, in_column, in_file_name) {}
  virtual ~AstCtrlStmnt();
};

// base ast node
struct AstNode {
  Scope *scope;
  AstFile *file;

  AstNodeId node_id = AstNodeId::AST_SOURCE_CODE;
  union {
    AstType type;
    AstVarDef var;
    AstFunction fn;
    AstDirective directive;
    AstBlock block;
    AstUnaryExpr unary_expr;
    AstBinaryExpr binary_expr;
    AstIdentifier identifier;
    AstConstValue const_value;
    AstFnCallExpr fn_call;
    AstIfStmnt if_stmnt;
    AstLoopStmnt loop_stmnt;
    AstCtrlStmnt ctrl_stmnt;
  };
};
