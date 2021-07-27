#include "util.h"
#include <stdio.h>

void log_debug(const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("[DEBUG] ");
    vprintf(format, args);
    va_end(args);
    return;
}

void log_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("[ERROR] ");
    vprintf(format, args);
    va_end(args);
    return;
}
