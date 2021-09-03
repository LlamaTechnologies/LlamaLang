#include "Types.hpp"

static TypesRepository *repository = nullptr;

TypesRepository &TypesRepository::get() {
  if (!repository) {
    repository = new TypesRepository();
    repository->types_id = { { "i8", TypeInfo(AstTypeId::INTEGER, "i8", 8, true) },
                             { "i16", TypeInfo(AstTypeId::INTEGER, "i16", 16, true) },
                             { "i32", TypeInfo(AstTypeId::INTEGER, "i32", 32, true) },
                             { "i64", TypeInfo(AstTypeId::INTEGER, "i64", 64, true) },
                             { "i128", TypeInfo(AstTypeId::INTEGER, "i128", 128, true) },
                             { "u8", TypeInfo(AstTypeId::INTEGER, "u8", 8, false) },
                             { "u16", TypeInfo(AstTypeId::INTEGER, "u16", 16, false) },
                             { "u32", TypeInfo(AstTypeId::INTEGER, "u32", 32, false) },
                             { "u64", TypeInfo(AstTypeId::INTEGER, "u64", 64, false) },
                             { "u128", TypeInfo(AstTypeId::INTEGER, "u128", 128, false) },
                             { "f32", TypeInfo(AstTypeId::FLOATING_POINT, "f32", 32, true) },
                             { "f64", TypeInfo(AstTypeId::FLOATING_POINT, "f64", 64, true) },
                             { "f128", TypeInfo(AstTypeId::FLOATING_POINT, "f128", 128, true) },
                             { "void", TypeInfo(AstTypeId::VOID, "void", 0, false) },
                             { "bool", TypeInfo(AstTypeId::BOOL, "bool", 1, false) },
                             { "array", TypeInfo(AstTypeId::ARRAY, "array", 64, false) },
                             { "pointer", TypeInfo(AstTypeId::POINTER, "pointer", 64, false) } };
  }

  return *repository;
}

const AstTypeId TypesRepository::get_type_id(std::string_view in_name) noexcept {
  if (types_id.find(in_name) == types_id.end()) {
    return AstTypeId::UNKNOWN;
  }
  return types_id[in_name].type_id;
}

AstType *TypesRepository::get_type_node(std::string_view in_name) noexcept {
  if (types_id.find(in_name) == types_id.end()) {
    return nullptr;
  }
  return new AstType(&types_id[in_name]);
}

const TypeInfo *TypesRepository::get_type(std::string_view in_name) noexcept {
  // FIX(pablo96): if the map bucket grows the pointers become invalid!!
  if (types_id.find(in_name) == types_id.end()) {
    return nullptr;
  }
  return &types_id[in_name];
}

bool TypesRepository::register_type(std::string_view in_name) noexcept { return true; }
