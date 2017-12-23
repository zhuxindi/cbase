/**
 * File: utils.c
 * Author: ZhuXindi
 * Date: 2017-12-22
 */

#include <utils.h>
#include <stdlib.h>

static void *mem_alloc_func(size_t size, void *unused)
{
	return malloc(size);
}

static void mem_free_func(void *ptr, void *unused)
{
	free(ptr);
}

struct memops memops = {
	.alloc = mem_alloc_func,
	.free = mem_free_func,
};
