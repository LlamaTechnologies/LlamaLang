#include "Types.hpp"

static TypesRepository *repository = nullptr;

TypesRepository &TypesRepository::get() {
  if (!repository) {
    repository = new TypesRepository();
    repository->types_id = { { "i8", { AstTypeId::INTEGER, "i8", 8, true } },
                             { "i16", { AstTypeId::INTEGER, "i16", 16, true } },
                             { "i32", { AstTypeId::INTEGER, "i32", 32, true } },
                             { "i64", { AstTypeId::INTEGER, "i64", 64, true } },
                             { "i128", { AstTypeId::INTEGER, "i128", 128, true } },
                             { "u8", { AstTypeId::INTEGER, "u8", 8, false } },
                             { "u16", { AstTypeId::INTEGER, "u16", 16, false } },
                             { "u32", { AstTypeId::INTEGER, "u32", 32, false } },
                             { "u64", { AstTypeId::INTEGER, "u64", 64, false } },
                             { "u128", { AstTypeId::INTEGER, "u128", 128, false } },
                             { "f32", { AstTypeId::FLOATING_POINT, "f32", 32, true } },
                             { "f64", { AstTypeId::FLOATING_POINT, "f64", 64, true } },
                             { "f128", { AstTypeId::FLOATING_POINT, "f128", 128, true } },
                             { "void", { AstTypeId::VOID, "void", 0, false } },
                             { "bool", { AstTypeId::BOOL, "bool", 1, false } } };
  }

  return *repository;
}

const AstTypeId TypesRepository::get_type_id(std::string_view in_name) noexcept {
  if (types_id.find(in_name) == types_id.end()) {
    return AstTypeId::UNKNOWN;
  }
  return types_id[in_name].type_id;
}

AstNode *TypesRepository::get_type_node(std::string_view in_name) noexcept {
  if (types_id.find(in_name) == types_id.end()) {
    return nullptr;
  }
  return AstNode::CreatePredefType(types_id[in_name]);
}

bool TypesRepository::register_type(std::string_view in_name) noexcept { return true; }
