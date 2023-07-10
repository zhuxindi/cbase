/**
 * File: pool.h
 * Author: ZhuXindi
 * Date: 2017-07-19
 */

#ifndef _POOL_H
#define _POOL_H

#include <cbase/list.h>

struct pool {
	void **free_list;
	unsigned int refcnt;	/* reference count */
	unsigned int allocated;	/* how many chunks have been allocated */
	unsigned int limit;	/* hard limit on the number of chunks */
	size_t size;		/* chunk size */
	struct list list;
};

/* create a new pool which chunks in it have size bytes */
struct pool *pool_create(size_t size);

/* if no chunk in use then destroy the pool right away and return NULL,
   or return the pool and destroy it after last chunk freed */
struct pool *pool_destroy(struct pool *pool);

/* flush the pool, any unused trunks will be freed */
void pool_flush(struct pool *pool);

/* flush all pools */
void pool_flush_all(void);

/* alloc a chunk from the pool */
void *pool_alloc(struct pool *pool);

/* free a chunk */
void pool_free(struct pool *pool, void *ptr);

/* get chunk size */
static inline size_t pool_size(const struct pool *pool)
{
	return pool->size;
}

#endif /* _POOL_H */
