#pragma once

#ifdef __GNUG__
#define LLANG_GCC
#elif defined(_MSC_VER )
#define LLANG_VISUALSTUDIO
#elif defined(__clang__)
#define LLANG_CLANG
#endif
