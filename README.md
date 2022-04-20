# logcpp
c-log
Simple logging library for C. Just include the header and you are ready to start. It is possible to log to stdout or to a file. Library is not thread-safe.

Usage
Log to stdout:

#include "log.h"

int main() {
  LOG_INFO("This is %d logging statement.", 1);
  return 0;
}

Log to file:

#include "log.h"

int main() {
  set_logging_file("file.log");
  LOG_INFO("This is %d logging statement.", 1);
  return 0;
}
Compile with gcc -DLOGINFO sample.c
