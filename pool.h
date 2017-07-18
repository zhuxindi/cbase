/**
 * File: pool.h
 * Author: ZhuXindi
 * Date: 2013-07-11
 */

#ifndef _POOL_H
#define _POOL_H

#include <stdlib.h>
#include <list.h>

struct pool_base;

struct pool_ops {
	struct pool_base *(*destroy)(struct pool_base *pool);
	void *(*alloc)(struct pool_base *pool, size_t n);
	void (*free)(struct pool_base *pool, void *ptr);
	void (*flush)(struct pool_base *pool);
};

struct pool_base {
	struct pool_ops ops;
};

struct pool {
	struct pool_base base;
	void **free_list;
	unsigned int used;	/* how many chunks are currently in use */
	unsigned int allocated;	/* how many chunks have been allocated */
	unsigned int limit;	/* hard limit on the number of chunks */
	size_t size;		/* chunk size */
	struct list_head list;
};

/* create a new pool which trunks in it have size bytes */
struct pool *pool_create(size_t size);

/* destroy the pool, if any trunk in use then failed
   and return itself or NULL on succeed */
struct pool *pool_destroy(struct pool *pool);

/* flush the pool, any unused trunks will be freed */
void pool_flush(struct pool *pool);

/* flush all pools */
void pool_flush_all(void);

/* alloc a trunk from the pool */
void *pool_alloc(struct pool *pool);

/* free a trunk */
void pool_free(struct pool *pool, void *ptr);

/* get trunk size */
static inline size_t pool_size(const struct pool *pool)
{
	return pool->size;
}

#endif /* _POOL_H */
