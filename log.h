/**
 * File: log.h
 * Author: ZhuXindi
 * Date: 2013-07-11
 */

#ifndef _LOG_H
#define _LOG_H

#include <stdlib.h>

enum {
	LOG_DEBUG = 0,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
	LOG_DIE,
};

extern time_t current_time;
extern struct timeval time_of_day;

int set_log_level(int min_level);

void update_sys_time(void);

void write_log(const char *file, int line, int level, const char *fmt, ...);

#define log_debug(fmt, ...) \
	write_log(__FILE__, __LINE__, LOG_DEBUG, fmt, ##__VA_ARGS__)

#define log_info(fmt, ...) \
	write_log(__FILE__, __LINE__, LOG_INFO, fmt, ##__VA_ARGS__)

#define log_warn(fmt, ...) \
	write_log(__FILE__, __LINE__, LOG_WARN, fmt, ##__VA_ARGS__)

#define log_error(fmt, ...) \
	write_log(__FILE__, __LINE__, LOG_ERROR, fmt, ##__VA_ARGS__)

#define log_die(fmt, ...) \
	write_log(__FILE__, __LINE__, LOG_DIE, fmt, ##__VA_ARGS__)

#endif /* _LOG_H */
