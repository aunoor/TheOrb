#include "commondefs.h"
#include <cstdarg>
#include <cstring>

std::string asprintf(const char *format, ...) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    va_list args;
    va_start (args, format);
    vsnprintf (buffer, sizeof(buffer), format, args);
    va_end (args);
    return std::string(buffer);
}
