#pragma once
#include <vector>
#include "ErrorDescription.hpp"
#include "antlr4-runtime.h"
#include <filesystem>

namespace llang::error_handling
{
    struct SyntaxErrorListener : antlr4::ANTLRErrorListener
    {
    private:
        const std::string _fileName;
    public:
        std::vector<Error> Errors;
        
        SyntaxErrorListener(const std::string &fileName) : antlr4::ANTLRErrorListener(), _fileName(fileName) {}

        void syntaxError(antlr4::Recognizer *recognizer,
                    antlr4::Token *offendingSymbol, size_t line,
                    size_t charPositionInLine, const std::string &msg,
                    std::exception_ptr e) override {
            auto error = Error(line, _fileName, msg, charPositionInLine);
            Errors.push_back(error);
        }

        void reportAmbiguity(antlr4::Parser *recognizer,
                             const antlr4::dfa::DFA &dfa,
                             size_t startIndex, size_t stopIndex, bool exact,
                             const antlrcpp::BitSet &ambigAlts,
                             antlr4::atn::ATNConfigSet *configs) override {

        }
        void reportAttemptingFullContext(antlr4::Parser *recognizer,
                                        const antlr4::dfa::DFA &dfa,
                                        size_t startIndex, size_t stopIndex,
                                        const antlrcpp::BitSet &conflictingAlts,
                                        antlr4::atn::ATNConfigSet *configs) override {

        }

        void reportContextSensitivity(antlr4::Parser *recognizer,
                                      const antlr4::dfa::DFA &dfa,
                                      size_t startIndex, size_t stopIndex,
                                      size_t prediction,
                                      antlr4::atn::ATNConfigSet *configs) override {

        }
    };
}