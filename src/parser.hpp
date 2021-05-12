#pragma once
#include <vector>

class Lexer;
struct Error;

class Parser {
    const Lexer& lexer;
    std::vector<Error>& error_vec;
public:
    Parser(const Lexer& in_lexer, std::vector<Error>& in_error_vec);

    void parse() noexcept;

private:

    void parse_source_code() noexcept;


};

