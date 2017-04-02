/**
 * File: buffer.c
 * Author: ZhuXindi
 * Date: 2014-04-22
 */

#include <buffer.h>
#include <log.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

struct buffer *buffer_alloc(size_t bufsize, struct pool *pool)
{
	struct buffer *b;

	/* allocate the structure's space and bufsize */
	if (!pool || pool_size(pool) < bufsize + sizeof(struct buffer)) {
		b = malloc(bufsize + sizeof(struct buffer));
		if (!b) {
			log_error("malloc() error: %s", strerror(errno));
			return NULL;
		}
		b->pool = NULL;
	} else {
		b = pool_alloc(pool);
		if (!b) {
			log_error("alloc buffer from pool %p failed", pool);
			return NULL;
		}
		b->pool = pool;
	}

	/* main buffer's parent set to itself */
	b->parent = b;
	b->refcnt = 1;
	b->head = b->data = b->tail = (char *)(b+1);
	b->end = b->head + bufsize;
	INIT_LIST_HEAD(&b->list);

	log_debug("alloc buffer %p size %lu", b, bufsize);
	return b;
}

struct buffer *buffer_separate(struct buffer *b, size_t n, struct pool *pool)
{
	struct buffer *child;

	if (n == 0)
		return b;

	if (n >= (size_t)(b->tail - b->data)) {
		log_error("%lu is out of buffer %p size %lu",
			  n, b, b->tail - b->data);
		return NULL;
	}

	/* only allocate the structure's space */
	if (!pool || pool_size(pool) < sizeof(struct buffer)) {
		child = malloc(sizeof(struct buffer));
		if (!child) {
			log_error("malloc() error: %s", strerror(errno));
			return NULL;
		}
		child->pool = NULL;
	} else {
		child = pool_alloc(pool);
		if (!child) {
			log_error("alloc buffer from pool %p failed", pool);
			return NULL;
		}
		child->pool = pool;
	}

	/* set to original parent, maybe b is a child too */
	child->parent = b->parent;
	child->parent->refcnt++;
	child->refcnt = 1;

	/* adjust pointers */
	child->data = b->data + n;
	child->head = child->data;
	child->tail = b->tail;
	child->end = b->end;
	b->tail = child->data;
	b->end = b->tail;

	/* link child after b */
	INIT_LIST_HEAD(&child->list);
	list_add(&child->list, &b->list);

	log_debug("separate buffer %p from %p at %lu parent %p",
		  child, b, n, child->parent);
	return child;
}

static inline void __buffer_free(struct buffer *b)
{
	/* unlink from chain */
	list_del_init(&b->list);

	/* have child buffer ? */
	if (--b->refcnt == 0) {
		log_debug("free buffer %p", b);
		if (b->pool)
			pool_free(b->pool, b);
		else
			free(b);
	}
}

void buffer_free(struct buffer *b)
{
	/* child buffer */
	if (b->parent != b)
		__buffer_free(b->parent);

	__buffer_free(b);
}
