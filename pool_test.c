/**
 * File: pool_test.c
 * Author: ZhuXindi
 * Date: 2013-07-11
 */

#include <pool.h>
#include <trunklist.h>
#include <log.h>
#include <systime.h>

int main()
{
	struct trunklist *pool;
	void *ptr;

	update_sys_time();
	set_log_level(LOG_DEBUG);

	pool = trunklist_create(4);

	ptr = trunklist_alloc(pool);

	trunklist_destroy(pool);

	trunklist_free(pool, ptr);

	trunklist_destroy(pool);
	return 0;
}
