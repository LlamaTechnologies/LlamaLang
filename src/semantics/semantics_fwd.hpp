#pragma once
#include "../common_defs.hpp"

#include <string>
#include <vector>

struct AstType;
struct AstConstant;
struct AstVariable;

enum class BasicTypeId
{
  BOOL,

  S8,
  S16,
  S32,
  S64,
  S128,

  U8,
  U16,
  U32,
  U64,
  U128,

  RUNE,

  F16,
  F32,
  F64,

  COMPLEX32,  // 2 * F16
  COMPLEX64,  // 2 * F32
  COMPLEX128, // 2 * F64

  QUATERNION64,  // 4 * F16
  QUATERNION128, // 4 * F32
  QUATERNION256, // 4 * F64

  TYPE_ID, // enum of type kind
  RAW_PTR,
  STRING,  // ^u8 + S64
  CSTRING, // ^u8
  ANY,     // rawptr + TypeId

  // Untyped types
  UNTYPED_BOOL,
  UNTYPED_INTEGER,
  UNTYPED_FLOAT,
  UNTYPED_COMPLEX,
  UNTYPED_QUATERNION,
  UNTYPED_STRING,
  UNTYPED_RUNE,
  UNTYPED_NIL,
  UNTYPED_UNDEF,

  COUNT,

  BYTE = U8,
  CVOID = RAW_PTR,
};

struct BasicType {
  BasicTypeId id;
  std::string name;
};

enum FnCallingConvention
{
  C_DECL, // C windows
  C_FAST, // C windows fast
  C_STD,  // C standard
  INLINE_ASM,
};

enum FnFlags
{
  EXTERN,
  VARIADIC,
  C_VARARGS,
  NO_RETURN,
};

struct FnType {
  std::vector<AstType *> params;
  std::vector<AstType *> results;
  FnCallingConvention calling_convention;
  s32 variadic_index;
  FnFlags flags;
};

struct Pointer {
  AstType *type;
};

struct Array {
  AstType *type;
  u64 count;
};

struct Enum {
  std::vector<AstConstant *> fields;
  AstType *base_type;
  Token name;
  AstConstant *min_value;
  AstConstant *max_value;
};

enum StructFlags
{ IS_UNAMED, };

struct Struct {
  std::vector<AstVariable *> fields;
  Token name_or_struct;
  StructFlags flags;
};

enum UnionFlags
{ IS_UNAMED, };

struct Union {
  std::vector<AstType *> variants;
  Token name_or_union;
  u64 block_size;
  UnionFlags flags;
};
