/**
 * File: pool.c
 * Author: ZhuXindi
 * Date: 2013-07-11
 */

#include <pool.h>
#include <log.h>
#include <errno.h>
#include <string.h>

static LIST_HEAD(pools);

void pool_flush(struct pool *pool)
{
	void *next = pool->free_list;

	log_debug("flush pool %p", pool);

	/* free all trunks in free list */
	while (next) {
		void *temp = next;
		log_debug("free trunk %p @pool %p", temp, pool);
		next = *(void **)temp;
		pool->allocated--;
		free(temp);
	}

	pool->free_list = next;
}

struct pool *pool_destroy(struct pool *pool)
{
	if (pool) {
		/* free all unused trunks */
		pool_flush(pool);

		/* pool has trunks in use */
		if (pool->used) {
			log_warn("pool %p is still in use, can't be destroyed", pool);
			return pool;
		}

		log_debug("destroy pool %p", pool);

		list_del(&pool->list);
		free(pool);
	}
	return NULL;
}

void pool_flush_all(void)
{
	struct pool *pool;

	list_for_each_entry(pool, &pools, list)
		pool_flush(pool);
}

static inline void *__pool_alloc(struct pool *pool)
{
	void *ptr;

	/* check limit */
	if (pool->limit && pool->allocated >= pool->limit) {
		log_error("pool %p exceeds the limit %u", pool, pool->limit);
		return NULL;
	}

	/* alloc trunk size */
	ptr = malloc(pool->size);
	if (!ptr) {
		/* flush all pools then retry */
		pool_flush_all();
		ptr = malloc(pool->size);
		if (!ptr) {
			log_error("malloc() error: %s", strerror(errno));
			return NULL;
		}
	}

	pool->allocated++;
	pool->used++;
	log_debug("alloc trunk %p @pool %p", ptr, pool);
	return ptr;
}

void *pool_alloc(struct pool *pool)
{
	void *ptr = pool->free_list;

	if (!ptr)
		return __pool_alloc(pool);

	/* reuse a trunk allocated before */
	pool->free_list = *(void **)pool->free_list;
	pool->used++;
	log_debug("reuse trunk %p @pool %p", ptr, pool);
	return ptr;
}

static void *pool_alloc_mem(struct pool *pool, size_t n)
{
	if (n > pool_size(pool)) {
		log_error("can't alloc %lu bytes from pool %p", (long)n, pool);
		return NULL;
	}

	return pool_alloc(pool);
}

void pool_free(struct pool *pool, void *ptr)
{
	/* link to free list */
	if (ptr) {
		log_debug("recycle trunk %p @pool %p", ptr, pool);
		*(void **)ptr = (void *)pool->free_list;
		pool->free_list = (void *)ptr;
		pool->used--;
	}
}

struct pool *pool_create(size_t size)
{
	const size_t align = sizeof(void *);
	struct pool *pool;

	/* alloc pool struct */
	pool = calloc(1, sizeof(*pool));
	if (!pool) {
		log_error("calloc() error: %s", strerror(errno));
		return NULL;
	}

	pool->base.ops.destroy = (struct pool_base *(*)(struct pool_base *))pool_destroy;
	pool->base.ops.alloc = (void *(*)(struct pool_base *, size_t))pool_alloc_mem;
	pool->base.ops.free = (void (*)(struct pool_base *, void *))pool_free;
	pool->base.ops.flush = (void (*)(struct pool_base *))pool_flush;

	/* we need to write a next pointer in each trunk after pool_free,
	 * so we need to adjust the size up to a multiple of align
	 */
	pool->size = ((size + align - 1) & -align);
	list_add_tail(&pool->list, &pools);

	log_debug("create pool %p size %lu", pool, size);
	return pool;
}
