/**
 * File: utils.h
 * Author: ZhuXindi
 * Date: 2014-04-22
 */

#ifndef _UTILS_H
#define _UTILS_H

#include <stddef.h>

struct memops {
	void *(*alloc)(size_t, void *);
	void (*free)(void *, void *);
	void *user;
};

extern struct memops memops;

static inline void *mem_alloc(struct memops *ops, size_t size)
{
	return ops->alloc(size, ops->user);
}

static inline void mem_free(struct memops *ops, void *ptr)
{
	ops->free(ptr, ops->user);
}

#define container_of(ptr, type, member) ({			\
	const typeof(((type *)0)->member) *__ptr = (ptr);	\
	(type *)((char *)__ptr - offsetof(type, member));	\
})

#define min(a, b) ((a) < (b) ? (a) : (b))

#define max(a, b) ((a) > (b) ? (a) : (b))

#endif /* _UTILS_H */
