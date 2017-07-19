/**
 * File: pool.h
 * Author: ZhuXindi
 * Date: 2013-07-11
 */

#ifndef _POOL_H
#define _POOL_H

#include <stdlib.h>

struct pool;

struct pool_ops {
	void *(*alloc)(struct pool *pool, size_t n);
	void (*free)(struct pool *pool, void *ptr);
};

struct pool {
	struct pool_ops ops;
};

extern struct pool dummy_pool;

#endif /* _POOL_H */
