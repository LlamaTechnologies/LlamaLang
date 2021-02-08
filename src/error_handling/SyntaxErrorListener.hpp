#pragma once
#include <vector>
#include "ErrorDescription.hpp"
#include "antlr4-runtime.h"

namespace llang::error_handling
{
    struct SyntaxErrorListener : antlr::IAntlrErrorListener<IToken>
    {
        using namespace antlr4;
        
        std::vector<Error> Errors;
        SyntaxErrorListener() = default;

        void SyntaxError(TextWriter output, IRecognizer recognizer, IToken offendingSymbol, int line, int charPositionInLine, string msg, RecognitionException e)
        {
            std::string file = Path.GetFileName(recognizer.InputStream.SourceName);
            auto error = Error(line, file, msg, charPositionInLine);
            Errors.push_back(error);
        }
    };
}