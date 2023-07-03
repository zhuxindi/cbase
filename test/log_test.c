/**
 * File: log_test.c
 * Author: ZhuXindi
 * Date: 2023-06-28
 */

#include <cbase/systime.h>
#include <cbase/log.h>

int main()
{
	FILE *f;

	update_pid();
	update_sys_time();
	set_log_level(LOG_DEBUG);

	f = fopen("/tmp/cbase.log", "w");
	set_log_file(f);
	log_debug("log_debug");
	log_info("log_info");
	log_warn("log_warn");
	log_error("log_error");
	fclose(f);

	set_log_file(NULL);
	log_debug("log_debug");
	log_info("log_info");
	log_warn("log_warn");
	log_error("log_error");
	log_fatal("log_fatal");

	return 0;
}
