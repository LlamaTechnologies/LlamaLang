#pragma once
#include "bigfloat.hpp"
#include "bigint.hpp"
#include "common_defs.hpp"

#include <assert.h>
#include <cstddef>
#include <llvm/IR/Type.h>
#include <string>
#include <vector>

class Lexer;

namespace llvm {
class Function;
class Value;
} // namespace llvm

typedef struct LLVMOpaqueType *LLVMTypeRef;

// ast nodes
struct Token;
struct AstNode;
struct AstDirective;
struct AstType;
struct AstFnDef;
struct AstFnProto;
struct AstIfStmnt;
struct AstBlock;
struct AstSourceCode;

struct AstVarDef;
struct AstParamDef;
struct AstSymbol;
struct AstFnCallExpr;
struct AstBinaryExpr;
struct AstUnaryExpr;
struct AstConstValue;

// ast nodes enum
enum class AstNodeType
{
  AST_SOURCE_CODE,
  AST_DIRECTIVE,
  AST_TYPE,
  AST_FN_DEF,
  AST_FN_PROTO,
  AST_IF_STMNT,
  AST_BLOCK,
  AST_PARAM_DEF,
  AST_VAR_DEF,
  AST_SYMBOL,
  AST_CONST_VALUE,
  AST_FN_CALL_EXPR,
  AST_BINARY_EXPR,
  AST_UNARY_EXPR
};

// base ast node
struct AstNode {
  AstNode *parent = nullptr;
  std::string_view file_name;
  size_t line;
  size_t column;
  AstNodeType node_type = AstNodeType::AST_SOURCE_CODE;

  AstNode(AstNodeType in_node_type, size_t in_line, size_t in_column, std::string_view in_file_name)
      : parent(nullptr), line(in_line), column(in_column), node_type(in_node_type), file_name(in_file_name) {}

  virtual ~AstNode() {}

  AstSourceCode *source_code() { return (AstSourceCode *)this; }
  AstDirective *directive() { return (AstDirective *)this; }     // # dir_name args*
  AstFnDef *fn_def() { return (AstFnDef *)this; }                // function definition
  AstFnProto *fn_proto() { return (AstFnProto *)this; }          // fn name L_PAREN param_decl (, param_decl)* R_PAREN
  AstParamDef *param_decl() { return (AstParamDef *)this; }      // name type
  AstBlock *block() { return (AstBlock *)this; }                 // L_CURLY statements R_CURLY
  AstVarDef *var_def() { return (AstVarDef *)this; }             // name type
  AstType *ast_type() { return (AstType *)this; }                // type
  AstUnaryExpr *unary_expr() { return (AstUnaryExpr *)this; }    // unary_op expr
  AstBinaryExpr *binary_expr() { return (AstBinaryExpr *)this; } // expr binary_op expr
  AstSymbol *symbol() { return (AstSymbol *)this; }              // symbol_name
  AstConstValue *const_value() { return (AstConstValue *)this; } // constant value
  AstFnCallExpr *fn_call() { return (AstFnCallExpr *)this; }     // func_name L_PAREN (expr (, expr)*)? R_PAREN
  AstIfStmnt *if_stmnt() { return (AstIfStmnt *)this; }          // if elif? else

  const AstSourceCode *source_code() const { return (AstSourceCode *)this; }
  const AstDirective *directive() const { return (AstDirective *)this; }
  const AstFnDef *fn_def() const { return (AstFnDef *)this; }
  const AstFnProto *fn_proto() const { return (AstFnProto *)this; }
  const AstParamDef *param_decl() const { return (AstParamDef *)this; }
  const AstBlock *block() const { return (AstBlock *)this; }
  const AstVarDef *var_def() const { return (AstVarDef *)this; }
  const AstType *ast_type() const { return (AstType *)this; }
  const AstUnaryExpr *unary_expr() const { return (AstUnaryExpr *)this; }
  const AstBinaryExpr *binary_expr() const { return (AstBinaryExpr *)this; }
  const AstSymbol *symbol() const { return (AstSymbol *)this; }
  const AstConstValue *const_value() const { return (AstConstValue *)this; }
  const AstFnCallExpr *fn_call() const { return (AstFnCallExpr *)this; }
  const AstIfStmnt *if_stmnt() const { return (AstIfStmnt *)this; }
};

struct AstVarDef : public AstNode {
  std::string_view name;
  AstType *type = nullptr; // OWNED BY TYPE REPOSITORY
  AstNode *initializer = nullptr;
  mutable llvm::Value *llvm_value;

  AstVarDef(size_t in_line, size_t in_column, std::string_view in_file_name)
      : AstNode(AstNodeType::AST_VAR_DEF, in_line, in_column, in_file_name) {}

  virtual ~AstVarDef();

protected:
  AstVarDef(AstNodeType in_type, size_t in_line, size_t in_column, std::string_view in_file_name)
      : AstNode(in_type, in_line, in_column, in_file_name) {}
};

struct AstParamDef : public AstVarDef {
  AstParamDef(size_t in_line, size_t in_column, std::string_view in_file_name)
      : AstVarDef(AstNodeType::AST_PARAM_DEF, in_line, in_column, in_file_name) {}
};

// IMPORTANT: do not change order of labels!
enum class DirectiveType
{
  LOAD,    // load file (first directive in the file)
  MAIN,    // set entry point (1 per executable)
  RUN,     // run fn at compile-time
  COMPILE, // symbol is safe for use in compile-time
  FN_TYPE  // check function time at compile time
};

const std::string get_directive_type_name(const DirectiveType) noexcept;

struct AstDirective : public AstNode {
  DirectiveType directive_type = DirectiveType::COMPILE;
  union {
    const char *str = nullptr;
    AstNode *ast_node; // may be a function proto node (OWNED BY PARSER)
  } argument;

  AstDirective(size_t in_line, size_t in_column, std::string_view in_file_name)
      : AstNode(AstNodeType::AST_DIRECTIVE, in_line, in_column, in_file_name) {}
};

struct AstFnDef : public AstNode {
  AstFnProto *proto = nullptr;
  AstBlock *block = nullptr;
  llvm::Function *llvm_value = nullptr;

  AstFnDef(size_t in_line, size_t in_column, std::string_view in_file_name)
      : AstNode(AstNodeType::AST_FN_DEF, in_line, in_column, in_file_name) {}
  virtual ~AstFnDef();
};

struct AstFnProto : public AstNode {
  std::vector<AstParamDef *> params;
  std::string_view name;
  AstType *return_type = nullptr;
  bool is_extern = false;

  AstFnProto(size_t in_line, size_t in_column, std::string_view in_file_name)
      : AstNode(AstNodeType::AST_FN_PROTO, in_line, in_column, in_file_name) {}

  virtual ~AstFnProto();
};

struct AstIfStmnt : public AstNode {
  AstNode *condition_expr;
  AstBlock *if_block = nullptr;
  AstNode *else_block = nullptr; // AstBlock | AstIfStmnt (for elif)

  AstIfStmnt(size_t in_line, size_t in_column, std::string_view in_file_name)
      : AstNode(AstNodeType::AST_IF_STMNT, in_line, in_column, in_file_name) {}

  ~AstIfStmnt();
};

struct AstBlock : public AstNode {
  std::vector<AstNode *> statements;

  AstBlock(size_t in_line, size_t in_column, std::string_view in_file_name)
      : AstNode(AstNodeType::AST_BLOCK, in_line, in_column, in_file_name), statements(std::vector<AstNode *>()) {}

  virtual ~AstBlock();
};

enum class SymbolType
{
  FUNC,
  VAR
};

struct AstSymbol : public AstNode {
  std::string_view cached_name;
  const Token *token;          // token (OWNED BY LEXER)
  mutable const AstNode *data; // resolved ast node (OWNED BY PARSER)
  mutable SymbolType type;

  AstSymbol(size_t in_line, size_t in_column, std::string_view in_file_name)
      : AstNode(AstNodeType::AST_SYMBOL, in_line, in_column, in_file_name) {}
};

enum class ConstValueType
{
  BOOL,
  INT,
  FLOAT,
  CHAR
};

struct AstConstValue : public AstNode {
  union {
    bool boolean;
    uint32_t unicode_char;
    const char *number = nullptr;
  };
  ConstValueType type = ConstValueType::BOOL;
  mutable uint8_t bit_size = 1;
  bool is_negative = false;

  AstConstValue(size_t in_line, size_t in_column, std::string_view in_file_name)
      : AstNode(AstNodeType::AST_CONST_VALUE, in_line, in_column, in_file_name) {}

  virtual ~AstConstValue();
};

struct AstFnCallExpr : public AstNode {
  std::vector<AstNode *> args;
  std::string_view fn_name;
  mutable const AstFnProto *fn_ref = nullptr;

  AstFnCallExpr(size_t in_line, size_t in_column, std::string_view in_file_name)
      : AstNode(AstNodeType::AST_FN_CALL_EXPR, in_line, in_column, in_file_name) {}

  virtual ~AstFnCallExpr();
};

enum class BinaryExprType
{
  ADD,               // expr +  expr
  SUB,               // expr -  expr
  MUL,               // expr *  expr
  DIV,               // expr /  expr
  MOD,               // expr %  expr
  EQUALS,            // expr == expr
  NOT_EQUALS,        // expr != expr
  GREATER_OR_EQUALS, // expr >= expr
  LESS_OR_EQUALS,    // expr <= expr
  GREATER,           // expr >  expr
  LESS,              // expr <  expr
  LSHIFT,            // expr << expr
  RSHIFT,            // expr >> expr
  BIT_XOR,           // expr ^  expr
  BIT_AND,           // expr &  expr
  ASSIGN,            // expr =  expr
};

struct AstBinaryExpr : public AstNode {
  AstNode *left_expr = nullptr;
  AstNode *right_expr = nullptr;
  BinaryExprType bin_op;

  AstBinaryExpr(size_t in_line, size_t in_column, std::string_view in_file_name)
      : AstNode(AstNodeType::AST_BINARY_EXPR, in_line, in_column, in_file_name) {}

  virtual ~AstBinaryExpr();
};

// IMPORTANT: do not change order of labels!
enum class UnaryExprType
{
  INC,     // ++  primaryExpr
  DEC,     // --  primaryExpr
  NEG,     // -   primaryExpr
  NOT,     // !   primaryExpr
  BIT_INV, // ~   primaryExpr
  RET      // ret Expr
};

const std::string get_unary_op_symbol(const UnaryExprType op_type) noexcept;

struct AstUnaryExpr : public AstNode {
  UnaryExprType op;
  AstNode *expr = nullptr;

  AstUnaryExpr(size_t in_line, size_t in_column, std::string_view in_file_name)
      : AstNode(AstNodeType::AST_UNARY_EXPR, in_line, in_column, in_file_name) {}

  virtual ~AstUnaryExpr();
};

struct AstSourceCode : public AstNode {
  std::vector<AstNode *> children;
  const Lexer *lexer = nullptr;
  bool is_analyzed = false;

  AstSourceCode(size_t in_line, size_t in_column, std::string_view in_file_name)
      : AstNode(AstNodeType::AST_SOURCE_CODE, in_line, in_column, in_file_name) {}

  virtual ~AstSourceCode();
};

enum class AstTypeId
{
  VOID,
  BOOL,
  INTEGER,
  FLOATING_POINT,
  POINTER,
  ARRAY,
  STRUCT,
  UNKNOWN
};

struct TypeInfo {
  std::string_view name;
  LLVMTypeRef llvm_type;
  uint32_t bit_size;
  AstTypeId type_id;
  bool is_signed;

  TypeInfo() : name(""), llvm_type(nullptr), bit_size(0), type_id(AstTypeId::VOID), is_signed(false) {}

  TypeInfo(const AstTypeId in_type_id, const std::string_view in_name, const uint32_t in_bit_size,
           const bool in_is_signed)
      : name(in_name), llvm_type(nullptr), bit_size(in_bit_size), type_id(in_type_id), is_signed(in_is_signed) {}

  virtual ~TypeInfo();
};

struct AstType : public AstNode {
  const TypeInfo *type_info;     // null if type == (pointer | array)
  AstType *child_type = nullptr; // Not null if type == (pointer | array)

  AstType();

  AstType(const size_t in_line, const size_t in_column, const std::string_view in_file_name);

  AstType(const TypeInfo *in_type);

  AstType(const TypeInfo *in_type, const size_t in_line, const size_t in_column, const std::string_view in_file_name);

  virtual ~AstType();
};
