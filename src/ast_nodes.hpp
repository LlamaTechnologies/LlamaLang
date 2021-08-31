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
struct AstBlock;
struct AstSourceCode;

struct AstVarDef;
struct AstSymbol;
struct AstFnCallExpr;
struct AstBinaryExpr;
struct AstUnaryExpr;

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

struct AstDirective {
  DirectiveType directive_type = DirectiveType::COMPILE;
  union {
    std::string_view str = {};
    AstNode *ast_node;
  } argument;
};

struct AstFnDef {
  AstNode *proto = nullptr;
  AstNode *block = nullptr;
  llvm::Function *llvm_value = nullptr;

  virtual ~AstFnDef();
};

struct AstFnProto {
  std::vector<AstNode *> params;
  std::string_view name;
  AstNode *return_type = nullptr;
  bool is_extern = false;

  virtual ~AstFnProto();
};

struct AstBlock {
  std::vector<AstNode *> statements;

  AstBlock() : statements(std::vector<AstNode *>()) {}

  virtual ~AstBlock();
};

struct AstVarDef {
  std::string_view name;
  AstNode *type = nullptr;
  AstNode *initializer = nullptr;
  mutable llvm::Value *llvm_value;

  virtual ~AstVarDef();
};

typedef AstVarDef AstParamDef;

enum class SymbolType
{
  FUNC,
  VAR
};

struct AstSymbol {
  std::string_view cached_name;
  const Token *token;
  mutable const AstNode *data; // resolved ast node (DO NOT OWN IT)
  mutable SymbolType type;
};

enum class ConstValueType
{
  BOOL,
  INT,
  FLOAT,
  CHAR
};

struct AstConstValue {
  union {
    bool boolean;
    uint32_t unicode_char;
    const char *number = nullptr;
  };
  ConstValueType type = ConstValueType::BOOL;
  mutable uint8_t bit_size = 1;
  bool is_negative = false;

  virtual ~AstConstValue();
};

struct AstFnCallExpr {
  std::vector<AstNode *> args;
  std::string_view fn_name;
  mutable const AstNode *fn_ref;

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

struct AstBinaryExpr {
  AstNode *left_expr = nullptr;
  AstNode *right_expr = nullptr;
  BinaryExprType bin_op;

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

struct AstUnaryExpr {
  UnaryExprType op;
  AstNode *expr = nullptr;

  virtual ~AstUnaryExpr();
};

struct AstSourceCode {
  std::vector<AstNode *> children;
  const Lexer *lexer = nullptr;

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
  std::string_view name = "";
  LLVMTypeRef llvm_type = nullptr;
  uint32_t bit_size = 1;
  bool is_signed = false;
};

struct AstType {
  TypeInfo type_info;            // null if type == (pointer | array)
  AstNode *child_type = nullptr; // Not null if type == (pointer | array)
  AstTypeId type_id;             // Pointer Array Integer FloatingPoint

  AstType() : type_id(AstTypeId::VOID), child_type(nullptr) {}

  AstType(AstTypeId in_type_id, std::string_view in_name, uint32_t in_bit_size, bool in_is_signed)
      : type_id(in_type_id), child_type(nullptr), type_info({ in_name, nullptr, in_bit_size, in_is_signed }) {}

  virtual ~AstType();
};

// ast nodes enum
enum class AstNodeType
{
  AST_SOURCE_CODE,
  AST_DIRECTIVE,
  AST_TYPE,
  AST_FUNC_DEF,
  AST_FUNC_PROTO,
  AST_BLOCK,
  AST_PARAM_DECL,
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

  // actual node
  // TODO: try to use std::variant instead of union
  // union {
  AstSourceCode source_code;
  AstDirective directive;    // # dir_name args*
  AstFnDef function_def;     // function definition
  AstFnProto function_proto; // fn name L_PAREN param_decl (, param_decl)* R_PAREN
  AstParamDef param_decl;   // name type
  AstBlock block;            // L_CURLY statements R_CURLY
  AstVarDef var_def;         // name type
  AstType ast_type;          // type
  AstUnaryExpr unary_expr;   // unary_op expr
  AstBinaryExpr binary_expr; // expr binary_op expr
  AstSymbol symbol;          // symbol_name
  AstConstValue const_value; // constant value
  AstFnCallExpr func_call;   // func_name L_PAREN (expr (, expr)*)? R_PAREN
  //};

  AstNode(AstNodeType in_node_type, size_t in_line, size_t in_column, std::string_view in_file_name)
      : parent(nullptr), line(in_line), column(in_column), node_type(in_node_type), file_name(in_file_name) {}

  static AstNode *CreatePredefType(const AstType &in_type) {
    AstNode *node = new AstNode(AstNodeType::AST_TYPE, 0, 0, "predefined");
    node->ast_type = in_type;
    return node;
  }

  virtual ~AstNode() {}
};
