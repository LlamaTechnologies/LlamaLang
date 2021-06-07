#pragma once

#define LL_NODISCARD    [[nodiscard]]
#define LL_FALLTHROUGH  [[fallthrough]];

[[noreturn]]
void panic(const char* format, ...);


#if _WIN32 == 1
#define __func__ __FUNCTION__
#define LL_NODISCARD _NODISCARD
#elif __linux == 1

#endif

#define UNREACHEABLE panic("Unreachable at %s:%d in %s. This is a bug in the compiler.", __FILE__, __LINE__, __func__)
