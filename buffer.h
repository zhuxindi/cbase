/**
 * File: buffer.h
 * Author: ZhuXindi
 * Date: 2014-04-22
 */

#ifndef _BUFFER_H
#define _BUFFER_H

#include <list.h>
#include <pool.h>
#include <stdlib.h>

struct buffer {
	/* head  data  tail   end
	 *   |-----|-----|-----|
	 */
	char *head, *data, *tail, *end;	/* position pointers */
	struct pool *pool;	/* relatived memory pool */
	struct buffer *parent;	/* if separated from another buffer */
	unsigned int refcnt;	/* ref counter */
	struct list_head list;	/* join with other buffers into a chain */
};

/* allocate a buffer contains bufsize bytes at most */
struct buffer *buffer_alloc(size_t bufsize, struct pool *pool);

/* separate a buffer into two parts at postion n, return the tail part */
struct buffer *buffer_separate(struct buffer *b, size_t n, struct pool *pool);

/* free a single buffer */
void buffer_free(struct buffer *b);

/* free all buffers in the chain */
static inline void buffer_free_chain(struct list_head *chain)
{
	struct buffer *b, *n;

	list_for_each_entry_safe(b, n, chain, list)
		buffer_free(b);
}

#endif /* _BUFFER_H */
