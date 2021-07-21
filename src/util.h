#ifndef __GCAP_UTIL_H__
#define __GCAP_UTIL_H__

#include <syslog.h>

/**
 * Log debug message.
 * @param text message
 */
void log_debug(char *text);

/**
 * Log error message.
 * @param text message
 */
void log_error(char *text);

#endif
