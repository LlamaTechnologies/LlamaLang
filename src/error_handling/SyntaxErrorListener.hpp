#pragma once
#include <vector>
#include "ErrorDescription.hpp"

namespace llang::error_handling
{
    struct SyntaxErrorListener : IAntlrErrorListener<IToken>
    {
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