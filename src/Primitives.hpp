#pragma once
#include <string>

namespace llang {
    /**
    * This enum contains all prmitives names
    **/
    enum class PRIMITIVE_TYPE
    {
        /* SPECIALS */
        VOID,
        BOOL,

        /* SIGNED */
        INT8,
        INT16,
        INT32,
        INT64,
        
        /* CHARACTERS */
        SCHAR,  // ASCII
        CHAR,   // UTF-8
        WCHAR,  // UTF-16
        UCHAR,  // UNICODE

        /* UNSIGNED */
        BYTE,   // Alias for uint8
        UINT8,
        UINT16,
        UINT32,
        UINT64,
        
        /* FLOATING POINT */
        FLOAT32,
        FLOAT64       
    };

    /**
     * This is a holder for all primitives types.
     * Useful for checking if the type is primitive.
     **/
    namespace Primitives
    {
        bool Exists(const std::string &name);
        
        PRIMITIVE_TYPE Get(const std::string &name);
    };
}