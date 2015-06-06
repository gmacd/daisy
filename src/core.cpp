#include "core.h"

#include <cstdarg>
#include <cstdio>

namespace daisy {

void Log(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    printf(fmt, args);

    va_end(args);
}

}
