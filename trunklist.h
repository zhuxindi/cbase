/**
 * File: trunklist.h
 * Author: ZhuXindi
 * Date: 2017-07-19
 */

#ifndef _TRUNKLIST_H
#define _TRUNKLIST_H

#include <list.h>
#include <pool.h>

struct trunklist {
	struct pool base;
	void **free_list;
	unsigned int used;	/* how many chunks are currently in use */
	unsigned int allocated;	/* how many chunks have been allocated */
	unsigned int limit;	/* hard limit on the number of chunks */
	size_t size;		/* chunk size */
	struct list_head list;
};

/* create a new pool which trunks in it have size bytes */
struct trunklist *trunklist_create(size_t size);

/* destroy the pool, if any trunk in use then failed
   and return itself or NULL on succeed */
struct trunklist *trunklist_destroy(struct trunklist *pool);

/* flush the pool, any unused trunks will be freed */
void trunklist_flush(struct trunklist *pool);

/* flush all pools */
void trunklist_flush_all(void);

/* alloc a trunk from the pool */
void *trunklist_alloc(struct trunklist *pool);

/* free a trunk */
void trunklist_free(struct trunklist *pool, void *ptr);

/* get trunk size */
static inline size_t trunklist_size(const struct trunklist *pool)
{
	return pool->size;
}

#endif /* _TRUNKLIST_H */
