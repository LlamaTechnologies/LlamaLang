#pragma once
#include <vector>
#include "ErrorDescription.hpp"
#include "antlr4-runtime.h"
#include <filesystem>

namespace llang::error_handling
{
    struct SyntaxErrorListener : antlr4::ANTLRErrorListener
    {
        
        
        std::vector<Error> Errors;
        SyntaxErrorListener() = default;

        void syntaxError(antlr4::Recognizer *recognizer,
                    antlr4::Token *offendingSymbol, size_t line,
                    size_t charPositionInLine, const std::string &msg,
                    std::exception_ptr e) override {
            std::string file = std::filesystem::path(recognizer->getInputStream()->getSourceName()).filename().string();
            auto error = Error(line, file, msg, charPositionInLine);
            Errors.push_back(error);
        }
    };
}