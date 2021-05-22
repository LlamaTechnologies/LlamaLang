#pragma once

[[noreturn]]
void panic(const char* format, ...);

#ifdef _WIN32
#define __func__ __FUNCTION__
#endif

#define UNREACHEABLE panic("Unreachable at %s:%d in %s. This is a bug in the compiler.", __FILE__, __LINE__, __func__)


