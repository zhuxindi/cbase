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

struct buffer *buffer_create(size_t bufsize, struct memops *ops)
{
	struct buffer *b;

	/* allocate the structure's space and bufsize */
	b = ops->alloc(bufsize + sizeof(struct buffer), ops->user);
	if (!b) {
		log_error("alloc buffer space failed");
		return NULL;
	}

	/* main buffer's parent set to itself */
	b->parent = b;
	b->refcnt = 1;
	b->head = b->data = b->tail = (char *)(b+1);
	b->end = b->head + bufsize;
	b->ops = ops;
	INIT_LIST_HEAD(&b->list);

	log_debug("alloc buffer %p size %lu", b, bufsize);
	return b;
}

struct buffer *buffer_separate(struct buffer *b, size_t n)
{
	struct buffer *child;

	if (n >= (size_t)(b->tail - b->data)) {
		log_error("%lu is out of buffer %p size %lu",
			  n, b, b->tail - b->data);
		return NULL;
	}

	/* only allocate the structure's space */
	child = b->ops->alloc(sizeof(struct buffer), b->ops->user);
	if (!child) {
		log_error("alloc buffer space failed");
		return NULL;
	}

	/* set to original parent, maybe b is a child too */
	child->parent = b->parent;
	child->parent->refcnt++;
	child->refcnt = 1;
	child->ops = b->ops;

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

static inline void __buffer_release(struct buffer *b)
{
	/* unlink from chain */
	list_del_init(&b->list);

	/* have child buffer ? */
	if (--b->refcnt == 0) {
		log_debug("free buffer %p", b);
		b->ops->free(b, b->ops->user);
	}
}

void buffer_release(struct buffer *b)
{
	/* child buffer */
	if (b->parent != b)
		__buffer_release(b->parent);

	__buffer_release(b);
}
