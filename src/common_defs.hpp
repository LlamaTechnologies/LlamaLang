#pragma once

// define compillers
#ifdef __GNUG__
#define LL_GCC
#elif defined(_MSC_VER )
#define LL_VISUALSTUDIO
#elif defined(__clang__)
#define LL_CLANG
#endif

#ifdef LL_VISUALSTUDIO
#define __func__ __FUNCTION__
#ifdef _DEBUG
#define LL_DEBUG
#undef _DEBUG
#endif
#endif

#define LL_NODISCARD    [[nodiscard]]
#define LL_FALLTHROUGH  [[fallthrough]];

[[noreturn]]
void panic(const char* format, ...);

#define UNREACHEABLE panic("Unreachable at %s:%d in %s. This is a bug in the compiler.", __FILE__, __LINE__, __func__)

