/**
 * File: utils.h
 * Author: ZhuXindi
 * Date: 2014-04-22
 */

#ifndef _UTILS_H
#define _UTILS_H

#include <stddef.h>

#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)

#define ARRAY_SIZE(x)	(sizeof(x) / sizeof(*(x)))

#define container_of(ptr, type, member) ({			\
	const typeof(((type *)0)->member) *__ptr = (ptr);	\
	(type *)((char *)__ptr - offsetof(type, member));	\
})

#define min(x, y) ({						\
	typeof(x) __x = (x);					\
	typeof(y) __y = (y);					\
	(void) (&__x == &__y);					\
	__x < __y ? __x : __y;					\
})

#define max(x, y) ({						\
	typeof(x) __x = (x);					\
	typeof(y) __y = (y);					\
	(void) (&__x == &__y);					\
	__x > __y ? __x : __y;					\
})

#endif /* _UTILS_H */
