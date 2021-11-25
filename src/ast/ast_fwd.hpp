#pragma once
#include "../common_defs.hpp"

#include <string>
#include <string_view>

// ast nodes
struct AstFile;
struct AstCommentGroup;
struct AstNode;
struct AstType;
struct AstVariable;
struct AstIdentifier;
struct AstDirective;
struct AstFunction;

struct AstBadExpr;
struct AstBinaryExpr;
struct AstUnaryExpr;
struct AstCallExpr;
struct AstIfStmnt;
struct AstLoopStmnt;
struct AstBlock;
struct AstCtrlStmnt;
struct AstVarDef;
struct AstParamDef;
struct AstConstValue;

// ast nodes enum
enum class AstNodeId
{
  AST_SOURCE_CODE,
  AST_DIRECTIVE,
  AST_TYPE,
  AST_FN_DEF,
  AST_FN_PROTO,
  AST_IF_STMNT,
  AST_CTRL_STMNT,
  AST_LOOP_STMNT,
  AST_BLOCK,
  AST_PARAM_DEF,
  AST_VAR_DEF,
  AST_SYMBOL,
  AST_CONST_VALUE,
  AST_FN_CALL_EXPR,
  AST_BINARY_EXPR,
  AST_UNARY_EXPR
};

enum class DirectiveType
{
  LOAD,    // load file (first directive in the file)
  MAIN,    // set entry poi32 (1 per executable)
  RUN,     // run fn at compile-time
  COMPILE, // symbol is safe for use in compile-time
  FN_TYPE  // check function time at compile time
};

enum class SymbolType
{
  FUNC,
  VAR
};

enum class ConstValueType
{
  BOOL,
  INT,
  FLOAT,
  CHAR,
  PTR
};

enum class BinaryExprType
{
  ASSIGN,                // expr =  expr
  ADD,                   // expr +  expr
  SUB,                   // expr -  expr
  MUL,                   // expr *  expr
  DIV,                   // expr /  expr
  MOD,                   // expr %  expr
  CMP_EQUALS,            // expr == expr
  CMP_NOT_EQUALS,        // expr != expr
  CMP_GREATER_OR_EQUALS, // expr >= expr
  CMP_LESS_OR_EQUALS,    // expr <= expr
  CMP_GREATER,           // expr >  expr
  CMP_LESS,              // expr <  expr
  CMP_AND,               // expr && expr
  CMP_OR,                // expr || expr
  BIT_SHL,               // expr << expr
  BIT_SHR,               // expr >> expr
  BIT_XOR,               // expr ~  expr
  BIT_AND,               // expr &  expr
};

enum class UnaryExprType
{
  NEG,         // -   primaryExpr
  NOT,         // !   primaryExpr
  BIT_INV,     // ~   primaryExpr
  ADDRESS_OF,  // &   primaryExpr
  DEREFERENCE, // ^   primaryExpr
  RET          // ret Expr
};

enum CtrlStmntType
{
  BREAK,
  CONTINUE
};

enum class AstTypeId
{
  BASIC,
  PTR,
  ARRAY,
  ENUM,
  STRUCT,
  UNION,
  FN_TYPE,
};

const std::string get_unary_op_symbol(const UnaryExprType op_type) noexcept;
const std::string get_directive_type_name(const DirectiveType) noexcept;
const std::string_view get_type_id_name_lower_case(AstTypeId in_type_id) noexcept;
const std::string_view get_type_id_name(AstTypeId in_type_id) noexcept;
