#pragma once
#include "antlr4-runtime.h"
#include "LlamaLangLexer.h"

namespace llang {
    class LlamaLangParserBase : public antlr4::Parser {
    public:
        explicit LlamaLangParserBase(antlr4::TokenStream* input) : Parser(input) {}
        virtual ~LlamaLangParserBase() = default; 
        /**
        * Returns {@code true} iff on the current index of the parser's
        * token stream a token exists on the {@code HIDDEN} channel which
        * either is a line terminator, or is a multi line comment that
        * contains a line terminator.
        *
        * @return {@code true} iff on the current index of the parser's
        * token stream a token exists on the {@code HIDDEN} channel which
        * either is a line terminator, or is a multi line comment that
        * contains a line terminator.
        */
        bool lineTerminatorAhead() {
            // Get the token ahead of the current index.
            auto possibleIndexEosToken = this->getCurrentToken()->getTokenIndex() - 1;

            if (possibleIndexEosToken == -1)
                return true;

            auto ahead = _input->get(possibleIndexEosToken);
            if (ahead->getChannel() != antlr4::Lexer::HIDDEN) {
                // We're only interested in tokens on the HIDDEN channel.
                return false;
            }

            if (ahead->getType() == LlamaLangLexer::TERMINATOR) {
                // There is definitely a line terminator ahead.
                return true;
            }

            if (ahead->getType() == LlamaLangLexer::WS) {
                // Get the token ahead of the current whitespaces.
                possibleIndexEosToken = this->getCurrentToken()->getTokenIndex() - 2;
                ahead = _input->get(possibleIndexEosToken);
            }

            // Get the token's text and type.
            auto text = ahead->getText();
            auto type = ahead->getType();

            // Check if the token is, or contains a line terminator.
            return (
                    type == LlamaLangLexer::COMMENT 
                    && (text.find("\r") != std::string::npos || text.find("\n") != std::string::npos)
                )
                || (type == LlamaLangLexer::TERMINATOR);
        }

        /**
        * Returns {@code true} if no line terminator exists after any encounterd
        * parameters beyond the specified token offset and the next on the
        * {@code HIDDEN} channel.
        *
        * @return {@code true} if no line terminator exists after any encounterd
        * parameters beyond the specified token offset and the next on the
        * {@code HIDDEN} channel.
        */
        bool noTerminatorAfterParams(int tokenOffset) {
            auto stream = (antlr4::BufferedTokenStream*)_input;
            int leftParams = 1;
            int rightParams = 0;
            size_t valueType;

            if (stream->LT(tokenOffset)->getType() == LlamaLangLexer::L_PAREN) {
                // Scan past parameters
                while (leftParams != rightParams) {
                    tokenOffset++;
                    valueType = stream->LT(tokenOffset)->getType();

                    if (valueType == LlamaLangLexer::L_PAREN) {
                        leftParams++;
                    }
                    else if (valueType == LlamaLangLexer::R_PAREN) {
                        rightParams++;
                    }
                }

                tokenOffset++;
                return noTerminatorBetween(tokenOffset);
            }

            return true;
        }


        /**
         * Returns {@code true} if no line terminator exists between the specified
         * token offset and the prior one on the {@code HIDDEN} channel.
         *
         * @return {@code true} if no line terminator exists between the specified
         * token offset and the prior one on the {@code HIDDEN} channel.
         */
        bool noTerminatorBetween(int tokenOffset) {
            auto stream = (antlr4::BufferedTokenStream*)_input;
            auto tokens = stream->getHiddenTokensToLeft(stream->LT(tokenOffset)->getTokenIndex());

            if (tokens.empty()) {
                return true;
            }

            for (auto& token : tokens) {
                if (token->getText().find("\n") != std::string::npos)
                    return false;
            }

            return true;
        }

        bool checkPreviousTokenText(std::string text)
        {
            auto stream = (antlr4::BufferedTokenStream*)_input;
            auto prevTokenText = stream->LT(1)->getText();

            if (prevTokenText == "")
                return false;

            return prevTokenText == text;
        }
    };
}