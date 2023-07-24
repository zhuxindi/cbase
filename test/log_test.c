/**
 * File: log_test.c
 * Author: ZhuXindi
 * Date: 2023-06-28
 */

#include <cbase/systime.h>
#include <cbase/log.h>
#include <assert.h>
#include <fcntl.h>

int main()
{
	int fd;

	update_pid();
	update_sys_time();
	set_log_level(LOG_DEBUG);
	assert(get_log_level() == LOG_DEBUG);

	fd = open("/tmp/cbase.log", O_WRONLY|O_CREAT|O_TRUNC, 0644);
	set_log_file(fd);
	assert(get_log_file() == fd);
	log_debug("log_debug");
	log_info("log_info");
	log_warn("log_warn");
	log_error("log_error");
	close(fd);

	set_log_file(-1);
	assert(get_log_file() == STDERR_FILENO);
	log_debug("log_debug");
	log_info("log_info");
	log_warn("log_warn");
	log_error("log_error");
	log_fatal("log_fatal");

	return 0;
}
