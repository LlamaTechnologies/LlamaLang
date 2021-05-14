#include "common_defs.h"
#include <cstdarg>
#include <cstdlib>
#include <iostream>

[[noreturn]]
void panic(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    fflush(stderr);
    va_end(ap);
    abort();
}
