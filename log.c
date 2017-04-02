/**
 * File: log.c
 * Author: ZhuXindi
 * Date: 2013-07-11
 */

#include <log.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

static int log_level = LOG_WARN;
static const char *str_levels[] = {
	[LOG_DEBUG]	= "DEBUG",
	[LOG_INFO]	= "INFO",
	[LOG_WARN]	= "WARN",
	[LOG_ERROR]	= "ERROR",
	[LOG_DIE]	= "DIE",
};

static char str_time[] = "2017-01-25 23:41:36";
static pid_t pid;

time_t current_time;
struct timeval time_of_day;

int set_log_level(int min_level)
{
	int ret = log_level;

	log_level = min_level;
	if (log_level < LOG_DEBUG)
		log_level = LOG_DEBUG;
	else if (log_level > LOG_DIE)
		log_level = LOG_DIE;
	return ret;
}

void update_sys_time(void)
{
	pid = getpid();
	current_time = time(NULL);
	gettimeofday(&time_of_day, NULL);
	strftime(str_time, sizeof(str_time), "%F %T", localtime(&current_time));
}

void write_log(const char *file, int line, int level, const char *fmt, ...)
{
	if (level >= log_level) {
		va_list va;
		va_start(va, fmt);
		printf("%s #%d %s:%d [%s] ", str_time, (int)pid, file, line, str_levels[level]);
		vprintf(fmt, va);
		printf("\n");
		va_end(va);
	}

	if (level == LOG_DIE) {
		fflush(stdout);
		_exit(1);
	}
}
