/**
 * File: log.h
 * Author: ZhuXindi
 * Date: 2013-07-11
 */

#ifndef _LOG_H
#define _LOG_H

enum {
	LOG_DEBUG = 0,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
	LOG_FATAL,
};

/* get/set log level, default is LOG_WARN */
int get_log_level(void);
int set_log_level(int min_level);

/* get/set log file, default is STDERR_FILENO */
int get_log_file(void);
int set_log_file(int fd);

void write_log(const char *file, int line, int level, const char *fmt, ...);

#define log_debug(fmt, ...) \
	write_log(__FILE__, __LINE__, LOG_DEBUG, fmt, ##__VA_ARGS__)

#define log_info(fmt, ...) \
	write_log(__FILE__, __LINE__, LOG_INFO, fmt, ##__VA_ARGS__)

#define log_warn(fmt, ...) \
	write_log(__FILE__, __LINE__, LOG_WARN, fmt, ##__VA_ARGS__)

#define log_error(fmt, ...) \
	write_log(__FILE__, __LINE__, LOG_ERROR, fmt, ##__VA_ARGS__)

#define log_fatal(fmt, ...) \
	write_log(__FILE__, __LINE__, LOG_FATAL, fmt, ##__VA_ARGS__)

#endif /* _LOG_H */
