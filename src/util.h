#ifndef __GCAP_UTIL_H__
#define __GCAP_UTIL_H__

#include <syslog.h>

/**
 * Log debug message.
 * @param format message
 */
void LogDebug(const char *format, ...);

/**
 * Log error message.
 * @param format message
 */
void LogError(const char *format, ...);

#endif
