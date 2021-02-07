#pragma once
#include <string>
#include <unordered_map>

namespace llang
{
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
        static bool Exists(std::string &name);
        
        static PRIMITIVE_TYPE Get(std::string &name);
    };
}