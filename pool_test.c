/**
 * File: pool_test.c
 * Author: ZhuXindi
 * Date: 2013-07-11
 */

#include <pool.h>

int main()
{
	struct pool *pool;
	void *ptr;

	pool = pool_create(4);

	ptr = pool_alloc(pool);

	pool_destroy(pool);

	pool_free(pool, ptr);

	pool_destroy(pool);
	return 0;
}
