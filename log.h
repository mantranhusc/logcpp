/**
 * Copyright (c) 2020 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 */

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

#define LOG_VERSION "0.1.0"

typedef struct {
  va_list ap;
  const char *fmt;
  const char *file;
  struct tm *time;
  void *udata;
  int line;
  int level;
} log_Event;

typedef void (*log_LogFn)(log_Event *ev);
typedef void (*log_LockFn)(bool lock, void *udata);

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

#define log_trace(...) log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

const char* log_level_string(int level);
void log_set_lock(log_LockFn fn, void *udata);
void log_set_level(int level);
void log_set_quiet(bool enable);
int log_add_callback(log_LogFn fn, void *udata, int level);
int log_add_fp(FILE *fp, int level);

void log_log(int level, const char *file, int line, const char *fmt, ...);

#endif



/**
 * @file log.h
 * Logging utility.
 * To print to stdout, just use the macro LOG_INFO or LOG_DEBUG.
 * INFO or DEBUG tag, file name, line number and current function
 * will be always printed.
 * <br>
 * To activate logging pass -DLOGDEBUG or/and -DLOGINFO to the compiler.
 * Alternatively #define LOGDEBUG or/and LOGINFO.
 * <br>
 * Usage:
 * LOG_INFO("This is %d logging statement.", 1);
 * <br>
 * Output:
 * [INFO] file.c:205: sample_func(): This is 1 logging statement.
 * <br>
 * To print to a file, set the file using:
 * set_logging_file(fname);
 */

#ifndef LOG_H_
#define LOG_H_

#define _XOPEN_SOURCE
#define _ISOC99_SOURCE
#define _DEFAULT_SOURCE

#if (__STDC_VERSION__ < 199901L)
#error "C99 Compiler for logging needed."
#endif

#include <stdarg.h>
#include <stdio.h>

#define PR_BUFFER_SIZE 100

char pr_buffer[PR_BUFFER_SIZE];
FILE** log_file = &stdout;

/**
 * Sets the logging file.
 * The file will be created if it not exists.
 * If the file exist, it will be erased.
 * @param fname Filename.
 */
void set_logging_file(char* fname) {
    FILE *f = fopen(fname, "w");
    if (f == NULL)
        return;
    log_file = &f;
}

void dlog(const char* tag, const char* fname, int linnum, const char* fxname,
          char* fmt, ...) {
    va_list va;
    va_start(va, fmt);
    vsnprintf(pr_buffer, PR_BUFFER_SIZE, fmt, va);
    va_end(va);
    fprintf(*log_file, "[%s] %s:%d %s(): %s\n", tag, fname, linnum, fxname, pr_buffer);
}

#ifdef LOGINFO
#define LOG_INFO(fmt, ...)                                              \
    dlog("INFO", __FILE__, __LINE__, __func__, (fmt), ##__VA_ARGS__);
#else
#define LOG_INFO(fmt, ...) do {} while (0)
#endif

#ifdef LOGDEBUG
#define LOG_DEBUG(fmt, ...)                                             \
    dlog("DEBUG", __FILE__, __LINE__, __func__, (fmt), ##__VA_ARGS__);
#else
#define LOG_DEBUG(fmt, ...) do {} while (0)
#endif

#endif /* LOG_H_ */
