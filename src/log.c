/**
 * File: log.c
 * Author: ZhuXindi
 * Date: 2013-07-11
 */

#include <cbase/log.h>
#include <cbase/systime.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define LOG_BUF_SIZE 1024

static int log_file = STDERR_FILENO;
static int log_level = LOG_WARN;
static const char *level_names[] = {
	[LOG_DEBUG]	= "DEBUG",
	[LOG_INFO]	= "INFO",
	[LOG_WARN]	= "WARN",
	[LOG_ERROR]	= "ERROR",
	[LOG_FATAL]	= "FATAL",
};

int set_log_level(int min_level)
{
	int ret = log_level;

	log_level = min_level;
	if (log_level < LOG_DEBUG)
		log_level = LOG_DEBUG;
	else if (log_level > LOG_FATAL)
		log_level = LOG_FATAL;
	return ret;
}

int set_log_file(int fd)
{
	int ret = log_file;

	log_file = fd < 0 ? STDERR_FILENO : fd;
	return ret;
}

static void __write_log(const char *file, int line, int level,
			const char *fmt, va_list va)
{
	char buf[LOG_BUF_SIZE], *p = buf, *end = buf + LOG_BUF_SIZE - 1;
	int n;

	n = snprintf(p, end - p, "%s #%d %s:%d [%s] ",
		     str_time, (int)pid, file, line, level_names[level]);
	if (n < 0 || n >= end - p)
		return;
	p += n;

	n = vsnprintf(p, end - p, fmt, va);
	if (n < 0)
		return;
	if (n >= end - p) {
		memcpy(end - 12, " (truncated)", 12);
		n = end - p;
	}
	p += n;

	*p++ = '\n';
	(void)write(log_file, buf, p - buf);
}

void write_log(const char *file, int line, int level, const char *fmt, ...)
{
	if (level >= log_level) {
		va_list va;
		va_start(va, fmt);
		__write_log(file, line, level, fmt, va);
		va_end(va);
	}

	if (level == LOG_FATAL)
		exit(1);
}
