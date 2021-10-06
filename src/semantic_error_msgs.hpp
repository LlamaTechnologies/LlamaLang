#pragma once
constexpr const char *ERROR_STRUCTS_UNSUPORTED = "structs are not supported yet!";
constexpr const char *ERROR_TYPES_MISMATCH = "types mismatch";
constexpr const char *ERROR_TYPES_SIZE_MISMATCH = "types size mismatch";
constexpr const char *ERROR_INTEGERS_SIGN_MISMATCH = "integers sign mismatch";
constexpr const char *ERROR_GLOBAL_NEED_INITIALIZER = "global variable has to be initialized";
constexpr const char *ERROR_GLOBAL_INITIALIZER_NO_CONST =
  "global variables allow only constants expression as init value";
constexpr const char *ERROR_UNKNOWN_SYMBOL = "unknown symbol '%s'";
constexpr const char *ERROR_BIT_SHIFT_LEFT_EXPR_NO_INT = "bit shift lext expression should be of type integer";
constexpr const char *ERROR_UNSUPORTED_OP_BOOL_EXPR = "unsuported opearator %s on boolean expr";
constexpr const char *ERROR_UNSUPORTED_OP_NOT_BOOL_EXPR = "unsuported opearator ! on not boolean expr";
constexpr const char *ERROR_REQUIRE_RET_STMNT = "function requires a ret statement";
constexpr const char *ERROR_UNDECLARED_FN = "undeclared function '%s'";
constexpr const char *ERROR_ARGUMENT_COUNT_MISMATCH = "function takes %zu and %zu arguments are passed";
constexpr const char *ERROR_SYMBOL_NOT_A_FN = "symbol '%s' is not a function";
constexpr const char *ERROR_UNSUPORTED_UNARY_OP_STRUCT_EXPR = "unsuported operator %s on expresion of type struct";
constexpr const char *ERROR_UNSUPPORTED_PARAM_INITIALIZER = "unsupported: param default initializer";
constexpr const char *ERROR_EXPECTED_BOOL_EXPR = "expected boolean expression but got expression of type '%s'";
constexpr const char *ERROR_CTRL_STMNT_INVALID_LOCATION =
  "invalid location '%s' for control statement (break/continue)";
constexpr const char *ERROR_RET_STMNT_INVALID_LOCATION = "invalid location '%s' for return statement";
