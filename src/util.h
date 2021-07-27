#ifndef __GCAP_UTIL_H__
#define __GCAP_UTIL_H__

#include <syslog.h>

/**
 * Log debug message.
 * @param format message
 */
void log_debug(const char *format, ...);

/**
 * Log error message.
 * @param format message
 */
void log_error(const char *format, ...);

#endif
