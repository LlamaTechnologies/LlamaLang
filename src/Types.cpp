#include "Types.hpp"

struct TypeIdAstTypeId {
    AstTypeId id;
    TypeInfo info;
};

std::unordered_map<std::string_view, TypeIdAstTypeId> typesIds = {
    {"i8",   {AstTypeId::Integer,       {"i8"  , nullptr, 8  , true}}},
    {"i16",  {AstTypeId::Integer,       {"i16" , nullptr, 16 , true}}},
    {"i32",  {AstTypeId::Integer,       {"i32" , nullptr, 32 , true}}},
    {"i64",  {AstTypeId::Integer,       {"i64" , nullptr, 64 , true}}},
    {"i128", {AstTypeId::Integer,       {"i128", nullptr, 128, true}}},
    {"u8"  , {AstTypeId::Integer,       {"u8"  , nullptr, 8  , false}}},
    {"u16" , {AstTypeId::Integer,       {"u16" , nullptr, 16 , false}}},
    {"u32" , {AstTypeId::Integer,       {"u32" , nullptr, 32 , false}}},
    {"u64" , {AstTypeId::Integer,       {"u64" , nullptr, 64 , false}}},
    {"u128", {AstTypeId::Integer,       {"u128", nullptr, 128, false}}},
    {"f32" , {AstTypeId::FloatingPoint, {"f32" , nullptr, 32 , true}}},
    {"f64" , {AstTypeId::FloatingPoint, {"f64" , nullptr, 64 , true}}},
    {"f128", {AstTypeId::FloatingPoint, {"f128", nullptr, 128, true}}},
    {"void", {AstTypeId::Void,          {"void", nullptr, 0  , false}}},
    {"bool", {AstTypeId::Bool,          {"bool", nullptr, 1  , false}}}
};

AstTypeId get_type_id(std::string_view in_name, TypeInfo** info) noexcept {
    if (typesIds.find(in_name) == typesIds.end()) {
        *info = new TypeInfo();
        (*info)->name = in_name;
        (*info)->llvm_type = nullptr;
        (*info)->bit_size = 8;
        (*info)->is_signed = false;
        return AstTypeId::Struct;
    }
    auto type_info = typesIds[in_name];
    *info = new TypeInfo(type_info.info);
    return type_info.id;
}

AstNode* get_type_node(std::string_view in_name) noexcept {
    auto type_node = new AstNode(AstNodeType::AstType, 0, 0, "internal");
    type_node->ast_type.type_id = get_type_id(in_name, &type_node->ast_type.type_info);
    type_node->ast_type.child_type = nullptr;
    return type_node;
}

