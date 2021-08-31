#pragma once
constexpr const char *ERROR_UNEXPECTED_EOF_AFTER = "unexpected end of file after '%s'";
constexpr const char *ERROR_EXPECTED_R_PAREN_AFTER = "expected ')' after '%s'";
constexpr const char *ERROR_EXPECTED_NUMBER_IDENTIFIER_CHAR_TOKEN =
  "expected number, identifier or char instead of '%s'";
constexpr const char *ERROR_EXPECTED_TYPE_EXPR_INSTEAD_OF =
  "expected type name, array type '[]' or pointer type '*' instead of '%s'";
constexpr const char *ERROR_EXPECTED_CLOSING_BRAKET_BEFORE = "expected clossing bracket ']' before '%s'";
constexpr const char *ERROR_EXPECTED_NEWLINE_OR_SEMICOLON_AFTER = "expected new line or semicolor ';' after '%s'";
constexpr const char *ERROR_UNEXPECTED_EXTERN = "extern only goes behind a 'fn' token, received token '%s'";
constexpr const char *ERROR_EXTERN_FN_HAS_BODY = "extern function declaration should not have a body.";
constexpr const char *ERROR_UNEXPECTED_HASH = "unexpected # before '%s'";
constexpr const char *ERROR_UNKNOWN_DIRECTIVE = "unknown directive '%s'";
