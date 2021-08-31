#include "Types.hpp"

struct TypeIdAstTypeId {
  AstTypeId id;
  TypeInfo info;
};

std::unordered_map<std::string_view, const AstType> typesIds = {
  { "i8", { AstTypeId::Integer, "i8", 8, true } },
  { "i16", { AstTypeId::Integer, "i16", 16, true } },
  { "i32", { AstTypeId::Integer, "i32", 32, true } },
  { "i64", { AstTypeId::Integer, "i64", 64, true } },
  { "i128", { AstTypeId::Integer, "i128", 128, true } },
  { "u8", { AstTypeId::Integer, "u8", 8, false } },
  { "u16", { AstTypeId::Integer, "u16", 16, false } },
  { "u32", { AstTypeId::Integer, "u32", 32, false } },
  { "u64", { AstTypeId::Integer, "u64", 64, false } },
  { "u128", { AstTypeId::Integer, "u128", 128, false } },
  { "f32", { AstTypeId::FloatingPoint, "f32", 32, true } },
  { "f64", { AstTypeId::FloatingPoint, "f64", 64, true } },
  { "f128", { AstTypeId::FloatingPoint, "f128", 128, true } },
  { "void", { AstTypeId::Void, "void", 0, false } },
  { "bool", { AstTypeId::Bool, "bool", 1, false } }
};

const AstTypeId get_type_id(std::string_view in_name) noexcept {
  if (typesIds.find(in_name) == typesIds.end()) {
    return AstTypeId::UNKNOWN;
  }
  return typesIds[in_name].type_id;
}

AstNode *get_type_node(std::string_view in_name) noexcept {
  if (typesIds.find(in_name) == typesIds.end()) {
    return nullptr;
  }
  return AstNode::CreatePredefType(typesIds[in_name]);
}
