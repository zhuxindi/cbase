/**
 * File: pool.c
 * Author: ZhuXindi
 * Date: 2013-07-11
 */

#include <pool.h>

static void *dummy_alloc(struct pool *pool, size_t n)
{
	return malloc(n);
}

static void dummy_free(struct pool *pool, void *ptr)
{
	free(ptr);
}

struct pool dummy_pool = {
	.ops = {
		.alloc = dummy_alloc,
		.free = dummy_free,
	}
};
