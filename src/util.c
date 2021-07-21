#include "util.h"
#include <stdio.h>

void log_debug(char *text) {
  printf("[DEBUG] %s\n", text);
  return;
}

void log_error(char *text) {
  printf("[ERROR] %s\n", text);
  return;
}
