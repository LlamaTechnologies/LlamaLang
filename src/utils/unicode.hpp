#pragma once

#include "../common_defs.hpp"

constexpr s32 UTF_BOM = 0xef | 0xbb | 0xbf;
constexpr s32 UTF_MAX = 4;
constexpr s32 UTF_SELF = 0x80;
constexpr s32 UTF_EOF = ~u64(0);
constexpr s32 MAX_RUNE = '\U00010fff';     // Maximum valid unicode code point
constexpr s32 MAX_ASCII = '\u007f';        // Maximum ASCII value
constexpr s32 MAX_LATIN1 = '\u00ff';       // Maximum Latin-1 value
constexpr s32 UTF_INVALID_RUNE = '\ufffd'; // Represented an invalid code point
constexpr s32 UTF_ERROR = '\ufffd';

u64 utf8_decode(u8 const *str, u64 str_len, rune *codepoint_out);
