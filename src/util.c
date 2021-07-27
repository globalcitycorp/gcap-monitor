#include "util.h"
#include <stdio.h>

void LogDebug(const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("[DEBUG] ");
    vprintf(format, args);
    va_end(args);
    return;
}

void LogDebug(const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("[ERROR] ");
    vprintf(format, args);
    va_end(args);
    return;
}
