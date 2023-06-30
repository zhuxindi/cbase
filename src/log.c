/**
 * File: log.c
 * Author: ZhuXindi
 * Date: 2013-07-11
 */

#include <log.h>
#include <systime.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static FILE *log_file;
static int log_level = LOG_WARN;
static struct {
	const char *name;
	const char *color[2];
} levels[] = {
	[LOG_DEBUG]	= {"DEBUG", {"", ""}},
	[LOG_INFO]	= {"INFO",  {"", ""}},
	[LOG_WARN]	= {"WARN",  {"\033[33m", "\033[0m"}},
	[LOG_ERROR]	= {"ERROR", {"\033[31m", "\033[0m"}},
	[LOG_FATAL]	= {"FATAL", {"\033[31m", "\033[0m"}},
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

static inline FILE *__set_log_file(FILE *f)
{
	FILE *ret = log_file;

	log_file = f;
	return ret;
}

FILE *set_log_file(FILE *f)
{
	if (log_file)
		fflush(log_file);
	return __set_log_file(f ?: stderr);
}

void write_log(const char *file, int line, int level, const char *fmt, ...)
{
	if (!log_file)
		__set_log_file(stderr);

	if (level >= log_level) {
		int coloring = (log_file == stderr || log_file == stdout);
		va_list va;

		va_start(va, fmt);
		fprintf(log_file, "%s%s #%d %s:%d [%s] ",
			(coloring ? levels[level].color[0] : ""),
			str_time, (int)pid, file, line, levels[level].name);
		vfprintf(log_file, fmt, va);
		fprintf(log_file, "%s\n", (coloring ? levels[level].color[1] : ""));
		va_end(va);
	}

	if (level == LOG_FATAL) {
		fflush(log_file);
		exit(1);
	}
}
