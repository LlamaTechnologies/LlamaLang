#pragma once

// define platforms

#ifdef _WIN32
  #define LL_WIN32
#elif defined(__unix__)
  #ifdef __linux__
    #define LL_LINUX
  #else
    #define LL_MACOS
  #endif
#endif

// define compillers
#ifdef __GNUG__
  #define LL_GCC
#elif defined(_MSC_VER)
  #define LL_VISUALSTUDIO
#elif defined(__clang__)
  #define LL_CLANG
#endif

#ifdef LL_VISUALSTUDIO
  #define __func__ __FUNCTION__
#endif

#ifdef _DEBUG
  #define LL_DEBUG
#endif

#define LL_NODISCARD [[nodiscard]]
#define LL_FALLTHROUGH [[fallthrough]];

// define primitives
typedef unsigned char u8;
typedef signed char s8;

typedef unsigned short u16;
typedef signed short s16;

typedef unsigned long u32;
typedef signed long s32;

typedef unsigned long long u64;
typedef signed long long s64;

typedef float f32;
typedef double f64;

[[noreturn]] void panic(const char *format, ...);

#define LL_UNREACHEABLE \
  panic("Unreachable at %s:%d in %s :: This is a bug in the compiler.", __FILE__, __LINE__, __func__)

#define LL_SUNREACHEABLE(msg) panic("Unreachable at %s:%d in %s :: %s", __FILE__, __LINE__, __func__, msg)

#ifdef LL_DEBUG
  #define LL_ASSERT(condition) static_cast<bool>(condition) ? void(0) : LL_UNREACHEABLE
  #define LL_SASSERT(condition, msg) static_cast<bool>(condition) ? void(0) : LL_UNREACHEABLE(msg)
#else
  #define LL_ASSERT(condition)
  #define LL_SASSERT(condition, msg)
#endif

#define LL_LIKELY(condition) __builtin_expect(!!(x), 1)
