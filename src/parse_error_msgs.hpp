#pragma once
constexpr const char *ERROR_UNEXPECTED_EOF_AFTER = "unexpected end of file after '%s'";
constexpr const char *ERROR_EXPECTED_R_PAREN_AFTER = "expected ')' after '%s'";
constexpr const char *ERROR_EXPECTED_NUMBER_IDENTIFIER_CHAR_TOKEN =
  "expected number, identifier or char instead of '%s'";
constexpr const char *ERROR_EXPECTED_TYPE_EXPR_INSTEAD_OF = "expected type name or pointer type '*' instead of '%s'";
constexpr const char *ERROR_EXPECTED_CLOSING_BRAKET_BEFORE = "expected clossing bracket ']' before '%s'";
constexpr const char *ERROR_EXPECTED_NEWLINE_OR_SEMICOLON_AFTER = "expected new line or semicolor ';' after '%s'";
constexpr const char *ERROR_EXPECTED_OPENING_CURLY_AFTER = "expected '{' after ']'";
constexpr const char *ERROR_EXPECTED_COLON_LOOP = "expected ':' after loop initializer";
constexpr const char *ERROR_EXPECTED_ELEMENTS_IN_CONST_ARRAY =
  "expected elements in the array but got an empty array. Empty arrays are not admited in this language. Use nil ptr instead.";
constexpr const char *ERROR_EXPECTED_ARRAY_SIZE_BUT_GOT = "expected array of size %llu but got %llu elements";
constexpr const char *ERROR_EXPECTED_NO_ELEMENTS_AFTER_UNDEF = "expected no element after '---'";
constexpr const char *ERROR_EXTERN_FN_HAS_BODY = "extern function declaration should not have a body.";
constexpr const char *ERROR_UNEXPECTED_EXTERN = "extern only goes behind a 'fn' token, received token '%s'";
constexpr const char *ERROR_UNEXPECTED_HASH = "unexpected # before '%s'";
constexpr const char *ERROR_UNEXPECTED_UNDEF_AFTER = "unexpected '---'(UNDEF) after '%s'";
constexpr const char *ERROR_UNEXPECTED_AFTER_UNDEF = "unexpected '%s' after '---'(UNDEF)";
constexpr const char *ERROR_FORBIDEN_OP_AFTER_UNDEF = "forbiden operator '%s' after operand '---'(UNDEF)";
constexpr const char *ERROR_FORBIDEN_UNDEF_AFTER_OP = "forbiden operand '---'(UNDEF) after operator '%s'";
constexpr const char *ERROR_UNKNOWN_DIRECTIVE = "unknown directive '%s'";
constexpr const char *ERROR_DIRECTIVE_EXPECTED_STRING = "expected string as directive argument, got '%s'";
constexpr const char *ERROR_NOT_A_FILE = "path '%s' is a directory";
constexpr const char *ERROR_FILE_NOT_FOUND = "file '%s' was not found";
constexpr const char *ERROR_NOT_LLAMA_FILE = "file '%s' is not a llama file (.llama)";
constexpr const char *ERROR_BRANCH_EXPR_NOT_BOOL = "expression in branch statement should be of type boolean";
constexpr const char *ERROR_BRANCH_EXPECTED_RPAREN =
  "expected ')' at the end of the branch expression but got '%s' instead";
constexpr const char *WARN_EMPTY_BRANCH = "empty branch";
constexpr const char *ERROR_UNNAMED_PARAM = "unnamed parameter of type '%s'";
constexpr const char *ERROR_IMPLICIT_UNDEFINED_VAR = "implicit undefined variable";
